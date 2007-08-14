package com.nui.tuio {
	
/**
 * LastChanged:
 * 
 * $Author$
 * $Revision$
 * $LastChangedDate$
 * $URL$
 * 
 */
 	
	import flash.display.*;
	import flash.events.*;
	import flash.geom.*;
	import flash.net.*;
	import flash.text.*;
	import flash.xml.*;

	public class TUIOObject 
	{
		public var x:Number;
		public var y:Number;		
		public var dX:Number;
		public var dY:Number;						
		public var area:Number;		
		public var TUIOClass:String;		// cur or Obj.. 
		public var sID:int;
		public var ID:int;
		public var angle:Number;		
		public var pressure:Number;		
		private var isNew:Boolean;
		public var isAlive:Boolean;		
		public var obj;
		public var spr:Sprite;		
		private var color:int;

		public function TUIOObject (cls:String, id:int, px:Number, py:Number, dx:Number, dy:Number, sid:int = -1, ang:Number = 0, o = null)
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
/* FLAME ON
var myFlame:Flame=new Flame();
var myBitmapData:BitmapData = new BitmapData(400, 500, false, 0x000000);

stage.addEventListener(Event.ENTER_FRAME, drawFlame);
function drawFlame(myevent:Event):void {
	var myMatrix:Matrix = new Matrix();
	myMatrix.translate(mouseX, mouseY + 3);
	myBitmapData.draw(myFlame, myMatrix);
	var myBlur:BlurFilter = new BlurFilter(2, 10, 2);
	myBitmapData.applyFilter(myBitmapData, myBitmapData.rect, myBitmapData.rect.topLeft, myBlur);
	var myPoint:Point = new Point(0, -3);
	myBitmapData.copyPixels(myBitmapData, myBitmapData.rect, myPoint);
}
var myBitmap:Bitmap=new Bitmap(myBitmapData);
addChild(myBitmap);
*/
			spr = new MovieClip();
			spr.graphics.beginFill( 0xFFFFFF , 0.5);					
			spr.graphics.drawCircle(0,0,7);
			spr.graphics.endFill();
			spr.graphics.lineStyle(1, 0x000000, 1);	
			spr.graphics.drawCircle(0,0,7);		
			spr.graphics.drawCircle(0,0,11);		
			spr.graphics.lineStyle(1, 0xFFFFFF, 1);			
			spr.graphics.drawCircle(0,0,12);		
			//spr.blendMode="invert";		
			//var dropshadow:DropShadowFilter=new DropShadowFilter(0,90, 0x000000, 1, 50, 50);
			//spr.filters=new Array(dropshadow);
			spr.x = x;
			spr.y = y;  		

			try {
 	 			obj = o;
			} catch (e:Error)
			{
				obj = null;
			}
			trace("------------------------------------------------------");			
			trace("Start " + ID + ", " + sID + " (" + int(px) + "," + int(py) + ")");
			
			if(obj)
			{
				try
				{	
				spr.graphics.beginFill( 0xFFFFFF , 1);					
				spr.graphics.drawCircle(0,0,7);
				spr.graphics.endFill();
				var localPoint:Point = obj.parent.globalToLocal(new Point(x, y));				
					trace("Down : " + localPoint.x + "," + localPoint.y);
					obj.dispatchEvent(new TUIOEvent(TUIOEvent.RollOverEvent, true, false, x, y, localPoint.x, localPoint.y, obj, false,false,false, true, 0, TUIOClass, ID, sID, angle));													
					obj.dispatchEvent(new TUIOEvent(TUIOEvent.DownEvent, true, false, x, y, localPoint.x, localPoint.y, obj, false,false,false, true, 0, TUIOClass, ID, sID, angle));									
				} catch (e:Error)
				{
						trace("Failed : " + e);
//					trace(obj.name);
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
						obj.dispatchEvent(new TUIOEvent(TUIOEvent.RollOverEvent, true, false, x, y, localPoint.x, localPoint.y, obj, false,false,false, true, 0, TUIOClass, ID, sID, angle));					
					}
				} else if(obj != o) 
				{
					
					var localPoint:Point = obj.parent.globalToLocal(new Point(x, y));								
					obj.dispatchEvent(new TUIOEvent(TUIOEvent.RollOutEvent, true, false, x, y, localPoint.x, localPoint.y, obj, false,false,false, true, 0, TUIOClass, ID, sID, angle));
					if(o)
					{
						localPoint = obj.parent.globalToLocal(new Point(x, y));
						o.dispatchEvent(new TUIOEvent(TUIOEvent.RollOverEvent, true, false, x, y, localPoint.x, localPoint.y, obj, false,false,false, true, 0, TUIOClass, ID, sID, angle));
					}
					obj = o;								
				}
			} catch (e:Error)
			{
//				trace("ERROR " + e);
			}
		}
		
		public function kill():void
		{
			trace("End: " + ID);		
			if(obj && obj.parent)
			{				
				var localPoint:Point = obj.parent.globalToLocal(new Point(x, y));				
				obj.dispatchEvent(new TUIOEvent(TUIOEvent.RollOutEvent, true, false, x, y, localPoint.x, localPoint.y, obj, false,false,false, true, 0, TUIOClass, ID, sID, angle));				
				obj.dispatchEvent(new TUIOEvent(TUIOEvent.UpEvent, true, false, x, y, localPoint.x, localPoint.y, obj, false,false,false, true, 0, TUIOClass, ID, sID, angle));									
			}			
			obj = null;
		}
	}
}