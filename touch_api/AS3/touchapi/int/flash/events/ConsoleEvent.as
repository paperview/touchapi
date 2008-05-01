package flash.events {
	
	import flash.events.Event;	
	
	public class ConsoleEvent extends Event
	{
		public var string:String;			
		public var stringAlt:String;		
		public var stringBlt:String;	
		
		public static const WRITE:String = "WRITE";		 
		
		public static const OBJECT_DESTOYED:String = "OBJECT_DESTOYED";	
	//	public static const OBJECT_CREATED:String = "Object Loaded: ";
		
//---------------------------------------------------------------------------------------------------------------------------------------------	
// CONSTRUCTOR
//---------------------------------------------------------------------------------------------------------------------------------------------
		public function ConsoleEvent(type:String, bubbles:Boolean = false, cancelable:Boolean = false, string:String = "", stringAlt:String = "",stringBlt:String = "")
		{
			this.string = string;	
			this.stringAlt = stringAlt;	
			this.stringBlt = stringBlt;	
			super(type, bubbles, cancelable);			
		}
//---------------------------------------------------------------------------------------------------------------------------------------------	
		public override function clone():Event
		{	
			//return new ConsoleEvent(profile, true, false, stageX, stageY, localX, localY,oldX, oldY, targetObject, false,false,false, true, 0, profile, ID, sID, angle);
		}
//---------------------------------------------------------------------------------------------------------------------------------------------	
		public override function toString():String
		{	
			return formatToString("ConsoleEvent", profile, "...");
		}
//---------------------------------------------------------------------------------------------------------------------------------------------	
	}
}