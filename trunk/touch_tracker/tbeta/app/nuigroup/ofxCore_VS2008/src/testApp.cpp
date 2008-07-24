#include "testApp.h"
#include "uiDefinition.h"


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
	bW			= false;
	bA			= false;
	bS			= false;
	bD			= false;

	//Load Settings from config.xml file 
	loadXMLSettings();
	myTUIO.setup();

	//Load Calibration Settings from calibration.xml file
	calibrate.setCamRes(camWidth, camHeight);
	calibrate.loadXMLSettings();

	//Setup Window Properties 
	ofSetWindowShape(winWidth,winHeight);
	ofSetFrameRate(30);			//This will be based on camera fps in the future		
	ofSetVerticalSync(false);	//Set vertical sync to false for better performance



	#ifdef _USE_LIVE_VIDEO // MAKE BOTH LIVE VIDEO AND VCR MODE WORK AT SAME TIME 
       		
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
	#else

		activeIput = true;	

        //vidPlayer.loadMovie("test_videos/FrontDI.m4v");
		//vidPlayer.loadMovie("test_videos/HCI_FTIR.mov");
		vidPlayer.loadMovie("test_videos/raw.mp4");
        vidPlayer.play();	
		printf("Video Mode\n");
		camHeight = vidPlayer.height;
		camWidth = vidPlayer.width;		
	#endif
    
	/*****************************************************************************************************
	* Allocate images (needed for drawing/processing images) ----Most of These won't be needed in the end
	******************************************************************************************************/
	sourceImg.allocate(camWidth, camHeight);    //Source Image
	grayImg.allocate(camWidth, camHeight);		//Gray Image
	grayBg.allocate(camWidth, camHeight);		//Background Image
	subtractBg.allocate(camWidth, camHeight);   //Background After subtraction
	grayDiff.allocate(camWidth, camHeight);		//Difference Image between Background and Source
	highpassImg.allocate(camWidth, camHeight);  //Highpass Image
	giWarped.allocate(camWidth, camHeight);     //Warped Image (used for warped calibration)

	fiLearn.allocate(camWidth, camHeight);		//ofxFloatImage used for simple dynamic background subtracti
//	fiLearn.setUseTexture(false);

	pressureMap.allocate(camWidth, camHeight);	//Pressure Map Image
	/********************************************************************************************************/

	//Fonts - Is there a way to dynamically change font size?
	verdana.loadFont("verdana.ttf", 8, true, true);	   //Font used for small images
	calibrationText.loadFont("verdana.ttf", 11, true, true);
	bigvideo.loadFont("verdana.ttf", 13, true, true);  //Font used for big images.
	
	//Static Images
	logo.loadImage("images/logo.jpg");
	background.loadImage("images/background.jpg"); //Main (Temp?) Background

	//Setup green warped box
	m_box.setup( 40, 146, 320, 240); 

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



	gui	= ofxGui::Instance(this);


	//if(!gui->buildFromXml(OFXGUI_XML))
	//{	
		//ofxGuiPanel* panel4 = gui->addPanel(kParameter_Panel4, "new", 155, 10, OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING);
		//panel4->addKnob(kParameter_Distance, "distance", 60, 60, 10.0f, 100.0f, distance, kofxGui_Display_Float2, 0);
		//panel4->addKnob(kParameter_Size, "threshold", 60, 60, 0.0f, 500.0f, threshold, kofxGui_Display_Float2, 0);


