package {

import com.gfxcomplex.display.BrowserBackground;
import com.touchlib.*;
import com.multigesture.*;
import app.*;
import flash.display.Sprite;
import flash.display.Shape;
import flash.system.Capabilities;
import flash.events.*;
import Settings;
import Arrange; 
import ScrollExample;
import app.documentClass.MultiKey; 
//import pong.Game;

[SWF(width='1024', height='786', backgroundColor='0x000000')]

public class Touchlib_AS3 extends Sprite
{		
	public var sortCanvas0:nCanvas;
	public var sortCanvas1:Arrange;
	public var slider0:TouchlibSlider;
	public var slider1:TouchlibSlider;	
	//private var scrollCanvas0:ScrollExample;
	//private var wong:Game;
	private var temporary_test:MultiKey;
	
	public function Touchlib_AS3()
		{					
		Settings.instance.loadSettings('config.xml');
		Settings.instance.addEventListener(Settings.INIT, initApp);	
		}
		
	public function initApp(event:Event):void
		{	
		if (Settings.instance.debug == true){	
		var DEBUG_FPS:FPS = new FPS();
		var debugMode:Boolean = Settings.instance.debug;
		this.addChild(DEBUG_FPS);
		}
		this.stage.frameRate = Settings.instance.framerate;
		trace(' ~ '); 
        trace(' v '+Settings.instance.version);
        trace('-------------------------------------------------------------------------------------------------------------------------------');
        trace(' Theme : '+Settings.instance.theme); 
        trace(' Debug : '+Settings.instance.debug); 
        trace(' FPS : '+Settings.instance.framerate);		
        TUIO.init( this, Settings.instance.host, Settings.instance.TCP, Capabilities.screenResolutionX, Capabilities.screenResolutionY, '', debugMode);	        
        trace(' TUIO Socket Enabled : Host: '+Settings.instance.host+' TCP: '+Settings.instance.TCP+' UDP: '+Settings.instance.UDP);	
	    trace('-------------------------------------------------------------------------------------------------------------------------------');		
		trace(' Modules Available : '+Settings.instance.modules_avail);
		trace('-------------------------------------------------------------------------------------------------------------------------------');
		trace(' Modules Loaded : '+Settings.instance.modules);	
  		trace('-------------------------------------------------------------------------------------------------------------------------------');
  		setupStage();
  		}
  		
	public function setupStage()
		{			
		var sortCanvas0 = new nCanvas(0,"square",0x000000,0.25, "normal", 10000, 10000, -5000, -5000);
		this.addChildAt(sortCanvas0, 0);	
		
		var nCanvas_obj_4 = new nCanvas(4, "circle",0xFFFFFF,1, "invert", 250, 250, -150, -150);
		//sortCanvas0.addChild(nCanvas_obj_4);	
		
		var sortCanvas1:Arrange = new Arrange();
		sortCanvas1.x = -sortCanvas0.width/8, sortCanvas1.y = -sortCanvas0.height/8;
		sortCanvas0.addChildAt(sortCanvas1,1);	
		
		var scrollCanvas0 = new ScrollExample();
		//this.addChildAt(scrollCanvas0, 1);	
		
	
		
		var temporary_test:MultiKey = new MultiKey();
		//sortCanvas0.addChildAt(temporary_test, 1);	
		
		/*
		var spr4:Shape = new Shape();
		spr4.graphics.beginFill(0x000000,1.0);
		spr4.graphics.drawRect(-0,-0,300,300);
		spr4.graphics.endFill();		
		//spr4.x = photos.width/2-300; spr4.y = -photos.height/2;
		spr4.addEventListener(TUIOEvent.DownEvent, testFunction);	
		sortCanvas1.addChild(spr4);	
		*/
		
		//trace(TUIO.recordedXML.toString());
		
		//var nLoad_obj_0:nLoad = new nLoad(sortCanvas0, 100, "mine", "", 0, 0, 1,"");
		//var sortCanvas1:Arrange = new Arrange();
		//sortCanvas0.addChild(sortCanvas1);	
		
		//var subobj1:PhotoCanvas = new PhotoCanvas();	
		//this.addChild(subobj1);
		
		//var subobj2:PhotoCanvas = new PhotoCanvas();	
		//this.addChild(subobj2);
			
		//var flickrLoader:Flickr = new Flickr(subobj1);
		//var flickrLoader:Flickr = new Flickr(subobj2);
		//var sub_object_01:PhotoLocal = new PhotoLocal(sortCanvas);

		var slider0:TouchlibSlider = new TouchlibSlider(50,210);
		var slider1:TouchlibSlider = new TouchlibSlider(50,210);
		//var knoby:TouchlibKnob = new TouchlibKnob(120);

		//this.addChild(knoby);
		//this.addChild(slider0);	
		//this.addChild(slider1);
		//slider0.x = 110, slider0.y = 35;		
		//slider1.x = 175, slider1.y = 35;		
		//trace(slider0.getValue());
		
		//this.stage.frameRate = slider0.getValue;
		
		//slidy.setValue(0.1);		
		//slider0.addEventListener(TUIOEvent.MoveEvent,scaleCanvas);	
		
		//var naturalPaint:PaintSurface = new PaintSurface(800, 600);
		//sortCanvas0.addChildAt(naturalPaint,0);
		
		var bFullscreen:Shape = new Shape();
		bFullscreen.graphics.beginFill(0x000000,0.0);
		bFullscreen.graphics.drawRect(Capabilities.screenResolutionX-200,0,200,200);
		bFullscreen.graphics.endFill();	
		this.addChild(bFullscreen);  
		
		bFullscreen.addEventListener(TUIOEvent.DownEvent, function() {
		if (stage.displayState == "fullScreen") {stage.displayState = "normal";} 
		else {stage.displayState = "fullScreen";}
		});	
		
		//setChildIndex(subobj,3);
		if (Settings.instance.background != "none"){
		var bg = new BrowserBackground(Settings.instance.background);
		this.addChildAt(bg, 0);	

		bg.addEventListener(BrowserBackground.BACKGROUND_LOADING, loading);
		bg.addEventListener(BrowserBackground.BACKGROUND_LOADED, loaded);

		function loaded(e:Event) {
		bg.removeEventListener(BrowserBackground.BACKGROUND_LOADING, loading);
		bg.removeEventListener(BrowserBackground.BACKGROUND_LOADED, loaded);
		}

		function loading(e:Event) {
		var bL:Number = bg.bytesLoaded;
		var bT:Number = bg.bytesTotal;	
		var bT:Number = bg.bytesTotal;		
		 trace('Loading URL: ' + Settings.instance.background +' - '+ bT / 1000 + 'KB / ' +uint(100 * bL / bT)+'%');
		}	
  		}	
  		}
	public function testFunction(e:TUIOEvent)
		{	
		trace('Test');
		//scrollCanvas0._scrollContent._tf.text = '';
		//scrollCanvas0._scrollContent._tf.text = TUIO.recordedXML.toString();
		//var sliderValue = slider0.getValue;		
		//this.stage.frameRate = sliderValue;
		//trace(sliderValue);		
		}
  }	
}