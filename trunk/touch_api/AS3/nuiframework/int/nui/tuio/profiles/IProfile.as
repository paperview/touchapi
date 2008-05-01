package nui.tuio.profiles
{
	import nui.osc.OSCMessage;
	import nui.tuio.TuioClient;;
	
	/**
	 * 
	 * @author Ignacio Delgado
	 * @see http://code.google.com/p/tuio-as3-lib
	 * @see http://mtg.upf.es/reactable/?software
	 * 
	 */
	public interface IProfile
	{
		function get profileName() : String;
		function processCommand(message:OSCMessage) : void;
		function addDispatcher(dispatcher:TuioClient) : void;
	}
}