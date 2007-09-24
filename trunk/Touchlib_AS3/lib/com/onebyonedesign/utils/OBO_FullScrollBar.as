package com.onebyonedesign.utils {
    
    import flash.geom.ColorTransform;
    import flash.geom.Transform;
    import flash.geom.Rectangle;
    import flash.display.Sprite;
    import flash.display.DisplayObjectContainer;
    import flash.events.MouseEvent;
    import flash.events.Event;
	//import com.touchlib.*;

    import com.onebyonedesign.utils.events.ScrollBarEvent;
    
    public class OBO_FullScrollBar extends Sprite {
        private static var OBO_FSB:OBO_FullScrollBar;
        
        public static const RIGHT:String = "right";
        public static const LEFT:String = "left";
        
        private var _addedToStage:Boolean = false;
        private var _pressed:Boolean = false;
        
        private var _content:DisplayObjectContainer;
        private var _scrollValue:Number;
        private var _easeAmount:int;
        private var _scrollBar:Sprite;
        private var _scroller:Sprite;
        private var _scrollTrack:Sprite;
        private var _trackColor:uint;
        private var _scrollerColor:uint;
        private var _position:String;
        private var _useScrollWheel:Boolean;
   		//private var _wrapper:TouchlibWrapper;
        /**
        * 
        * @author      Devon O. Wolfgang
        * @date   5/26/2007
        * @description  creates a full screen scroll bar for use with full browser flash applications
        * 
        * @param        trackColor      uint          hexadecimal color of scrollbar track
        * @param        scrollerColor      uint               hexadecimal color of scrollbar scroller
        * @param        content    DisplayObjectContainer   the content that will be scrolled - Sprite, MovieClip, or Shape
        * @param        easeAmount      Number                    how much easing applied to scrolled content (1 = no ease, Number.POSITIVE_INFINITY = way too much ease) - default is 1
        * @param        position            String          OBO_FullScrollBar.LEFT or OBO_FullScrollBar.RIGHT - determines which side of browser scrollbar appears on - default is RIGHT
        * @param        useScrollWheel    Boolean               specifies whether or not scrollbar is mouse wheel enabled - default is false
        * 
        * @usage        var myScrollBar:OBO_FullScrollBar = OBO_FullScrollBar.createScrollBar(0x000000, 0x535353, scrollContent, 1, OBO_FullScrollBar.RIGHT, true);
        */

        function OBO_FullScrollBar(creator:CreatorClass, tc:uint, sc:uint, c:DisplayObjectContainer, e:int = 1, pos:String = RIGHT, sw:Boolean = false) {   
            if (pos != "right" && pos != "left") throw new Error("OBO_FullScrollBar error: constructor method position argument requires either \"left\" (OBO_FullScrollBar.LEFT) or \"right\" (OBO_FullScrollBar.RIGHT).");
            
            _trackColor = tc;
            _scrollerColor = sc;
            _content = c;
            _easeAmount = (e > 0) ? Math.ceil(e) : 1;
            _position = pos;
            _useScrollWheel = sw;
            
            initScroller();
        }
        
        private function initScroller():void {
            makeScrollBar();
            addChild(_scrollBar);
            
			//var _wrapper = new TouchlibWrapper(_scrollBar);
			//addChild(_wrapper);
			
            _scrollBar.addEventListener(Event.ADDED_TO_STAGE, addedListener);
            
            _scroller.buttonMode = true;
            _scroller.useHandCursor = false;
        }
        
        private function addedListener(e:Event):void {
            _scrollBar.removeEventListener(Event.ADDED_TO_STAGE, adjustSize);
            _addedToStage = true;
            
            if (_useScrollWheel) stage.addEventListener(MouseEvent.MOUSE_WHEEL, mouseWheelListener);
            _scroller.addEventListener(MouseEvent.MOUSE_DOWN, onDownListener);
            stage.addEventListener(MouseEvent.MOUSE_UP, onUpListener);
            
            adjustSize();
        }
        
        private function onDownListener(me:MouseEvent):void {
            _pressed = true;
            dispatchEvent(new ScrollBarEvent(ScrollBarEvent.SCROLL_DOWN));
            
            _scroller.startDrag(false, new Rectangle(0, 0, 0, stage.stageHeight - _scroller.height));
            stage.addEventListener(MouseEvent.MOUSE_MOVE, mouseMoveListener);
        }
        
        private function onUpListener(me:MouseEvent):void {
            if (_pressed) {
                _pressed = false;
                dispatchEvent(new ScrollBarEvent(ScrollBarEvent.SCROLL_UP));
                
                _scroller.stopDrag();
                stage.removeEventListener(MouseEvent.MOUSE_MOVE, mouseMoveListener);
            }
        }
        
        private function mouseWheelListener(me:MouseEvent):void {
            var d:Number = me.delta;
            if (d > 0) {
                if ((_scroller.y - (d * 4)) >= 0){
                        _scroller.y -= d * 4;
                    } else {
                        _scroller.y = 0;
                    }
                    if (!_scroller.willTrigger(Event.ENTER_FRAME)) _scroller.addEventListener(Event.ENTER_FRAME, scrollContent);
                } else {
                    if (((_scroller.y + _scroller.height) + (Math.abs(d) * 4)) <= stage.stageHeight){
                        _scroller.y += Math.abs(d) * 4;
                    } else {
                        _scroller.y = stage.stageHeight - _scroller.height;
                    }
                    if (!_scroller.willTrigger(Event.ENTER_FRAME)) _scroller.addEventListener(Event.ENTER_FRAME, scrollContent);
                }
        }
        
        private function mouseMoveListener(me:MouseEvent):void {
            me.updateAfterEvent();
            if (!_scroller.willTrigger(Event.ENTER_FRAME)) _scroller.addEventListener(Event.ENTER_FRAME, scrollContent);
        }
        
        private function scrollContent(e:Event):void {
            var ty:Number = -((_content.height - stage.stageHeight) * (_scroller.y / _scrollValue));
            var dist:Number = ty - _content.y;
            var moveAmount:Number = dist / _easeAmount;
            _content.y += moveAmount;
            
            if (Math.abs(_content.y - ty) < .5) {
                _scroller.removeEventListener(Event.ENTER_FRAME, scrollContent);
                _content.y = Math.round(ty);
            }
        }
        
        
        private function makeScrollBar():void {
            _scrollBar = new Sprite();
            _scrollTrack = createTrack();
            _scroller = createScroller();
            _scrollBar.addChild(_scrollTrack);
            _scrollBar.addChild(_scroller);
        }
        
        private function createTrack():Sprite {
            var t:Sprite = new Sprite();
            
            t.graphics.beginFill(_trackColor, 0.5);
            t.graphics.drawRect(0, 0, 16, 16);
            t.graphics.endFill();
        
            return t;
        }
        
        private function createScroller():Sprite {
            var s:Sprite = new Sprite();

            s.graphics.beginFill(_scrollerColor, 0.5);
            s.graphics.drawRect(0, 0, 500, 16);
            s.graphics.endFill();

            return s;
        }
        
        /*
        * 
        *   public methods
        * 
        */  
        
        // singleton instantiation
        public static function createScrollBar(trackColor:uint, scrollerColor:uint, content:DisplayObjectContainer, easeAmount:int = 1, position:String = RIGHT, useMouseWheel:Boolean = false):OBO_FullScrollBar {
            if (OBO_FSB == null) OBO_FSB = new OBO_FullScrollBar(new CreatorClass(), trackColor, scrollerColor, content, easeAmount, position, useMouseWheel);
            return OBO_FSB;
        }
        
        
        // Adusts and repositions scrollbar. Call this when stage or content object is resized.
        
        public function adjustSize():void {
            if (_addedToStage) {
                _scrollBar.x = (_position == LEFT) ? 0 : stage.stageWidth - 500;
                _scrollTrack.height = stage.stageHeight;
                _scroller.height = Math.ceil((stage.stageHeight / _content.height) * stage.stageHeight);
                if ((_scroller.y + _scroller.height) > stage.stageHeight) _scroller.y = stage.stageHeight - _scroller.height;
                _scroller.y = (_content.height < stage.stageHeight) ? 0 : _scroller.y;
                _scrollBar.visible = (_content.height > stage.stageHeight);
                _scrollValue = stage.stageHeight - _scroller.height;
                _content.y = -((_content.height - stage.stageHeight) * (_scroller.y / _scrollValue));
            }
        }
        
        // allows or disallows mouse wheel scrolling
        
        public function set useScrollWheel(b:Boolean):void {
            if (b && _addedToStage && !stage.willTrigger(MouseEvent.MOUSE_WHEEL)) {
                stage.addEventListener(MouseEvent.MOUSE_WHEEL, mouseWheelListener);
            } else if (stage.willTrigger(MouseEvent.MOUSE_WHEEL)) {
                stage.removeEventListener(MouseEvent.MOUSE_WHEEL, mouseWheelListener);
            }
        }
        
        public function get useScrollWheel():Boolean {
            return _useScrollWheel;
        }
        
        // sets the display object that will be scrolled
        
        public function set scrollTarget(targ:DisplayObjectContainer):void {
            _content = targ;
            adjustSize();
        }
        
        public function get scrollTarget():DisplayObjectContainer {
            return _content;
        }
        
        // sets the amout of ease to use
        
        public function set easeAmount(e:int):void {
            _easeAmount = (e <=0 ) ? 1 : Math.ceil(e);
        }
        
        public function get easeAmount():int {
            return _easeAmount;
        }
        
        // sets the color of the scrollbar track
        
        public function set trackColor(col:uint):void {
            var tColor_cxf:ColorTransform = new ColorTransform();
            _trackColor = col;
            tColor_cxf.color = _trackColor;
            _scrollTrack.transform.colorTransform = tColor_cxf;
        }
        
        public function get trackColor():uint {
            return _trackColor;
        }
        
        // sets the color of the scrollbar scroller
        public function set scrollerColor(col:Number):void {
            var sColor_cxf:ColorTransform = new ColorTransform();
            _scrollerColor = col;
            sColor_cxf.color = _scrollerColor;
            _scroller.transform.colorTransform = sColor_cxf;
        }
        
        public function get scrollerColor():Number {
            return _scrollerColor;
        }
    }
}

internal class CreatorClass {};