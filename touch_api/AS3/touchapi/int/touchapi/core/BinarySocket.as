package touchapi.core
{	
	import flash.display.Stage;
	import flash.display.DisplayObject;
	import flash.display.DisplayObjectContainer;
	import flash.net.Socket;
	import flash.events.*;
	import flash.display.Sprite;
	import flash.utils.Endian;
	import flash.utils.ByteArray;

	internal class BinarySocket extends Sprite
	{	
		private static var debug:Boolean;				
		private static var initialized:Boolean;	
		private static var n_socket:Socket = new Socket();
		private static var coordsBytes:ByteArray = new ByteArray();
		private static var inputs:ByteArray = new ByteArray();
		private static var streamLength:int = 136;
	
//---------------------------------------------------------------------------------------------------------------------------------------------	
		public static function init($stage:DisplayObjectContainer, $host:String = "localhost", $port:Number = 3000, $url:String = '', $debug:Boolean = true):void
		{
			if (initialized) { return; }			
			initialized = true;		
			debug = $debug;	

			n_socket.addEventListener (Event.CONNECT, onSocketConnected);
			n_socket.addEventListener (ProgressEvent.SOCKET_DATA, onSocketData);
			n_socket.addEventListener (IOErrorEvent.IO_ERROR, onSocketError);
			n_socket.connect ('localhost', 0x4a54);
			
			//balle_mc.addEventListener (Event.ENTER_FRAME, moveIt);
			//connect_btn.addEventListener (MouseEvent.CLICK, onClick);
		}
//---------------------------------------------------------------------------------------------------------------------------------------------	
		private static function moveIt( e:Event ):void
		{
			e.target.x += e.target.posX;
			e.target.y += e.target.posY;
		}
//---------------------------------------------------------------------------------------------------------------------------------------------	
		private static function onSocketData( e:Event ):void
		{
		// grab the bytes coming in
		while ( e.target.bytesAvailable > 0 ) 
		{
			coordsBytes.writeByte ( e.target.readByte() );	
			// when a full packet is available, parse it
			if ( coordsBytes.position == streamLength ) 
		
			{		
			inputs.position = 0;
			inputs.writeBytes ( coordsBytes );
			coordsBytes.position = inputs.position = 0;
			parseBytes ( inputs );		
			}	
		}
		}
//---------------------------------------------------------------------------------------------------------------------------------------------	
		private static function parseBytes( pInputs:ByteArray ):void 
		{
		// get X and Y values, 8 first bytes
		// int X : 4bytes
		// int Y : 4bytes
	
		var xValue:int = pInputs.readInt();
		var yValue:int = pInputs.readInt();
	
	//	xvalues.text = " x " + xValue;
	//	yvalues.text = " y " + yValue
	
		// set destination values
	//	balle_mc.posX = xValue;
	//	balle_mc.posY = yValue;
	
		// following is buttons inputs
		// each byte is set to 1 when a button is pressed otherwise it is set to 0
	
		var aButton:int = pInputs.readByte();
		var bButton:int = pInputs.readByte();
		var cButton:int = pInputs.readByte();
		var dButton:int = pInputs.readByte();
	
		var eButton:int = pInputs.readByte();
		var fButton:int = pInputs.readByte();
		var gButton:int = pInputs.readByte();
		var hButton:int = pInputs.readByte();
	
		var iButton:int = pInputs.readByte();
		var jButton:int = pInputs.readByte();
	
		// if a button is set to 1, then display the input
		if ( aButton ) trace("A");
		if ( bButton ) trace("B");
		if ( cButton ) trace("C");
		if ( dButton ) trace("D");
		if ( eButton ) trace("L1");
		if ( fButton ) trace("R1");
		if ( gButton ) trace("L2");
		if ( hButton ) trace("R2");
		if ( iButton ) trace("9");
		if ( jButton ) trace("10");
		}		
//---------------------------------------------------------------------------------------------------------------------------------------------	
		private static function onSocketConnected( e:Event ):void
		{
		trace('Connected');
		}
//---------------------------------------------------------------------------------------------------------------------------------------------	
		private static function onSocketError( e:IOErrorEvent ):void
		{
			trace('Socket Error!');
		}
//---------------------------------------------------------------------------------------------------------------------------------------------	
	}
}