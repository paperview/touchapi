package com.nui.tuio {
	
/**
 * LastChanged:
 * 
 * $Author: naturalui $
 * $Revision: 65 $
 * $LastChangedDate: 2007-08-25 08:10:12 -0500 (Sat, 25 Aug 2007) $
 * $URL: https://touchapi.googlecode.com/svn/trunk/TouchAPI_Flex/src/lib/com/nui/tuio/TUIOEvent.as $
 * 
 */
import flash.display.InteractiveObject;
import flash.events.Event;	


public class TUIOEvent extends Event
{
	public static const MOVE:String = 'com.nui.TUIOEvent.MOVE';
	public static const DOWN:String = 'com.nui.TUIOEvent.DOWN';		
	public static const DOUBLE_DOWN:String = "com.nui.TUIOEvent.DOUBLE_OWN"; //double tap-click
	public static const UP:String = 'com.nui.TUIOEvent.UP';				
	public static const ROLL_OVER:String = 'com.nui.TUIOEvent.ROLL_OVER';						
	public static const ROLL_OUT:String = 'com.nui.TUIOEvent.ROLL_OUT';
	
	public var TUIOClass:String;
	public var sID:int;
	public var ID:int;
	public var angle:Number;
	public var stageX:Number;
	public var stageY:Number;
	public var localX:Number;
	public var localY:Number;
	public var buttonDown:Boolean;
	public var relatedObject:InteractiveObject;		
	
	public function TUIOEvent(type:String, bubbles:Boolean = false, cancelable:Boolean = false, p_stageX:Number = 0, p_stageY:Number = 0, p_localX:Number = 0, p_localY:Number = 0, p_relatedObject:InteractiveObject = null, p_ctrlKey:Boolean = false, p_altKey:Boolean = false, p_shiftKey:Boolean = false, p_buttonDown:Boolean = false, p_delta:int = 0, p_TUIOClass:String = '2Dcur', p_ID:int = -1, p_sID:int = -1, p_angle:Number = 0.0)
	{
		TUIOClass = p_TUIOClass;
		sID = sID;
		ID = p_ID;
		angle = p_angle;
		stageX = p_stageX;
		stageY = p_stageY;
		localX = p_localX;
		localY = p_localY;
		buttonDown = p_buttonDown;
		relatedObject = relatedObject;
		super(type, bubbles, cancelable);	
        //super(type, bubbles, cancelable, localX, localY, relatedObject, ctrlKey, altKey, shiftKey, buttonDown, delta);	
	}	
}
}