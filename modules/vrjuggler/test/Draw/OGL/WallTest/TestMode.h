
/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _TEST_MODE_H_
#define _TEST_MODE_H_

#include <boost/shared_ptr.hpp>
#include <string>

class TestMode;
typedef boost::shared_ptr<TestMode> TestModePtr;

class WallTest;

/** Base class for all test modes. */
class TestMode
{
public:
   TestMode()
   {;}
   virtual ~TestMode()
   {;}
   virtual std::string getName() = 0;
   virtual void update(WallTest* wallTest) = 0;
   virtual void bufferPreDraw(WallTest* wallTest)
   {;}
   virtual void draw(WallTest* wallTest) = 0;
};

#endif

