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
	ofxOscBundle b;

	if(blobs.size() == 0)
	{
		//Sends alive message - saying 'Hey, there's no alive blobs'
		ofxOscMessage m;
		m.setAddress("/tuio/2Dcur");		
		m.addStringArg("alive");
		b.addMessage( m ); //add message to bundle

		//Send fseq message
		//Commented out Since We're not using fseq right now
		m.clear();
		m.setAddress( "/tuio/2Dcur" );		
		m.addStringArg( "fseq" );
		m.addIntArg(frameseq);
		b.addMessage( m ); //add message to bundle
		TUIOSocket.sendBundle( b ); //send bundle
	}
	else //actually send the blobs
	{
		map<int, ofxCvBlob>::iterator this_blob;
		for(this_blob = blobs.begin(); this_blob != blobs.end(); this_blob++) 
		{		
			//Set Message
			ofxOscMessage m;
			m.setAddress("/tuio/2Dcur");
			m.addStringArg("set");
			m.addIntArg(this_blob->second.id); //id
			m.addFloatArg(this_blob->second.centroid.x);  // x
			m.addFloatArg(this_blob->second.centroid.y); // y 
			m.addFloatArg(0); //X
			m.addFloatArg(0); //Y
			m.addFloatArg(this_blob->second.area); //m	
			m.addFloatArg(this_blob->second.boundingRect.width); // wd
			m.addFloatArg(this_blob->second.boundingRect.height);// ht
			b.addMessage( m ); //add message to bundle

			//Send alive message of all alive IDs
			m.clear();
			m.setAddress("/tuio/2Dcur");		
			m.addStringArg("alive");

			std::map<int, ofxCvBlob>::iterator this_blobID;
			for(this_blobID = blobs.begin(); this_blobID != blobs.end(); this_blobID++)
			{
				m.addIntArg(this_blobID->second.id); //Get list of ALL active IDs
			}
			b.addMessage( m ); //add message to bundle	

			//Send fseq message
			//Commented out Since We're not using fseq right now
			m.clear();
			m.setAddress( "/tuio/2Dcur" );		
			m.addStringArg( "fseq" );
			m.addIntArg(frameseq);
			b.addMessage( m ); //add message to bundle
			TUIOSocket.sendBundle( b ); //send bundle			
		}
	}
}