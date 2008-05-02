/*
 *  AParameterUI.cpp
 *  SDL OpenGL Template
 *
 *  Created by Andreas Müller on 28/04/2007.
 *  Copyright 2007 Nanika. All rights reserved.
 *
 */

#include "AParameterUI.h"

AParameterUI* AParameterUI::Instance()
{
	static AParameterUI inst;
	return &inst;
}


AParameterUI::AParameterUI()
{
	isActive = false;

	initialised = false;
	mainTransitionParam = 0.0f;

	isTransitioning = false;
	currTransitionFrame = 0;
	transitionFramesToTake = 20;
	currTransitionFrameTarget = transitionFramesToTake;	

	parameterUIAssets = AParameterUIAssets::Instance();
	parameterUIAssets->currTransitionParam = 0.0f;

	currEaseTypeIn = EASE_OUT_BACK;
	currEaseTypeOut = EASE_IN_EXPO;
	currEaseType = currEaseTypeIn;
	
}

void AParameterUI::init( int _screenWidth, int _screenHeight )
{

	initialised = true;
}

AParameterUIPanel* AParameterUI::addPanel( string _name, int _x, int _y )
{
	AParameterUIPanel* newPanel = new AParameterUIPanel(); // must be created with new, AParameterUIPanel newPanel will NOT work.
	newPanel->init( _name, _x, _y );

	uiObjects.push_back( newPanel );
	
	return newPanel;
}

void AParameterUI::addObject( AParameterUIObjectBase* _obj )
{
	uiObjects.push_back( _obj );
}


void AParameterUI::update()
{
	
	if( !initialised ) return;

	if( isActive )
	{
		if( isTransitioning )
		{
		
			if( currTransitionFrame < currTransitionFrameTarget) { currTransitionFrame++; currEaseType = currEaseTypeIn; }
			else if( currTransitionFrame > currTransitionFrameTarget ) { currTransitionFrame--; currEaseType = currEaseTypeOut; }

			if( currTransitionFrame == currTransitionFrameTarget ) 
			{
				isTransitioning = false;
				if( currTransitionFrameTarget == 0) { isActive = false; }
			}

			mainTransitionParam = (float)currTransitionFrame / (float)transitionFramesToTake;
			mainTransitionParam = AEasingEquations::ease( mainTransitionParam, currEaseType );

			parameterUIAssets->currTransitionParam = mainTransitionParam;
		}
	}

	if( isActive )
	{
		for( unsigned int i = 0; i < uiObjects.size(); i++ ) 
		{ 
			uiObjects.at( i )->update(); 
		}
	}

}

//		tmpx = (rects[i].disp_x - rects[i].x)/moveSmoothSpeed;
//if( fabsf(tmpx) < 0.03f ) { rects[i].disp_x = rects[i].x; } else { rects[i].disp_x -= tmpx; }

void AParameterUI::render()
{
	if( !initialised ) return;
	
cout << "Rendering and we have " << uiObjects.size() << " objects." << endl;
	
	
	if( isTransitioning || isActive )
	{

		glEnable( GL_BLEND );
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

		AParameterUIObjectBase* tmpObj;
		for( unsigned int i = 0; i < uiObjects.size(); i++ )
		{
			tmpObj = (AParameterUIObjectBase*) uiObjects.at( i );
			tmpObj->render();
		}
		
		glDisable( GL_BLEND );
	}
	
}

void AParameterUI::activate()
{
	isActive = true;

	isTransitioning = true;
	currTransitionFrameTarget = transitionFramesToTake;	

}

void AParameterUI::deActivate()
{

	isTransitioning = true;
	currTransitionFrameTarget = 0;	
}


void AParameterUI::mouseMotion( int _x, int _y )
{
	if( isActive )
	{
		AParameterUIObjectBase* tmpObj;
		for( unsigned int i = 0; i < uiObjects.size(); i++ )
		{
			tmpObj = (AParameterUIObjectBase*) uiObjects.at( i );
			tmpObj->mouseMotion( _x, _y );
		}
	}
}

void AParameterUI::mouseDown( int _x, int _y, int _button )
{
	if( isActive )
	{
		AParameterUIObjectBase* tmpObj;
		for( unsigned int i = 0; i < uiObjects.size(); i++ )
		{
			tmpObj = (AParameterUIObjectBase*) uiObjects.at( i );
			tmpObj->mouseDown( _x, _y, _button );
		}
	}
}

void AParameterUI::mouseUp( int _x, int _y, int _button )
{
	if( isActive )
	{
		AParameterUIObjectBase* tmpObj;
		for( unsigned int i = 0; i < uiObjects.size(); i++ )
		{
			tmpObj = (AParameterUIObjectBase*) uiObjects.at( i );
			tmpObj->mouseUp( _x, _y, _button );
		}
	}
}

void AParameterUI::keyDown( int _key )
{

}

void AParameterUI::keyUp( int _key )
{

}


