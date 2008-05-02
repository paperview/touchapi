#ifndef __UI_DEFINITION
#define __UI_DEFINITION


void changedSquareAmountToDraw( int _amount, int arg1, int arg2, int arg3 )
{
	//squareAmountToDraw = _amount;
}


void changedSpin( float _newVal, int arg1, int arg2, int arg3  )
{
	//OFSAptr->spin = _newVal;
}


void lightingToggleFunction( bool _newVal, int arg1, int arg2, int arg3 )
{
	//doLighting = _newVal;
}


//OFSAptr

void testApp::setupUI()
{

	AParameterUI* ui = AParameterUI::Instance();
	AParameterUIPanel* tmpPanel1 =  ui->addPanel( "Video Source", 20.0f, 20.0f );
	AParameterUIPanel* tmpPanel12 = ui->addPanel( "Video Settings", 20.0f, 220.0f );
	AParameterUIPanel* tmpPanel13 = ui->addPanel( "Effects & Filters", 20.0f, 430.0f );

	AParameterUIObjectSliderFloat* tmpSpinSlider = tmpPanel13->addSliderFloat( "Blur", 300, 0.0f, 360.0f, 0.0f );
	//tmpSpinSlider->changedParameterCallback = &changedSpin;	//tmpSpinSlider->funcPointerSet = true;

	AParameterUIObjectSliderFloat* tmpFloatSlider = tmpPanel13->addSliderFloat( "Gasian Blur", 300, 0.0f, 1.0f, 0.0f );
	//tmpFloatSlider->changedParameterCallback = &qValueChanged;
	//tmpFloatSlider->funcPointerSet = true;

	AParameterUIObjectSliderFloat* tmpFloatSlider3 = tmpPanel12->addSliderFloat( "Threshold", 300, 0.0f, 360, 0.0f );
	AParameterUIObjectSliderFloat* tmpFloatSlider2a = tmpPanel12->addSliderFloat( "Brightness", 300, 1.0f, 300, 1 );	
	AParameterUIObjectSliderFloat* tmpFloatSlider2 = tmpPanel12->addSliderFloat( "Contrast", 300, 1.0f, 300, 1 );
	AParameterUIObjectPushButton*	tmpOnePushButton2 = tmpPanel1->addPushButton( "Active" );
	AParameterUIObjectSliderInteger* tmpIntegerSlider = tmpPanel1->addSliderInteger( "Source", 300, 1, 100, 100 );
	AParameterUIObjectPushButton*	tmpOnePushButton1b = tmpPanel1->addPushButton( "Camera" );
	AParameterUIObjectPushButton*	tmpOnePushButton2b = tmpPanel1->addPushButton( "VCR" );
	//tmpIntegerSlider->changedParameterCallback = &changedSquareAmountToDraw;
	//tmpIntegerSlider->funcPointerSet = true;


	//AParameterUIObjectCheckbox*	tmpAutoModeCheckbox = tmpIntegerSlider->addCheckbox( "Auto", doLighting );
	//tmpAutoModeCheckbox->changedParameterCallback = &lightingToggleFunction;
	//tmpAutoModeCheckbox->funcPointerSet = true;
	AParameterUIObjectSliderInteger* tmpIntegerSliderx = tmpPanel13->addSliderInteger( "Background Subtract", 300, 1, 100, 100 );
	AParameterUIObjectPushButton*	tmpOnePushButton1x = tmpPanel13->addPushButton( "Clear" );
	AParameterUIObjectPushButton*	tmpOnePushButton2x = tmpPanel13->addPushButton( "Invert" );
	AParameterUIObjectPushButton*	tmpOnePushButton3x = tmpPanel13->addPushButton( "Warp" );
	AParameterUIObjectPushButton*	tmpOnePushButton4x = tmpPanel13->addPushButton( "Mirror Vertical" );
	AParameterUIObjectPushButton*	tmpOnePushButton5x = tmpPanel13->addPushButton( "Mirror Horizontal" );
	//tmpOnePushButton->changedParameterCallback = &buttonPressed;
	//tmpOnePushButton->funcPointerSet = true;


/*

	AParameterUIObjectCheckbox*	tmpCheckBox = tmpPanel1->addCheckbox( "Lighting", doLighting );
	tmpCheckBox->changedParameterCallback = &lightingToggleFunction;
	tmpCheckBox->funcPointerSet = true;


	AParameterUIObjectPushButton*	tmpPushButton = tmpPanel1->addPushButton( "PushButton" );
	tmpPushButton->changedParameterCallback = &buttonPressed;
	tmpPushButton->funcPointerSet = true;
	*/
}

#endif //__UI_DEFINITION

