/*
 *  AParameterUIObjectPushButton.cpp
 *  SDL OpenGL Template
 *
 *  Created by Andreas Muller on 17/05/2007.
 *  Copyright 2007 Nanika. All rights reserved.
 *
 */

#include "AParameterUIObjectPushButton.h"

AParameterUIObjectPushButton::AParameterUIObjectPushButton()
{
	type = PARAMETER_UI_OBJECT_TYPE_PUSHBUTTON;

	textColor[0] = 1.0f;
	textColor[1] = 1.0f;
	textColor[2] = 1.0f;
	textColor[3] = 1.0f;
	arg1 = 0;
	arg2 = 0;	
	arg3 = 0;		
	funcPointerSet = false;
	clickStartedInsideBox = false;
	currVal = false;
}

void AParameterUIObjectPushButton::init( string _name, int _x, int _y, int _size )
{
	paramTitle = _name;

	posx = _x; 
	posy = _y;	

	checkbox.init( 0, 0, _size );
	checkbox.setVal( false );

	width = _size + 50; // really the length of the string
	height = checkbox.posy + checkbox.height;

	titleHeight = 10;
}

void AParameterUIObjectPushButton::render()
{
	glPushMatrix();

		glTranslatef( posx, posy, 0.0f );

		glColor3f( textColor[0], textColor[1], textColor[2] );
		//drawString( checkbox.width + 5, titleHeight, (char* )paramTitle.c_str(), GLUT_BITMAP_HELVETICA_10 );
		parameterUIAssets->myFont->printText2D( (char* )paramTitle.c_str(), (float)(checkbox.width + 5), (float)titleHeight, PARAMETER_UI_OBJECT_TITLE_SIZE );
		// after drawing every piece of text we re-enable BLEND as font unbind disables it
		glEnable( GL_BLEND );

		checkbox.render();

	glPopMatrix();
}

void AParameterUIObjectPushButton::mouseMotion( int _x, int _y )
{

}

void AParameterUIObjectPushButton::mouseDown( int _x, int _y, int _button )
{
	if( isPointInsideRect( _x, _y, posx, posy, width, height ) )
	{
		int insideX = _x - posx;
		int insideY = _y - posy;

		if( checkbox.pointIsInsideBox( insideX, insideY ) ) 
		{
			checkbox.setVal( true );
			clickStartedInsideBox = true;

			if( funcPointerSet )
			{
				changedParameterCallback( arg1, arg2, arg3 );
			}
		}
	}
}

void AParameterUIObjectPushButton::mouseUp( int _x, int _y, int _button )
{

	if( isPointInsideRect( _x, _y, posx, posy, width, height ) )
	{
		int insideX = _x - posx;
		int insideY = _y - posy;

		if( checkbox.pointIsInsideBox( insideX, insideY ) ) 
		{
			if( clickStartedInsideBox )
			{
				/*
				if( funcPointerSet )
				{
					changedParameterCallback( arg );
				}
				*/
			}	
		}
	}

	checkbox.setVal( false );

}
