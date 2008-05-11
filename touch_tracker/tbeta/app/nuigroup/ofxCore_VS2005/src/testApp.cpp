#include "testApp.h"
#include "uiDefinition.h"

/******************************************************************************
 * The setup function is run once to perform initializations in the application
 *****************************************************************************/
void testApp::setup()
{	 		
//-------------------------------------------------------------- 
//	FIRST LETS LOAD THE CONFIG XML  
//-------------------------------------------------------------- // TODO: a seperate XML to map keyboard commands to action 
	message = "Loading config.xml...";
	if( XML.loadFile("config.xml") ){
		message = "Settings Loaded!";
	}else{
		//FAIL!
		message = "No Settings Found...";
		// GENERATE DEFAULT XML DATA WHICH WILL BE SAVED INTO THE CONFIG
	}

	// XML COLOR TEST (VERIFIES ITS WORKING)
	red		= XML.getValue("CONFIG:BACKGROUND:COLOR:RED", 0);
	green	= XML.getValue("CONFIG:BACKGROUND:COLOR:GREEN", 0);
	blue	= XML.getValue("CONFIG:BACKGROUND:COLOR:BLUE", 0);
	
	// MISC VARS FOR SETTINGS (MARKED FOR GC) 
	lastTagNumber	= 0;
	pointCount		= 0;
	lineCount		= 0;
	snapCounter			= 6; // MARKED	
	frameseq			= 0;

//-------------------------------------------------------------- 
//  START BINDING XML TO VARS
//-------------------------------------------------------------- 
	frameRate			= XML.getValue("CONFIG:APPLICATION:FRAMERATE",0);
	
	winWidth			= XML.getValue("CONFIG:WINDOW:WIDTH",0);
	winHeight			= XML.getValue("CONFIG:WINDOW:HEIGHT",0);
	minWidth			= XML.getValue("CONFIG:WINDOW:MIN",0);
	minHeight			= XML.getValue("CONFIG:WINDOW:MAX",0);
	bFullscreen			= XML.getValue("CONFIG:WINDOW:FULLSCREEN",0);
	
	camWidth			= XML.getValue("CONFIG:CAMERA_0:WIDTH",0);
	camHeight			= XML.getValue("CONFIG:CAMERA_0:HEIGHT",0);
	//camRate			= XML.getValue("CONFIG:CAMERA_0:FRAMERATE",0);

	bShowLabels			= XML.getValue("CONFIG:BOOLEAN:LABELS",0);
	bDrawVideo			= XML.getValue("CONFIG:BOOLEAN:VIDEO",0);
	bSnapshot			= XML.getValue("CONFIG:BOOLEAN:SNAPSHOT",0);
	bFastMode			= XML.getValue("CONFIG:BOOLEAN:FAST",0);	
	bDrawOutlines		= XML.getValue("CONFIG:BOOLEAN:OUTLINES",0);
	bInvertVideo		= XML.getValue("CONFIG:BOOLEAN:INVERT",0);
	bLearnBakground		= XML.getValue("CONFIG:BOOLEAN:LEARNBG",0);
	bTUIOMode			= XML.getValue("CONFIG:BOOLEAN:TUIO",0);
	bCalibration		= XML.getValue("CONFIG:BOOLEAN:CALIBRATION",0);
	bVerticalMirror		= XML.getValue("CONFIG:BOOLEAN:VMIRROR",0);
	bHorizontalMirror	= XML.getValue("CONFIG:BOOLEAN:HMIRROR",0);	
	
	threshold			= XML.getValue("CONFIG:INT:THRESHOLD",0);
	wobbleThreshold		= XML.getValue("CONFIG:INT:WTHRESHOLD",0);
	blurValue			= XML.getValue("CONFIG:INT:BLUR",0);
	blurGaussianValue	= XML.getValue("CONFIG:INT:BLURG",0);
	lowRange			= XML.getValue("CONFIG:INT:LOWRANGE",0);
	highRange			= XML.getValue("CONFIG:INT:HIGHRANGE",0);
	
	
	myLocalHost			= XML.getValue("CONFIG:NETWORK:LOCALHOST",0);
	myRemoteHost		= XML.getValue("CONFIG:NETWORK:HOSTA",0);
	myTUIOPort			= XML.getValue("CONFIG:NETWORK:TUIO_PORT_OUT",0);
	
	TUIOSocket.setup(myLocalHost, myTUIOPort); 
//-------------------------------------------------------------- 
//  END XML SETUP

	ofSetWindowShape(winWidth,winHeight);
	ofSetFullscreen(bFullscreen);
	ofSetFrameRate(frameRate);
	
	#ifdef _USE_LIVE_VIDEO // MAKE BOTH LIVE VIDEO AND VCR MODE WORK AT SAME TIME 
        vidGrabber.setVerbose(true);
        vidGrabber.initGrabber(camWidth,camHeight);
		printf("Camera Mode\n");
		int grabW = myGrabber.width;
		int grabH = myGrabber.height;
		printf("Asked for %i by %i - actual size is %i by %i \n", 
				camWidth, camHeight, grabW, grabH);
	#else
        vidPlayer.loadMovie("test_videos/FrontDI.m4v");
        vidPlayer.play();	
		printf("Video Mode\n");
	#endif
    
	sourceImg.allocate(camWidth, camHeight);
	grayImg.allocate(camWidth, camHeight);
	grayBg.allocate(camWidth, camHeight);
	grayDiff.allocate(camWidth, camHeight);
	
	videoInverted = new unsigned char[camWidth*camHeight*3];
	videoInvertTexture.allocate(camWidth, camHeight, GL_RGB);	

	ofSetWindowTitle("Configuration");
	printf("Touchlib application is setup!\n");

	verdana.loadFont("verdana.ttf", 8, false, true);		
	logo.loadImage("images/logo.jpg");

	//Parameter UI
	setupUI();	
	parameterUI = AParameterUI::Instance();
	parameterUI->init(ofGetWidth(), ofGetHeight());
	
	//for smooth animation... should not effect video output?
	ofSetVerticalSync(true);	

}
/******************************************************************************
 * The update function runs continuously. Use it to update states and variables
 *****************************************************************************/
