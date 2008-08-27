#include "testApp.h"
#include "gui.h"

/******************************************************************************
 * The setup function is run once to perform initializations in the application
 *****************************************************************************/
void testApp::setup()
{	
	/********************
	* Initalize Variables
	*********************/
	fuzzy.loadImage("particle.png");
	fuzzy.setUseTexture(true);

	ofImage blah;
	blah.clone(fuzzy);
	

	//For screengrab
	snapCounter	= 6; 
	//Background Subtraction Learning Rate
	fLearnRate	= 0.0001f;
	//Intialize FPS variables
	frames		= 0;
	fps			= 0;
	lastFPSlog	= 0;
	//Calibration Booleans
	bCalibration= false;
	bShowTargets = true;
	bW			= false;
	bA			= false;
	bS			= false;
	bD			= false;

	downColor = 0xFF0000;

	bDrawVideo = true;
	bFullscreen = false;

	ofSetBackgroundAuto(false);

	//Load Settings from config.xml file 
	loadXMLSettings();

	//Load Calibration Settings from calibration.xml file
	calibrate.setCamRes(camWidth, camHeight);
	calibrate.loadXMLSettings();

	//Setup Window Properties 
	ofSetWindowShape(winWidth,winHeight);
	ofSetFrameRate(camRate);			//This will be based on camera fps in the future		
	ofSetVerticalSync(false);	//Set vertical sync to false for better performance

	//Pick the Source - camera or video
	if(bcamera){

		activeInput = true;

		vidGrabber.listDevices();
		vidGrabber.setDeviceID(deviceID);
		vidGrabber.setVerbose(true);		
        vidGrabber.initGrabber(camWidth,camHeight);		

		printf("Camera Mode\n");
		int grabW = vidGrabber.width;
		int grabH = vidGrabber.height;
		printf("Asked for %i by %i - actual size is %i by %i \n", 
				camWidth, camHeight, grabW, grabH);
	}
	else{
		
		activeInput = true;	

        //vidPlayer.loadMovie("test_videos/FrontDI.m4v");
		//vidPlayer.loadMovie("test_videos/HCI_FTIR.mov");
		vidPlayer.loadMovie("test_videos/raw.mp4");
		//vidPlayer.loadMovie("test_videos/5point.avi");
        vidPlayer.play();	
		printf("Video Mode\n");
		camHeight = vidPlayer.height;
		camWidth = vidPlayer.width;
	}
    
	/*****************************************************************************************************
	* Allocate images (needed for drawing/processing images) ----Most of These won't be needed in the end
	******************************************************************************************************/
	processedImg.allocate(camWidth, camHeight); //main Image that'll be processed.
	processedImg.setUseTexture(false);
	sourceImg.allocate(camWidth, camHeight);    //Source Image
	sourceImg.setUseTexture(false);				//We don't need to draw this so don't create a texture

	//These images are used for drawing only
	grayImg.allocate(camWidth, camHeight);		//Gray Image
	grayBg.allocate(camWidth, camHeight);		//Background Image
	subtractBg.allocate(camWidth, camHeight);   //Background After subtraction
	grayDiff.allocate(camWidth, camHeight);		//Difference Image between Background and Source
	highpassImg.allocate(camWidth, camHeight);  //Highpass Image
	ampImg.allocate(camWidth, camHeight);		//Amplied Image	
	fiLearn.allocate(camWidth, camHeight);		//ofxFloatImage used for simple dynamic background subtracti
//	fiLearn.setUseTexture(false);
	pressureMap.allocate(camWidth, camHeight);	//Pressure Map Image
	
	//For camera warp
	giWarped.allocate(camWidth, camHeight);     //Warped Image (used for warped calibration)
	giWarped.setUseTexture(false);
	/********************************************************************************************************/


	/**********************************************************/
	//GPU stuff initialization
	/**********************************************************/
	glGenTextures(1, &gpuSourceTex);
	glGenTextures(1, &gpuBGTex);

	//initialize texture once with glTexImage2D so we can use gltexSubImage2D afetrwards (fastser)
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, gpuSourceTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8,  camWidth, camHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, gpuBGTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8,  camWidth, camHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);

	grabFrameToGPU(gpuBGTex);

	//so very inefficient..but only for now..until i fix the gpu blob detection and geoemtry shader for variable length output
	gpuReadBackBuffer = new unsigned char[camWidth*camHeight*3]; 
	gpuReadBackImage.allocate(camWidth, camHeight);
	gpuReadBackImageGS.allocate(camWidth, camHeight);

	subtractFilter = new ImageFilter("filters/absSubtract.xml", camWidth, camHeight);
	subtractFilter2 = new ImageFilter("filters/subtract.xml", camWidth, camHeight);
	contrastFilter = new ImageFilter("filters/contrast.xml", camWidth, camHeight);
	gaussVFilter = new ImageFilter("filters/gaussV.xml", camWidth, camHeight);
	gaussHFilter = new ImageFilter("filters/gauss.xml", camWidth, camHeight);
	gaussVFilter2 = new ImageFilter("filters/gaussV2.xml", camWidth, camHeight);
	gaussHFilter2 = new ImageFilter("filters/gauss2.xml", camWidth, camHeight);
	threshFilter = new ImageFilter("filters/threshold.xml", camWidth, camHeight);
	/**********************************************************/




	//Fonts - Is there a way to dynamically change font size?
	verdana.loadFont("verdana.ttf", 8, true, true);	   //Font used for small images
	sidebarTXT.loadFont("verdana.ttf", 8, true, true);
	calibrationText.loadFont("verdana.ttf", 10, true, true);
	bigvideo.loadFont("verdana.ttf", 13, true, true);  //Font used for big images.
	
	//Static Images
	logo.loadImage("images/logo.jpg");
	background.loadImage("images/background.jpg"); //Main (Temp?) Background

	//Setup green warped box
	warp_box.setup( 40, 30, camWidth, camHeight, camWidth/320, camHeight/240); 

	warp_box.fHandles[0].x = handle1x;
	warp_box.fHandles[0].y = handle1y;
	warp_box.fHandles[1].x = handle2x;
	warp_box.fHandles[1].y = handle2y;
	warp_box.fHandles[2].x = handle3x;
	warp_box.fHandles[2].y = handle3y;
	warp_box.fHandles[3].x = handle4x;
	warp_box.fHandles[3].y = handle4y;

	//Warped points
	dstPts[0].x = 0.0f;
    dstPts[0].y = camHeight;   
    dstPts[1].x = camWidth;
    dstPts[1].y = camHeight;   
    dstPts[2].x = camWidth;
    dstPts[2].y = 0.0f;   
    dstPts[3].x = 0.0f;
    dstPts[3].y = 0.0f;

	//Setup testApp to listen for touch events
	tracker.setListener(this); 

	gui = ofxGui::Instance(this);
	setupGUI();
	
	printf("Touchlib application is setup!\n");
}




