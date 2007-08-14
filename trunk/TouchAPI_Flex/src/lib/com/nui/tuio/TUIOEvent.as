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
 
 /**
 * Copyright (c) 2007 Christian Moore, www.nuigroup.com, Jens Franke, www.jensfranke.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.   
 */
 	
	import flash.events.*;
	import flash.xml.*;
	import flash.net.*
	import flash.display.*;
	import flash.geom.*;
	import flash.text.*;
	import flash.net.*;
	import flash.utils.describeType;	
	public class TUIOEvent extends Event
	{
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
		public static var MoveEvent:String = "TUIO_MOVE";
		public static var DownEvent:String = "TUIO_DOWN";		
		public static var UpEvent:String = "TUIO_UP";				
		public static var RollOverEvent:String = "TUIO_ROLLOVER";						
		public static var RollOutEvent:String = "TUIO_ROLLOUT";								
		
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