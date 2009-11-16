/****************** <SNX heading BEGIN do not edit this line> *****************
 *
 * sonix
 *
 * Original Authors:
 *   Kevin Meinert, Carolina Cruz-Neira
 *
 ****************** <SNX heading END do not edit this line> ******************/

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

/* Generated by Together */

#ifndef SOUND_INFO_DATA
#define SOUND_INFO_DATA

#include <snx/snxConfig.h>

#include <string>
#include <vector>
#include <gmtl/Math.h>
#include <gmtl/Matrix.h>
#include <gmtl/Vec.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/VecOps.h>
#include <gmtl/Xforms.h>

namespace snx
{

/** \struct SoundInfo SoundInfo.h snx/SoundInfo.h
 *
 * Informational type that describes one sound entry.  Typically, user code
 * will fill in the members of an instance and pass it to a snx::SoundHandle
 * object to configure the sound.
 *
 * @ingroup SonixAPI
 */
struct SoundInfo
{
   SoundInfo()
      : alias()
      , datasource(FILESYSTEM)
      , filename()
      , ambient(false)
      , retriggerable(false)
      , repeat(1)
      , pitchbend(1.0f)
      , cutoff(1.0f)
      , volume(1.0f)
      , streaming(false)
      , triggerOnNextBind(false)
      , repeatCountdown(0)
   {
      //position.makeIdent();
      position[0] = 0.0f;
      position[1] = 0.0f;
      position[2] = 0.0f;
   }

   /**
    * Name of the sound.  This is the "handle" that user code will use to
    * refer to the sound.
    */
   std::string alias;

   enum DataSource
   {
      FILESYSTEM,
      DATA_16_MONO,
      DATA_8_MONO
   };

   /// Which of the data sources to use.
   DataSource datasource;

   /**
    * The source of the sound.  For AudioWorks, the file should be a WAV
    * file sampled at 11,025 Hz mono.  For OpenAL, the file should be WAV
    * file recorded in mono.
    */
   std::string filename;

   /// 3D position.
   gmtl::Vec3f position;

   /// Is the sound ambient (true) or positional (false)?
   bool ambient;

   /// Can the sound be retriggered while playing?
   bool retriggerable;

   /// Number of times to repeat (static), -1 is infinite.
   int repeat;

   /// From 0 to 1.  0 is not a legal value.
   float pitchbend;

   /// From 0 to 1.
   float cutoff;

   /// From 0 to 1.
   float volume;

   /// Should we stream the sound from disk?
   bool streaming;

public:
   /// Do not use. Internal use only.
   bool triggerOnNextBind;

   /// Do not use. Internal use only.
   int repeatCountdown; // number of times left to repeat (changes during play)
};

} // end namespace

#endif //SOUND_INFO_DATA
