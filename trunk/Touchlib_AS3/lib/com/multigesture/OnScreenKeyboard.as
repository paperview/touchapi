/* 
Class/Package name: On Screen Keyboard
Description: Adds a keyboard to the multitouch desktop
Created by Laurence Muller (aka Falcon4ever)
E-mail: info@falcon4ever.com
Site: www.multigesture.net
License: please check the readme.txt

Usage example:
	var myKeyboard:OnScreenKeyboard = new OnScreenKeyboard();	// Make a new OnScreenKeyboard object
	thestage.addChild(myKeyboard);								// Add it to the stage
			
	var paper:NoteBlock = new NoteBlock();						// Make a new NoteBlock object
	thestage.addChild(paper);									// Add it to the stage
	myKeyboard.connect_to_textfield(paper.virtual_noteblock_text);	// Connect the keyboard to the textfield of NoteBlock
*/

package com.multigesture {
	import flash.display.*;
	import flash.events.*;
	import flash.net.*;
	import com.touchlib.*;		
	import app.RotatableScalable;	
	import flash.text.*;
	
	class OnScreenKeyboard extends RotatableScalable {
		// Items
		private var clickgrabber:Shape = new Shape();
		private var keyboard:Sprite = new Sprite();				
		private var keyboard_buttons:Array;
		private var keyboard_buttons_text:Array;		
		private var keyboard_buttons_text_special:Array;		
		private var key:Array;		
		private var shift:Boolean;
		private var capslock:Boolean;
		
		// Default values
		private var velX:Number = 0.0;
		private var velY:Number = 0.0;
		private var velAng:Number = 0.0;
		private var friction:Number = 0.85;
		private var angFriction:Number = 0.92;
		
		// Variable coupled to the noteblock
		private var virtual_noteblock_textfield:TextField;
		
		public function OnScreenKeyboard() {
			// I dont like my keyboard getting lost ;)...
			//this.x = 1600 * Math.random() - 800;
			//this.y = 1600 * Math.random() - 800;
			
			// Reset to default states
			shift = false;
			capslock = false;
						
			// Enumerate keys
			var key = new Array();
		// 1st  row
			key[0] = "`";
			key[1] = "1";
			key[2] = "2";
			key[3] = "3";
			key[4] = "4";
			key[5] = "5";
			key[6] = "6";
			key[7] = "7";
			key[8] = "8";
			key[9] = "9";
			key[10] = "0";
			key[11] = "-";
			key[12] = "=";
			key[13] = "Backspace";			
		// 2nd row
			key[14] = "Tab";
			key[15] = "Q";
			key[16] = "W";
			key[17] = "E";
			key[18] = "R";
			key[19] = "T";
			key[20] = "Y";
			key[21] = "U";
			key[22] = "I";
			key[23] = "O";
			key[24] = "P";
			key[25] = "[";
			key[26] = "]";
			key[27] = "\\";
		// 3rd row
			key[28] = "Capslock";
			key[29] = "A";
			key[30] = "S";
			key[31] = "D";
			key[32] = "F";
			key[33] = "G";
			key[34] = "H";
			key[35] = "J";
			key[36] = "K";
			key[37] = "L";
			key[38] = ";";
			key[39] = "'";
			key[40] = "Enter";			
		// 4th row
			key[41] = "Shift";
			key[42] = "Z";
			key[43] = "X";
			key[44] = "C";
			key[45] = "V";
			key[46] = "B";
			key[47] = "N";
			key[48] = "M";
			key[49] = ",";
			key[50] = ".";
			key[51] = "/";
			key[52] = "Shift_up";			
		// 5th row
			key[53] = "Space";
		
		// Shift buttons (original number + 100)
			key[100] = "~";
			key[101] = "!";
			key[102] = "@";
			key[103] = "#";
			key[104] = "$";
			key[105] = "%";
			key[106] = "^";
			key[107] = "&";
			key[108] = "*";
			key[109] = "(";
			key[110] = ")";
			key[111] = "_";
			key[112] = "+";
			
			key[125] = "{";
			key[126] = "}";
			key[127] = "|";
			
			key[138] = ":";
			key[139] = "\"";
			
			key[149] = "<";
			key[150] = ">";
			key[151] = "?";
		
			// Keyboard border/background
			keyboard.graphics.lineStyle(2, 0xFFFFFF);
			keyboard.graphics.beginFill(0xFFFFFF, 0.35);
			keyboard.graphics.drawRoundRect(0, 0, 505, 160, 15);
			keyboard.graphics.endFill();
	
			keyboard.x = -keyboard.width/2;
			keyboard.y = -keyboard.height/2;
			keyboard.scaleX = 1.0;
			keyboard.scaleY = 1.0;
				
		//////////////////////////////////////////////				
		// Insane code for the buttons starts here! //
		// It ugly but working ...                  //
		//////////////////////////////////////////////	
		
			// Need to set offset of upperleft corner...
			//  keyboard.x and keyboard.y
		
			// Embedded font / Rotating text fix
			// http://www.kirupa.com/forum/showthread.php?t=262770
		//	var linkedFont:ArialTest = new ArialTest();
		//	var fmt:TextFormat = new TextFormat(linkedFont.fontName);
		var fmt:TextFormat = new TextFormat("_sans", 10, 0xFFFFFF);
			keyboard_buttons = new Array();	
			keyboard_buttons_text = new Array();
			keyboard_buttons_text_special = new Array();
			
			// Square == physical button
			// char == primary character
			// char2 == character used with shift
			
			// 1st row			
			for(var i:int=0; i<13; i++)
			{
				var square:Sprite = new Sprite();				
				square.graphics.lineStyle( 1, 0xFFFFFF );
				square.graphics.beginFill( 0xFFFFFF, 0.6 );
				square.graphics.drawRect( keyboard.x + 25 + (i * 30), keyboard.y + 10, 20, 20);
				square.graphics.endFill();				
				keyboard_buttons.push(square);	
								
				var char:TextField = new TextField();
				char.x = keyboard.x + 25 + (i * 30);
				char.y = keyboard.y + 10;			
				char.height = 20;
				char.width = 20;
				char.defaultTextFormat = fmt;
				char.embedFonts = true;				
				char.selectable = false;
		        char.text = " " + key[i];				
				keyboard_buttons_text.push(char);					
				
				var char2:TextField = new TextField();
				char2.textColor = 0x000000;
				char2.x = keyboard.x +  27 + (i * 30);
				char2.y = keyboard.y +  7;
				char2.height = 20;
				char2.width = 20;
				char2.defaultTextFormat = fmt;
				char2.embedFonts = true;				
				char2.selectable = false;
		        char2.text = "  " + key[i+100];				
				keyboard_buttons_text_special.push(char2);				
			}
			
			var square:Sprite = new Sprite();				
			square.graphics.lineStyle( 1, 0xFFFFFF );
			square.graphics.beginFill( 0xFFFFFF, 0.6 );
			square.graphics.drawRect(keyboard.x + 25 + (13 * 30), keyboard.y + 10, 65, 20);
			square.graphics.endFill();				
			keyboard_buttons.push(square);
			
			var char:TextField = new TextField();
			char.x = keyboard.x +  25 + (13 * 30);
		 	char.y = keyboard.y +  10;			
			char.height = 20;
			char.width = 65;
			char.defaultTextFormat = fmt;
			char.embedFonts = true;				
			char.selectable = false;
		    char.text = "  " + key[13];				
			keyboard_buttons_text.push(char);
			
			// 2nd 
			var square:Sprite = new Sprite();				
			square.graphics.lineStyle( 1, 0xFFFFFF );
			square.graphics.beginFill( 0xFFFFFF, 0.6 );
			square.graphics.drawRect(keyboard.x + 25 + (0 * 30),keyboard.y + 40, 35, 20);
			square.graphics.endFill();				
			keyboard_buttons.push(square);
			
			var char:TextField = new TextField();
			char.x =keyboard.x + 25 + (0 * 30);
		 	char.y =keyboard.y + 40;			
			char.height = 20;
			char.width = 35;
			char.defaultTextFormat = fmt;
			char.embedFonts = true;				
			char.selectable = false;			
		    char.text = " " + key[14];				
			keyboard_buttons_text.push(char);
			
			for(var i:int=0; i<12; i++)
			{
				var square:Sprite = new Sprite();				
				square.graphics.lineStyle( 1, 0xFFFFFF );
				square.graphics.beginFill( 0xFFFFFF, 0.6 );
				square.graphics.drawRect(keyboard.x + 70 + (i * 30),keyboard.y + 40, 20, 20);
				square.graphics.endFill();				
				keyboard_buttons.push(square);	
								
				var char:TextField = new TextField();
				char.x =keyboard.x + 70 + (i * 30);
				char.y =keyboard.y + 40;			
				char.height = 20;
				char.width = 20;
				char.defaultTextFormat = fmt;
				char.embedFonts = true;				
				char.selectable = false;
		        char.text = " " + key[i+15];				
				keyboard_buttons_text.push(char);					
				
				if(i > 9)
				{					
					var char2:TextField = new TextField();
					char2.textColor = 0x000000;
					char2.x =keyboard.x + 70 + (i * 30);
					char2.y =keyboard.y + 40;
					char2.height = 20;
					char2.width = 20;
					char2.defaultTextFormat = fmt;
					char2.embedFonts = true;				
					char2.selectable = false;
					char2.text = "   " + key[i+115];				
					keyboard_buttons_text_special.push(char2);
				}
			}
			var square:Sprite = new Sprite();				
			square.graphics.lineStyle( 1, 0xFFFFFF );
			square.graphics.beginFill( 0xFFFFFF, 0.6 );
			square.graphics.drawRect(keyboard.x + 70 + (12 * 30),keyboard.y + 40, 50, 20);
			square.graphics.endFill();				
			keyboard_buttons.push(square);	
							
			var char:TextField = new TextField();
			char.x =keyboard.x +  70 + (12 * 30);
			char.y =keyboard.y +  40;			
			char.height = 20;
			char.width = 50;
			char.defaultTextFormat = fmt;
				char.embedFonts = true;				
				char.selectable = false;
			char.text = "     " + key[12+15];				
			keyboard_buttons_text.push(char);		
				
			var char2:TextField = new TextField();
			char2.textColor = 0x000000;
			char2.x =keyboard.x +  70 + (12 * 30);
			char2.y =keyboard.y +  40;
			char2.height = 20;
			char2.width = 20;
			char2.defaultTextFormat = fmt;
			char2.embedFonts = true;				
			char2.selectable = false;
			char2.text = "   " + key[12+115];				
			keyboard_buttons_text_special.push(char2);
				
			// 3rd 
			var square:Sprite = new Sprite();				
			square.graphics.lineStyle( 1, 0xFFFFFF );
			square.graphics.beginFill( 0xFFFFFF, 0.6 );
			square.graphics.drawRect(keyboard.x +  25 + (0 * 30),keyboard.y +  70, 50, 20);
			square.graphics.endFill();				
			keyboard_buttons.push(square);
			
			var char:TextField = new TextField();
			char.x =keyboard.x +  25 + (0 * 30);
		 	char.y =keyboard.y +  70;			
			char.height = 20;
			char.width = 70;
			char.defaultTextFormat = fmt;
				char.embedFonts = true;				
				char.selectable = false;
		    char.text = key[28];				
			keyboard_buttons_text.push(char);
			
			for(var i:int=0; i<11; i++)
			{
				var square:Sprite = new Sprite();				
				square.graphics.lineStyle( 1, 0xFFFFFF );
				square.graphics.beginFill( 0xFFFFFF, 0.6 );
				square.graphics.drawRect(keyboard.x +  85 + (i * 30),keyboard.y +  70, 20, 20);
				square.graphics.endFill();				
				keyboard_buttons.push(square);	
								
				var char:TextField = new TextField();
				char.x =keyboard.x + 85 + (i * 30);
				char.y =keyboard.y + 70;	
				char.height = 20;
				char.width = 20;
				char.defaultTextFormat = fmt;
				char.embedFonts = true;				
				char.selectable = false;
		        char.text = " " + key[i+29];				
				keyboard_buttons_text.push(char);					
				
				if(i > 8)
				{					
					var char2:TextField = new TextField();
					char2.textColor = 0x000000;
					char2.x =keyboard.x + 85 + (i * 30);
					char2.y =keyboard.y + 70;
					char2.height = 20;
					char2.width = 20;
					char2.defaultTextFormat = fmt;
					char2.embedFonts = true;				
					char2.selectable = false;
					char2.text = "   " + key[i+129];				
					keyboard_buttons_text_special.push(char2);
				}
			}
			
			var square:Sprite = new Sprite();				
			square.graphics.lineStyle( 1, 0xFFFFFF );
			square.graphics.beginFill( 0xFFFFFF, 0.6 );
			square.graphics.drawRect(keyboard.x +  85 + (11 * 30),keyboard.y +  70, 65, 20);
			square.graphics.endFill();				
			keyboard_buttons.push(square);
			
			var char:TextField = new TextField();
			char.x =keyboard.x +  85 + (11 * 30);
		 	char.y =keyboard.y +  70;			
			char.height = 20;
			char.width = 65;
			char.defaultTextFormat = fmt;
				char.embedFonts = true;				
				char.selectable = false;
		    char.text = "      " + key[40];				
			keyboard_buttons_text.push(char);
			
			// 4th
			var square:Sprite = new Sprite();				
			square.graphics.lineStyle( 1, 0xFFFFFF );
			square.graphics.beginFill( 0xFFFFFF, 0.6 );
			square.graphics.drawRect(keyboard.x +  25 + (0 * 30),keyboard.y +  100, 60, 20);
			square.graphics.endFill();				
			keyboard_buttons.push(square);
			
			var char:TextField = new TextField();
			char.x =keyboard.x +  25 + (0 * 30);
		 	char.y =keyboard.y +  100;			
			char.height = 20;
			char.width = 60;
			char.defaultTextFormat = fmt;
				char.embedFonts = true;				
				char.selectable = false;
		    char.text = "     " + key[41];				
			keyboard_buttons_text.push(char);
			
			for(var i:int=0; i<10; i++)
			{
				var square:Sprite = new Sprite();				
				square.graphics.lineStyle( 1, 0xFFFFFF );
				square.graphics.beginFill( 0xFFFFFF, 0.6 );
				square.graphics.drawRect(keyboard.x +  100 + (i * 30),keyboard.y +  100, 20, 20);
				square.graphics.endFill();				
				keyboard_buttons.push(square);	
								
				var char:TextField = new TextField();
				char.x =keyboard.x +  100 + (i * 30);
				char.y =keyboard.y +  100;			
				char.height = 20;
				char.width = 20;
				char.defaultTextFormat = fmt;
				char.embedFonts = true;				
				char.selectable = false;
		        char.text = " " + key[i+42];				
				keyboard_buttons_text.push(char);			
				
				if(i > 6)
				{					
					var char2:TextField = new TextField();
					char2.textColor = 0x000000;
					char2.x =keyboard.x +  100 + (i * 30);
					char2.y =keyboard.y +  100;
					char2.height = 20;
					char2.width = 20;
					char2.defaultTextFormat = fmt;
					char2.embedFonts = true;				
					char2.selectable = false;
					char2.text = "   " + key[i+142];				
					keyboard_buttons_text_special.push(char2);
				}
			}
			
			var square:Sprite = new Sprite();				
			square.graphics.lineStyle( 1, 0xFFFFFF );
			square.graphics.beginFill( 0xFFFFFF, 0.6 );
			square.graphics.drawRect(keyboard.x +  40 + (12 * 30),keyboard.y +  100, 80, 20);
			square.graphics.endFill();				
			keyboard_buttons.push(square);
			
			var char:TextField = new TextField();
			char.x =keyboard.x +  40 + (12 * 30);
		 	char.y =keyboard.y +  100;			
			char.height = 20;
			char.width = 80;
			char.defaultTextFormat = fmt;
				char.embedFonts = true;				
				char.selectable = false;
		    char.text = "         " + key[52];				
			keyboard_buttons_text.push(char);
			
			// 5th
			var square:Sprite = new Sprite();				
			square.graphics.lineStyle( 1, 0xFFFFFF );
			square.graphics.beginFill( 0xFFFFFF, 0.6 );
			square.graphics.drawRect(keyboard.x +  120,keyboard.y +  130, 250, 20);
			square.graphics.endFill();				
			keyboard_buttons.push(square);
			
			var char:TextField = new TextField();
			char.x =keyboard.x +  120;
		 	char.y =keyboard.y +  130;			
			char.height = 20;
			char.width = 250;
			char.defaultTextFormat = fmt;
				char.embedFonts = true;				
				char.selectable = false;
		    char.text = "                                  " + key[53];				
			keyboard_buttons_text.push(char);
			
		/////////////////////////////////////////////			
		// Insane code for the buttons stops here! //						
		/////////////////////////////////////////////
						
			clickgrabber.scaleX = keyboard.width;
			clickgrabber.scaleY = keyboard.height;
			clickgrabber.x = -keyboard.width/2;
			clickgrabber.y = -keyboard.height/2;
			
		/*
			// Disabled since small keyboards are useless ;)...
			this.scaleX = (Math.random()*0.4) + 0.3;
			this.scaleY = this.scaleX;
			this.rotation = Math.random()*180 - 90;
		*/

			clickgrabber.graphics.beginFill(0xff00ff, 0.0);
			clickgrabber.graphics.drawRect(0, 0, 1, 1);
			clickgrabber.graphics.endFill();

			addChild(keyboard);
			addChild(clickgrabber);
			//this.addEventListener(Event.ENTER_FRAME, slide);

			// Add special button text (gray)
			for(var btn_sp:int=0; btn_sp < keyboard_buttons_text_special.length; btn_sp++)
			{
				addChild(keyboard_buttons_text_special[btn_sp]);
			}
			
			// Add button and button text
			for(var btn:int=0; btn < keyboard_buttons.length; btn++)
			{				
				addChild(keyboard_buttons[btn]);
				addChild(keyboard_buttons_text[btn]);
				keyboard_buttons[btn].addEventListener(TUIOEvent.UpEvent, addArguments(handlePressUp, [key[btn]]));
				keyboard_buttons[btn].addEventListener(TUIOEvent.DownEvent, addArguments(handlePressDown, [key[btn]]));
				keyboard_buttons_text[btn].addEventListener(TUIOEvent.UpEvent, addArguments(handlePressUp, [key[btn]]));		
				keyboard_buttons_text[btn].addEventListener(TUIOEvent.DownEvent, addArguments(handlePressDown, [key[btn]]));		

/*
				// Enable this for testing with a mouse
				keyboard_buttons[btn].addEventListener(MouseEvent.MOUSE_UP, addArguments(handlePressUp, [key[btn]]));
				keyboard_buttons[btn].addEventListener(MouseEvent.MOUSE_DOWN, addArguments(handlePressDown, [key[btn]]));
				keyboard_buttons_text[btn].addEventListener(MouseEvent.MOUSE_UP, addArguments(handlePressUp, [key[btn]]));
				keyboard_buttons_text[btn].addEventListener(MouseEvent.MOUSE_DOWN, addArguments(handlePressDown, [key[btn]]));		
*/		
			
			}
					
			this.x = 1024/2;
			this.y = (768/4)*3;
		}
		
		// Connect the onscreenkeyboard to a textfield in flash. 
		public function connect_to_textfield(e:flash.text.TextField){
			virtual_noteblock_textfield = e;
		}
		
		// Wrapper to pass values to an event.
		public function addArguments(method:Function, additionalArguments:Array):Function {
			return function(event:TUIOEvent):void {
				method.apply(null, [event].concat(additionalArguments));
			}
		}

		public function handlePressDown(event:TUIOEvent, button:String):void {					
			if(button == "Capslock") {
				if(capslock == true){
					capslock = false;
				} 
				else{
					capslock = true;
				}
			} else if(button == "Shift") {
				shift = true;
			} else if(button == "Shift_up") {	// Fix for touchlib | release works fine on tuio simulator
				shift = false;
			} else if(button == "Space") {
				virtual_noteblock_textfield.appendText(" ");	
			} else if(button == "Enter") {
				virtual_noteblock_textfield.appendText("\n");
			} else if(button == "Tab") {
				virtual_noteblock_textfield.appendText("\t");
			} else if(button == "Backspace") {
				virtual_noteblock_textfield.text = virtual_noteblock_textfield.text.substring(0, virtual_noteblock_textfield.text.length-1);

			// custom ones
			} else if(button == "`") {
				if(shift == true) {
					virtual_noteblock_textfield.appendText("~");
				} else {
					virtual_noteblock_textfield.appendText("`");
				}
			} else if(button == "1") {
				if(shift == true) {
					virtual_noteblock_textfield.appendText("!");
				} else {
					virtual_noteblock_textfield.appendText("1");
				}				
			} else if(button == "2") {
				if(shift == true) {
					virtual_noteblock_textfield.appendText("@");
				} else {
					virtual_noteblock_textfield.appendText("2");
				}	
			} else if(button == "3") {
				if(shift == true) {
					virtual_noteblock_textfield.appendText("#");
				} else {
					virtual_noteblock_textfield.appendText("3");
				}	
			} else if(button == "4") {
				if(shift == true) {
					virtual_noteblock_textfield.appendText("$");
				} else {
					virtual_noteblock_textfield.appendText("4");
				}	
			} else if(button == "5") {
				if(shift == true) {
					virtual_noteblock_textfield.appendText("%");
				} else {
					virtual_noteblock_textfield.appendText("5");
				}	
			} else if(button == "6") {
				if(shift == true) {
					virtual_noteblock_textfield.appendText("^");
				} else {
					virtual_noteblock_textfield.appendText("6");
				}	
			} else if(button == "7") {
				if(shift == true) {
					virtual_noteblock_textfield.appendText("&");
				} else {
					virtual_noteblock_textfield.appendText("7");
				}	
			} else if(button == "8") {
				if(shift == true) {
					virtual_noteblock_textfield.appendText("*");
				} else {
					virtual_noteblock_textfield.appendText("8");
				}	
			} else if(button == "9") {
				if(shift == true) {
					virtual_noteblock_textfield.appendText("(");
				} else {
					virtual_noteblock_textfield.appendText("9");
				}	
			} else if(button == "0") {
				if(shift == true) {
					virtual_noteblock_textfield.appendText(")");
				} else {
					virtual_noteblock_textfield.appendText("0");
				}	
			} else if(button == "-") {
				if(shift == true) {
					virtual_noteblock_textfield.appendText("_");
				} else {
					virtual_noteblock_textfield.appendText("-");
				}	
			} else if(button == "=") {
				if(shift == true) {
					virtual_noteblock_textfield.appendText("+");
				} else {
					virtual_noteblock_textfield.appendText("=");
				}					

			} else if(button == "[") {
				if(shift == true) {
					virtual_noteblock_textfield.appendText("{");
				} else {
					virtual_noteblock_textfield.appendText("[");
				}		
			} else if(button == "]") {
				if(shift == true) {
					virtual_noteblock_textfield.appendText("}");
				} else {
					virtual_noteblock_textfield.appendText("]");
				}		
			} else if(button == "\\") {
				if(shift == true) {
					virtual_noteblock_textfield.appendText("|");
				} else {
					virtual_noteblock_textfield.appendText("\\");
				}		
			} else if(button == ";") {
				if(shift == true) {
					virtual_noteblock_textfield.appendText(":");
				} else {
					virtual_noteblock_textfield.appendText(";");
				}		
			} else if(button == "'") {
				if(shift == true) {
					virtual_noteblock_textfield.appendText("\"");
				} else {
					virtual_noteblock_textfield.appendText("'");
				}		
			} else if(button == ",") {
				if(shift == true) {
					virtual_noteblock_textfield.appendText("<");
				} else {
					virtual_noteblock_textfield.appendText(",");
				}	
			} else if(button == ".") {
				if(shift == true) {
					virtual_noteblock_textfield.appendText(">");
				} else {
					virtual_noteblock_textfield.appendText(".");
				}	
			} else if(button == "/") {
				if(shift == true) {
					virtual_noteblock_textfield.appendText("?");
				} else {
					virtual_noteblock_textfield.appendText("/");
				}					
				
			// alphabet
			} else {
				if(capslock == true || shift == true) {
					virtual_noteblock_textfield.appendText(button.toUpperCase());
				} else {
					virtual_noteblock_textfield.appendText(button.toLowerCase());					
				}
			}		
		}
		
		public function handlePressUp(event:TUIOEvent, button:String):void {
			// Release key...
			if(button == "Shift"){
				shift = false;
			}			
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