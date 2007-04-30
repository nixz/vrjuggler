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
#include <gadget/gadgetParam.h>

#include <plugins/RIMPlugin/RIMPlugin.h>

// Sharing Devices
#include <gadget/Type/BaseTypeFactory.h>
#include <gadget/Type/DeviceFactory.h>
#include <gadget/InputManager.h>
#include <gadget/AbstractNetworkManager.h>
//#include <gadget/RemoteInputManager.h>
#include <plugins/RIMPlugin/DeviceServer.h>
#include <plugins/RIMPlugin/VirtualDevice.h>

// ClusterNetwork
#include <cluster/ClusterNetwork.h>
#include <gadget/Node.h>
#include <cluster/ClusterManager.h>

// IO Packets
#include <cluster/Packets/PacketFactory.h>
#include <cluster/Packets/ConnectionRequest.h>
#include <cluster/Packets/DeviceRequest.h>
#include <cluster/Packets/DeviceAck.h>
#include <cluster/Packets/ApplicationDataRequest.h>
#include <cluster/Packets/EndBlock.h>
#include <cluster/Packets/DataPacket.h>

// Configuration
#include <jccl/RTRC/ConfigManager.h>
#include <jccl/Config/ConfigElement.h>

#include <map>

extern "C"
{
   GADGET_CLUSTER_PLUGIN_EXPORT(vpr::Uint32) getGadgeteerVersion()
   {
      return __GADGET_version;
   }

   GADGET_CLUSTER_PLUGIN_EXPORT(void) initPlugin(cluster::ClusterManager* mgr)
   {
      mgr->addPlugin(new cluster::RIMPlugin());
   }
}

namespace cluster
{
   RIMPlugin::RIMPlugin()
      : mHandlerGUID("9c3fb301-b142-4c6f-8ca3-1570898974d0")
      //, mRIM(mHandlerGUID)
   {;}

   RIMPlugin::~RIMPlugin()
   {
      for ( std::map<vpr::GUID, VirtualDevice*>::iterator j = mVirtualDevices.begin(); j != mVirtualDevices.end(); j++ )
      {
         if ( (*j).second != NULL )
         {
            delete (*j).second;
         }
      }
      for ( std::vector<DeviceServer*>::iterator j = mDeviceServers.begin(); j != mDeviceServers.end(); j++ )
      {
         if ( (*j) != NULL )
         {
            delete (*j);
         }
      }
   }

   void RIMPlugin::preDraw()
   {
      // Do nothing we are only here to sync.
   }

   void RIMPlugin::postPostFrame()
   {
      sendDataAndSync();
   }

   void RIMPlugin::sendRequests()
   {
      sendDeviceRequests();
   }

   //////////////////////////
   //    CONFIG METHODS    //
   //////////////////////////

   /** Add the pending element to the configuration.
    *  @pre configCanHandle (element) == true.
    *  @return true iff element was successfully added to configuration.
    */
   bool RIMPlugin::configAdd(jccl::ConfigElementPtr element)
   {
      vprASSERT(ClusterManager::instance()->recognizeRemoteDeviceConfig(element));
      vprASSERT(ClusterManager::instance()->isClusterActive());

      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_STATUS_LVL)
         << clrOutBOLD(clrCYAN,"[RIMPlugin] ")
         << "Adding device: " << element->getName()
         << std::endl << vprDEBUG_FLUSH;

