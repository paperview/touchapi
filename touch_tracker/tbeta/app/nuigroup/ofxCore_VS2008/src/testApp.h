#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#define OF_ADDON_USING_OFXXMLSETTINGS  // LOAD CONFIG.XML
#define OF_ADDON_USING_OFXPARAMETERUI  // RENDER GUI (SLIDERS/BUTTONS).
#define OF_ADDON_USING_OFXOPENCV	   // COMPUTER VISION STUFF
#define OF_ADDON_USING_OFXOSC		   // OSC COMMUNICATION		
#include "ofAddons.h"

//Used for tracking algo
#include "Tracking\tracking.h"

//Used for warped image calibration
#include "Calibration\boxAlign.h"

//Used other calibration
#include "Calibration\calibrationB.h"

#include "Thingy.h"
					
#define HOST "localhost"
#define PORT 3333

#define _USE_LIVE_VIDEO					// uncomment this to use a live camera



class testApp : public ofSimpleApp, public ofCvBlobListener
{
	public:

		vector<Thingy> thingies;

		/****************************************************************
		*						Public functions
		****************************************************************/
		
		//Basic Methods
		void setup();
		void update();
		void draw();
		void exit();
		void setupUI();

		//Key events
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased();

		//Touch Events
		void blobOn(ofxCvBlob b);
		void blobMoved(ofxCvBlob b);   
		void blobOff(ofxCvBlob b);


		//Other Methods
		void loadXMLSettings();								  // Load Settings
		void SendOSC();										  //Send data through OSC
        void bgCapture(ofxCvGrayscaleImage & _giLive);        //Background Capture
	    void learnBackground( ofxCvGrayscaleImage & _giLive,  //Background Learn (bgCapture and dynamic Bg subtraction 
							  ofxCvGrayscaleImage & _grayBg, 
							  ofxCvFloatImage & _fiLearn,
							  float _fLearnRate );
		void doCalibration();

		
		
		/***************************************************************
		*						Video Settings
		***************************************************************/
       
		#ifdef _USE_LIVE_VIDEO
		  ofVideoGrabber 		vidGrabber;
		#else
		  ofVideoPlayer 		vidPlayer;
		#endif


		/****************************************************************
		*            Variables in config.xml Settings file
		*****************************************************************/
        
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
		int					highpassBlur;
		int					highpassNoise;
		int					highpassAmp;
		
		bool				bDrawVideo;
		bool  				bFastMode;
		bool				bShowInterface;
		bool				bShowPressure;
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
		bool				bNewFrame;		

		/****************************************************
		*End config.xml variables
		*****************************************************/

		bool				bW;
		bool				bS;
		bool				bA;
		bool				bD;

		float				fLearnRate;// rate to learn background
		
		float				transformedX;
		float				transformedY;


		//FPS variables
		int 					frames;
		int  					fps;
		float					lastFPSlog;

		char				eventString[255];
		char				timeString[255];
		
		//---------------------------------------Fonts
		ofTrueTypeFont		verdana;
		ofTrueTypeFont		calibrationText;
		ofTrueTypeFont		bigvideo;
		ofImage				logo;
		
		//---------------------------------------Images
		ofImage				background;
		ofImage				menuBG;


		//---------------------------------------GUI
		AParameterUI*		parameterUI;
		bool				bSpaced;	


		//---------------------------------------Blob Tracker	
		BlobTracker			tracker;

		float				downColor;


	private:
		
		//---------------------------------------Calibration	
		calibrationB calibrate;

		//---------------------------------------Blob Finder	
		ofxCvContourFinder	contourFinder;

		//---------------------------------------OSC
		ofxOscSender		TUIOSocket; 

		//---------------------------------------Images
		ofxCvColorImage		sourceImg;
        ofxCvGrayscaleImage grayImg;
		ofxCvGrayscaleImage grayBg;
		ofxCvGrayscaleImage subtractBg;
		ofxCvGrayscaleImage grayDiff;	
		ofxCvGrayscaleImage highpassImg;
		ofxCvGrayscaleImage	giWarped;

		//---------------------------------------Pressure Map
		ofxCvColorImage		pressureMap;

		//---------------------------------------Background Subtraction
	    ofxCvFloatImage		fiLearn;

		//---------------------------------------Warping Box				
		CBoxAligner			m_box;
		ofxPoint2f			dstPts[4];

		//---------------------------------------XML Settings Vars (BLOATED)
		ofxXmlSettings		XML;
		ofxXmlSettings		calibrationXML;
		string				xmlStructure;
		string				message;


		//---------------------------------------FOR NETWORK 
		char				myLocalHost[255];
		char				myRemoteHost[255];
		int					myTUIOPort;


};
#endif
