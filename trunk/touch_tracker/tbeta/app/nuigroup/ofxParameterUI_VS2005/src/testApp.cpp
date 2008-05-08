#include "testApp.h"
#include "uiDefinition.h"

//--------------------------------------------------------------
void testApp::setup(){	 
	
	setupUI();
	
	ofSetWindowShape(1024,768);
	
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


	
	parameterUI->render();

}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
	if ( key == 126 || key == 96) 
	{

//cout << "Here." << endl;	
	
		if( parameterUI->isActive )
		{
		
		//cout << "parameterUI->isActive " << endl;	
		
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
