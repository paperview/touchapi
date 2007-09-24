/* 
Class/Package name: PhotoLocal
Description: Create a multitouch enviroment based on local content
Created by Laurence Muller (aka Falcon4ever)
E-mail: info@falcon4ever.com
Site: www.multigesture.net
License: please check the readme.txt

Usage example:
	photolocalLoader = new PhotoLocal(subobj);
*/

package com.multigesture {
	import flash.display.*;		
	import flash.events.*;
	import flash.net.*;
	import com.touchlib.*;		
	import app.RotatableScalable;	
	
	
	public class PhotoLocal extends MovieClip
	{
		// Class properties
		private var thestage:Sprite;
		private var allPics:Array;			

		// Misc.
		private var LIcontainer:String;	// Images
		private var LVcontainer:String; // Videos
		
		public function PhotoLocal(d:Sprite) 
		{
			thestage = d;
			allPics = new Array();						
			clearPics();			
			
			// Load images
			LocalImageLoader();
			
			// Load videos
			LocalVideoLoader();
			
			// Optional stuff below... (Can be commented out if needed)
			
			// Add Keyboard object
			var myKeyboard:OnScreenKeyboard = new OnScreenKeyboard();			
			//this.addChild(myKeyboard);
			
			// Add NoteBlock object
			//var paper:NoteBlock = new NoteBlock();
			//paper.x = 0;
			//paper.y = -132;
			//myKeyboard.addChild(paper);
			
			//myKeyboard.connect_to_textfield(paper.virtual_noteblock_text);			
			
			// Add GoogleMapsSimple object
			//var gm:GoogleMapsSimple = new GoogleMapsSimple("www.falcon4ever.com");
			//thestage.addChild(gm);
		}
													
		public function LocalImageLoader()
        {
            var request:URLRequest = new URLRequest("www/photos.txt");
            var variables:URLLoader = new URLLoader();
            variables.dataFormat = URLLoaderDataFormat.TEXT;
            variables.addEventListener(Event.COMPLETE, LIcompleteHandler);
            try
            {
                variables.load(request);
            } 
            catch (error:Error)
            {
                trace("Unable to load (images) file " + error);
            }
        }
		
        private function LIcompleteHandler(event:Event):void
        {
            var loader:URLLoader = URLLoader(event.target);
			LIcontainer = loader.data;
			showPics();	// Show pics when done loading textfile
        }													
													
		public function showPics()
		{
			var myArray:Array = new Array();
			myArray = LIcontainer.split("\r\n");
			for(var i:int=0; i<myArray.length; i++)
			{
				myArray[i] = "www/photos/" + myArray[i];							
			}	

			for(i=0; i < myArray.length-1; i++)
			{
				trace('Trying to load :'+(myArray[i]));
				var photo:lmPhoto = new lmPhoto( myArray[i] );				
				thestage.addChild(photo);
				allPics.push(photo);				
			}			
		}
										
		public function LocalVideoLoader()
        {
            var request:URLRequest = new URLRequest("www/youtube.txt");
            var variables:URLLoader = new URLLoader();
            variables.dataFormat = URLLoaderDataFormat.TEXT;
            variables.addEventListener(Event.COMPLETE, LVcompleteHandler);
            try
            {
                variables.load(request);
            } 
            catch (error:Error)
            {
                trace("Unable to load (videos) file " + error);
            }
        }
		
		private function LVcompleteHandler(event:Event):void
        {
            var loader:URLLoader = URLLoader(event.target);
			LVcontainer = loader.data;
			showVids();	// Show videos when done loading textfile
        }			
		
		public function showVids()
		{
			var myArray:Array = new Array();
			myArray = LVcontainer.split("\r\n");
			for(var i:int=0; i<myArray.length; i++)
			{
				myArray[i] = "http://cache.googlevideo.com/get_video?video_id=" + myArray[i];	
				
			}	

			for(i=0; i < myArray.length-1; i++)
			{	
				trace('Trying to load :'+(myArray[i]));
				var flv_video:FLVvideo = new FLVvideo( myArray[i] );				
				thestage.addChild(flv_video);
				allPics.push(flv_video);								
			}			
		}
		
		public function clearPics()
		{
			for(var i:int = 0; i<allPics.length; i++)
			{
				this.removeChild(allPics[i]);
			}
			
			allPics = new Array();
		}

	}
}