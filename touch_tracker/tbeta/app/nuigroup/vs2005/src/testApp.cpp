#include "testApp.h"
//--------------------------------------------------------------
void testApp::setup(){	 		
	
	snapCounter = 6;
	frameseq = 0;
	//ofSetCircleResolution(50);
	//int screenW = ofGetScreenWidth();
	//int screenH = ofGetScreenHeight();
	//ofSetWindowPosition(screenW/2-300/2, screenH/2-300/2);

	sender.setup( HOST, PORT ); // Set in Header

	frameRate = 120;
	camWidth = 320;	
	camHeight = 240;
	threshold = 10;
	wobbleThreshold = 5;
	blurhold = 3;
	
	bSnapshot = 0;
	bFastMode = 0;	
	bFullscreen	= 0;
	bDrawOutlines = 1;
	bInvertVideo = 0;
	bLearnBakground = 1;
	bTUIOMode = 1;
	bCalibration = 0;
	
	ofSetFrameRate(frameRate);
	
	#ifdef _USE_LIVE_VIDEO
        vidGrabber.setVerbose(true);
        vidGrabber.initGrabber(camWidth,camHeight);
		printf("Webcam Mode\n");
	#else
	//---------------------------------------- CHOOSE VIDEO
        vidPlayer.loadMovie("test_videos/hand.mp4");
        vidPlayer.play();	
		printf("File Mode\n");
	#endif
    
	colorImg.allocate(camWidth,camHeight);
	grayImage.allocate(camWidth,camHeight);
	grayBg.allocate(camWidth,camHeight);
	grayDiff.allocate(camWidth,camHeight);
	
	videoInverted 	= new unsigned char[camWidth*camHeight*3];
	videoInvertTexture.allocate(camWidth,camHeight, GL_RGB);	
		
	ofSetWindowTitle("Configuration");
	printf("Touchlib application is setup!\n");

	verdana.loadFont("verdana.ttf",8, false, true);		
	logo.loadImage("images/logo.jpg");

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
		  colorImg.setFromPixels(vidGrabber.getPixels(), camWidth,camHeight);	
		  int totalPixels = camWidth*camHeight*3;
		unsigned char * pixels = vidGrabber.getPixels();
		
		if (bInvertVideo){	
		for (int i = 0; i < totalPixels; i++){
			videoInverted[i] = 255 - pixels[i];
		}
		videoInvertTexture.loadData(videoInverted, camWidth,camHeight, GL_RGB);
		}		
	    #else
            colorImg.setFromPixels(vidPlayer.getPixels(), 320,240);
        #endif	
			

//---------------------------------------------------------------------------------------------------- SET FILTERS HERE

		//colorImg.mirror(true,true);
			if(blurhold > 3){
				//colorImg.blur(blurhold);
			}
		//colorImg.erode();


        grayImage = colorImg;

		if (bLearnBakground == true){
			grayBg = grayImage;	
			bLearnBakground = false;
		}
	
		grayDiff.absDiff(grayBg, grayImage);
		grayDiff.threshold(threshold);
		contourFinder.findContours(grayDiff, 20, (340*240)/3, 10, true);	
	}
	frameseq ++;
}

