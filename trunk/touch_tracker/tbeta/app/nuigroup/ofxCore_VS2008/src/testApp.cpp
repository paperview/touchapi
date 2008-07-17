#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0500

#include "testApp.h"
#include "uiDefinition.h"

#include <windows.h>

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

	// ---------------------------------Window Properties 
	ofSetWindowShape(winWidth,winHeight);
	ofSetFullscreen(bFullscreen);
	ofSetFrameRate(130);			//This will be based on camera fps in the future		
	ofSetVerticalSync(false);	//Set vertical sync to false for better performance


	//Intialize FPS variables
	frames = 0;
	fps = 0;
	lastFPSlog = 0;


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
		vidPlayer.loadMovie("test_videos/HCI_FTIR.mov");
		//vidPlayer.loadMovie("test_videos/raw.mp4");
        vidPlayer.play();	
		printf("Video Mode\n");
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
	calibrationText.loadFont("verdana.ttf", 12, true, true);
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

		  //Set the RGB pixels to their colors
		  colorRawPixels[k]     = red;
		  colorRawPixels[k + 1] = green;
		  colorRawPixels[k + 2] = blue;

		  k +=3;
		}		           

		pressureMap.setFromPixels(colorRawPixels, camWidth, camHeight);
		delete colorRawPixels;//End Pressure Map

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
		fLearnRate = 0.04f;

		if(contourFinder.nBlobs > 0){

			fLearnRate = 0.001f;
		}//End Background Learning rate
	}
	
	//We're not using frameseq right now with OSC
	//frameseq ++;	
	
	//----------------------------------------------ParameterUI	
	parameterUI->update();
}



//--------------------------------------------------------------
void testApp::learnBackground( ofxCvGrayscaleImage & _giLive, ofxCvGrayscaleImage & _grayBg, ofxCvFloatImage & _fiLearn, float _fLearnRate )
 {

	_fiLearn.addWeighted( _giLive, _fLearnRate);
    
    _grayBg = _fiLearn;

}
//--------------------------------------------------------------
void testApp::bgCapture( ofxCvGrayscaleImage & _giLive )
{
	learnBackground( _giLive, grayBg, fiLearn, 1.0f);       
}
//--------------------------------------------------------------



/******************************************************************************
 * The draw function paints the textures onto the screen. It runs after update.
 *****************************************************************************/
