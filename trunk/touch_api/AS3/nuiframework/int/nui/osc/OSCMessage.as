
	 
package nui.osc
{
	 /**
	 * An OSCMessage object is the data unit for encapsulating OSC messages. 
	 * OSC messages, as in OSC specs, are defined by a mandatory <b>name</b>
	 * and a following collection of OSC arguments.
	 * 
	 * <p>@copy 2007 http://www.h-umus.it</p>
	 * 
	 * @author Ignacio Delgado
	 * @see http://code.google.com/p/flosc
	 * @see http://opensoundcontrol.org
	 * @see nui.osc.OSCPacket
	 * @see nui.osc.OSCArgument
	 * @langversion ActionScript 3.0
	 * @playerversion Flash 9+
	 */
	public class OSCMessage
	{
		private var _name			: String;
		private var _pattern		: RegExp = /^((\/(\w)+)+)$|^\/$/;
		private var _argumentsArray : Array;
		
		/**
		 * Creates a new os message object. If no name value passed it's 
		 * default value is "/".
		 * 
		 * @param name The name value must be a valid osc name.
		 * 
		 */
		public function OSCMessage(name:String="/")
		{
			this.name = name;
			_argumentsArray = new Array();
		}
		
		//getters and setters
		/**
		 * 
		 * Get or set the <code>name</code> property of the message.
		 * 
		 * <p>When setting a name it must be a valid osc name.</p>
		 * 
		 */
		public function get name() : String
		{
			return _name;
		}
		
		/**
		 * @private
		 **/
		public function set name(value:String) : void
		{
			//TODO check it's a valid osc name
			if(_pattern.test(value))
				_name=value;
			else
				throw new Error("Not a valid OSC name");
		}
		
		/**
		 * Arguments array of an OSC message.
		 **/
		public function get arguments() : Array
		{
			return _argumentsArray;
		}
		
		
		/**
		 * Add an OSCArgument to the message.
		 * 
		 * @param arg An OSCArgument
		 * 
		 */
		public function addArg(arg:OSCArgument):void
		{
			_argumentsArray.push(arg);
		}
		
		
		/**
		 * Get OSCArgument <code>value</code> at <code>index</code> position. If the 
		 * <code>index</code> value is out of bounds it will return null;
		 * 
		 * @param index
		 * @return The OSCArgument at <code>index</code> position
		 * 
		 */
		public function getArgumentValue(index:uint):Object{
			if(index>_argumentsArray.length)
				return null;
			return OSCArgument(_argumentsArray[index]).value;
		}
		
		
		/**
		 * This methods generates and XML rapresentation of the OSCMessage 
		 * defined by the Flosc DTD.
		 * 
		 * <p>
		 * 	It would look like :
		 * 	<pre>		 
		 * 		&lt;MESSAGE NAME="/osc/address"&gt;
		 * 			&lt;ARGUMENT TYPE="i" VALUE="2"/&gt;
		 * 			&lt;ARGUMENT TYPE="f" VALUE="3.0"/&gt;
		 * 		&lt;/MESSAGE&gt;
		 * 	</pre>
		 * </p>
		 * 
		 * @return 
		 * 
		 */
		public function getXML():XML{
			var messageNode:XML = 	<MESSAGE NAME={_name}/>;
			for each(var arg:OSCArgument in _argumentsArray)
				messageNode.ARGUMENT+=arg.getXML();
				
			return messageNode;
		}	
	}
}