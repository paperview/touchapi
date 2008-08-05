#include "testApp.h"


/******************************************************************************
 * The setup function is run once to perform initializations in the application
 *****************************************************************************/
void testApp::setup()
{	
	/********************
	* Initalize Variables
	*********************/

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
	bW			= false;
	bA			= false;
	bS			= false;
	bD			= false;

	bDrawVideo = true;

	bFullscreen = false;

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
	
	grayBg.allocate(camWidth, camHeight);		//Background Image
	grayBg.setUseTexture(false);

	fiLearn.allocate(camWidth, camHeight);		//ofxFloatImage used for simple dynamic background subtracti
	fiLearn.setUseTexture(false);

	
	//For camera warp
	giWarped.allocate(camWidth, camHeight);     //Warped Image (used for warped calibration)
	giWarped.setUseTexture(false);
	/********************************************************************************************************/




	/**********************************************************/
	//GPU stuff initialization
	/**********************************************************/
	glGenTextures(1, &gpuSourceTex);
	glGenTextures(1, &gpuBGTex);
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
	calibrationText.loadFont("verdana.ttf", 11, true, true);
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

	printf("Touchlib application is setup!\n");
}




void testApp::grabFrameToGPU(GLuint target){
	//grab the frame to a raw openGL texture
	if(bcamera)
	{
		glEnable(GL_TEXTURE_2D);
		//glPixelStorei(1);
		glBindTexture(GL_TEXTURE_2D, target);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, camWidth, camHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, vidGrabber.getPixels());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D,0);


	}
	else{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, target);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, camWidth, camHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, vidPlayer.getPixels());
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
	
	int totalPixels = camWidth*camHeight*3;
	unsigned char * pixels = vidGrabber.getPixels();
}


void testApp::applyGPUImageFilters(){

	if (bLearnBakground == true){
		grabFrameToGPU(gpuBGTex);
		bLearnBakground = false;
	}
	//GLuint tmp;
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

	if(bSmooth){
		processedImg.blur((smooth * 2) + 1); //needs to be an odd number
	}

	//HighPass
	if(bHighpass){
		processedImg.highpass(highpassBlur, highpassNoise);
	}

	//Amplify
	if(bAmplify){
		processedImg.amplify(processedImg, highpassAmp);
	}
	
	//Set a threshold value
	processedImg.threshold(threshold);
}




/******************************************************************************
 * The update function runs continuously. Use it to update states and variables
 *****************************************************************************/
void testApp::update()
{	
	ofBackground(110, 110, 110);

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
			//Calculate FPS of Camera
			frames++;
			float time = ofGetElapsedTimeMillis();
			if(time > (lastFPSlog + 1000)){		
				fps = frames;
				frames = 0;
				lastFPSlog = time;			
			}//End calculation

			//Find contours/blobs
			if(bGPUMode){
				grabFrameToGPU(gpuSourceTex);
				applyGPUImageFilters();
				contourFinder.findContours(gpuReadBackImageGS, 1, (camWidth*camHeight)/25, 10, false);
			}
			else{
				grabFrame();
				applyImageFilters();
				contourFinder.findContours(processedImg, 1, (camWidth*camHeight)/25, 10, false);
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

				fLearnRate = 0.0008f;
			}//End Background Learning rate



			/************************************************
			* TUIO MODE
			***************************************************/
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

	/*********************************
	* IF NOT CALIBRATING
	*********************************/
	if(!bCalibration)
	{
		//Draw main interface
		bShowInterface = true;

		//Display applicaion and camera FPS in title 
		string str = "Application: ";
		str+= ofToString(ofGetFrameRate(), 2)+"fps \n";	
		string str2 = "Camera:   ";
		str2+= ofToString(fps, 1)+"fps";

		ofSetColor(0xFFFFFF);
		sidebarTXT.drawString(str + str2, 740, 410);	
		
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
			sidebarTXT.drawString(buf, 740, 450);

			//Draw GREEN CIRCLE 'ON' LIGHT
			ofSetColor(0x00FF00);
			ofFill();		
			ofCircle(35, 10, 5);
			ofNoFill();
		}
	}
		ofSetColor(0xFFFFFF);
	
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
		case 't':
			if(bTUIOMode)
			{
				bTUIOMode = false;	
				myTUIO.blobs.clear();
				//ofSetWindowShape(700,600);
			}
			else
			{	
				bTUIOMode = true;	
				//ofSetWindowShape(950,700);
			}
			break;	
		case 'v':
			if(bcamera)
				vidGrabber.videoSettings();
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
}

void testApp::mousePressed(int x, int y, int button)
{		
}

/*****************************************************************************
*								TOUCH EVENTS
*****************************************************************************/
void testApp::blobOn( ofxCvBlob b)
{
	//printf("Blob DOWN %i \n", b.id); 

	if(bCalibration)//If calibrating change target color when a finger is down
	downColor = 0x2FB5FF; 
		
	if(bTUIOMode)//If sending TUIO, add the blob to the map list
	{
		calibrate.cameraToScreenSpace(b.centroid.x, b.centroid.y);
		myTUIO.blobs[b.id] = b;
	}
}

void testApp::blobMoved( ofxCvBlob b) 
{	
	if(bTUIOMode)//If sending TUIO, update the move information for the blob
	{
		calibrate.cameraToScreenSpace(b.centroid.x, b.centroid.y);
		myTUIO.blobs[b.id] = b;
	}
}  

void testApp::blobOff( ofxCvBlob b) 
{
	//printf("Blob UP %i \n", b.id);

	if(bCalibration)
	downColor = 0xFF0000;
	
	if(calibrate.bCalibrating)//If Calibrating, register the calibration point on blobOff
	{			
		calibrate.cameraPoints[calibrate.calibrationStep] = vector2df(b.centroid.x, b.centroid.y);
		calibrate.nextCalibrationStep();
		
		printf("%d (%f, %f)\n", calibrate.calibrationStep, b.centroid.x, b.centroid.y);
	}

	if(bTUIOMode)//If sending TUIO, Delete Blobs from map list
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

	printf("tBeta module has exited!\n");	
}