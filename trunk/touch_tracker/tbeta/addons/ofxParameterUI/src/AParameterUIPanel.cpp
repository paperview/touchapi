/*
*  AParameterUIPanel.cpp
*  SDL OpenGL Template
*
*  Created by Andreas M¸ller on 28/04/2007.
*  Copyright 2007 Nanika. All rights reserved.
*
*/

#include "AParameterUIPanel.h"

AParameterUIPanel::AParameterUIPanel()
{
	borderPixelAmount = 15;
	currentUIElementsHeight = borderPixelAmount;
	
	bgColor[0] = 0.0f;
	bgColor[1] = 0.0f;
	bgColor[2] = 0.0f;
	bgColor[3] = 0.6f;
	
	outlineColor[0] = 1.0f;		
	outlineColor[1] = 1.0f;
	outlineColor[2] = 1.0f;
	outlineColor[3] = 1.0f;			

	currWaitFrame = 0;
	framesToWaitBeforeUpdate = 5;

	clickStartedInsideUs = false;

	//cout << "AParameterUIPanel constructor called" << endl;
}

void AParameterUIPanel::init( string _name, int _x, int _y )
{
	paramTitle = _name;

	posx = _x; 
	posy = _y;

	width = 200; 
	height = 300;

	//currentUIElementsHeight += borderPixelAmount;

	if( paramTitle != "" ) currentUIElementsHeight += 36;
}


void AParameterUIPanel::update()
{
	currWaitFrame++;
	
	if ( currWaitFrame > framesToWaitBeforeUpdate) 
	{
		AParameterUIObjectBase* tmpObj = NULL;
		for( unsigned int i = 0; i < uiObjects.size(); i++ )
		{
			tmpObj = (AParameterUIObjectBase*) uiObjects.at( i );
			tmpObj->update();
		}
		
		currWaitFrame = 0;
	}
	
}

void AParameterUIPanel::render()
{


	float tmpS = parameterUIAssets->currTransitionParam;
	float sInv = 1.0f - tmpS;

	glPushMatrix();

		glTranslatef( posx + ((width/2.0f)*sInv), posy + ((height/2.0f)*sInv), 0.0f );

		glScalef(tmpS, tmpS, tmpS);

		drawFilledQuad( 0.0f, 0.0f, width, height, bgColor );
		drawOtlineQuad( 0.0f, 0.0f, width, height, outlineColor );	

		glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
		//if( paramTitle != "" ) drawString( borderPixelAmount, 18 + borderPixelAmount, (char* )paramTitle.c_str(), GLUT_BITMAP_HELVETICA_18 );
		if( paramTitle != "" ) parameterUIAssets->myFont->printText2D( (char* )paramTitle.c_str(), (float)borderPixelAmount, 18.0f + (float)borderPixelAmount, PARAMETER_UI_OBJECT_PANEL_TITLE_SIZE );
	
		// after drawing every piece of text we re-enable BLEND as font unbind disables it
		glEnable( GL_BLEND );


		AParameterUIObjectBase* tmpObj;
		for( unsigned int i = 0; i < uiObjects.size(); i++ )
		{
			tmpObj = (AParameterUIObjectBase*) uiObjects.at( i );
			tmpObj->render();
		}

	glPopMatrix();
}


void AParameterUIPanel::adjustToNewContent( int _w, int _h )
{
	//currentUIElementsHeight += newSlider->height + borderPixelAmount;
	if( width < _w + (borderPixelAmount*2) ) { width = _w + (borderPixelAmount*2); }
	currentUIElementsHeight = _h + borderPixelAmount;	
	height = currentUIElementsHeight + borderPixelAmount;
}



AParameterUIObjectSliderFloat* AParameterUIPanel::addSliderFloat( string _name, int _width, float _minVal, float _maxVal, float _startVal )
{
	// make sure we move down in case we've added some sub children
	if( uiObjects.size() > 0 )
	{
		AParameterUIObjectBase* tmpObj = (AParameterUIObjectBase*) uiObjects.at( uiObjects.size() -1 );
		adjustToNewContent( 0, tmpObj->posy + tmpObj->height );
	}
	AParameterUIObjectSliderFloat* newSlider = new AParameterUIObjectSliderFloat(); // must be created with new, AParameterUIObjectSliderFloat newSlider will NOT work.
	int _x = borderPixelAmount;
	int _y = currentUIElementsHeight;
	newSlider->init(  _name, _x, _y, _width, _minVal, _maxVal, _startVal );
	uiObjects.push_back( newSlider );
	//currentUIElementsHeight += newSlider->height + borderPixelAmount;
	adjustToNewContent( _width, newSlider->posy + newSlider->height );
	return newSlider;
}

AParameterUIObjectSliderInteger* AParameterUIPanel::addSliderInteger( string _name, int _width, int _minVal, int _maxVal, int _startVal )
{
	// make sure we move down in case we've added some sub children
	if( uiObjects.size() > 0 )
	{
		AParameterUIObjectBase* tmpObj = (AParameterUIObjectBase*) uiObjects.at( uiObjects.size() -1 );
		adjustToNewContent( 0,  tmpObj->posy + tmpObj->height );
	}
	AParameterUIObjectSliderInteger* newSlider = new AParameterUIObjectSliderInteger(); // must be created with new, AParameterUIObjectSliderFloat newSlider will NOT work.
	int _x = borderPixelAmount;
	int _y = currentUIElementsHeight;
	newSlider->init(  _name, _x, _y, _width, _minVal, _maxVal, _startVal );
	uiObjects.push_back( newSlider );
	//currentUIElementsHeight += newSlider->height + borderPixelAmount;
	adjustToNewContent( _width, newSlider->posy + newSlider->height );
	return newSlider;
}

