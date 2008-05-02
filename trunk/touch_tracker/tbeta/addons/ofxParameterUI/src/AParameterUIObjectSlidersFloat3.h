/*
 *  AParameterUIObjectSlidersFloat3.cpp
 *  SDL OpenGL Template
 *
 *  Created by Andreas Müller on 12/02/2008.
 *  Copyright 2007 Nanika. All rights reserved.
 *
 */


#ifndef _A_PARAMETER_UI_OBJECT_SLIDERS_FLOAT3_H
#define _A_PARAMETER_UI_OBJECT_SLIDERS_FLOAT3_H

#include <sstream>
#include <string>
#include <vector>
using namespace std;



#include "AParameterUIObjectBase.h"
#include "AParameterUIObjectSlidersFloat3.h"
#include "AParameterUIAssetDraggableSlider.h"
#include "AParameterUIObjectCheckbox.h"
#include "AParameterUIObjectPushButton.h"

class AParameterUIObjectSlidersFloat3 : public AParameterUIObjectBase
{

public:

	AParameterUIObjectSlidersFloat3();

	void init( string _name, int _x, int _y, int _width );


	void setRangesAndStartVal( float _low0, float _low1, float _low2, 
							   float _high0, float _high1, float _high2, 
							   float _start0, float _start1, float _start2 );

	void render();
	void update();

	void mouseMotion( int _x, int _y );
	void mouseDown( int _x, int _y, int _button );
	void mouseUp( int _x, int _y, int _button );
	
	void setValue( float _val, int _index );

	string currValuesToString();
	
	AParameterUIObjectCheckbox*			addCheckbox( string _name, bool _startVal );
	AParameterUIObjectPushButton*		addPushButton( string _name );

	int borderPixelAmount;
	
	int currChildObjectX, currChildObjectY;

	int titleHeight;
	string titlePlusValue;

	float minVal[3];
	float maxVal[3];
	float valDelta[3];
	float currVal[3];
		

	float valToFrac( float _val, int _index );
	float fracToVal( float _frac, int _index );

	AParameterUIAssetDraggableSlider sliders[3];

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