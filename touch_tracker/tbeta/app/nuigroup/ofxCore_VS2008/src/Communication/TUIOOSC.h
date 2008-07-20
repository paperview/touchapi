#ifndef _TUIOOSC
#define _TUIOOSC

#include "ofxCvContourFinder.h"
#define OF_ADDON_USING_OFXOSC		   // OSC COMMUNICATION		
#include "ofAddons.h"

#define HOST "localhost"
#define PORT 3333

#include <map>

class TUIOOSC {

private:

	int		   frameseq;

public:
	
	// constructors
	TUIOOSC();
	// destructor
	~TUIOOSC();
	
	// methods
	void setup();
	void update();
	void sendOSC();

	//---------------------------------------FOR NETWORK 
	ofxOscSender		TUIOSocket; 
	char				myLocalHost[255];
	char				myRemoteHost[255];
	int					myTUIOPort;	

	std::map<int, ofxCvBlob> blobs;
};

#endif