AParameterUIObjectCheckbox*	AParameterUIPanel::addCheckbox( string _name, bool _startVal )
{
	// make sure we move down in case we've added some sub children
	if( uiObjects.size() > 0 )
	{
		AParameterUIObjectBase* tmpObj = (AParameterUIObjectBase*) uiObjects.at( uiObjects.size() -1 );
		adjustToNewContent( 0, tmpObj->posy + tmpObj->height );
	}
	AParameterUIObjectCheckbox* newCheckbox = new AParameterUIObjectCheckbox(); // must be created with new, AParameterUIObjectSliderFloat newSlider will NOT work.
	int _x = borderPixelAmount;
	int _y = currentUIElementsHeight;
	newCheckbox->init(  _name, _x, _y, 10, _startVal );
	uiObjects.push_back( newCheckbox );
	//currentUIElementsHeight += newCheckbox->height + borderPixelAmount;
	adjustToNewContent( 0, newCheckbox->posy + newCheckbox->height );
	return newCheckbox;
}


AParameterUIObjectPushButton* AParameterUIPanel::addPushButton( string _name )
{
	// make sure we move down in case we've added some sub children
	if( uiObjects.size() > 0 )
	{
		AParameterUIObjectBase* tmpObj = (AParameterUIObjectBase*) uiObjects.at( uiObjects.size() -1 );
		adjustToNewContent( 0, tmpObj->posy + tmpObj->height );
	}

	AParameterUIObjectPushButton* newPushButton = new AParameterUIObjectPushButton(); // must be created with new, AParameterUIObjectSliderFloat newSlider will NOT work.
	int _x = borderPixelAmount;
	int _y = currentUIElementsHeight;
	newPushButton->init(  _name, _x, _y, 10 );
	uiObjects.push_back( newPushButton );
	adjustToNewContent( 0, newPushButton->posy + newPushButton->height );
	return newPushButton;
}

AParameterUIObjectSlidersFloat2* AParameterUIPanel::addSlidersFloat2( string _name, int _width )
{
	// make sure we move down in case we've added some sub children
	if( uiObjects.size() > 0 )
	{
		AParameterUIObjectBase* tmpObj = (AParameterUIObjectBase*) uiObjects.at( uiObjects.size() -1 );
		adjustToNewContent( 0, tmpObj->posy + tmpObj->height );
	}
	
	AParameterUIObjectSlidersFloat2* newFloat2DSliders = new AParameterUIObjectSlidersFloat2(); // must be created with new, AParameterUIObjectSliderFloat newSlider will NOT work.
	int _x = borderPixelAmount;
	int _y = currentUIElementsHeight;
	newFloat2DSliders->init( _name, _x, _y, _width );
	uiObjects.push_back( newFloat2DSliders );
	adjustToNewContent( _width, newFloat2DSliders->posy + newFloat2DSliders->height );

	return newFloat2DSliders;	
}
	
	
AParameterUIObjectSlidersFloat3* AParameterUIPanel::addSlidersFloat3( string _name, int _width )
{
	// make sure we move down in case we've added some sub children
	if( uiObjects.size() > 0 )
	{
		AParameterUIObjectBase* tmpObj = (AParameterUIObjectBase*) uiObjects.at( uiObjects.size() -1 );
		adjustToNewContent( 0, tmpObj->posy + tmpObj->height );
	}
	
	AParameterUIObjectSlidersFloat3* newFloat3DSliders = new AParameterUIObjectSlidersFloat3(); // must be created with new, AParameterUIObjectSliderFloat newSlider will NOT work.
	int _x = borderPixelAmount;
	int _y = currentUIElementsHeight;
	newFloat3DSliders->init( _name, _x, _y, _width );
	uiObjects.push_back( newFloat3DSliders );
	adjustToNewContent( _width, newFloat3DSliders->posy + newFloat3DSliders->height );

	return newFloat3DSliders;	
}
	
	

void AParameterUIPanel::mouseMotion( int _x, int _y )
{
	int insideX = _x - posx;
	int insideY = _y - posy;

	if( isPointInsideRect( insideX, insideY, 0, 0, width, height ) || clickStartedInsideUs  )
	{
		AParameterUIObjectBase* tmpObj;
		for( unsigned int i = 0; i < uiObjects.size(); i++ )
		{
			tmpObj = (AParameterUIObjectBase*) uiObjects.at( i );
			tmpObj->mouseMotion( insideX, insideY );
		}

		bgColor[3] = 0.8f;
	}
	else
	{
		bgColor[3] = 0.6f;
	}


}

void AParameterUIPanel::mouseDown( int _x, int _y, int _button )
{
	int insideX = _x - posx;
	int insideY = _y - posy;

	if( isPointInsideRect( insideX, insideY, 0, 0, width, height ) )
	{
		AParameterUIObjectBase* tmpObj;
		for( unsigned int i = 0; i < uiObjects.size(); i++ )
		{
			tmpObj = (AParameterUIObjectBase*) uiObjects.at( i );
			tmpObj->mouseDown( insideX, insideY, _button );
		}

		clickStartedInsideUs = true;
	}
	else
	{

	}
}

void AParameterUIPanel::mouseUp( int _x, int _y, int _button )
{
	int insideX = _x - posx;
	int insideY = _y - posy;

	if( isPointInsideRect( insideX, insideY, 0, 0, width, height ) || clickStartedInsideUs )
	{
		AParameterUIObjectBase* tmpObj;
		for( unsigned int i = 0; i < uiObjects.size(); i++ )
		{
			tmpObj = (AParameterUIObjectBase*) uiObjects.at( i );
			tmpObj->mouseUp( insideX, insideY, _button );
		}
	}
	else
	{

	}

	clickStartedInsideUs = false;
}