void testApp::update()
{	
	//we change the background color here based on the values
	//affected by the mouse position
	ofBackground((int)red,(int)green,(int)blue);
	//ofBackground(0,0,0);
    bool bNewFrame = false;
		
	#ifdef _USE_LIVE_VIDEO
       vidGrabber.grabFrame();
	   bNewFrame = vidGrabber.isFrameNew();
    #else
        vidPlayer.idleMovie();
        bNewFrame = vidPlayer.isFrameNew();
	#endif
	
	if (bNewFrame)
	{
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
		//INVERT TEXTURE
		if (bInvertVideo)
		{	
			for( int i=0; i<totalPixels; i++)
				videoInverted[i] = 255 - pixels[i];
			videoInvertTexture.loadData(videoInverted, camWidth,
										camHeight, GL_RGB);
		}		
		
		//Set Mirroring Horizontal/Vertical
		sourceImg.mirror(bHorizontalMirror, bVerticalMirror);

		grayImg = sourceImg;
			
		//Blur Video
		if(blurValue > 3)
			grayImg.blur(blurValue);

		//Blur Gaussian Video
		if(blurGaussianValue > 3)
			grayImg.blurGaussian(blurGaussianValue);

		//Invert Video
		if(bInvertVideo)
			grayImg.invert();

		//Create Image Level Range 0 - 255		
		grayImg.convertToRange(lowRange, highRange);
		
    	//sourceImg.erode();
        
		if (bLearnBakground == true)
		{
			grayBg = grayImg;	
			bLearnBakground = false;
		}
	
		grayDiff.absDiff(grayBg, grayImg);
		grayDiff.threshold(threshold);
		grayDiff.dilate_3x3();

		contourFinder.findContours(grayDiff, 20,
			                      (camWidth*camHeight)/3, 10, true);
	}

	frameseq ++;	
	//----------------------------------------------ParameterUI	
	parameterUI->update();
}


/******************************************************************************
 * The draw function paints the textures onto the screen. It runs after update.
 *****************************************************************************/
