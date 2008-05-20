/***
 * 
 * SHOULD BE THE ONLY PUBLICY FACING CLASS FROM THE TOUCHAPI (THINK BLACKBOX / ENCAPSULATION)
 * 
 * Creates a gateway between external and native package assets. Offers more customizability... and abstraction.
 * 
 * 
 * COMPARE PERFORMANCE VS STRUCUTRE??
*/

package touchapi
{
	import flash.display.DisplayObjectContainer;
	import flash.events.Event;
	
	import touchapi.core.TUIOObject;
	import touchapi.core.TUIOSocket;
	public class TouchAPI
	{
//---------------------------------------------------------------------------------------------------------------------------------------------
// PUBLIC METHODS
//---------------------------------------------------------------------------------------------------------------------------------------------
		public static function init ($stage:DisplayObjectContainer, $host:String = "127.0.0.1", $debug:Boolean = true, $port:Number = 3000, $url:String = ''):void
		{
			TUIOSocket.init($stage, $debug, $host, $port, $url);
		}
//---------------------------------------------------------------------------------------------------------------------------------------------
		public static function getObjectById(id:Number):TUIOObject
		{
			return TUIOSocket.getObjectById(id);
		}
//---------------------------------------------------------------------------------------------------------------------------------------------
		public static function addObjectListener(id:Number, reciever:Object):void
		{
			TUIOSocket.addObjectListener(id, reciever);
		}
//---------------------------------------------------------------------------------------------------------------------------------------------
		public static function removeObjectListener(id:Number, reciever:Object):void
		{
			TUIOSocket.removeObjectListener(id, reciever);
		}
//---------------------------------------------------------------------------------------------------------------------------------------------
		public static function clear(event:Event = null):void
		{
			TUIOSocket.clear();
		}			
//---------------------------------------------------------------------------------------------------------------------------------------------
	}
}