void testApp::grabFrameToGPU(GLuint target){
	//grab the frame to a raw openGL texture
	if(bcamera)
	{
		glEnable(GL_TEXTURE_2D);
		//glPixelStorei(1);
		glBindTexture(GL_TEXTURE_2D, target);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, camWidth, camHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, vidGrabber.getPixels());
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, camWidth, camHeight, GL_RGB, GL_UNSIGNED_BYTE, vidGrabber.getPixels());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D,0);
	}
	else{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, target);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, camWidth, camHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, vidPlayer.getPixels());
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, camWidth, camHeight, GL_RGB, GL_UNSIGNED_BYTE, vidPlayer.getPixels());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D,0);
	}
}


void testApp::grabFrame(){
	//Set sourceImg as new camera/video frame
	if(bcamera)
		sourceImg.setFromPixels(vidGrabber.getPixels(), camWidth, camHeight);
	else
		sourceImg.setFromPixels(vidPlayer.getPixels(), 	camWidth, camHeight);
}


void testApp::applyGPUImageFilters(){

	if (bLearnBakground == true){
		grabFrameToGPU(gpuBGTex);
		bLearnBakground = false;
	}



	GLuint processedTex; 
	
	//tmp = contrastFilter->apply(gpuBGTex);
	processedTex = subtractFilter->apply(gpuSourceTex, gpuBGTex);

	//blur
	if(bSmooth){
		gaussHFilter->parameters["kernel_size"]->value = (float)smooth;
		gaussVFilter->parameters["kernel_size"]->value = (float)smooth;
		processedTex = gaussHFilter->apply(processedTex);
		processedTex = gaussVFilter->apply(processedTex);
	}

	//highpass
	if(bHighpass){
		gaussHFilter2->parameters["kernel_size"]->value = (float)highpassBlur;
		gaussVFilter2->parameters["kernel_size"]->value = (float)highpassBlur;
		processedTex = gaussHFilter2->apply(processedTex);
		processedTex = gaussVFilter2->apply(processedTex);
		processedTex = subtractFilter2->apply(gaussVFilter->output_texture, processedTex);
	}

	//amplifys
	if(bAmplify){

	}

	threshFilter->parameters["Threshold"]->value = (float)threshold / 255.0;
	threshFilter->apply(processedTex);


	//until the rest of the pipeline is fixed well just download the preprocessing result from the gpu and use that for the blob detection
	//TODO: make this part not super slow ;)
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, threshFilter->output_buffer);
	//glReadBuffer(gaussVFilter->output_buffer);
	glReadPixels(0,0,camWidth, camHeight, GL_RGB, GL_UNSIGNED_BYTE, gpuReadBackBuffer);
	gpuReadBackImage.setFromPixels(gpuReadBackBuffer, camWidth, camHeight);
	gpuReadBackImageGS = gpuReadBackImage;
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

}


void testApp::applyImageFilters(){
	/************************************************
	*				SET FILTERS HERE
	************************************************/
	processedImg = sourceImg;
	//Set Mirroring Horizontal/Vertical
	processedImg.mirror(bVerticalMirror, bHorizontalMirror);

	grayImg = processedImg; //for drawing

	if(bWarpImg){
		giWarped.warpIntoMe(processedImg, warp_box.fHandles, dstPts );				
		processedImg = giWarped;
	}

	//Dynamic background with learn rate...eventually learnrate will have GUI sliders
	if(bDynamicBG){
		learnBackground( processedImg, grayBg, fiLearn, fLearnRate);
	}
	
	//Capture full background
	if (bLearnBakground == true){
		bgCapture( processedImg );
		bLearnBakground = false;
	}

	//Background Subtraction
	processedImg.absDiff(grayBg, processedImg);

//	if(contourFinder.blobs.size() > 0)
//	processedImg.contrastStretch();

	if(bSmooth){
		processedImg.blur((smooth * 2) + 1); //needs to be an odd number
		subtractBg = processedImg; //for drawing
	}

	//HighPass
	if(bHighpass){
		processedImg.highpass(highpassBlur, highpassNoise);
		highpassImg = processedImg; //for drawing
	}

	//Amplify
	if(bAmplify){
		processedImg.amplify(processedImg, highpassAmp);
		ampImg = processedImg; //for drawing	
	}
	
	//Set a threshold value
	processedImg.threshold(threshold);
	grayDiff = processedImg; //for drawing

	//fidTrack.findFiducials(processedImg);
}




/******************************************************************************
 * The update function runs continuously. Use it to update states and variables
 *****************************************************************************/
