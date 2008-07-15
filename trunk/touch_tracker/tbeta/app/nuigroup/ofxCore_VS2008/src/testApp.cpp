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
	ofSetFrameRate(30);			//This will be based on camera fps in the future		
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
		//vidPlayer.loadMovie("test_videos/HCI.mov");
		vidPlayer.loadMovie("test_videos/raw.mp4");
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
	//	FOR XML TEST
	ofBackground((int)red,(int)green,(int)blue);
 
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

		learnBackground( subtractBg, grayBg, fiLearn, 0.0001f);
 
			
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



		//Set a threshold value
		grayDiff.threshold(threshold);

		//Find contours/blobs
		contourFinder.findContours(grayDiff, 1, (camWidth*camHeight)/25, 10, false);

		//Track found contours/blobs
		tracker.track(&contourFinder);

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
void testApp::draw()
{	

//string str = "fps = ";

//str+= ofToString(ofGetFrameRate(), 2)+"fps";
//ofSetWindowTitle(str);

//	printf("Boolean: %i \n", 
//	bNewFrame);


	string str = "Tbeta: ";
	str+= ofToString(ofGetFrameRate(), 2)+" fps";
	
	string str2 = " Camera: ";
	str2+= ofToString(fps, 1)+" fps";

	ofSetWindowTitle(str + str2);

	//ofSetupScreen(); //? NEEDED?
	
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
		w = (winWidth-80)/6;
	else
		w = (winWidth-60)/6;
	
	h = (winHeight-60)/6;


	if(bDrawVideo && !bFastMode)
	{

	//	ofSetColor(0xCCCCCC);
	//	ofFill();
	//	ofRect(10, 10, w*4 + 30,h*4 + 20);

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

		m_box.draw( 40, 146 );

} 

	//first send the OSC message of the contour data
	//why is it checking if outlines are to be drawn?
	//I think it should be bTuioMode...~~~~
	//~~ we have TUIO MODE mapped to the 't' key...

	if(bNewFrame){

		if(bDrawOutlines)
		{
			SendOSC();
		}
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
					ofSetColor(0xFFF000);
					char idStr[1024];		
/*					sprintf(idStr, "id: %i\nx: %f\ny: %f\ncx: %f\nc\
								   y: %f\nwd: %f\nht: %f\na: %f\n",
								   contourFinder.blobs[i].id,
								   contourFinder.blobs[i].pts[0].x,
								   contourFinder.blobs[i].pts[0].y,
								   contourFinder.blobs[i].centroid.x,
								   contourFinder.blobs[i].centroid.y,
								   contourFinder.blobs[i].boundingRect.width,
								   contourFinder.blobs[i].boundingRect.height,
								   contourFinder.blobs[i].area);	

					//if(bNewFrame){

						calibrate.transformDimension(drawBlob.boundingRect.width, drawBlob.boundingRect.height, 
										   contourFinder.blobs[i].centroid.x, contourFinder.blobs[i].centroid.y);

						calibrate.cameraToScreenSpace(contourFinder.blobs[i].centroid.x, contourFinder.blobs[i].centroid.y);

					//}
					
					//if((contourFinder.blobs[i].centroid.x * 320) >= 1 && (contourFinder.blobs[i].centroid.y * 240) >= 1){

					sprintf(idStr, "id: %i \n x: %f \n y: %f",contourFinder.blobs[i].id, 
															  (contourFinder.blobs[i].centroid.x * 320.0f), 
															  (contourFinder.blobs[i].centroid.y * 240.0f));

					//verdana.drawString(idStr, drawBlob.pts[0].x+drawBlob.boundingRect.width + 435,
					//						  drawBlob.pts[0].y+drawBlob.boundingRect.height + 120);
*/				


				//}

			}
		}
	}


	//---------------------------------------------------------------------HELP
	// IF HELP and NO SLIDERS
