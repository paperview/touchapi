package app {
	import flash.display.*;
	import flash.display.Loader;
	import flash.net.URLRequest
	public class nLoader extends MovieClip {
		public function nLoader(getURLv:String,cordX:int, cordY:int)
		{
			var tURL:String = getURLv;
			var urImage:URLRequest = new URLRequest( tURL );
			var img:Loader = new Loader();
			img.load( urImage );
			//img.height = 1000;
			addChild( img );
			//img.width = 200;
			//img.height = 200;
			img.x = 0;	
			img.y = 0;	
				
		}
		
	}
	
}