      return addDevice(element);
   }


   /** Remove the pending element from the current configuration.
    *  @pre configCanHandle (element) == true.
    *  @return true iff the element (and any objects it represented)
    *          were successfully removed.
    */
   bool RIMPlugin::configRemove(jccl::ConfigElementPtr element)
   {
      boost::ignore_unused_variable_warning(element);
      return false;

      // XXX: We may still use this to handle the configuration 
      //      of clustered RIM connections.
      /*
      if ( ClusterManager::instance()->recognizeRemoteDeviceConfig(element) )
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutBOLD(clrCYAN,"[RIMPlugin] ")
         << "Removing the Remote Device: " << element->getName()
         << " from the active configuration \n" << vprDEBUG_FLUSH;

         removeVirtualDevice(element->getName());
         if ( this->mVirtualDevices.size()== 0 && mDeviceServers.size() == 0 )
         {
            setActive(false);
         }
         return(true);
      }
      else
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
            << "[RIMPlugin::configRemove] ERROR, Something is seriously wrong, we should never get here\n"
            << vprDEBUG_FLUSH;
         return(false);
      }
      */
   }


   /** Checks if this handler can process element.
    *  Typically, an implementation of handler will check the element's
    *  description name/token to decide if it knows how to deal with
    *  it.
    *  @return true iff this handler can process element.
    */
   bool RIMPlugin::configCanHandle(jccl::ConfigElementPtr element)
   {
      // XXX: We may still use this to handle the configuration 
      //      of clustered RIM connections.
      return ClusterManager::instance()->recognizeRemoteDeviceConfig(element);
   }

   bool RIMPlugin::isPluginReady()
   {
      //XXX: Fix this to check for any pending devices. We might want to keep a local
      //     copy of this also so that we don't stall when we are waiting for pending
      //     RIM requests that are not in the cluster configuration.

      //vpr::Guard<vpr::Mutex> guard(mPendingDeviceRequestsLock);
      //return(0 == mPendingDeviceRequests.size());
      return true;
   }

   void RIMPlugin::recoverFromLostNode(gadget::Node* lost_node)
   {
      removeVirtualDevicesOnHost(lost_node->getHostname());
      removeDeviceClientsForHost(lost_node->getHostname());

      // Since we have lost a connection we need to set a flag so
      // that when we gain a new connection we will reconfigure.
      //mReconfigurationNeededOnConnection = true;
   }

   bool RIMPlugin::addDevice(jccl::ConfigElementPtr elm)
   {
      vprASSERT(cluster::ClusterManager::instance()->isClusterActive() && "RIM called in non-cluster mode.")
      bool master = cluster::ClusterManager::instance()->isMaster();
      bool result(false);

      std::string device_name = elm->getName();
      // If we are the master, configure the device and tell all slaves to prepare
      // virtual devices.
      if (master)
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_STATUS_LVL)
            << clrOutBOLD(clrMAGENTA, "[RemoteInputManager]")
            << "Configuring device on master node: " << device_name
            << std::endl << vprDEBUG_FLUSH;

         gadget::InputManager::instance()->configureDevice(elm);
         gadget::Input* input_device = gadget::InputManager::instance()->getDevice(device_name);
         if ( input_device != NULL )
         {
            result = addDeviceServer(device_name, input_device);
            DeviceServer* device_server = getDeviceServer(device_name);
            vprASSERT(NULL != device_server && "Must have device server.");
   
            std::string temp_string = input_device->getInputTypeName();
            vpr::GUID   temp_guid   = device_server->getId();
            cluster::DeviceAck* device_ack =
               new cluster::DeviceAck(mHandlerGUID, temp_guid,
                                      device_name, temp_string, true);

            gadget::AbstractNetworkManager::node_list_t nodes = cluster::ClusterManager::instance()->getNetwork()->getNodes();
            std::cout << "XXXX: NUM NODES: " << nodes.size() << std::endl;
            for (gadget::AbstractNetworkManager::node_list_t::iterator itr = nodes.begin(); itr != nodes.end(); itr++)
            {
               vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_STATUS_LVL)
                  << clrOutBOLD(clrMAGENTA, "[RemoteInputManager]")
                  << "Sending device ack [" << device_name << "] to [" << (*itr)->getName() << "]"
                  << std::endl << vprDEBUG_FLUSH;

               device_server->addClient(*itr);

               // Create a responce ACK
               (*itr)->send(device_ack);
            }
         }
      }
      else
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_STATUS_LVL)
            << clrOutBOLD(clrMAGENTA, "[RemoteInputManager]")
            << "Configuring device on slave node: " << device_name
            << std::endl << vprDEBUG_FLUSH;

         result = true;
         /*
         addVirtualDevice(temp_device_ack->getId(), device_name,
                          temp_device_ack->getDeviceBaseType(),
                          temp_device_ack->getHostname());

         // Add this virtual device to the InputManager's list of devices.
         gadget::InputManager::instance()->addRemoteDevice(getVirtualDevice(device_name), device_name);
         */
      }
      return result;
   }

   /**
    * Handle a incoming packet.
    */
   void RIMPlugin::handlePacket(cluster::Packet* packet, gadget::Node* node)
   {
      //We are only handling data packets right now.
      if ( NULL != packet && NULL != node )
      {
         switch ( packet->getPacketType() )
         {
         //case cluster::Header::RIM_DEVICE_REQ:
         //   {
         //      cluster::DeviceRequest* temp_device_request = dynamic_cast<cluster::DeviceRequest*>(packet);
         //      vprASSERT(NULL != temp_device_request && "Dynamic cast failed!");
         //      std::string device_name = temp_device_request->getDeviceName();

         //      if ( jccl::ConfigManager::instance()->isPendingStale() )
         //      {
         //         gadget::Input* temp_input_device = gadget::InputManager::instance()->getDevice(device_name);
         //         if ( temp_input_device != NULL )
         //         {
         //            DeviceServer* temp_device_server = getDeviceServer(device_name);
         //            if ( NULL == temp_device_server )
         //            {
         //               addDeviceServer(device_name, temp_input_device);
         //               temp_device_server = getDeviceServer(device_name);
         //            }

         //            temp_device_server->addClient(node);

         //            // Create a responce ACK
         //            std::string temp_string = temp_input_device->getInputTypeName();
         //            vpr::GUID   temp_guid   = temp_device_server->getId();
         //            cluster::DeviceAck* temp_ack =
         //               new cluster::DeviceAck(mHandlerGUID, temp_guid,
         //                                      device_name, temp_string, true);
         //            node->send(temp_ack);
         //         }
         //         else
         //         {
         //            std::string temp_string = "";
         //            vpr::GUID empty_id;
         //            cluster::DeviceAck* temp_ack =
         //               new cluster::DeviceAck(mHandlerGUID, empty_id, device_name,
         //                                      temp_string/*BaseType*/, false);
         //            node->send(temp_ack);
         //         }
         //      }
         //      else
         //      {
         //         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
         //         << clrOutBOLD(clrRED,"Pending List is not stale(Config Manager is still configuring the local system) ")
         //         << clrOutBOLD(clrRED,"So we can not process this device request right now.") << std::endl << vprDEBUG_FLUSH;

         //         std::string temp_string = "";
         //         vpr::GUID empty_id;
         //         cluster::DeviceAck* temp_ack =
         //            new cluster::DeviceAck(mHandlerGUID, empty_id, device_name,
         //                                   temp_string/*BaseType*/, false);
         //         node->send(temp_ack);
         //      }
         //      break;
         //   }
         case cluster::Header::RIM_DEVICE_ACK:
            {
               // -If ACK
               //   -Create VirtualDevice
               // -If Nack
               //   -Do nothing(let the config manager worry about re-trying)

               cluster::DeviceAck* temp_device_ack = dynamic_cast<cluster::DeviceAck*>(packet);
               vprASSERT(NULL != temp_device_ack && "Dynamic cast failed!");
               std::string device_name = temp_device_ack->getDeviceName();

               if ( temp_device_ack->getAck() )
               {
                  removePendingDeviceRequest(device_name);

                  if ( getVirtualDevice(device_name) != NULL )
                  {
                     vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutBOLD(clrRED, "ERROR:")
                     << "Somehow we already have a virtual device named: " << device_name << std::endl << vprDEBUG_FLUSH;
                  }
                  else
                  {
                     addVirtualDevice(temp_device_ack->getId(), device_name,
                                      temp_device_ack->getDeviceBaseType(),
                                      temp_device_ack->getHostname());

                     // Add this virtual device to the InputManager's list of devices.
                     gadget::InputManager::instance()->addRemoteDevice(getVirtualDevice(device_name), device_name);
                  }
               }
               else
               {  //XXX: FIX
                  // Do Nothing Since we will just re-try later
                  //createPendingConfigRemoveAndAdd(mDeviceName);
                  //jccl::ConfigManager::instance()->delayStalePendingList();
               }
               break;
            }
         case cluster::Header::RIM_DATA_PACKET:
            {
               cluster::DataPacket* temp_data_packet = dynamic_cast<cluster::DataPacket*>(packet);
               vprASSERT(NULL != temp_data_packet && "Dynamic cast failed!");

               //vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "RIM::handlePacket()..." << std::endl <<  vprDEBUG_FLUSH;
               //temp_data_packet->printData(1);

               gadget::Input* virtual_device = getVirtualDevice(temp_data_packet->getObjectId());
               if ( virtual_device != NULL )
               {
                  vpr::BufferObjectReader* temp_reader = new vpr::BufferObjectReader(temp_data_packet->getDeviceData());

                  temp_reader->setAttrib("rim.timestamp.delta", node->getDelta());
                  virtual_device->readObject(temp_reader);
               }
               break;
            }
         default:
            {
               std::cout << "RIM DOES NOT HANDLE THIS PACKET TYPE" << packet->getPacketType() << std::endl;
               break;
            }
         } // End switch
      } // End if
   }


   bool RIMPlugin::addVirtualDevice(const vpr::GUID& device_id,
                                             const std::string& name,
                                             const std::string& device_base_type,
                                             const std::string& hostname)
   {
      vpr::Guard<vpr::Mutex> guard(mVirtualDevicesLock);

      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
      << clrOutBOLD(clrMAGENTA, "[RemoteInputManager]")
      << "Creating Virtual Device: " << name << std::endl << vprDEBUG_FLUSH;

      gadget::Input* temp_input_device = gadget::BaseTypeFactory::instance()->loadNetDevice(device_base_type);
      VirtualDevice* temp_virtual_device = new VirtualDevice(name, device_id, device_base_type, hostname, temp_input_device);

      mVirtualDevices[device_id] = temp_virtual_device;

      return true;
   }

   void RIMPlugin::addVirtualDevice(VirtualDevice* device)
   {
      vpr::Guard<vpr::Mutex> guard(mVirtualDevicesLock);

      mVirtualDevices[device->getId()] = device;
   }

   gadget::Input* RIMPlugin::getVirtualDevice(const vpr::GUID& device_id)
   {
      vpr::Guard<vpr::Mutex> guard(mVirtualDevicesLock);

      for ( std::map<vpr::GUID, VirtualDevice*>::iterator i = mVirtualDevices.begin();
          i != mVirtualDevices.end() ; i++ )
      {
         if ( (*i).first == device_id )
         {
            return((*i).second->getDevice());
         }
      }
      return NULL;
   }

   gadget::Input* RIMPlugin::getVirtualDevice(const std::string& device_name)
   {
      vpr::Guard<vpr::Mutex> guard(mVirtualDevicesLock);

      for ( std::map<vpr::GUID, VirtualDevice*>::iterator i = mVirtualDevices.begin();
          i != mVirtualDevices.end() ; i++ )
      {
         if ( (*i).second->getName() == device_name )
         {
            return((*i).second->getDevice());
         }
      }
      return NULL;
   }

   bool RIMPlugin::removeVirtualDevicesOnHost(const std::string& host_name)
   {
      // - Get a list of all remote devices on the given host
      // - Remove them from the current configuration
      vpr::Guard<vpr::Mutex> guard(mVirtualDevicesLock);

      std::vector<std::string> devices_to_remove;
      for ( std::map<vpr::GUID, VirtualDevice*>::iterator i = mVirtualDevices.begin();
          i != mVirtualDevices.end() ; i++ )
      {
         if ( (*i).second->getRemoteHostname() == host_name )
         {
            devices_to_remove.push_back((*i).second->getName());
         }
      }

      for ( std::vector<std::string>::iterator i = devices_to_remove.begin();
          i != devices_to_remove.end();i++ )
      {
         // We could just remove it here, but for the sake of testing RTRC
         // we will create a pending remove
         // removeVirtualDevice(*i);
         createPendingConfigRemoveAndAdd(*i);
      }
      return true;
   }

   bool RIMPlugin::removeDeviceClientsForHost(const std::string& host_name)
   {
      // Loop through all Device Servers and remove any device clients that
      // may exist for the given host
      vpr::Guard<vpr::Mutex> guard(mDeviceServersLock);

      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
         << clrOutBOLD(clrMAGENTA,"[RemoteInputManager]")
         << " Removing client, " << host_name << " from all Device Servers.\n"
         << vprDEBUG_FLUSH;

      for ( std::vector<DeviceServer*>::iterator i = mDeviceServers.begin();
          i != mDeviceServers.end() ; i++ )
      {
         (*i)->removeClient(host_name);
      }
      return true;
   }

   void RIMPlugin::removeVirtualDevice(const vpr::GUID& device_id)
   {
      vpr::Guard<vpr::Mutex> guard(mVirtualDevicesLock);

      for ( std::map<vpr::GUID, VirtualDevice*>::iterator i = mVirtualDevices.begin();
          i != mVirtualDevices.end() ; i++ )
      {
         if ( (*i).first == device_id )
         {
            // Remove remote device from the InputManager
            gadget::InputManager::instance()->removeDevice((*i).second->getName());
            delete (*i).second;
            mVirtualDevices.erase(i);
            return;
         }
      }
   }

   void RIMPlugin::removeVirtualDevice(const std::string& device_name)
   {
      vpr::Guard<vpr::Mutex> guard(mVirtualDevicesLock);

      // Remove remote device from the InputManager
      gadget::InputManager::instance()->removeDevice(device_name);

      for ( std::map<vpr::GUID, VirtualDevice*>::iterator i = mVirtualDevices.begin();
          i != mVirtualDevices.end() ; i++ )
      {
         if ( (*i).second->getName() == device_name )
         {
            (*i).second->debugDump(vprDBG_CONFIG_LVL);
            delete (*i).second;
            mVirtualDevices.erase(i);
            return;
         }
      }
   }

   void RIMPlugin::debugDumpVirtualDevices(int debug_level)
   {
      vpr::Guard<vpr::Mutex> guard(mVirtualDevicesLock);

      vpr::DebugOutputGuard dbg_output(gadgetDBG_RIM,debug_level,
                                       std::string("-------------- Virtual Devices --------------\n"),
                                       std::string("---------------------------------------------\n"));
      for ( std::map<vpr::GUID, VirtualDevice*>::iterator j = mVirtualDevices.begin(); j != mVirtualDevices.end(); j++ )
      {
         (*j).second->debugDump(debug_level);
      }
   }


   // ===================== DEVICE SERVERS =============================

   bool RIMPlugin::addDeviceServer(const std::string& name,
                                            gadget::Input* device)
   {
      vpr::Guard<vpr::Mutex> guard(mDeviceServersLock);

      DeviceServer* temp_device_server =
         new DeviceServer(name, device, mHandlerGUID);
      mDeviceServers.push_back(temp_device_server);

      return true;
   }

   void RIMPlugin::addDeviceServer(DeviceServer* device)
   {
      vpr::Guard<vpr::Mutex> guard(mDeviceServersLock);
      mDeviceServers.push_back(device);
   }

   DeviceServer* RIMPlugin::getDeviceServer(const std::string& device_name)
   {
      vpr::Guard<vpr::Mutex> guard(mDeviceServersLock);

      for ( std::vector<DeviceServer*>::iterator i = mDeviceServers.begin();
          i != mDeviceServers.end() ; i++ )
      {
         if ( (*i)->getName() == device_name )
         {
            return(*i);
         }
      }
      return NULL;
   }

   void RIMPlugin::removeDeviceServer(const std::string& device_name)
   {
      vpr::Guard<vpr::Mutex> guard(mDeviceServersLock);

      for ( std::vector<DeviceServer*>::iterator i = mDeviceServers.begin();
          i != mDeviceServers.end() ; i++ )
      {
         if ( (*i)->getName() == device_name )
         {
            delete (*i);
            mDeviceServers.erase(i);
            return;
         }
      }
   }

   void RIMPlugin::debugDumpDeviceServers(int debug_level)
   {
      vpr::Guard<vpr::Mutex> guard(mDeviceServersLock);

      vpr::DebugOutputGuard dbg_output(gadgetDBG_RIM,debug_level,
                                       std::string("-------------- Device Servers --------------\n"),
                                       std::string("---------------------------------------------\n"));
      for ( std::vector<DeviceServer*>::iterator j = mDeviceServers.begin(); j != mDeviceServers.end(); j++ )
      {
         (*j)->debugDump(debug_level);
      }
   }

   void RIMPlugin::sendDataAndSync()
   {
      //      vpr::Interval first;
      //      vpr::Interval second;

      //      std::cout << "Number Device Servers: " << mDeviceServers.size() << " Number Virtual Devices" << mVirtualDevices.size() << std::endl;

      vpr::Guard<vpr::Mutex> guard(mDeviceServersLock);

      // Update all local device servers and send their data.
      for ( unsigned int i=0; i<mDeviceServers.size(); i++ )
      {
         mDeviceServers[i]->updateLocalData();
         mDeviceServers[i]->send();
      }

      //      second.setNow();
      //      vpr::Interval diff_time4(second-first);
      //      std::cout << "Recv DeviceData Time: " << diff_time4.getBaseVal() << std::endl;

   }

   vpr::Uint16 RIMPlugin::getNumberPendingDeviceRequests()
   {
      return 0;
      /*
      vpr::Guard<vpr::Mutex> guard(mPendingDeviceRequestsLock);
      return(mPendingDeviceRequests.size());
      */
   }

   bool RIMPlugin::createPendingConfigRemove(std::string device_name)
   {
      jccl::ConfigManager* cfg_mgr = jccl::ConfigManager::instance();

      cfg_mgr->lockActive();
      std::vector<jccl::ConfigElementPtr>::iterator active_begin = cfg_mgr->getActiveBegin();
      std::vector<jccl::ConfigElementPtr>::iterator active_end   = cfg_mgr->getActiveEnd();
      std::vector<jccl::ConfigElementPtr>::iterator i;

      // Find the active device that we want to remove
      for ( i = active_begin ; i != active_end ; i++ )
      {
         if ( /*recognizeRemoteDeviceConfig(*i) && */(*i)->getName() == device_name )
         {
            cfg_mgr->addConfigElement(*i, jccl::ConfigManager::PendingElement::REMOVE);

            cfg_mgr->unlockActive();
            cfg_mgr->removeActive(device_name);
            cfg_mgr->lockActive();
         }
      }
      cfg_mgr->unlockActive();
      return true;
   }

   bool RIMPlugin::createPendingConfigRemoveAndAdd(std::string device_name)
   {
      jccl::ConfigManager* cfg_mgr = jccl::ConfigManager::instance();

      cfg_mgr->lockActive();
      std::vector<jccl::ConfigElementPtr>::iterator active_begin = cfg_mgr->getActiveBegin();
      std::vector<jccl::ConfigElementPtr>::iterator active_end   = cfg_mgr->getActiveEnd();
      std::vector<jccl::ConfigElementPtr>::iterator i;
      for ( i = active_begin ; i != active_end ; i++ )
      {
         if ( /*recognizeRemoteDeviceConfig(*i) && */(*i)->getName() == device_name )
         {
            cfg_mgr->addConfigElement(*i, jccl::ConfigManager::PendingElement::REMOVE);
            cfg_mgr->addConfigElement(*i, jccl::ConfigManager::PendingElement::ADD);

            cfg_mgr->unlockActive();
            cfg_mgr->removeActive(device_name);
            cfg_mgr->lockActive();
         }
      }
      cfg_mgr->unlockActive();
      return true;
   }

   void RIMPlugin::addPendingDeviceRequest(cluster::DeviceRequest* new_device_req, gadget::Node* node)
   {
      /*
      vpr::Guard<vpr::Mutex> guard(mPendingDeviceRequestsLock);
      mPendingDeviceRequests[new_device_req] = node;
      */
   }

   void RIMPlugin::removePendingDeviceRequest(std::string device_name)
   {
      /*
      vpr::Guard<vpr::Mutex> guard(mPendingDeviceRequestsLock);

      std::map<cluster::DeviceRequest*, Node*>::iterator begin = mPendingDeviceRequests.begin();
      std::map<cluster::DeviceRequest*, Node*>::iterator end = mPendingDeviceRequests.end();
      std::map<cluster::DeviceRequest*, Node*>::iterator i;

      for ( i = begin ; i != end ; i++ )
      {
         if ( (*i).first->getDeviceName() == device_name )
         {
            mPendingDeviceRequests.erase(i);
            return;
         }
      }
      */
   }

   void RIMPlugin::sendDeviceRequests()
   {
      /*
      vpr::Guard<vpr::Mutex> guard(mPendingDeviceRequestsLock);

      std::map<cluster::DeviceRequest*, Node*>::iterator begin = mPendingDeviceRequests.begin();
      std::map<cluster::DeviceRequest*, Node*>::iterator end   = mPendingDeviceRequests.end();
      std::map<cluster::DeviceRequest*, Node*>::iterator i;

      for ( i = begin ; i != end ; i++ )
      {
         if ( (*i).second->isConnected() )
         {
            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
               << clrOutBOLD(clrMAGENTA, "[RemoteInputManager]")
               << " Sending device request for: " << (*i).first->getDeviceName()
               << std::endl << vprDEBUG_FLUSH;

            (*i).second->send((*i).first);
         }
      }
      */
   }
}  // end namespace cluster
