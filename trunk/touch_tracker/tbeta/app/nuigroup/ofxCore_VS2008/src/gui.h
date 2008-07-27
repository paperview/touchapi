#ifndef __GUI_DEFINITION
#define __GUI_DEFINITION

#include "testApp.h"

testApp *appPtr;

void testApp::setupGUI()
{
	appPtr = this;

	if(!gui->buildFromXml(OFXGUI_XML))
	{	
		gui->mGlobals->mBorderColor.r = 0;
		gui->mGlobals->mBorderColor.g = 0;
		gui->mGlobals->mBorderColor.b = 0;
		gui->mGlobals->mBorderColor.a = .3;

		gui->mGlobals->mCoverColor.r = 1;
		gui->mGlobals->mCoverColor.g = 1;
		gui->mGlobals->mCoverColor.b = 1;
		gui->mGlobals->mCoverColor.a = .4;

		gui->mGlobals->mFrameColor.r = 0;
		gui->mGlobals->mFrameColor.g = 0;
		gui->mGlobals->mFrameColor.b = 0;
		gui->mGlobals->mFrameColor.a = .3;
		
		gui->mGlobals->mTextColor.r = 0;
		gui->mGlobals->mTextColor.g = 0;
		gui->mGlobals->mTextColor.b = 0;
		gui->mGlobals->mTextColor.a = 1;

		gui->mGlobals->mButtonColor.r = 0;
		gui->mGlobals->mButtonColor.g = .6;
		gui->mGlobals->mButtonColor.b = 1;
		gui->mGlobals->mButtonColor.a = .5;

		gui->mGlobals->mHandleColor.r = 0;
		gui->mGlobals->mHandleColor.g = 0;
		gui->mGlobals->mHandleColor.b = 0;

		gui->mGlobals->mSliderColor.r = 1;
		gui->mGlobals->mSliderColor.g = 0;
		gui->mGlobals->mSliderColor.b = 0;
		gui->mGlobals->mSliderColor.a = .8;

		gui->mGlobals->mPointSize = 100;

		ofxGuiPanel* propPanel = gui->addPanel(appPtr->propertiesPanel, "Source Properties", 735, 30, 12, OFXGUI_PANEL_SPACING);
		propPanel->addButton(appPtr->propertiesPanel_settings, "Camera Settings (v)", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, kofxGui_Button_Off, kofxGui_Button_Trigger, "");
		propPanel->addButton(appPtr->propertiesPanel_flipV, "Flip Vertical (v)", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, kofxGui_Button_Off, kofxGui_Button_Switch, "");
		propPanel->addButton(appPtr->propertiesPanel_flipH, "Flip Horizontal (h)", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, kofxGui_Button_Off, kofxGui_Button_Switch, "");
		propPanel->mObjWidth = 200;

		ofxGuiPanel* oPanel = gui->addPanel(appPtr->optionPanel, "Tracking Options", 735, 137, OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING);
		oPanel->addButton(appPtr->optionPanel_tuio, "Send TUIO (t)", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, kofxGui_Button_Off, kofxGui_Button_Switch, "");
		oPanel->addButton(appPtr->optionPanel_draw, "Draw Windows (d)", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, kofxGui_Button_Off, kofxGui_Button_Switch, "");
		oPanel->mObjWidth = 200;

		ofxGuiPanel* cPanel = gui->addPanel(appPtr->calibrationPanel, "Calibration", 735, 233, OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING);
		cPanel->addButton(appPtr->calibrationPanel_calibrate, "Enter Calibration (c)", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, kofxGui_Button_Off, kofxGui_Button_Trigger, "");
		cPanel->addButton(appPtr->calibrationPanel_warp, "Warp (w)", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, kofxGui_Button_Off, kofxGui_Button_Switch, "");
		cPanel->mObjWidth = 200;


		ofxGuiPanel* panel2 = gui->addPanel(appPtr->savePanel, "files", 735, 320, OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING);
		//savePanel->addFiles(kParameter_File, "files", 110, OFXGUI_FILES_HEIGHT, "", "", "xml");
		panel2->addButton(appPtr->kParameter_SaveXml, "saveToXml", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, kofxGui_Button_Off, kofxGui_Button_Trigger, "");
		panel2->mObjWidth = 200;

		//Tracked Image
		ofxGuiPanel* trackPanel = gui->addPanel(appPtr->trackedPanel, "Tracked Image", 386, 270, OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING);
		trackPanel->addButton(appPtr->trackedPanel_outlines, "Show Outlines (o)", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, kofxGui_Button_Off, kofxGui_Button_Switch, "");
		trackPanel->addButton(appPtr->trackedPanel_ids, "Show IDs (i)", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, kofxGui_Button_Off, kofxGui_Button_Switch, "");
		trackPanel->addSlider(appPtr->trackedPanel_threshold, "Threshold (a/z)", 300, 13, 0.0f, 255.0f, threshold, kofxGui_Display_Int, 0);
		trackPanel->mObjHeight = 85;
		trackPanel->mObjWidth = 319;
		trackPanel->mObjects[1]->mObjX = 130;
		trackPanel->mObjects[1]->mObjY = 32;
		trackPanel->mObjects[2]->mObjY = 52;

		//Source Image
		ofxGuiPanel* srcPanel = gui->addPanel(appPtr->sourcePanel, "Source Image", 41, 270, OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING);
		srcPanel->addButton(appPtr->sourcePanel_cam, "Use Camera", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, kofxGui_Button_Off, kofxGui_Button_Switch, "");
		srcPanel->addButton(appPtr->sourcePanel_previousCam, "Previous Camera", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, kofxGui_Button_Off, kofxGui_Button_Trigger, "");
		srcPanel->addButton(appPtr->sourcePanel_nextCam, "Next Camera", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, kofxGui_Button_Off, kofxGui_Button_Trigger, "");
		srcPanel->addButton(appPtr->sourcePanel_video, "Use Video", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, kofxGui_Button_Off, kofxGui_Button_Switch, "");
		srcPanel->mObjHeight = 85;
		srcPanel->mObjWidth = 319;
		srcPanel->mObjects[1]->mObjX = 100;
		srcPanel->mObjects[1]->mObjY = 32;
		srcPanel->mObjects[2]->mObjX = 214;
		srcPanel->mObjects[2]->mObjY = 32;
		srcPanel->mObjects[3]->mObjY = 55;

		//Background Image
		ofxGuiPanel* bkPanel2 = gui->addPanel(appPtr->backgroundPanel, "Background", 86, 487, 10, 7);
		bkPanel2->mObjWidth = 127;
		bkPanel2->mObjHeight = 60;

		//Background Image
		ofxGuiPanel* bkPanel = gui->addPanel(appPtr->backgroundPanel, "Background", 236, 487, 10, 7);
		bkPanel->addButton(backgroundPanel_remove, "Remove (b)\nBackground", 10, 10, kofxGui_Button_Off, kofxGui_Button_Trigger, "");
		bkPanel->mObjWidth = 127;
		bkPanel->mObjHeight = 60;

		//Highpass Image
		ofxGuiPanel* hpPanel = gui->addPanel(appPtr->highpassPanel, "Highpass", 386, 487, OFXGUI_PANEL_BORDER, 7);
		hpPanel->addSlider(highpassPanel_blur, "Blur", 110, 13, 0.0f, 200.0f, highpassBlur, kofxGui_Display_Int, 0);
		hpPanel->addSlider(highpassPanel_noise, "Noise", 110, 13, 0.0f, 30.0f, highpassNoise, kofxGui_Display_Int, 0);
		hpPanel->mObjWidth = 127;

		//Amplify Image
		ofxGuiPanel* ampPanel = gui->addPanel(appPtr->amplifyPanel, "Amplify", 536, 487, OFXGUI_PANEL_BORDER, 7);
		ampPanel->addSlider(amplifyPanel_amp, "Amplify", 110, 13, 0.0f, 300.0f, highpassAmp, kofxGui_Display_Int, 0);
		ampPanel->mObjWidth = 127;

		//do update while inactive
		gui->forceUpdate(false);	
		gui->activate(true);

	}

		gui->update(appPtr->propertiesPanel_flipV, kofxGui_Set_Bool, &appPtr->bVerticalMirror, sizeof(bool));
		gui->update(appPtr->propertiesPanel_flipV, kofxGui_Set_Bool, &appPtr->bHorizontalMirror, sizeof(bool));
		gui->update(appPtr->optionPanel_draw, kofxGui_Set_Bool, &appPtr->bDrawVideo, sizeof(bool));
		gui->update(appPtr->trackedPanel_outlines, kofxGui_Set_Bool, &appPtr->bDrawOutlines, sizeof(bool));
		gui->update(appPtr->trackedPanel_ids, kofxGui_Set_Bool, &appPtr->bShowLabels, sizeof(bool));
		gui->update(appPtr->sourcePanel_cam, kofxGui_Set_Bool, &appPtr->bcamera, sizeof(bool));
		gui->update(appPtr->calibrationPanel_warp, kofxGui_Set_Bool, &appPtr->bWarpImg, sizeof(bool));
		//gui->update(sourcePanel_cam, kofxGui_Set_Bool, &->appPtrbcamera, sizeof(bool));
}

