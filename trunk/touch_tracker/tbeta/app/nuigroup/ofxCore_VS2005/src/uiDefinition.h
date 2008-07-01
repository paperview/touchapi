#ifndef __UI_DEFINITION
#define __UI_DEFINITION

#include "testApp.h"

testApp *appPtr;

void setThreshold(int newVal, int arg1, int arg2, int arg3)
{
	appPtr->threshold = newVal;
}

void setBlur(int newVal, int arg1, int arg2, int arg3)
{
	//smoothing kernel must be odd
	appPtr->blurValue = newVal+((newVal%2)?0:1);
}

void setHighpassBlur(int newVal, int arg1, int arg2, int arg3)
{
	appPtr->highpassBlur = newVal;
}

void setHighpassNoise(int newVal, int arg1, int arg2, int arg3)
{
	appPtr->highpassNoise = newVal;
}

void setHighpassAmp(int newVal, int arg1, int arg2, int arg3)
{
	appPtr->highpassAmp = newVal;
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

	//Make the Panels
	AParameterUIPanel* tmpPanel_1 = 
		ui->addPanel("Video Source", 800.0f, 35.0f);	
		tmpPanel_1->borderPixelAmount = 12;
	AParameterUIPanel* tmpPanel_2 =
		ui->addPanel("Options", 800.0f, 210.0f);
		tmpPanel_2->borderPixelAmount = 12;
	AParameterUIPanel* tmpPanel_3 =
		ui->addPanel("Background", 800.0f, 370.0f);
		tmpPanel_3->borderPixelAmount = 12;
	AParameterUIPanel* tmpPanel_4 =
		ui->addPanel("Highpass Filter", 800.0f, 516.0f);
		tmpPanel_4->borderPixelAmount = 12;
	AParameterUIPanel* tmpPanel_5 =
		ui->addPanel("Threshold", 800.0f, 700.0f);
		tmpPanel_5->borderPixelAmount = 12;

	AParameterUIObjectPushButton* tmpOnePushButton1 = 
		tmpPanel_1->addPushButton("Active");
	AParameterUIObjectPushButton* tmpOnePushButton2 = 
		tmpPanel_1->addPushButton("Camera");
	AParameterUIObjectPushButton* tmpOnePushButton3 = 
		tmpPanel_1->addPushButton("Video");
	AParameterUIObjectSliderInteger* tmpIntegerSlider1 = 
		tmpPanel_1->addSliderInteger("ID", 100, 1, 100, 100);
		

	AParameterUIObjectPushButton* tmpOnePushButton4 = 
		tmpPanel_2->addPushButton("Clear");

	AParameterUIObjectPushButton* tmpOnePushButton6 = 
		tmpPanel_2->addPushButton("Warp");

	AParameterUIObjectPushButton* tmpOnePushButton7 = 
		tmpPanel_2->addPushButton("Flip Horizontal");
	tmpOnePushButton7->changedParameterCallback = &setMirrorVertical;
	tmpOnePushButton7->funcPointerSet = true;
	tmpOnePushButton7->borderPixelAmount = 2;

	AParameterUIObjectPushButton* tmpOnePushButton8 = 
		tmpPanel_2->addPushButton("Flip Vertical");
	tmpOnePushButton8->changedParameterCallback = &setMirrorHorizontal;
	tmpOnePushButton8->funcPointerSet = true;

	

	AParameterUIObjectSliderInteger* tmpIntegerSlider2 = 
		tmpPanel_5->addSliderInteger("Threshold", 150, 0, 255, threshold);
	tmpIntegerSlider2->changedParameterCallback = &setThreshold;
	tmpIntegerSlider2->funcPointerSet = true;



	AParameterUIObjectSliderInteger* tmpIntegerSlider5 = 
		tmpPanel_3->addSliderInteger("Blur", 150, 0, min(camWidth, camHeight), blurValue);
	tmpIntegerSlider5->changedParameterCallback = &setBlur;
	tmpIntegerSlider5->funcPointerSet = true;

	AParameterUIObjectSliderInteger* tmpIntegerSlider7 = 
		tmpPanel_3->addSliderInteger("Background Subtract", 150, 1, 100, 100);

	AParameterUIObjectSliderInteger* tmpIntegerSlider9 = 
	tmpPanel_4->addSliderInteger("Blur", 150, 0, 255, highpassBlur);
	tmpIntegerSlider9->changedParameterCallback = &setHighpassBlur;
	tmpIntegerSlider9->funcPointerSet = true;
	
	AParameterUIObjectSliderInteger* tmpIntegerSlider10 = 
	tmpPanel_4->addSliderInteger("Noise", 150, 0, 30, highpassNoise);
	tmpIntegerSlider10->changedParameterCallback = &setHighpassNoise;
	tmpIntegerSlider10->funcPointerSet = true;

	AParameterUIObjectSliderInteger* tmpIntegerSlider11 = 
	tmpPanel_4->addSliderInteger("Amplify", 150, 0, 100, highpassAmp);
	tmpIntegerSlider11->changedParameterCallback = &setHighpassAmp;
	tmpIntegerSlider11->funcPointerSet = true;


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

