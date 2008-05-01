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
	import flash.events.Event;
	import touchapi.core.TUIOSocket;
	import touchapi.core.TUIOObject;
	public class TouchAPI
	{
//---------------------------------------------------------------------------------------------------------------------------------------------
// PUBLIC METHODS
//---------------------------------------------------------------------------------------------------------------------------------------------
		public static function init ($stage:Object, $host:String = "localhost", $debug:Boolean = true, $port:Number = 3000, $url:String = ''):void
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