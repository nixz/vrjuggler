/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2009 by Iowa State University
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

#include "cubeGeometry.h"

// number of verticies
// this can be passed to opengl's glDrawArrays( .., .. );
const unsigned int cubeGeometry::mSize( 36 );

// format is: T2F_C4F_N3F_V3F
const float cubeGeometry::mData[432] = { 0, 0, 1, 0, 0, 1, 0, 0, 1, -1, -1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 1, -1, 1, 1, 0, 0.3f, 0.3f, 0.3f, 1, 0, 0, 1, -1, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 1, -1, 1, 1, 0, 0.3f, 0.3f, 0.3f, 1, 0, 0, 1, -1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, -1, 0, 0, -1, -1, 1, 0, 1, 0.3f, 0.3f, 0.3f, 1, -1, 0, 0, -1, 1, 1, 1, 0, 1, 1, 1, 1, -1, 0, 0, -1, -1, -1, 0, 1, 0.3f, 0.3f, 0.3f, 1, -1, 0, 0, -1, 1, 1, 1, 0, 1, 1, 1, 1, -1, 0, 0, -1, -1, -1, 1, 1, 0.5f, 0, 0.5f, 1, -1, 0, 0, -1, 1, -1, 0, 0, 1, 1, 1, 1, 0, 0, -1, -1, -1, -1, 0, 1, 0.5f, 0, 0.5f, 1, 0, 0, -1, -1, 1, -1, 1, 0, 0, 0.5f, 1.5f, 1, 0, 0, -1, 1, -1, -1, 0, 1, 0.5f, 0, 0.5f, 1, 0, 0, -1, -1, 1, -1, 1, 0, 0, 0.5f, 1.5f, 1, 0, 0, -1, 1, -1, -1, 1, 1, 0.5f, 0.5f, 0, 1, 0, 0, -1, 1, 1, -1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, -1, 1, 0, 1, 0, 0.5, 1.5f, 1, 1, 0, 0, 1, -1, -1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0, 0.5f, 1.5f, 1, 1, 0, 0, 1, -1, -1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0.5f, 0.5f, 0, 1, 1, 0, 0, 1, 1, -1, 0, 0, 0.3f, 0.3f, 0.3f, 1, 0, 1, 0, -1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0.5f, 0, 0.5f, 1, 0, 1, 0, -1, 1, -1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0.5f, 0, 0.5f, 1, 0, 1, 0, -1, 1, -1, 1, 1, 0.5f, 0.5f, 0, 1, 0, 1, 0, 1, 1, -1, 0, 0, 1, 0, 0, 1, 0, -1, 0, -1, -1, 1, 0, 1, 1, 1, 1, 1, 0, -1, 0, -1, -1, -1, 1, 0, 0, 0, 1, 1, 0, -1, 0, 1, -1, 1, 0, 1, 1, 1, 1, 1, 0, -1, 0, -1, -1, -1, 1, 0, 0, 0, 1, 1, 0, -1, 0, 1, -1, 1, 1, 1, 0, 0.5f, 1.5f, 1, 0, -1, 0, 1, -1, -1 };
