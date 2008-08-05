#ifndef _TEST_APP
#define _TEST_APP

#define OF_ADDON_USING_OFXXMLSETTINGS  // LOAD CONFIG.XML
#define OF_ADDON_USING_OFXOPENCV	   // COMPUTER VISION STUFF
#define OF_ADDON_USING_OFXOSC		   // OSC COMMUNICATION		


#include "ofMain.h"

#include "ofAddons.h"

//Used for tracking algo
#include "Tracking/tracking.h"

//GPU image filters
#include "Filters/ImageFilter.h"

//Used for warped image calibration
#include "Calibration/boxAlign.h"

//Used other calibration
#include "Calibration/calibrationB.h"

//Communications
#include "Communication/TUIOOSC.h"

class testApp : public ofSimpleApp, public ofCvBlobListener 
{


public:

		/****************************************************************
		*						Public functions
		****************************************************************/
		
		//Basic Methods
		void setup();
		void update();
		void draw();
		void exit();

		//Key events
		void keyPressed(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);

		//Touch Events
		void blobOn(ofxCvBlob b);
		void blobMoved(ofxCvBlob b);   
		void blobOff(ofxCvBlob b);


		//image processing stuff
		void grabFrame();
		void applyImageFilters();

		void grabFrameToGPU(GLuint target);
		void applyGPUImageFilters();
		

		//Other Methods
		void loadXMLSettings();								  // Load Settings
        void bgCapture(ofxCvGrayscaleImage & _giLive);        //Background Capture
	    void learnBackground( ofxCvGrayscaleImage & _giLive,  //Background Learn (bgCapture and dynamic Bg subtraction 
							  ofxCvGrayscaleImage & _grayBg, 
							  ofxCvFloatImage & _fiLearn,
							  float _fLearnRate );
				
		
		/***************************************************************
		*						Video Settings
		***************************************************************/
       
		//#ifdef _USE_LIVE_VIDEO
		  ofVideoGrabber 		vidGrabber;
		//#else
		  ofVideoPlayer 		vidPlayer;
		//#endif


		/****************************************************************
		*            Variables in config.xml Settings file
		*****************************************************************/
        
	    int					deviceID;
		int 				frameseq;	
		int 				threshold;
		int 				blurValue;
		int 				blurGaussianValue;
		int					wobbleThreshold;
		int 				camRate;
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
		int					smooth;
		
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
		bool 				bSnapshot;	
		bool 				bCalibration;	
		bool				bVerticalMirror;
		bool				bHorizontalMirror;
		bool				bSlimMode;
		bool				bShowLabels;
		bool				bNewFrame;
		bool				bWarpImg;

		bool				bcamera;

		//filters
		bool				bHighpass;
		bool				bAmplify;
		bool				bThreshold;
		bool				bSmooth;
		bool				bDynamicBG;

		//modes
		bool				bGPUMode;

		/****************************************************
		*End config.xml variables
		*****************************************************/

		bool				bW;
		bool				bS;
		bool				bA;
		bool				bD;

		bool				activeInput;

		float				fLearnRate;// rate to learn background
		
		//FPS variables
		int 					frames;
		int  					fps;
		float					lastFPSlog;

		char				eventString[255];
		char				timeString[255];
		
		//---------------------------------------Fonts
		ofTrueTypeFont		verdana;
		ofTrueTypeFont      sidebarTXT;
		ofTrueTypeFont		calibrationText;
		ofTrueTypeFont		bigvideo;
		ofImage				logo;
		
		//---------------------------------------Images
		ofImage				background;
		ofImage				menuBG;


		//---------------------------------------GUI

		bool				bSpaced;	


		//---------------------------------------Blob Tracker	
		BlobTracker			tracker;

		float				downColor;


		string				tmpLocalHost;
		int					tmpPort;

	private:
		
		//---------------------------------------Calibration	
		calibrationB calibrate;

		//---------------------------------------Blob Finder	
		ofxCvContourFinder	contourFinder;

		//---------------------------------------Images

		ofxCvColorImage		sourceImg;
		ofxCvGrayscaleImage processedImg;

		ofxCvGrayscaleImage grayBg;
		ofxCvGrayscaleImage	giWarped;

		//---------------------------------------Background Subtraction
	    ofxCvFloatImage		fiLearn;

		//---------------------------------------Warping Box				
		CBoxAligner			warp_box;
		ofxPoint2f			dstPts[4];
			//Warp
		float					handle1x;
		float					handle1y;
		float					handle2x;			
		float					handle2y;		
		float					handle3x;		
		float					handle3y;		
		float					handle4x;		
		float					handle4y;

		//---------------------------------------XML Settings Vars (BLOATED)
		ofxXmlSettings		XML;
		ofxXmlSettings		calibrationXML;
		string				xmlStructure;
		string				message;


		//---------------------------------------FOR NETWORK 
		TUIOOSC				myTUIO;



		//---------------------------------------FOR gpuTracker
		GLuint			gpuSourceTex;
		GLuint			gpuBGTex;

		unsigned char * gpuReadBackBuffer;
		ofxCvColorImage gpuReadBackImage;
		ofxCvGrayscaleImage gpuReadBackImageGS;
		
		ImageFilter*	contrastFilter;
		ImageFilter*	subtractFilter;
		ImageFilter*	subtractFilter2; //we need 2 because we are showing the output of each
		ImageFilter*	gaussVFilter;
		ImageFilter*	gaussVFilter2;
		ImageFilter*	gaussHFilter;
		ImageFilter*	gaussHFilter2;
		ImageFilter*	threshFilter;

		
};
#endif
