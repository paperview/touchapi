

#include "AParameterUIAssetDraggableSlider.h"

AParameterUIAssetDraggableSlider::AParameterUIAssetDraggableSlider()
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

	isBeingDragged = false;
}

void AParameterUIAssetDraggableSlider::init( int _x, int _y, int _width, int _height )
{

	currFrac = 0.0f;

	posx = _x;
	posy = _y;
	
	width = _width;
	height = _height;
}

void AParameterUIAssetDraggableSlider::setPos( float _frac )
{
	currFrac = _frac;
}

float AParameterUIAssetDraggableSlider::posFracInsideSlider( int _x, int _y )
{
	if( _x < posx ) { _x = posx; } else if( _x > (posx+width) ) { _x = (posx+width); }
	return (float)(_x - posx ) / (float)width;
}


void AParameterUIAssetDraggableSlider::render()
{
	glPushMatrix();

		glTranslatef( posx, posy, 0.0f );

		float tmpX = (width * currFrac);

		drawFilledQuad( 0.0f, 0.0f, width, height, bgColor );

		//drawFilledQuad( 0.0f, 0.0f, tmpX, height, 0.1f, 0.1f, 0.1f );		
		drawFilledQuad( tmpX-0.5f, 0.0f, 1.0f, height, sliderColor );

		drawOtlineQuad( 0.0f, 0.0f, width, height, outlineColor );

		//drawFilledCicle( tmpX, tmpY, 2.0f, 1.0f, 0.0f, 0.0f );

	glPopMatrix();
}


bool AParameterUIAssetDraggableSlider::pointIsInsideSlider( int _x, int _y )
{
tmpX = _x - posx;
tmpY = _y - posy;
	//cout << _x << ", " << _y << "   " << posx << ", " << posy << "   " << width << ", " << height << endl;

	if( isPointInsideRect( _x, _y, posx, posy, width, height ) ) { return true; } else { return false; }
}