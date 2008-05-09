#include "testApp.h"
#include "uiDefinition.h"
//--------------------------------------------------------------
void testApp::setup(){	 		
	snapCounter = 6;
	frameseq = 0;

	TUIOSocket.setup( HOST, PORT ); // Set in Header

	frameRate = 120;
	camWidth = 320;	
	camHeight = 240;
	threshold = 99;
	wobbleThreshold = 5;
	blurValue = 1;
	blurGaussianValue = 1;
	lowRange = 0;
	highRange = 255;
	

	bDrawVideo = 1;
	bSnapshot = 0;
	bFastMode = 0;	
	bFullscreen	= 0;
	bDrawOutlines = 1;
	bInvertVideo = 0;
	bLearnBakground = 1;
	bTUIOMode = 0;
	bCalibration = 0;
	bVerticalMirror = 0;
	bHorizontalMirror = 0;
	
	ofSetFrameRate(frameRate);
	
	#ifdef _USE_LIVE_VIDEO
        vidGrabber.setVerbose(true);
        vidGrabber.initGrabber(camWidth,camHeight);
		printf("Camera Mode\n");
	#else
	//---------------------------------------- CHOOSE VIDEO
        vidPlayer.loadMovie("test_videos/FrontDI.m4v");
        vidPlayer.play();	
		printf("VCR Mode\n");
	#endif
    
	sourceImg.allocate(camWidth,camHeight);
	grayImage.allocate(camWidth,camHeight);
	grayBg.allocate(camWidth,camHeight);
	grayDiff.allocate(camWidth,camHeight);
	
	videoInverted 	= new unsigned char[camWidth*camHeight*3];
	videoInvertTexture.allocate(camWidth,camHeight, GL_RGB);	

//-------------------------------------------------------------- Parameter UI	
	ofSetWindowTitle("Configuration");
	printf("Touchlib application is setup!\n");

	verdana.loadFont("verdana.ttf",8, false, true);		
	logo.loadImage("images/logo.jpg");

	setupUI();	
	parameterUI = AParameterUI::Instance();
	parameterUI->init( ofGetWidth(), ofGetHeight() );
	
	//for smooth animation... should not effect video output?
	ofSetVerticalSync(true);	

}
//--------------------------------------------------------------
void testApp::update(){
	
	ofBackground(0,0,0);
    bool bNewFrame = false;
		
	#ifdef _USE_LIVE_VIDEO
       vidGrabber.grabFrame();
	   bNewFrame = vidGrabber.isFrameNew();
    #else
        vidPlayer.idleMovie();
        bNewFrame = vidPlayer.isFrameNew();
	#endif
	
	if (bNewFrame){
		#ifdef _USE_LIVE_VIDEO
		  sourceImg.setFromPixels(vidGrabber.getPixels(), camWidth,camHeight);	
		  int totalPixels = camWidth*camHeight*3;
		  unsigned char * pixels = vidGrabber.getPixels();
		
	    #else
            sourceImg.setFromPixels(vidPlayer.getPixels(), camWidth,camHeight);  
			int totalPixels = camWidth*camHeight*3;
		    unsigned char * pixels = vidPlayer.getPixels();
        #endif	
			
//---------------------------------------------------------------------------------------------------- SET FILTERS HERE
		//INVERT TEXTURE
		if (bInvertVideo){	
		for (int i = 0; i < totalPixels; i++){
			videoInverted[i] = 255 - pixels[i];
		}
		videoInvertTexture.loadData(videoInverted, camWidth,camHeight, GL_RGB);
		}		
		
		//Set Mirroring Horizontal/Vertical
		sourceImg.mirror(bHorizontalMirror, bVerticalMirror);

		grayImage = sourceImg;
			
		//Blur Video
		if(blurValue > 3){
			grayImage.blur(blurValue);
		}
		//Blur Gaussian Video
		if(blurGaussianValue > 3){
			grayImage.blurGaussian(blurGaussianValue);
		}		
		//Invert Video
		if(bInvertVideo){
			grayImage.invert();
		}
		//Create Image Level Range 0 - 255		
		grayImage.convertToRange(lowRange, highRange);
		

    	//sourceImg.erode();
        

		if (bLearnBakground == true){
			grayBg = grayImage;	
			bLearnBakground = false;
		}
	
		grayDiff.absDiff(grayBg, grayImage);
		grayDiff.threshold(threshold);
		contourFinder.findContours(grayDiff, 20, (camWidth*camHeight)/3, 10, true);	
	}
	frameseq ++;	
//-------------------------------------------------------------- Parameter UI	
	parameterUI->update();
}

