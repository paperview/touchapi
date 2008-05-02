/*
 *  AParameterUIObjectPushButton.h
 *  SDL OpenGL Template
 *
 *  Created by Andreas Muller on 17/05/2007.
 *  Copyright 2007 Nanika. All rights reserved.
 *
 */


#ifndef _A_PARAMETER_UI_OBJECT_PUSH_BUTTON_H
#define _A_PARAMETER_UI_OBJECT_PUSH_BUTTON_H


#include "AParameterUIObjectBase.h"
#include "AParameterUIAssetClickableCheckbox.h"

class AParameterUIObjectPushButton : public AParameterUIObjectBase
{

public:

	AParameterUIObjectPushButton();

	void init( string _name, int _x, int _y, int _size );

	void render();

	void mouseMotion( int _x, int _y );
	void mouseDown( int _x, int _y, int _button );
	void mouseUp( int _x, int _y, int _button );

	int borderPixelAmount;

//	int posx, posy;
//	int width, height;

	int titleHeight;
	string titlePlusValue;

	bool clickStartedInsideBox;
	bool currVal;

	AParameterUIAssetClickableCheckbox checkbox;

	bool funcPointerSet;
	void (*changedParameterCallback)( int, int, int ); 

	int arg1;
	int arg2;
	int arg3;	

	float outlineColor[4];
	float textColor[4];	

private:

};


#endif