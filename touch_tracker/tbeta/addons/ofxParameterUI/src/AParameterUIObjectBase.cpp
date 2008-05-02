/*
*  AParameterUIObjectBase.cpp
*  SDL OpenGL Template
*
*  Created by Andreas Müller on 28/04/2007.
*  Copyright 2007 Nanika. All rights reserved.
*
*/

#include "AParameterUIObjectBase.h"

AParameterUIObjectBase::AParameterUIObjectBase()
{
	parameterUIAssets = AParameterUIAssets::Instance();
	objectID = parameterUIAssets->nextID();
	featureID = 0;
	
	p_boolVal  = NULL;
	p_floatVal = NULL;
	p_intVal   = NULL;	
	
	p_floatValVec0 = NULL;
	p_floatValVec1 = NULL;
	p_floatValVec2 = NULL;	
	
	//cout << "AParameterUIObjectBase contructor called" << endl;
}


string AParameterUIObjectBase::floatToString( float _num, int _floatPrecision )
{
	ostringstream tmp; tmp << _num; 
	string tmpString = tmp.str();	
	
		
	// shorten the strings so they only show 2 decimal places acuracy
	unsigned int dotPos = 0;
	dotPos = tmpString.find_first_of( "." );
	if( dotPos != std::string::npos ) 
	{
		if( dotPos+_floatPrecision < tmpString.length() ) // if we for instance requested max X.YY, but it is now X.YYY (not X.Y) 
		{
			tmpString = tmpString.substr(0, dotPos+_floatPrecision+1 );
		}
	}
	
	
	return tmpString;
}

void AParameterUIObjectBase::drawOtlineQuad( GLfloat _x, GLfloat _y, GLfloat _width, GLfloat _height, 
											 GLfloat _r, GLfloat _g, GLfloat _b, GLfloat _a )
{
	glColor4f( _r, _g, _b, _a );

	glPushMatrix();
		glTranslatef( _x,_y,0.0f);
		glScalef( _width,_height,0.0);
		glCallList( parameterUIAssets->SQUARE_OUTLINE_LIST );
	glPopMatrix();


/*
		glBegin( GL_LINE_LOOP );
			glVertex2f( _x,				_y );
			glVertex2f( _x + _width,	_y );
			glVertex2f( _x + _width,	_y + _height );
			glVertex2f( _x,				_y + _height );
		glEnd();
*/
	
}


void AParameterUIObjectBase::drawFilledQuad( GLfloat _x, GLfloat _y, GLfloat _width, GLfloat _height, 
											 GLfloat _r, GLfloat _g, GLfloat _b, GLfloat _a )
{

	glColor4f( _r, _g, _b, _a );

	glPushMatrix();
		glTranslatef( _x,_y,0.0f);
		glScalef( _width,_height,0.0);
		glCallList( parameterUIAssets->SQUARE_LIST );
	glPopMatrix();
}


void AParameterUIObjectBase::drawOtlineQuad( GLfloat _x, GLfloat _y, GLfloat _width, GLfloat _height, float col[4] )
{
	drawOtlineQuad(  _x,  _y,  _width,  _height, col[0], col[1], col[2], col[3] ); 
}

void AParameterUIObjectBase::drawFilledQuad( GLfloat _x, GLfloat _y, GLfloat _width, GLfloat _height, float col[4] )
{
	drawFilledQuad(  _x,  _y,  _width,  _height, col[0], col[1], col[2], col[3] ); 
}


void AParameterUIObjectBase::drawFilledCicle( GLfloat _x, GLfloat _y, GLfloat _size, GLfloat _r, GLfloat _g, GLfloat _b, GLfloat _a  )
{
	glColor4f( _r, _g, _b, _a );

	glPushMatrix();
		glTranslatef( _x,_y,0.0f);
		glScalef( _size,_size,0.0);
		glCallList( parameterUIAssets->CIRCLE_LIST );
	glPopMatrix();
}

void AParameterUIObjectBase::drawFilledCicle( GLfloat _x, GLfloat _y, GLfloat _size, float col[4] )
{
	drawFilledCicle( _x, _y, _size, col[0], col[1], col[2], col[3] );
}


void AParameterUIObjectBase::drawString(GLfloat x, GLfloat y, char *_string )
{
	drawString( x, y, _string, GLUT_BITMAP_HELVETICA_12 );
}

void AParameterUIObjectBase::drawString(GLfloat x, GLfloat y, char *_string, void* _font )
{
	int len, i;
	glRasterPos2f(x, y);
	len = (int) strlen( _string );
	for (i = 0; i < len; i++) { glutBitmapCharacter( _font, _string[i]); }
}

int AParameterUIObjectBase::stringWidth( char *_string, void* _font )
{
	int len, i;
	len = (int) strlen( _string );
	int width = 0;
	
	for (i = 0; i < len; i++) { width += glutBitmapWidth( _font, _string[i]); }

	return width;
}