void testApp::draw(){	

	/********************
	* IF CALIBRATING
	********************/
	if(bCalibration)
	{
		bDrawVideo = false;
		bDrawOutlines = false;
		bShowLabels = false;

		doCalibration();
	}
	/**********************
	* ELSE (NOT CALIBRATING
	***********************/
	else
	{
		bDrawVideo = true;
		bDrawOutlines = true;
		bShowLabels = true;

		string str = "Tbeta: ";
		str+= ofToString(ofGetFrameRate(), 2)+" fps";	
		string str2 = " Camera: ";
		str2+= ofToString(fps, 1)+" fps";
		ofSetWindowTitle(str + str2);

		ofSetColor(0xFFFFFF);	
		logo.draw(820,5);
		
		// render the UI
		// Change to the projection matrix and set our viewing volume.
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
		gluOrtho2D(0,  ofGetWidth(), ofGetHeight() ,0);
	    // We don't want to modify the projection matrix. 
		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();	
	
		parameterUI->render();
	}

	/********************************
	* IF DRAW STUFF
	********************************/
	if(bDrawVideo && !bFastMode)
	{
		background.draw(0, 0);

		sourceImg.draw(40, 146, 320, 240);
		pressureMap.draw(445, 146, 320, 240);
		fiLearn.draw(40, 521, 128, 96);
		subtractBg.draw(187, 521, 128, 96);
		highpassImg.draw(337, 521, 128, 96);
		grayImg.draw(487, 521, 128, 96);

		ofSetColor(0x000000);
		bigvideo.drawString("Raw Image", 145, 135);
		bigvideo.drawString("Tracked Image", 535, 135);	
		verdana.drawString("Background", 70, 630);
		verdana.drawString("Background Removed", 189, 630);
		verdana.drawString("Highpass", 375, 630);
		verdana.drawString("Amplify", 530, 630);

		//Warped Box
		//m_box.draw( 40, 146 );

		//Draw PINK CIRCLE 'ON' LIGHT
		ofSetColor(255, 0, 255);
		ofFill();		
		ofCircle(20, 10, 5);
		ofNoFill();
	} 

	/********************************
	* IF TUIO MODE
	********************************/
	if(bTUIOMode){

		if(bDrawOutlines)
		{
			SendOSC();
		}

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

	if(bDrawOutlines && !bFastMode)
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
				drawBlob.pts[j].x = (320/camWidth) * (drawBlob.pts[j].x);
				drawBlob.pts[j].y = (240/camHeight) * (drawBlob.pts[j].y);
			}

			//draw contours on the figures
			drawBlob.draw(445, 146);

			if(bShowLabels)
			{
				calibrate.cameraToScreenSpace(drawBlob.centroid.x, drawBlob.centroid.y);
				//printf("blah %f \n", drawBlob.centroid.x);				

				//Displat Text of blob information
				ofSetColor(0xCCCCCC);
				char idStr[1024];	
				sprintf(idStr, "id: %i \n x: %f \n y: %f",drawBlob.id, ceil(drawBlob.centroid.x * camWidth), 
																	   ceil(drawBlob.centroid.y * camHeight));
				verdana.drawString(idStr, drawBlob.centroid.x * 320.0f + drawBlob.boundingRect.width/2 + 455, 
										  drawBlob.centroid.y * 240.0f + drawBlob.boundingRect.height/2 + 156);			
			}
		}
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
}

/*****************************************************************************
 * TODO:
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
		case 's':
/*			PUT THIS IN ANOTHER METHOD
			XML.setValue("CONFIG:BOOLEAN:LABELS",bShowLabels);
			XML.setValue("CONFIG:BOOLEAN:VIDEO",bDrawVideo);
			XML.setValue("CONFIG:BOOLEAN:SNAPSHOT",bSnapshot);
			XML.setValue("CONFIG:BOOLEAN:FAST",bFastMode);
			XML.setValue("CONFIG:BOOLEAN:OUTLINES",bDrawOutlines);
			XML.setValue("CONFIG:BOOLEAN:LEARNBG",bLearnBakground);
			XML.setValue("CONFIG:BOOLEAN:CALIBRATION",bCalibration);
			XML.setValue("CONFIG:BOOLEAN:VMIRROR",bVerticalMirror);
			XML.setValue("CONFIG:BOOLEAN:HMIRROR",bHorizontalMirror);
*/
			XML.saveFile("config.xml");
			message ="Settings Saved!";
			break;	
		case '1':
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
				ofSetWindowShape(max((3.0/2.0)*(ofGetWidth()-60)+80, minWidth),
					             max(ofGetHeight(), minHeight));
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
				//ofSetWindowShape(700,600);
			}
			else
			{	
				bTUIOMode = true;	
				//ofSetWindowShape(950,700);
			}
			break;	
		case 'c':
			if(bCalibration)
				bCalibration = false;		
			else	
				bCalibration = true;	
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
		case 'a':
			threshold ++;
			if(threshold > 255) threshold = 255;
			break;		
		case 'z':
			threshold --;
			if(threshold < 10) threshold = 10;
			//if (threshold < 0) threshold = 0;
			break;	
		case '[':
			calibrate.GRID_X ++;
			calibrate.GRID_Y ++;
			calibrate.screenBB.lowerRightCorner.X += .001;
			if(calibrate.screenBB.lowerRightCorner.X > 1) calibrate.screenBB.lowerRightCorner.X = 1;
			if(calibrate.GRID_X > 16) calibrate.GRID_X = 16; if(calibrate.GRID_Y > 16) calibrate.GRID_Y = 16; calibrate.setGrid(calibrate.GRID_X, calibrate.GRID_Y);
			calibrate.calibrationStep = 0;
			break;	
		case ']':
			calibrate.GRID_X --;
			calibrate.GRID_Y --;
			calibrate.screenBB.lowerRightCorner.X -= .001;
			if(calibrate.screenBB.lowerRightCorner.X < 0) calibrate.screenBB.lowerRightCorner.X = 0;
			if(calibrate.GRID_X < 1) calibrate.GRID_X = 1; if(calibrate.GRID_Y < 1) calibrate.GRID_Y = 1; calibrate.setGrid(calibrate.GRID_X, calibrate.GRID_Y);
			calibrate.calibrationStep = 0;
			break;	
		case 'g':
			bSnapshot = true;
			break;
		case 'r':
			#ifdef _USE_LIVE_VIDEO
				vidGrabber.videoSettings();
			#endif
			break;	
		case 'f':
			ofToggleFullscreen();
			break;
		case 'h':
			if(bVerticalMirror)
				bVerticalMirror = false;
			else
				bVerticalMirror = true;
			break;
		case 'v':
			if(bHorizontalMirror)
				bHorizontalMirror = false;
			else	
				bHorizontalMirror = true;
			break;
		case 'l':
			if(bShowLabels)
				bShowLabels = false;
			else	
				bShowLabels = true;
			break;
	}
	// --------------------------------- END MAIN KEYBOARD SWITCH
}


