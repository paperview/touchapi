/*
 *  AParameterUIObjectSliderInteger.cpp
 *  SDL OpenGL Template
 *
 *  Created by Andreas Müller on 28/04/2007.
 *  Copyright 2007 Nanika. All rights reserved.
 *
 */

#include "AParameterUIObjectSliderInteger.h"



AParameterUIObjectSliderInteger::AParameterUIObjectSliderInteger()
{
	type = PARAMETER_UI_OBJECT_TYPE_INT_SLIDER;

	outlineColor[0] = 1.0f;
	outlineColor[1] = 1.0f;
	outlineColor[2] = 1.0f;
	outlineColor[3] = 1.0f;

	textColor[0] = 1.0f;
	textColor[1] = 1.0f;
	textColor[2] = 1.0f;
	textColor[3] = 1.0f;

	tmpX = 0; tmpY = 0;

	funcPointerSet = false;
	
	arg1 = 0;
	arg2 = 0;
	arg3 = 0; 	
	
	titleHeight = 10;
	
	borderPixelAmount = 0;	
}

void AParameterUIObjectSliderInteger::init( string _name, int _x, int _y, int _width, int _minVal, int _maxVal, int _startVal )
{
	paramTitle = _name;

	posx = _x; 
	posy = _y;

	width = _width; 

	minVal  = _minVal;
	maxVal  = _maxVal;
	valDelta = maxVal - minVal;
	currVal = _startVal;

	//cout << "min: " << minVal << "  max: " << maxVal << "  delta: " << valDelta << " curr: " << currVal << "  " << (currVal/valDelta) << endl;

	slider.init( 0, titleHeight + 6, _width, 10 );

	slider.setPos( valToFrac( currVal ) );

	height = slider.posy + slider.height;
	currChildObjectX = posx;
	currChildObjectY = posy + height + 5;

	ostringstream buffer; buffer << currVal; 
	titlePlusValue = paramTitle + ": " + buffer.str();

}

void AParameterUIObjectSliderInteger::setValue( int _val )
{
	currVal = _val;
	slider.setPos( valToFrac( currVal ) );	
	
	ostringstream buffer; buffer << currVal; 
	titlePlusValue = paramTitle + ": " + buffer.str();	
}

void AParameterUIObjectSliderInteger::setRanges( int _minVal, int _maxVal ) 
{
	minVal  = _minVal;
	maxVal  = _maxVal;
	valDelta = maxVal - minVal;
}

void AParameterUIObjectSliderInteger::update()
{
	if( p_intVal != NULL )
	{
		setValue( *p_intVal );
	}
}



void AParameterUIObjectSliderInteger::render()
{

	glPushMatrix();

		glTranslatef( posx, posy, 0.0f );

		glColor3f( textColor[0], textColor[1], textColor[2] );

		//drawString( 0, titleHeight, (char* )titlePlusValue.c_str(), GLUT_BITMAP_HELVETICA_10 );
		parameterUIAssets->myFont->printText2D( (char* )titlePlusValue.c_str(), 0.0f, (float)titleHeight, PARAMETER_UI_OBJECT_TITLE_SIZE );
		// after drawing every piece of text we re-enable BLEND as font unbind disables it
		glEnable( GL_BLEND );

//		drawFilledCicle( tmpX, tmpY, 8.0f, 0.0f, 1.0f, 1.0f );

		slider.render();

//		drawOtlineQuad( 0.0f, 0.0f, width, height, outlineColor );

	glPopMatrix();

	// update child objects
	AParameterUIObjectBase* tmpObj;
	for( unsigned int i = 0; i < uiObjects.size(); i++ )
	{
		tmpObj = (AParameterUIObjectBase*) uiObjects.at( i );
		tmpObj->render();
	}

}


void AParameterUIObjectSliderInteger::mouseMotion( int _x, int _y )
{

	int insideX = _x - posx;
	int insideY = _y - posy;

tmpX = insideX;
tmpY = insideY;

	// update child objects
	AParameterUIObjectBase* tmpObj;
	for( unsigned int i = 0; i < uiObjects.size(); i++ )
	{
		tmpObj = (AParameterUIObjectBase*) uiObjects.at( i );
		tmpObj->mouseMotion( _x, _y );
	}


	if( slider.isBeingDragged )
	{
		slider.setPos( slider.posFracInsideSlider( insideX, insideY ) );
		currVal = fracToVal( slider.currFrac );

		ostringstream buffer; buffer << currVal; 
		titlePlusValue = paramTitle + ": " + buffer.str();

		if( funcPointerSet )
		{
			changedParameterCallback( currVal, arg1, arg2, arg3 );
		}
	}

	if( isPointInsideRect( _x, _y, posx, posy, width, height ) )
	{


		outlineColor[0] = 0.5f; outlineColor[1] = 0.5f; outlineColor[2] = 1.0f;
	}
	else
	{
		outlineColor[0] = 1.0f; outlineColor[1] = 1.0f; outlineColor[2] = 1.0f;	
	}


}

