/*
 *  AParameterUIObjectCheckbox.cpp
 *  SDL OpenGL Template
 *
 *  Created by Andreas Muller on 15/05/2007.
 *  Copyright 2007 Nanika. All rights reserved.
 *
 */

#include "AParameterUIObjectCheckbox.h"

AParameterUIObjectCheckbox::AParameterUIObjectCheckbox()
{
	type = PARAMETER_UI_OBJECT_TYPE_CHECKBOX;

	textColor[0] = 1.0f;
	textColor[1] = 1.0f;
	textColor[2] = 1.0f;
	textColor[3] = 1.0f;

	tmpX = 0; tmpY = 0;

	arg1 = 0;
	arg2 = 0;
	arg3 = 0;
	
	funcPointerSet = false;
	currVal = false;
}

void AParameterUIObjectCheckbox::init( string _name, int _x, int _y, int _size, bool _startVal )
{
	paramTitle = _name;

	posx = _x; 
	posy = _y;	
	
	checkbox.init( 0, 0, _size );
	checkbox.setVal( _startVal );
	
	width = _size + 50; // really the length of the string
	height = checkbox.posy + checkbox.height;
	
	titleHeight = 10;
	
//	checkbox.set
	
}

void AParameterUIObjectCheckbox::setValue( bool _val )
{
	currVal = _val;
	checkbox.setVal( _val );
}

void AParameterUIObjectCheckbox::render()
{
	glPushMatrix();

	glTranslatef( posx, posy, 0.0f );

	glColor3f( textColor[0], textColor[1], textColor[2] );
	//drawString( checkbox.width + 5, titleHeight, (char* )paramTitle.c_str(), GLUT_BITMAP_HELVETICA_10 );
	parameterUIAssets->myFont->printText2D( (char* )paramTitle.c_str(), (float)(checkbox.width + 5), (float)titleHeight, PARAMETER_UI_OBJECT_TITLE_SIZE);
	// after drawing every piece of text we re-enable BLEND as font unbind disables it
	glEnable( GL_BLEND );

//	drawFilledCicle( tmpX, tmpY, 3.0f, 1.0f, 0.0f, 1.0f );

	checkbox.render();

	//drawOtlineQuad( 0.0f, 0.0f, width, height, outlineColor );


	glPopMatrix();
}


void AParameterUIObjectCheckbox::update()
{
	if( p_boolVal != NULL )
	{
		setValue( *p_boolVal );
	}
}



void AParameterUIObjectCheckbox::mouseMotion( int _x, int _y )
{
//int insideX = _x; // - posx;
//int insideY = _y; // - posy;

//tmpX = insideX;
//tmpY = insideY;
//checkbox.pointIsInsideBox( tmpX, tmpY );

}

void AParameterUIObjectCheckbox::mouseDown( int _x, int _y, int _button )
{

	int insideX = _x - posx;
	int insideY = _y - posy;

	tmpX = _x;
	tmpY = _y;

	if( isPointInsideRect( _x, _y, posx, posy, width, height ) )
	{

		if( checkbox.pointIsInsideBox( insideX, insideY ) ) 
		{
			//cout << _button;
			checkbox.toggle();
			
			if( funcPointerSet )
			{
				changedParameterCallback( checkbox.value, arg1, arg2, arg3 );
			}
		}
	}

}

void AParameterUIObjectCheckbox::mouseUp( int _x, int _y, int _button )
{

}

