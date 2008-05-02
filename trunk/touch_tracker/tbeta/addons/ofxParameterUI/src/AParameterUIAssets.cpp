/*
*  AParameterUIAssets.cpp
*  SDL OpenGL Template
*
*  Created by Andreas Müller on 14/05/2007.
*  Copyright 2007 Nanika. All rights reserved.
*
*/

#include "AParameterUIAssets.h"

AParameterUIAssets* AParameterUIAssets::Instance()
{
	static AParameterUIAssets inst;
	return &inst;
}

AParameterUIAssets::AParameterUIAssets()
{
	initDisplayLists();

	TGATextureLoader tgaLoader; 
	Texture tmpFontTexture;

	tgaLoader.LoadTGAIntoTextureFromMem( &tmpFontTexture, 
		optima_14pt_bitmap_compressed, 
		optima_14pt_bitmap_compressed_size, 
		GL_LINEAR, GL_LINEAR );

	myFont = new AFont( createoptima_14ptData(), tmpFontTexture.texID, (GLuint)tmpFontTexture.width, (GLuint)tmpFontTexture.height, 0.0f );

	currTransitionParam = 1.0f;

	currID = 0;
}

int AParameterUIAssets::nextID( void )
{
	currID++;
	return currID;
}

void AParameterUIAssets::initDisplayLists()
{


	SQUARE_LIST = glGenLists(1);
	glNewList(SQUARE_LIST, GL_COMPILE);
		glBegin( GL_QUADS );
			glVertex2f( 0.0f, 1.0f );
			glVertex2f( 0.0f, 0.0f );
			glVertex2f( 1.0f, 0.0f );
			glVertex2f( 1.0f, 1.0f );
		glEnd();
	glEndList();

	SQUARE_OUTLINE_LIST = glGenLists(1);
	glNewList( SQUARE_OUTLINE_LIST, GL_COMPILE);
		glBegin( GL_LINE_LOOP );
//			glVertex2f( 0.0f, 1.0f );
			glVertex2f( 0.0f, 0.0f );
			glVertex2f( 1.0f, 0.0f );
			glVertex2f( 1.0f, 1.0f );
//			glVertex2f( 1.0f, 1.0f );

			glVertex2f( 0.0f, 1.0f );

		glEnd();
	glEndList();
	
	
	int res = 20;
	CIRCLE_LIST = glGenLists(1);

	GLint i;
	GLfloat cosine, sine;
	GLfloat fRes = (float)res;

	glNewList( CIRCLE_LIST, GL_COMPILE);
	glBegin(GL_POLYGON);
	for(i=0;i<res;i++){
		cosine=(float)cos(i*2.0*3.1415926535897932384626433832795/fRes);
		sine=(float)sin(i*2.0*3.1415926535897932384626433832795/fRes);
		glVertex2f(cosine,sine);
	}
	glEnd();
	glEndList();	

}