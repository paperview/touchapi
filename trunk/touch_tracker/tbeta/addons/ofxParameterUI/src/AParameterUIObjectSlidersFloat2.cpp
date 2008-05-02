/*
 *  AParameterUIObjectSlidersFloat2.cpp
 *  SDL OpenGL Template
 *
 *  Created by Andreas Müller on 12/02/2008.
 *  Copyright 2007 Nanika. All rights reserved.
 *
 */

#include "AParameterUIObjectSlidersFloat2.h"


AParameterUIObjectSlidersFloat2::AParameterUIObjectSlidersFloat2()
{
	type = PARAMETER_UI_OBJECT_TYPE_FLOAT_SLIDER2;

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

void AParameterUIObjectSlidersFloat2::init( string _name, int _x, int _y, int _width )
{
	paramTitle = _name;

	posx = _x; 
	posy = _y;

	width = _width; 
	height = 25;


	for( int i = 0; i < 2; i++ )
	{
		sliders[i].init( 0, (titleHeight + 6) + (i*15), _width, 10 );
	}

	height = sliders[1].posy + sliders[1].height;	
	
	currChildObjectX = posx;
	currChildObjectY = posy + height + 5;	

	ostringstream buffer; buffer << currVal; 
	titlePlusValue = paramTitle + ": " + buffer.str();

}

void AParameterUIObjectSlidersFloat2::setValue( float _val, int _index )
{
	currVal[_index] = _val;
	sliders[_index].setPos( valToFrac( currVal[_index], _index ) );	
	
	titlePlusValue = paramTitle + ": " + currValuesToString();
		
}

string AParameterUIObjectSlidersFloat2::currValuesToString()
{	
	string _result = floatToString( currVal[0], 2 ) + ", " + floatToString( currVal[1], 2 );
	
	return _result;
}

void AParameterUIObjectSlidersFloat2::update()
{
	if( p_floatValVec0 != NULL ) { setValue( *p_floatValVec0, 0 ); }
	if( p_floatValVec1 != NULL ) { setValue( *p_floatValVec1, 1 ); }
}

void AParameterUIObjectSlidersFloat2::setRangesAndStartVal( float _low0, float _low1, float _high0, float _high1, float _start0, float _start1 )
{
	minVal[0]  = _low0;
	maxVal[0]  = _high0;
	valDelta[0] = maxVal[0] - minVal[0];
	currVal[0] = _start0;
	sliders[0].setPos( valToFrac( currVal[0], 0 ) );

	minVal[1]  = _low1;
	maxVal[1]  = _high1;
	valDelta[1] = maxVal[1] - minVal[1];
	currVal[1] = _start1;
	sliders[1].setPos( valToFrac( currVal[1], 1 ) );		
		
}


void AParameterUIObjectSlidersFloat2::render()
{

	glPushMatrix();

		glTranslatef( posx, posy, 0.0f );

		glColor3f( textColor[0], textColor[1], textColor[2] );
		//drawString( 0, titleHeight, (char* )titlePlusValue.c_str(), GLUT_BITMAP_HELVETICA_10 );
		parameterUIAssets->myFont->printText2D( (char* )titlePlusValue.c_str(), 0.0f, (float)titleHeight, PARAMETER_UI_OBJECT_TITLE_SIZE );
		// after drawing every piece of text we re-enable BLEND as font unbind disables it
		glEnable( GL_BLEND );

		//slider.render();
		for( int i = 0; i < 2; i++ )
		{
			sliders[i].render();
		}

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


void AParameterUIObjectSlidersFloat2::mouseMotion( int _x, int _y )
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
	
	
	for( int i = 0; i< 2; i++ )
	{
		if( sliders[i].isBeingDragged )
		{
			sliders[i].setPos( sliders[i].posFracInsideSlider( insideX, insideY ) );
			currVal[i] = fracToVal( sliders[i].currFrac, i );

			titlePlusValue = paramTitle + ": " + currValuesToString();

			if( funcPointerSet )
			{
				changedParameterCallback( currVal[i], arg1, arg2, i );
			}
		}	
	}


	if( isPointInsideRect( _x, _y, posx, posy, width, height ) )
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

void AParameterUIObjectSlidersFloat2::mouseDown( int _x, int _y, int _button )
{
	
	// update child objects
	AParameterUIObjectBase* tmpObj;
	for( unsigned int i = 0; i < uiObjects.size(); i++ )
	{
		tmpObj = (AParameterUIObjectBase*) uiObjects.at( i );
		tmpObj->mouseDown( _x, _y, _button );
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
			tmpObj->mouseDown( insideX, insideY, _button );
		}

		
		for( int i = 0; i< 2; i++ )
		{
			if( sliders[i].pointIsInsideSlider( insideX, insideY ) ) 
			{
				sliders[i].isBeingDragged = true;
				mouseMotion( _x, _y ); // generate a dragged event to move the slider into this position
			}
			else
			{
				sliders[i].isBeingDragged = false;
			}		
		}
		
	}
	else
	{
		//slider.isBeingDragged = false;
		for( int i = 0; i< 3; i++ )
		{
			sliders[i].isBeingDragged = false;
		}
	}
}

void AParameterUIObjectSlidersFloat2::mouseUp( int _x, int _y, int _button )
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

	//slider.isBeingDragged = false;
	for( int i = 0; i< 3; i++ )
	{
		sliders[i].isBeingDragged = false;
	}
}


AParameterUIObjectCheckbox*	AParameterUIObjectSlidersFloat2::addCheckbox( string _name, bool _startVal )
{
	AParameterUIObjectCheckbox* newCheckbox = new AParameterUIObjectCheckbox(); // must be created with new, AParameterUIObjectSlidersFloat2 newSlider will NOT work.
	
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


AParameterUIObjectPushButton* AParameterUIObjectSlidersFloat2::addPushButton( string _name )
{
	AParameterUIObjectPushButton* newPushButton = new AParameterUIObjectPushButton(); // must be created with new, AParameterUIObjectSlidersFloat2 newSlider will NOT work.
	
	int _x = currChildObjectX;
	int _y = currChildObjectY;
	
	newPushButton->init(  _name, _x, _y, 10 );
	
	uiObjects.push_back( newPushButton );
	
	//currChildObjectX += 20 + stringWidth( (char* )_name.c_str(), GLUT_BITMAP_HELVETICA_10 );
	currChildObjectX += 25 + parameterUIAssets->myFont->stringWidth( (char* )_name.c_str(), PARAMETER_UI_OBJECT_TITLE_SIZE );

	height = (currChildObjectY-posy) + newPushButton->height + borderPixelAmount; //newCheckbox->posy ;//+ newCheckbox->height;
	
	return newPushButton;
}



float AParameterUIObjectSlidersFloat2::valToFrac( float _val, int _index )
{
	float tmp = 0.0f;
	tmp = (_val - minVal[_index]) / valDelta[_index];
	return tmp;
}

float AParameterUIObjectSlidersFloat2::fracToVal( float _frac, int _index )
{
	float tmpVal = 0.0f;
	tmpVal = (valDelta[_index] * _frac) + minVal[_index];
	return tmpVal;
}


