package nui.tuio.events
{
	import flash.events.Event;
	import nui.tuio.Tuio2DCur;
	
	/**
	 * The Tuio2DCurEvent is the specific event for the Tuio2DCur profile.
	 * There are three kinds of Tuio2DCurEvent:
	 * <ul>
	 * 	<li>Tuio2DCurEvent.ADD_TUIO_2D_CUR: dispatched when a tuio2Dcur has been added</li>
	 * 	<li>Tuio2DCurEvent.UPDATE_TUIO_2D_CUR: dispatched when an existing tuio2Dcur has been updated</li>
	 * 	<li>Tuio2DCurEvent.REMOVE_TUIO_2D_CUR: dispatched when a tuio2Dcur has been removed</li>
	 * </ul>
	 * 
	 * <p>Take note there's no specific REFRESH event kind for tuioProfiles. The <code>REFRESH</code> 
	 * event kind is a TuioEvent.REFRESH</p>
	 * 
	 * @author Ignacio Delgado
	 * @see nui.tuio.profiles.Tuio2DCurProfile
	 * @see nui.tuio.TuioEvent;
	 * @see nui.tuio.profiles.AbstractProfile;
	 * @see http://code.google.com/p/tuio-as3-lib
	 * @see http://mtg.upf.es/reactable/?software
	 */
	public class Tuio2DCurEvent extends Event
	{
		/**
		 * The Tuio2DCurEvent.ADD_TUIO_2D_CUR constant defines the value of the 
		 * <code>type</code> property of the event object 
		 * for a <code>addTuio2DCur</code> event.
		 * 
		 * @eventType addTuio2DCur
		 **/
		public static const ADD_TUIO_2D_CUR:String		= "addTuio2DCur";
		
		/**
		 * The Tuio2DCurEvent.UPDATE_TUIO_2D_CUR constant defines the value of the 
		 * <code>type</code> property of the event object 
		 * for a <code>updateTuio2DCur</code> event.
		 * 
		 * @eventType updateTuio2DCur
		 **/
		public static const UPDATE_TUIO_2D_CUR:String 	= "updateTuio2DCur";
		
		/**
		 * The Tuio2DCurEvent.REMOVE_TUIO_2D_CUR constant defines the value of the 
		 * <code>type</code> property of the event object 
		 * for a <code>removeTuio2DCur</code> event.
		 * 
		 * @eventType removeTuio2DCur
		 **/
		public static const REMOVE_TUIO_2D_CUR:String	= "removeTuio2DCur";
				
		/**
		* A Tuio2DCur object containing all data associated to the event.
		*/
		public var data:Tuio2DCur;
		
		/**
		 * Creates a Tuio2DCurEvent object to pass as a parameter to event listeners.
		 * 
		 * @param type The type of the event, accessible 
		 * as <code>Tuio2DCurEvent.type</code>.
		 * @param data The Tuio2DCur object data associated to the event. Tuio2DCurEvent 
		 * listeners can access this information through the <code>data</code> property.
		 */
		public function Tuio2DCurEvent(type:String, data:Tuio2DCur)
		{
			super(type);
			this.data = data;
		}
		
		public override function clone():Event
		{
			return new Tuio2DCurEvent(type,data);
		}
		
	}
}