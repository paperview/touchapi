//////////////////////////////////////////////////////////////////////
//                                                                  //
//    Main Document Class. Sets TUIO and adds main parts to stage   //
////
//////////////////////////////////////////////////////////////////////

package app.documentClass
{

	import flash.display.Sprite;
	
	import com.touchlib.TUIO;
	import app.PaintSurface;	
	import flash.system.Capabilities;

	public class Paint extends Sprite {

		private var naturalPaint:PaintSurface;

		public function Paint() {
		trace("Paint Initialized");
		TUIO.init( this, 'localhost', 3000, 1024, 786, '', false );
		var naturalPaint = new PaintSurface(1024, 786);
		this.addChild(naturalPaint);
		}
	}
}