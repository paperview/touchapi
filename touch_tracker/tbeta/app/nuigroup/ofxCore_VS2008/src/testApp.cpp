#include "testApp.h"
#include "uiDefinition.h"


/******************************************************************************
 * The setup function is run once to perform initializations in the application
 *****************************************************************************/
void testApp::setup()
{	
	// ---------------------------------MISC VARS FOR SETTINGS (MARKED FOR GC) 
	snapCounter		= 6; 
	frameseq		= 0;


	//Background Subtraction Learning Rate
	fLearnRate = 0.0001f;

	// ---------------------------------Load Settings from config.xml File 
	loadXMLSettings();
	myTUIO.setup();

	// ---------------------------------Window Properties 
	ofSetWindowShape(winWidth,winHeight);
	ofSetFullscreen(bFullscreen);
	ofSetFrameRate(30);			//This will be based on camera fps in the future		
	ofSetVerticalSync(false);	//Set vertical sync to false for better performance


	//Intialize FPS variables
	frames = 0;
	fps = 0;
	lastFPSlog = 0;

	bW = false;
	bA = false;
	bS = false;
	bD = false;


	#ifdef _USE_LIVE_VIDEO // MAKE BOTH LIVE VIDEO AND VCR MODE WORK AT SAME TIME 
       		
		vidGrabber.setVerbose(true);
        vidGrabber.initGrabber(camWidth,camHeight);		

		printf("Camera Mode\n");
		int grabW = vidGrabber.width;
		int grabH = vidGrabber.height;
		printf("Asked for %i by %i - actual size is %i by %i \n", 
				camWidth, camHeight, grabW, grabH);

	#else
        //vidPlayer.loadMovie("test_videos/FrontDI.m4v");
		//vidPlayer.loadMovie("test_videos/HCI_FTIR.mov");
		vidPlayer.loadMovie("test_videos/raw.mp4");
        vidPlayer.play();	
		printf("Video Mode\n");
		camHeight = vidPlayer.height;
		camWidth = vidPlayer.width;
		
	#endif
    
	//camWidth = 640;
	//camHeight = 480;

	//Allocate images (needed for drawing/processing images) ----Most of These won't be needed in the end
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	sourceImg.allocate(camWidth, camHeight);    //Source Image
	grayImg.allocate(camWidth, camHeight);		//Gray Image
	grayBg.allocate(camWidth, camHeight);		//Background Image
	subtractBg.allocate(camWidth, camHeight);   //Background After subtraction
	grayDiff.allocate(camWidth, camHeight);		//Difference Image between Background and Source
	highpassImg.allocate(camWidth, camHeight);  //Highpass Image
	giWarped.allocate(camWidth, camHeight);     //Warped Image (used for warped calibration)

	fiLearn.allocate(camWidth, camHeight);		//ofxFloatImage used for simple dynamic background subtracti
//	fiLearn.setUseTexture(false);

	pressureMap.allocate(camWidth, camHeight);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Fonts - Is there a way to dynamically change font size?
	verdana.loadFont("verdana.ttf", 8, true, true);	   //Font used for small images
	calibrationText.loadFont("verdana.ttf", 11, true, true);
	bigvideo.loadFont("verdana.ttf", 13, true, true);  //Font used for big images.
	
	//Static Images
	logo.loadImage("images/logo.jpg");
	background.loadImage("images/background.jpg"); //Main (Temp) Background

	//Parameter UI
	setupUI();	
	parameterUI = AParameterUI::Instance();
	parameterUI->init(ofGetWidth(), ofGetHeight());
	
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

	printf("Touchlib application is setup!\n");

	tracker.setListener(this);

}


/******************************************************************************
 * The update function runs continuously. Use it to update states and variables
 *****************************************************************************/