/*	if(bToggleHelp && bSpaced)
	{		
	
		//-------------------------------------------- DRAW XML SETTINGS OUTPUT
		//red for error, green for good
		if(message=="No Settings Found...")
			ofSetColor(255, 0, 0);	
		else
			ofSetColor(0, 255, 0);
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
						press 'i' to invert\npress 'x' to set filter bg\n\n\
						press 'a/z' to set threshold: %i\n\n\
						press 'w/e' to set DisplacementThreshold: %i\n\n\
						press 'n/m' to set Blur Amount: %i\n\
						press 'j/k' to set Gaussian Blur Amount: %i\n\
						press '-/=' to set Highpass Blur: %i\n\
						press '9/0' to set Highpass Noise: %i\n\
						press 'h/v' to set Mirror Mode: None\n\n\
						press 'm' toggle DI or FTIR mode\n\n\
						press 'ESC' to exit (bug)\n\n\nblobs found: %i",
						threshold, wobbleThreshold, blurValue, 
						blurGaussianValue,highpassBlur,highpassNoise, 
						contourFinder.nBlobs);
		verdana.drawString(reportStr, w - 60, 45);

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
*/
	
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
    


	//-----------------------------------------------DRAW LED FOR SHOWING VIDEO
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
		
		//Change the background color when a finger presses down/up
		if(downColor){
			ofSetColor(0x2F2F2F);
		}
		else
		ofSetColor(0x000000);	

		ofFill();	
		ofRect(0, 0, screenW, screenH);	

		ofSetColor(0xFFFFFF);	
		//ofSetWindowTitle("Calibration");
		verdana.drawString("Calibration", 33, 60);	
		char reportStr[1024];	
		sprintf(reportStr, "press '] or [' resize grid\n\
							press '2' to resize bounding box\n\
							use arrow keys to move bounding box");
		verdana.drawString(reportStr, 700, 50);


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
	}		




	//------------------------------------------------------------ Parameter UI
	if(!bCalibration)
	{
			ofSetColor(0xFFFFFF);	
			logo.draw(820,5);
			
			// render the UI
			// Change to the projection matrix and set our viewing volume.
//			glMatrixMode( GL_PROJECTION );
//			glLoadIdentity();

//			gluOrtho2D(0,  ofGetWidth(), ofGetHeight() ,0);

			// We don't want to modify the projection matrix. 
//			glMatrixMode( GL_MODELVIEW );
//			glLoadIdentity();	

			parameterUI->render();
//		}
	}


}

/*****************************************************************************
 * TODO:
 *****************************************************************************/
void testApp::keyPressed(int key)
{ 
	sprintf(eventString, "keyPressed = (%i)", key);
	//printf(int(key));
//	if(ofGetWidth()>1000)
//	{
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
//	}

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
			XML.setValue("CONFIG:BOOLEAN:INVERT",bInvertVideo);
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

	// XML COLOR TEST (VERIFIES ITS WORKING!!)
	red		= XML.getValue("CONFIG:BACKGROUND:COLOR:RED", 0);
	green	= XML.getValue("CONFIG:BACKGROUND:COLOR:GREEN", 0);
	blue	= XML.getValue("CONFIG:BACKGROUND:COLOR:BLUE", 0);

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
	highpassBlur		= XML.getValue("CONFIG:INT:LOWRANGE",0);
	highpassNoise		= XML.getValue("CONFIG:INT:HIGHRANGE",0);
	highpassAmp		= XML.getValue("CONFIG:INT:HIGHPASSAMP",0);
	
//--------------------------------------------------- TODO XML NETWORK SETTINGS	
	bTUIOMode			= XML.getValue("CONFIG:BOOLEAN:TUIO",0);
	//myLocalHost			= XML.getValue("CONFIG:NETWORK:LOCALHOST",0);
	//myRemoteHost		= XML.getValue("CONFIG:NETWORK:HOSTA",0);
	//myTUIOPort			= XML.getValue("CONFIG:NETWORK:TUIO_PORT_OUT",0);
	TUIOSocket.setup(HOST, PORT); 
//-------------------------------------------------------------- 
//  END XML SETUP
//-------------------------------------------------------------- 
// NOW TO SET ALL THE XML VARS :/
//	XML.setValue("CONFIG:BACKGROUND:COLOR:RED", red);
//	XML.setValue("CONFIG:BACKGROUND:COLOR:GREEN", green);
//	XML.setValue("CONFIG:BACKGROUND:COLOR:BLUE", blue);
//-------------------------------------------------------------- 


//////////////////////////////////////////////////////////////////
//						STEUP CALIBRATION						//
//////////////////////////////////////////////////////////////////

	
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
//void testApp::fireFunction(){printf('-');}
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



void testApp::blobOn( ofxCvBlob b) { printf("Blob %i \n", b.id); downColor = true;}
void testApp::blobMoved( ofxCvBlob b) {}  

void testApp::blobOff( ofxCvBlob b) 
{
	downColor = false;

	printf("Blob Up %i \n", b.id);

	if(calibrate.bCalibrating){			
		
		//time_t now = time(0);		
		//if((now-m_lastPress)>0){
		//	m_lastPress = now;

			calibrate.cameraPoints[calibrate.calibrationStep] = vector2df(b.centroid.x, b.centroid.y);
			calibrate.nextCalibrationStep();
			
			printf("%d (%f, %f)\n", calibrate.calibrationStep, b.centroid.x, b.centroid.y);
			
		//	curcalib ++;
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