void testApp::doCalibration(){

	int screenW = ofGetWidth();
	int screenH = ofGetHeight();
	
	//Change the background color when a finger presses down/up		
	ofSetColor(0x000000);
	if(calibrate.bCalibrating && downColor){ofSetColor(0x2F2F2F);}
	ofFill();	
	ofRect(0, 0, screenW, screenH);	

	//Get the screen points so we can make a grid 
	vector2df *screenpts = calibrate.getScreenPoints();

	int i;

	//For each grid point
	for(i=0; i<calibrate.GRID_POINTS; i++)
	{
		//Draw a Red Circle if the it's the active confg points
		if(calibrate.calibrationStep == i){
			ofSetColor(0xFF0000);
			ofFill();
			ofCircle(screenpts[i].X * screenW, screenpts[i].Y * screenH, 9);	
		}
		//Draw a green target (t) if the it's NOT the active point
		else{ 
			ofSetColor(0x00FF00);
			ofFill();
			ofCircle(screenpts[i].X * screenW, screenpts[i].Y * screenH, 5);				
		}	

		//Make Plus Sign
		ofSetColor(0xFF0000);
		//Horizontal Plus
		ofLine(screenpts[i].X * screenW - 20, screenpts[i].Y * screenH, screenpts[i].X * screenW + 20, screenpts[i].Y * screenH);
		//Vertical Plus
		ofLine(screenpts[i].X * screenW, screenpts[i].Y * screenH - 20, screenpts[i].X * screenW, screenpts[i].Y * screenH + 20);
	

		ofSetColor(0xDDDDDD);
		ofNoFill();

		//if(i <= 2)
		//ofLine(screenpts[i].X * screenW, screenpts[i].Y * screenH, screenBB.getWidth() * screenW, screenpts[i].Y * screenH);
		//if(i <= 2)
		//ofLine(screenpts[i].X * screenW, screenpts[i].Y * screenH, screenpts[i].X * screenW, screenBB.getHeight() * screenH);
	}

	ofSetColor(0x00FF00);
	ofNoFill();
	ofRect(calibrate.screenBB.upperLeftCorner.X * screenW, calibrate.screenBB.upperLeftCorner.Y * screenH, 
		   calibrate.screenBB.getWidth() * screenW, calibrate.screenBB.getHeight() * screenH);

	
	//Draw blobs in calibration to see where you are touching
	if(!calibrate.bCalibrating){

		//Find the blobs
		for(int i=0; i<contourFinder.nBlobs; i++)
		{
			//temp blob to rescale and draw on screen
			ofxCvBlob drawBlob2;
			drawBlob2 = contourFinder.blobs[i];

			calibrate.cameraToScreenSpace(drawBlob2.centroid.x, drawBlob2.centroid.y);

			ofSetColor(0xFF0099);
			//ofFill();
			ofEllipse(drawBlob2.centroid.x * ofGetWidth(), drawBlob2.centroid.y * ofGetHeight(), 
				      drawBlob2.boundingRect.width, drawBlob2.boundingRect.height);
			
			//Displat Text of blob information
			ofSetColor(0x00FF00);
			char idStr[1024];	
			sprintf(idStr, "id: %i \n x: %f \n y: %f",drawBlob2.id, ceil(drawBlob2.centroid.x * ofGetWidth()), 
																	ceil(drawBlob2.centroid.y * ofGetHeight()));
			verdana.drawString(idStr, drawBlob2.centroid.x * ofGetWidth() + drawBlob2.boundingRect.width/2 + 20, 
									  drawBlob2.centroid.y * ofGetHeight() + drawBlob2.boundingRect.height/2 + 20);
		}	
	}//End Blob Drawing


	/*****************************************************************************
	* Calibration Instructions
	*****************************************************************************/		
	ofSetColor(0xFFFFFF);	
	ofSetWindowTitle("Calibration");		
	calibrationText.drawString("Calibration", 33, 60);	
	char reportStr[1024];	
	sprintf(reportStr, "-press '] or [' resize grid \n\
						-press '2' to resize bounding box \n\
						-use arrow keys to move bounding box");
	calibrationText.drawString(reportStr, ofGetWidth()/2.5, ofGetHeight()/2.5);
}