void testApp::draw()
{
	//bLearnBakground = true; // ADAPTIVE BACKGROUND SUBTRATION?

	ofSetupScreen(); //? NEEDED?
	ofSetColor(0xffffff);	

	winWidth = ofGetWidth();
	winHeight = ofGetHeight();

	if(bToggleHelp)//set a minimum size only if help is on
	{
		if(winWidth<minWidth && winHeight<minHeight)
		{
			winWidth = minWidth;
			winHeight = minHeight;
			ofSetWindowShape(winWidth, winHeight);
		}
		else if(winWidth<minWidth)
		{
			winWidth = minWidth;
			ofSetWindowShape(winWidth, winHeight);
		}
		else if(winHeight<minHeight)
		{
			winHeight = minHeight;
			ofSetWindowShape(winWidth, winHeight);
		}
	}
	//image dimemsions with 20 pixel buffer between each frame
	//if help is toggled a third column is added so we compensate for it
	int w, h;
	if(bToggleHelp)
		w = (winWidth-80)/3;
	else
		w = (winWidth-60)/2;
	h = (winHeight-60)/2;

	if(bDrawVideo && !bFastMode)
	{
		sourceImg.draw(20, 20, w, h);
		grayImg.draw(w+40, 20, w, h);
		grayBg.draw(20, h+40, w, h);
		grayDiff.draw(w+40, h+40, w, h);
	}  
	
	//first send the OSC message of the contour data
	//why is it checking if outlines are to be drawn?
	//I think it should be bTuioMode...~~~~~~~~~ we have TUIO MODE mapped to the 't' key...
	if(bDrawOutlines)
		SendOSC();

	if(bDrawOutlines && !bFastMode)
	{
		//scale and draw on screen
		double wScale = double(w)/double(camWidth);
		double hScale = double(h)/double(camHeight);

		//contourFinder.draw(15,35);
		for(int i=0; i<contourFinder.nBlobs; i++)
		{
			//temp blob to rescale and draw on screen
			ofxCvBlob drawBlob;
			drawBlob = contourFinder.blobs[i];

			//rescale the blob
			drawBlob.boundingRect.x = wScale*(drawBlob.boundingRect.x);
			drawBlob.boundingRect.y = hScale*(drawBlob.boundingRect.y);
			drawBlob.boundingRect.width = wScale*(drawBlob.boundingRect.width);
			drawBlob.boundingRect.height=hScale*(drawBlob.boundingRect.height);
			drawBlob.area = wScale*hScale*(drawBlob.area);

			for( int j=0; j<contourFinder.blobs[i].nPts; j++ )
			{
				drawBlob.pts[j].x = wScale*(drawBlob.pts[j].x);
				drawBlob.pts[j].y = hScale*(drawBlob.pts[j].y);
			}

			//draw contours on the figures
			drawBlob.draw(20, 20);
			drawBlob.draw(w+40, 20);
			drawBlob.draw(w+40, h+40);

			//if(i < 3)
			//{
			if(50<contourFinder.blobs[i].area)
			{	
				if(bShowLabels){
				ofSetColor(0xffffff);
				char idStr[1024];		
				sprintf(idStr, "id: %i\nx: %f\ny: %f\ncx: %f\nc\
							   y: %f\nwd: %f\nht: %f\na: %f\n",i,
							   contourFinder.blobs[i].pts[0].x,
							   contourFinder.blobs[i].pts[0].y,
							   contourFinder.blobs[i].centroid.x,
							   contourFinder.blobs[i].centroid.y,
							   contourFinder.blobs[i].boundingRect.width,
							   contourFinder.blobs[i].boundingRect.height,
							   contourFinder.blobs[i].area);
				verdana.drawString(idStr,
					drawBlob.pts[0].x+drawBlob.boundingRect.width+30,
					drawBlob.pts[0].y+drawBlob.boundingRect.height);		
				}
			}
			//}
		}
	} 


	

	//---------------------------------------------------------------------HELP
	// IF HELP and NO SLIDERS
	if(bToggleHelp && bSpaced)
	{		
	
		//------------------------------------------------- DRAW XML SETTINGS OUTPUT	
		ofSetColor(255,0,0);	
		verdana.drawString("Status: "+message, 2*w+60, ofGetHeight()-15);
		//-------------------------------------------------
		ofSetColor(0xffffff);
		char reportStr[1024];	
		sprintf(reportStr, "press '~' for ui\npress '1' for help\n\
						press '2' for video\n\npress ' ' for mini\n\
						press 'f' for fullscreen\npress 't' for TUIO\n\
						press 'o' for outlines\npress 'l' to show labels\n\n\
						press 's' to save settings\n\press 'c' to calibrate\n\
						press 'r' camera setup\npress 'b' to capture bg\n\
						press 'i' to invert\npress 'x' to set filter bg\n\
						\npress 'a/z' to set threshold: %i\n\n\
						press 'w/e' to set DisplacementThreshold: %i\n\n\
						press 'n/m' to set Blur Amount: %i\n\
						press 'j/k' to set Gaussian Blur Amount: %i\n\
						press '-/=' to set Low Level: %i\n\
						press '9/0' to set High Level: %i\n\
						press 'h/v' to set Mirror Mode: None\n\n\
						press 'm' toggle DI or FTIR mode\n\n\
						press 'ESC' to exit (bug)\n\n\nblobs found: %i",
						threshold, wobbleThreshold, blurValue, 
						blurGaussianValue,lowRange,highRange, 
						contourFinder.nBlobs);
		verdana.drawString(reportStr, 2*w+60, 45);

		verdana.drawString("Original", 33, 40);
		verdana.drawString("Grayscale", w+53, 40);
		verdana.drawString("Background", 33, h+60);
		verdana.drawString("Contour", w+53, h+60);

		string fpsStr = "FPS: "+ofToString(ofGetFrameRate(), 2)+
						"\nCamera FPS: 34.00\nPipeline Use: 2323\n";
		verdana.drawString(fpsStr, 2*w+60, ofGetHeight()-50);	
		//verdana.drawString(eventString,80,25);

		//---------------------------------------------------------------EVENTS

		//sprintf(timeString, "Time: %0.2i:%0.2i:%0.2i \nElapsed time %i", 
		//		  ofGetHours(), ofGetMinutes(), ofGetSeconds(), 
		//		  ofGetElapsedTimeMillis());

		//verdana.drawString(timeString, 98,98);

		//ofEnableAlphaBlending();
		//ofSetColor(255,255,255,127);   // white, 50% transparent
		//ofFill();		
		//ofCircle(mouseX,mouseY,10);	
		//ofNoFill();
		//ofDisableAlphaBlending();

		if(bTUIOMode)
		{		
			//---------------------------------
			ofSetColor(0xffffff);
			char buf[256];
			sprintf(buf, "Sending OSC messages to %s : %d", HOST, PORT);
			verdana.drawString(buf, 2*w+60, ofGetHeight()-63);
			//	verdana.drawString( "move the mouse to send OSC message\
			//						[/tuio/2Dcur <x> <y>] ", 20, 585 );
		}//END TUIO MODE
	}	

	//----------------------------------------------------------------------OSC
	// display instructions

	/*
	if (bSmooth)
	{
		//ofEnableSmoothing();
		//ofDisableSmoothing();
	}
	
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
    //----------------------------------------------------------------DRAW LED FOR SHOWING VIDEO
	if(bDrawVideo)
	{		
		ofSetColor(255, 0, 255);
		ofFill();		
		ofCircle(20, 10, 5);
		ofNoFill();
	}		
	if(bTUIOMode)
	{		
			ofSetColor(0,255,0);
			ofFill();		
			ofCircle(35, 10, 5);
			ofNoFill();
	}		
	//----------------------------------------------------------------DRAW LEDS

	if(bCalibration)
	{		
		int screenW = ofGetWidth();
		int screenH = ofGetHeight();
		ofSetColor(0x000000);	
		ofFill();	
		ofRect(0, 0, screenW, screenH);	

		//ofSetColor(0xFFFFFF);	
		//videoInvertTexture.draw(0, 0, screenW, screenH);
		ofSetWindowTitle("Calibration");
		verdana.drawString("Calibration", 33, 60);	
		char reportStr[1024];	
		sprintf(reportStr, "press '] or [' resize grid\n\
							press '2' to resize bounding box\n\
							use arrow keys to move bounding box");
		verdana.drawString(reportStr, 700, 50);

		ofSetColor(0x00FF00);	
		int iCount = snapCounter;
		int iWidth = screenW;
		int iHeight = screenH;
		for(int i = 1; i < iCount; i++)
		{
			ofNoFill();
			ofRect(0, 2, iWidth-2, iHeight-2);
			ofLine(0, (iHeight/iCount) * i, iWidth, (iHeight/iCount)*i);
			ofLine((iWidth/iCount)*i, 0, (iWidth/iCount)*i, iHeight);
		}
	}		

	//------------------------------------------------------------ Parameter UI
	if(!bCalibration)
	{
		if(ofGetWidth() > 1000)
		{
			ofSetColor(0xFFFFFF);	
			logo.draw(ofGetWidth()-155,50);
			
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
	}
}

/******************************************************************************
 * TODO:
 *****************************************************************************/
void testApp::keyPressed(int key)
{ 
	sprintf(eventString, "keyPressed = (%i)", key);
	//printf(int(key));
	if(ofGetWidth()>1000)
	{


		// THIS IS FOR THE ~ toggle...
		if(key==126 || key==96) 
		{
			if(parameterUI->isActive)
			{
				parameterUI->deActivate();	
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
	}

// --------------------------------- BEGIN MAIN KEYBOARD SWITCH (LONGEST EVER)
	switch(key)
	{	
		case 's':
			XML.saveFile("config.xml");
			message ="Settings Saved!";
			break;	
		case '1':
			if(bToggleHelp)
			{	
				parameterUI->deActivate();	
				bToggleHelp = false;	
				ofSetWindowShape( (2.0/3.0)*(ofGetWidth()-80)+60, ofGetHeight());
			}
			else
			{	
				parameterUI->deActivate();	
				bToggleHelp = true;
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
		case ' ':
			if(bFastMode)
			{	
				bFastMode = false;	
				bToggleHelp = true;
				ofSetWindowShape(1024,768); //default size
				ofSetWindowTitle("Configuration");
			}
			else
			{	
				bFastMode = true;
				bToggleHelp = false;
				ofSetWindowShape(175,18); //minimized size
				ofSetWindowTitle("Mini");
			}
			break;
		case 'i':
			if(bInvertVideo)
				bInvertVideo = false;
			else
				bInvertVideo = true; 
			break;	
		case 'n':
			blurValue +=  2;
			if(blurValue > 255) blurValue = 255;
			break;		
		case 'm':
			blurValue -= 2;
			if(blurValue < 1) blurValue = 1;
			break;
		case 'j':
			blurGaussianValue +=  2;
			if(blurGaussianValue > 255) blurGaussianValue = 255;
			break;		
		case 'k':
			blurGaussianValue -= 2;
			if(blurGaussianValue < 1) blurGaussianValue = 1;
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
		case 'w':
			wobbleThreshold ++;
			if(wobbleThreshold > 255) wobbleThreshold = 255;
			break;	
		case 'e':
			wobbleThreshold --;
			if(wobbleThreshold < 0) wobbleThreshold = 0;
			break;	
		case '[':
			snapCounter ++;
			if(snapCounter > 16) snapCounter = 16;
			break;	
		case ']':
			snapCounter --;
			if(snapCounter < 2) snapCounter = 2;
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
		case '-':
			lowRange ++;
			if(lowRange > 255) lowRange = 255;
			break;	
		case '=':
			lowRange --;
			if(lowRange < 0) lowRange = 0;
			break;
		case '9':
			highRange ++;
			if(highRange > 255) highRange = 255;
			break;	
		case '0':
			highRange --;
			if(highRange < 0) highRange = 0;
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

/******************************************************************************
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
			newX = contourFinder.blobs[i].pts[0].x;
			newY = contourFinder.blobs[i].pts[0].y;

			if(abs(oldX-newX)<wobbleThreshold)
				newX = oldX;
			if(abs(oldY-newY)<wobbleThreshold)
				newY = oldY;

			ofxOscMessage m1;
			m1.setAddress("/tuio/2Dcur");
			m1.addStringArg("set");
			m1.addIntArg(i + 1); //id (id can't be == 0)
			m1.addFloatArg(newX/camWidth);  // x/camWidth
			m1.addFloatArg(newY/camHeight); // y/camHeight 
			m1.addFloatArg(0); //X
			m1.addFloatArg(0); //Y
			m1.addFloatArg(contourFinder.blobs[i].area); //m	
			m1.addFloatArg(contourFinder.blobs[i].boundingRect.width); // wd
			m1.addFloatArg(contourFinder.blobs[i].boundingRect.height);// ht
			TUIOSocket.sendMessage(m1 );

			//Send alive message of all alive IDs
			ofxOscMessage m2;
			m2.setAddress("/tuio/2Dcur");		
			m2.addStringArg("alive");

			for(int i=0; i<contourFinder.nBlobs; i++)
				m2.addIntArg(i + 1); //Get list of ALL active IDs		

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

		oldX = newX;
		oldY = newY;
	}
}

/******************************************************************************
 * TODO:
 *****************************************************************************/
//void testApp::fireFunction(){printf('-');}
/******************************************************************************
 * TODO:
 *****************************************************************************/
void testApp::mouseMoved(int x, int y)
{
	sprintf(eventString, "mouseMoved = (%i,%i)", x, y);
}	

/******************************************************************************
 * TODO:
 *****************************************************************************/
void testApp::mouseDragged(int x, int y, int button)
{	
	sprintf(eventString, "mouseDragged = (%i,%i - button %i)", x, y, button);
	
	//-------------------------------- BG COLOR
	//we change the background color based on 
	//the two mouse coords coming in
	float xpct = (float)x / ofGetWidth();
	float ypct = (float)y / ofGetHeight();
	red			= xpct * 255.0;
	green		= ypct * 255.0;
	blue		= (int)(red - green) % 255;
	
	//-------------------------------- PARAMETER UI EVENT
	parameterUI->mouseMotion(x, y);	
}

/******************************************************************************
 * TODO:
 *****************************************************************************/
void testApp::mousePressed(int x, int y, int button)
{
	sprintf(eventString, "mousePressed = (%i,%i - button %i)", x, y, button);
	printf("button\n", button);

	//-------------------------------- PARAMETER UI EVENT
	parameterUI->mouseDown(x, y, button);	
}

/******************************************************************************
 * TODO:
 *****************************************************************************/
void testApp::mouseReleased()
{	
	sprintf(eventString, "mouseReleased");

	//update the colors to the XML structure when the mouse is released
	XML.setValue("CONFIG:BACKGROUND:COLOR:RED", red);
	XML.setValue("CONFIG:BACKGROUND:COLOR:GREEN", green);
	XML.setValue("CONFIG:BACKGROUND:COLOR:BLUE", blue);

	//-------------------------------- PARAMETER UI EVENT
	parameterUI->mouseUp(0, 0, 0);	
}
/******************************************************************************
 * TODO:
 *****************************************************************************/
void testApp::fingerMoved(int x, int y)
{
}	

/******************************************************************************
 * TODO:
 *****************************************************************************/
void testApp::fingerDragged(int x, int y, int button)
{
}

/******************************************************************************
 * TODO:
 *****************************************************************************/
void testApp::fingerPressed(int x, int y, int button)
{
}

/******************************************************************************
 * TODO:
 *****************************************************************************/
void testApp::fingerReleased()
{
}
/******************************************************************************
 * TODO:
 *****************************************************************************/
void testApp::exit()
{
	//TODO: SEND ESC KEY TO KEEP FROM CRASHING -  can we emulate a keyboard event to trick the app into closing properly?
	printf("tBeta module has exited!\n");	
	
	// -------------------------------- SAVE STATE ON EXIT
	XML.saveFile("config.xml");
	//message ="Exited...";
}

