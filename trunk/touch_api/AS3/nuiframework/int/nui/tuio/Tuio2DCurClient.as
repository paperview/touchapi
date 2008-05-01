package nui.tuio
{
	import nui.tuio.profiles.Tuio2DCurProfile;
	
	/**
	 * @eventType nui.tuio.events.Tuio2DCurEvent.ADD_TUIO_2D_CUR
	 **/
	[Event(name="addTuio2DCur", type="nui.tuio.events.Tuio2DCurEvent")]
	
	/**
	 * @eventType nui.tuio.events.Tuio2DCurEvent.UPDATE_TUIO_2D_CUR
	 **/
	[Event(name="updateTuio2DCur", type="nui.tuio.events.Tuio2DCurEvent")]
	
	/**
	 * @eventType nui.tuio.events.Tuio2DCurEvent.REMOVE_TUIO_2D_CUR
	 **/
	[Event(name="removeTuio2DCur", type="nui.tuio.events.Tuio2DCurEvent")]
	
	/**
	 * 
	 * @author Ignacio Delgado
	 * @see http://code.google.com/p/tuio-as3-lib
	 * @see http://mtg.upf.es/reactable/?software
	 * 
	 */
	public class Tuio2DCurClient extends TuioClient
	{
		private var _tuio2DCurProfile:Tuio2DCurProfile;
		
		public function Tuio2DCurClient(host:String="localhost", port:uint=3000)
		{
			super(host, port);
			_tuio2DCurProfile = new Tuio2DCurProfile();
			addProfile(_tuio2DCurProfile);
		}
		
	}
}