//--------------------------------------------------------------
void testApp::draw(){
	
	
	//ofSetupScreen();

	ofSetColor(0xffffff);	
	if (bInvertVideo){	
	videoInvertTexture.draw(20,40,camWidth,camHeight);
	}else{	
	colorImg.draw(20,40);
	}
	grayImage.draw(360,40);
	grayBg.draw(20,300);	
	grayDiff.draw(360,300);
    
	if (bDrawOutlines){	
	contourFinder.draw(20,40);
    for (int i = 0; i < contourFinder.nBlobs; i++){


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
		sender.sendMessage( m1 );

		//Send alive message of all alive IDs
		ofxOscMessage m2;
		m2.setAddress( "/tuio/2Dcur" );		
		m2.addStringArg( "alive" );

		for (int i = 0; i < contourFinder.nBlobs; i++)	{

			m2.addIntArg( i + 1 ); //Get list of ALL active IDs		
		}		
		sender.sendMessage( m2 );//send them		

		
		//Send fseq message
/*		Commented out Since We're not using fseq right now
		ofxOscMessage m3;
		m3.setAddress( "/tuio/2Dcur" );		
		m3.addStringArg( "fseq" );
		m3.addIntArg(frameseq);
		sender.sendMessage( m3 );
*/

//--------------------------------------------------------------
        contourFinder.blobs[i].draw(360,40);		

		//printf("Blobs: "+ofToString(contourFinder.getBlob(i).area, 2)+"\n");	
		if(i < 2){
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
		sender.sendMessage( m1 );

		//Send fseq message
/*		Commented out Since We're not using fseq right now
		ofxOscMessage m2;
		m2.setAddress( "/tuio/2Dcur" );		
		m2.addStringArg( "fseq" );
		m2.addIntArg(frameseq);
		sender.sendMessage( m2 );
*/
	}	

		
//----------------------------------------------------------------------------------CURSORS

//----------------------------------------------------------------------------------HELP

	if (bToggleHelp){		
	ofSetColor(0xffffff);	
	logo.draw(ofGetWidth()-335,25);
	char reportStr[1024];	
	sprintf(reportStr, "press '~' for help\npress ' ' for mini\npress 'f' for fullscreen\npress 't' for TUIO\npress 'o' for outlines\npress 'm' toggle DI or FTIR mode\n\npress 'c' to calibrate\npress 's' to camera setup\npress 'b' to capture bg\npress 'i' to invert\npress 'x' to set filter bg\n\npress 'a/z' to set threshold: %i\n\npress 'w/e' to set DisplacementThreshold: %i\npress 'n/m' to set Blur Amount: %i\npress 'h/v' to set Mirror Mode: None\n\nblobs found: %i\n\npress 'ESC' to exit (bug)\n", threshold, wobbleThreshold, blurhold, contourFinder.nBlobs);
	verdana.drawString(reportStr, 700, 95);
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
	if (bTUIOMode){		
	ofSetColor(0,255,0); 
	ofFill();		
	ofCircle(10,15,5);
	ofNoFill();
	}//END TUIO MODE

	if (bInvertVideo){		
	ofSetColor(255,0,255);
	ofFill();		
	ofCircle(25,15,5);
	ofNoFill();
	}
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

}
//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
	sprintf(eventString, "keyPressed = (%i)", key);
	//printf(int(key));
	switch (key){
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
		case '~':
			if(bToggleHelp){	
				bToggleHelp = false;	
				ofSetWindowShape(700,768);
			}else{	
				bToggleHelp = true;
				ofSetWindowShape(1024,768);
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
			blurhold = blurhold + 3;
			if (blurhold > 255) blurhold = 255;
			break;		
		case 'm':
			blurhold = blurhold - 3;
			if (blurhold < 0) blurhold = 0;
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
			//#ifdef _USE_LIVE_VIDEO
			//vidGrabber.videoSettings();

			break;	
		case 'f':
		bFullscreen = !bFullscreen;
		if(!bFullscreen){
			ofSetFullscreen(false);
		} else if(bFullscreen == 1){
			ofSetFullscreen(true);
		}
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
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	sprintf(eventString, "mousePressed = (%i,%i - button %i)", x, y, button);
}

//--------------------------------------------------------------
void testApp::mouseReleased(){	
	sprintf(eventString, "mouseReleased");
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
	printf("Touchlib application has exited!\n");	
}


/* -- FROM MTLIB
void frame()
	{
		if(!transmitSocket)
			return;

		// send update messages..

		char buffer[OUTPUT_BUFFER_SIZE];
		osc::OutboundPacketStream p( buffer, OUTPUT_BUFFER_SIZE );
		
		PointMap::iterator iter1, iter2, iter_last;	
		if(m_Points.size() > 0)
		{	    
			//p << osc::BeginBundleImmediate;


			int scount = 0, acount = 0;
			iter1=m_Points.begin();
			
			bool done=false;

			while(!done)
			{
				p.Clear();
				p << osc::BeginBundle();

				for(; iter1 != m_Points.end(); iter1++)
				{					
					Point d = (*iter1).second;
					float m = sqrtf((d.dx*d.dx) + (d.dy*d.dy));
					float area = 0;
					if(!(d.x == 0 && d.y == 0)) {
						p << osc::BeginMessage( "/tuio/2Dcur" ) << "set" << d.id << d.lx << d.ly << d.dx << d.dy << m << osc::EndMessage;

						scount ++;
						if(scount >= 10)
						{
							scount = 0;
							break;
						}
					}
				}

				if(iter1 == m_Points.end())
					done = true;


				p << osc::BeginMessage( "/tuio/2Dcur" );
				p << "alive";
				for(iter2=m_Points.begin(); iter2 != m_Points.end(); iter2++)
				{
					Point d = (*iter2).second;
					if(!(d.lx == 0 && d.ly == 0)) {
						p << d.id;
					}
				}
				p << osc::EndMessage;

				p << osc::BeginMessage( "/tuio/2Dcur" ) << "fseq" << frameSeq << osc::EndMessage;
				p << osc::EndBundle;

				//printf("%d size. %d #fingers\n", p.Size(), fingerList.size());
				frameSeq ++; 
				if(p.IsReady())
					transmitSocket->Send( p.Data(), p.Size() );
			}


		} else {
			//p << osc::BeginBundleImmediate;
			p.Clear();
			p << osc::BeginBundle();

			p << osc::BeginMessage( "/tuio/2Dcur" );
			p << "alive";
			p << osc::EndMessage;

			p << osc::BeginMessage( "/tuio/2Dcur" ) << "fseq" << frameSeq << osc::EndMessage;

			p << osc::EndBundle;

			frameSeq ++;
			//printf("%d size\n", p.Size());

			if(p.IsReady())
				transmitSocket->Send( p.Data(), p.Size() );


		}

	}
*/