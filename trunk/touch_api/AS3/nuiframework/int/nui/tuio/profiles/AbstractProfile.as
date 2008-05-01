package nui.tuio.profiles
{
	import flash.utils.Dictionary;
	import nui.osc.OSCMessage;
	import nui.tuio.events.TuioEvent;
	import nui.tuio.AbstractTuio;
	import flash.errors.IllegalOperationError;
	import nui.tuio.TuioClient;
	
	//ABSTRACT Class (should be subclassed and not instantiaded)
	/**
	 * The AbstractProfile class defines the base for all tuio profiles implementation.
	 * It defines a base structure to analyse data recieved from a profile and
	 * decide to dispatch proper events when tuio added, removed, updated o simply 
	 * a refresh. All tuio profiles must extend the AbstractProfile. The main methods 
	 * to override are:
	 * 
	 * @author Ignacio Delgado
	 * @see http://code.google.com/p/tuio-as3-lib
	 * @see http://mtg.upf.es/reactable/?software
	 * 
	 */
	public class AbstractProfile implements IProfile
	{	
		/**
		* The tuio client that will dispatch the events.
		*/
		protected var _dispatcher:TuioClient;
		
		/**
		* @private The current id frame being analized
		*/
		private var _currentFrame:int = 0;
		
		/**
		* @private Last analized id frame
		*/
		private var _lastFrame:int =0;
		
		/**
		* @private current objects in frame
		*/
		protected var _objectsList:Dictionary = new Dictionary();
		
		//protected var _newObjectList:Dictionary = new Dictionary();
		
		/**
		* @private current alive objects
		*/
		protected var _aliveObjectList:Dictionary = new Dictionary();
		
		/**
		 * Get the profile name
		 */
		public function get profileName():String
		{
			throw new IllegalOperationError("Abstract method:" + 
					"must be overriden in a subclass");
		}
		
		/**
		 * Sets the TuioClient that will dispatch the Tuio events.
		 * 
		 * @param dispatcher The tuio client
		 * 
		 */
		public final function addDispatcher(dispatcher:TuioClient):void
		{
			_dispatcher = dispatcher;
		}
		
		/**
		 * Processes a Tuio message looking for <code>set</code>,<code>alive</code>
		 * and <code>fseq</code> messages so to decide the proper event to dispatch.
		 * 
		 * @param message The TUIO/OSC message to parse
		 * 
		 */
		public final function processCommand(message:OSCMessage) : void
		{
			var command:String = (String)(message.getArgumentValue(0));
			
			if((command == "set") && (_currentFrame >= _lastFrame))
			{
				processSet(message);
			}
			else if((command=="alive")&&(_currentFrame >= _lastFrame))
			{
				processAlive(message);
			}
			else if(command == "fseq")
			{
				processFseq(message);
			}
		}
		
		/**
		 * 
		 * @param tuio
		 * 
		 */
		protected function dispatchRemove(tuio:AbstractTuio) : void
		{
			throw new IllegalOperationError("Abstract method:" + 
					"must be overriden in a subclass");
		}
		
		/**
		 * 
		 * @param message
		 * 
		 */
		protected function processSet(message:OSCMessage) : void
		{
			throw new IllegalOperationError("Abstract method:" + 
					"must be overriden in a subclass");
		}
		
		protected function processAlive(message:OSCMessage):void
		{
			var _newObjectList:Dictionary = new Dictionary();
			
			for(var index:uint = 1; index < message.arguments.length; index++)
				{
					var s_id:int = message.getArgumentValue(index) as int;
					_newObjectList[s_id]=s_id;
					if(_aliveObjectList[s_id]!=null)
					{
						_aliveObjectList[s_id]=null;
						delete(_aliveObjectList[s_id]);
					}
				}
						
				for each(var s:int in _aliveObjectList){
					dispatchRemove(_objectsList[s] as AbstractTuio);
					_objectsList[s]=null;
					delete(_objectsList[s]);
				}
						
				//var buffer:Dictionary = _aliveObjectList;
				_aliveObjectList = _newObjectList;
				/*_newObjectList = buffer;
				for (var key:Object in _newObjectList) {
					_newObjectList[key]=null;
					delete(_newObjectList[key]);
				}*/
		}
		
		protected function processFseq(message:OSCMessage):void
		{
			_lastFrame = _currentFrame;
			_currentFrame = message.getArgumentValue(1) as int;
						
			if(_currentFrame == -1)
				_currentFrame = _lastFrame;
						
			if(_currentFrame >= _lastFrame)
				_dispatcher.dispatchEvent(new TuioEvent(TuioEvent.REFRESH));
		}
	}
}