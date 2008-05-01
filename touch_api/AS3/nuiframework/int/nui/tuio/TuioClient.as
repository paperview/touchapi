	package nui.tuio
{
	import flash.events.EventDispatcher;
	import flash.events.Event;
	import nui.osc.OSCConnection;
	import nui.osc.OSCConnectionEvent;
	import nui.osc.OSCMessage;
	import flash.events.IOErrorEvent;
	import flash.events.SecurityErrorEvent;
	import nui.tuio.events.TuioEvent;
	//import nui.tuio.profiles.Tuio2DObjProfile;
	//import nui.tuio.profiles.Tuio2DCurProfile;
	import nui.tuio.profiles.IProfile;
	
	
	/**
	 * @eventType nui.tuio.TuioEvent.REFRESH
	 **/
	[Event(name="refresh", type="nui.tuio.events.TuioEvent")]

	/**
	 * The TuioClient is a generic tuio client for listening to tuio messages of 
	 * specific tuoi profiles.
	 *  
	 * @author Ignacio Delgado
	 * @see http://code.google.com/p/tuio-as3-lib
	 * @see http://code.google.com/p/flosc
	 * @see http://mtg.upf.es/reactable/?software
	 * 
	 */
	public class TuioClient extends EventDispatcher
	{
		private var _OSCReceiver:OSCConnection;
		private var _port:int = 3000;
		private var _host:String = "localhost";
		
		protected var profiles:Array = new Array();
		
		
		/**
		 * 
		 * @param address
		 * @param port
		 * 
		 */
		public function TuioClient(host:String="nui.mine.nu", port:int=3000){
			_port = port;
			_host = host;
			_OSCReceiver = new OSCConnection(_host, _port);
			_OSCReceiver.addEventListener(OSCConnectionEvent.OSC_PACKET_IN, onPacketIn);		
			_OSCReceiver.addEventListener(Event.CONNECT,onConnect);
			_OSCReceiver.addEventListener(Event.CLOSE,onClose);
			_OSCReceiver.addEventListener(IOErrorEvent.IO_ERROR, onIOError);
			_OSCReceiver.addEventListener(SecurityErrorEvent.SECURITY_ERROR, onSecurityError);
			
		}
		
		public function addProfile(profile:IProfile):void
		{
			profile.addDispatcher(this);
			profiles.push(profile);
		}
		
		public function get port():int{
			return _port;
		}
		
		
		public function connect():void{
			_OSCReceiver.connect(_host, _port)
		}
		
		public function disconnect():void{
			_OSCReceiver.close();
		}
		

		
		private function onPacketIn(event:OSCConnectionEvent):void{
			for each(var message:OSCMessage in event.data.messages)
				processMessage(message);
		}
		
		private function processMessage(message:OSCMessage):void
		{
			for each( var profile:IProfile in profiles)
			{
				if(profile.profileName == message.name)
					profile.processCommand(message);
			}
		}
		
		
		// *** event handler to respond to successful connection attempt
		protected function onConnect (event:Event) : void {
			dispatchEvent(event);
		}
	
	
		// *** event handler called when server kills the connection
		protected function onClose (event:Event) : void {
			dispatchEvent(event);
		}
	
		
		protected function onIOError(event:IOErrorEvent) : void {
			//trace("OSCConnection.onConnectError()");		
			dispatchEvent(event);
		}
		
		protected function onSecurityError(event:SecurityErrorEvent) : void {
			//trace("OSCConnection.onConnectError()");		
			dispatchEvent(event);
		}
		
	}
}