void testApp::update()
{	
    bNewFrame = false;
		
	if(activeInput){

		if(bcamera){
		   vidGrabber.grabFrame();
		   bNewFrame = vidGrabber.isFrameNew();
		}
		else{
			vidPlayer.idleMovie();
			bNewFrame = vidPlayer.isFrameNew();
		}

		
		if (bNewFrame)
		{
			ofBackground(110, 110, 110);

			//Calculate FPS of Camera
			frames++;
			float time = ofGetElapsedTimeMillis();
			if(time > (lastFPSlog + 1000)){		
				fps = frames;
				frames = 0;
				lastFPSlog = time;			
			}//End calculation

		
			if(bGPUMode){
				grabFrameToGPU(gpuSourceTex);
				applyGPUImageFilters();
				contourFinder.findContours(gpuReadBackImageGS, 1, (camWidth*camHeight)/25, 20, false);
			}
			else{
				grabFrame();
				applyImageFilters();
				contourFinder.findContours(processedImg, 1, (camWidth*camHeight)/25, 20, false);
			}
			
			//Track found contours/blobs
			tracker.track(&contourFinder);

			/**************************************************
			* Background subtraction LearRate
			* If there are no blobs, add the background faster.
			* If there ARE blobs, add the background slower.
			***************************************************/
			fLearnRate = 0.01f;
			
			if(contourFinder.nBlobs > 0){

				fLearnRate = 0.0003f;
			}//End Background Learning rate


			if(bTUIOMode){
				//We're not using frameseq right now with OSC
				//myTUIO.update();

				//Start sending OSC
				myTUIO.sendOSC();
			}
		}
	} 
}

/******************************************************************************
 * The draw function paints the textures onto the screen. It runs after update.
 *****************************************************************************/
void testApp::draw(){

	ofSetFullscreen(bFullscreen);

    if (bNewFrame){

		/*********************************
		* IF CALIBRATING
		*********************************/
		if(bCalibration)
		{
			//Don't draw main interface
			bShowInterface = false;

			doCalibration();
		}
		/********************************
		* IF SHOWING MAIN INTERFACE STUFF
		********************************/
		if(bDrawVideo && bShowInterface && !bFastMode)
		{
			ofSetColor(0xFFFFFF);	
			//Draw Everything
			background.draw(0, 0);

			//Draw arrows
			ofSetColor(187, 200, 203);
			ofFill();
			ofTriangle(680, 420, 680, 460, 700, 440);
			ofTriangle(70, 420, 70, 460, 50, 440);
			ofSetColor(255, 255, 0);
			ofNoFill();
			ofTriangle(70, 420, 70, 460, 50, 440);
			
			ofSetColor(0xFFFFFF);

			if(bGPUMode){
				drawGLTexture(40, 30, 320, 240, gpuSourceTex);
				//subtractFilter->drawOutputTexture(85, 392, 128, 96);
				drawGLTexture(85, 392, 128, 96, gpuBGTex);
				gaussVFilter->drawOutputTexture(235, 392, 128, 96); 
				subtractFilter2->drawOutputTexture(385, 392, 128, 96); 
				threshFilter->drawOutputTexture(535, 392, 128, 96);
				gpuReadBackImageGS.draw(385, 30, 320, 240);
			}
			else{
				if(bShowPressure)
					pressureMap.draw(40, 30, 320, 240);
				else
					grayImg.draw(40, 30, 320, 240);

				grayDiff.draw(385, 30, 320, 240);
				fiLearn.draw(85, 392, 128, 96);
				subtractBg.draw(235, 392, 128, 96);
				highpassImg.draw(385, 392, 128, 96);
				ampImg.draw(535, 392, 128, 96);
			}

			ofSetColor(0x000000);
			if(bShowPressure){bigvideo.drawString("Pressure Map", 140, 20);}
			else             {bigvideo.drawString("Source Image", 140, 20);}		
							  bigvideo.drawString("Tracked Image", 475, 20);	
			//Warped Box
			if(bWarpImg)
			warp_box.draw( 0, 0);
		} 
		/*********************************
		* IF NOT CALIBRATING
		*********************************/
		if(!bCalibration)
		{
			//Draw main interface
			bShowInterface = true;

			//Display Application information in bottom right
			string str = "Tracker FPS:     ";
			str+= ofToString(ofGetFrameRate(), 0)+"\n\n";

			if(bcamera)
			{
				string str2 = "Camera Res:     ";
				str2+= ofToString(vidGrabber.width, 0) + " x " + ofToString(vidGrabber.height, 0)  + "\n";
				string str4 = "Camera FPS:     ";
				str4+= ofToString(fps, 0)+"\n";
				ofSetColor(0xFFFFFF);
				sidebarTXT.drawString(str + str2 + str4, 740, 410);
			}
			else
			{
				string str2 = "Video Res:       ";
				str2+= ofToString(vidPlayer.width, 0) + " x " + ofToString(vidPlayer.height, 0)  + "\n";
				string str4 = "Video FPS:       ";
				str4+= ofToString(fps, 0)+"\n";
				ofSetColor(0xFFFFFF);
				sidebarTXT.drawString(str + str2 + str4, 740, 410);
			}


			//Draw PINK CIRCLE 'ON' LIGHT
			ofSetColor(255, 0, 255);
			ofFill();		
			ofCircle(20, 10, 5);
			ofNoFill();

			if(bTUIOMode)
			{	//Draw Port and IP to screen
				ofSetColor(0xffffff);
				char buf[256];
				sprintf(buf, "Sending TUIO messages to:\nHost: %s\nPort: %i", myTUIO.localHost, myTUIO.TUIOPort);
				sidebarTXT.drawString(buf, 740, 480);

				//Draw GREEN CIRCLE 'ON' LIGHT
				ofSetColor(0x00FF00);
				ofFill();		
				ofCircle(35, 10, 5);
				ofNoFill();
			}
		}
		/*********************************
		* IF DRAWING BLOB OUTLINES
		*********************************/
		if(bShowInterface && !bFastMode)
		{
			//Find the blobs
			for(int i=0; i<contourFinder.nBlobs; i++)
			{
				//temp blob to rescale and draw on screen  
				ofxCvBlob drawBlob;
				drawBlob = contourFinder.blobs[i];

				if(bDrawOutlines)
				{
					//Get the contour (points) so they can be drawn
					for( int j=0; j<contourFinder.blobs[i].nPts; j++ )
					{
						drawBlob.pts[j].x = (320.0f/camWidth)  * (drawBlob.pts[j].x);
						drawBlob.pts[j].y = (240.0f/camHeight) * (drawBlob.pts[j].y);
					}
			
					//This adjusts the blob drawing for different cameras
					drawBlob.boundingRect.width  *= (320.0f/camWidth);
					drawBlob.boundingRect.height *= (240.0f/camHeight);
					drawBlob.boundingRect.x		 *= (320.0f/camWidth);;
					drawBlob.boundingRect.y		 *= (240.0f/camHeight);
					
					//Draw contours (outlines) on the tracked image
					//drawBlob.draw(385, 30);
					drawBlob.draw(40, 30);
				}

				//Show ID label;
				if(bShowLabels)
				{
					float xpos = drawBlob.centroid.x * (320.0f/camWidth);
					float ypos = drawBlob.centroid.y * (240.0f/camHeight);

					ofSetColor(0xCCFFCC);
					char idStr[1024];	
					sprintf(idStr, "id: %i",drawBlob.id);
					verdana.drawString(idStr, xpos + 365, ypos + drawBlob.boundingRect.height/2 + 45);			
				}
			}
			ofSetColor(0xFFFFFF);
		}

		if(!bCalibration)
			gui->draw();
	}
}


