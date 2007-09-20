//////////////////////////////////////////////////////////////////////
//                                                                  //
//    Main Document Class. Sets TUIO and adds main parts to stage   //
////
//////////////////////////////////////////////////////////////////////

package app.createMultiKey {

	import flash.display.Shape;		
	import flash.display.Loader;		
	import flash.events.Event;
	import flash.net.URLLoader;
	import flash.net.URLRequest;
	import flash.geom.Point;			
    import flash.filters.BitmapFilter;
    import flash.filters.BitmapFilterQuality;
    import flash.filters.DropShadowFilter;
	import flash.geom.ColorTransform;
	import flash.media.SoundMixer;
	import flash.media.SoundTransform;

    import AssemblePiano;
	import RotatableScalable;
	import whitenoise.*;
	import flash.display.Sprite;
	
	

	public class AddPiano extends RotatableScalable {
		
		private var clickgrabber:Shape = new Shape();
		
		private var velX:Number = 0.0;
		private var velY:Number = 0.0;			
		private var velAng:Number = 0.0;		
		private var friction:Number = 0.85;
		private var angFriction:Number = 0.92;
		
		var wholePiano:AssemblePiano;


		public function AddPiano(octaves:int) {
			
			wholePiano = new AssemblePiano(octaves);			
			
			bringToFront = true;
			noScale = true;          //make it not scale
			noRotate = true;         //make it not rotate
			noMove = true;           //make it not move
	
			moveScalePoints(octaves);
			
			clickgrabber.graphics.beginFill(0xFFFFFF, .01);
			clickgrabber.graphics.drawRect(0, 0, 1,1);
			clickgrabber.graphics.endFill();			

			addChild(clickgrabber);		
			addChild(wholePiano);   
			
			arrange();

			this.addEventListener(Event.ENTER_FRAME, slide);			
		}				
		
		
		private function arrange() {

				wholePiano.x = -wholePiano.width/2;
				wholePiano.y = -wholePiano.height/2;
				
				wholePiano.scaleX = 1;
				wholePiano.scaleY = 1;	
				
				clickgrabber.scaleX = wholePiano.width;
				clickgrabber.scaleY = wholePiano.height;
				
				clickgrabber.x = -wholePiano.width/2;
				clickgrabber.y = -wholePiano.height/2;
		}		
		
		
		public override function released(dx:Number, dy:Number, dang:Number) {
			
			velX = dx;			
			velY = dy;					
			velAng = dang;
		}
		
		
		private function slide(e:Event):void {
			
			if(this.state == "none")
			{		
				if(Math.abs(velX) < 0.001)
					velX = 0;
				else {
					x += velX;
					velX *= friction;										
				}
				if(Math.abs(velY) < 0.001)
					velY = 0;					
				else {
					y += velY;
					velY *= friction;						
				}
				if(Math.abs(velAng) < 0.001)
					velAng = 0;					
				else {
					velAng *= angFriction;				
					this.rotation += velAng;					
				}
			}
		}
		
		
		
		private function moveScalePoints(octaves:int){
			
			var scalePoints:Sprite = new Sprite();             
			
			if (octaves == 2)
			{			
				var outlineW:Number = -wholePiano.width - 80 ;
				var outlineH:Number = -wholePiano.height - 80;			
			}		
			if (octaves == 1) 
			{
				var outlineW:Number = -920;
				var outlineH:Number = 680;
			}			
			
			scalePoints.graphics.lineStyle(4, 0xffffff, 1);
			scalePoints.graphics.beginFill(0xFFFFFF, .6);
			scalePoints.graphics.moveTo(-outlineW/2, -outlineH/2);
			scalePoints.graphics.lineTo(outlineW/2, -outlineH/2);
			scalePoints.graphics.lineTo(outlineW/2, outlineH/2);
			scalePoints.graphics.lineTo(-outlineW/2, outlineH/2);
			scalePoints.graphics.lineTo(-outlineW/2, -outlineH/2);
			scalePoints.graphics.endFill();	
					
			scalePoints.graphics.lineStyle(15, 0xFFFFFF, 1 );
			scalePoints.graphics.moveTo(-outlineW/2, -outlineH/2);
			scalePoints.graphics.lineTo(-outlineW/2.2, -outlineH/2);
			scalePoints.graphics.moveTo(-outlineW/2, -outlineH/2);
			scalePoints.graphics.lineTo(-outlineW/2, -outlineH/2.5);			
			scalePoints.graphics.moveTo(outlineW/2, -outlineH/2);
			scalePoints.graphics.lineTo(outlineW/2.2, -outlineH/2);
			scalePoints.graphics.moveTo(outlineW/2, -outlineH/2);
			scalePoints.graphics.lineTo(outlineW/2, -outlineH/2.5);			
			scalePoints.graphics.moveTo(-outlineW/2, outlineH/2);
			scalePoints.graphics.lineTo(-outlineW/2, outlineH/2.5);
			scalePoints.graphics.moveTo(-outlineW/2, outlineH/2);
			scalePoints.graphics.lineTo(-outlineW/2.2, outlineH/2);
			scalePoints.graphics.moveTo(outlineW/2, outlineH/2);
			scalePoints.graphics.lineTo(outlineW/2.2, outlineH/2);
			scalePoints.graphics.moveTo(outlineW/2, outlineH/2);
			scalePoints.graphics.lineTo(outlineW/2, outlineH/2.5);
			
			addChild(scalePoints);

			scalePoints.addEventListener(TUIOEvent.DownEvent, makeRotatableScalable);
			scalePoints.addEventListener(TUIOEvent.UpEvent, removeRotatableScalable);		
		}	
		
			
		private function makeRotatableScalable(event:TUIOEvent):void {
			
			//trace("Scaling/Rotating On");
		
			noScale = false;
			noRotate = false;
			noMove = false;			
			
			// highlight key when pressed	
			var colorTransform:ColorTransform = transform.colorTransform;
			colorTransform.color = 0xFFFF99;
			event.target.transform.colorTransform = colorTransform;	
			
			// turn sound off when scaling/rotating	
			var globalSound: SoundTransform = SoundMixer.soundTransform; 
			globalSound.volume = 0; 
			SoundMixer.soundTransform = globalSound; 			
		}		
		
		public function removeRotatableScalable(event:TUIOEvent):void {
			
			//trace("Scaling/Rotating Off");		
			
			noScale = true;
			noRotate = true;
			noMove = true;	
			
			// highlight key when pressed				
			var colorTransform:ColorTransform = transform.colorTransform;
			colorTransform.color = 0xFFFFFF;
			event.target.transform.colorTransform = colorTransform;
			
			// turn sound back on	
			var globalSound: SoundTransform = SoundMixer.soundTransform; 
			globalSound.volume = 1; 
			SoundMixer.soundTransform = globalSound; 
		}			
	}
}