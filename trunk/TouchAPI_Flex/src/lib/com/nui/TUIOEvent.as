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
import flash.display.DisplayObject;	

public class TUIOEvent extends Event
{
	public static var MOVE:String = 'com.nui.TUIOEvent.MOVE';
	public static var DOWN:String = 'com.nui.TUIOEvent.DOWN';		
	public static var UP:String = 'com.nui.TUIOEvent.UP';				
	public static var ROLL_OVER:String = 'com.nui.TUIOEvent.ROLL_OVER';						
	public static var ROLL_OUT:String = 'com.nui.TUIOEvent.ROLL_OUT';
	
	public var TUIOClass:String;
	public var sID:int;
	public var ID:int;
	public var angle:Number;
	public var stageX:Number;
	public var stageY:Number;
	public var localX:Number;
	public var localY:Number;
	public var buttonDown:Boolean;
	public var relatedObject:DisplayObject;		
	
	public function TUIOEvent(type:String, bubbles:Boolean = false, cancelable:Boolean = false, stageX:Number = 0, stageY:Number = 0, localX:Number = 0, localY:Number = 0, relatedObject:DisplayObject = null, ctrlKey:Boolean = false, altKey:Boolean = false, shiftKey:Boolean = false, buttonDown:Boolean = false, delta:int = 0, TUIOClass:String = "2Dcur", ID:int = -1, sID:int = -1, angle:Number = 0.0)
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
}
}