/******************************************************************************
 *							OTHER FUNCTIONS
 *****************************************************************************/


/**************************************************************** 
*	Load Settings from the config.xml file 
****************************************************************/ 
void testApp::loadXMLSettings(){

	// TODO: a seperate XML to map keyboard commands to action 
	message = "Loading config.xml...";
	// Can this load via http?
	if( XML.loadFile("config.xml")){
		message = "Settings Loaded!";
	}else{		
		//FAIL!
		message = "No Settings Found...";
	}

	//-------------------------------------------------------------- 
	//  START BINDING XML TO VARS
	//-------------------------------------------------------------- 
	//frameRate			= XML.getValue("CONFIG:APPLICATION:FRAMERATE",0);
	
	winWidth			= XML.getValue("CONFIG:WINDOW:WIDTH",0);
	winHeight			= XML.getValue("CONFIG:WINDOW:HEIGHT",0);
	minWidth			= XML.getValue("CONFIG:WINDOW:MINX",0);
	minHeight			= XML.getValue("CONFIG:WINDOW:MINY",0);

	bcamera				= XML.getValue("CONFIG:CAMERA_0:USECAMERA",0);
	deviceID			= XML.getValue("CONFIG:CAMERA_0:DEVICE",0);
	camWidth			= XML.getValue("CONFIG:CAMERA_0:WIDTH",0);
	camHeight			= XML.getValue("CONFIG:CAMERA_0:HEIGHT",0);
	camRate				= XML.getValue("CONFIG:CAMERA_0:FRAMERATE",0);

	bShowPressure		= XML.getValue("CONFIG:BOOLEAN:PRESSURE",0);

	bShowLabels			= XML.getValue("CONFIG:BOOLEAN:LABELS",0);
	bSnapshot			= XML.getValue("CONFIG:BOOLEAN:SNAPSHOT",0);
	bFastMode			= XML.getValue("CONFIG:BOOLEAN:FAST",0);	
	bDrawOutlines		= XML.getValue("CONFIG:BOOLEAN:OUTLINES",0);
	bLearnBakground		= XML.getValue("CONFIG:BOOLEAN:LEARNBG",0);
	bWarpImg			= XML.getValue("CONFIG:BOOLEAN:WARP",0);

	bVerticalMirror		= XML.getValue("CONFIG:BOOLEAN:VMIRROR",0);
	bHorizontalMirror	= XML.getValue("CONFIG:BOOLEAN:HMIRROR",0);

	//Filters
	bHighpass			= XML.getValue("CONFIG:BOOLEAN:HIGHPASS",1);
	bAmplify			= XML.getValue("CONFIG:BOOLEAN:AMPLIFY", 1);
	bSmooth				= XML.getValue("CONFIG:BOOLEAN:SMOOTH", 1);
	bDynamicBG			= XML.getValue("CONFIG:BOOLEAN:DYNAMICBG", 1);

	//GPU
	bGPUMode			= XML.getValue("CONFIG:BOOLEAN:GPU", 0);
	
	//Filter Settings
	threshold			= XML.getValue("CONFIG:INT:THRESHOLD",0);
	highpassBlur		= XML.getValue("CONFIG:INT:HIGHPASSBLUR",0);
	highpassNoise		= XML.getValue("CONFIG:INT:HIGHPASSNOISE",0);
	highpassAmp			= XML.getValue("CONFIG:INT:HIGHPASSAMP",0);
	smooth				= XML.getValue("CONFIG:INT:SMOOTH",0);

	//Warp
	handle1x			= XML.getValue("CONFIG:WARP:BOTTOMLEFT:X", 0);
	handle1y			= XML.getValue("CONFIG:WARP:BOTTOMLEFT:Y", 0);
	handle2x			= XML.getValue("CONFIG:WARP:BOTTOMRIGHT:X", 0);
	handle2y			= XML.getValue("CONFIG:WARP:BOTTOMRIGHT:Y", 0);
	handle3x			= XML.getValue("CONFIG:WARP:TOPRIGHT:X", 0);
	handle3y			= XML.getValue("CONFIG:WARP:TOPRIGHT:Y", 0);
	handle4x			= XML.getValue("CONFIG:WARP:TOPLEFT:X", 0);
	handle4y			= XML.getValue("CONFIG:WARP:TOPLEFT:Y", 0);
	
//--------------------------------------------------- TODO XML NETWORK SETTINGS	
	bTUIOMode			= XML.getValue("CONFIG:BOOLEAN:TUIO",0);
	tmpLocalHost		= XML.getValue("CONFIG:NETWORK:LOCALHOST", "localhost");
	tmpPort				= XML.getValue("CONFIG:NETWORK:TUIOPORT_OUT", 3333);
	myTUIO.setup(tmpLocalHost.c_str(), tmpPort); //have to convert tmpLocalHost to a const char*
//-------------------------------------------------------------- 
//  END XML SETUP
}



/******************************
*	BACKGROUND SUBTRACTION						
*******************************/
void testApp::learnBackground( ofxCvGrayscaleImage & _giLive, ofxCvGrayscaleImage & _grayBg, ofxCvFloatImage & _fiLearn, float _fLearnRate )
 {

	_fiLearn.addWeighted( _giLive, _fLearnRate);
    
    _grayBg = _fiLearn;

}
void testApp::bgCapture( ofxCvGrayscaleImage & _giLive )
{
	learnBackground( _giLive, grayBg, fiLearn, 1.0f);       
}
//Background Subtraction