void AParameterUIObjectSliderInteger::mouseDown( int _x, int _y, int _button )
{

	// update child objects
	AParameterUIObjectBase* tmpObj;
	for( unsigned int i = 0; i < uiObjects.size(); i++ )
	{
		tmpObj = (AParameterUIObjectBase*) uiObjects.at( i );
		//tmpObj->mouseDown( insideX, insideY, _button );
		tmpObj->mouseDown( _x, _y, _button );
	}
	

	if( isPointInsideRect( _x, _y, posx, posy, width, height ) )
	{
		
		int insideX = _x - posx;
		int insideY = _y - posy;

		tmpX = insideX;
		tmpY = insideY;

		if( slider.pointIsInsideSlider( insideX, insideY ) ) 
		{
			//cout << _button;
			slider.isBeingDragged = true;
			mouseMotion( _x, _y ); // generate a dragged event to move the slider into this position
		}
		else
		{
			slider.isBeingDragged = false;
		}
		


	}
	else
	{
		slider.isBeingDragged = false;
	}



}

void AParameterUIObjectSliderInteger::mouseUp( int _x, int _y, int _button )
{

	// update child objects
	AParameterUIObjectBase* tmpObj;
	for( unsigned int i = 0; i < uiObjects.size(); i++ )
	{
		tmpObj = (AParameterUIObjectBase*) uiObjects.at( i );
		tmpObj->mouseUp( _x, _y, _button );
	}

	if( isPointInsideRect( _x, _y, posx, posy, width, height ) )
	{

		int insideX = _x - posx;
		int insideY = _y - posy;

		// update child objects
/*		AParameterUIObjectBase* tmpObj;
		for( int i = 0; i < uiObjects.size(); i++ )
		{
			tmpObj = (AParameterUIObjectBase*) uiObjects.at( i );
			tmpObj->mouseUp( _x - tmpObj->posx, _y - tmpObj->posy, _button );
		}
*/
	}

	slider.isBeingDragged = false;
}



AParameterUIObjectCheckbox*	AParameterUIObjectSliderInteger::addCheckbox( string _name, bool _startVal )
{
	AParameterUIObjectCheckbox* newCheckbox = new AParameterUIObjectCheckbox(); // must be created with new, AParameterUIObjectSliderFloat newSlider will NOT work.

	int _x = currChildObjectX;
	int _y = currChildObjectY;

	newCheckbox->init(  _name, _x, _y, 10, _startVal );

	uiObjects.push_back( newCheckbox );

	//currChildObjectX += 20 + stringWidth( (char* )_name.c_str(), GLUT_BITMAP_HELVETICA_10 );
	currChildObjectX += 25 + parameterUIAssets->myFont->stringWidth( (char* )_name.c_str(), PARAMETER_UI_OBJECT_TITLE_SIZE );

	height = (currChildObjectY-posy) + newCheckbox->height + borderPixelAmount; //newCheckbox->posy ;//+ newCheckbox->height;

//	adjustToNewContent( 0, newCheckbox->height );

	return newCheckbox;
}


AParameterUIObjectPushButton* AParameterUIObjectSliderInteger::addPushButton( string _name )
{
	AParameterUIObjectPushButton* newPushButton = new AParameterUIObjectPushButton(); // must be created with new, AParameterUIObjectSliderFloat newSlider will NOT work.

	int _x = currChildObjectX;
	int _y = currChildObjectY;

	newPushButton->init(  _name, _x, _y, 10 );

	uiObjects.push_back( newPushButton );

	//currChildObjectX += 20 + stringWidth( (char* )_name.c_str(), GLUT_BITMAP_HELVETICA_10 );
	currChildObjectX += 25 + parameterUIAssets->myFont->stringWidth( (char* )_name.c_str(), PARAMETER_UI_OBJECT_TITLE_SIZE );

	height = (currChildObjectY-posy) + newPushButton->height + borderPixelAmount; //newCheckbox->posy ;//+ newCheckbox->height;

	return newPushButton;
}




float AParameterUIObjectSliderInteger::valToFrac( int _val )
{
	float tmp = 0.0f;
	tmp = ((float)(_val - minVal)) / (float)valDelta;
	return tmp;
}

int AParameterUIObjectSliderInteger::fracToVal( float _frac )
{
	int tmpVal = 0;
	tmpVal = (int)(((float)valDelta * _frac) + minVal);
	return tmpVal;
}