/*		srcPanel->mObjHeight = 100;
		srcPanel->mObjWidth = 320;
		srcPanel->mBorder = 10;
		srcPanel->mCtrX = 40;
		srcPanel->mCtrHeight = 100;
		srcPanel->mBorder = 100;
		srcPanel->mCtrX = 50;
*/

		gui->mGlobals->mBorderColor.r = 0;
		gui->mGlobals->mBorderColor.g = 0;
		gui->mGlobals->mBorderColor.b = 0;
		gui->mGlobals->mBorderColor.a = .5;

		gui->mGlobals->mCoverColor.r = 140;
		gui->mGlobals->mCoverColor.g = 40;
		gui->mGlobals->mCoverColor.b = 40;
		gui->mGlobals->mCoverColor.a = .5;

		gui->mGlobals->mFrameColor.r = 0;
		gui->mGlobals->mFrameColor.g = 0;
		gui->mGlobals->mFrameColor.b = 0;
		gui->mGlobals->mFrameColor.a = .2;
		
		gui->mGlobals->mTextColor.r = 0;
		gui->mGlobals->mTextColor.g = 0;
		gui->mGlobals->mTextColor.b = 0;
		gui->mGlobals->mTextColor.a = 1;

		gui->mGlobals->mButtonColor.r = 120;
		gui->mGlobals->mButtonColor.g = 0;
		gui->mGlobals->mButtonColor.b = 0;
		gui->mGlobals->mButtonColor.a = 1;

		gui->mGlobals->mCurveColor.r = 110;
		gui->mGlobals->mCurveColor.g = 0;
		gui->mGlobals->mCurveColor.b = 0;
		gui->mGlobals->mCurveColor.a = 255;


		ofxGuiPanel* propPanel = gui->addPanel(propertiesPanel, "Source Properties", 730, 60, 12, OFXGUI_PANEL_SPACING);
		propPanel->addButton(propertiesPanel_flipV, "Flip Vertical (v)", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, kofxGui_Button_Off, kofxGui_Button_Switch);
		propPanel->addButton(propertiesPanel_flipH, "Flip Horizontal (h)", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, kofxGui_Button_Off, kofxGui_Button_Switch);
		propPanel->addButton(propertiesPanel_pressure, "Pressure Map (p)", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, kofxGui_Button_Off, kofxGui_Button_Switch);

		ofxGuiPanel* oPanel = gui->addPanel(optionPanel, "Tracking Options", 730, 163, OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING);
		oPanel->addButton(optionPanel_tuio, "Send TUIO (t)", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, kofxGui_Button_Off, kofxGui_Button_Switch);
		oPanel->addButton(optionPanel_draw, "Draw Windows (d)", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, kofxGui_Button_Off, kofxGui_Button_Switch);


