package app {	
	import flash.display.Sprite;	
	import flash.display.Shape;		
	import flash.events.Event;
	import flash.geom.Point;
	import com.touchlib.TUIOEvent;		
	
	public class PhotoCanvas extends RotatableScalable
	{

		private var clickgrabber:Shape = new Shape();				
		private var sizeX:int = 4000;
		private var sizeY:int = 4000;
		
		private var velX:Number = 0.0;
		private var velY:Number = 0.0;		
		
		private var velAng:Number = 0.0;
		
		private var friction:Number = 0.85;
		private var angFriction:Number = 0.92;
		
		function PhotoCanvas()
		{
			bringToFront = true;			
			noScale = false;
			noRotate = false;		
			
			clickgrabber.graphics.lineStyle(10,0x000000,0.05);	
			clickgrabber.graphics.beginFill(0xffffff, 1.0);
			clickgrabber.graphics.drawRoundRect(-sizeX/2,-sizeY/2,sizeX,sizeY,100);
			//clickgrabber.graphics.drawRoundRect(0,0,sizeX,sizeY,30);
			clickgrabber.graphics.endFill();						
			this.addChild( clickgrabber );		
			clickgrabber.alpha = 0.25;
			
			var zui_btn:Shape = new Shape();	
			
			zui_btn.graphics.beginFill(0xF60A0A,0.45);
			//zui_btn.graphics.drawCircle(0, 0, 65);				 
			zui_btn.graphics.drawCircle(-40,-40, 80);	
			//zui_btn.graphics.drawRect(-75, -5, 150,10);	
			//zui_btn.graphics.drawRect(0, 0, 10,150);			
			zui_btn.addEventListener(TUIOEvent.DownEvent, toggleSetup);					
			this.addChild( zui_btn );
					
			this.addEventListener(Event.ENTER_FRAME, slide);			
		}
		
		public override function released(dx:Number, dy:Number, dang:Number)
		{
			velX = dx;
			velY = dy;					
			velAng = dang;
		}
		
		private function slide(e:Event)
		{
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
			}
		}		
		
		public function addPhoto(sz:String)
		{
			var photo:Photo = new Photo( sz );	
			this.addChild(photo);		
		}
			
			
		public function toggleSetup(e:TUIOEvent):void
		{
				if (noScale) {
				noScale = false; 
				trace("scale mode off");
				clickgrabber.alpha = 0.05;			
				}
				else {noScale = true;	
				clickgrabber.alpha = 0.25;	
				}	
				
				
				if (noRotate) {
				noRotate= false; 
				trace("rotate mode off");
				}
				else {noRotate = true;
				}	
						
		}
		
	}
}