/******************************
*		  CALIBRATION						
*******************************/
void testApp::doCalibration(){

	//ofSetFullscreen(bFullscreen);
	
	//Change the background color when a finger presses down/up		
	ofSetColor(0x000000);
	ofFill();	
	ofRect(0, 0, ofGetWidth(), ofGetHeight());	

	//Get the screen points so we can make a grid 
	vector2df *screenpts = calibrate.getScreenPoints();
	
	int i;
	//For each grid point
	for(i=0; i<calibrate.GRID_POINTS; i++)
	{
		//If calibrating, draw a red circle around the active point
		if(calibrate.calibrationStep == i && calibrate.bCalibrating){
		
			glPushMatrix();
			glTranslatef(screenpts[i].X * ofGetWidth(), screenpts[i].Y * ofGetHeight(), 0.0f);			
			//draw Circle
			ofFill();
			ofSetColor(downColor);
			ofCircle(0.f, 0.f, 40);
			//Cutout Middle of circle
			ofSetColor(0x000000);
			ofCircle(0.f, 0.f, 25);			
			glPopMatrix();		
		}

		//Make Plus Sign
		ofSetColor(0x00FF00);
		//Horizontal Plus
		ofRect(screenpts[i].X * ofGetWidth() - 2, screenpts[i].Y * ofGetHeight() - 10, 4, 20);
		//Vertical Plus
		ofRect(screenpts[i].X * ofGetWidth() - 10, screenpts[i].Y * ofGetHeight() - 2, 20, 4);
	}

	//Draw Bounding Box
	ofSetColor(0xFFFFFF);
	ofNoFill();
	ofRect(calibrate.screenBB.upperLeftCorner.X * ofGetWidth(), calibrate.screenBB.upperLeftCorner.Y * ofGetHeight(), 
		   calibrate.screenBB.getWidth() * ofGetWidth(), calibrate.screenBB.getHeight() * ofGetHeight());

	
	//Draw blobs in calibration to see where you are touching
	if(!calibrate.bCalibrating){

		//Find the blobs
		for(int i=0; i<contourFinder.nBlobs; i++)
		{
			//temp blob to rescale and draw on screen
			ofxCvBlob drawBlob2;
			drawBlob2 = contourFinder.blobs[i];

			//transform height/width to calibrated space
			calibrate.transformDimension(drawBlob2.boundingRect.width, drawBlob2.boundingRect.height); 
			drawBlob2.boundingRect.width *= calibrate.screenBB.getWidth() * ofGetWidth();
			drawBlob2.boundingRect.height *= calibrate.screenBB.getHeight() * ofGetHeight() ;

			//transform x/y position to calibrated space
			calibrate.cameraToScreenSpace(drawBlob2.centroid.x, drawBlob2.centroid.y);

			//Get a random color for each blob
			if(blobcolor[drawBlob2.id] == 0)
			{
				int r = ofRandom(0, 255);
				int g = ofRandom(0, 255);
				int b = ofRandom(0, 255);
				//Convert to hex
				int rgbNum = ((r & 0xff) << 16) | ((g & 0xff) << 8) | (b & 0xff);
				//Set hex into map position
				blobcolor[drawBlob2.id] = rgbNum;
			}

			//Draw Fuzzy Circles
			ofEnableAlphaBlending();
			ofImage tempFuzzy;
			tempFuzzy.clone(fuzzy);
			ofSetColor(blobcolor[drawBlob2.id]);
			tempFuzzy.draw(drawBlob2.centroid.x * ofGetWidth() - drawBlob2.boundingRect.width * .75, drawBlob2.centroid.y * ofGetHeight() - drawBlob2.boundingRect.height * .75, 
						   drawBlob2.boundingRect.width * 1.5, drawBlob2.boundingRect.height * 1.5);
			ofDisableAlphaBlending();

			//Draw Targets
			if(bShowTargets)
			{
				ofSetColor(0xFFFFFF);
				glLineWidth(3);
				glPushMatrix();
				glLoadIdentity();
				glTranslatef(drawBlob2.centroid.x * ofGetWidth(), ((drawBlob2.centroid.y * ofGetHeight()) * -1) + ofGetHeight(), 0);		
				ofEllipse(0, 0, drawBlob2.boundingRect.width/2, drawBlob2.boundingRect.height/2);
				ofLine(0, -drawBlob2.boundingRect.height/2, 0, drawBlob2.boundingRect.height/2); 
				ofLine(-drawBlob2.boundingRect.width/2, 0, drawBlob2.boundingRect.width/2, 0);				   
				glPopMatrix();
			}
			//Displat Text of blob information
			ofSetColor(0xFFFFFF);
			glLineWidth(1);
			char idStr[1024];	
			sprintf(idStr, "id: %i \n x: %f \n y: %f",drawBlob2.id, ceil(drawBlob2.centroid.x * ofGetWidth()), 
																	ceil(drawBlob2.centroid.y * ofGetHeight()));
			verdana.drawString(idStr, drawBlob2.centroid.x * ofGetWidth() - drawBlob2.boundingRect.width/2 + 20, 
									  drawBlob2.centroid.y * ofGetHeight() - drawBlob2.boundingRect.height/2 + 20);
			}
		}	

	/******************************
	* Calibration Instructions
	******************************/		
	ofSetColor(0xFF00FF);	
	ofSetWindowTitle("Calibration");		
	char reportStr[10240];	
	calibrationText.setLineHeight(20.0f);
	
	if(calibrate.bCalibrating){
		sprintf(reportStr, 
		"CALIBRATING: \n\n\-Touch current circle target and lift up to calibrate point \n\-Press [b] to recapture background (if there's false blobs) \n\-Press [r] to go back to previous point(s) \n");
		calibrationText.drawString(reportStr, ofGetWidth()/2 - calibrationText.stringWidth(reportStr)/2, ofGetHeight()/2 - calibrationText.stringHeight(reportStr)/2);
	}else
	{
		sprintf(reportStr,  
		"CALIBRATION \n\n\-Press [c] to start calibrating \n-Press [x] to return main screen \n-Press [b] to recapture background \n-Press [t] to toggle blob targets \n\n\CHANGING GRID SIZE (number of points): \n\n\-Current Grid Size is %i x %i \n\-Press [+]/[-] to add/remove points on X axis \n\-Press [shift][+]/[-] to add/remove points on Y axis \n\n\ALINGING BOUNDING BOX TO PROJECTION SCREEN: \n\n\-Use arrow keys to move bounding box\n\-Press and hold [w],[a],[s],[d] (top, left, bottom, right) and arrow keys to adjust each side\n", calibrate.GRID_X + 1, calibrate.GRID_Y + 1);
		calibrationText.drawString(reportStr, ofGetWidth()/2 - calibrationText.stringWidth(reportStr)/2, ofGetHeight()/2 - calibrationText.stringHeight(reportStr)/2);
	}
}

