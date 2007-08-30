package {
	
import com.nui.tuio.TUIO;	
import flash.display.Sprite;
import flash.system.Capabilities;


[SWF(width='1024', height='768', backgroundColor='0x000000', frameRate='33')]

public class MainJens extends Sprite
{
	public function MainJens()
	{		
		TUIO.init( this, 'nui.mine.nu', 3000, Capabilities.screenResolutionX, Capabilities.screenResolutionY, '', false );	
	}
}
}

