/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <cluster/PluginConfig.h>

#include <gadget/Util/Debug.h>

#include <vpr/IO/BufferObjectWriter.h>
#include <cluster/Packets/DataPacket.h>
#include <plugins/ApplicationDataManager/ApplicationData.h>
#include <plugins/ApplicationDataManager/ApplicationDataServer.h>

namespace cluster
{

ApplicationDataServer::ApplicationDataServer(const vpr::GUID& guid,  ApplicationData* userData, const vpr::GUID& pluginGuid)
   : mApplicationData(userData)
   , mDataPacket()
   , mBufferObjectWriter(NULL)
   , mDeviceData(NULL)
{
   // mDataPacket and mBufferObjectWriter both use mDeviceData as their data buffer.
   mDeviceData = new std::vector<vpr::Uint8>;
   
   // Create a DataPacket that will be updated and sent continually.
   mDataPacket = DataPacket::create(pluginGuid, guid, mDeviceData);
   mBufferObjectWriter = new vpr::BufferObjectWriter(mDeviceData);
}

ApplicationDataServerPtr ApplicationDataServer::create(const vpr::GUID& guid,
                                                       ApplicationData* userData,
                                                       const vpr::GUID& pluginGuid)
{
   return ApplicationDataServerPtr(new ApplicationDataServer(guid, userData, pluginGuid));
}

ApplicationDataServer::~ApplicationDataServer()
{
   // User is responsible to clean up mApplicationData
   // mDataPacket will clean up the memory that mDeviceData points
   // to since mDataPacket contains a reference to the ame memory.
   // vpr::BufferObjectWritter does not release mDeviceData
   delete mBufferObjectWriter;
   mDeviceData = NULL;   
}

void ApplicationDataServer::serializeAndSend()
{
   // Clear old data and reset the position of mBufferObjectWriter
   mBufferObjectWriter->getData()->clear();
   mBufferObjectWriter->setCurPos(0);

   // This updates the mApplicationData which both mBufferedObjectReader and mDevicePacket point to
   mApplicationData->writeObject(mBufferObjectWriter);

   // We must update the size of the actual data that we are going to send
   mDataPacket->getHeader()->setPacketLength(Header::RIM_PACKET_HEAD_SIZE 
                                    + 16 /*Plugin GUID*/
                                    + 16 /*Plugin GUID*/
                                    + mDeviceData->size());

   // We must serialize the header again so that we can reset the size.
   mDataPacket->getHeader()->serializeHeader();

   cluster::ClusterManager::instance()->getNetwork()->sendToAll(mDataPacket);
}

void ApplicationDataServer::debugDump(int debug_level)
{
   vpr::DebugOutputGuard dbg_output(gadgetDBG_RIM,debug_level,
                              std::string("-------------- ApplicationDataServer --------------\n"),
                              std::string("------------------------------------------\n"));

   vprDEBUG(gadgetDBG_RIM,debug_level) << "Name:     " << getId().toString() << std::endl << vprDEBUG_FLUSH;
}

vpr::GUID ApplicationDataServer::getId() 
{ 
   return(mApplicationData->getId()); 
}

} // End of cluster namespace