/*****************************************************************************
 * KEY EVENTS
 *****************************************************************************/
void testApp::keyPressed(int key)
{ 
	switch(key)
	{	
		case 'b':
			bLearnBakground = true;
			break;		
		case 'o':
			bDrawOutlines ? bDrawOutlines = false : bDrawOutlines = true;
			gui->update(appPtr->trackedPanel_outlines, kofxGui_Set_Bool, &appPtr->bDrawOutlines, sizeof(bool));
			break;
		case 'h':
			bHorizontalMirror ? bHorizontalMirror = false : bHorizontalMirror = true;
			gui->update(appPtr->propertiesPanel_flipH, kofxGui_Set_Bool, &appPtr->bHorizontalMirror, sizeof(bool));
			break;
		case 'j':
			bVerticalMirror ? bVerticalMirror = false : bVerticalMirror = true;
			gui->update(appPtr->propertiesPanel_flipV, kofxGui_Set_Bool, &appPtr->bVerticalMirror, sizeof(bool));
			break;
		case 't':
			if(!bCalibration && bTUIOMode)
			{
				bTUIOMode = false;	
				myTUIO.blobs.clear();
				gui->update(appPtr->optionPanel_tuio, kofxGui_Set_Bool, &appPtr->bTUIOMode, sizeof(bool));
			}
			else
			{	
				bTUIOMode = true;	
				gui->update(appPtr->optionPanel_tuio, kofxGui_Set_Bool, &appPtr->bTUIOMode, sizeof(bool));
			}
			bCalibration && bShowTargets ? bShowTargets = false : bShowTargets = true;
			break;	
		case 'g':
			bGPUMode ? bGPUMode = false : bGPUMode = true;
			gui->update(appPtr->gpuPanel_use, kofxGui_Set_Bool, &appPtr->bGPUMode, sizeof(bool));
			bLearnBakground = true;
			break;
		case 'v':
			if(bcamera)
				vidGrabber.videoSettings();
			break;	
		case 'l':
			bShowLabels ? bShowLabels = false : bShowLabels = true;
			gui->update(appPtr->trackedPanel_ids, kofxGui_Set_Bool, &appPtr->bShowLabels, sizeof(bool));
			break;
		case 'p':
			//bShowPressure ? bShowPressure = false : bShowPressure = true;
			break;
		case ' ':
			if(bFastMode)
			{	
				bFastMode = false;	
				//bToggleHelp = true;
				ofSetWindowShape(950,600); //default size
				ofSetWindowTitle("Configuration");
			}
			else
			{	
				bFastMode = true;
				//bToggleHelp = false;
				ofSetWindowShape(175,18); //minimized size
				ofSetWindowTitle("Mini");
			}
			break;
		/***********************
		* Keys for Calibration
		***********************/
		case 'c': //Enter/Exit Calibration
			if(!bCalibration){
				bCalibration = true;
				bFullscreen = true;
			}
			else if(bCalibration && calibrate.bCalibrating)
				calibrate.bCalibrating = false;
			else if(bCalibration)
				calibrate.beginCalibration();
			break;
		case 'x': //Begin Calibrating
			if(bCalibration){
				bCalibration = false;
				bFullscreen = false;
			}
		case 'r': //Revert Calibration
			if(calibrate.bCalibrating)calibrate.revertCalibrationStep();
		case 'w': //Up
			bW = true;
			if(!bCalibration){
				bWarpImg ? bWarpImg = false : bWarpImg = true;
				gui->update(appPtr->calibrationPanel_warp, kofxGui_Set_Bool, &appPtr->bWarpImg, sizeof(bool));
			}
			break;	
		case 's': //Down
			bS = true;
			if(!bCalibration) saveConfiguration();
			break;
		case 'a': //Left
			bA = true;
			break;
		case 'd': //Right
			bD = true;
			break;
		case OF_KEY_RIGHT: //Move bounding box right
			if(bD){
				calibrate.screenBB.lowerRightCorner.X += .001;
				if(calibrate.screenBB.lowerRightCorner.X > 1) calibrate.screenBB.lowerRightCorner.X = 1;
				calibrate.setGrid(calibrate.GRID_X, calibrate.GRID_Y);
				calibrate.calibrationStep = 0;
			}else if(bA){
				calibrate.screenBB.upperLeftCorner.X += .001;
				if(calibrate.screenBB.upperLeftCorner.X > 1) calibrate.screenBB.upperLeftCorner.X = 1;
				calibrate.setGrid(calibrate.GRID_X, calibrate.GRID_Y);
				calibrate.calibrationStep = 0;
			}else{
				calibrate.screenBB.lowerRightCorner.X += .001;
				if(calibrate.screenBB.lowerRightCorner.X > 1) calibrate.screenBB.lowerRightCorner.X = 1;
				calibrate.screenBB.upperLeftCorner.X += .001;
				if(calibrate.screenBB.upperLeftCorner.X > 1) calibrate.screenBB.upperLeftCorner.X = 1;
				calibrate.setGrid(calibrate.GRID_X, calibrate.GRID_Y);
				calibrate.calibrationStep = 0;
			}
			break;
		case OF_KEY_LEFT: //Move bounding box left
			if(bD){
				calibrate.screenBB.lowerRightCorner.X -= .001;
				if(calibrate.screenBB.lowerRightCorner.X < 0) calibrate.screenBB.lowerRightCorner.X = 0;
				calibrate.setGrid(calibrate.GRID_X, calibrate.GRID_Y);
				calibrate.calibrationStep = 0;
			}else if(bA){
				calibrate.screenBB.upperLeftCorner.X -= .001;
				if(calibrate.screenBB.upperLeftCorner.X < 0) calibrate.screenBB.upperLeftCorner.X = 0;
				calibrate.setGrid(calibrate.GRID_X, calibrate.GRID_Y);
				calibrate.calibrationStep = 0;
			}else{
				calibrate.screenBB.lowerRightCorner.X -= .001;
				if(calibrate.screenBB.lowerRightCorner.X < 0) calibrate.screenBB.lowerRightCorner.X = 0;
				calibrate.screenBB.upperLeftCorner.X -= .001;
				if(calibrate.screenBB.upperLeftCorner.X < 0) calibrate.screenBB.upperLeftCorner.X = 0;
				calibrate.setGrid(calibrate.GRID_X, calibrate.GRID_Y);
				calibrate.calibrationStep = 0;
			}
			break;
		case OF_KEY_DOWN: //Move bounding box down
			if(bS){
				calibrate.screenBB.lowerRightCorner.Y += .001;
				if(calibrate.screenBB.lowerRightCorner.Y > 1) calibrate.screenBB.lowerRightCorner.Y = 1;
				calibrate.setGrid(calibrate.GRID_X, calibrate.GRID_Y);
				calibrate.calibrationStep = 0;
			}else if(bW){
				calibrate.screenBB.upperLeftCorner.Y += .001;
				if(calibrate.screenBB.upperLeftCorner.Y > 1) calibrate.screenBB.upperLeftCorner.Y = 1;
				calibrate.setGrid(calibrate.GRID_X, calibrate.GRID_Y);
				calibrate.calibrationStep = 0;
			}else{
				calibrate.screenBB.lowerRightCorner.Y += .001;
				if(calibrate.screenBB.lowerRightCorner.Y > 1) calibrate.screenBB.lowerRightCorner.Y = 1;
				calibrate.screenBB.upperLeftCorner.Y += .001;
				if(calibrate.screenBB.upperLeftCorner.Y > 1) calibrate.screenBB.upperLeftCorner.Y = 1;
				calibrate.setGrid(calibrate.GRID_X, calibrate.GRID_Y);
				calibrate.calibrationStep = 0;
			}
			break;
		case OF_KEY_UP: //Move bounding box up
			if(bS){
				calibrate.screenBB.lowerRightCorner.Y -= .001;
				if(calibrate.screenBB.lowerRightCorner.Y < 0) calibrate.screenBB.lowerRightCorner.Y = 0;
				calibrate.setGrid(calibrate.GRID_X, calibrate.GRID_Y);
				calibrate.calibrationStep = 0;
			}else if(bW){
				calibrate.screenBB.upperLeftCorner.Y -= .001;
				if(calibrate.screenBB.upperLeftCorner.Y < 0) calibrate.screenBB.upperLeftCorner.Y = 0;
				calibrate.setGrid(calibrate.GRID_X, calibrate.GRID_Y);
				calibrate.calibrationStep = 0;
			}else{
				calibrate.screenBB.lowerRightCorner.Y -= .001;
				if(calibrate.screenBB.lowerRightCorner.Y < 0) calibrate.screenBB.lowerRightCorner.Y = 0;
				calibrate.screenBB.upperLeftCorner.Y -= .001;
				if(calibrate.screenBB.upperLeftCorner.Y < 0) calibrate.screenBB.upperLeftCorner.Y = 0;
				calibrate.setGrid(calibrate.GRID_X, calibrate.GRID_Y);
				calibrate.calibrationStep = 0;
			}
			break;
		//Start Grid Point Changing
		case '=':
			if(bCalibration)
			calibrate.GRID_X ++;
			if(calibrate.GRID_X > 16) calibrate.GRID_X = 16; calibrate.setGrid(calibrate.GRID_X, calibrate.GRID_Y);
			calibrate.calibrationStep = 0;
			break;
		case '-':
			if(bCalibration)
			calibrate.GRID_X --;
			if(calibrate.GRID_X < 1) calibrate.GRID_X = 1; calibrate.setGrid(calibrate.GRID_X, calibrate.GRID_Y);
			calibrate.calibrationStep = 0;
			break;
		case '+':
			if(bCalibration)
			calibrate.GRID_Y ++;
			if(calibrate.GRID_Y > 16) calibrate.GRID_Y = 16; calibrate.setGrid(calibrate.GRID_X, calibrate.GRID_Y);
			calibrate.calibrationStep = 0;
			break;
		case '_':
			if(bCalibration)
			calibrate.GRID_Y --;
			if(calibrate.GRID_Y < 1) calibrate.GRID_Y = 1; calibrate.setGrid(calibrate.GRID_X, calibrate.GRID_Y);
			calibrate.calibrationStep = 0;
			break;
	}
}