/*
	bSnapshot			= XML.getValue("CONFIG:BOOLEAN:SNAPSHOT",0);
	bFastMode			= XML.getValue("CONFIG:BOOLEAN:FAST",0);	
	bCalibration		= XML.getValue("CONFIG:BOOLEAN:CALIBRATION",0);
*/


	//	ofxGuiPanel* panel2 = gui->addPanel(kParameter_Panel2, "files", 800, 380, OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING);
		//panel2->addFiles(kParameter_File, "files", 110, OFXGUI_FILES_HEIGHT, "", "", "xml");
	//	panel2->addButton(kParameter_SaveXml, "saveToXml", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, kofxGui_Button_Off, kofxGui_Button_Trigger);

		//Tracked Image
		ofxGuiPanel* trackPanel = gui->addPanel(trackedPanel, "Tracked Image", 386, 270, OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING);
		trackPanel->addButton(trackedPanel_outlines, "Show Outlines (o)", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, kofxGui_Button_Off, kofxGui_Button_Switch);
		trackPanel->addButton(trackedPanel_ids, "Show IDs (i)", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, kofxGui_Button_Off, kofxGui_Button_Switch);
		trackPanel->addSlider(trackedPanel_threshold, "Threshold (a/z)", 300, 13, 0.0f, 255.0f, threshold, kofxGui_Display_Int, 0);
		trackPanel->mObjHeight = 105;
		trackPanel->mObjWidth = 319;

		//Source Image
		ofxGuiPanel* srcPanel = gui->addPanel(sourcePanel, "Source Image", 41, 270, OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING);
		srcPanel->addButton(sourcePanel_previousCam, "Previous Camera", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, kofxGui_Button_Off, kofxGui_Button_Trigger);
		srcPanel->addButton(sourcePanel_nextCam, "Next Camera", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, kofxGui_Button_Off, kofxGui_Button_Trigger);
		srcPanel->mObjHeight = 105;
		srcPanel->mObjWidth = 319;

		//Background Image
		ofxGuiPanel* bkPanel = gui->addPanel(backgroundPanel, "Background", 236, 487, 10, 7);
		bkPanel->addButton(backgroundPanel_remove, "Remove (b)\nBackground", 10, 10, kofxGui_Button_Off, kofxGui_Button_Trigger);
		bkPanel->mObjWidth = 127;

		//Highpass Image
		ofxGuiPanel* hpPanel = gui->addPanel(highpassPanel, "Highpass", 386, 487, OFXGUI_PANEL_BORDER, 7);
		hpPanel->addSlider(highpassPanel_blur, "Blur", 110, 13, 0.0f, 200.0f, highpassBlur, kofxGui_Display_Int, 0);
		hpPanel->addSlider(highpassPanel_noise, "Noise", 110, 13, 0.0f, 30.0f, highpassNoise, kofxGui_Display_Int, 0);
		hpPanel->mObjWidth = 127;

		//Amplify Image
		ofxGuiPanel* ampPanel = gui->addPanel(amplifyPanel, "Amplify", 536, 487, OFXGUI_PANEL_BORDER, 7);
		ampPanel->addSlider(amplifyPanel_amp, "Amplify", 110, 13, 0.0f, 300.0f, highpassAmp, kofxGui_Display_Int, 0);
		ampPanel->mObjWidth = 127;

		//do update while inactive
		gui->forceUpdate(false);	
		gui->activate(true);

	//}
}


/******************************************************************************
 * The update function runs continuously. Use it to update states and variables
 *****************************************************************************/
