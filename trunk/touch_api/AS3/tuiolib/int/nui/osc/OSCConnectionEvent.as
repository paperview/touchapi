
	 
package nui.osc 
{
	import flash.events.Event;
	
	[Exclude(name="clone", kind="method")]
	 
	/**
	 * An OSCConnection dispatches an OSCConnectionEvent object when an 
	 * osc packet is recieved or send. Ther are two types of data event:
	 * <ul>
	 * 	<li>OSCConnectionEvent.OSC_PACKET_IN: dispatched for osc data received.</li>
	 * 	<li>OSCConnectionEvent.OSC_PACKET_OUT: dispatched for osc data send.</li>
	 * </ul>
	 * 
	 * <p>@copy 2007 http://www.h-umus.it</p>
	 * 
	 * @author Ignacio Delgado
	 * @see http://code.google.com/p/flosc
	 * @see nui.osc.OSCConnection
	 * @langversion ActionScript 3.0
	 * @playerversion Flash 9+
	 */
	public class OSCConnectionEvent extends Event 
	{	
		/**
		 * The OSCConnectionEvent.OSC_PACKET_IN constant defines the value of the 
		 * <code>type</code> property of the event object 
		 * for a <code>OSCPacketIn</code> event.
		 * 
 		 * @eventType OSCPacketIn 
		 */		
		public static const OSC_PACKET_IN : String 	= "OSCPacketIn";
		
		/**
		 * The OSCConnectionEvent.OSC_PACKET_OUT constant defines the value of the 
		 * <code>type</code> property of the event object 
		 * for a <code>OSCPacketOut</code> event.
		 * 
		 * @eventType OSCPacketOut
		 */		
		public static const OSC_PACKET_OUT : String = "OSCPacketOut";
		
		/**
		* The OSCPacket loaded into Flash Player.
		*/
		public var data:OSCPacket;
		
		/**
		 * Creates an Event object to pass as a parameter to event listeners.
		 * 
		 * @param type The type of the event, accessible 
		 * as <code>OSCConnectionEvent.type</code>.
		 * @param data The OSCPacket loaded into Flash Player. Event 
		 * listeners can access this information through the <code>data</code> property
		 * 
		 */
		public function OSCConnectionEvent(type:String,data:OSCPacket=null) 
		{
			super(type);
			this.data = data; 
		}
		
		
		public override function clone():Event
		{
			return new OSCConnectionEvent(type, data);
		}	
	}
}