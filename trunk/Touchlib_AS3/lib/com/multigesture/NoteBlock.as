/* 
Class/Package name: NoteBlock
Description: Adds a Post-it Note like virtual paper to the multitouch desktop
Created by Laurence Muller (aka Falcon4ever)
E-mail: info@falcon4ever.com
Site: www.multigesture.net
License: please check the readme.txt

Usage example (with the OnScreenKeyboard):
	var myKeyboard:OnScreenKeyboard = new OnScreenKeyboard();	// Make a new OnScreenKeyboard object
	thestage.addChild(myKeyboard);								// Add it to the stage
			
	var paper:NoteBlock = new NoteBlock();						// Make a new NoteBlock object
	thestage.addChild(paper);									// Add it to the stage
	myKeyboard.connect_to_textfield(paper.virtual_noteblock_text);	// Connect the keyboard to the textfield of NoteBlock
*/

package com.multigesture {
	import flash.display.*;
	import flash.events.*;
	import com.touchlib.*;		
	import app.RotatableScalable;	
	import flash.text.*;
	
	class NoteBlock extends RotatableScalable {		
		// Default values
		private var velX:Number = 0.0;
		private var velY:Number = 0.0;
		private var velAng:Number = 0.0;
		private var friction:Number = 0.85;
		private var angFriction:Number = 0.92;

		// Items
		private var clickgrabber:Shape = new Shape();
		private var virtual_noteblock:Sprite = new Sprite();	
		
		// Public Items
		public var virtual_noteblock_text:TextField = new TextField();	// Keep public since this variable needs to be modified
			
		public function NoteBlock() {			
			// Embedded font / Rotating text fix
			// http://www.kirupa.com/forum/showthread.php?t=262770
			//var linkedFont:ArialTest = new ArialTest();
			//var fmt:TextFormat = new TextFormat(linkedFont.fontName);
			var fmt:TextFormat = new TextFormat("_sans", 10, 0xFFFFFF);
																
			// Keyboard border/background
			virtual_noteblock.graphics.lineStyle(1, 0xFFFFFF);
			virtual_noteblock.graphics.beginFill(0xFFFFFF, 0.75);	// Yellow like a post-it note ;)...
			virtual_noteblock.graphics.drawRoundRect(0, 0, 500, 100,0);
			virtual_noteblock.graphics.endFill();
				
			virtual_noteblock.x = -virtual_noteblock.width/2;
			virtual_noteblock.y = -virtual_noteblock.height/2;
			virtual_noteblock.scaleX = 1.0;
			virtual_noteblock.scaleY = 1.0;
			
			// Textbox
			virtual_noteblock_text.x = virtual_noteblock.x + 5;
			virtual_noteblock_text.y = virtual_noteblock.y + 10;			
			virtual_noteblock_text.width = 500;
			virtual_noteblock_text.height = 100;
				
			var format:TextFormat = new TextFormat();
     		format.color = 0x000000;
        	format.size = 60;
        
			virtual_noteblock_text.defaultTextFormat = format;
			virtual_noteblock_text.defaultTextFormat = fmt;
			virtual_noteblock_text.embedFonts = true;
			virtual_noteblock_text.wordWrap = true;
		    virtual_noteblock_text.text = "";			
																	
			// Clickgrabber
			clickgrabber.scaleX = virtual_noteblock.width;
			clickgrabber.scaleY = virtual_noteblock.height;
			clickgrabber.x = -virtual_noteblock.width/2;
			clickgrabber.y = -virtual_noteblock.height/2;
			
			clickgrabber.graphics.beginFill(0xff00ff, 0.0);
			clickgrabber.graphics.drawRect(0, 0, 1, 1);
			clickgrabber.graphics.endFill();						

			addChild(virtual_noteblock);
			addChild(virtual_noteblock_text);
			//addChild(clickgrabber);
			
			//this.addEventListener(Event.ENTER_FRAME, slide);
			
			this.x = 1024/2;
			this.y = (768/4);
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