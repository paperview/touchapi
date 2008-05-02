


#ifndef _A_PARAMETER_UI_ASSET_DRAGGABLE_SLIDER_H
#define _A_PARAMETER_UI_ASSET_DRAGGABLE_SLIDER_H

#include <sstream>
#include <string>
#include <vector>
using namespace std;

#include "AParameterUIObjectBase.h"


class AParameterUIAssetDraggableSlider : public AParameterUIObjectBase
{

public:

	AParameterUIAssetDraggableSlider();

	void init( int _x, int _y, int _width, int _height );
	void setPos( float _frac );

	bool pointIsInsideSlider( int _x, int _y );

	float posFracInsideSlider( int _x, int _y );

	void render();

	float currFrac;

	bool isBeingDragged;

	int posx, posy;
	int width, height;

	float bgColor[4];
	float outlineColor[4];
	float sliderColor[4];

	int tmpX, tmpY;
};

#endif