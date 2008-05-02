/*
 *  AParameterUIObjectSliderFloat.cpp
 *  SDL OpenGL Template
 *
 *  Created by Andreas MŸller on 28/04/2007.
 *  Copyright 2007 Nanika. All rights reserved.
 *
 */

#include "AParameterUIObjectSliderFloat.h"


AParameterUIObjectSliderFloat::AParameterUIObjectSliderFloat()
{
	type = PARAMETER_UI_OBJECT_TYPE_FLOAT_SLIDER;

	outlineColor[0] = 1.0f;
	outlineColor[1] = 1.0f;
	outlineColor[2] = 1.0f;
	outlineColor[3] = 1.0f;

	textColor[0] = 1.0f;
	textColor[1] = 1.0f;
	textColor[2] = 1.0f;
	textColor[3] = 1.0f;	

	funcPointerSet = false;
	arg1 = 0;
	arg2 = 0;
	arg3 = 0;
	titleHeight = 10;
	
	borderPixelAmount = 0;
}

void AParameterUIObjectSliderFloat::init( string _name, int _x, int _y, int _width, float _minVal, float _maxVal, float _startVal )
{
	paramTitle = _name;

	posx = _x; 
	posy = _y;

	width = _width; 
	height = 25;

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

void AParameterUIObjectSliderFloat::setValue( float _val )
{
	currVal = _val;
	slider.setPos( valToFrac( currVal ) );	
	
	ostringstream buffer; buffer << currVal; 
	titlePlusValue = paramTitle + ": " + buffer.str();
		
}

void AParameterUIObjectSliderFloat::update()
{
	if( p_floatVal != NULL )
	{
		setValue( *p_floatVal );
	}
}

void AParameterUIObjectSliderFloat::setRanges( float _minVal, float _maxVal ) 
{
	minVal  = _minVal;
	maxVal  = _maxVal;
	valDelta = maxVal - minVal;
}


void AParameterUIObjectSliderFloat::render()
{

	glPushMatrix();

		glTranslatef( posx, posy, 0.0f );

		glColor3f( textColor[0], textColor[1], textColor[2] );
		//drawString( 0, titleHeight, (char* )titlePlusValue.c_str(), GLUT_BITMAP_HELVETICA_10 );
		parameterUIAssets->myFont->printText2D( (char* )titlePlusValue.c_str(), 0.0f, (float)titleHeight, PARAMETER_UI_OBJECT_TITLE_SIZE );
		// after drawing every piece of text we re-enable BLEND as font unbind disables it
		glEnable( GL_BLEND );

		slider.render();

		//drawOtlineQuad( 0.0f, 0.0f, width, height, outlineColor );

	glPopMatrix();


	// update child objects
	AParameterUIObjectBase* tmpObj;
	for( unsigned int i = 0; i < uiObjects.size(); i++ )
	{
		tmpObj = (AParameterUIObjectBase*) uiObjects.at( i );
		tmpObj->render();
	}

}


void AParameterUIObjectSliderFloat::mouseMotion( int _x, int _y )
{

	int insideX = _x - posx;
	int insideY = _y - posy;

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
	//if( isPointInsideMe( _x, _y ) )
	{


		// update child objects
		AParameterUIObjectBase* tmpObj;
		for( unsigned int i = 0; i < uiObjects.size(); i++ )
		{
			tmpObj = (AParameterUIObjectBase*) uiObjects.at( i );
			tmpObj->mouseMotion( insideX, insideY );
		}

		outlineColor[0] = 0.5f; outlineColor[1] = 0.5f; outlineColor[2] = 1.0f;
	}
	else
	{
		outlineColor[0] = 1.0f; outlineColor[1] = 1.0f; outlineColor[2] = 1.0f;	
	}
}

void AParameterUIObjectSliderFloat::mouseDown( int _x, int _y, int _button )
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

		if( slider.pointIsInsideSlider( insideX, insideY ) ) 
		{
			//cout << _button;
			slider.isBeingDragged = true;
			mouseMotion( _x, _y ); // generate a dragged event to move the slider into this position

			// update child objects
			AParameterUIObjectBase* tmpObj;
			for( unsigned int i = 0; i < uiObjects.size(); i++ )
			{
				tmpObj = (AParameterUIObjectBase*) uiObjects.at( i );
				tmpObj->mouseDown( insideX, insideY, _button );
			}

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

void AParameterUIObjectSliderFloat::mouseUp( int _x, int _y, int _button )
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
		AParameterUIObjectBase* tmpObj;
		for( unsigned int i = 0; i < uiObjects.size(); i++ )
		{
			tmpObj = (AParameterUIObjectBase*) uiObjects.at( i );
			tmpObj->mouseUp( insideX, insideY, _button );
		}

	}

	slider.isBeingDragged = false;
}


AParameterUIObjectCheckbox*	AParameterUIObjectSliderFloat::addCheckbox( string _name, bool _startVal )
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


AParameterUIObjectPushButton* AParameterUIObjectSliderFloat::addPushButton( string _name )
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



float AParameterUIObjectSliderFloat::valToFrac( float _val )
{
	float tmp = 0.0f;
	tmp = (_val - minVal) / valDelta;
	return tmp;
}

float AParameterUIObjectSliderFloat::fracToVal( float _frac )
{
	float tmpVal = 0.0f;
	tmpVal = (valDelta * _frac) + minVal;
	return tmpVal;
}


