

package nui.osc 
{ 
	 /**
	 * An OSCArgument object is the data unit for encapsulating OSC arguments.
	 * An OSCArgument is composed by an osc argument type defined in the OSC specs
	 * and it's value. As some limits may appear during this implementation and the 
	 * Flosc server implementation not all osc types and values are supported at the 
	 * moment of this writing. Future updates on this libary and the Flosc server
	 * may upgrade for new types and values.
	 * 
	 * <p>@copy 2007 http://www.h-umus.it</p>
	 * 
	 * @author Ignacio Delgado
	 * @see http://code.google.com/p/flosc
	 * @see http://opensoundcontrol.org
	 * @see nui.osc.OSCMessage
	 * @langversion ActionScript 3.0
	 * @playerversion Flash 9+
	 * 
	 */
	 public class OSCArgument
	 {	 
		 /**
		 * int32 <code>type</code> value.
		 */
		 public static const i:String = "i";		// int32
		 
		 /**
		 * float32 <code>type</code> value.
		 */
		 public static const f:String = "f";		// float32
		 
		 /**
		 * OSC-String <code>type</code> value.
		 */
		 public static const s:String = "s";		// OSC-String
//		 public static var b:String = "b";		// OSC-blob

		 /**
		 * @private
		 * 64 bit big-endian two's complement integer <code>type</code> value.
		 */
		 public static const h:String = "h";		// 64 bit big-endian two's complement integer

		 /**
		 * OSC-timetag <code>type</code> value.
		 */
		 public static const t:String = "t";		// OSC-timetag
		 
		 /**
		 * 64 bit ("double") <code>type</code> value.
		 */
		 public static const d:String = "d";		// 64 bit ("double")
		 
		 /**
		 * alternate type for OSC-String <code>type</code> value.
		 */
		 public static const S:String = "S";		// alternate type for OSC-String
		 
		 /**
		 * ascii character, 32 bit <code>type</code> value.
		 */
		 public static const c:String = "c";		// ascii character, 32 bit
		 
		 /**
		 * 32 bit RGBA color <code>type</code> value.
		 */
		 public static const r:String = "r";		// 32 bit RGBA color
		 
		 /**
		 * 4 byte MIDI message <code>type</code> value.
		 */
		 public static const m:String = "m";		// 4 byte MIDI message
		 
		 /**
		 * TRUE <code>type</code> value.
		 */
		 public static const T:String = "T";		// TRUE
		 
		 /**
		 * FALSE <code>type</code> value.
		 */
		 public static const F:String = "F"; 		// FALSE
//		 public static var N:String = "N";
//		 public static var I:String = "I";
		 
		 private var _type	: String;
		 private var _value	: Object;
		 
		 /**
		  * Constructor
		  * 
		  * @param type The type of the Argument
		  * @param value The value of the Argument
		  * 
		  */
		 public function OSCArgument (type:String , value:Object)
		 {
		 	// TODO: check is a valid OSCArgument type
		 	_type=type;
		 	_value=value;
		 }
		 
		 //getters and setters
		 
		 /**
		  * The type of the OSCArgument.
		  */
		 public function get type() : String
		 {
		 	return _type;
		 }
		 
		 /**
		 * The value of the OSCArgument.
		 **/
		 public function get value() : Object
		 {
		 	return _value;
		 }
		 
		 //public functions
		 /**
		  * This methods generates and XML rapresentation of the OSCArgument 
		  * defined by the Flosc DTD.
		  * <p>
		  * 	It would look like
		  * 	<pre>
		  * 			&lt;ARGUMENT TYPE="s" VALUE="string value" /&gt;
		  * 	</pre>
		  * </p>
		  * 
		  * @return An XML rapresentation of the OSCArgument
		  * 
		  */
		 public function getXML():XML{
		 	var value:String = (_type == OSCArgument.s || _type == OSCArgument.S) ? escape(String(_value)) : _value.toString();
		 	var node:XML = 	<ARGUMENT TYPE={_type} VALUE={value} />;

			return node;
		 } 
	 }
 }