void testApp::keyReleased(int key){

	switch(key)
	{	
		case 'w':
			bW = false;
			break;	
		case 's':
			bS = false;
			break;
		case 'a':
			bA = false;
			break;
		case 'd':
			bD = false;
			break;
	}
}


/*****************************************************************************
*								MOUSE EVENTS
*****************************************************************************/
void testApp::mouseMoved(int x, int y)
{
}	

void testApp::mouseDragged(int x, int y, int button)
{
	//Warp Box
	if(bWarpImg && x < (40 + 320) && x > 40)
	{
		if(y < (30 + 240) && y > 30)
		  warp_box.adjustHandle(x,y);		
	}		
	//gui listener
	gui->mouseDragged(x, y, button);
}

void testApp::mousePressed(int x, int y, int button)
{	//gui listener
	gui->mousePressed(x, y, button);	
}

void testApp::mouseReleased()
{	//guilistener
	gui->mouseReleased(mouseX, mouseY, 0);	
}


/*****************************************************************************
*								TOUCH EVENTS
*****************************************************************************/
void testApp::blobOn( ofxCvBlob b)
{
	//printf("Blob DOWN %i \n", b.id); 

	if(bCalibration && contourFinder.nBlobs == 1)//If calibrating change target color when a finger is down
	downColor = 0x2FB5FF; 
		
	if(bTUIOMode)//If sending TUIO, add the blob to the map list
	{
		calibrate.cameraToScreenSpace(b.centroid.x, b.centroid.y);		
		//if blob is not otuside calibration mesh
		if(b.centroid.x != 0 && b.centroid.y != 0)
		myTUIO.blobs[b.id] = b;
	}
}

