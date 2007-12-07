package app {	
	import flash.display.Shape;		
	import flash.events.Event;
	import flash.geom.Point;		
	
	public class nCanvas extends nRotatableScalable
	{	
		private var clickgrabber:Shape = new Shape();				
		private var velX:Number = 0;
		private var velY:Number = 0;				
		private var velAng:Number = 0;		
		private var friction:Number = 0;
		private var angFriction:Number = 0;			
		
		function nCanvas(setID:int, setShapi:String, setColor:uint, setAlpha:Number, setBlend:String, setWidth:int, setHeight:int, setX:int, setY:int)
		{
			clickgrabber.blendMode=setBlend;
			bringToFront = false;			
			noScale = false;
			noRotate = false;			
			clickgrabber.graphics.beginFill(setColor, setAlpha);
			if(setShapi == "square")
			{clickgrabber.graphics.drawRoundRect(0,0,setWidth,setHeight,20);}
			else{clickgrabber.graphics.drawCircle(0,0,setWidth);}			
			clickgrabber.graphics.endFill();						
			this.addChild( clickgrabber );			
			clickgrabber.x = setX;
			clickgrabber.y = setY;
			this.addEventListener(Event.ENTER_FRAME, slide);			
		}
		
		
		public override function released(dx:Number, dy:Number, dang:Number)
		{
			velX = dx;
			velY = dy;						
			velAng = dang;
		}
		
		private function slide(e:Event):void
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
				if(Math.abs(velAng) < 0.001)
					velAng = 0;					
				else {
					velAng *= angFriction;				
					this.rotation += velAng;					
				}
			}

		}
		
	}
}