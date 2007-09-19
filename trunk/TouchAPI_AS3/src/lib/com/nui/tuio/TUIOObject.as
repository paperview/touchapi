package com.nui.tuio {
	
/**
 * LastChanged:
 * 
 * $Author: naturalui $
 * $Revision: 66 $
 * $LastChangedDate: 2007-08-25 08:11:49 -0500 (Sat, 25 Aug 2007) $
 * $URL: https://touchapi.googlecode.com/svn/trunk/TouchAPI_Flex/src/lib/com/nui/tuio/TUIOObject.as $
 * 
 */ 
import flash.display.DisplayObject;
//import flash.display.MovieClip;
import flash.display.Sprite;
import flash.geom.Point;
//import flash.events.MouseEvent;


	public class TUIOObject 
	{
		public var x:Number;
		public var y:Number;		
		public var dX:Number;
		public var dY:Number;						
		public var area:Number;		
		public var TUIOClass:String;		// cur or Obj.. 
		public var ID:int;
		public var sID:int;	
		public var xid:int;
		public var angle:Number;		
		public var pressure:Number;		
		private var isNew:Boolean;
		public var isAlive:Boolean;		
		public var obj:DisplayObject; //changed to interactive object?
		public var spr:TUIOCursor;		
		private var color:int;
		private var DEBUG_TEXT:TextField;

		public function TUIOObject (cls:String, id:int, px:Number, py:Number, dx:Number, dy:Number, sid:int, ang:Number = 0, o:DisplayObject = null)
		{
			TUIOClass = cls;
			ID = id;	
			x = px;
			y = py;
			dX = dx;
			dY = dy;
			sID = sid;
			angle = ang;
			isAlive = true;		

			spr = new TUIOCursor(ID.toString());		
			spr.x = x;
			spr.y = y;  			
						
			try {
 	 			obj = o;
			} catch (e:Error)
			{
				obj = null;
			}			
	
			if(obj)
			{
				try
				{	
				spr.graphics.beginFill( 0xFFFFFF , 1);					
				spr.graphics.drawCircle(0,0,20);
				spr.graphics.endFill();
				var localPoint:Point = obj.parent.globalToLocal(new Point(x, y));				
				//obj.dispatchEvent(new MouseEvent(MouseEvent.ROLL_OVER,false, false, localPoint.x, localPoint.y, obj, false, false, false, true, 0));	
				//obj.dispatchEvent(new MouseEvent(MouseEvent.CLICK,false, false, localPoint.x, localPoint.y, obj, false, false, false, true, 0));	
				obj.dispatchEvent(new TUIOEvent(TUIOEvent.ROLL_OVER, true, false, x, y, localPoint.x, localPoint.y, obj, false,false,false, true, 0, TUIOClass, ID, sID, angle));													
				obj.dispatchEvent(new TUIOEvent(TUIOEvent.DOWN, true, false, x, y, localPoint.x, localPoint.y, obj, false,false,false, true, 0, TUIOClass, ID, sID, angle));									
				} catch (e:Error)
				{
					obj = null;
				}
			}
			
			isNew = true;
		}
		
		public function setObjOver(o:DisplayObject):void
		{
			try {
				
				if(obj == null)
				{
					obj = o;				
					if(obj) 
					{
						var localPoint:Point = obj.parent.globalToLocal(new Point(x, y));
						//obj.dispatchEvent(new MouseEvent(MouseEvent.ROLL_OVER,false, false, localPoint.x, localPoint.y, obj, false, false, false, true, 0));					
						obj.dispatchEvent(new TUIOEvent(TUIOEvent.ROLL_OVER, true, false, x, y, localPoint.x, localPoint.y, obj, false,false,false, true, 0, TUIOClass, ID, sID, angle));					
					}
				} else if(obj != o) 
				{					
					//var localPoint:Point = obj.parent.globalToLocal(new Point(x, y));	
					//obj.dispatchEvent(new MouseEvent(MouseEvent.ROLL_OUT,false, false, localPoint.x, localPoint.y, obj, false, false, false, true, 0));								
					obj.dispatchEvent(new TUIOEvent(TUIOEvent.ROLL_OUT, true, false, x, y, localPoint.x, localPoint.y, obj, false,false,false, true, 0, TUIOClass, ID, sID, angle));
					if(o)
					{
						localPoint = obj.parent.globalToLocal(new Point(x, y));
						//obj.dispatchEvent(new MouseEvent(MouseEvent.ROLL_OVER,false, false, localPoint.x, localPoint.y, obj, false, false, false, true, 0));	
						o.dispatchEvent(new TUIOEvent(TUIOEvent.ROLL_OVER, true, false, x, y, localPoint.x, localPoint.y, obj, false,false,false, true, 0, TUIOClass, ID, sID, angle));
					}
					obj = o;								
				}
			} catch (e:Error)
			{
//				trace("ERROR " + e);
			}
		}
		
		public function disposeObject():void
		{
			if(obj && obj.parent)
			{				
				var localPoint:Point = obj.parent.globalToLocal(new Point(x, y));
				//obj.dispatchEvent(new MouseEvent(MouseEvent.ROLL_OUT,false, false, localPoint.x, localPoint.y, obj, false, false, false, true, 0));					
				//obj.dispatchEvent(new MouseEvent(MouseEvent.UP,false, false, localPoint.x, localPoint.y, obj, false, false, false, true, 0));	
				obj.dispatchEvent(new TUIOEvent(TUIOEvent.ROLL_OUT, true, false, x, y, localPoint.x, localPoint.y, obj, false,false,false, true, 0, TUIOClass, ID, sID, angle));				
				obj.dispatchEvent(new TUIOEvent(TUIOEvent.UP, true, false, x, y, localPoint.x, localPoint.y, obj, false,false,false, true, 0, TUIOClass, ID, sID, angle));									
			}			
			obj = null;
		}
	}
}