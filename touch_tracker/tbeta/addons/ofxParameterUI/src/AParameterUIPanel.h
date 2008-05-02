/*
*  AParameterUIPanel.h
*  SDL OpenGL Template
*
*  Created by Andreas M¸ller on 28/04/2007.
*  Copyright 2007 Nanika. All rights reserved.
*
*/


#ifndef _A_PARAMETER_UI_PANEL_H
#define _A_PARAMETER_UI_PANEL_H

#include <sstream>
#include <string>
#include <vector>
using namespace std;

//#include "../AInputState.h"

#include "AParameterUIObjectBase.h"
#include "AParameterUIObjectSliderInteger.h"
#include "AParameterUIObjectSliderFloat.h"
#include "AParameterUIObjectCheckbox.h"
#include "AParameterUIObjectPushButton.h"
#include "AParameterUIObjectSlidersFloat2.h"
#include "AParameterUIObjectSlidersFloat3.h"

class AParameterUIPanel : public AParameterUIObjectBase
{

public:

	AParameterUIPanel();

	void init( string _name, int _x, int _y );

	void render();
	void update();

	AParameterUIObjectSliderFloat*		addSliderFloat( string _name, int _width, float _minVal, float _maxVal, float _startVal );
	AParameterUIObjectSliderInteger*	addSliderInteger( string _name, int _width, int _minVal, int _maxVal, int _startVal );	
	AParameterUIObjectCheckbox*			addCheckbox( string _name, bool _startVal );
	AParameterUIObjectPushButton*		addPushButton( string _name );
	AParameterUIObjectSlidersFloat2*	addSlidersFloat2( string _name, int _width );
	AParameterUIObjectSlidersFloat3*	addSlidersFloat3( string _name, int _width );
	

	AParameterUIObjectBase*				getObjectWithID( int _id );


	void adjustToNewContent( int _w, int _h );

	void mouseMotion( int _x, int _y );
	void mouseDown( int _x, int _y, int _button );
	void mouseUp( int _x, int _y, int _button );



	int borderPixelAmount;

	int posx, posy;
	int width, height;

	int currentUIElementsHeight;

	float bgColor[4];
	float outlineColor[4];	

	bool clickStartedInsideUs;

	int currWaitFrame;
	int framesToWaitBeforeUpdate;

	vector< AParameterUIObjectBase* > uiObjects;


private:



};

#endif