void testApp::handleGui(int parameterId, int task, void* data, int length)
{
	if(activeInput)
	{
		switch(parameterId)
		{			
			case sourcePanel_cam:
				if(length == sizeof(bool))
				{
					if(*(bool*)data)
					{
						if(!bcamera){
							activeInput = false; //this stops the app from doing everything when changing source
							bcamera = true;
							vidGrabber.close();
							vidGrabber.setDeviceID(deviceID);
							vidGrabber.setVerbose(false);		
							camWidth = vidGrabber.width;
							camHeight = vidGrabber.height;
							vidGrabber.initGrabber(camWidth,camHeight);

							processedImg.allocate(camWidth, camHeight); //Processed Image
							sourceImg.allocate(camWidth, camHeight);    //Source Image
							grayImg.allocate(camWidth, camHeight);		//Gray Image
							grayBg.allocate(camWidth, camHeight);		//Background Image
							subtractBg.allocate(camWidth, camHeight);   //Background After subtraction
							grayDiff.allocate(camWidth, camHeight);		//Difference Image between Background and Source
							highpassImg.allocate(camWidth, camHeight);  //Highpass Image
							giWarped.allocate(camWidth, camHeight);     //Warped Image (used for warped calibration)
							fiLearn.allocate(camWidth, camHeight);		//ofxFloatImage used for simple dynamic background subtracti
							pressureMap.allocate(camWidth, camHeight);	//Pressure Map Image
							//reset warp box
							warp_box.setup( 40, 30, camWidth, camHeight, camWidth/320, camHeight/240); 
							
							activeInput = true;		//set to active again
							bLearnBakground = true; //recapture background
							//Turn off the video button;
							bool setBool = false;
							gui->update(sourcePanel_video, kofxGui_Set_Bool, &setBool, length);
						}
					}					
				}
				break;
			case sourcePanel_video:
				if(length == sizeof(bool))
				{
					if(*(bool*)data)
					{
						if(bcamera){
							activeInput = false; //this stops the app from doing everything when changing source
							bcamera = false;						
					//		vidPlayer.loadMovie("test_videos/FrontDI.m4v");
							//vidPlayer.loadMovie("test_videos/HCI_FTIR.mov");
							vidPlayer.loadMovie("test_videos/raw.mp4");
							vidPlayer.play();	
							printf("Video Mode\n");
							camHeight = vidPlayer.height;
							camWidth = vidPlayer.width;

							processedImg.allocate(camWidth, camHeight); //Processed Image
							sourceImg.allocate(camWidth, camHeight);    //Source Image
							grayImg.allocate(camWidth, camHeight);		//Gray Image
							grayBg.allocate(camWidth, camHeight);		//Background Image
							subtractBg.allocate(camWidth, camHeight);   //Background After subtraction
							grayDiff.allocate(camWidth, camHeight);		//Difference Image between Background and Source
							highpassImg.allocate(camWidth, camHeight);  //Highpass Image
							giWarped.allocate(camWidth, camHeight);     //Warped Image (used for warped calibration)
							fiLearn.allocate(camWidth, camHeight);		//ofxFloatImage used for simple dynamic background subtracti
							pressureMap.allocate(camWidth, camHeight);	//Pressure Map Image

							//reset warp box
							warp_box.setup( 40, 30, camWidth, camHeight, camWidth/320, camHeight/240); 
					
							activeInput = true;
							bLearnBakground = true;
							//Turn off the camera button;
							bool setBool = false;
							gui->update(sourcePanel_cam, kofxGui_Set_Bool, &setBool, length);
						}
					}					
				}
				break;

			case sourcePanel_nextCam:
				if(length == sizeof(bool))
				{
					if(*(bool*)data)
					{
						activeInput = false; //this stops the app from doing everything when changing source

						if(deviceID < vidGrabber.getNumDevices() - 2)
						{
							deviceID += 1;
							vidGrabber.close();
							vidGrabber.setDeviceID(deviceID);
							vidGrabber.setVerbose(false);		
							vidGrabber.initGrabber(camWidth,camHeight);
						}
						activeInput = true;
						bLearnBakground = true;
					}					
				}
				break;
			case sourcePanel_previousCam:
				if(length == sizeof(bool))
				{
					if(*(bool*)data)
					{
						activeInput = false; //this stops the app from doing everything when changing source

						if(deviceID > 0)
						{
							deviceID -= 1;
							vidGrabber.close();
							vidGrabber.setDeviceID(deviceID);
							vidGrabber.setVerbose(false);		
							vidGrabber.initGrabber(camWidth,camHeight);
						}
						activeInput = true;
						bLearnBakground = true;
					}					
				}
				break;

			case propertiesPanel_settings:
				if(length == sizeof(bool))
				{
					if(*(bool*)data && bcamera)
					{						
						vidGrabber.videoSettings();
					}					
				}
				break;

			case calibrationPanel_calibrate:
				//if(length == sizeof(bool))
					bCalibration = true;
					bFullscreen = true;
				break;
			case calibrationPanel_warp:
				if(length == sizeof(bool))
					bWarpImg = *(bool*)data;
				break;



			case propertiesPanel_flipH:
				if(length == sizeof(bool))
					bHorizontalMirror = *(bool*)data;
				break;
			case propertiesPanel_flipV:
				if(length == sizeof(bool))
					bVerticalMirror = *(bool*)data;
				break;

			case optionPanel_tuio:
				if(length == sizeof(bool))
					bTUIOMode = *(bool*)data;
				break;
			case optionPanel_draw:
				if(length == sizeof(bool))
					bDrawVideo = *(bool*)data;
				break;
				

			case backgroundPanel_remove:
				if(length == sizeof(bool))
					bLearnBakground = *(bool*)data;
				break;
			case highpassPanel_blur:
				if(length == sizeof(float))
					highpassBlur = *(float*)data;
				break;
			case highpassPanel_noise:
				if(length == sizeof(float))
					highpassNoise = *(float*)data;
				break;
			case amplifyPanel_amp:
				if(length == sizeof(float))
					highpassAmp = *(float*)data;
				break;
			case trackedPanel_threshold:
				if(length == sizeof(float))
					threshold = *(float*)data;
				break;
			case trackedPanel_outlines:
				if(length == sizeof(bool))
					bDrawOutlines = *(bool*)data;
				break;
			case trackedPanel_ids:
				if(length == sizeof(bool))
					bShowLabels = *(bool*)data;
				break;
						
									
				
			case kParameter_SaveXml:
				if(length == sizeof(bool))
				{
					if(*(bool*)data)
						gui->saveToXml(OFXGUI_XML);
				}
				break;
			case kParameter_File:
				if(length == sizeof(string))
				{
					string file = *(string*)data;
					gui->buildFromXml(file);
				}
				break;
			
		}
	}

}





#endif //__UI_DEFINITION

