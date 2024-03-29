
void display(void);
void mouse_cb(int button, int state, int x, int y);
void motion_cb(int x, int y);
void passive_motion_cb(int x, int y);
void idle_cb(void);
void keyboard_cb(unsigned char key, int x, int y);
void keyboard_up_cb(unsigned char key, int x, int y);
void special_key_cb(int key, int x, int y) ;
void special_key_up_cb(int key, int x, int y) ;


//--------------------------------
static float timeNow, timeThen, fps;
static int nFramesForFPS;
static int nFrameCount = 0;
static int buttonInUse = 0;

static bool 	bFrameRateSet;
int 			millisForFrame;
int 			prevMillis;
int 			diffMillis;

static bool		newScreenMode = true;
int				requestedWidth;
int				requestedHeight;
int 			nonFullScreenX = -1;
int 			nonFullScreenY = -1;
//---------------------------------

//------------------------------------------------------------
void display(void){

	//--------------------------------
	// when I had "glutFullScreen()"
	// in the initOpenGl, I was gettings a "heap" allocation error
	// when debugging via visual studio.  putting it here, changes that.
	// maybe it's voodoo, or I am getting rid of the problem
	// by removing something unrelated, but everything seems
	// to work if I put fullscreen on the first frame of display.

	if (windowMode != OF_GAME_MODE){
		if ( newScreenMode ){
			if( windowMode == OF_FULLSCREEN){

				//----------------------------------------------------
				// before we go fullscreen, take a snapshot of where we are:
				nonFullScreenX = glutGet(GLUT_WINDOW_X);
				nonFullScreenY = glutGet(GLUT_WINDOW_Y);
				//----------------------------------------------------

				glutFullScreen();

				#ifdef TARGET_OSX
					SetSystemUIMode(kUIModeAllHidden,NULL);
				#endif

			}else if( windowMode == OF_WINDOW ){

				glutReshapeWindow(requestedWidth, requestedHeight);

				//----------------------------------------------------
				// if we have recorded the screen posion, put it there
				// if not, better to let the system do it (and put it where it wants)
				if (nFrameCount > 0){
					glutPositionWindow(nonFullScreenX,nonFullScreenY);
				}
				//----------------------------------------------------

				#ifdef TARGET_OSX
					SetSystemUIMode(kUIModeNormal,NULL);
				#endif
			}
			newScreenMode = false;
		}
	}

	width  = ofGetWidth();
	height = ofGetHeight();

	height = height > 0 ? height : 1;
	// set viewport, clear the screen
	glViewport( 0, 0, width, height );
	float * bgPtr = ofBgColorPtr();
	bool bClearAuto = ofbClearBg();

	// I don't know why, I need more than one frame at the start in fullscreen mode
	// also, in non-fullscreen mode, windows/intel graphics, this bClearAuto just fails.
	// I seem to have 2 buffers, alot of flickering
	// and don't accumulate the way I expect.
	// with this line:   if ((bClearAuto == true) || nFrameCount < 3){
	// we do nFrameCount < 3, so that the buffers are cleared at the start of the app
	// or else we have video memory garbage to draw on to...

	#ifdef TARGETWIN32
		//windows doesn't get accumulation in window mode
		if ((bClearAuto == true || windowMode == OF_WINDOW) || nFrameCount < 3){
	#else
		//mac and linux does :)
		if ( bClearAuto == true || nFrameCount < 3){
	#endif
		glClearColor(bgPtr[0],bgPtr[1],bgPtr[2], bgPtr[3]);
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	ofSetupScreen();
	OFSAptr->draw();
  	glutSwapBuffers();

  	// -------------- fps calculation:
	timeNow = ofGetElapsedTimef();
	if( (timeNow-timeThen) > 0.05f || nFramesForFPS == 0 ) {
 		fps = (double)nFramesForFPS / (timeNow-timeThen);
      	timeThen = timeNow;
		nFramesForFPS = 0;
		ofFrameRate = 0.9f * ofFrameRate + 0.1f * fps;
  	}
  	nFramesForFPS++;
  	// --------------

	nFrameCount++;		// increase the overall frame count

	setFrameNum(nFrameCount); // get this info to ofUtils for people to access

}


//------------------------------------------------------------
void mouse_cb(int button, int state, int x, int y) {


	y = height - y;


	if (nFrameCount > 0){
		OFSAptr->mouseX = x;
		OFSAptr->mouseY = height - y;

		if (state == GLUT_DOWN) {
			OFSAptr->mousePressed(x, height - y, button);
		} else if (state == GLUT_UP) {
			OFSAptr->mouseReleased();
		}
		buttonInUse = button;
	}
}

//------------------------------------------------------------
void motion_cb(int x, int y) {

	y = height - y;

	if (nFrameCount > 0){
		OFSAptr->mouseX = x;
		OFSAptr->mouseY = height - y;
		OFSAptr->mouseDragged(x, height - y, buttonInUse);
	}
}

//------------------------------------------------------------
void passive_motion_cb(int x, int y) {


	y = height - y;

	if (nFrameCount > 0){
		OFSAptr->mouseX = x;
		OFSAptr->mouseY = height - y;
		OFSAptr->mouseMoved(x, height - y);
	}
}


void idle_cb(void) {

	//	thanks to jorge for the fix:
	//	http://www.openframeworks.cc/forum/viewtopic.php?t=515&highlight=frame+rate
	
	if (nFrameCount != 0 && bFrameRateSet == true){
		diffMillis = ofGetElapsedTimeMillis() - prevMillis;
		if (diffMillis > millisForFrame){
			; // we do nothing, we are already slower than target frame
		} else {
			int waitMillis = millisForFrame - diffMillis;
			#ifdef TARGET_WIN32
				Sleep(waitMillis);         //windows sleep in milliseconds
			#else
				usleep(waitMillis * 1000);   //mac sleep in microseconds - cooler :)
			#endif
		}   
	}
	prevMillis = ofGetElapsedTimeMillis(); // you have to measure here
	OFSAptr->update();
	glutPostRedisplay();
}


//------------------------------------------------------------
void keyboard_cb(unsigned char key, int x, int y) {
	OFSAptr->keyPressed((int)key);
	if (key == 27){				// "escape"
		OF_EXIT_APP(0);
	}
}

//------------------------------------------------------------
void keyboard_up_cb(unsigned char key, int x, int y) {
	OFSAptr->keyReleased((int)key);
}

//------------------------------------------------------
void special_key_cb(int key, int x, int y) {
   OFSAptr->keyPressed((key | OF_KEY_MODIFIER));

}

//------------------------------------------------------------
void special_key_up_cb(int key, int x, int y) {
   OFSAptr->keyReleased((key | OF_KEY_MODIFIER));
}
