package nui.tuio.events
{
	import flash.events.Event;
	
	[Exclude(name="clone", kind="method")]
	
	/**
	 * The TuioEvent is the generic dispatched event for all TuioProfiles for "refreshing".
	 * It's dispatched when, as specified for the tuio protocol, all frame data has 
	 * been recieved so clients must be notified they should be refreshed with the previosly
	 *  acquisted data.
	 * 
	 * @author Ignacio Delgado
	 * @see http://code.google.com/p/tuio-as3-lib
	 * @see http://mtg.upf.es/reactable/?software
	 */
	public final class TuioEvent extends Event
	{
 		
		/**
		 * The TuioEvent.REFRESH constant defines the value of the 
		 * <code>type</code> property of the event object 
		 * for a <code>refresh</code> event.
		 * 
		 * @eventType refresh
		 **/
		public static const REFRESH:String			= "refresh";
		
		
		//public var data:Object;
		
		/**
		 * Creates a TuioEvent object to pass as a parameter to event listeners.
		 * 
		 * @param type The type of the event, accessible 
		 * as <code>TuioEvent.type</code>.
		 */
		public function TuioEvent(type:String/*, inData:Object=null*/){
			super(type);
			//data = inData;
		}
		
		public override function clone():Event {
                return new TuioEvent(type/*, data*/);     
        }
	}
}