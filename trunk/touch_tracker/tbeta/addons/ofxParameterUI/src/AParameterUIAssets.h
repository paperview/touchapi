/*
*  AParameterUIAssets.h
*  SDL OpenGL Template
*
*  Created by Andreas Müller on 14/05/2007.
*  Copyright 2007 Nanika. All rights reserved.
*
*/


#ifndef _A_PARAMETER_UI_ASSETS_H
#define _A_PARAMETER_UI_ASSETS_H

#include "ofConstants.h"

#include <cmath>

#include "Utils/Texture.h"
#include "Utils/TGATextureLoader.h"

#include "Utils/AFont.h"

#include "FontData/optima_14pt.h"
#include "FontData/optima_14pt_bitmap_compressed.h"

#define PARAMETER_UI_OBJECT_TYPE_PANEL			0
#define PARAMETER_UI_OBJECT_TYPE_FLOAT_SLIDER	1
#define PARAMETER_UI_OBJECT_TYPE_INT_SLIDER		2
#define PARAMETER_UI_OBJECT_TYPE_CHECKBOX		3
#define PARAMETER_UI_OBJECT_TYPE_PUSHBUTTON		4
#define PARAMETER_UI_OBJECT_TYPE_FLOAT_SLIDER3D		5
#define PARAMETER_UI_OBJECT_TYPE_FLOAT_SLIDER2		6
#define PARAMETER_UI_OBJECT_TYPE_FLOAT_SLIDER3		7

#define PARAMETER_UI_OBJECT_TITLE_SIZE 17.0f
#define PARAMETER_UI_OBJECT_PANEL_TITLE_SIZE 20.0f

class AParameterUIAssets
{

public:

	static AParameterUIAssets* Instance();

	int nextID( void );

	float currTransitionParam;

	AFont* myFont;

	GLuint SQUARE_OUTLINE_LIST;
	GLuint SQUARE_LIST;
	GLuint CIRCLE_LIST;

	int currID;

private:

	AParameterUIAssets();

	void initDisplayLists();


};


#endif