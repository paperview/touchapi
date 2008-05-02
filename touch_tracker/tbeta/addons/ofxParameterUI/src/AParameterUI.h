/*
 *  AParameterUI.h
 *  SDL OpenGL Template
 *
 *  Created by Andreas Müller on 28/04/2007.
 *  Copyright 2007 Nanika. All rights reserved.
 *
 */




#ifndef _A_PARAMETER_UI_H
#define _A_PARAMETER_UI_H

#ifdef _WIN32
	#include <windows.h>    // included in all Windows apps
	#include <winuser.h>    // Windows constants
#else
#endif

#include <math.h>

#include <sstream>
#include <string>
#include <vector>
#include <map>
using std::string; 

#include <iostream>
using namespace std;

//#include "../AKeyDefinitions.h"
//#include "../AInputState.h"

#include "Math/Easing/AEasingEquations.h"

#include "AParameterUIObjectBase.h"
#include "AParameterUIPanel.h"
#include "AParameterUIAssets.h"

class AParameterUI
{
	
public:
	
	static AParameterUI* Instance();
	
	void init( int _screenWidth, int _screenHeight );
	
	void update();
	void render();
	
	void activate();
	void deActivate();

	AParameterUIPanel* addPanel( string _name, int _x, int _y );

	void addObject( AParameterUIObjectBase* _obj );

	void mouseMotion( int _x, int _y );
	void mouseDown( int _x, int _y, int _button );
	void mouseUp( int _x, int _y, int _button );

	void keyDown( int _key );
	void keyUp( int _key );


	bool initialised;
	bool isTransitioning;
	bool isActive;	
	
	// rendering variables
	float mainTransitionParam; // moves from 0 (inactive) to 1 (active)

	int currTransitionFrame;
	int transitionFramesToTake;
	int currTransitionFrameTarget;
	
	AParameterUIAssets* parameterUIAssets;

	AEaseType currEaseType;

	AEaseType currEaseTypeIn;
	AEaseType currEaseTypeOut;

	vector< AParameterUIObjectBase* > uiObjects;
	map<string, int> nameToIndex;
	
private:
		
	AParameterUI();

};


#endif