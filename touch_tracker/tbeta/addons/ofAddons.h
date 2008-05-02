
// here is where you paste any "add me to ofAddons.h" code
// for the addon you want to add
// you will also need to follow the other steps
// like adding the library linking paths, etc,etc.


//-------------------------------------------- ofxParameterUI
#ifdef OF_ADDON_USING_OFXPARAMETERUI
	#include "AParameterUI.h"
#endif

//-------------------------------------------- ofxOpenCv
#ifdef OF_ADDON_USING_OFXOPENCV
	#include "ofxCvMain.h"
#endif

//-------------------------------------------- ofxObjLoader
#ifdef OF_ADDON_USING_OFXOBJLOADER
	#include "ofxObjLoader.h"
#endif

//-------------------------------------------- ofxDirList
#ifdef OF_ADDON_USING_OFXDIRLIST
	#include "ofxDirList.h"
#endif

//-------------------------------------------- ofxVectorMath
#ifdef OF_ADDON_USING_OFXVECTORMATH
	#include "ofxVectorMath.h"
#endif

//-------------------------------------------- ofxNetwork
#ifdef OF_ADDON_USING_OFXNETWORK
	#include "ofxNetwork.h"
#endif

//-------------------------------------------- ofxVectorGraphics
#ifdef OF_ADDON_USING_OFXVECTORGRAPHICS
	#include "ofxVectorGraphics.h"
#endif

//-------------------------------------------- ofxOSC
#ifdef OF_ADDON_USING_OFXOSC
	#include "ofxOsc.h"
#endif

//-------------------------------------------- ofxThread
#ifdef OF_ADDON_USING_OFXTHREAD
	#include "ofxThread.h"
#endif

//-------------------------------------------- ofxXmlSettings
#ifdef OF_ADDON_USING_OFXXMLSETTINGS
	#include "ofxXmlSettings.h"
#endif