void testApp::update()
{	
	

	ofBackground(0, 0, 0);

    bNewFrame = false;
		
	if(activeInput){

	#ifdef _USE_LIVE_VIDEO
       vidGrabber.grabFrame();
	   bNewFrame = vidGrabber.isFrameNew();
    #else
        vidPlayer.idleMovie();
        bNewFrame = vidPlayer.isFrameNew();
	#endif
	
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

		//Set sourceImg as new camera/video frame
		#ifdef _USE_LIVE_VIDEO
		  sourceImg.setFromPixels(vidGrabber.getPixels(), camWidth, camHeight);
		  int totalPixels = camWidth*camHeight*3;
		  unsigned char * pixels = vidGrabber.getPixels();
		
	    #else
            sourceImg.setFromPixels(vidPlayer.getPixels(), 
									camWidth, camHeight);
			int totalPixels = camWidth*camHeight*3;
		    unsigned char * pixels = vidPlayer.getPixels();
        #endif	
		
			
		/************************************************
		*				SET FILTERS HERE
		************************************************/
		//Set Mirroring Horizontal/Vertical
		sourceImg.mirror(bVerticalMirror, bHorizontalMirror);

		//sourceImg.warpIntoMe(sourceImg, m_box.fHandles, dstPts );

		subtractBg = sourceImg;

		subtractBg.blur(3);

/*		if(warpImg){
			giWarped.warpIntoMe(subtractBg, m_box.fHandles, dstPts );
			subtractBg = giWarped;
		}|
*/
		//Dynamic background with learn rate
		learnBackground( subtractBg, grayBg, fiLearn, fLearnRate);
		
		//Capture full background
		if (bLearnBakground == true)
		{
			bgCapture( subtractBg );
			bLearnBakground = false;
		}

		//Background Subtraction
		subtractBg.absDiff(grayBg, subtractBg);

		//HighPass
		highpassImg = subtractBg;
		highpassImg.highpass(highpassBlur, highpassNoise);

		//Amplify
		grayImg = highpassImg;		
		grayImg.amplify(grayImg, highpassAmp);
		
		grayDiff = grayImg;

		//Set a threshold value
		grayDiff.threshold(threshold);

		//Find contours/blobs
		contourFinder.findContours(grayDiff, 1, (camWidth*camHeight)/25, 10, false);

		//Track found contours/blobs
		tracker.track(&contourFinder);

		/**************************************************
		* Background subtraction LearRate
		* If there are no blobs, add the background faster.
		* If there ARE blobs, add the background slower.
		***************************************************/
		fLearnRate = 0.01f;
		
		if(contourFinder.nBlobs > 1){

			fLearnRate = 0.0008f;
		}//End Background Learning rate


		/*****************************************************
		* Pressure Map	
		*****************************************************/
		if(bShowPressure){

			unsigned char * rgbaPixels = grayImg.getPixels();
			unsigned char * colorRawPixels = new unsigned char[camWidth*camHeight*3]; 

			//total rgb pixels
			int totalPixel = camWidth * camHeight;

			int k = 0;

			for(int i = 0; i < totalPixel; i++){

			  //Get the 
			  float red = rgbaPixels[i];
			  float green = 0;
			  float blue = 255 - rgbaPixels[i] + 40; // Add 60

			  //Set some limitations	
			  if(blue >= 255){blue = 0;}
			  if(blue <= 50){blue = 0;}

			  //Set the RGB pixels to their colors
			  colorRawPixels[k]     = red;
			  colorRawPixels[k + 1] = green;
			  colorRawPixels[k + 2] = blue;

			  k +=3;
			}	
			pressureMap.setFromPixels(colorRawPixels, camWidth, camHeight);
			delete colorRawPixels;//End Pressure Map
		}

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

		if(bShowPressure)
			pressureMap.draw(40, 30, 320, 240);
		else
			sourceImg.draw(40, 30, 320, 240);

		grayDiff.draw(385, 30, 320, 240);

		fiLearn.draw(85, 392, 128, 96);
		subtractBg.draw(235, 392, 128, 96);
		highpassImg.draw(385, 392, 128, 96);
		grayImg.draw(535, 392, 128, 96);

		ofSetColor(0x000000);
		if(bShowPressure){bigvideo.drawString("Pressure Map", 140, 20);}
		else             {bigvideo.drawString("Source Image", 140, 20);}		
						  bigvideo.drawString("Tracked Image", 475, 20);	

		//Warped Box
		//m_box.draw( 40, 146 );

		//Draw PINK CIRCLE 'ON' LIGHT
		ofSetColor(255, 0, 255);
		ofFill();		
		ofCircle(20, 10, 5);
		ofNoFill();
	} 
	/*********************************
	* IF SENDING TUIO (TUIO MODE)
	*********************************/
	if(bTUIOMode){

		//Draw GREEN CIRCLE 'ON' LIGHT
		ofSetColor(0x00FF00);
		ofFill();		
		ofCircle(35, 10, 5);
		ofNoFill();

	/*	if(bToggleHelp && bSpaced){

			//---------------------------------
			ofSetColor(0xffffff);
			char buf[256];
			sprintf(buf, "Sending OSC messages to %s : %d", HOST, PORT);
			verdana.drawString(buf, 2*w+60, ofGetHeight()-63);
			//	verdana.drawString( "move the mouse to send OSC message\
			//						[/tuio/2Dcur <x> <y>] ", 20, 585 );
	*///}
	}
	/*********************************
	* IF NOT CALIBRATING
	*********************************/
	if(!bCalibration)
	{
		//Draw main interface
		bShowInterface = true;

		//Display applicaion and camera FPS in title 
		string str = "Tbeta: ";
		str+= ofToString(ofGetFrameRate(), 2)+"fps";	
		string str2 = "    Camera: ";
		str2+= ofToString(fps, 1)+"fps";
		ofSetWindowTitle(str + str2);
	}
	/*********************************
	* IF DRAWING BLOB OUTLINES
	*********************************/
	if(bDrawOutlines && bShowInterface && !bFastMode)
	{
		//Find the blobs
		for(int i=0; i<contourFinder.nBlobs; i++)
		{
			//temp blob to rescale and draw on screen
			ofxCvBlob drawBlob;
			drawBlob = contourFinder.blobs[i];

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
			drawBlob.draw(385, 30);

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

	/*
	if (bSnapshot == true)
	{
		// grab a rectangle at 200,200, width and height of 300,180
		img.grabScreen(20,300,camWidth,camHeight);
		char fileName[255];
		sprintf(fileName, "snapshot_%0.3i.png", snapCounter);
		img.saveImage(fileName);
		//sprintf(snapString, "saved %s", fileName);
		snapCounter++;
		bSnapshot = false;
	}
	ofSetColor(0xFFFFFF);
	img.draw(0,0,camWidth,camHeight);
	*/ 	


	//Update Demo Blobs
	if(!calibrate.bCalibrating && bCalibration){

		// go through all the thingies
		for (int i=0; i < thingies.size(); i++) {
			thingies[i].update();
		}
	}

	if(!bCalibration)
		gui->draw();
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
	frameRate			= XML.getValue("CONFIG:APPLICATION:FRAMERATE",0);
	
	winWidth			= XML.getValue("CONFIG:WINDOW:WIDTH",0);
	winHeight			= XML.getValue("CONFIG:WINDOW:HEIGHT",0);
	minWidth			= XML.getValue("CONFIG:WINDOW:MINX",0);
	minHeight			= XML.getValue("CONFIG:WINDOW:MINY",0);
	bFullscreen			= XML.getValue("CONFIG:WINDOW:FULLSCREEN",0);

	deviceID			= XML.getValue("CONFIG:CAMERA_0:DEVICE",0);
	camWidth			= XML.getValue("CONFIG:CAMERA_0:WIDTH",0);
	camHeight			= XML.getValue("CONFIG:CAMERA_0:HEIGHT",0);
	//camRate			= XML.getValue("CONFIG:CAMERA_0:FRAMERATE",0);

	bShowLabels			= XML.getValue("CONFIG:BOOLEAN:LABELS",0);
	bDrawVideo			= XML.getValue("CONFIG:BOOLEAN:VIDEO",0);
	bSnapshot			= XML.getValue("CONFIG:BOOLEAN:SNAPSHOT",0);
	bFastMode			= XML.getValue("CONFIG:BOOLEAN:FAST",0);	
	bDrawOutlines		= XML.getValue("CONFIG:BOOLEAN:OUTLINES",0);
	bLearnBakground		= XML.getValue("CONFIG:BOOLEAN:LEARNBG",0);

	bCalibration		= XML.getValue("CONFIG:BOOLEAN:CALIBRATION",0);

	bVerticalMirror		= XML.getValue("CONFIG:BOOLEAN:VMIRROR",0);
	bHorizontalMirror	= XML.getValue("CONFIG:BOOLEAN:HMIRROR",0);	
	
	threshold			= XML.getValue("CONFIG:INT:THRESHOLD",0);
	highpassBlur		= XML.getValue("CONFIG:INT:HIGHPASSBLUR",0);
	highpassNoise		= XML.getValue("CONFIG:INT:HIGHPASSNOISE",0);
	highpassAmp			= XML.getValue("CONFIG:INT:HIGHPASSAMP",0);
	
//--------------------------------------------------- TODO XML NETWORK SETTINGS	
	bTUIOMode			  = XML.getValue("CONFIG:BOOLEAN:TUIO",0);
	//myTUIO.myLocalHost  = XML.getValue("CONFIG:NETWORK:LOCALHOST",0);
	//myTUIO.myRemoteHost = XML.getValue("CONFIG:NETWORK:HOSTA",0);
	//myTUIO.myTUIOPort	  = XML.getValue("CONFIG:NETWORK:TUIO_PORT_OUT",0);
	myTUIO.TUIOSocket.setup(HOST, PORT);
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

	if(activeInput){	

	ofSetFullscreen(bFullscreen);
	

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

			calibrate.cameraToScreenSpace(drawBlob2.centroid.x, drawBlob2.centroid.y);

		/*	ofSetColor(0xFF0099);
			//ofFill();
			ofEllipse(drawBlob2.centroid.x * ofGetWidth(), drawBlob2.centroid.y * ofGetHeight(), 
				      drawBlob2.boundingRect.width, drawBlob2.boundingRect.height);
		*/

			//Displat Text of blob information
			ofSetColor(0x00FF00);
			char idStr[1024];	
			sprintf(idStr, "id: %i \n x: %f \n y: %f",drawBlob2.id, ceil(drawBlob2.centroid.x * ofGetWidth()), 
																	ceil(drawBlob2.centroid.y * ofGetHeight()));
			verdana.drawString(idStr, drawBlob2.centroid.x * ofGetWidth() + drawBlob2.boundingRect.width/2 + 20, 
									  drawBlob2.centroid.y * ofGetHeight() + drawBlob2.boundingRect.height/2 + 20);
		
		
			/*************************
			* Fading Blobs
			*************************/
			thingies.push_back(Thingy(drawBlob2.centroid.x * ofGetWidth(), drawBlob2.centroid.y * ofGetHeight(), 
									  drawBlob2.boundingRect.width, drawBlob2.boundingRect.height));
			                                   
			vector<Thingy>::iterator this_thingy;
			// go through all the thingies
			for(this_thingy = thingies.begin(); this_thingy != thingies.end();) {
						
				//Erasing this way makes it so the vector iterator is not invalidated
				if(this_thingy->alpha <= 0){
					this_thingy = thingies.erase(this_thingy);
				}
				else{
					this_thingy++;
				}
			}
		}	
		//Draw fading blobs
		for (int i=0; i < thingies.size(); i++) {
		
			thingies[i].draw();
		}//end fading blobs	

	}//End Blob Drawing


	/******************************
	* Calibration Instructions
	******************************/		
	ofSetColor(0xFF00FF);	
	ofSetWindowTitle("Calibration");		
	char reportStr[1024];	
	calibrationText.setLineHeight(20);
	
	if(calibrate.bCalibrating){
		sprintf(reportStr, 
		"CALIBRATING: \n\n\
		-Touch current circle target and lift up to calibrate point \n\
		-Press [b] to recapture background (if there's false blobs) \n\
		-Press [r] to go back to previous point(s) \n");
		calibrationText.drawString(reportStr, 33, 60);
	}else
	{
		sprintf(reportStr, 
		"CALIBRATION: \n\n\
		-Press [x] to start calibrating \n\
		-Press [c] to return main screen \n\
		-Press [b] to recapture background \n\n\
		CHANGING GRID SIZE (number of points): \n\n\
		-Current Grid Size is %i x %i \n\
		-Press [+]/[-] to add/remove points on X axis \n\
		-Press [shift][+]/[-] to add/remove points on Y axis \n\n\
		ALINGING BOUNDING BOX TO PROJECTION SCREEN: \n\n\
		-Use arrow keys to move bounding box\n\
		-Press and hold [w],[a],[s],[d] (top, left, bottom, right) to adjust each side\n", calibrate.GRID_X + 1, calibrate.GRID_Y + 1);
		calibrationText.drawString(reportStr, 33, 60);
	}
}
}