//-------------------------------------------------------------- 
//	Load Settings from the config.xml file 
//-------------------------------------------------------------- 
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
	bTUIOMode			= XML.getValue("CONFIG:BOOLEAN:TUIO",0);
	//myLocalHost		= XML.getValue("CONFIG:NETWORK:LOCALHOST",0);
	//myRemoteHost		= XML.getValue("CONFIG:NETWORK:HOSTA",0);
	//myTUIOPort		= XML.getValue("CONFIG:NETWORK:TUIO_PORT_OUT",0);
	TUIOSocket.setup(HOST, PORT); 
//-------------------------------------------------------------- 
//  END XML SETUP


/****************************************************************
*						STEUP CALIBRATION						
****************************************************************/
	
	calibrate.setCamRes(camWidth, camHeight);
	calibrate.loadXMLSettings();
//End calibrationXML Calibration Settings
}




/*****************************************************************************
 * Send Set message of ID, x, y, X, Y, m, weight, width to OSC
 *****************************************************************************/
void testApp::SendOSC()
{
	
	//If there are no blobs, send alive message and fseq
	if(contourFinder.nBlobs==0)
	{
		//Sends alive message - saying 'Hey, there's no alive blobs'
		ofxOscMessage m1;
		m1.setAddress("/tuio/2Dcur");		
		m1.addStringArg("alive");
		TUIOSocket.sendMessage(m1);

		//Send fseq message
		/*		Commented out Since We're not using fseq right now
		ofxOscMessage m2;
		m2.setAddress( "/tuio/2Dcur" );		
		m2.addStringArg( "fseq" );
		m2.addIntArg(frameseq);
		TUIOSocket.sendMessage( m2 );
		*/
	}
	else //actually send the blobs
	{
		for(int i=0; i<contourFinder.nBlobs; i++)
		{

			float transformX = contourFinder.blobs[i].centroid.x;
			float transformY = contourFinder.blobs[i].centroid.y;

			float transformWidth = contourFinder.blobs[i].boundingRect.width;
			float transformHeight = contourFinder.blobs[i].boundingRect.height;
	
			calibrate.transformDimension(transformWidth, transformHeight, transformX, transformY);
			calibrate.cameraToScreenSpace(transformX, transformY);

			//Set Message
			ofxOscMessage m1;
			m1.setAddress("/tuio/2Dcur");
			m1.addStringArg("set");
			m1.addIntArg(contourFinder.blobs[i].id); //id
			m1.addFloatArg(transformX);  // x
			m1.addFloatArg(transformY); // y 
			m1.addFloatArg(0); //X
			m1.addFloatArg(0); //Y
			m1.addFloatArg(contourFinder.blobs[i].area); //m	
			m1.addFloatArg(transformWidth); // wd
			m1.addFloatArg(transformHeight);// ht
			TUIOSocket.sendMessage(m1 );

			//Send alive message of all alive IDs
			ofxOscMessage m2;
			m2.setAddress("/tuio/2Dcur");		
			m2.addStringArg("alive");

			for(int i=0; i<contourFinder.nBlobs; i++)
				m2.addIntArg(contourFinder.blobs[i].id); //Get list of ALL active IDs		

			TUIOSocket.sendMessage(m2);//send them		

			//Send fseq message
			/* Commented out Since We're not using fseq right now
			ofxOscMessage m3;
			m3.setAddress( "/tuio/2Dcur" );		
			m3.addStringArg( "fseq" );
			m3.addIntArg(frameseq);
			TUIOSocket.sendMessage( m3 );
			*/
		}
	}
}


