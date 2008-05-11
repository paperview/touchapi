#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#define OF_ADDON_USING_OFXXMLSETTINGS  // LOAD CONFIG.XML
#define OF_ADDON_USING_OFXPARAMETERUI  // RENDER GUI (SLIDERS/BUTTONS).
#define OF_ADDON_USING_OFXOPENCV	   // COMPUTER VISION STUFF
#define OF_ADDON_USING_OFXOSC		   // OSC COMMUNICATION		
#include "ofAddons.h"
					
#define HOST "localhost"
#define PORT 3333

//#define _USE_LIVE_VIDEO		// uncomment this to use a live camera

class testApp : public ofSimpleApp
{
	public:
		void setup();
		void update();
		void draw();
		void exit();
		void setupUI();

		void keyPressed(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased();

		void fingerMoved(int x, int y);
		void fingerDragged(int x, int y, int button);
		void fingerPressed(int x, int y, int button);
		void fingerReleased();

		void changedVar(float _newVal, int arg1, int arg2, int arg3);

        #ifdef _USE_LIVE_VIDEO
		  ofVideoGrabber 		vidGrabber;
		#else
		  ofVideoPlayer 		vidPlayer;
		#endif

		int 				frameseq;	
		int 				threshold;
		int 				blurValue;
		int 				blurGaussianValue;
		int					wobbleThreshold;
		int 				frameRate;
		int 				camWidth;
		int 				camHeight;
		int					winWidth;
		int					winHeight;
		int					minWidth;
		int					minHeight;
		int 				snapCounter;
		int					lowRange;
		int					highRange;
		
		bool				bDrawVideo;
		bool  				bFastMode;
		bool				bLearnBakground;		
		bool				bInvertVideo;		
		bool				bToggleHelp;
		bool				bDrawOutlines;	
		bool				bTUIOMode;
		bool  				bFullscreen;
		bool				bSmooth;	
		bool 				bSnapshot;	
		bool 				bCalibration;	
		bool				bVerticalMirror;
		bool				bHorizontalMirror;
		bool				bSlimMode;
		bool				bShowLabels;

		float 				counter;
		float				oldX;
		float				oldY;
		float				newX;
		float				newY;

		char				eventString[255];
		char				timeString[255];
		
		ofTrueTypeFont		verdana;
		ofImage				logo;
		ofImage				menuBG;

		//CBoxAligner		m_box;
		//CTrackingManager	m_tracker;
		//bool				bShowTouchScreen;		
//	

		AParameterUI*		parameterUI;
		bool				bSpaced;	
	
		//--------------------------ParameterUI  Vars
		//void fireFunction();

		//Send contour data to OSC
		void SendOSC();
	private:

		ofxOscSender		TUIOSocket; 

		ofxCvColorImage		sourceImg;
        ofxCvGrayscaleImage grayImg;
		ofxCvGrayscaleImage grayBg;
		ofxCvGrayscaleImage grayDiff;		
		
		unsigned char * 	videoInverted;
		ofTexture			videoInvertTexture;
		ofTexture			videoTexture;
        ofxCvContourFinder	contourFinder;

	//--------------------------XML Settings Vars (BLOATED)
		ofxXmlSettings XML;
		string xmlStructure;
		string message;		
		
		int pointCount;
		int lineCount;
		int lastTagNumber;		
		
		float red;
		float green;
		float blue;
};
#endif
