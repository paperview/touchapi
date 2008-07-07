#include "testApp.h"
#include "uiDefinition.h"
#include "vector2d.h"
#include "rect2d.h"
#include "tracking.h"


/******************************************************************************
 * The setup function is run once to perform initializations in the application
 *****************************************************************************/

BlobTracker			tracker;

//rect2df bBox(vector2df(0.0f,0.0f),vector2df(1.0f,1.0f));
//rect2df previewBox(vector2df(0.0f,0.0f),vector2df(0.15f,0.25f));

void testApp::setup()
{	 		

	// ---------------------------------MISC VARS FOR SETTINGS (MARKED FOR GC) 
	lastTagNumber	= 0;
	pointCount		= 0;
	lineCount		= 0;
	snapCounter		= 6; 
	frameseq		= 0;

	//Background Subtraction Learning Rate
	fLearnRate = 0.00001f;

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
        vidPlayer.loadMovie("test_videos/FrontDI.m4v");
        vidPlayer.play();	
		printf("Video Mode\n");
	#endif
    

	//Allocate images (needed for drawing/processing images) ----Most of These won't be needed in the end
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	sourceImg.allocate(camWidth, camHeight);    //Source Image
	grayImg.allocate(camWidth, camHeight);		//Gray Image
	grayBg.allocate(camWidth, camHeight);		//Background Image
	subtractBg.allocate(camWidth, camHeight);   //Background After subtraction
	grayDiff.allocate(camWidth, camHeight);		//Difference Image between Background and Source
	highpassImg.allocate(camWidth, camHeight);  //Highpass Image
	giWarped.allocate(camWidth, camHeight);     //Warped Image (used for warped calibration)

	fiLearn.allocate(camWidth, camHeight);		//ofxFloatImage used for simple dynamic background subtraction
//	fiLearn.setUseTexture(false);
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

		//Set a threshold value
		grayDiff.threshold(threshold);

		//Find contours/blobs
		contourFinder.findContours(grayDiff, 4, (camWidth*camHeight)/25, 10, false);

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
		grayDiff.draw(445, 146, 320, 240);
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
			SendOSC();
	}



	if(bDrawOutlines && !bFastMode)
	{
		//scale and draw on screen
		double wScale = double(320)/double(camWidth);
		double hScale = double(240)/double(camHeight);


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
			//drawBlob.draw(40, 146);
			drawBlob.draw(445, 146);


			//if(i < 3)
			//{
			//if(50<contourFinder.blobs[i].area)
			//{	
				if(bShowLabels)
				{
					ofSetColor(0xFFF000);
					char idStr[1024];		
					//sprintf(idStr, "id: %i\nx: %f\ny: %f\ncx: %f\nc\
					//			   y: %f\nwd: %f\nht: %f\na: %f\n",
					//			   contourFinder.blobs[i].id,
					//			   contourFinder.blobs[i].pts[0].x,
					//			   contourFinder.blobs[i].pts[0].y,
					//			   contourFinder.blobs[i].centroid.x,
					//			   contourFinder.blobs[i].centroid.y,
					//			   contourFinder.blobs[i].boundingRect.width,
					//			   contourFinder.blobs[i].boundingRect.height,
					//			   contourFinder.blobs[i].area);
					
					
				/*		CTouchEvent e;

						float tx, ty;

						tx = data.X + data.dX;
						ty = data.Y + data.dY;

						transformDimension(data.width, data.height, data.X, data.Y);
						data.area = data.width * data.height;

						cameraToScreenSpace(data.X, data.Y);
						cameraToScreenSpace(tx, ty);

						e.data = data;
						e.type = TOUCH_UPDATE;
						e.data.dX = tx - data.X;
						e.data.dY = ty - data.Y;

						eventList.push_back(e);
				*/

/*
					transformDimension(drawBlob.boundingRect.width, drawBlob.boundingRect.height, 
									   contourFinder.blobs[i].centroid.x, contourFinder.blobs[i].centroid.y);

					cameraToScreenSpace(contourFinder.blobs[i].centroid.x, contourFinder.blobs[i].centroid.y);

					//float X;
					//float Y;
      if((contourFinder.blobs[i].centroid.x * 320) >= 1 && (contourFinder.blobs[i].centroid.y * 240) >= 1){

					sprintf(idStr, "id: %i \n x: %f \n y: %f",contourFinder.blobs[i].id, 
															  (contourFinder.blobs[i].centroid.x * 320.0f), 
															  (contourFinder.blobs[i].centroid.y * 240.0f));

					printf("X: %f \n", (contourFinder.blobs[i].centroid.x * 320.0f));
					
					verdana.drawString(idStr, drawBlob.pts[0].x+drawBlob.boundingRect.width + 435,
											  drawBlob.pts[0].y+drawBlob.boundingRect.height + 120);
				}
			//}
			//}
*/		}
		}
	}



		//If there are no blobs, add to background
		
