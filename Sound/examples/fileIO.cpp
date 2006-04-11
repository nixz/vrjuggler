/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000, 2001, 2002
 *   by Iowa State University
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

#include "fileIO.h"

std::vector<std::string> fileIO::mPaths;

//: true - 
bool fileIO::fileExists( const char* const name )
{
	FILE* file = ::fopen( name, "r" );
	if (file == NULL)
	{
		return false;
	}

	else
	{
		::fclose( file );
		return true;
	}
}

bool fileIO::fileExists( const std::string& name )
{
   return fileExists( name.c_str() );
}

bool fileIO::fileExistsResolvePath( const char* const filename, std::string& realPath )
{
   realPath = resolvePathForName( filename );
   return fileExists( realPath.c_str() );
}

bool fileIO::fileExistsResolvePath( const std::string& filename, std::string& realPath )
{
   return fileExistsResolvePath( filename.c_str(), realPath );
}

std::string fileIO::resolvePathForName( const char* const filename )
{
   for (int x = 0; x < fileIO::mPaths.size(); ++x)
   {
      std::string slash = "/";
      std::string temp  = fileIO::mPaths[x] + slash + filename;
      
      // if this path works, then return it.
      if (fileExists( temp ))
      {
         cout<<"Fixed path: "<<temp.c_str()<<"\n"<<flush;
         return temp;
      }
   }
   
   // couldn't find any that matched, so just return the filename.
   cout<<"Did not fix path: "<<filename<<"\n"<<flush;
   return filename;
}
