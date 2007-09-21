package {

import com.touchlib.*;
import app.*;
import flash.display.Sprite;
import flash.system.Capabilities;


[SWF(width='1680', height='1050', backgroundColor='0x000000', frameRate='33')]

public class Touchlib_AS3 extends Sprite
{
	public function Touchlib_AS3()
		{			
		TUIO.init( this, 'localhost', 3000, Capabilities.screenResolutionX, Capabilities.screenResolutionY, '', true );
		
		var subobj:PhotoCanvas = new PhotoCanvas();
		this.addChild(subobj);
		var flickrLoader:Flickr = new Flickr(subobj);

		var slidy:TouchlibSlider = new TouchlibSlider(50,160);
		var knoby:TouchlibKnob = new TouchlibKnob(120);

		this.addChild(knoby);
		this.addChild(slidy);
	
		//var naturalPaint:PaintSurface = new PaintSurface(Capabilities.screenResolutionX, Capabilities.screenResolutionY);
		//this.addChild(naturalPaint);

		//setChildIndex(subobj,3);
	
		}
	}
}