void testApp::update()
{	
	ofBackground(0,0,0);

    bNewFrame = false;
		
	#ifdef _USE_LIVE_VIDEO
       vidGrabber.grabFrame();
	   bNewFrame = vidGrabber.isFrameNew();
    #else
        vidPlayer.idleMovie();
        bNewFrame = vidPlayer.isFrameNew();
	#endif

	//printf("Boolean2: %i \n", bNewFrame);
	
	if (bNewFrame)
	{
		//Calculate FPS of Camera
		frames++;
		float time = ofGetElapsedTimeMillis();
		if(time > (lastFPSlog + 1000)){		
			fps = frames;
			frames = 0;
			lastFPSlog = time;			
		}

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

	
			
		//------------------------ SET FILTERS HERE ---------------------------
		
		//Set Mirroring Horizontal/Vertical
		sourceImg.mirror(bHorizontalMirror, bVerticalMirror);

		//sourceImg.warpIntoMe(sourceImg, m_box.fHandles, dstPts );

		subtractBg = sourceImg;

		subtractBg.blur(3);

		//giWarped.warpIntoMe(subtractBg, m_box.fHandles, dstPts );

		//subtractBg = giWarped;

		learnBackground( subtractBg, grayBg, fiLearn, fLearnRate);
 
			
		if (bLearnBakground == true)
		{
			bgCapture( subtractBg );
			bLearnBakground = false;
		}

		//Background Subtraction
		subtractBg.absDiff(grayBg, subtractBg);

		highpassImg = subtractBg;

		//HighPass
		highpassImg.highpass(highpassBlur, highpassNoise);

		grayImg = highpassImg;
		
		//Amplify
		grayImg.amplify(grayImg, highpassAmp);
		
		//
		grayDiff = grayImg;


		/*****************************************************
		* Pressure Map	
		*****************************************************/
		if(bShowPressure){

			unsigned char * rgbaPixels = grayDiff.getPixels();
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

		if(contourFinder.nBlobs > 0){

			fLearnRate = 0.0001f;
		}//End Background Learning rate
	}
	
	if(bTUIOMode){
		//We're not using frameseq right now with OSC
		//myTUIO.update();
	}
	
	//----------------------------------------------ParameterUI	
	parameterUI->update();
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
		//Draw Everything
		background.draw(0, 0);

		sourceImg.draw(40, 146, 320, 240);

		if(bShowPressure){
			pressureMap.draw(445, 146, 320, 240);
		}
		else{
			grayDiff.draw(445, 146, 320, 240);
		}

		fiLearn.draw(40, 521, 128, 96);
		subtractBg.draw(187, 521, 128, 96);
		highpassImg.draw(337, 521, 128, 96);
		grayImg.draw(487, 521, 128, 96);

		ofSetColor(0x000000);
		bigvideo.drawString("Raw Image", 145, 135);		
		if(bShowPressure){bigvideo.drawString("Pressure Map", 535, 135);}
		else			 {bigvideo.drawString("Tracked Image", 535, 135);}	
						   verdana.drawString("Background", 70, 630);
						   verdana.drawString("Background Removed", 189, 630);
						   verdana.drawString("Highpass", 375, 630);
						   verdana.drawString("Amplify", 530, 630);

		//Warped Box
		m_box.draw( 40, 146 );

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

		myTUIO.sendOSC();
		
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

		ofSetColor(0xFFFFFF);	
		logo.draw(820,5);
		
/*		// render the UI
		// Change to the projection matrix and set our viewing volume.
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
		gluOrtho2D(0,  ofGetWidth(), ofGetHeight() ,0);
	    // We don't want to modify the projection matrix. 
		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();	
*/	
		parameterUI->render();
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
				drawBlob.pts[j].x = (320.0f/camWidth) * (drawBlob.pts[j].x);
				drawBlob.pts[j].y = (240.0f/camHeight) * (drawBlob.pts[j].y);
			}
	
			//This adjusts the blob drawing for different cameras
			drawBlob.boundingRect.width  *= (320.0f/camWidth);
			drawBlob.boundingRect.height *= (240.0f/camHeight);
			drawBlob.boundingRect.x		 *= (320.0f/camWidth);;
			drawBlob.boundingRect.y		 *= (240.0f/camHeight);
			
			//Draw contours on the figures
			drawBlob.draw(445, 146);

			//Show Labels (ID, x, y);
			if(bShowLabels)
			{
				float xpos = drawBlob.centroid.x * (320.0f/camWidth);
				float ypos = drawBlob.centroid.y * (240.0f/camHeight);

				//Displat Text of blob information
				ofSetColor(0xCCFFCC);
				char idStr[1024];	
				sprintf(idStr, "id: %i",drawBlob.id);
				verdana.drawString(idStr, xpos + 430, ypos + drawBlob.boundingRect.height/2 + 160);			
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

	


/******************************
* STEUP CALIBRATION						
******************************/	
	calibrate.setCamRes(camWidth, camHeight);
	calibrate.loadXMLSettings();
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

/*****************************************************************************
 * KEY EVENTS
 *****************************************************************************/
void testApp::keyPressed(int key)
{ 
	// THIS IS FOR THE ~ toggle...
	if(key==126 || key==96) 
	{
		if(parameterUI->isActive)
		{
//			parameterUI->deActivate();	
			bSpaced = true;
			//bToggleHelp = true;	
		}
		else
		{
			parameterUI->activate();	
			bSpaced = false;		
			//bToggleHelp = false;	
		}
	}

	//------------------------------- BEGIN MAIN KEYBOARD SWITCH (LONGEST EVER)
	switch(key)
	{	
/*		case 's':
			XML.saveFile("config.xml");
			message ="Settings Saved!";
			break;	
*/		case '1':
			if(bToggleHelp)
			{	
				parameterUI->deActivate();	
				//bToggleHelp = false;	
				ofSetWindowShape((2.0/3.0)*(ofGetWidth()-80)+60,ofGetHeight());
			}
			else
			{	
				parameterUI->deActivate();	
				//bToggleHelp = true;
				ofSetWindowShape(max((3.0/2.0)*(ofGetWidth()-60)+80, minWidth), max(ofGetHeight(), minHeight));
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
		case 'c':
			if(bCalibration){
				bCalibration = false;
				bFullscreen = false;
			}
			else{	
				bCalibration = true;
				bFullscreen = true;
			}
			break;
		case 'x':
			if(bCalibration && calibrate.bCalibrating)
				calibrate.bCalibrating = false;
			else if(bCalibration)
				//bCalibrating = true;
				calibrate.beginCalibration();
			else
			    bCalibration = false;
			break;
		case ' ':
			if(bFastMode)
			{	
				bFastMode = false;	
				//bToggleHelp = true;
				ofSetWindowShape(1024,768); //default size
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
/*		case 'a':
			threshold ++;
			if(threshold > 255) threshold = 255;
			break;		
*/		case 'z':
			threshold --;
			if(threshold < 10) threshold = 10;
			//if (threshold < 0) threshold = 0;
			break;	
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
		case 'g':
			bSnapshot = true;
			break;
		case 'v':
			#ifdef _USE_LIVE_VIDEO
				vidGrabber.videoSettings();
			#endif
			break;	
		case 'f':
			ofToggleFullscreen();
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
			}
			else if(bA){
				calibrate.screenBB.upperLeftCorner.X += .001;
				if(calibrate.screenBB.upperLeftCorner.X > 1) calibrate.screenBB.upperLeftCorner.X = 1;
				calibrate.setGrid(calibrate.GRID_X, calibrate.GRID_Y);
				calibrate.calibrationStep = 0;
			}
			else{
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
			}
			else if(bA){
				calibrate.screenBB.upperLeftCorner.X -= .001;
				if(calibrate.screenBB.upperLeftCorner.X < 0) calibrate.screenBB.upperLeftCorner.X = 0;
				calibrate.setGrid(calibrate.GRID_X, calibrate.GRID_Y);
				calibrate.calibrationStep = 0;
			}
			else{
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
			}
			else if(bW){
				calibrate.screenBB.upperLeftCorner.Y += .001;
				if(calibrate.screenBB.upperLeftCorner.Y > 1) calibrate.screenBB.upperLeftCorner.Y = 1;
				calibrate.setGrid(calibrate.GRID_X, calibrate.GRID_Y);
				calibrate.calibrationStep = 0;
			}
			else{
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
			}
			else if(bW){
				calibrate.screenBB.upperLeftCorner.Y -= .001;
				if(calibrate.screenBB.upperLeftCorner.Y < 0) calibrate.screenBB.upperLeftCorner.Y = 0;
				calibrate.setGrid(calibrate.GRID_X, calibrate.GRID_Y);
				calibrate.calibrationStep = 0;
			}
			else{
				calibrate.screenBB.lowerRightCorner.Y -= .001;
				if(calibrate.screenBB.lowerRightCorner.Y < 0) calibrate.screenBB.lowerRightCorner.Y = 0;
				calibrate.screenBB.upperLeftCorner.Y -= .001;
				if(calibrate.screenBB.upperLeftCorner.Y < 0) calibrate.screenBB.upperLeftCorner.Y = 0;
				calibrate.setGrid(calibrate.GRID_X, calibrate.GRID_Y);
				calibrate.calibrationStep = 0;
			}
			break;
	}
	// --------------------------------- END MAIN KEYBOARD SWITCH
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
* MOUSE EVENTS
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
		
	//-------------------------------- PARAMETER UI EVENT
	parameterUI->mouseMotion(x, y);	
}

void testApp::mousePressed(int x, int y, int button)
{
	sprintf(eventString, "mousePressed = (%i,%i - button %i)", x, y, button);
	printf("button\n", button);

	//-------------------------------- PARAMETER UI EVENT
	parameterUI->mouseDown(x, y, button);	
}

void testApp::mouseReleased()
{	
	sprintf(eventString, "mouseReleased");
	//-------------------------------- PARAMETER UI EVENT
	parameterUI->mouseUp(0, 0, 0);	
}


/*****************************************************************************
* TOUCH EVENTS
*****************************************************************************/
void testApp::blobOn( ofxCvBlob b)
{
	printf("Blob DOWN %i \n", b.id); 

	if(bCalibration)
	{
	downColor = 0x2FB5FF; //change target color when a finger is down
	}
	
	if(bTUIOMode)
	{
		calibrate.cameraToScreenSpace(b.centroid.x, b.centroid.y);
		myTUIO.blobs[b.id] = b;
	}

}

void testApp::blobMoved( ofxCvBlob b) 
{
	if(bTUIOMode)
	{
		calibrate.cameraToScreenSpace(b.centroid.x, b.centroid.y);
		myTUIO.blobs[b.id] = b;
	}
}  

void testApp::blobOff( ofxCvBlob b) 
{
	downColor = 0xFF0000;

	printf("Blob UP %i \n", b.id);

	
	if(calibrate.bCalibrating){			
		
		calibrate.cameraPoints[calibrate.calibrationStep] = vector2df(b.centroid.x, b.centroid.y);
		calibrate.nextCalibrationStep();
		
		printf("%d (%f, %f)\n", calibrate.calibrationStep, b.centroid.x, b.centroid.y);
	}

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

	calibrate.saveCalibration();
}

