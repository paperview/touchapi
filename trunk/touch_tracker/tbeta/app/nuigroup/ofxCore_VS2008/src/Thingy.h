#ifndef _THINGY
#define _THINGY

#include "ofMain.h"

struct Dimensions {
	float x, y;
};

class Thingy {

private:
	
public:
	
	// constructors
	Thingy(float _x, float _y, float _height, float _width);
	// destructor
	~Thingy();
	
	// methods
	void setup();
	void setup(float _x, float _y, float _height, float _width);
	void update();
	void draw();
	
	// properties
	Dimensions pos;
	float	   radius;
	float      height;
	float	   width;
	int        alpha;
};

#endif