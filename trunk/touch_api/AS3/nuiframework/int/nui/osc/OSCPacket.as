
	 
package nui.osc 
{ 
	 /**
	 * The OSCPacket is the main data structure in an OSCConnection. 
	 * The OSC packet structure is mainly defined in the 
	 * Open Sound Control protocol specs. This class tryes to encapsulate
	 * this packet definition in an AS3 object mantaining as much as possible
	 * as the original specification. However due to some limits it may in some
	 * ways defer from expecifications. An osc packet structure is mainly defined as
	 * a destination address, a destination port, a time and a collection of at least
	 * one osc message.
	 * 
	 * <p>&copy; 2007 http://www.h-umus.it<p>
	 * 
 	 * @author Ignacio Delgado
	 * @see http://code.google.com/p/flosc
	 * @see http://opensoundcontrol.org
	 * @see nui.osc.OSCConnection
	 * @langversion ActionScript 3.0
	 * @playerversion Flash 9+
	 */
	public class OSCPacket 
	{
		private var _address 	: String;
		private var _port 		: uint;
		private var _time		: Number;
		private var _messages	: Array;	
		
		/**
		 * An OSCPacket constructor that can called in two different ways.
		 * <ul>
		 * 	<li>with no arguments <br/>
		 * 		<pre>
		 * 			var myOSCPacket:OSCPacket = new OSCPacket();
		 * 		</pre>
		 *  </li>
		 *  <li>passing <b>time</b>, <b>destination address</b> and <b>destination port</b><br/>
		 * 		<pre>
		 * 			var time:Number = 0;
		 * 			var address:String = "localhost";
		 * 			var port:uint = 3333;
		 * 			var myOSCPacket:OSCPacket = new OSCPacket(time, address, port);
		 * 		</pre>
		 *  </li>
		 * </ul> 
		 * 
		 **/ 
		public function OSCPacket(...args)
		{
			switch(args.length){
				case 0:
					OSCPacket_constructor1();
					break;
				case 3:
					OSCPacket_constructor1();
					OSCPacket_constructor2(args[0] as Number, args[1] as String, args[2] as uint);
					break;
			}
		}

		// getters and setters
		/**
		 * Get/Set address value of the OSCPacket.
		 * <pre>
		 * 		var packetAddress:String = myOSCPacket.address;
		 * 
		 * 		myOSCPacket.address = "127.0.0.1";
		 * </pre>
		 **/
		public function get address() : String
		{
			return _address;	
		}
		
		 /**
		 * @private
		 **/
		public function set address(value:String) : void
		{
			_address = value;
		}
		
		/**
		 * Get/Set port value of the OSCPacket.
		 * <pre>
		 * 		var packetPort:uint = myOSCPacket.port;
		 * 	
		 * 		myOSCPacket.port = 3333;
		 * </pre>
		 **/
		public function get port() : uint
		{
			return _port;
		}
		
		/**
		 * @private
		 **/
		public function set port(value:uint) : void
		{
			_port = value;
		}
		
		/**
		 * Get/Set time of the OSCPacket.
		 **/
		public function get time():Number
		{
			return _time;
		}
		
		/**
		 * @private
		 **/
		public function set time(value:Number):void
		{
			_time=value;
		}
		
		
		/**
		 * Array containing the OSCMessages of the packet.
		 * @see nui.osc.OSCMessage
		 */
		public function get messages():Array
		{
			return _messages;
		}
		
		
		//public functions
				
		/**
		 * Add an OSCMessage to the packet.
		 * 
		 * @param message An OSCMessage to include in the OSCPacket
		 * 
		 */
		public function addMessage(message:OSCMessage):void
		{
			_messages.push(message);
		}
		
		/**
		 * This method returns an OSC XML-encoded defined in the Flosc DTD, 
		 * that can be understood by the Flosc server.
		 * 
		 * <p>
		 * 	It would look like :
		 * 	<pre>
		 * 			&lt;OSCPACKET ADDRESS="127.0.0.1" PORT="3333" TIME="0"&gt;
		 * 				&lt;MESSAGE NAME="/osc/address"&gt;
		 * 					&lt;ARGUMENT TYPE="i" VALUE="2"/&gt;
		 * 					&lt;ARGUMENT TYPE="f" VALUE="3.0"/&gt;
		 * 				&lt;/MESSAGE&gt;
		 * 			&lt;/OSCPACKET&gt;
		 * 	</pre>
		 * </p>
		 * 
		 * @return An XML that represents the OSCPacket understood by the Flosc server.
		 * 
		 */
		public function getXML():XML
		{
			var packetXML:XML = <OSCPACKET ADDRESS={address} PORT={port} TIME={time} />;
			for each( var message:OSCMessage in messages)
				packetXML.MESSAGE+=message.getXML();
			return packetXML;
		}
		
		
		//private functions
		/**
		 * @private
		 * Constructor for OSCPacket with no aruments
		 **/
		private function OSCPacket_constructor1():void
		{
			_time=0;
			_messages = new Array();
		}
		

		/**
		 * @private
		 * Constructor for OSCPacket with arguments
		 * 
		 * @param time The TIME value of the Packet
		 * @param inAddress The destination address
		 * @param inPort The destination port
		 * 
		 */
		private function OSCPacket_constructor2(time:Number, inAddress:String, inPort:uint):void
		{
			_time=time;
			_address = inAddress;
			_port = inPort;
		}
		
	}
}