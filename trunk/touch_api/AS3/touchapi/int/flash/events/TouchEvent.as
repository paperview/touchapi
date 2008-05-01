package flash.events {
	
	import flash.display.DisplayObject;
	import flash.events.Event;	
	/**
	* General TUIOSocket event schema - custom events tailored towards touch input paradigms.
	*/
	public class TouchEvent extends Event
	{
		public var profile:String;
		public var sID:int;
		public var ID:int;
		public var angle:Number;
		public var stageX:Number;
		public var stageY:Number;
		public var localX:Number;
		public var localY:Number;
		public var oldX:Number;
		public var oldY:Number;
		public var targetObject:DisplayObject;
		public var buttonDown:Boolean;
		
		/*  UNUSED VARS	
			-----------------------------------------------------
			public var timestamp:Date;
			public var info:Array;
			public var tapCount:Array;
		 */
		
		public static const MOUSE_DOWN:String = "MOUSE_DOWN";		
		/**
		 * Explain what this is here...
		 */
		public static const MOUSE_MOVE:String = "MOUSE_MOVE";
		/**
		 * Explain what this is here...
		 */
		public static const MOUSE_UP:String = "MOUSE_UP";
		/**
		 * Explain what this is here...
		 */
		public static const MOUSE_OVER:String = "MOUSE_OVER";
		/**
		 * Explain what this is here...
		 */
		public static const MOUSE_OUT:String = "MOUSE_OUT";
		/**
		 * Explain what this is here...
		 */
		public static const CLICK:String = "MOUSE_UP";
		/**
		 * Explain what this is here...
		 */
		public static const DOUBLE_CLICK:String = "DOUBLE_CLICK";
	
		// SHOULD THESE BE MOVED TO MULTITOUCHABLE???------------------------------------------------
		// TODO: RENAME LONG_PRESS to HOLD
		// Dynamic HOLD times [addEventListner(TouchEvent.HOLD, function, setHoldTime)]
		/**
		 * Explain what this is here...
		 */
		public static const LONG_PRESS:String = "LONG_PRESS";	
		
		/*
		public static const PINCH_IN:String = "PINCH_IN";		

		public static const PINCH_OUT:String = "PINCH_OUT";	

		public static const SWIPE_LEFT:String = "SWIPE_LEFT";

		public static const SWIPE_RIGHT:String = "SWIPE_RIGHT";
	
		public static const SWIPE_UP:String = "SWIPE_UP";
	
		public static const SWIPE_DOWN:String = "SWIPE_DOWN";

		
	
		// TAP COUNTING
		*/
		public static const TAP_SEQUENCE:String = "TAP_SEQUENCE";
//---------------------------------------------------------------------------------------------------------------------------------------------	
// CONSTRUCTOR
//---------------------------------------------------------------------------------------------------------------------------------------------
		public function TouchEvent(type:String, bubbles:Boolean = false, cancelable:Boolean = false, stageX:Number = 0, stageY:Number = 0, localX:Number = 0, localY:Number = 0, oldX:Number = 0, oldY:Number = 0, targetObject:DisplayObject = null, ctrlKey:Boolean = false, altKey:Boolean = false, shiftKey:Boolean = false, buttonDown:Boolean = false, delta:int = 0, profile:String = "2Dcur", ID:int = -1, sID:int = -1, angle:Number = 0.0)
		{
			this.profile = profile;
			this.sID = sID;
			this.ID = ID;
			this.angle = angle;
			this.stageX = stageX;
			this.stageY = stageY;
			this.localX = localX;
			this.localY = localY;			
			this.oldX = oldX;
			this.oldY = oldY;
			this.buttonDown = buttonDown;
			this.targetObject = targetObject;			
			super(type, bubbles, cancelable);			
			//super(type, bubbles, cancelable, localX, localY, targetObject, ctrlKey, altKey, shiftKey, buttonDown, delta);	
		}
//---------------------------------------------------------------------------------------------------------------------------------------------	
		public override function clone():Event
		{	
			return new TouchEvent(profile, true, false, stageX, stageY, localX, localY,oldX, oldY, targetObject, false,false,false, true, 0, profile, ID, sID, angle);
		}
//---------------------------------------------------------------------------------------------------------------------------------------------	
		public override function toString():String
		{	
			return formatToString("TouchEvent", profile, "...");
		}
//---------------------------------------------------------------------------------------------------------------------------------------------	
	}
}