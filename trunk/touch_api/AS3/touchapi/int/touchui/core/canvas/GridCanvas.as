package touchui.core.canvas {
	import flash.display.Shape;		
	import flash.events.Event;
	import touchui.core.action.RotatableScalable;	
	
	public class GridCanvas extends RotatableScalable
	{		
		private var clickgrabber:Shape = new Shape();				
		private var sizeX:int = 2000;
		private var sizeY:int = 2000;
		
		private var velX:Number = 0.0;
		private var velY:Number = 0.0;		
		
		private var velAng:Number = 0.0;
		
		private var friction:Number = 0.85;
		private var angFriction:Number = 0.92;
		
		
		function GridCanvas()
		{
			bringToFront = false;			
			noScale = false;
			//noRotate = true;
			
			clickgrabber.graphics.lineStyle(0,0x626262, 1, true);
			clickgrabber.graphics.beginFill(0xffffff, 0.0);
			clickgrabber.graphics.drawRect(-sizeX/2,-sizeY/2,sizeX,sizeY);
			
			var xPos:int=-1000;
			for (var i:int = 0; i<20; i++){
				clickgrabber.graphics.moveTo(xPos,-1000);
				clickgrabber.graphics.lineTo(xPos,1000);
				xPos +=100;
				
				}
				
			var yPos:int=-1000;			
			for (var j:int = 0; j<20; j++){
				clickgrabber.graphics.moveTo(-1000,yPos);
				clickgrabber.graphics.lineTo(1000,yPos);
				yPos +=100;				
				}			
			
			
			this.clickgrabber.graphics.endFill();					
			this.addChild( clickgrabber );		
			
			//this.addEventListener(Event.ENTER_FRAME, slide);		
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
	}
}