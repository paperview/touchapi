/**
 * This will be a low level class that handles the basics of tracking multiple blob id's that may affect this Sprite. This includes touching this Sprite,
 * moving over this sprite. It handles keeping a list of blob id's that are alive and firing off event handler functions. When you extend this class
 * you just implement the event handler funcs. This class will be used to greatly simplify the paint canvas and could also be used to simplify 'RotatableScalable'.

*/
package touchapi.extend
{
	import flash.display.Sprite;		
	import flash.events.Event;
	import flash.events.MouseEvent;	
	import flash.events.KeyboardEvent;
	 import touchapi.*;
	import flash.events.TouchEvent;	
	import flash.geom.Point;
	
	public class Surface extends Sprite
	{
		protected var blobs:Array = new Array();
		public var passive:Boolean = false;
		//protected var state:String;		// blobs we are currently interacting with			
//---------------------------------------------------------------------------------------------------------------------------------------------
// CONSTRUCTOR
//---------------------------------------------------------------------------------------------------------------------------------------------
		public function Surface()
		{
			this.addEventListener(Event.ADDED_TO_STAGE, this.mtAddedToStage, false, 0, true);			
			this.addEventListener(Event.REMOVED_FROM_STAGE, this.mtRemovedFromStage, false, 0, true);						
		}
//---------------------------------------------------------------------------------------------------------------------------------------------
// PUBLIC METHODS
//---------------------------------------------------------------------------------------------------------------------------------------------
		public function getBlobInfo(id:int):Object
		{
			for(var i:int=0; i<blobs.length; i++)
			{
				if(blobs[i].id == id)
					return blobs[i];
			}			
			
			return null;
		}		
		public function getStateInfo():Boolean
		{
		//	return TUIOSocket.state;
		}
//---------------------------------------------------------------------------------------------------------------------------------------------
// PRIVATE METHODS
//---------------------------------------------------------------------------------------------------------------------------------------------
		private function mtAddedToStage(e:Event):void
		{			
			// MOUSE
			this.addEventListener(TouchEvent.MOUSE_MOVE, this.moveHandler, false, 0, true);			
			this.addEventListener(TouchEvent.MOUSE_DOWN, this.downHandler, false, 0, true);						
			this.addEventListener(TouchEvent.MOUSE_UP, this.upHandler, false, 0, true);									
			this.addEventListener(TouchEvent.MOUSE_OVER, this.rollOverHandler, false, 0, true);									
			this.addEventListener(TouchEvent.MOUSE_OUT, this.rollOutHandler, false, 0, true);		
			// TOUCH
			this.addEventListener(MouseEvent.MOUSE_MOVE, this.mouseMoveHandler, false, 0, true);
			this.addEventListener(MouseEvent.MOUSE_DOWN, this.mouseDownHandler, false, 0, true);			
			this.addEventListener(MouseEvent.MOUSE_UP, this.mouseUpHandler, false, 0, true);
			//this.addEventListener(MouseEvent.MOUSE_OVER, this.mouserollOverHandler, false, 0, true);									
			//this.addEventListener(MouseEvent.MOUSE_OUT, this.mouserollOutHandler, false, 0, true);
		}
//---------------------------------------------------------------------------------------------------------------------------------------------
		private function mtRemovedFromStage(e:Event):void
		{		
			// MOUSE
			this.removeEventListener(TouchEvent.MOUSE_MOVE, this.moveHandler);			
			this.removeEventListener(TouchEvent.MOUSE_DOWN, this.downHandler);						
			this.removeEventListener(TouchEvent.MOUSE_UP, this.upHandler);									
			this.removeEventListener(TouchEvent.MOUSE_OVER, this.rollOverHandler);									
			this.removeEventListener(TouchEvent.MOUSE_OUT, this.rollOutHandler);		
			// TOUCH
			this.removeEventListener(MouseEvent.MOUSE_MOVE, this.mouseMoveHandler);
			this.removeEventListener(MouseEvent.MOUSE_DOWN, this.mouseDownHandler);			
			this.removeEventListener(MouseEvent.MOUSE_UP, this.mouseUpHandler);
			//this.addEventListener(MouseEvent.MOUSE_OVER, this.mouserollOverHandler);									
			//this.addEventListener(MouseEvent.MOUSE_OUT, this.mouserollOutHandler);
			
			trace('SURFACE DESTROYED -----------------------------------------------!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!');
		}		
//---------------------------------------------------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------------------------------------------------
		private function idExists(id:int):Boolean
		{
			for(var i:int=0; i<blobs.length; i++)
			{
				if(blobs[i].id == id)
					return true;
			}
			return false;
		}
//---------------------------------------------------------------------------------------------------------------------------------------------
		private function addBlob(id:int, origX:Number, origY:Number, c:Boolean):void
		{	
			if(idExists(id))
				return;

			//trace("Creating new blob " + id + " " + origX + " " + origY + " " + c) ;
			blobs.push( {id: id, clicked:c, origX: origX, origY:origY, clicked:c, history:new Array(new Point(origX, origY)), dX:0.0, dY:0.0, x: origX, y:origY} );
			
			handleBlobCreated(id, origX, origY);
		}
//---------------------------------------------------------------------------------------------------------------------------------------------
		private function removeBlob(id:int):void
		{
			for(var i:int=0; i<blobs.length; i++)
			{
				if(blobs[i].id == id) 
				{
					//trace("blob removed " + id);										
					handleBlobRemoved(id,0,0);										
					blobs.splice(i, 1);		

					return;
				}
			}
		}	
//---------------------------------------------------------------------------------------------------------------------------------------------
		// One of the blobs changed position
		private function updateBlob(id:int, origX:Number, origY:Number):void
		{
			var i:int = 0;
			for(i=0; i<blobs.length; i++)
			{
				if(blobs[i].id == id) 
				{
					blobs[i].history.push(new Point(origX, origY));
					blobs[i].x = origX;
					blobs[i].y = origY;
					//trace("Update blob " + id + " " + origX + "," + origY);
					
					if(blobs[i].history.length >= 2)
					{
						var len:int = blobs[i].history.length;
						blobs[i].dX = blobs[i].history[len-1].x - blobs[i].history[len-2].x;
						blobs[i].dY = blobs[i].history[len-1].y - blobs[i].history[len-2].y;						

//						trace("X: " + blobs[i].history[len-1].x + "," + blobs[i].history[len-2].x);
						//trace("DELTA : " + blobs[i].dX + "," + blobs[i].dY);
					}
					
//					trace(blobs[i].history.length);
					return;
				}
			}
			

		}
//---------------------------------------------------------------------------------------------------------------------------------------------
		private function downHandler(e:TouchEvent):void
		{	
			if(e.stageX == 0 && e.stageY == 0)
				return;			
			
			//-----------------------------------------------------
				TouchAPI.addObjectListener(e.ID, this);	
				//TUIOLite.addObjectListener(e.ID, this);	
			//-----------------------------------------------------						
			
			var curPt:Point = this.globalToLocal(new Point(e.stageX, e.stageY));												

			addBlob(e.ID, curPt.x, curPt.y, true);
			
			handleDownEvent(e.ID, curPt.x, curPt.y, e.target);
			
			if(!passive)
			e.stopPropagation();			
			
		}
//---------------------------------------------------------------------------------------------------------------------------------------------
		private function rollOverHandler(e:TouchEvent):void
		{
			//trace("Rollover");
			if(e.stageX == 0 && e.stageY == 0)
				return;			
			
			var curPt:Point = this.globalToLocal(new Point(e.stageX, e.stageY));																		
			
			if(!idExists(e.ID))
			{	
				//-----------------------------------------------------
				TouchAPI.addObjectListener(e.ID, this);	
				//TUIOLite.addObjectListener(e.ID, this);	
				//-----------------------------------------------------
				addBlob(e.ID, curPt.x, curPt.y, false);
			}
			handleRollOverEvent(e.ID, curPt.x, curPt.y);			
			
			if(!passive)
			e.stopPropagation();							
		}
//---------------------------------------------------------------------------------------------------------------------------------------------
		private function upHandler(e:TouchEvent):void
		{
			var curPt:Point = this.globalToLocal(new Point(e.stageX, e.stageY));				
			handleUpEvent(e.ID, curPt.x, curPt.y);
			removeBlob(e.ID);
			
			if(!passive)
			e.stopPropagation();
		}		
//---------------------------------------------------------------------------------------------------------------------------------------------
		private function moveHandler(e:TouchEvent):void
		{		
			if(e.stageX == 0 && e.stageY == 0)
				return;			
			
			var curPt:Point = this.globalToLocal(new Point(e.stageX, e.stageY));															
			
			if(!idExists(e.ID))
			{
				//trace("Error, shouldn't this blob already be in the list?");
				//TUIOSocket.listenForObject(e.ID, this);			
				addBlob(e.ID, curPt.x, curPt.y, false);
			} else {
				updateBlob(e.ID, curPt.x, curPt.y);				
			}
			
			handleMoveEvent(e.ID, curPt.x, curPt.y, e.target);			
			
			if(!passive)
			e.stopPropagation();						
		}		
//---------------------------------------------------------------------------------------------------------------------------------------------
		private function mouseDownHandler(e:MouseEvent):void
		{
			//trace("Mouse down");
			var curPt:Point = this.globalToLocal(new Point(e.stageX, e.stageY));			
			addBlob(0, curPt.x, curPt.y, true);								
			handleDownEvent(0, curPt.x, curPt.y, e.target);
			
			if(!passive)
			e.stopPropagation();
		}
//---------------------------------------------------------------------------------------------------------------------------------------------
		private function mouseMoveHandler(e:MouseEvent):void
		{
			//trace("Mouse move");			
			var curPt:Point = this.globalToLocal(new Point(e.stageX, e.stageY));						
			
			if(!idExists(0))
			{	
				if(!passive)
				e.stopPropagation();				
				return;				
			} else {
				updateBlob(0, curPt.x, curPt.y);				
			}
		
			handleMoveEvent(0, curPt.x, curPt.y, e.target);
			
			if(!passive)
			e.stopPropagation();
		}
//---------------------------------------------------------------------------------------------------------------------------------------------
		private function stageMouseMoveHandler(e:MouseEvent):void
		{
			//trace("Mouse move");			
			var curPt:Point = this.globalToLocal(new Point(e.stageX, e.stageY));						
			
			if(!idExists(0))
			{	
				if(!passive)
				e.stopPropagation();				
				return;				
			} else {
				updateBlob(0, curPt.x, curPt.y);				
			}			
			handleMoveEvent(0, curPt.x, curPt.y, e.target);
			
			if(!passive)
			e.stopPropagation();
		}		
//---------------------------------------------------------------------------------------------------------------------------------------------
		private function mouseUpHandler(e:MouseEvent):void
		{
			var curPt:Point = this.globalToLocal(new Point(e.stageX, e.stageY));	
			handleUpEvent(0, curPt.x, curPt.y);
			removeBlob(0);			
			
			if(!passive)
			e.stopPropagation();
		}		

//---------------------------------------------------------------------------------------------------------------------------------------------
		private function rollOutHandler(e:TouchEvent):void
		{
			//trace("Rollout");
			if(e.stageX == 0 && e.stageY == 0)
				return;			
		
		// FIXME: only remove if not clicked?
			var curPt:Point = this.globalToLocal(new Point(e.stageX, e.stageY));												
			handleRollOutEvent(e.ID, curPt.x, curPt.y);			

				removeBlob(e.ID);			
			
			if(!passive)
			e.stopPropagation();								
		}
//---------------------------------------------------------------------------------------------------------------------------------------------
// PUBLIC OVERRIDES 
// TODO: Use "interface" [RotateScale extends Touchable implements ITouch]
// TODO: MAKE ALL IDENTICAL (other than first two)
//---------------------------------------------------------------------------------------------------------------------------------------------
		public function handleBlobCreated(id:int, mx:Number=0, my:Number=0, targetObj:Object=null):void
		{
		}	
		public function handleBlobRemoved(id:int, mx:Number=0, my:Number=0, targetObj:Object=null):void
		{
		}
		public function handleDownEvent(id:int, mx:Number=0, my:Number=0, targetObj:Object=null):void
		{
		}
		public function handleUpEvent(id:int, mx:Number=0, my:Number=0, targetObj:Object=null):void
		{
		}		
		public function handleRollOverEvent(id:int, mx:Number=0, my:Number=0, targetObj:Object=null):void
		{
		}
		public function handleRollOutEvent(id:int, mx:Number=0, my:Number=0, targetObj:Object=null):void
		{
		}				
		public function handleMoveEvent(id:int, mx:Number=0, my:Number=0, targetObj:Object=null):void
		{
		}
//---------------------------------------------------------------------------------------------------------------------------------------------
	}
}