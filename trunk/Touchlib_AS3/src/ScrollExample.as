package {
   // import com.touchlib.*;
	//import app.*;
	//import flash.system.Capabilities;

    import flash.display.MovieClip;
    import flash.display.Sprite;
    import flash.display.StageAlign;
    import flash.display.StageScaleMode;
    import flash.events.Event;
    import com.onebyonedesign.utils.OBO_FullScrollBar;
    import com.onebyonedesign.utils.events.ScrollBarEvent;
    
    public class ScrollExample extends Sprite {
        
        public var _scrollContent:ScrollContent;
        private var _myScrollBar:OBO_FullScrollBar;
  		//public var _wrapper:TouchlibWrapper;
		
        public function ScrollExample() {
            //initStage();
            _scrollContent = new ScrollContent();
            _scrollContent.x = 1680-500;
            addChild(_scrollContent);
            
            _myScrollBar = OBO_FullScrollBar.createScrollBar(0x000000, 0x999999, _scrollContent, 5, OBO_FullScrollBar.RIGHT, true);
            _myScrollBar.addEventListener(ScrollBarEvent.SCROLL_DOWN, downListener);
            _myScrollBar.addEventListener(ScrollBarEvent.SCROLL_UP, upListener);
            addChild(_myScrollBar);
			
			//var _wrapper = new TouchlibWrapper(_myScrollBar);
			//addChild(_wrapper);
            
            stageListener();
			
			
        }
        
        private function downListener(evt:ScrollBarEvent):void {
            _myScrollBar.scrollerColor = 0x993333;
        }
        
        private function upListener(evt:ScrollBarEvent):void {
            _myScrollBar.scrollerColor = 0x999999;
        }
   /*     
        private function initStage() {
			TUIO.init( this, 'localhost', 3000, Capabilities.screenResolutionX, Capabilities.screenResolutionY, '', true );
            stage.frameRate = 120;
            stage.showDefaultContextMenu = false;
            stage.scaleMode = StageScaleMode.NO_SCALE;
            stage.align = StageAlign.TOP_LEFT;
            stage.addEventListener(Event.RESIZE, stageListener);			
        }
     */   
        private function stageListener(e:Event = null) {
          //  _scrollContent.x = stage.stageWidth / 2 - _scrollContent.width / 2;
            _myScrollBar.adjustSize();
        }
    }
}