void testApp::blobMoved( ofxCvBlob b) 
{	
	if(bTUIOMode)//If sending TUIO, add the blob to the map list
	{
		calibrate.cameraToScreenSpace(b.centroid.x, b.centroid.y);		
		//if blob is not otuside calibration mesh
		if(b.centroid.x != 0 && b.centroid.y != 0)
		myTUIO.blobs[b.id] = b;
	}
}  

void testApp::blobOff( ofxCvBlob b) 
{
	//printf("Blob UP %i \n", b.id);
	
	if(bCalibration)
	downColor = 0xFF0000;

	if(calibrate.bCalibrating && contourFinder.nBlobs == 0)//If Calibrating, register the calibration point on blobOff
	{	
		calibrate.cameraPoints[calibrate.calibrationStep] = vector2df(b.centroid.x, b.centroid.y);
		calibrate.nextCalibrationStep();
		
		printf("%d (%f, %f)\n", calibrate.calibrationStep, b.centroid.x, b.centroid.y);
	}

	if(bTUIOMode)//If sending TUIO and blob is not outside calibration mesh, Delete Blobs from map list
	{
		std::map<int, ofxCvBlob>::iterator iter;
		for(iter = myTUIO.blobs.begin(); iter != myTUIO.blobs.end(); iter++)
		{
			if(iter->second.id == b.id)
			{
				myTUIO.blobs.erase(iter);

				break;
			}
		}
	}
}



/*****************************************************************************
* ON EXIT
*****************************************************************************/
void testApp::exit()
{
	//TODO: SEND ESC KEY TO KEEP FROM CRASHING -  can we emulate a keyboard
	//event to trick the app into closing properly?
	
	
	// -------------------------------- SAVE STATE ON EXIT
	saveConfiguration();
	calibrate.saveCalibration();


	printf("tBeta module has exited!\n");	
}

void testApp::saveConfiguration()
{

	XML.setValue("CONFIG:BOOLEAN:PRESSURE",bShowPressure);
	XML.setValue("CONFIG:BOOLEAN:LABELS",bShowLabels);
	XML.setValue("CONFIG:BOOLEAN:SNAPSHOT",bSnapshot);
	XML.setValue("CONFIG:BOOLEAN:OUTLINES",bDrawOutlines);
	XML.setValue("CONFIG:BOOLEAN:LEARNBG",bLearnBakground);
	XML.setValue("CONFIG:BOOLEAN:TUIO",bTUIOMode);
	XML.setValue("CONFIG:BOOLEAN:VMIRROR",bVerticalMirror);
	XML.setValue("CONFIG:BOOLEAN:HMIRROR",bHorizontalMirror);
	XML.setValue("CONFIG:BOOLEAN:WARP", bWarpImg);

	XML.setValue("CONFIG:BOOLEAN:HIGHPASS", bHighpass);
	XML.setValue("CONFIG:BOOLEAN:AMPLIFY", bAmplify);
	XML.setValue("CONFIG:BOOLEAN:SMOOTH", bSmooth);
	XML.setValue("CONFIG:BOOLEAN:DYNAMICBG", bDynamicBG);
	
	XML.setValue("CONFIG:BOOLEAN:GPU", bGPUMode);

	XML.setValue("CONFIG:INT:THRESHOLD", threshold);
	XML.setValue("CONFIG:INT:HIGHPASSBLUR", highpassBlur);
	XML.setValue("CONFIG:INT:HIGHPASSNOISE",highpassNoise);
	XML.setValue("CONFIG:INT:HIGHPASSAMP",highpassAmp);
	XML.setValue("CONFIG:INT:SMOOTH", smooth);

	XML.setValue("CONFIG:CAMERA_0:USECAMERA", bcamera);
	XML.setValue("CONFIG:CAMERA_0:DEVICE", deviceID);
	XML.setValue("CONFIG:CAMERA_0:WIDTH", camWidth);
	XML.setValue("CONFIG:CAMERA_0:HEIGHT", camHeight);
	XML.setValue("CONFIG:CAMERA_0:FRAMERATE", camRate);

	//WARP
	XML.setValue("CONFIG:WARP:BOTTOMLEFT:X", warp_box.getBottomLeftX());
	XML.setValue("CONFIG:WARP:BOTTOMLEFT:Y", warp_box.getBottomLeftY());
	XML.setValue("CONFIG:WARP:BOTTOMRIGHT:X", warp_box.getBottomRightX());
	XML.setValue("CONFIG:WARP:BOTTOMRIGHT:Y", warp_box.getBottomRightY());
	XML.setValue("CONFIG:WARP:TOPRIGHT:X", warp_box.getTopRightX());
	XML.setValue("CONFIG:WARP:TOPRIGHT:Y", warp_box.getTopRightY());
	XML.setValue("CONFIG:WARP:TOPLEFT:X", warp_box.getTopLeftX());
	XML.setValue("CONFIG:WARP:TOPLEFT:Y", warp_box.getTopLeftY());	
	

//	XML.setValue("CONFIG:NETWORK:LOCALHOST", *myTUIO.localHost);
//	XML.setValue("CONFIG:NETWORK:TUIO_PORT_OUT",myTUIO.TUIOPort);

	XML.saveFile("config.xml");
}