/*****************************************************************************
 * TODO:
 *****************************************************************************/
void testApp::mouseMoved(int x, int y)
{
	sprintf(eventString, "mouseMoved = (%i,%i)", x, y);
}	

/*****************************************************************************
 * TODO:
 *****************************************************************************/
void testApp::mouseDragged(int x, int y, int button)
{	
	sprintf(eventString, "mouseDragged = (%i,%i - button %i)", x, y, button);


	//-------------------------------- Warp Box
	if(x < (40 + 320) && x > 40){

		if(y < (146 + 240) && y > 146){
	
			m_box.adjustHandle(x, y);
		}
	}
		
	//-------------------------------- PARAMETER UI EVENT
	parameterUI->mouseMotion(x, y);	
}

/*****************************************************************************
 * TODO:
 *****************************************************************************/
void testApp::mousePressed(int x, int y, int button)
{
	sprintf(eventString, "mousePressed = (%i,%i - button %i)", x, y, button);
	printf("button\n", button);

	//-------------------------------- PARAMETER UI EVENT
	parameterUI->mouseDown(x, y, button);	
}

/*****************************************************************************
 * TODO:
 *****************************************************************************/
void testApp::mouseReleased()
{	
	sprintf(eventString, "mouseReleased");
	//-------------------------------- PARAMETER UI EVENT
	parameterUI->mouseUp(0, 0, 0);	
}



void testApp::blobOn( ofxCvBlob b)
{
	printf("Blob %i \n", b.id); downColor = true;

}

void testApp::blobMoved( ofxCvBlob b) 
{


}  

void testApp::blobOff( ofxCvBlob b) 
{
	downColor = false;

	//printf("Blob Up %i \n", b.id);

	if(calibrate.bCalibrating){			
		
		calibrate.cameraPoints[calibrate.calibrationStep] = vector2df(b.centroid.x, b.centroid.y);
		calibrate.nextCalibrationStep();
		
		printf("%d (%f, %f)\n", calibrate.calibrationStep, b.centroid.x, b.centroid.y);
	}
}



/*****************************************************************************
 * TODO:
 *****************************************************************************/
void testApp::exit()
{

	//Send ESC Key
/*	INPUT aInput;
	aInput.type = INPUT_KEYBOARD; 
	aInput.ki.wVk = VK_ESCAPE;
	aInput.ki.wScan = 0;//0x45;
	aInput.ki.time = 0;
	aInput.ki.dwFlags = KEYEVENTF_EXTENDEDKEY;
	aInput.ki.dwExtraInfo = 0;

	// Simulate a key press
	int aResult = SendInput( 1,&aInput,sizeof(INPUT) );
*/

	//TODO: SEND ESC KEY TO KEEP FROM CRASHING -  can we emulate a keyboard
	//event to trick the app into closing properly?
	printf("tBeta module has exited!\n");	
	
	// -------------------------------- SAVE STATE ON EXIT

	calibrate.saveCalibration();

}

