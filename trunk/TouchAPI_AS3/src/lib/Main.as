package {
	
import com.nui.tuio.TUIO;	
import flash.display.Sprite;
import flash.system.Capabilities;
import app.*;


[SWF(width='1680', height='1050', backgroundColor='0x000000', frameRate='33')]

public class Main extends Sprite
{
	public function Main()
	{		
		TUIO.init( this, 'localhost', 3000, Capabilities.screenResolutionX, Capabilities.screenResolutionY, '', false );
		var nCanvas_obj_0:nCanvas = new nCanvas(0,"square",0xFFFFFF,0.8, "normal", 800, 800, 0, 0);
		this.addChild(nCanvas_obj_0);	
	}
}
}