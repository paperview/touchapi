package touchapi.core
{
	import flash.display.DisplayObject;
	import flash.events.TouchEvent;
	import flash.geom.Point;	
	/**
	* Renders input from TUIOSocket - creates and instance of this class to keep track of input's attributes.
	*/
	public class TUIOObject 
	{		
		
		//FIXME: NAMES SHOULD NOT BE ALL CAPS UNLESS STATIC OR CONST
		private var isNew:Boolean;	
		private var event_array:Array;			
		
		//FIX ME NO PUBLIC
		public var x:Number;
		public var y:Number;		
		
		internal var oldX:Number;
		internal var oldY:Number;		
		internal var dX:Number;
		internal var dY:Number;					
		internal var ID:int;
		internal var sID:int;
		internal var area:Number = 0;	
		internal var width:Number = 0;
		internal var height:Number = 0;		
		internal var angle:Number;		
		internal var pressure:Number;		
		internal var startTime:Number;
		internal var lastModifiedTime:Number;			
		internal var endTime:Number;
		internal var age:Number;
		internal var alive:Boolean;		
		internal var type:String;		
		internal var cursor:Cursor;		
		internal var target:DisplayObject;			
		internal var tapCount:Number;	
	//	internal var lastTapTime:Number;	

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// CONSTRUCTOR
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		public function TUIOObject ($type:String, $id:int, $px:Number, $py:Number, $dx:Number, $dy:Number, $sid:int = -1, $angle:Number = 0, $height:Number=0.0, $width:Number=0.0, $target:DisplayObject = null)
		{
			event_array = new Array();
			type = $type;
			ID = $id;
			x = $px;
			y = $py;
			oldX = $px;
			oldY = $py;
			dX = $dx;
			dY = $dy;
			sID = $sid;
			angle = $angle;			
			width = $width;
			height = $height;
			area = $width * $height;
			alive = true;		
			
			cursor = new Cursor(ID+" ",0xFFFFFF, area, width, height,type,true);		
			cursor.x = x;
			cursor.y = y;  		
			
			var tmp:int = (int(area)/-100000);			
			if (tmp > 0) 
				{	
				trace(tmp);		
				//cursor.areaCursor.graphics.clear();
				cursor.areaCursor.graphics.drawCircle(0 ,0, tmp+10);
				}

			try {
 	 			target = $target;
			} catch (e:Error)
			{
				target = null;
			}
			
			isNew = true;
			
			var d:Date = new Date();
			startTime = d.time;
			lastModifiedTime = startTime;
			//trace(startTime);
			
			
			// 	static public var lastKey:int = 0;
		//static public var timeSinceLastCliked = 0;
		}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// PUBLIC METHODS
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		// FIXME: we could use this function to replace a bunch of the stuff above.. 
		internal function getTouchEvent(event:TouchEvent):TouchEvent
		{
			var localPoint:Point;
			
			if(target && target.parent)
			{
				localPoint = target.parent.globalToLocal(new Point(x, y));							
			} else {
				localPoint = new Point(x, y);
			}
			var t:TouchEvent=new TouchEvent(event.toString(), true, false, x, y, localPoint.x, localPoint.y, 0, 0, target, false,false,false, true, 0, type, ID, sID, angle);
			return t;
		}	
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// INTERNAL METHODS
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		internal function notifyCreated():void
		{
			if(target)
			{
				try
				{
					var localPoint:Point = target.parent.globalToLocal(new Point(x, y));				
					target.dispatchEvent(new TouchEvent(TouchEvent.MOUSE_DOWN, true, false, x, y, localPoint.x, localPoint.y, 0, 0, target, false,false,false, true, 0, type, ID, sID, angle));									
					target.dispatchEvent(new TouchEvent(TouchEvent.MOUSE_OVER, true, false, x, y, localPoint.x, localPoint.y, 0, 0, target, false,false,false, true, 0, type, ID, sID, angle));																		
				} catch (e:Error)
				{
					trace("Failed : " + e);
					target = null;
				}
			}			
		}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------		
		internal function notifyMoved():void
		{
			//var d:Date = new Date();
			//lastModifiedTime = d.time;	
			
	
		
			var localPoint:Point;
			for(var i:int=0; i<event_array.length; i++)
			{
				localPoint = event_array[i].parent.globalToLocal(new Point(x, y));			
				//trace("Notify moved"+ localPoint);
				event_array[i].dispatchEvent(new TouchEvent(TouchEvent.MOUSE_MOVE, true, false, x, y, localPoint.x, localPoint.y, 0, 0, event_array[i], false,false,false, true, 0, type, ID, sID, angle));	
			
				}		
			
			
		}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		internal function notifyRemoved():void
		{
			alive = false;
			var localPoint:Point;			
			if(target && target.parent)
			{				
				localPoint = target.parent.globalToLocal(new Point(x, y));				
				target.dispatchEvent(new TouchEvent(TouchEvent.MOUSE_OUT, true, false, x, y, localPoint.x, localPoint.y, 0, 0, target, false,false,false, true, 0, type, ID, sID, angle));				
				target.dispatchEvent(new TouchEvent(TouchEvent.MOUSE_UP, true, false, x, y, localPoint.x, localPoint.y, 0, 0, target, false,false,false, true, 0, type, ID, sID, angle));									
			}			
			for(var i:int=0; i<event_array.length; i++)
			{
				if(event_array[i] != target)
				{
					localPoint = event_array[i].parent.globalToLocal(new Point(x, y));				
					event_array[i].dispatchEvent(new TouchEvent(TouchEvent.MOUSE_UP, true, false, x, y, localPoint.x, localPoint.y, 0, 0, event_array[i], false,false,false, true, 0, type, ID, sID, angle));								
				}
			}
			
			event_array = new Array();			
			target = null;		
		}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		internal function setObjOver(o:DisplayObject):void
		{	
			var localPoint:Point;	
			try {
				
				if(target == null)
				{
					target = o;				
					if(target) 
					{
						localPoint = target.parent.globalToLocal(new Point(x, y));				
						target.dispatchEvent(new TouchEvent(TouchEvent.MOUSE_OVER, true, false, x, y, localPoint.x, localPoint.y, 0, 0, target, false,false,false, true, 0, type, ID, sID, angle));					
					}
				} else if(target != o) 
				{

					localPoint = target.parent.globalToLocal(new Point(x, y));
					target.dispatchEvent(new TouchEvent(TouchEvent.MOUSE_OUT, true, false, x, y, localPoint.x, localPoint.y, 0, 0, target, false,false,false, true, 0, type, ID, sID, angle));
					if(o)
					{
						localPoint = target.parent.globalToLocal(new Point(x, y));
						o.dispatchEvent(new TouchEvent(TouchEvent.MOUSE_OVER, true, false, x, y, localPoint.x, localPoint.y, 0, 0, target, false,false,false, true, 0, type, ID, sID, angle));
					}
					target = o;
				}
			} catch (e:Error)
			{
			//trace("ERROR " + e);
			}
		}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		internal function addListener(reciever:Object):void
		{
			for(var i:int = 0; i<event_array.length; i++)
			{
				if(event_array[i] == reciever)			
					return;
			}
			event_array.push(reciever);
		}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		internal function removeListener(reciever:Object):void
		{
			for(var i:int = 0; i<event_array.length; i++)
			{
				if(event_array[i] == reciever)
					event_array.splice(i, 1);
			}
		}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	} 
}