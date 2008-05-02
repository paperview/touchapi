/*
 *  AParameterUIObjectSliderFloat.h
 *  SDL OpenGL Template
 *
 *  Created by Andreas Müller on 28/04/2007.
 *  Copyright 2007 Nanika. All rights reserved.
 *
 */



//AParameterUIObjectSliderFloat


#ifndef _A_PARAMETER_UI_OBJECT_SLIDER_FLOAT_H
#define _A_PARAMETER_UI_OBJECT_SLIDER_FLOAT_H

#include <sstream>
#include <string>
#include <vector>
using namespace std;


#include "AParameterUIObjectBase.h"
#include "AParameterUIObjectSliderFloat.h"
#include "AParameterUIAssetDraggableSlider.h"
#include "AParameterUIObjectCheckbox.h"
#include "AParameterUIObjectPushButton.h"

class AParameterUIObjectSliderFloat : public AParameterUIObjectBase
{

public:

	AParameterUIObjectSliderFloat();

	void init( string _name, int _x, int _y, int _width, float _minVal, float _maxVal, float _startVal );

	void render();
	void update();

	void mouseMotion( int _x, int _y );
	void mouseDown( int _x, int _y, int _button );
	void mouseUp( int _x, int _y, int _button );
	
	void setValue( float _val );
	void setRanges( float _minVal, float _maxVal );
	
	AParameterUIObjectCheckbox*			addCheckbox( string _name, bool _startVal );
	AParameterUIObjectPushButton*		addPushButton( string _name );

	int borderPixelAmount;

//	int posx, posy;
//	int width, height;
	
	int currChildObjectX, currChildObjectY;

	int titleHeight;
	string titlePlusValue;

	float minVal;
	float maxVal;
	float valDelta;
	float currVal;

	float valToFrac( float _val );
	float fracToVal( float _frac );

	AParameterUIAssetDraggableSlider slider;

	bool funcPointerSet;
	void (*changedParameterCallback)( float, int, int, int ); 
	
	int arg1;
	int arg2;
	int arg3;

	vector< AParameterUIObjectBase* > uiObjects;

	float outlineColor[4];
	float textColor[4];	

private:



};

#endif