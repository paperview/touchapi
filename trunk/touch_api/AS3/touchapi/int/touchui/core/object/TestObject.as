	package touchui.core.object{
	import flash.display.Shape;
	import flash.text.TextField;
	import flash.text.TextFormat;
	import flash.text.TextFieldAutoSize;	
	
	//import touchui.core.utility.ColorUtil;
	import touchui.core.action.*;
	
	
	public class TestObject extends RotatableScalable //Physical//RotateScale//RotatableScalable 
	{		
		[Embed(source = "bin/Arial.ttf", fontFamily = "myFont")]  
		
		private var myFont:Class;
		private var clickGrabber:Shape = new Shape();			
		public var clickBorder:Shape = new Shape();
		public var _count:String;	
		public var n_text:TextField;		
		public var s_text:TextField;	
		public function TestObject (count:String, wt:Number, ht:Number, $color:int = 0x222222, shape:String = "square")
		{	
			_count=count;				 
			//var color:int=0x222222;
			//color = ColorUtil.random(25,25,50);						    			
			clickGrabber.graphics.beginFill($color,1);	
			clickGrabber.graphics.lineStyle(2.5,0x666666,1);
			
			drawLabel();
			
			if (shape == "circle"){
			clickGrabber.graphics.drawCircle(-wt/2, -ht/2, wt);
			clickGrabber.graphics.endFill();		
			
			clickBorder.graphics.lineStyle(2.5,0xFFFFFF,1);
			clickBorder.graphics.drawCircle(-wt/2, -ht/2, wt);		
			clickBorder.alpha = 0;	
			
			n_text.x = -wt/2 - (n_text.width/2);
			n_text.y = -ht/2 - (n_text.height/2)-12;
			}else
			{
			clickGrabber.graphics.drawRect(-wt/2, -ht/2, wt,ht);
			clickGrabber.graphics.endFill();		
			
			clickBorder.graphics.lineStyle(2.5,0xFFFFFF,1);
			clickBorder.graphics.drawRect(-wt/2, -ht/2, wt,ht);		
			clickBorder.alpha = 0;
			n_text.x = -n_text.width/2;
			n_text.y = -n_text.height / 2;
			}	
			
			this.addChild( clickGrabber );	
			this.addChild( clickBorder );
			this.setChildIndex(n_text, this.numChildren-1);
		}
		public function drawLabel():void
		{				
			var format:TextFormat = new TextFormat();
			format.font= "myFont";
			format.color = 0x000000;
			format.size = 24;				
			var count:int = 0;	
			n_text = new TextField();						
			n_text.autoSize = TextFieldAutoSize.CENTER;
			n_text.selectable = false;
			n_text.defaultTextFormat = format;
			n_text.embedFonts = true;
			n_text.text = _count;
			n_text.blendMode = 'invert';
			//n_text.alpha = 0;	
			this.addChild( n_text );
			
			format.color = 0x000000;
			format.size = 12;				
			s_text = new TextField();		
			s_text.blendMode = 'invert';					
			s_text.autoSize = TextFieldAutoSize.CENTER;
			s_text.selectable = false;
			s_text.defaultTextFormat = format;
			s_text.embedFonts = true;
			//s_text.text = "...";
			//n_text.alpha = 0;	
			this.addChild( s_text );			
			
			//s_text.x = 75;
			s_text.y = -100;
		}
	}
}