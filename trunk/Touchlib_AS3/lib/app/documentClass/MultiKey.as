//////////////////////////////////////////////////////////////////////
//                                                                  //
//    Main Document Class. Sets TUIO and adds main parts to stage   //
////
//////////////////////////////////////////////////////////////////////

package app.documentClass {

	import flash.display.Sprite;
	import flash.system.Capabilities;
	import whitenoise.*;
	import app.createKeyboard.*;
	
	public class MultiKey extends Sprite {
		
		private var octaves1 = 1;   //how many octaves on first keyboard
		private var octaves2 = 2;   //how many octaves on second keyboard
		//private var octaves3 = 1; //how many octaves on third keyboard
		//private var octaves4 = 2; //how many octaves on fourth keyboard
		

		public function MultiKey() {

			TUIO.init( this, 'localhost', 3000, Capabilities.screenResolutionX, Capabilities.screenResolutionY, '', true );
			
			//first keyboard							
			var fullPiano = new AddPiano(octaves1);
			addChild(fullPiano);			
			fullPiano.x = fullPiano.width/3.6;
			fullPiano.y = fullPiano.height/2;			
			fullPiano.scaleX = .32;
			fullPiano.scaleY = .32;
			fullPiano.rotation = 60;				
			
			//second keyboard	
			var fullPiano2 = new AddPiano(octaves2);
			addChild(fullPiano2);			
			
			if (octaves2 == 1) {
				fullPiano2.scaleX = .8;
				fullPiano2.scaleY = .8;
				fullPiano2.x = fullPiano2.width/1.6;
				fullPiano2.y = fullPiano2.height/1.6;			
			}			
			if (octaves2 == 2){
				fullPiano2.x = fullPiano2.width/3.6;
				fullPiano2.y = fullPiano2.height/2;			
				fullPiano2.scaleX = .42;
				fullPiano2.scaleY = .42;
			}

/*          //Keyboard surface. Needs to be fixed. Keyboards shouldn't move clicking on them (unless highlighted).
			var pianoSurface = new PianoSurface();
			this.addChild(pianoSurface);
			pianoSurface.addChild(fullPiano);		
			pianoSurface.addChild(fullPiano2);
			setChildIndex(pianoSurface, 2) 			
*/
			
		}
	}
}