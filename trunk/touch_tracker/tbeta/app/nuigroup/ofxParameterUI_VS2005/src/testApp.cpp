#include "testApp.h"
#include "uiDefinition.h"

//--------------------------------------------------------------
void testApp::setup(){	 
	
	setupUI();
	
	counter = 0.0;
	spin	= 0.0;
	spinPct	= 0.0;
	mouseX  = 263;
	mouseY  = 267;
	bFirstMouseMove = true;
	
	bFullscreen	= 0;
	ofSetWindowShape(1024,768);
	ofSetFullscreen(false);
	
	parameterUI = AParameterUI::Instance();
	parameterUI->init( ofGetWidth(), ofGetHeight() );

	ofBackground(0, 0, 0);	
	//for smooth animation
	ofSetVerticalSync(true);	
	logo.loadImage("images/logo.jpg");	
	thebg.loadImage("images/bg.jpg");
}

//--------------------------------------------------------------
void testApp::update(){
	counter = counter + 0.029f;
	parameterUI->update();
}

//--------------------------------------------------------------
void testApp::draw(){

	ofSetupScreen();	
		

	thebg.draw(0,0);	
	logo.draw(200,35);
	ofSetColor(0xFFFFFF);
	if(bSpaced){
		ofDrawBitmapString("press:", 135, 65);
	}

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

	gluOrtho2D(0,  ofGetWidth(), ofGetHeight() ,0);

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();	
	
	parameterUI->render();

}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
	printf(""+key);
	cout << "keyPressed = " << key << endl;
	
	if ( key == 126 || key == 96) 
	{

cout << "Here." << endl;	
	
		if( parameterUI->isActive )
		{
		
		cout << "parameterUI->isActive " << endl;	
		
			parameterUI->deActivate();	bSpaced = true;
		}
		else
		{
			parameterUI->activate();	bSpaced = false;	
		}
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){ 
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	parameterUI->mouseMotion( x, y );	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	cout << "button " << button << endl;
	parameterUI->mouseDown( x, y, button );	
}

//--------------------------------------------------------------
void testApp::mouseReleased(){
	parameterUI->mouseUp( 0, 0, 0 );	
}
