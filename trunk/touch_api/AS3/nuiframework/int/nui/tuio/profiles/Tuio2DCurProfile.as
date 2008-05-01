package nui.tuio.profiles
{
	import nui.tuio.Tuio2DCur;
	import nui.tuio.events.Tuio2DCurEvent;
	import nui.tuio.AbstractTuio;
	import nui.osc.OSCMessage;
	import flash.events.EventDispatcher;

	/**
	 * 
	 * @author Ignacio Delgado
	 * @see http://code.google.com/p/tuio-as3-lib
	 * @see http://mtg.upf.es/reactable/?software
	 * 
	 */
	public class Tuio2DCurProfile extends AbstractProfile
	{
		
		public function Tuio2DCurProfile()
		{

		}
		
		override public function get profileName():String
		{
			return "/tuio/2Dcur";
		}
			
		protected override function dispatchRemove(tuio:AbstractTuio):void
		{
			_dispatcher.dispatchEvent(new Tuio2DCurEvent(Tuio2DCurEvent.REMOVE_TUIO_2D_CUR, tuio as Tuio2DCur));
		}
		
		protected override function processSet(message:OSCMessage) : void
		{
			var s:int = message.getArgumentValue(1) as int;
			var x:Number = message.getArgumentValue(2) as Number;
			var y:Number = message.getArgumentValue(3) as Number;
			var X:Number = message.getArgumentValue(4) as Number;
			var Y:Number = message.getArgumentValue(5) as Number;
			var m:Number = message.getArgumentValue(6) as Number;
						
			var tuio2Dcur:Tuio2DCur = (_objectsList[s] == null) ?  new Tuio2DCur(s, x, y, X, Y, m) : _objectsList[s] as Tuio2DCur;
						
			if(_objectsList[s] == null)
			{
				_objectsList[s] = tuio2Dcur;
				_dispatcher.dispatchEvent(new Tuio2DCurEvent(Tuio2DCurEvent.ADD_TUIO_2D_CUR, tuio2Dcur));
				_dispatcher.dispatchEvent(new Tuio2DCurEvent(Tuio2DCurEvent.UPDATE_TUIO_2D_CUR, tuio2Dcur));	
			}
			else
			{
				tuio2Dcur.update(x, y, X, Y, m);
				_dispatcher.dispatchEvent(new Tuio2DCurEvent(Tuio2DCurEvent.UPDATE_TUIO_2D_CUR, tuio2Dcur));
				_objectsList[s] = tuio2Dcur;
			}
		}
	}
}