package nui.tuio.events
{
	import flash.events.Event;
	import nui.tuio.Tuio2DObj;
	
	/**
	 * The Tuio2DObjEvent is the specific event for the Tuio2DObj profile.
	 * There are three kinds of Tuio2DObjEvent:
	 * <ul>
	 * 	<li>Tuio2DObjEvent.ADD_TUIO_2D_OBJ: dispatched when a tuio2Dobj has been added</li>
	 * 	<li>Tuio2DObjEvent.UPDATE_TUIO_2D_OBJ: dispatched when an existing tuio2Dobj has been updated</li>
	 * 	<li>Tuio2DObjEvent.REMOVE_TUIO_2D_OBJ: dispatched when a tuio2Dobj has been removed</li>
	 * </ul>
	 * 
	 * <p>Take note there's no specific REFRESH event kind for tuioProfiles. The <code>REFRESH</code> 
	 * event kind is a TuioEvent.REFRESH</p>
	 * 
	 * @author Ignacio Delgado
	 * @see nui.tuio.profiles.Tuio2DObjProfile
	 * @see nui.tuio.TuioEvent;
	 * @see nui.tuio.profiles.AbstractProfile;
	 * @see http://code.google.com/p/tuio-as3-lib
	 * @see http://mtg.upf.es/reactable/?software
	 */
	public class Tuio2DObjEvent extends Event
	{
		/**
		 * The Tuio2DObjEvent.ADD_TUIO_2D_OBJ constant defines the value of the 
		 * <code>type</code> property of the event object 
		 * for a <code>addTuio2DObj</code> event.
		 * 
		 * @eventType addTuio2DObj
		 **/
		public static const ADD_TUIO_2D_OBJ:String 	= "addTuio2DObj";
		
		/**
		 * The Tuio2DObjEvent.UPDATE_TUIO_2D_OBJ constant defines the value of the 
		 * <code>type</code> property of the event object 
		 * for a <code>updateTuio2DObj</code> event.
		 * 
		 * @eventType updateTuio2DObj
		 **/
		public static const UPDATE_TUIO_2D_OBJ:String 	= "updateTuio2DObj";
		
		/**
		 * The Tuio2DObjEvent.REMOVE_TUIO_2D_OBJ constant defines the value of the 
		 * <code>type</code> property of the event object 
		 * for a <code>removeTuio2DObj</code> event.
		 * 
		 * @eventType removeTuio2DObj
		 **/
		public static const REMOVE_TUIO_2D_OBJ:String 	= "removeTuio2DObj";
		
		/**
		* A Tuio2DObj object containing all data associated to the event.
		*/
		public var data:Tuio2DObj;
		
		/**
		 * Creates a Tuio2DObjEvent object to pass as a parameter to event listeners.
		 * 
		 * @param type The type of the event, accessible 
		 * as <code>Tuio2DObjEvent.type</code>.
		 * @param data The Tuio2DObj object data associated to the event. Tuio2DObjEvent 
		 * listeners can access this information through the <code>data</code> property.
		 */
		public function Tuio2DObjEvent(type:String, data:Tuio2DObj)
		{
			super(type);
			this.data = data;
		}
		
		public override function clone():Event
		{
			return new Tuio2DObjEvent(type,data);
		}
		
	}
}