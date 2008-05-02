/*
 *  AParameterUIAssetClickableCheckbox.h
 *  SDL OpenGL Template
 *
 *  Created by Andreas Muller on 15/05/2007.
 *  Copyright 2007 Nanika. All rights reserved.
 *
 */



#ifndef _A_PARAMETER_UI_ASSET_CLICKABLE_CHECKBOX_H
#define _A_PARAMETER_UI_ASSET_CLICKABLE_CHECKBOX_H

#include <sstream>
#include <string>
#include <vector>
using namespace std;

#include "AParameterUIObjectBase.h"


class AParameterUIAssetClickableCheckbox : public AParameterUIObjectBase
{

public:

	AParameterUIAssetClickableCheckbox();

	void init( int _x, int _y, int _size );

	bool pointIsInsideBox( int _x, int _y );

	void render();

	bool value;
	
	void setVal( bool _val );
	void toggle();

	int posx, posy;
	int width, height;

	float bgColor[4];
	float outlineColor[4];
	float sliderColor[4];

	int tmpX, tmpY;
};

#endif