package flash.events {
	
	import flash.display.DisplayObject;
	import flash.events.Event;	
	/**
	* General TUIOSocket event schema - custom events tailored towards touch input paradigms.
	*/
	public class GestureEvent extends Event
	{
		public var ID:int;
		public var stageX:Number;
		public var stageY:Number;
		public var localX:Number;
		public var localY:Number;
		public var oldX:Number;
		public var oldY:Number;
		public var targetObject:DisplayObject;
		private var buttonDown:Boolean;
		
		/*  UNUSED VARS	
			-----------------------------------------------------
			public var timestamp:Date;
			public var info:Array;
			public var tapCount:Array;
		 */
		// TODO: SEE David's Physical.fla test (continuous vs discrete pinching)
		// SHOULD THESE BE MOVED TO MULTITOUCHABLE???------------------------------------------------
		// TODO: RENAME LONG_PRESS to HOLD
		// Dynamic HOLD times [addEventListner(TouchEvent.HOLD, function, setHoldTime)]
		public static const HOLD:String = "HOLD";	
		public static const PINCH_IN:String = "PINCH_IN";		
		public static const PINCH_OUT:String = "PINCH_OUT";	
		public static const SWIPE_LEFT:String = "SWIPE_LEFT";
		public static const SWIPE_RIGHT:String = "SWIPE_RIGHT";
		public static const SWIPE_UP:String = "SWIPE_UP";
		public static const SWIPE_DOWN:String = "SWIPE_DOWN";
		/*
		// TAP COUNTING
		*/
//---------------------------------------------------------------------------------------------------------------------------------------------	
// CONSTRUCTOR
//---------------------------------------------------------------------------------------------------------------------------------------------
		public function GestureEvent (type:String, bubbles:Boolean = false, cancelable:Boolean = false, stageX:Number = 0, stageY:Number = 0, localX:Number = 0, localY:Number = 0, oldX:Number = 0, oldY:Number = 0, targetObject:DisplayObject = null, ctrlKey:Boolean = false, altKey:Boolean = false, shiftKey:Boolean = false, buttonDown:Boolean = false, delta:int = 0,ID:int = -1)
		{
			this.ID = ID;
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
	}
}