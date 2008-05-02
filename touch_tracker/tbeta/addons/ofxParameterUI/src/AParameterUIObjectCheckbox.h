/*
 *  AParameterUIObjectCheckbox.h
 *  SDL OpenGL Template
 *
 *  Created by Andreas Muller on 15/05/2007.
 *  Copyright 2007 Nanika. All rights reserved.
 *
 */



#ifndef _A_PARAMETER_UI_OBJECT_CHECKBOX_H
#define _A_PARAMETER_UI_OBJECT_CHECKBOX_H


#include "AParameterUIObjectBase.h"
#include "AParameterUIAssetClickableCheckbox.h"

class AParameterUIObjectCheckbox : public AParameterUIObjectBase
{

public:

	AParameterUIObjectCheckbox();

	void init( string _name, int _x, int _y, int _size, bool _startVal );

	void render();
	void update();	

	void mouseMotion( int _x, int _y );
	void mouseDown( int _x, int _y, int _button );
	void mouseUp( int _x, int _y, int _button );

	void setValue( bool _val );

	int borderPixelAmount;

	int tmpX, tmpY;

	int titleHeight;
	string titlePlusValue;

	bool currVal;

	AParameterUIAssetClickableCheckbox checkbox;

	bool funcPointerSet;
	void (*changedParameterCallback)( bool, int, int, int ); 

	int arg1;
	int arg2;
	int arg3;
		
	float outlineColor[4];
	float textColor[4];	

private:


};


#endif