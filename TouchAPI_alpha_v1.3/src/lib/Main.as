package {
import com.nui.tuio.TUIO;

import flash.display.Sprite;
import flash.system.Capabilities;

[SWF(width='1680', height='1050', backgroundColor='0x000000', frameRate='33')]
public class Main extends Sprite
{
	public function Main()
	{		
		TUIO.init( this, 'nui.mine.nu', 3000, Capabilities.screenResolutionX, Capabilities.screenResolutionY, '', true );	
	}
}
}
