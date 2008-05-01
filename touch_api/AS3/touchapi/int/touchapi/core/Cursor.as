package touchapi.core
{
	
	import flash.display.Sprite;	
	import flash.display.Shape;
	import flash.text.TextFormat;
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;	
	//import flash.filters.DropShadowFilter;
	//import flash.filters.DropShadowFilter;
	//var n_shadow : DropShadowFilter = new DropShadowFilter(5,90,0,0.5,25,25,2,1);
	//this.filters=[n_shadow];				
	/**
	* A simple cursor class for TUIOObject - renders a small graphic on screen while in debug mode.
	*/
	internal class Cursor extends Sprite
	{
		public var text:TextField;	
		public var areaCursor:Sprite;
//---------------------------------------------------------------------------------------------------------------------------------------------	
// CONSTRUCTOR  // ID+" ",0xFFFFFF, area, width, height,type,true
//---------------------------------------------------------------------------------------------------------------------------------------------	
		public function Cursor($text:String = '', $color:int = 0xFFFFFF , $pressure:Number = 0, $width:Number= 0, $height:Number= 0, $type:String = null, $debug:Boolean=true)
		{
			//areaCursor.graphics.lineStyle( 0, 0xFFFF00);	
			//areaCursor.graphics.drawCircle(0, 0, 5);
			
			
			
			// FIXME: DO NOT!!!! PUT A FILL ON THIS CAUSE IT WILL CAUSE BUG IN UP/OVER/OUT Events... ------------------------
			//super();
			if (TUIOSocket.debug) { 	
				
			areaCursor = new Sprite();			
			//trace(areaCursor.name);
			areaCursor.x = 0;
			areaCursor.y = 0;
			this.addChild(areaCursor);
				
			if ($type == "2Dcur")
			{	
				this.blendMode="invert";
				this.graphics.lineStyle( 2, 0x000000);			
				//this.graphics.beginFill( 0xFFFFFF,0.75);	
				this.graphics.drawCircle(0 , 0,16);		
				//this.graphics.drawRect( -30 , -30, 60, 60);				
				
				
			}		
			else if ($type == "2Dobj")
			{
				this.graphics.lineStyle( 0, 0xFF0000);	
				this.graphics.drawRect(0 , 0, 25,25);	
			}		
		
			//var n_shadow : DropShadowFilter = new DropShadowFilter(5,90,0,0.5,25,25,2,1);
			//this.filters=[n_shadow];

			/*	
			 * var tmp:int = (int($pressure) / -100000);
			trace(tmp);
			 if (tmp > 0) {			
				this.graphics.drawCircle(0 ,0, tmp+10);
			   } 
			// Draw a cross and filled circle
			//graphics.beginFill(color , 1);	
			graphics.beginFill(0xFF00FF , 1);	
			graphics.drawCircle(0 ,0, 10);	
			graphics.lineStyle( 1, 0x000000 );
			graphics.moveTo( 0, -6 );
			graphics.lineTo( 0, 6 );
			graphics.moveTo( -6, 0 );
			graphics.lineTo( 6, 0 );
			graphics.endFill();
			*/
			
			if ($text != '')
			{	
			//TODO: GET ID/X/Y/Area Data into this text ------------------------
				//this.blendMode = "normal";
				var format:TextFormat = new TextFormat();
				text = new TextField();
	        	format.font = 'Verdana';
	     		format.color = 0xFFFFFF;
	       	 	format.size = 10;
				text.defaultTextFormat = format;
				text.autoSize = TextFieldAutoSize.LEFT;
				//text.background = true;	
				//text.backgroundColor = 0x000000;	
				//text.border = true;	
				text.text = '';
				text.appendText(' '+$text+'  ');				
				text.x = 25;
				text.y = -10;  				
				addChild(text);
			}
		} else {			
			// HIDE CURSOR
			   }	
		}		
//---------------------------------------------------------------------------------------------------------------------------------------------	
	}
}