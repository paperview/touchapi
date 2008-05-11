#ifndef __UI_DEFINITION
#define __UI_DEFINITION


void setValue( float newVal, int arg1, int arg2, int arg3  )
{
	char idStr[1024];		
	sprintf(idStr, "Value: %f\nArg1: %i\nArg2: %i\nArg3: %i\n",newVal,arg1,arg2,arg3);
	printf(idStr);
}



void testApp::setupUI()
{

	AParameterUI* ui = AParameterUI::Instance();
	AParameterUIPanel* tmpPanel_1 = ui->addPanel( "Video Source", 685.0f, 35.0f );
	AParameterUIPanel* tmpPanel_2 = ui->addPanel( "Video Settings", 685.0f, 235.0f );
	AParameterUIPanel* tmpPanel_3 = ui->addPanel( "Effects & Filters", 685.0f, 445.0f );

	AParameterUIObjectPushButton*	tmpOnePushButton2 = tmpPanel_1->addPushButton( "Active" );
	AParameterUIObjectSliderInteger* tmpIntegerSlider = tmpPanel_1->addSliderInteger( "ID", 300, 1, 100, 100 );
	AParameterUIObjectPushButton*	tmpOnePushButton1b = tmpPanel_1->addPushButton( "Camera" );
	AParameterUIObjectPushButton*	tmpOnePushButton2b = tmpPanel_1->addPushButton( "VCR" );

	AParameterUIObjectSliderFloat* tmpFloatSlider3 = tmpPanel_2->addSliderFloat( "Threshold", 300, 0.0f, 360, 0.0f );
	AParameterUIObjectSliderFloat* tmpFloatSlider2a = tmpPanel_2->addSliderFloat( "Low Level", 300, 1.0f, 300, 1 );	
	AParameterUIObjectSliderFloat* tmpFloatSlider2 = tmpPanel_2->addSliderFloat( "High Level", 300, 1.0f, 300, 1 );
	tmpFloatSlider2->changedParameterCallback = &setValue;
	tmpFloatSlider2->funcPointerSet = true;

	AParameterUIObjectSliderFloat* tmpSpinSlider = tmpPanel_3->addSliderFloat( "Blur", 300, 0.0f, 360.0f, 0.0f );
	tmpSpinSlider->changedParameterCallback = &setValue;
	tmpSpinSlider->funcPointerSet = true;

	AParameterUIObjectSliderFloat* tmpFloatSlider = tmpPanel_3->addSliderFloat( "Gasian Blur", 300, 0.0f, 1.0f, 0.0f );
	AParameterUIObjectSliderInteger* tmpIntegerSliderx = tmpPanel_3->addSliderInteger( "Background Subtract", 300, 1, 100, 100 );
	AParameterUIObjectPushButton*	tmpOnePushButton1x = tmpPanel_3->addPushButton( "Clear" );
	AParameterUIObjectPushButton*	tmpOnePushButton2x = tmpPanel_3->addPushButton( "Invert" );
	AParameterUIObjectPushButton*	tmpOnePushButton3x = tmpPanel_3->addPushButton( "Warp" );
	AParameterUIObjectPushButton*	tmpOnePushButton4x = tmpPanel_3->addPushButton( "Mirror Vertical" );
	AParameterUIObjectPushButton*	tmpOnePushButton5x = tmpPanel_3->addPushButton( "Mirror Horizontal" );

/*
	AParameterUIObjectCheckbox*	tmpCheckBox = tmpPanel_1->addCheckbox( "Lighting", exit );
	tmpCheckBox->changedParameterCallback = &testApp::exit;
	tmpCheckBox->funcPointerSet = true;

	AParameterUIObjectPushButton*	tmpPushButton = tmpPanel_1->addPushButton( "PushButton" );
	tmpPushButton->changedParameterCallback = &buttonPressed;
	tmpPushButton->funcPointerSet = true;
*/
}

#endif //__UI_DEFINITION

