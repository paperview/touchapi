#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#define OF_ADDON_USING_OFXXMLSETTINGS  // LOAD CONFIG.XML
#define OF_ADDON_USING_OFXPARAMETERUI  // RENDER GUI (SLIDERS/BUTTONS).
#define OF_ADDON_USING_OFXOPENCV	   // COMPUTER VISION STUFF
#define OF_ADDON_USING_OFXOSC		   // OSC COMMUNICATION		
#include "ofAddons.h"
#include "tracking.h"

#include "boxAlign.h"				   // Used for warped image

#include "rect2d.h"		
#include "mesh2d.h"
#include "vector2d.h"

					
#define HOST "localhost"
#define PORT 3333

#define _USE_LIVE_VIDEO					// uncomment this to use a live camera



class testApp : public ofSimpleApp, public ofCvBlobListener
{
	public:

		/////////////////////////////////////////////////////////////////
		//						Public functions
		/////////////////////////////////////////////////////////////////
		
		//Basic Methods
		void setup();
		void update();
		void draw();
		void exit();
		void setupUI();

		//Key events
		void keyPressed(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased();

		//Touch Events
		void blobOn(ofxCvBlob b);
		void blobMoved(ofxCvBlob b);   
		void blobOff(ofxCvBlob b);


		//Other Methods
		void loadXMLSettings();								// Load Settings
		void SendOSC();										//Send data through OSC
        void bgCapture(ofxCvGrayscaleImage & _giLive);      //Background Capture
	    void learnBackground( ofxCvGrayscaleImage & _giLive,//Background Learn (bgCapture and dynamic Bg subtraction 
					  ofxCvGrayscaleImage & _grayBg, 
					  ofxCvFloatImage & _fiLearn,
					  float _fLearnRate );

		
		//Calibration Methods		
		void setScreenScale(float s);
		void setScreenBBox(rect2df & bbox);

		void setGrid(int x, int y);
		void initTriangles();

		virtual vector2df *getScreenPoints() { return screenPoints; };
		virtual vector2df *getCameraPoints() { return cameraPoints; };

		float getScreenScale();
		rect2df getScreenBBox() { return screenBB; };

		void cameraToScreenSpace(float &x, float &y);
		void transformDimension(float &width, float &height, float centerX, float centerY);

		void initScreenPoints();
		void initCameraPoints();
	
		// returns -1 if none found..
		int findTriangleWithin(vector2df pt);

		//! starts calibration
		virtual void beginCalibration();

		//! goes to the next step
		virtual void nextCalibrationStep();

		//! return to the last step
		virtual void revertCalibrationStep();




		/////////////////////////////////////////////////////////////////
		//						Video Settings
		/////////////////////////////////////////////////////////////////
       
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
		ofTrueTypeFont		bigvideo;
		ofImage				logo;
		
		//---------------------------------------Images
		ofImage				background;
		ofImage				menuBG;


		//---------------------------------------GUI
		AParameterUI*		parameterUI;
		bool				bSpaced;	
		//void fireFunction();

		//---------------------------------------Blob Tracker	
		BlobTracker			tracker;

		bool bCalibrating;		
		int calibrationStep;


	private:
		//---------------------------------------Calibration

		int GRID_X;
		int GRID_Y;

		//set Calibration Points
		int GRID_POINTS;
		int GRID_INDICES;

		//vector2df screenPoints[GRID_POINTS];		// GRID_X * GRID_Y
		//vector2df cameraPoints[GRID_POINTS];		// GRID_X * GRID_Y
		//int triangles[GRID_INDICES];				// GRID_X * GRID_Y * 2t * 3i indices for the points
		
		vector2df* screenPoints;					// GRID_X * GRID_Y
		vector2df* cameraPoints;					// GRID_X * GRID_Y
		int* triangles;								// GRID_X * GRID_Y * 2t * 3i indices for the points



		rect2df screenBB;
		mesh2df screenMesh;

		bool bscreenPoints;
		bool bcameraPoints;

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

	    ofxCvFloatImage		fiLearn;

		//---------------------------------------Warping Box				
		CBoxAligner			m_box;
		ofxPoint2f			dstPts[4];

		//---------------------------------------XML Settings Vars (BLOATED)
		ofxXmlSettings		XML;
		string				xmlStructure;
		string				message;

		int					pointCount;
		int					lineCount;
		int					lastTagNumber;		

		//---------------------------------------FOR TEST MARKED FOR GC
		float				red;
		float				green;
		float				blue;
		bool				bblobs;

		//---------------------------------------FOR NETWORK 
		char				myLocalHost[255];
		char				myRemoteHost[255];
		int					myTUIOPort;
};
#endif
