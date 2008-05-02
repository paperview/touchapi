/*
 *  AParameterUIObjectSliderInteger.h
 *  SDL OpenGL Template
 *
 *  Created by Andreas MŸller on 28/04/2007.
 *  Copyright 2007 Nanika. All rights reserved.
 *
 */


#ifndef _A_PARAMETER_UI_OBJECT_SLIDER_INTEGER_H
#define _A_PARAMETER_UI_OBJECT_SLIDER_INTEGER_H

#include <sstream>
#include <string>
#include <vector>
using namespace std;


#include "AParameterUIObjectBase.h"
#include "AParameterUIObjectSliderInteger.h"
#include "AParameterUIAssetDraggableSlider.h"
#include "AParameterUIObjectCheckbox.h"
#include "AParameterUIObjectPushButton.h"

class AParameterUIObjectSliderInteger : public AParameterUIObjectBase
{

public:

	AParameterUIObjectSliderInteger();

	void init( string _name, int _x, int _y, int _width, int _minVal, int _maxVal, int _startVal );

	void render();
	void update();	

	void mouseMotion( int _x, int _y );
	void mouseDown( int _x, int _y, int _button );
	void mouseUp( int _x, int _y, int _button );

	AParameterUIObjectCheckbox*			addCheckbox( string _name, bool _startVal );
	AParameterUIObjectPushButton*		addPushButton( string _name );

	void setValue( int _val );
	void setRanges( int _minVal, int _maxVal );	

	int borderPixelAmount;

//	int posx, posy;
//	int width, height;

	int currChildObjectX, currChildObjectY;

	int titleHeight;
	string titlePlusValue;

	int minVal;
	int maxVal;
	int valDelta;
	int currVal;

	float valToFrac( int _val );
	int fracToVal( float _frac );

	AParameterUIAssetDraggableSlider slider;

	bool funcPointerSet;
	void (*changedParameterCallback)( int, int, int, int ); 
	
	int arg1;
	int arg2;
	int arg3;
		
	vector< AParameterUIObjectBase* > uiObjects;

	float outlineColor[4];
	float textColor[4];

	int tmpX, tmpY;

private:



};

#endif