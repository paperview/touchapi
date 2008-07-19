#include "Thingy.h"

Thingy::Thingy(float _x, float _y, float _height, float _width) {
	printf("created thingy(x,y): %p\n", this);
	// use given starting position
	setup(_x, _y, _height, _width);
}

Thingy::~Thingy() {
	// this could be useful for whenever we get rid of an object
	printf("killed thingy: %p\n", this);
}

void Thingy::setup(float _x, float _y, float _height, float _width) {
	// set up position & size of thingy
	pos.x = _x;
	pos.y = _y;
	height = _height;
	width = _width;
	alpha = 255;
}

void Thingy::update() {

	alpha -= 25;
	height -= 3;
	if(height <= 0){height = 0;}
	width -= 3;
	if(width <= 0){width = 0;}
}


void Thingy::draw() {
	// drawing parameters
	ofEnableSmoothing();
	ofEnableAlphaBlending();
	ofSetColor( 30, 0, 200, alpha );
	// draw the thingy insides
	ofFill();
	ofEllipse( pos.x, pos.y, height, width);
	//ofDisableAlphaBlending();
	//ofDisableSmoothing();
}