#include "ofMain.h"
#include "testApp.h"

int main( )
{
	ofSetupOpenGL(900,900, OF_WINDOW);	
	ofRunApp(new testApp());
}
