/*
 *  AParameterUIObjectSlidersFloat2.cpp
 *  SDL OpenGL Template
 *
 *  Created by Andreas Müller on 12/02/2008.
 *  Copyright 2007 Nanika. All rights reserved.
 *
 */


#ifndef _A_PARAMETER_UI_OBJECT_SLIDERS_FLOAT2_H
#define _A_PARAMETER_UI_OBJECT_SLIDERS_FLOAT2_H

#include <sstream>
#include <string>
#include <vector>
using namespace std;



#include "AParameterUIObjectBase.h"
#include "AParameterUIObjectSlidersFloat2.h"
#include "AParameterUIAssetDraggableSlider.h"
#include "AParameterUIObjectCheckbox.h"
#include "AParameterUIObjectPushButton.h"

class AParameterUIObjectSlidersFloat2 : public AParameterUIObjectBase
{

public:

	AParameterUIObjectSlidersFloat2();

	void init( string _name, int _x, int _y, int _width );

//	void setRangesAndStartVal( Vector3D* _low,Vector3D* _high, Vector3D* _start );
	void setRangesAndStartVal( float _low0, float _low1, float _high0, float _high1, float _start0, float _start1 );

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

//	int posx, posy;
//	int width, height;
	
	int currChildObjectX, currChildObjectY;

	int titleHeight;
	string titlePlusValue;

	float minVal[2];
	float maxVal[2];
	float valDelta[2];
	float currVal[2];
		

	float valToFrac( float _val, int _index );
	float fracToVal( float _frac, int _index );

	AParameterUIAssetDraggableSlider sliders[2];

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