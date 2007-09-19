﻿package com.nui.tuio {

/**
 * LastChanged:
 * 
 * $Author: naturalui $
 * $Revision: 66 $
 * $LastChangedDate: 2007-08-25 08:11:49 -0500 (Sat, 25 Aug 2007) $
 * $URL: https://touchapi.googlecode.com/svn/trunk/TouchAPI_Flex/src/lib/com/nui/tuio/TUIO.as $
 * 
 */
 

import flash.display.InteractiveObject;
import flash.display.Sprite;
import flash.events.DataEvent;
import flash.events.Event;
import flash.events.IOErrorEvent;
import flash.events.ProgressEvent;
import flash.events.SecurityErrorEvent;
import flash.geom.Point;
import flash.net.URLLoader;
import flash.net.URLRequest;
import flash.net.XMLSocket;
import flash.system.System;
import flash.text.TextField;
import flash.text.TextFieldAutoSize;
import flash.text.TextFormat;

import flash.events.MouseEvent;
import flash.display.DisplayObject;

public class TUIO 
{	
	private static var INSTANCE:TUIO;	

	private static var HOST:String;
	private static var PORT:Number;	

	private static var STAGE_WIDTH:int;
	private static var STAGE_HEIGHT:int;	
			
    private static var DEBUG_MODE:Boolean = true;
	private static var RECORD_MODE:Boolean;
		
	private static var SOCKET:XMLSocket;
	private static var STAGE:Sprite;

	private static var TESTING_XML_URL:String; 
	private static var TESTING_XML:XML;
	private static var TESTING_XML_LOADER:URLLoader;	
	
	private static var RECORDED_XML:XML;
		
	private static var objectArray:Array;
	private static var DEBUG_TEXT:TextField;
	
	private static var EMULATE_FLEX_MOUSE:Boolean;

	/**********************************************************
	 * INIT
	***********************************************************/

	public static function init ( s:Sprite, host:String, port:Number, stageWidth:int, stageHeight:int, testingXML:String, recordMode:Boolean ):TUIO
	{     	
		if( INSTANCE == null ) 
		{
			INSTANCE = new TUIO( init );

			STAGE = s;
						
			STAGE_WIDTH = stageWidth;
			STAGE_HEIGHT = stageHeight;
						
			TESTING_XML_URL = testingXML;
			
			HOST = host;
			PORT = port;
			
			RECORD_MODE = recordMode;
			
			INSTANCE.start();
			
		}
		return INSTANCE;	
	}

	/**********************************************************
	 * START
	***********************************************************/
		
	private function start ():void
	{
		EMULATE_FLEX_MOUSE = true;
		
		objectArray = new Array();
		
		if( DEBUG_MODE )
		{
			activateDebugMode();				
		}
	    if ( RECORD_MODE )
		{	
			activateRecordMode();
		}	
		if( TESTING_XML_URL != '' )
		{
			TESTING_XML_LOADER = new URLLoader();
			TESTING_XML_LOADER.addEventListener( 'complete', testingXmlOnLoaded );
			TESTING_XML_LOADER.load( new URLRequest( TESTING_XML_URL ) );				
		} else
		{
			SOCKET = new XMLSocket();
			
			SOCKET.addEventListener( Event.CLOSE, closeHandler );
			SOCKET.addEventListener( Event.CONNECT, connectHandler );
			SOCKET.addEventListener( DataEvent.DATA, dataHandler );
			SOCKET.addEventListener( IOErrorEvent.IO_ERROR, ioErrorHandler );
			SOCKET.addEventListener( ProgressEvent.PROGRESS, progressHandler );
			SOCKET.addEventListener( SecurityErrorEvent.SECURITY_ERROR, securityErrorHandler );
			
			SOCKET.connect( HOST, PORT );	
		}					
	}
	
	/**********************************************************
	 * ACTIVATE_DEBUG_MODE
	***********************************************************/
	
	private function activateDebugMode():void
	{
		var format:TextFormat = new TextFormat();
		DEBUG_TEXT = new TextField();
        format.font = 'Verdana';
     	format.color = 0xFFFFFF;
        format.size = 10;
        
		DEBUG_TEXT.defaultTextFormat = format;
		DEBUG_TEXT.autoSize = TextFieldAutoSize.LEFT;
		DEBUG_TEXT.background = true;	
		DEBUG_TEXT.backgroundColor = 0x000000;	
		DEBUG_TEXT.border = true;	
		DEBUG_TEXT.borderColor = 0x333333;	
		
		STAGE.addChild( DEBUG_TEXT );
	}

	/**********************************************************
	 * ACTIVATE_RECORD_MODE
	***********************************************************/
	
	private function activateRecordMode():void
	{
		RECORDED_XML = <OSCPackets></OSCPackets>;
			
		/*
		var record_btn:Sprite = new Sprite();
	
		record_btn.graphics.lineStyle( 2, 0x202020 );
		record_btn.graphics.beginFill( 0xF80101,0.5 );
		record_btn.graphics.drawRoundRect( 10, 10, 200, 200, 6 );				 
		record_btn.addEventListener( TUIOEvent.DOWN, stopRecording );
		
		STAGE.addChild( record_btn );
		*/
	}

	/**********************************************************
	 * TESTING_XML_ON_LOADED
	***********************************************************/
	
	private static function testingXmlOnLoaded( e:Event ):void 
	{
		TESTING_XML = new XML( e.target.data );
		
		STAGE.addEventListener( Event.ENTER_FRAME, updateFrame );
	}

	/**********************************************************
	 * UPDATE_FRAME
	***********************************************************/
	
	private static function updateFrame( e:Event ):void
	{
		if( TESTING_XML && TESTING_XML.OSCPACKET && TESTING_XML.OSCPACKET[ 0 ] )
		{			
			processXML( TESTING_XML.OSCPACKET[ 0 ] );
			delete TESTING_XML.OSCPACKET[ 0 ];
		}
	}		

	/**********************************************************
	 * GET_OBJECT_BY_ID
	***********************************************************/

	public static function getObjectById( id:int ):TUIOObject
	{
		var listAmount:int = objectArray.length;
		
		for( var i:int = 0; i < listAmount; i++ )
		{
			if( objectArray[ i ].ID == id )
			{
				return objectArray[ i ];
			}
		}	
		return null;
	}


	/**********************************************************
	 * EMULATE_MOUSE_EVENT
	***********************************************************/
	public static function emulateMouseEvent(target:InteractiveObject, type:String, x:Number, y:Number):void
	{
		trace("Emulating mouse event: " + type + " on " + target);
		if (target == null) 
			return;
			
		var p:Point = new Point(x, y);
		p = target.globalToLocal(p);
		
		var me:MouseEvent = new MouseEvent(MouseEvent.CLICK, false , false, p.x, p.y);
		target.dispatchEvent(me);
	}	

	private static function findTopObject(obj:DisplayObject):InteractiveObject
	{
		if (obj == null)
		{
			return null;
		}
		else if(obj is TUIOCursor)
		{
			return null;
		}
		else if (obj is InteractiveObject)
		{
			return InteractiveObject(obj);	
		}
		else if (obj.parent != null)
		{
			return findTopObject(obj.parent);
		}
		else
		{
			return null;
		}
		
	}


	/**********************************************************
	 * PROCESS_XML
	***********************************************************/

	public static function processXML( xml:XML ):void
	{
		// XML-Node
		var node:XML;
		
		// fseq
		var fseq:String;
		
		// list of TUIO-Objects
		var tuioObjList:Array;
		
		// the object at the bottom of the display chain (which the finger is currently over)
		var bottomObject:InteractiveObject;
		
		// TUIO-Object
		var tuioObj:Object;
		
		// type can be set / alive
		var type:String;
		
		// store positions
		var localPoint:Point;
		var stagePoint:Point
		
		// sessionID
		var sID:int;
		
		// classID or unique id for every touchEvent
		var id:int;
		
		// new x,y coordinates
		var x:Number;
		var y:Number;
		
		// old x,y coordinates
		var X:Number;
		var Y:Number;
		
		// rotation vector
		var a:Number;
		var A:Number;
		
		// motion acceleration
		var m:Number;
		
		// rotation accel
		var r:Number;
		
		for each( node in xml.MESSAGE )
		{
			// detect fseq
			
			if( node.ARGUMENT[0] && node.ARGUMENT[0].@VALUE == 'fseq' )
			{
				// in the moment not in use
				fseq = node.ARGUMENT[ 1 ].@VALUE;					
			}
			
			// detect alive status
			
			if( node.ARGUMENT[ 0 ] && node.ARGUMENT[ 0 ].@VALUE == 'alive' )
			{
				for each ( var obj:TUIOObject in objectArray )
				{
					obj.isAlive = false;
				}
													
				for each( var aliveItem:XML in node.ARGUMENT.( @VALUE != 'alive' ) )
				{
					if( getObjectById( aliveItem.@VALUE ) )
					{
						getObjectById( aliveItem.@VALUE ).isAlive = true;
					}	
				}   					
			}
			
			// detect new blobs / input
			
			if( node.ARGUMENT[ 0 ] )
			{
				stagePoint = new Point( x,y );
				tuioObjList = STAGE.stage.getObjectsUnderPoint( stagePoint );
				
				
				if (EMULATE_FLEX_MOUSE)
				{
					// List of object on stage, under the given finger
					// Take the bottom one and have it dispatch an event
					for (var j:String in tuioObjList)
					{
						var obj2:Object = tuioObjList[j];
						
						// For now we don't care about the cursor, but later on we might want to do something
						// with it when you hover over a button for instance... or do we?
						// Maybe make the other cursors transparent when dragging? 
						//(to indicate that you can only use one finger when dragging and using this method of mouse eventing the standard components)
						if (obj2 is TUIOCursor)// || obj2 is ApplicationBackground)
						{
							//continue;
						}
						//trace("Found object under point. Object" + j + "is: " + obj2 + " at " + obj2.x + ", " +obj2.y);
					}
					//trace("****************************");
					
					
					bottomObject = findTopObject( tuioObjList[tuioObjList.length-1] );
					
					//trace("Obj is: " + bottomObject);
					/*
					
					var bottomIndex:Number = -1;
					for (var k:Number = tuioObjList.length-1; k >= 0; k--)
					{
						trace("Looking for bottom object...");
						if ( tuioObjList[k] is InteractiveObject )
						{
							if (tuioObjList[k] is TUIOCursor)
							{
								// Don't do anything.
							}
							else
							{								
								trace("Bottom object has index: " + k.toString());
								bottomIndex = k;
								continue;
							}
						}
					}
					if (bottomIndex != -1)
					{
						trace("Found bottomObject!");
						
						bottomObject = InteractiveObject(tuioObjList[bottomIndex]);	
						trace("Dispatched event from: " + bottomObject);
					}
					else
					{
						//trace("Warning: No bottomObject found");
						bottomObject = null;
					}
					*/
				}
				
				if( node.@NAME == '/tuio/2Dobj' )
				{		
					
					type = node.ARGUMENT[0].@VALUE;				
					
					if( type == 'set' )
					{   
						var dobj:InteractiveObject = null;
						sID = node.ARGUMENT[ 1 ].@VALUE;
						id = node.ARGUMENT[ 2 ].@VALUE;
						x = Number( node.ARGUMENT[ 3 ].@VALUE ) * STAGE_WIDTH;
						y = Number( node.ARGUMENT[ 4 ].@VALUE ) * STAGE_HEIGHT;
						a = Number( node.ARGUMENT[ 5 ].@VALUE );
						X = Number( node.ARGUMENT[ 6 ].@VALUE );
						Y = Number( node.ARGUMENT[ 7 ].@VALUE );
						A = Number( node.ARGUMENT[ 8 ].@VALUE );
						m = node.ARGUMENT[ 9 ].@VALUE;
						r = node.ARGUMENT[ 10 ].@VALUE;
				
						tuioObj = getObjectById( id );
						if( tuioObj == null )
						{
							tuioObj = new TUIOObject('2Dobj', id, x, y, X, Y, objectArray.length, a );
							STAGE.addChild( tuioObj.spr );		
							//if (EMULATE_FLEX_MOUSE && bottomObject != null)
						    //emulateMouseEvent(bottomObject, MouseEvent.MOUSE_DOWN, x, y);						
							objectArray.push( tuioObj );
						} else {
							tuioObj.spr.x = x;
							tuioObj.spr.y = y;								
							tuioObj.x = x;
							tuioObj.y = y;
							tuioObj.dX = X;
							tuioObj.dY = Y;							
							tuioObj.setObjOver( dobj );
						}
					}
				} else if( node.@NAME == '/tuio/2Dcur' )
				{
			
					type = node.ARGUMENT[ 0 ].@VALUE;				
					if( type == 'set' )
					{	
						//var dobj:InteractiveObject = null;
						id = node.ARGUMENT[ 1 ].@VALUE;
						x = Number( node.ARGUMENT[ 2 ].@VALUE ) * STAGE_WIDTH;
						y = Number( node.ARGUMENT[ 3 ].@VALUE ) * STAGE_HEIGHT;
						X = Number( node.ARGUMENT[ 4 ].@VALUE );
						Y = Number( node.ARGUMENT[ 5 ].@VALUE );
						m = node.ARGUMENT[ 6 ].@VALUE;
						//a = node.ARGUMENT[ 7 ].@VALUE;		
						
						if (bottomObject != null)
							emulateMouseEvent(bottomObject, MouseEvent.CLICK, x, y);		
						trace('~');						

						tuioObj = getObjectById( id );
						if( tuioObj == null )
						{								
							tuioObj = new TUIOObject('2Dcur', id, x, y, X, Y, objectArray.length, 0 );
							//tuioObj.area = a;		
							STAGE.addChild( tuioObj.spr );															
							objectArray.push( tuioObj );
						} else {
							tuioObj.spr.x = x;
							tuioObj.spr.y = y;
							tuioObj.x = x;
							tuioObj.y = y;
							//tuioObj.area = a;								
							tuioObj.dX = X;
							tuioObj.dY = Y;								
							tuioObj.setObjOver( dobj );
							
							// TODO:
							/*
									To get roll over and roll out to work we need to tell the *last* object that the finger has rolled out
									This is done by comparing the bottomObject I presume. 
									the 'relatedObject' property of MouseEvent should get the new bottomObject on roll out.
									
									
							*/
							
							//if (EMULATE_FLEX_MOUSE && bottomObject != null)
							//emulateMouseEvent(bottomObject, MouseEvent.MOUSE_DOWN, x, y);
						}	
					}	
				}
			}
		}		
		if(DEBUG_MODE)
		{
			DEBUG_TEXT.text = '';
			DEBUG_TEXT.x = 2000;
			DEBUG_TEXT.y = 2000;
			for ( var i:int=0; i<objectArray.length; i++ )
			{
				if( objectArray[i].isAlive == false )
				{
					// TODO: cast objects
					objectArray[i].disposeObject();
					STAGE.removeChild(objectArray[i].spr);
					objectArray.splice(i, 1);
					i--;
	
				} else {
				//DEBUG DATA
			    if(DEBUG_MODE)
						DEBUG_TEXT.appendText('  ' + (i+1) + ' - ' + objectArray[i].ID + '  X: ' + int(objectArray[i].x) + '  Y: ' + int(objectArray[i].y) +  '   \n' );
						DEBUG_TEXT.y = STAGE_HEIGHT-300;
						DEBUG_TEXT.x = STAGE_WIDTH-250;				
				}
			}
		}
	}

	/**********************************************************
	 * STOP_RECORDING
	***********************************************************/
	
	private static function stopRecording( e:TUIOEvent ):void
	{		
		RECORD_MODE = false;
		DEBUG_MODE = false;
		trace("Dump the XML");
		//System.setClipboard( RECORDED_XML.toString() );
	}

	/**********************************************************
	 * SOCKET_SERVER - CLODE_HANDLER
	***********************************************************/
	
	private static function closeHandler( e:Event ):void 
	{
	}

	/**********************************************************
	 * SOCKET_SERVER - CONNECT_HANDLER
	***********************************************************/
	
	private static function connectHandler( e:Event ):void 
	{
		trace( 'TUIO Socket Enabled:' + e );
	}

	/**********************************************************
	 * SOCKET_SERVER - DATA_HANDLER
	***********************************************************/
	
	private static function dataHandler( e:DataEvent ):void 
	{
		if( RECORD_MODE )
		{
			RECORDED_XML.appendChild( XML( e.data ) );
		}			
		
		processXML( XML( e.data ) );
	}

	/**********************************************************
	 * SOCKET_SERVER - IO_ERROR_HANDLER
	***********************************************************/
	
	private static function ioErrorHandler( e:IOErrorEvent ):void 
	{
		trace( 'ioErrorHandler:' + e );
	}
	
	/**********************************************************
	 * SOCKET_SERVER - PROGRESS_HANDLER
	***********************************************************/
	
	private static function progressHandler( e:ProgressEvent ):void 
	{
	    //trace( 'progressHandler loaded:' + e.bytesLoaded + ' total: ' + e.bytesTotal );
	}

	/**********************************************************
	 * SOCKET_SERVER - SECURITY_ERROR_HANDLER
	***********************************************************/
	
	private static function securityErrorHandler( e:SecurityErrorEvent ):void 
	{		
	}
	
	/**********************************************************
	 * TUIO
	***********************************************************/
	
	public function TUIO( caller:Function ):void
	{
		if( caller == TUIO.init )
		{
			
		}
		else
		{
			throw new Error( 'TUIO is a singleton, use TUIO.init' );
		}
	}
}
}