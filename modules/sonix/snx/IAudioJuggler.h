/* Generated by Together */

#ifndef IAUDIOJUGGLER_H
#define IAUDIOJUGGLER_H

#include <string>
#include "aj/SoundInfo.h"
#include "aj/SoundAPIInfo.h"
#include "aj/Matrix44.h"
   
/** @interface*/
class IAudioJuggler
{
public:

   IAudioJuggler() {}

   virtual ~IAudioJuggler() {}

   /**
    * @input alias of the sound to trigger, and number of times to play
    * @preconditions alias does not have to be associated with a loaded sound.
    * @postconditions if it is, then the loaded sound is triggered.  if it isn't then nothing happens.
    * @semantics Triggers a sound
    */
   virtual void trigger(const std::string & alias, const unsigned int & repeat = -1) = 0;

   /*
    * when sound is already playing then you call trigger,
    * does the sound restart from beginning?
    * (if a tree falls and no one is around to hear it, does it make sound?)
    */
   virtual void setRetriggerable( const std::string& alias, bool onOff ) = 0;

   /**
    * ambient or positional sound.
    * is the sound ambient - attached to the listener, doesn't change volume
    * when listener moves...
    * or is the sound positional - changes volume as listener nears or retreats..
    */
   virtual void setAmbient( const std::string& alias, bool setting = false ) = 0;

   /**
    * @semantics stop the sound
    * @input alias of the sound to be stopped
    */
   virtual void stop( const std::string & name ) = 0;

   /**
    * pause the sound, use unpause to return playback where you left off...
    */
   virtual void pause( const std::string& alias ) = 0;

   /**
    * resume playback from a paused state.  does nothing if sound was not paused.
    */
   virtual void unpause( const std::string& alias ) = 0;

   /**
    * mute, sound continues to play, but you can't hear it...
    */
   virtual void mute( const std::string& alias ) = 0;

   /**
    * unmute, let the muted-playing sound be heard again
    */
   virtual void unmute( const std::string& alias ) = 0;

   /**
    * set sound's 3D position 
    * @input x,y,z are in OpenGL coordinates.  alias is a name that has been associate()d with some sound data
    */
   virtual void setPosition( const std::string& alias, const float& x, const float& y, const float& z ) = 0;

   /**
    * get sound's 3D position
    * @input alias is a name that has been associate()d with some sound data
    * @output x,y,z are returned in OpenGL coordinates.
    */
   virtual void getPosition( const std::string& alias, float& x, float& y, float& z ) = 0;


   /**
    * set the position of the listener
    */
   virtual void setListenerPosition( const aj::Matrix44& mat ) = 0;

   /**
    * get the position of the listener
    */
   virtual void getListenerPosition( aj::Matrix44& mat ) = 0;

   virtual void changeAPI( const std::string& apiName ) = 0;
   
   virtual void configure( const aj::SoundAPIInfo& sai ) = 0;

   /**
     * configure/reconfigure a sound
     * configure: associate a name (alias) to the description if not already done
     * reconfigure: change properties of the sound to the descriptino provided.
     * @preconditions provide an alias and a SoundInfo which describes the sound
     * @postconditions alias will point to loaded sound data
     * @semantics associate an alias to sound data.  later this alias can be used to operate on this sound data.
     */
   virtual void configure( const std::string& alias, const aj::SoundInfo& description ) = 0;

     
   /**
    * remove a configured sound, any future reference to the alias will not
    * cause an error, but will not result in a rendered sound
    */
   virtual void remove(const std::string alias) = 0;

   /**
    * @semantics call once per sound frame (doesn't have to be same as your graphics frame)
    * @input time elapsed since last frame
    */
   virtual void step( const float& timeElapsed ) = 0;

public:

   /** @link dependency */
   /*#  aj::SoundInfo lnkSoundInfo; */
private:

   /** @link dependency */
   /*#  aj::SoundAPIInfo lnkaj::SoundAPIInfo; */
public:
};

#endif  //IAUDIOJUGGLER_H
