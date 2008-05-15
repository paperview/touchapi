#ifndef __UI_DEFINITION
#define __UI_DEFINITION

#include "testApp.h"

testApp *appPtr;

void setThreshold(int newVal, int arg1, int arg2, int arg3)
{
	appPtr->threshold = newVal;
}
void setLowLevel(int newVal, int arg1, int arg2, int arg3)
{
	appPtr->lowRange = newVal;
}
void setHighLevel(int newVal, int arg1, int arg2, int arg3)
{
	appPtr->highRange = newVal;
}
void setBlur(int newVal, int arg1, int arg2, int arg3)
{
	//smoothing kernel must be odd
	appPtr->blurValue = newVal+((newVal%2)?0:1);
}
void setGaussianBlur(int newVal, int arg1, int arg2, int arg3)
{
	//smoothing kernel must be odd
	appPtr->blurGaussianValue = newVal+((newVal%2)?0:1);
}
void setInvert(int arg1, int arg2, int arg3)
{
	appPtr->bInvertVideo = !appPtr->bInvertVideo;
}
void setMirrorVertical(int arg1, int arg2, int arg3)
{
	appPtr->bVerticalMirror = !appPtr->bVerticalMirror;
}
void setMirrorHorizontal(int arg1, int arg2, int arg3)
{
	appPtr->bHorizontalMirror = !appPtr->bHorizontalMirror;
}

void testApp::setupUI()
{
	appPtr = this;
	AParameterUI* ui = AParameterUI::Instance();
	AParameterUIPanel* tmpPanel_1 = 
		ui->addPanel("Video Source", 685.0f, 35.0f);
	AParameterUIPanel* tmpPanel_2 =
		ui->addPanel("Video Settings", 685.0f, 235.0f);
	AParameterUIPanel* tmpPanel_3 =
		ui->addPanel("Effects & Filters", 685.0f, 445.0f);

	AParameterUIObjectPushButton* tmpOnePushButton1 = 
		tmpPanel_1->addPushButton("Active");
	AParameterUIObjectPushButton* tmpOnePushButton2 = 
		tmpPanel_1->addPushButton("Camera");
	AParameterUIObjectPushButton* tmpOnePushButton3 = 
		tmpPanel_1->addPushButton("Video");
	AParameterUIObjectSliderInteger* tmpIntegerSlider1 = 
		tmpPanel_1->addSliderInteger("ID", 300, 1, 100, 100);

	AParameterUIObjectSliderInteger* tmpIntegerSlider2 = 
		tmpPanel_2->addSliderInteger("Threshold", 300, 0, 255, 99);
	tmpIntegerSlider2->changedParameterCallback = &setThreshold;
	tmpIntegerSlider2->funcPointerSet = true;

	AParameterUIObjectSliderInteger* tmpIntegerSlider3 = 
		tmpPanel_2->addSliderInteger("Low Level", 300, 0, 255, 0);	
	tmpIntegerSlider3->changedParameterCallback = &setLowLevel;
	tmpIntegerSlider3->funcPointerSet = true;

	AParameterUIObjectSliderInteger* tmpIntegerSlider4 = 
		tmpPanel_2->addSliderInteger("High Level", 300, 0, 255, 255);
	tmpIntegerSlider4->changedParameterCallback = &setHighLevel;
	tmpIntegerSlider4->funcPointerSet = true;

	AParameterUIObjectSliderInteger* tmpIntegerSlider5 = 
		tmpPanel_3->addSliderInteger("Blur", 300, 0, 
									 min(camWidth, camHeight), 1);
	tmpIntegerSlider5->changedParameterCallback = &setBlur;
	tmpIntegerSlider5->funcPointerSet = true;

	AParameterUIObjectSliderInteger* tmpIntegerSlider6 = 
		tmpPanel_3->addSliderInteger("Gaussian Blur", 300, 0,
								     min(camWidth, camHeight), 1);
	tmpIntegerSlider6->changedParameterCallback = &setGaussianBlur;
	tmpIntegerSlider6->funcPointerSet = true;


	AParameterUIObjectSliderInteger* tmpIntegerSlider7 = 
		tmpPanel_3->addSliderInteger("Background Subtract", 300, 1, 100, 100);

	AParameterUIObjectPushButton* tmpOnePushButton4 = 
		tmpPanel_3->addPushButton("Clear");

	AParameterUIObjectPushButton* tmpOnePushButton5 = 
		tmpPanel_3->addPushButton("Invert");
	tmpOnePushButton5->changedParameterCallback = &setInvert;
	tmpOnePushButton5->funcPointerSet = true;

	AParameterUIObjectPushButton* tmpOnePushButton6 = 
		tmpPanel_3->addPushButton("Warp");

	AParameterUIObjectPushButton* tmpOnePushButton7 = 
		tmpPanel_3->addPushButton("Mirror Vertical");
	tmpOnePushButton7->changedParameterCallback = &setMirrorVertical;
	tmpOnePushButton7->funcPointerSet = true;

	AParameterUIObjectPushButton* tmpOnePushButton8 = 
		tmpPanel_3->addPushButton("Mirror Horizontal");
	tmpOnePushButton7->changedParameterCallback = &setMirrorHorizontal;
	tmpOnePushButton7->funcPointerSet = true;

	//AParameterUIObjectCheckbox*	tmpCheckBox = 
	//tmpPanel_1->addCheckbox( "Lighting", &setInvert );
	//tmpCheckBox->changedParameterCallback = &setInvert;
	//tmpCheckBox->funcPointerSet = true;
/*
	AParameterUIObjectPushButton*	tmpPushButton = 
	tmpPanel_1->addPushButton( "PushButton" );
	tmpPushButton->changedParameterCallback = &buttonPressed;
	tmpPushButton->funcPointerSet = true;
*/
}

#endif //__UI_DEFINITION

