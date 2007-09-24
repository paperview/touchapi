package com.onebyonedesign.utils.events {
    
    import flash.events.Event;
    
    public class ScrollBarEvent extends Event {
        
        public static const SCROLL_DOWN:String = "onDown";
        public static const SCROLL_UP:String = "onUp";
        
        public function ScrollBarEvent(dir:String) {
            super(dir);
        }
    }
}