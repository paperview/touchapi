package nui.tuio.profiles
{
	import nui.tuio.events.Tuio2DObjEvent;
	import nui.tuio.Tuio2DObj;
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
	public class Tuio2DObjProfile extends AbstractProfile
	{
		
		public function Tuio2DObjProfile()
		{

		}
		
		override public function get profileName():String
		{
			return "/tuio/2Dobj";
		}
		
		protected override function dispatchRemove(tuio:AbstractTuio):void
		{
			_dispatcher.dispatchEvent(new Tuio2DObjEvent(Tuio2DObjEvent.REMOVE_TUIO_2D_OBJ, tuio as Tuio2DObj));
		}
		
		protected override function processSet(message:OSCMessage):void
		{
			var s:int = message.getArgumentValue(1) as int;
			var i:int = message.getArgumentValue(2) as int;
			var x:Number = message.getArgumentValue(3) as Number;
			var y:Number = message.getArgumentValue(4) as Number;
			var a:Number = message.getArgumentValue(5) as Number;
			var X:Number = message.getArgumentValue(6) as Number;
			var Y:Number = message.getArgumentValue(7) as Number;
			var A:Number = message.getArgumentValue(8) as Number;
			var m:Number = message.getArgumentValue(9) as Number;
			var r:Number = message.getArgumentValue(10) as Number;
			
			var tuio2Dobj:Tuio2DObj = (_objectsList[s]==null) ? new Tuio2DObj(s, i, x, y, a, X, Y, A, m, r) : _objectsList[s] as Tuio2DObj;
			
			if(_objectsList[s]==null)
			{
				_objectsList[s] = tuio2Dobj;
				_dispatcher.dispatchEvent(new Tuio2DObjEvent(Tuio2DObjEvent.ADD_TUIO_2D_OBJ, tuio2Dobj));
				_dispatcher.dispatchEvent(new Tuio2DObjEvent(Tuio2DObjEvent.UPDATE_TUIO_2D_OBJ, tuio2Dobj));
			}
			else
			{
				if((tuio2Dobj.x!=x)||(tuio2Dobj.y!=y)||(tuio2Dobj.a!=a))
				{
					tuio2Dobj.update(x, y, a, X, Y, A, m, r);
					_dispatcher.dispatchEvent(new Tuio2DObjEvent(Tuio2DObjEvent.UPDATE_TUIO_2D_OBJ,tuio2Dobj));
					_objectsList[s] = tuio2Dobj;
				}
			}
		}
		
		
	}
}