/*****************************************************************************
 * KEY EVENTS
 *****************************************************************************/
void testApp::keyPressed(int key)
{ 
	// THIS IS FOR THE ~ toggle...
	if(key==126 || key==96) 
	{
	}

	switch(key)
	{	
/*		case 's':
			XML.saveFile("config.xml");
			message ="Settings Saved!";
			break;	
*/		case '1':
			if(bToggleHelp)
			{	
				//bToggleHelp = false;	
				//ofSetWindowShape((2.0/3.0)*(ofGetWidth()-80)+60,ofGetHeight());
			}
			else
			{	
				//bToggleHelp = true;
				//ofSetWindowShape(max((3.0/2.0)*(ofGetWidth()-60)+80, minWidth), max(ofGetHeight(), minHeight));
			}
			break;
		case '2':
			if(bDrawVideo)
				bDrawVideo = false;	
			else	
				bDrawVideo = true;
			break;
		case 'b':
			bLearnBakground = true;
			break;		
		case 'o':
			if(bDrawOutlines)
				bDrawOutlines = false;	
			else
				bDrawOutlines = true;
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
		case ' ':
			if(bFastMode)
			{	
				bFastMode = false;	
				//bToggleHelp = true;
				ofSetWindowShape(900,600); //default size
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
		case 'g':
			bSnapshot = true;
			break;
		case 'v':
			#ifdef _USE_LIVE_VIDEO
				vidGrabber.videoSettings();
			#endif
			break;	
		case 'l':
			if(bShowLabels)
				bShowLabels = false;
			else	
				bShowLabels = true;
			break;
		case 'i':
			if(bShowInterface)
				bShowInterface = false;
			else	
				bShowInterface = true;
			break;
		case 'p':
			if(bShowPressure)
				bShowPressure = false;
			else	
				bShowPressure = true;
			break;
		/***********************
		* Keys for Calibration
		***********************/
		case 'c': //Enter/Exit Calibration
			if(bCalibration){
				bCalibration = false;
				bFullscreen = false;
			}else{	
				bCalibration = true;
				bFullscreen = true;
			}
			break;
		case 'x': //Begin Calibrating
			if(bCalibration && calibrate.bCalibrating)
				calibrate.bCalibrating = false;
			else if(bCalibration)
				calibrate.beginCalibration();
			else
			    bCalibration = false;
			break;
		case 'r': //Revert Calibration
			if(calibrate.bCalibrating)calibrate.revertCalibrationStep();
		case 'w': //Up
			bW = true;
			break;	
		case 's': //Down
			bS = true;
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
	//sprintf(eventString, "mouseMoved = (%i,%i)", x, y);
}	

void testApp::mouseDragged(int x, int y, int button)
{	
	//sprintf(eventString, "mouseDragged = (%i,%i - button %i)", x, y, button);


	//-------------------------------- Warp Box
	if(x < (40 + 320) && x > 40){

		if(y < (146 + 240) && y > 146){
	
			if(m_box.findSelectionDistance(x, y) < 20){

				m_box.adjustHandle(x,y);
			}
		}
	}
		

	gui->mouseDragged(x, y, button);
}

void testApp::mousePressed(int x, int y, int button)
{
	sprintf(eventString, "mousePressed = (%i,%i - button %i)", x, y, button);
	printf("button\n", button);



	gui->mousePressed(x, y, button);	
}

void testApp::mouseReleased()
{	
	sprintf(eventString, "mouseReleased");

	gui->mouseReleased(mouseX, mouseY, 0);	
}


/*****************************************************************************
*								TOUCH EVENTS
*****************************************************************************/
void testApp::blobOn( ofxCvBlob b)
{
	printf("Blob DOWN %i \n", b.id); 

	if(bCalibration)
	downColor = 0x2FB5FF; //change target color when a finger is down
		
	//If sending TUIO, add the blob to the map list
	if(bTUIOMode)
	{
		calibrate.cameraToScreenSpace(b.centroid.x, b.centroid.y);
		myTUIO.blobs[b.id] = b;
	}

}

void testApp::blobMoved( ofxCvBlob b) 
{
	//If sending TUIO, update the move information for the blob
	if(bTUIOMode)
	{
		calibrate.cameraToScreenSpace(b.centroid.x, b.centroid.y);
		myTUIO.blobs[b.id] = b;
	}
}  

void testApp::blobOff( ofxCvBlob b) 
{
	printf("Blob UP %i \n", b.id);

	if(bCalibration)
	downColor = 0xFF0000;

	//If Calibrating, register the calibration point on blobOff
	if(calibrate.bCalibrating){			
		
		calibrate.cameraPoints[calibrate.calibrationStep] = vector2df(b.centroid.x, b.centroid.y);
		calibrate.nextCalibrationStep();
		
		printf("%d (%f, %f)\n", calibrate.calibrationStep, b.centroid.x, b.centroid.y);
	}

	//If sending TUIO, Delete Blobs from map list
	if(bTUIOMode)
	{
		std::map<int, ofxCvBlob>::iterator iter;
		for(iter = myTUIO.blobs.begin(); iter != myTUIO.blobs.end();)
		{
			if(iter->second.id == b.id)
			{
				iter = myTUIO.blobs.erase(iter);
			}
			else{
				iter++;
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
	
	// -------------------------------- SAVE STATE ON EXIT
	//saveSettngs();
	calibrate.saveCalibration();
}












void testApp::handleGui(int parameterId, int task, void* data, int length)
{
	switch(parameterId)
	{
		
		case sourcePanel_nextCam:
			if(length == sizeof(bool))
			{
				if(*(bool*)data)
				{
					activeInput = false;

					if(deviceID < vidGrabber.getNumDevices() - 2)
					{
						deviceID += 1;
						vidGrabber.close();
						vidGrabber.setDeviceID(deviceID);
						vidGrabber.setVerbose(false);		
						vidGrabber.initGrabber(camWidth,camHeight);
					}
					activeInput = true;
					bLearnBakground = true;
				}					
			}
			break;
		case sourcePanel_previousCam:
			if(length == sizeof(bool))
			{
				if(*(bool*)data)
				{
					activeInput = false;

					if(deviceID > 0)
					{
						deviceID -= 1;
						vidGrabber.close();
						vidGrabber.setDeviceID(deviceID);
						vidGrabber.setVerbose(false);		
						vidGrabber.initGrabber(camWidth,camHeight);
					}
					activeInput = true;
					bLearnBakground = true;
				}					
			}
			break;

		case propertiesPanel_flipH:
			if(length == sizeof(bool))
				bHorizontalMirror = *(bool*)data;
			break;
		case propertiesPanel_flipV:
			if(length == sizeof(bool))
				bVerticalMirror = *(bool*)data;
			break;

		case optionPanel_tuio:
			if(length == sizeof(bool))
				bTUIOMode = *(bool*)data;
			break;
		case optionPanel_draw:
			if(length == sizeof(bool))
				bDrawVideo = *(bool*)data;
			break;
			

		case backgroundPanel_remove:
			if(length == sizeof(bool))
				bLearnBakground = *(bool*)data;
			break;
		case highpassPanel_blur:
			if(length == sizeof(float))
				highpassBlur = *(float*)data;
			break;
		case highpassPanel_noise:
			if(length == sizeof(float))
				highpassNoise = *(float*)data;
			break;
		case amplifyPanel_amp:
			if(length == sizeof(float))
				highpassAmp = *(float*)data;
			break;
		case trackedPanel_threshold:
			if(length == sizeof(float))
				threshold = *(float*)data;
			break;
		case trackedPanel_outlines:
			if(length == sizeof(bool))
				bDrawOutlines = *(bool*)data;
			break;
		case trackedPanel_ids:
			if(length == sizeof(bool))
				bShowLabels = *(bool*)data;
			break;
					
								
			
		case kParameter_SaveXml:
			if(length == sizeof(bool))
			{
				if(*(bool*)data)
					gui->saveToXml(OFXGUI_XML);
			}
			break;
		case kParameter_File:
			if(length == sizeof(string))
			{
				string file = *(string*)data;
				gui->buildFromXml(file);
			}
			break;
		
	}
}