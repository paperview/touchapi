#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"

#define OF_ADDON_USING_OFXOPENCV
#define OF_ADDON_USING_OFXOSC

#include "ofAddons.h"
					
#define HOST "localhost"
#define PORT 3333

#define _USE_LIVE_VIDEO		// uncomment this to use a live camera

class testApp : public ofSimpleApp{

	public:
		void setup();
		void update();
		void draw();
		void exit();

		void keyPressed  (int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased();

		void fingerMoved(int x, int y );
		void fingerDragged(int x, int y, int button);
		void fingerPressed(int x, int y, int button);
		void fingerReleased();

        #ifdef _USE_LIVE_VIDEO
		  ofVideoGrabber 		vidGrabber;
		#else
		  ofVideoPlayer 		vidPlayer;
		#endif

		int 				frameseq;	
		int 				threshold;
		int 				blurhold;
		int					wobbleThreshold;
		int 				frameRate;
		int 				camWidth;
		int 				camHeight;		
		int 				snapCounter;
//	
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
//
		float 				counter;
		float				oldX;
		float				oldY;
		float				newX;
		float				newY;

		char eventString[255];
		char timeString[255];
//		
		ofTrueTypeFont	verdana;			
		ofImage logo;

		//CBoxAligner          m_box;
		//CTrackingManager	 m_tracker;
		//bool				bShowTouchScreen;		
//
	private:

		ofxOscSender			TUIOSocket; 

		ofxCvColorImage			sourceImg;
        ofxCvGrayscaleImage 	grayImage;
		ofxCvGrayscaleImage 	grayBg;
		ofxCvGrayscaleImage 	grayDiff;		
		
		unsigned char * 	videoInverted;
		ofTexture			videoInvertTexture;
		ofTexture			videoTexture;
        ofxCvContourFinder 	contourFinder;
 
	protected:
			
			
};

#endif
