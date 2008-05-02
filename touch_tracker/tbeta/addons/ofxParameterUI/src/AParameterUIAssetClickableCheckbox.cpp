/*
 *  AParameterUIAssetClickableCheckbox.cpp
 *  SDL OpenGL Template
 *
 *  Created by Andreas Muller on 15/05/2007.
 *  Copyright 2007 Nanika. All rights reserved.
 *
 */

#include "AParameterUIAssetClickableCheckbox.h"


AParameterUIAssetClickableCheckbox::AParameterUIAssetClickableCheckbox()
{
	bgColor[0] = 0.0f;
	bgColor[1] = 0.0f;
	bgColor[2] = 0.0f;
	bgColor[3] = 1.0f;

	outlineColor[0] = 1.0f;
	outlineColor[1] = 1.0f;
	outlineColor[2] = 1.0f;
	outlineColor[3] = 1.0f;

	sliderColor[0] = 1.0f;
	sliderColor[1] = 1.0f;
	sliderColor[2] = 1.0f;
	sliderColor[3] = 1.0f;

	value = false;
}

void AParameterUIAssetClickableCheckbox::init( int _x, int _y, int _size )
{

	posx = _x;
	posy = _y;
	
	width = _size;
	height = _size;
}

void AParameterUIAssetClickableCheckbox::setVal( bool _val )
{
	value = _val;
}

void AParameterUIAssetClickableCheckbox::toggle()
{
	value = !value;
}
	
void AParameterUIAssetClickableCheckbox::render()
{
	glPushMatrix();

		glTranslatef( posx, posy, 0.0f );

		drawFilledQuad( 0.0f, 0.0f, width, height, bgColor );
//		drawFilledQuad( 0.0f, 0.0f, width * currFrac, height, sliderColor );

		if( value == true ) { drawFilledQuad( 2.0f, 2.0f, width-3, height-3, sliderColor ); }

		drawOtlineQuad( 0.0f, 0.0f, width, height, outlineColor );

//		drawFilledCicle( tmpX, tmpY, 8.0f, 1.0f, 0.0f, 0.0f );

	glPopMatrix();
}


bool AParameterUIAssetClickableCheckbox::pointIsInsideBox( int _x, int _y )
{
	tmpX = _x - posx;
	tmpY = _y - posy;
	//cout << _x << ", " << _y << "   " << posx << ", " << posy << "   " << width << ", " << height << endl;

	if( isPointInsideRect( _x, _y, posx, posy, width, height ) ) { return true; } else { return false; }
}