/*	if(bNewFrame){

		if (contourFinder.nBlobs == 0){		
		
		subtractBg = giWarped;

		learnBackground( subtractBg, grayBg, fiLearn, 0.01f);

		}
	}
*/




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
		ofSetColor(0x000000);	
		ofFill();	
		ofRect(0, 0, screenW, screenH);	

		//ofSetColor(0xFFFFFF);	
		//ofSetWindowTitle("Calibration");
/*		verdana.drawString("Calibration", 33, 60);	
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
			//ofNoFill();
			//ofRect(0, 2, iWidth-2, iHeight-2);
			//ofLine(0, (iHeight/iCount) * i, iWidth, (iHeight/iCount)*i);
			//ofLine((iWidth/iCount)*i, 0, (iWidth/iCount)*i, iHeight);

		}
*/
		vector2df *screenpts = getScreenPoints();

		int i;

		for(i=0; i<GRID_POINTS; i++)
		{

			if(calibrationStep == i){
				ofSetColor(0xFF0000);
				ofCircle(screenpts[i].X * (screenBB.getWidth() * screenW), screenpts[i].Y * (screenBB.getHeight() * screenH), 7);
				//ofLine(screenpts[i].X * (screenBB.getWidth() * iWidth), screenpts[i].Y * (screenBB.getHeight() * iHeight),
				//	   screenpts[i].X * (screenBB.getWidth() * iWidth) + 10, screenpts[i].Y * (screenBB.getHeight() * iHeight) + 10);
			
			/*	ofBeginShape();
			    ofVertex(400,135);
			    ofVertex(215,135);
			    ofVertex(365,25);
			    ofVertex(305,200);
			    ofVertex(250,25);
			    ofEndShape();
			*/
			}
			else{
				ofSetColor(0x00FF00);
				ofCircle(screenpts[i].X * screenW, screenpts[i].Y * screenH, 7);	
				
			}					
		}

		ofSetColor(0x00FF00);
		ofNoFill();
		ofRect(screenBB.upperLeftCorner.X * screenW, screenBB.upperLeftCorner.Y * screenH, screenBB.getWidth() * screenW, screenBB.getHeight() * screenH);
	}		

	//------------------------------------------------------------ Parameter UI
	if(!bCalibration)
	{
//		if(ofGetWidth() > 1000)
//		{
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
/* PUT THIS IN ANOTHER METHOD
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
		case 'i':
			if(bInvertVideo)
				bInvertVideo = false;
			else
				bInvertVideo = true; 
			break;	
		case 'n':
			blurValue =  1;
			if(blurValue > 255) blurValue = 255;
			break;		
		case 'm':
			blurValue -= 1;
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
			GRID_X ++;
			GRID_Y ++;
			screenBB.lowerRightCorner.X += .001;
			if(screenBB.lowerRightCorner.X > 1) screenBB.lowerRightCorner.X = 1;
			if(GRID_X > 16) GRID_X = 16; if(GRID_Y > 16) GRID_Y = 16; setGrid(GRID_X, GRID_Y);
			break;	
		case ']':
			GRID_X --;
			GRID_Y --;
			screenBB.lowerRightCorner.X -= .001;
			if(screenBB.lowerRightCorner.X < 0) screenBB.lowerRightCorner.X = 0;
			if(GRID_X < 1) GRID_X = 1; if(GRID_Y < 1) GRID_Y = 1; setGrid(GRID_X, GRID_Y);
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
/*		case '-':
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
*/		case 'l':
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
	if( XML.loadFile("config.xml") ){
		//WOOT!
		message = "Settings Loaded!";
	}else{
		//FAIL!
		message = "No Settings Found...";
		// GENERATE DEFAULT XML DATA WHICH WILL BE SAVED INTO THE CONFIG
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
	bInvertVideo		= XML.getValue("CONFIG:BOOLEAN:INVERT",0);
	bLearnBakground		= XML.getValue("CONFIG:BOOLEAN:LEARNBG",0);

	bCalibration		= XML.getValue("CONFIG:BOOLEAN:CALIBRATION",0);

	bVerticalMirror		= XML.getValue("CONFIG:BOOLEAN:VMIRROR",0);
	bHorizontalMirror	= XML.getValue("CONFIG:BOOLEAN:HMIRROR",0);	
	
	threshold			= XML.getValue("CONFIG:INT:THRESHOLD",0);
	wobbleThreshold		= XML.getValue("CONFIG:INT:WTHRESHOLD",0);
	blurValue			= XML.getValue("CONFIG:INT:BLUR",0);
	blurGaussianValue	= XML.getValue("CONFIG:INT:BLURG",0);
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
//						CALIBRATION								//
//////////////////////////////////////////////////////////////////

	bool bboxRoot = true;
	bool screenRoot = true;

	calibrationStep = 0;

	//Set grid and init everything that relates to teh grid.

	GRID_X		= XML.getValue("SCREEN:GRIDMESH:GRIDX",50);
	GRID_Y		= XML.getValue("SCREEN:GRIDMESH:GRIDX",50);

	setGrid(GRID_X, GRID_Y);
 
	
	//Bounding Box Points
	if(bboxRoot){

	    vector2df ul(XML.getValue("SCREEN:BOUNDINGBOX:ulx", 0.000000),XML.getValue("SCREEN:BOUNDINGBOX:uly", 0.000000));
	    vector2df lr(XML.getValue("SCREEN:BOUNDINGBOX:lrx", 1.000000),XML.getValue("SCREEN:BOUNDINGBOX:lry", 1.000000));
		rect2df boundingbox(ul, lr);

		setScreenBBox(boundingbox);

	}else{
		setScreenScale(1.0f);
	}

	//Calibration Points
	if(screenRoot)
	{
		//lets see how many <STROKE> </STROKE> tags there are in the xml file
		int numDragTags = XML.getNumTags("SCREEN:POINT"); 

			printf("Points: %i \n", numDragTags);

			//if there is at least one <POINT> tag we can read the list of points
			if(numDragTags > 0){

				//we push into the last POINT tag this temporarirly treats the tag as the document root.
				XML.pushTag("SCREEN:POINT", numDragTags-1);

				//we see how many points we have stored in <POINT> tags
				int numPtTags = XML.getNumTags("POINT");

			if(numPtTags > 0){

				//We then read those x y values into our array
				for(int i = 0; i < numPtTags; i++){

					//the last argument of getValue can be used to specify
					//which tag out of multiple tags you are refering to.
					int x = XML.getValue("POINT:X", 0.000000, i);
					int y = XML.getValue("POINT:Y", 0.000000, i);

					cameraPoints[i] = vector2df(x,y);
					printf("Calibration: %f, %f\n", cameraPoints[i].X, cameraPoints[i].Y);

					bscreenPoints = true;
					bcameraPoints = true;
				}
			}
			XML.popTag(); //Set XML root back to highest level
		}
	}
	//End XML Calibration Settings

}


/*****************************************************************************
 * Start of Calibration Methods
 *****************************************************************************/

//Bounding Box Size
void testApp::setScreenBBox(rect2df &box)
{
	screenBB = box;
	initScreenPoints();
}


void testApp::setGrid(int x, int y)
{
	GRID_Y = y;
	GRID_X = x;

	GRID_POINTS = ((GRID_X+1) * (GRID_Y+1));
    GRID_INDICES = (GRID_X * GRID_Y * 3 * 2);

	screenPoints = new vector2df[GRID_POINTS];
	cameraPoints = new vector2df[GRID_POINTS];
	triangles = new int[GRID_INDICES];

	initTriangles();

	if(bscreenPoints && bcameraPoints){
	initScreenPoints();
	initCameraPoints();
	}
}

void testApp::initTriangles(){

	int i,j;
	int t = 0;

	for(j=0; j<GRID_Y; j++)
	{
		for(i=0; i<GRID_X; i++)
		{
			triangles[t+0] = (i+0) + ((j+0) * (GRID_X+1));
			triangles[t+1] = (i+1) + ((j+0) * (GRID_X+1));
			triangles[t+2] = (i+0) + ((j+1) * (GRID_X+1));

			t += 3;

			triangles[t+0] = (i+1) + ((j+0) * (GRID_X+1));
			triangles[t+1] = (i+1) + ((j+1) * (GRID_X+1));
			triangles[t+2] = (i+0) + ((j+1) * (GRID_X+1));

			t += 3;
		}
	}
}


//Initialize Points
void testApp::initScreenPoints()
{
	int p = 0;

	int i,j;

	vector2df xd(screenBB.lowerRightCorner.X-screenBB.upperLeftCorner.X,0.0f);
	vector2df yd(0.0f, screenBB.lowerRightCorner.Y-screenBB.upperLeftCorner.Y);
	
	xd /= (float) GRID_X;
	yd /= (float) GRID_Y;
	
	for(j=0; j<=GRID_Y; j++)
	{
		for(i=0; i<=GRID_X; i++)
		{			 
			screenPoints[p] = screenBB.upperLeftCorner + xd*i + yd*j;			
			//printf("(%d, %d) = (%f, %f)\n", i, j, screenPoints[p].X, screenPoints[p].Y);
			p++;
		}
	}
}

void testApp::initCameraPoints()
{
	int p = 0;
	
	int i,j;
	for(j=0; j<=GRID_Y; j++)
	{
		for(i=0; i<=GRID_X; i++)
		{
			cameraPoints[p] = vector2df((i * camWidth) / (float)GRID_X, (j * camHeight) / (float)GRID_Y);
			p++;
		}
	}
}

void testApp::setScreenScale(float s)
{
	// legacy
	float offset = (1.0f - s)*0.5f;
	screenBB = rect2df(vector2df(offset,offset),vector2df(1.0f-offset,1.0f-offset));
	initScreenPoints();
}

float testApp::getScreenScale()
{
	// legacy, take the minimum scale value that fits inside the bounding box
	float minValL = MIN(screenBB.lowerRightCorner.X,screenBB.lowerRightCorner.Y);
	minValL = 1.0f - minValL;
	float minValU = MAX(screenBB.upperLeftCorner.X,screenBB.upperLeftCorner.Y);
	float minVal = MIN(minValL,minValU);
	return 1.0f - (2.0f * minVal);	
}


void testApp::transformDimension(float &width, float &height, float centerX, float centerY)
{
	// transform width/height
    float halfX = width * 0.5f;
    float halfY = height * 0.5f;

    float ulX = centerX - halfX;
    float ulY = centerY - halfY;
    float lrX = centerX + halfX;
    float lrY = centerY + halfY;

	//printf("%f:  %f:  %f:   %f:  \n", ulX, ulY, lrX, lrY);    
	
	cameraToScreenSpace(ulX, ulY);
    cameraToScreenSpace(lrX, lrY);

    width = lrX - ulX;
    height = ulY - lrY;
}

bool isPointInTriangle(vector2df p, vector2df a, vector2df b, vector2df c)
{
	if (vector2df::isOnSameSide(p,a, b,c) && vector2df::isOnSameSide(p,b, a,c) && vector2df::isOnSameSide(p, c, a, b))
		return true;
    else 
		return false;
}


int testApp::findTriangleWithin(vector2df pt)
{
	int t;

	for(t=0; t<GRID_INDICES; t+=3)
	{
		if( isPointInTriangle(pt, cameraPoints[triangles[t]], cameraPoints[triangles[t+1]], cameraPoints[triangles[t+2]]) )
		{
			return t;
		}
	}

	return -1;
}


// Transforms a camera space coordinate into a screen space coord
void testApp::cameraToScreenSpace(float &x, float &y)
{

	vector2df pt(x, y);
	
	int t = findTriangleWithin(pt);

	if(t != -1)
	{

		vector2df A = cameraPoints[triangles[t+0]];
		vector2df B = cameraPoints[triangles[t+1]];
		vector2df C = cameraPoints[triangles[t+2]];
		float total_area = (A.X - B.X) * (A.Y - C.Y) - (A.Y - B.Y) * (A.X - C.X);

		// pt,B,C
		float area_A = (pt.X - B.X) * (pt.Y - C.Y) - (pt.Y - B.Y) * (pt.X - C.X);

		// A,pt,C
		float area_B = (A.X - pt.X) * (A.Y - C.Y) - (A.Y - pt.Y) * (A.X - C.X);

		float bary_A = area_A / total_area;
		float bary_B = area_B / total_area;
		float bary_C = 1.0f - bary_A - bary_B;	// bary_A + bary_B + bary_C = 1

		vector2df sA = screenPoints[triangles[t+0]];
		vector2df sB = screenPoints[triangles[t+1]];
		vector2df sC = screenPoints[triangles[t+2]];

		vector2df transformedPos;

		transformedPos = (sA*bary_A) + (sB*bary_B) + (sC*bary_C);

		x = transformedPos.X;
		y = transformedPos.Y;
		return;
	}

	x = 0;
	y = 0;
	// FIXME: what to do in the case that it's outside the mesh?
}


void testApp::beginCalibration()
{
	 bCalibrating = true;
	 calibrationStep = 0;
}


void testApp::nextCalibrationStep()
{
	if(bCalibrating)
	{
		calibrationStep++;

		if(calibrationStep >= GRID_POINTS)
		{

			printf("Calibration complete\n");

			bCalibrating = false;
			calibrationStep = 0;
		}
	}
}

void testApp::revertCalibrationStep()
{
	if(bCalibrating)
	{
		calibrationStep--;
		if(calibrationStep < 0)
		{
			calibrationStep = 0;
		}
	}
}

/*************************
* End Calibration Methods
*************************/

/*
//! A finger is no longer active..
virtual void fingerUp(TouchData data)
{
	if(curcalib != -1){			
		
		time_t now = time(0);
		
		if((now-m_lastPress)>0){
			m_lastPress = now;
			screen->nextCalibrationStep();
			curcalib ++;

			if(curcalib >= GRID_POINTS){
				curcalib = -1;					
			}
		}
	}
}


void testApp::fingerUp(TouchData data)
{
	testApp e;

	if(bCalibrating) {
		cameraPoints[calibrationStep] = vector2df(data.X, data.Y);
		//printf("%d (%f, %f)\n", calibrationStep, data.X, data.Y);
	}
}

*/




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
	
			transformDimension(transformWidth, transformHeight, transformX, transformY);
			cameraToScreenSpace(transformX, transformY);

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




/*
void BlobTracker::downEvent(ofxCvBlob blobs)
{ 	
	//printf("Down: %i \n", blobs.id); 
	testApp::fingerPressed(blobs);
	
}
void BlobTracker::upEvent(ofxCvBlob blobs)
{ 
	//printf("UP: %i \n", blobs.id); 
	testApp::fingerReleased(blobs);	
}

void BlobTracker::moveEvent(ofxCvBlob blobs)
{
	//printf("MOVE: %i \n", blobs.id); 
	testApp::fingerMoved(blobs);

}
*/


/*****************************************************************************
 * TODO:
 *****************************************************************************/
void testApp::fingerMoved(ofxCvBlob blob)
{
	//printf("MOVE: %i \n", blob.id);  
}	

/*****************************************************************************
 * TODO:
 *****************************************************************************/
void testApp::fingerPressed(ofxCvBlob blob)
{
	printf("Down: %i \n", blob.id); 

}

/*****************************************************************************
 * TODO:
 *****************************************************************************/
void testApp::fingerReleased(ofxCvBlob blob)
{
	printf("UP: %i \n", blob.id); 

	calibrationStep = 0;

	printf("calibration step: %i", calibrationStep);

	//if(bCalibrating){			
		
		//time_t now = time(0);		
		//if((now-m_lastPress)>0){
		//	m_lastPress = now;
		    
			//cameraPoints[calibrationStep] = vector2df(blob.centroid.x, blob.centroid.y);
			//nextCalibrationStep();
			
	//		printf("%d (%f, %f)\n", calibrationStep, blob.centroid.x, blob.centroid.y);
			
		//	curcalib ++;
	//}	
}

/*****************************************************************************
 * TODO:
 *****************************************************************************/
void testApp::fingerDragged(ofxCvBlob blobs)
{
}

/*****************************************************************************
 * TODO:
 *****************************************************************************/
void testApp::exit()
{
	//TODO: SEND ESC KEY TO KEEP FROM CRASHING -  can we emulate a keyboard
	//event to trick the app into closing properly?
	printf("tBeta module has exited!\n");	
	
	// -------------------------------- SAVE STATE ON EXIT


	//lets see how many <STROKE> </STROKE> tags there are in the xml file
	int numDragTags = XML.getNumTags("SCREEN:POINT"); 

	//if there is at least one <POINT> tag we can read the list of points
	if(numDragTags > 0){

		//we push into the last POINT tag this temporarirly treats the tag as the document root.
		XML.pushTag("SCREEN:POINT", numDragTags-1);

		XML.clear();

		//Save the Grid Mesh X/Y
		XML.setValue("GRIDMESH:GRIDX", GRID_X);
	    XML.setValue("GRIDMESH:GRIDY", GRID_Y);

		//Save the Bounding Box
		XML.setValue("BOUNDINGBOX:ulx", screenBB.upperLeftCorner.X);
		XML.setValue("BOUNDINGBOX:uly", screenBB.upperLeftCorner.Y);
		XML.setValue("BOUNDINGBOX:lrx", screenBB.lowerRightCorner.X);
		XML.setValue("BOUNDINGBOX:lry", screenBB.lowerRightCorner.Y);

		//Save all the Calibration Points
		if(GRID_POINTS > 0){

			//We then read those x y values into our array
			for(int i = 0; i < GRID_POINTS; i++){

				//the last argument of getValue can be used to specify
				//which tag out of multiple tags you are refering to.
				XML.setValue("POINT:X", cameraPoints[i].X, i);
				XML.setValue("POINT:Y", cameraPoints[i].Y, i);
			}
		}
		XML.popTag(); //Set XML root back to highest level
	}

	XML.saveFile("config.xml");
	//message ="Exited...";
}