//void testApp::fireFunction(){printf('fuck');}
//--------------------------------------------------------------
void testApp::draw(){
	
	//bLearnBakground = true; // ADAPTIVE BACKGROUND SUBTRATION?

	ofSetupScreen(); //? NEEDED?
	ofSetColor(0xffffff);	

	if (bDrawVideo){	
	sourceImg.draw(15,35);
	grayImage.draw(350,35);
	grayBg.draw(15,290);	
	grayDiff.draw(350,290);
	}  
	if (bDrawOutlines){	
	contourFinder.draw(15,35);
    for (int i = 0; i < contourFinder.nBlobs; i++){
    contourFinder.blobs[i].draw(350,35);		
//--------------------------------------------------------------OSC
		
		//Send Set message of ID, x, y, X, Y, m, weight, width

		newX = contourFinder.blobs[i].pts[0].x;
		newY = contourFinder.blobs[i].pts[0].y;
		
		if(abs(oldX - newX) < wobbleThreshold){

			newX = oldX;
		}
		if(abs(oldY - newY) < wobbleThreshold){

			newY = oldY;
		}

		
		ofxOscMessage m1;
		m1.setAddress( "/tuio/2Dcur" );
		m1.addStringArg( "set" );
		m1.addIntArg( i + 1 ); //id (id can't be == 0)
		m1.addFloatArg( newX/camWidth ); //  x/camWidth
		m1.addFloatArg( newY/camHeight ); // y/camHeight 
		m1.addFloatArg( 0 ); //X
		m1.addFloatArg( 0 ); //Y
		m1.addFloatArg( contourFinder.blobs[i].area ); //m	
		m1.addFloatArg( contourFinder.blobs[i].boundingRect.width ); //  wd
		m1.addFloatArg( contourFinder.blobs[i].boundingRect.height );// ht
		TUIOSocket.sendMessage( m1 );

		//Send alive message of all alive IDs
		ofxOscMessage m2;
		m2.setAddress( "/tuio/2Dcur" );		
		m2.addStringArg( "alive" );

		for (int i = 0; i < contourFinder.nBlobs; i++)	{
			m2.addIntArg( i + 1 ); //Get list of ALL active IDs		
		}		
		TUIOSocket.sendMessage( m2 );//send them		

		
		//Send fseq message
/*		Commented out Since We're not using fseq right now
		ofxOscMessage m3;
		m3.setAddress( "/tuio/2Dcur" );		
		m3.addStringArg( "fseq" );
		m3.addIntArg(frameseq);
		TUIOSocket.sendMessage( m3 );
*/

//--------------------------------------------------------------
  
		//printf("Blobs: "+ofToString(contourFinder.getBlob(i).area, 2)+"\n");	
		if(i < 1){
			if(200 < contourFinder.blobs[i].area){	
				ofSetColor(0xffffff);
				char idStr[1024];		
				sprintf(idStr, "id: %i\nx: %f\ny: %f\ncx: %f\ncy: %f\nwd: %f\nht: %f\na: %f\n", i,contourFinder.blobs[i].pts[0].x,contourFinder.blobs[i].pts[0].y,contourFinder.blobs[i].centroid.x,contourFinder.blobs[i].centroid.y,contourFinder.blobs[i].boundingRect.width,contourFinder.blobs[i].boundingRect.height,contourFinder.blobs[i].area);
				verdana.drawString(idStr,contourFinder.blobs[i].pts[0].x+contourFinder.blobs[i].boundingRect.width+30,contourFinder.blobs[i].pts[0].y+contourFinder.blobs[i].boundingRect.height);		
			}
		}
	  }

		oldX = newX;
		oldY = newY;
	} 
//-------------------------------------------------------------- continue OSC

	//If there are no blobs, send alive message and fseq
	if(contourFinder.nBlobs == 0){
		
		//Sends alive message - saying 'Hey, there's no alive blobs'
		ofxOscMessage m1;
		m1.setAddress( "/tuio/2Dcur" );		
		m1.addStringArg( "alive" );
		TUIOSocket.sendMessage( m1 );

		//Send fseq message
/*		Commented out Since We're not using fseq right now
		ofxOscMessage m2;
		m2.setAddress( "/tuio/2Dcur" );		
		m2.addStringArg( "fseq" );
		m2.addIntArg(frameseq);
		TUIOSocket.sendMessage( m2 );
*/
	}	

		
//----------------------------------------------------------------------------------CURSORS

//----------------------------------------------------------------------------------HELP

	if (bToggleHelp){		
	ofSetColor(0xffffff);	

	char reportStr[1024];	
  // sprintf(reportStr, "press '~' for help\npress ' ' for mini\npress 'f' for fullscreen\npress 't' for TUIO\npress 'o' for outlines\npress 'm' toggle DI or FTIR mode\n\npress 'c' to calibrate\npress 's' to camera setup\npress 'b' to capture bg\npress 'i' to invert\npress 'x' to set filter bg\n\npress 'a/z' to set threshold: %i\n\npress 'w/e' to set DisplacementThreshold: %i\npress 'n/m' to set Blur Amount: %i\n press 'h/v' to set Mirror Mode: None\n\nblobs found: %i\n\npress 'ESC' to exit (bug)\n", threshold, wobbleThreshold, blurhold, contourFinder.nBlobs);
    sprintf(reportStr, "press '~' for ui\npress '1' for help\npress '2' for video\npress ' ' for mini\npress 'f' for fullscreen\npress 't' for TUIO\npress 'o' for outlines\n\npress 'c' to calibrate\npress 's' to camera setup\npress 'b' to capture bg\npress 'i' to invert\npress 'x' to set filter bg\n\npress 'a/z' to set threshold: %i\n\npress 'w/e' to set DisplacementThreshold: %i\n\npress 'n/m' to set Blur Amount: %i\npress 'j/k' to set Gaussian Blur Amount: %i\npress '-/=' to set Low Level: %i\npress '9/0' to set High Level: %i\npress 'h/v' to set Mirror Mode: None\n\npress 'm' toggle DI or FTIR mode\n\npress 'ESC' to exit (bug)\n\n\nblobs found: %i", threshold, wobbleThreshold, blurValue,blurGaussianValue,lowRange,highRange, contourFinder.nBlobs);
	verdana.drawString(reportStr, 690, 45);

	verdana.drawString("Original", 33,60);
    verdana.drawString("Grayscale", 375,60);
    verdana.drawString("Background", 33,320);
	verdana.drawString("Contour", 375,320);	
	
	string fpsStr = "FPS: "+ofToString(ofGetFrameRate(), 2)+"\nCamera FPS: 34.00\nPipeline Use: 2323\n";
    verdana.drawString(fpsStr, 10,ofGetHeight()-35);	
	//verdana.drawString(eventString,80,25);

	//----------------------------------------------------------------------------------EVENTS

	//sprintf (timeString, "Time: %0.2i:%0.2i:%0.2i \nElapsed time %i", ofGetHours(), ofGetMinutes(), ofGetSeconds(), ofGetElapsedTimeMillis());

	//verdana.drawString(timeString, 98,98);
	
	//ofEnableAlphaBlending();
	//ofSetColor(255,255,255,127);   // white, 50% transparent
	//ofFill();		
	//ofCircle(mouseX,mouseY,10);	
	//ofNoFill();
	//ofDisableAlphaBlending();
	
	if (bTUIOMode){			
	ofSetColor(0xffffff);
	char buf[256];
	sprintf( buf, "Sending OSC messages to %s : %d", HOST, PORT );
	verdana.drawString( buf, 25, 575 );
//	verdana.drawString( "move the mouse to send OSC message [/tuio/2Dcur <x> <y>] ", 20, 585 );
	}//END TUIO MODE
	}	

	//------------------------------------------------------------------OSC
	// display instructions

	
	/*
	if (bSmooth){
		//ofEnableSmoothing();
		//ofDisableSmoothing();
	}
	
	if (bSnapshot == true){
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
//-----------------------------------------------------------------------------DRAW LEDS
	if (bDrawVideo){		
	ofSetColor(255,0,255);
	ofFill();		
	ofCircle(20,15,5);
	ofNoFill();
	}
	if (bTUIOMode){		
	ofSetColor(0,255,0); 
	ofFill();		
	ofCircle(35,15,5);
	ofNoFill();
	}
//-----------------------------------------------------------------------------DRAW LEDS

	if (bCalibration){		
		int screenW = ofGetWidth();
		int screenH = ofGetHeight();
		ofSetColor(0x000000);	
		ofFill();	
		ofRect(0,0,screenW,screenH);	

		//ofSetColor(0xFFFFFF);	
		//videoInvertTexture.draw(0,0,screenW,screenH);
		ofSetWindowTitle("Calibration");
		verdana.drawString("Calibration", 33,60);	
		char reportStr[1024];	
		sprintf(reportStr, "press '] or [' resize grid\npress '2' to resize bounding box\nuse arrow keys to move bounding box");
		verdana.drawString(reportStr, 700, 50);

		ofSetColor(0x00FF00);	
		int iCount = snapCounter;
		int iWidth = screenW;
		int iHeight = screenH;
		for(int i = 1; i < iCount; i++){
			ofNoFill();
			ofRect(0,2,iWidth-2,iHeight-2);
			ofLine(0, (iHeight/iCount) * i, iWidth, (iHeight/iCount) * i);
			ofLine((iWidth/iCount) * i, 0, (iWidth/iCount) * i, iHeight);
		}
	}		

//-------------------------------------------------------------- Parameter UI	
	if(!bCalibration){
		if(ofGetWidth() > 1000){
		ofSetColor(0xFFFFFF);	
		logo.draw(ofGetWidth()-155,50);
		//---------------------------
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
//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
	sprintf(eventString, "keyPressed = (%i)", key);
	//printf(int(key));
if(ofGetWidth() > 1000){
	if ( key == 126 || key == 96) 
	{
		if( parameterUI->isActive )
		{
			parameterUI->deActivate();	
			bSpaced = true;
			bToggleHelp = true;	
		}
		else
		{
			parameterUI->activate();	
			bSpaced = false;		
			bToggleHelp = false;	
		}
	}
}

	switch (key){		
		case '1':
			if(bToggleHelp){	
				parameterUI->deActivate();	
				bToggleHelp = false;	
				ofSetWindowShape(685,768);	
			}else{	
				parameterUI->deActivate();	
				bToggleHelp = true;
				ofSetWindowShape(1024,768);	
			}
			break;
		case '2':
			if(bDrawVideo){	
				bDrawVideo = false;	
			}else{	
				bDrawVideo = true;
			}
			break;
		case 'b':
			bLearnBakground = true;
			break;		
		case 'o':
			if(bDrawOutlines){	
				bDrawOutlines = false;	
			}else{	
				bDrawOutlines = true;
			}
			break;
		case 't':
			if(bTUIOMode){	
				bTUIOMode = false;		
				//ofSetWindowShape(700,600);
			}else{	
				bTUIOMode = true;	
				//ofSetWindowShape(950,700);
			}
			break;	
		case 'c':
			if(bCalibration){	
				bCalibration = false;		
			}else{	
				bCalibration = true;	
			}
			break;
		case ' ':
			if(bFastMode){	
				bFastMode = false;	
				bToggleHelp = true;
				ofSetWindowShape(1024,768);
				ofSetWindowTitle("Configuration");
			}else{	
				bFastMode = true;
				bToggleHelp = false;
				ofSetWindowShape(200,25);
				ofSetWindowTitle("~Mini");
			}
			break;
		case 'i':
			if(bInvertVideo){	
				bInvertVideo = false;
			}else{	
				bInvertVideo = true; 
			}
			break;	
		case 'n':
			blurValue +=  2;
			if (blurValue > 255) blurValue = 255;
			break;		
		case 'm':
			blurValue -= 2;
			if (blurValue < 1) blurValue = 1;
			break;
		case 'j':
			blurGaussianValue +=  2;
			if (blurGaussianValue > 255) blurGaussianValue = 255;
			break;		
		case 'k':
			blurGaussianValue -= 2;
			if (blurGaussianValue < 1) blurGaussianValue = 1;
			break;	
		case 'a':
			threshold ++;
			if (threshold > 255) threshold = 255;
			break;		
		case 'z':
			threshold --;
			if (threshold < 10) threshold = 10;
			//if (threshold < 0) threshold = 0;
			break;	
		case 'w':
			wobbleThreshold ++;
			if (wobbleThreshold > 255) wobbleThreshold = 255;
			break;	
		case 'e':
			wobbleThreshold --;
			if (wobbleThreshold < 0) wobbleThreshold = 0;
			break;	
		case '[':
			snapCounter ++;
			if (snapCounter > 16) snapCounter = 16;
			break;	
		case ']':
			snapCounter --;
			if (snapCounter < 2) snapCounter = 2;
			break;	
		case 'g':
			bSnapshot = true;
			break;
		case 's':
			#ifdef _USE_LIVE_VIDEO
			vidGrabber.videoSettings();
			#endif
			break;	
		case 'f':
		bFullscreen = !bFullscreen;
		if(!bFullscreen){
			ofSetFullscreen(false);
		} else if(bFullscreen == 1){
			ofSetFullscreen(true);
		}
		case 'h':
		if(bVerticalMirror){
			bVerticalMirror = false;
		}else{	
			bVerticalMirror = true;
		}
			break;
		case 'v':
		if(bHorizontalMirror){
			bHorizontalMirror = false;
		}else{	
			bHorizontalMirror = true;
		}
			break;
		case '-':
			lowRange ++;
			if (lowRange > 255) lowRange = 255;
			break;	
		case '=':
			lowRange --;
			if (lowRange < 0) lowRange = 0;
			break;
		case '9':
			highRange ++;
			if (highRange > 255) highRange = 255;
			break;	
		case '0':
			highRange --;
			if (highRange < 0) highRange = 0;
			break;	
	}
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	sprintf(eventString, "mouseMoved = (%i,%i)", x, y);
}	

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){	
	sprintf(eventString, "mouseDragged = (%i,%i - button %i)", x, y, button);
	parameterUI->mouseMotion( x, y );	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	sprintf(eventString, "mousePressed = (%i,%i - button %i)", x, y, button);
	//
	cout << "button " << button << endl;
	parameterUI->mouseDown( x, y, button );	
}

//--------------------------------------------------------------
void testApp::mouseReleased(){	
	sprintf(eventString, "mouseReleased");
	parameterUI->mouseUp( 0, 0, 0 );	
}
//--------------------------------------------------------------
void testApp::fingerMoved(int x, int y ){
}	

//--------------------------------------------------------------
void testApp::fingerDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::fingerPressed(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::fingerReleased(){
}
//--------------------------------------------------------------
void testApp::exit(){
	printf("tBeta application has exited!\n");	
}

