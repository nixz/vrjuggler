/*
 *  File:          $RCSfile$
 *  Date modified: $Date$
 *  Version:       $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                             Patrick Hartling
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                  Copyright (C) - 1997, 1998, 1999, 2000
 *              Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


#ifndef _VJ_PF_DRAW_MANAGER_
#define _VJ_PF_DRAW_MANAGER_

#include <vjConfig.h>
#include <function.h>
#include <algorithm>

#include <Performer/pf/pfChannel.h>

#include <Kernel/vjDrawManager.h>
#include <Kernel/vjDisplay.h>
//#include <Kernel/Pf/vjPfApp.h>
#include <Kernel/Pf/vjPfUtil.h>


class vjPfApp;
class vjProjection;
class vjConfigChunkDB;
class vjSimDisplay;

    // Performer Config function called in draw proc after window is set up
void vjPFconfigPWin(pfPipeWindow* pWin);
void vjPfDrawFunc(pfChannel *chan, void* chandata,bool left_eye, bool right_eye, bool stereo, bool simulator);

//------------------------------------------------------------
//: Concrete singleton class for API specific Draw Manager.
//
// Responsible for all Performer rendering and windowing
//
// @author Allen Bierbaum
//  Date: 9-7-97
//------------------------------------------------------------
class vjPfDrawManager : public vjDrawManager
{
protected:

   struct pfDisp
   {
      pfDisp()
      {
         chans[0] = chans[1] = NULL;
         disp = NULL;
         pWin = NULL;
      }

      enum {LEFT = 0, RIGHT = 1};

      vjDisplay*     disp;
      pfPipeWindow*  pWin;
      pfChannel*     chans[2];
   };

   struct findPfDispChan : unary_function<pfDisp, bool>
   {
      findPfDispChan(pfChannel* chan) : mChan(chan) {;}
      bool operator()(pfDisp disp) {
         return ((disp.chans[0] == mChan) || (disp.chans[1] == mChan));
      }
      pfChannel* mChan;                // Channel to find
   };

public:
    //: Function to config API specific stuff.
    // Takes a chunkDB and extracts API specific stuff
   virtual void configInitial(vjConfigChunkDB*  chunkDB);

    //: Blocks until the end of the frame
    //! PRE: none
    //! POST: The frame has been drawn
   virtual void sync();

   //: Enable a frame to be drawn
   //! PRE: none
   //! POST: Frame has been triggered to begin drawing
   virtual void draw();


   //: Set the app the draw whould interact with.
   //! PRE: none
   //! POST: self'.app = _app
   virtual void setApp(vjApp* _app);


   //: Initialize the drawing API (if not already running)
   //  should call pfInit()
   virtual void initAPI();

   //: Initialize the drawing state for the API based on
   //: the data in the display manager.
   //
   //! PRE: API is running (initAPI has been called)
   //! POST: API is ready do draw
   //
   // Configure process model
   // Configure multi-pipe model
   // Then call pfConfig to start the MP stuff
   // Sets up channels and pWins.
   //
   //!NOTE: Fork happens here
   virtual void initDrawing();

   //: Callback when display is added to display manager
   virtual void addDisplay(vjDisplay* disp);

   //: Callback when display is removed to display manager
   virtual void removeDisplay(vjDisplay* disp)
   {;}

      //: Shutdown the drawing API
   virtual void closeAPI();

      //: Update all the projections for the displays
      //!POST: All windows have the projections correctly set.
   virtual void updateProjections();

   //: dumps the object's internal state
   void debugDump();

   friend void vjPFconfigPWin(pfPipeWindow* pWin);
   friend void vjPfDrawFunc(pfChannel *chan, void* chandata,bool left_eye, bool right_eye, bool stereo, bool simulator);

public: // Chunk handlers
   //: Add the chunk to the configuration
   //! PRE: configCanHandle(chunk) == true
   //! RETURNS: success
   virtual bool configAdd(vjConfigChunk* chunk)
   { return false; }

   //: Remove the chunk from the current configuration
   //! PRE: configCanHandle(chunk) == true
   //!RETURNS: success
   virtual bool configRemove(vjConfigChunk* chunk)
   { return false; }

   //: Can the handler handle the given chunk?
   //! RETURNS: true - Can handle it
   //+          false - Can't handle it
   virtual bool configCanHandle(vjConfigChunk* chunk)
   { return false; }

protected:
   //: Call all the application channel callbacks
   void callAppChanFuncs();

   //: Helper to set channel view params from a vjProjection
   void updatePfProjection(pfChannel* chan, vjProjection* proj, bool simulator=false);

   //: Helper function to bind Performer to the pfApp
   void initPerformerApp();

   //: Helper to initialize the Performer simulator
   void initSimulator();
   void initLoaders();
   void updateSimulator(vjSimDisplay* sim);

   //: Helper to get the pfDisp given a channel
   //! RETURNS: NULL - Not found
   pfDisp* getPfDisp(pfChannel* chan);

   //: Return the needed mono frame buffer config
   std::vector<int> getMonoFBConfig();
   //: Return the needed stereo frame buffer config
   std::vector<int> getStereoFBConfig();

protected:
   // NOTE:  ---- REMEMBER THAT PF HAS SHARED MEM Model ---
   // Rember that Performer uses forks, so it's processes need to
   // have shared memory allocated variable in order to work
   // correctly.

   // --- Config Data --- //
   unsigned int numPipes;    // The number of Performer pipes

   // --- Performer State --- //
   vjPfApp*             app;        // There User applications
   std::vector<pfDisp>  disps;      // List of displays with Performer data
   std::vector<pfPipe*> pipes;      // Performer pipes we have opened
   std::vector<char*> pipeStrs;     // The X-Strs of the pipes
   pfScene*          sceneRoot;     // Root of Performer tree to render
   pfGroup*          mSceneGroup;   // The group node with only sceneRoot under it

   // ---- Simulator stuff --- //
   pfGroup*          mSimTree;      // The simulator scene graph
   pfScene*          mRootWithSim;  // The root with the simulator group & the sceneRoot
   pfDCS*            mHeadDCS;      // The DCS above the head
   pfDCS*            mWandDCS;      // The DCS above the wand
   std::string       mHeadModel;    // The head model file path
   std::string       mWandModel;    // The wand model file path

   // --- Singleton Stuff --- //
public:
   static vjPfDrawManager* instance()
   {
      if (_instance == NULL)
         _instance = new vjPfDrawManager();
      return _instance;
   }
protected:
   vjPfDrawManager() {
      sceneRoot    = NULL;
      mSimTree     = NULL;
      mRootWithSim = NULL;
      mHeadDCS     = NULL;
      mWandDCS     = NULL;
   }

   virtual ~vjPfDrawManager() {}

private:
   static vjPfDrawManager* _instance;
};


#endif
