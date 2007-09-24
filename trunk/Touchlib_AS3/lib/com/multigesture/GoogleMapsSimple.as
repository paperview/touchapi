/* 
Class/Package name: GoogleMapsSimple
Description: GoogleMaps
Created by Laurence Muller (aka Falcon4ever)
E-mail: info@falcon4ever.com
Site: www.multigesture.net
License: please check the readme.txt

Usage example (set a proxy server):
	var gm:GoogleMapsSimple = new GoogleMapsSimple("www.falcon4ever.com");
	thestage.addChild(gm);	
	
sources:
	http://web.media.mit.edu/~nvawter/projects/googlemaps/
*/

package com.multigesture {
	import flash.display.*;
	import flash.events.*;
	import com.touchlib.*;		
	import app.RotatableScalable;	
	import flash.net.*;	
	import flash.util.trace;
	
	class GoogleMapsSimple extends RotatableScalable {		
		// Items
		private var clickgrabber:Shape = new Shape();		
		private var photoLoader:Loader = null;		
		private var photoLoader_hybrid:Loader = null;				
		private var border:Shape = new Shape();
		
		private var upperleft:Shape = new Shape();
		private var upperright:Shape = new Shape();
		private var bottomleft:Shape = new Shape();
		private var bottomright:Shape = new Shape();

		private var goback:Shape = new Shape();
		
		private var move_right:Shape = new Shape();
		private var move_left:Shape = new Shape();
		private var move_up:Shape = new Shape();
		private var move_down:Shape = new Shape();
				
		private var current_location:String;
		private var conv_location:String;
		private var H:String;
		private var V:String;		
		
		// Default values
		private var proxy_server:String;	
		
		private var border_size:Number = 5.0;
		private var velX:Number = 0.0;
		private var velY:Number = 0.0;		
		private var velAng:Number = 0.0;		
		private var friction:Number = 0.85;
		private var angFriction:Number = 0.92;
		
		public function GoogleMapsSimple(proxy:String) {			
			proxy_server = proxy;
		
			photoLoader = new Loader();
			photoLoader_hybrid = new Loader();
			photoLoader.contentLoaderInfo.addEventListener(Event.COMPLETE, arrange);					
			
			upperleft.graphics.beginFill(0xffffff, 0.3);
			upperleft.graphics.drawRect(0, 0, 0.25, 0.25);
			upperleft.graphics.endFill();	
			
			bottomleft.graphics.beginFill(0xffffff, 0.3);
			bottomleft.graphics.drawRect(0, 0, 0.25, 0.25);
			bottomleft.graphics.endFill();	

			upperright.graphics.beginFill(0xffffff, 0.3);
			upperright.graphics.drawRect(0, 0, 0.25, 0.25);
			upperright.graphics.endFill();	
			
			bottomright.graphics.beginFill(0xffffff, 0.3);
			bottomright.graphics.drawRect(0, 0, 0.25, 0.25);
			bottomright.graphics.endFill();	
			
			goback.graphics.beginFill(0xffffff, 0.3);
			goback.graphics.drawRect(0, 0, 0.25, 0.25);
			goback.graphics.endFill();				

			move_right.graphics.beginFill(0xffffff, 0.3);
			move_right.graphics.drawRect(0, 0, 0.25, 0.25);
			move_right.graphics.endFill();	
			
			move_left.graphics.beginFill(0xffffff, 0.3);
			move_left.graphics.drawRect(0, 0, 0.25, 0.25);
			move_left.graphics.endFill();	

			move_up.graphics.beginFill(0xffffff, 0.3);
			move_up.graphics.drawRect(0, 0, 0.25, 0.25);
			move_up.graphics.endFill();	
			
			move_down.graphics.beginFill(0xffffff, 0.3);
			move_down.graphics.drawRect(0, 0, 0.25, 0.25);
			move_down.graphics.endFill();	
			
			clickgrabber.graphics.beginFill(0xffffff, 0.1);
			clickgrabber.graphics.drawRect(0, 0, 1,1);
			clickgrabber.graphics.endFill();			
			
			H = "";
			V = "";		
			conv_location = "";			
			current_location = "t";
			
			var request:URLRequest = new URLRequest("http://" + proxy_server + "/googlemaps_proxy/cache.php?mode=tile&t=" + current_location);			
			photoLoader.unload();
			photoLoader.load(request);	
			
			var request:URLRequest = new URLRequest("http://" + proxy_server + "/googlemaps_proxy/cache_hybrid.php?mode=tile&t=" + current_location);			
			photoLoader_hybrid.unload();
			photoLoader_hybrid.load(request);	
			
			addChild(border);
			addChild(photoLoader);				
			addChild(photoLoader_hybrid);
			addChild(clickgrabber);
			
			addChild(upperleft);
			addChild(bottomleft);			
			addChild(upperright);
			addChild(bottomright);		
			
			addChild(goback);		
			
			addChild(move_right);		
			addChild(move_left);		
			addChild(move_up);		
			addChild(move_down);					
			
			upperleft.addEventListener(TUIOEvent.DownEvent, addArguments(change_location, ["q"]));
			bottomleft.addEventListener(TUIOEvent.DownEvent, addArguments(change_location, ["t"]));			
			upperright.addEventListener(TUIOEvent.DownEvent, addArguments(change_location, ["r"]));
			bottomright.addEventListener(TUIOEvent.DownEvent, addArguments(change_location, ["s"]));			
			
			goback.addEventListener(TUIOEvent.DownEvent, addArguments(change_location, ["back"]));

			move_right.addEventListener(TUIOEvent.DownEvent, addArguments(change_location, ["move_right"]));
			move_left.addEventListener(TUIOEvent.DownEvent, addArguments(change_location, ["move_left"]));
			move_up.addEventListener(TUIOEvent.DownEvent, addArguments(change_location, ["move_up"]));
			move_down.addEventListener(TUIOEvent.DownEvent, addArguments(change_location, ["move_down"]));			

			this.addEventListener(Event.ENTER_FRAME, slide);	
			
			this.x = 1024/4;
			this.y = 768/2;
		}
		
		// Wrapper to pass values to an event.
		public function addArguments(method:Function, additionalArguments:Array):Function {
			return function(event:TUIOEvent):void {
				method.apply(null, [event].concat(additionalArguments));
			}
		}
		
		private function change_location(event:TUIOEvent, pos:String)
		{
			if(pos == "back")
			{
				if(current_location.length > 1)
					current_location = current_location.substring(0, current_location.length-1);
				else
					return;			
			} 
			else if(pos == "move_right") {
				conv_location = str_to_int(current_location);	// Convert string to new format
				conv_to_bin(conv_location);	// Calculate H and V array

				// Increment H value to move right
				var temp1:int = binToDec(H) + 1;		
				var temp2:String = decToBin(temp1);
				
				// Fix padding 0's
				if(temp2.length < current_location.length)
				{
					while(temp2.length < current_location.length)
						temp2 = "0" + temp2;
				}				
				H = temp2;
				
				var new_localtion:String = conv_to_gm(current_location);
				trace("possible new location: " + new_localtion);
				
				if(new_localtion.length == current_location.length && new_localtion.charAt(0) == "t")
					current_location = new_localtion;
				else
					return;				
			} 
			else if(pos == "move_left") {				
				conv_location = str_to_int(current_location);	// Convert string to new format
				conv_to_bin(conv_location);	// Calculate H and V array

				// Decrement H value to move left
				var temp1:int = binToDec(H) - 1;		
				var temp2:String = decToBin(temp1);
				
				// Fix padding 0's
				if(temp2.length < current_location.length)
				{
					while(temp2.length < current_location.length)
						temp2 = "0" + temp2;
				}				
				H = temp2;
				
				var new_localtion:String = conv_to_gm(current_location);
				trace("possible new location: " + new_localtion);
				
				if(new_localtion.length == current_location.length && new_localtion.charAt(0) == "t")
					current_location = new_localtion;
				else
					return;
			} 
			else if(pos == "move_up") {			
				conv_location = str_to_int(current_location);	// Convert string to new format
				conv_to_bin(conv_location);	// Calculate H and V array

				// Decrement v value to move up
				var temp1:int = binToDec(V) - 1;		
				var temp2:String = decToBin(temp1);
				
				// Fix padding 0's
				if(temp2.length < current_location.length)
				{
					while(temp2.length < current_location.length)
						temp2 = "0" + temp2;
				}				
				V = temp2;
				
				var new_localtion:String = conv_to_gm(current_location);
				trace("possible new location: " + new_localtion);
				
				if(new_localtion.length == current_location.length && new_localtion.charAt(0) == "t")
					current_location = new_localtion;
				else
					return;
			} 
			else if(pos == "move_down") {
				conv_location = str_to_int(current_location);	// Convert string to new format
				conv_to_bin(conv_location);	// Calculate H and V array

				// Increment v value to move down
				var temp1:int = binToDec(V) + 1;		
				var temp2:String = decToBin(temp1);
				
				// Fix padding 0's
				if(temp2.length < current_location.length)
				{
					while(temp2.length < current_location.length)
						temp2 = "0" + temp2;
				}				
				V = temp2;
				
				var new_localtion:String = conv_to_gm(current_location);
				trace("possible new location: " + new_localtion);
				
				if(new_localtion.length == current_location.length && new_localtion.charAt(0) == "t")
					current_location = new_localtion;
				else
					return;				
			}
			else
			{
				if(current_location.length < 12)
					current_location += pos;				
				else
					return;
			}
			
			trace("new location: " + current_location);
			var request:URLRequest = new URLRequest("http://" + proxy_server + "/googlemaps_proxy/cache.php?mode=tile&t=" + current_location);			
			photoLoader.unload();
			photoLoader.load(request);					
			
			var request:URLRequest = new URLRequest("http://" + proxy_server + "/googlemaps_proxy/cache_hybrid.php?mode=tile&t=" + current_location);			
			photoLoader_hybrid.unload();
			photoLoader_hybrid.load(request);	
		}
		
		private function arrange(event:Event = null):void {
			photoLoader.x = -photoLoader.width/2;
			photoLoader.y = -photoLoader.height/2;			
			photoLoader.scaleX = 1.0;
			photoLoader.scaleY = 1.0;			
												
			photoLoader_hybrid.x = photoLoader.x;
			photoLoader_hybrid.y = photoLoader.y;
			photoLoader_hybrid.scaleX = 1.0;
			photoLoader_hybrid.scaleY = 1.0;
			
			border.x = photoLoader.x-border_size;
			border.y = photoLoader.y-border_size;

			border.graphics.beginFill(0xffffff, 1.0);					
			border.graphics.drawRect(0, 0, photoLoader.width + (border_size * 2), photoLoader.height + (border_size * 2));
			border.graphics.endFill();
			
			clickgrabber.scaleX = photoLoader.width;
			clickgrabber.scaleY = photoLoader.height;			
			clickgrabber.x = -photoLoader.width/2;
			clickgrabber.y = -photoLoader.height/2;			
			
			upperleft.x = clickgrabber.x;
			upperleft.y = clickgrabber.y;			
			upperleft.width = clickgrabber.width/4;
			upperleft.height = clickgrabber.height/4;
			
			bottomleft.x = clickgrabber.x;
			bottomleft.y = (clickgrabber.y/4) * -2;			
			bottomleft.width = clickgrabber.width/4;
			bottomleft.height = clickgrabber.height/4;
			
			upperright.x = (clickgrabber.x/4) * -2;	
			upperright.y = clickgrabber.y;			
			upperright.width = clickgrabber.width/4;
			upperright.height = clickgrabber.height/4;
			
			bottomright.x = (clickgrabber.x/4) * -2;	
			bottomright.y = (clickgrabber.y/4) * -2;		
			bottomright.width = clickgrabber.width/4;
			bottomright.height = clickgrabber.height/4;
			
			goback.x = (photoLoader.width/8) * -1;	
			goback.y = (photoLoader.height/8) * -1;	
			goback.width = clickgrabber.width/4;
			goback.height = clickgrabber.height/4;
			
			move_right.x = (clickgrabber.x/4) * -2;
			move_right.y = (photoLoader.height/8) * -1;	
			move_right.width = clickgrabber.width/4;
			move_right.height = clickgrabber.height/4;
			
			move_left.x = (clickgrabber.x/4) * 4;
			move_left.y = (photoLoader.height/8) * -1;	
			move_left.width = clickgrabber.width/4;
			move_left.height = clickgrabber.height/4;
			
			move_up.x = (photoLoader.width/8) * -1;	
			move_up.y = (photoLoader.height/2) * -1;	
			move_up.width = clickgrabber.width/4;
			move_up.height = clickgrabber.height/4;
			
			move_down.x = (photoLoader.width/8) * -1;
			move_down.y = (clickgrabber.y/4) * -2;	
			move_down.width = clickgrabber.width/4;
			move_down.height = clickgrabber.height/4;
			
			//this.scaleX = (Math.random()*0.4) + 0.3;
//			this.scaleY = this.scaleX;
//			this.rotation = Math.random()*180 - 90;
		}
		
		private function str_to_int(str:String):String {
			
		     // Convert string
			var pat_q:RegExp = /q/g;  				  
			var pat_r:RegExp = /r/g;  				  
			var pat_t:RegExp = /t/g;  				  
			var pat_s:RegExp = /s/g;																
			str = str.replace(pat_q, "0");  
			str = str.replace(pat_r, "1");  
			str = str.replace(pat_t, "2");  
			str = str.replace(pat_s, "3"); 		
			
			return str;
        }
		
		private function conv_to_bin(conv_string:String):void {
			size = conv_string.length;
			H = "";
			V = "";			
			
			for(var i:int=0; i < size; i++)
			{				
				if(conv_string.charAt(i) == 0) // q
				{
					H = H + "0";
					V = V + "0";
				}
				else if(conv_string.charAt(i) == 1) // r
				{
					H = H + "1";
					V = V + "0";
				}
				else if(conv_string.charAt(i) == 2) // t
				{
					H = H + "0";
					V = V + "1";
				}
				else if(conv_string.charAt(i) == 3) // s
				{
					H = H + "1";
					V = V + "1";
				}						
				else
				{				
				}				
			}
		}
		
		private function conv_to_gm(conv_string:String):String {
			size = conv_string.length;
			var result1:String = "";
			
			for(var i:int=0; i < size; i++)
			{
				if(H.charAt(i) == "0" && V.charAt(i) == "0") // q
				{
					result1 = result1 + "q";
				}
				else if(H.charAt(i) == "1" && V.charAt(i) == "0") // r
				{
					result1 = result1 + "r";
				}
				else if(H.charAt(i) == "0" && V.charAt(i) == "1") // t
				{
					result1 = result1 + "t";
				}
				else if(H.charAt(i) == "1" && V.charAt(i) == "1") // s
				{
					result1 = result1 + "s";
				}						
				else
				{				
				}
			}
			
			return result1;		
		}
	
		private function binToDec(bin:String):int {
    		return parseInt(bin, 2);
    	}
		
		private function decToBin(dec:int):String
	    {
    		return dec.toString(2);
	    }
	
		public override function released(dx:Number, dy:Number, dang:Number) {
			velX = dx;
			velY = dy;

			velAng = dang;
		}
		
		private function slide(e:Event) {
			if (this.state == "none") {
				if (Math.abs(velX) < 0.001) {
					velX = 0;
				} else {
					x += velX;
					velX *= friction;
				}
				if (Math.abs(velY) < 0.001) {
					velY = 0;
				} else {
					y += velY;
					velY *= friction;
				}
				if (Math.abs(velAng) < 0.001) {
					velAng = 0;
				} else {
					velAng *= angFriction;
					this.rotation += velAng;
				}
			}
		}
	}
}
