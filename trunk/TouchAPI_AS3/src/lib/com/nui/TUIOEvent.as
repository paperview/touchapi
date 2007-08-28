package com.nui {
	
/**
 * LastChanged:
 * 
 * $Author: naturalui $
 * $Revision: 65 $
 * $LastChangedDate: 2007-08-25 08:10:12 -0500 (Sat, 25 Aug 2007) $
 * $URL: https://touchapi.googlecode.com/svn/trunk/TouchAPI_Flex/src/lib/com/nui/tuio/TUIOEvent.as $
 * 
 */
import flash.events.Event;	
import flash.display.InteractiveObject;	


public class TUIOEvent extends Event
{
	public static const MOVE:String = 'com.nui.TUIOEvent.MOVE';
	public static const DOWN:String = 'com.nui.TUIOEvent.DOWN';		
	public static const DOUBLE_DOWN:String = "com.nui.TUIOEvent.DOUBLE_OWN"; //double tap/click
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
	
	public function TUIOEvent(type:String, bubbles:Boolean = false, cancelable:Boolean = false, stageX:Number = 0, stageY:Number = 0, localX:Number = 0, localY:Number = 0, relatedObject:InteractiveObject = null, ctrlKey:Boolean = false, altKey:Boolean = false, shiftKey:Boolean = false, buttonDown:Boolean = false, delta:int = 0, TUIOClass:String = "2Dcur", ID:int = -1, sID:int = -1, angle:Number = 0.0)
	{
		this.TUIOClass = TUIOClass;
		this.sID = sID;
		this.ID = ID;
		this.angle = angle;
		this.stageX = stageX;
		this.stageY = stageY;
		this.localX = localX;
		this.localY = localY;
		this.buttonDown = buttonDown;
		this.relatedObject = relatedObject;
		super(type, bubbles, cancelable);	
        //super(type, bubbles, cancelable, localX, localY, relatedObject, ctrlKey, altKey, shiftKey, buttonDown, delta);	
	}	
	
	/*
		private var _finger:uint; 

		public function get finger():uint {
			  return _finger;
		} 
		
		public function TUIOEvent(type:String, finger:uint, localX:Number, localY:Number, bubbles:Boolean = true, cancelable:Boolean = false, relatedObject:InteractiveObject = null, ctrlKey:Boolean = false, altKey:Boolean = false, shiftKey:Boolean = false, buttonDown:Boolean = false, delta:int = 0) {
			super(type, bubbles, cancelable, localX, localY); 
			_finger = finger;
		}
		
		public override function clone():Event {
			return new TouchEvent(type, _finger, this.localX, this.localY);
		}
	*/
}
}