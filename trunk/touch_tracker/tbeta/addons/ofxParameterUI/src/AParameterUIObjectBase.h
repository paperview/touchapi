/*
 *  AParameterUIObjectBase.h
 *  SDL OpenGL Template
 *
 *  Created by Andreas Müller on 28/04/2007.
 *  Copyright 2007 Nanika. All rights reserved.
 *
 */


#ifndef _A_PARAMETER_UI_OBJECT_BASE_H
#define _A_PARAMETER_UI_OBJECT_BASE_H

#include "ofConstants.h"


#include <sstream>
#include <string> 
#include <sstream>
#include <iostream>
using namespace std;

#include "AParameterUIAssets.h"
//#include "../AInputState.h"



class AParameterUIObjectBase
{
	
public:

	AParameterUIObjectBase();
	
	virtual void render() {}
	virtual void update() {}

	virtual void mouseMotion( int _x, int _y ) {}
	virtual void mouseDown( int _x, int _y, int _button ) {}
	virtual void mouseUp( int _x, int _y, int _button ) {}

	virtual void setValue( float _val ) {}
	virtual void setValue( int _val )	{}
	virtual void setValue( bool _val )	{}
	void setRanges( float _minVal, float _maxVal ) {}
	void setRanges( int _minVal, int _maxVal ) {}		

	void drawOtlineQuad( GLfloat _x, GLfloat _y, GLfloat _width, GLfloat _height, GLfloat _r = 1.0f, GLfloat _g = 1.0f, GLfloat _b = 1.0f, GLfloat _a = 1.0f );
	void drawFilledQuad( GLfloat _x, GLfloat _y, GLfloat _width, GLfloat _height, GLfloat _r = 1.0f, GLfloat _g = 1.0f, GLfloat _b = 1.0f, GLfloat _a = 1.0f );

	void drawOtlineQuad( GLfloat _x, GLfloat _y, GLfloat _width, GLfloat _height, float col[4] );
	void drawFilledQuad( GLfloat _x, GLfloat _y, GLfloat _width, GLfloat _height, float col[4] );

	void drawFilledCicle( GLfloat _x, GLfloat _y, GLfloat _size, GLfloat _r = 1.0f, GLfloat _g = 1.0f, GLfloat _b = 1.0f, GLfloat _a = 1.0f );
	void drawFilledCicle( GLfloat _x, GLfloat _y, GLfloat _size, float col[4] );


	void drawString(GLfloat x, GLfloat y, char *string, void* _font );
	void drawString(GLfloat x, GLfloat y, char *string );	

	int stringWidth( char *string, void* _font );

	bool isPointInsideRect( int _px, int _py, int _rx, int _ry, int _rw, int _rh ) 
	{ 
		if( _px >= _rx && _px <= ( _rx + _rw) ) { if( _py >= _ry && _py <= ( _ry + _rh) ) { return true; } else { return false; } }
		else { return false; }
	}

	string floatToString( float _num, int _floatPrecision = 99 );

	bool isPointInsideMe( int _x, int _y  ) 
	{ 
		/*
		int insideX = _x - posx;
		int insideY = _y - posy;
		if( isPointInsideRect( insideX, insideY, 0, 0, width, height ) ) { return true; } else { return false; }
		*/

		if( isPointInsideRect( _x, _y, posx, posy, width, height ) ) { return true; } else { return false; }
	}

	int objectID; // optional id field 
	int type;
	int featureID;

	string paramName;
	string paramTitle;

	int posx, posy;
	int width, height;

	bool*	p_boolVal;
	float*	p_floatVal;
	int*	p_intVal;	

	float*	p_floatValVec0;
	float*	p_floatValVec1;
	float*	p_floatValVec2;

	AParameterUIAssets* parameterUIAssets;

private:
	
	
};

#endif