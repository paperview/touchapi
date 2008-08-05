#include "TUIOOSC.h"

TUIOOSC::TUIOOSC() {
	printf("TUIO created: \n");
}

TUIOOSC::~TUIOOSC() {
	// this could be useful for whenever we get rid of an object
	printf("tuio killed: \n");
}

void TUIOOSC::setup(const char* host, int port) {

	printf("TUIO setup: \n");

	localHost = host;
	TUIOPort = port;

	TUIOSocket.setup(localHost, TUIOPort); 
	
	frameseq = 0;
}

void TUIOOSC::update() {

	frameseq += 1;
}

void TUIOOSC::sendOSC()
{	
	//If there are no blobs, send alive message and fseq
	if(blobs.size() == 0)
	{
		//Sends alive message - saying 'Hey, there's no alive blobs'
		ofxOscMessage m1;
		m1.setAddress("/tuio/2Dcur");		
		m1.addStringArg("alive");
		TUIOSocket.sendMessage(m1);

		//Send fseq message
		//Commented out Since We're not using fseq right now
		ofxOscMessage m2;
		m2.setAddress( "/tuio/2Dcur" );		
		m2.addStringArg( "fseq" );
		m2.addIntArg(frameseq);
		TUIOSocket.sendMessage( m2 );
	}
	else //actually send the blobs
	{
		map<int, ofxCvBlob>::iterator this_blob;
		for(this_blob = blobs.begin(); this_blob != blobs.end(); this_blob++) 
		{		
			//Set Message
			ofxOscMessage m1;
			m1.setAddress("/tuio/2Dcur");
			m1.addStringArg("set");
			m1.addIntArg(this_blob->second.id); //id
			m1.addFloatArg(this_blob->second.centroid.x);  // x
			m1.addFloatArg(this_blob->second.centroid.y); // y 
			m1.addFloatArg(0); //X
			m1.addFloatArg(0); //Y
			m1.addFloatArg(this_blob->second.area); //m	
			m1.addFloatArg(this_blob->second.boundingRect.width); // wd
			m1.addFloatArg(this_blob->second.boundingRect.height);// ht
			TUIOSocket.sendMessage(m1);

			//Send alive message of all alive IDs
			ofxOscMessage m2;
			m2.setAddress("/tuio/2Dcur");		
			m2.addStringArg("alive");

			std::map<int, ofxCvBlob>::iterator this_blobID;
			for(this_blobID = blobs.begin(); this_blobID != blobs.end(); this_blobID++)
			{
				m2.addIntArg(this_blobID->second.id); //Get list of ALL active IDs
			}
			TUIOSocket.sendMessage(m2);//send them		

			//Send fseq message
			//Commented out Since We're not using fseq right now
			ofxOscMessage m3;
			m3.setAddress( "/tuio/2Dcur" );		
			m3.addStringArg( "fseq" );
			m3.addIntArg(frameseq);
			TUIOSocket.sendMessage( m3 );			
		}
	}
}