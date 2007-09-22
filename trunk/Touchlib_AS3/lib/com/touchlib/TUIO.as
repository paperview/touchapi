﻿// FIXME: need velocity

package com.touchlib {
import flash.display.Sprite;
import flash.display.DisplayObject;
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



	public class TUIO
	{
		static var FLOSCSocket:XMLSocket;
		static var thestage:Sprite;
		static var objectArray:Array;
		static var idArray:Array;
		

		public static var debugMode:Boolean;		
		public static var DEBUG_TEXT:TextField;
		
		static var recordedXML:XML;
		static var bRecording:Boolean = true;
		static var xmlPlaybackURL:String;  
		static var xmlPlaybackLoader:URLLoader;
		static var playbackXML:XML;
		
		static var stagewidth:int;
		static var stageheight:int;
		
		static var bInitialized = false;


		public static function init (s:Sprite, host:String, port:Number, wd:int, ht:int, nXML:String, dbug:Boolean = true):void
		{
			xmlPlaybackURL = nXML; 
			if(bInitialized)
				return;
			debugMode = dbug;
			bInitialized = true;
			stagewidth = wd;
			stageheight = ht;
			thestage = s;
			objectArray = new Array();
			idArray = new Array();
			try
			{
			
			FLOSCSocket = new XMLSocket();

            FLOSCSocket.addEventListener(Event.CLOSE, closeHandler);
            FLOSCSocket.addEventListener(Event.CONNECT, connectHandler);
            FLOSCSocket.addEventListener(DataEvent.DATA, dataHandler);
            FLOSCSocket.addEventListener(IOErrorEvent.IO_ERROR, ioErrorHandler);
            FLOSCSocket.addEventListener(ProgressEvent.PROGRESS, progressHandler);
            FLOSCSocket.addEventListener(SecurityErrorEvent.SECURITY_ERROR, securityErrorHandler);

			FLOSCSocket.connect(host, port);			
			
			} catch (e:Event)
			{
			}
			DEBUG_TEXT = new TextField();
			if(debugMode)
			{
				var format:TextFormat = new TextFormat();
			
       			format.font = "Verdana";
     			format.color = 0xFFFFFF;
        		format.size = 10;
        
				DEBUG_TEXT.defaultTextFormat = format;
				DEBUG_TEXT.autoSize = TextFieldAutoSize.LEFT;
				DEBUG_TEXT.background = true;	
				DEBUG_TEXT.backgroundColor = 0x000000;	
				DEBUG_TEXT.border = true;	
				DEBUG_TEXT.borderColor = 0x333333;	
				thestage.addChild( DEBUG_TEXT );		
		
				recordedXML = <OSCPackets></OSCPackets>;				
				
				var debug_btn:Sprite = new Sprite();
				debug_btn.graphics.beginFill(0xF80101,0.5);
				debug_btn.graphics.drawRoundRect(10, 10, 100, 100,10);				 
				debug_btn.addEventListener(TUIOEvent.DownEvent, stopRecording);				 
				thestage.addChild(debug_btn);
				
				var cursor_btn:Sprite = new Sprite();
				cursor_btn.graphics.beginFill(0xFFFFFF,0.5);
				cursor_btn.graphics.drawRoundRect(10, 115, 100, 100,10);				 
				cursor_btn.addEventListener(TUIOEvent.DownEvent, DebugToggle);				 
				thestage.addChild(cursor_btn);
				 
				 if(xmlPlaybackURL != "")
				 {
					xmlPlaybackLoader = new URLLoader();
					xmlPlaybackLoader.addEventListener("complete", xmlPlaybackLoaded);
					xmlPlaybackLoader.load(new URLRequest(xmlPlaybackURL));
			
					thestage.addEventListener(Event.ENTER_FRAME, frameUpdate);
				 }
				
			} else {
				recordedXML = <OSCPackets></OSCPackets>;
				bRecording = false;
			}
			
		}
		
		private static function setDimensions(wd:int = 800, ht:int = 600):void
		{
			stagewidth = wd;
			stageheight = ht;			
		}
		
		
		

		private static function xmlPlaybackLoaded(evt:Event):void
		{
			trace("Loaded xml debug data");
			playbackXML = new XML(xmlPlaybackLoader.data);
		}
		
		private static function frameUpdate(evt:Event):void
		{
			if(playbackXML && playbackXML.OSCPACKET && playbackXML.OSCPACKET[0])
			{
				processMessage(playbackXML.OSCPACKET[0]);

				delete playbackXML.OSCPACKET[0];
			}
		}		
		
		public static function getObjectById(id:Number): TUIOObject
		{
			for(var i=0; i<objectArray.length; i++)
			{
				if(objectArray[i].ID == id)
				{
					//trace("found " + id);
					return objectArray[i];
				}
			}
			//trace("Notfound");
			
			return null;
		}
		
		public static function listenForObject(id:Number, reciever:Object):void
		{
			var tmpObj:TUIOObject = getObjectById(id);
			
			if(tmpObj)
			{
				tmpObj.addListener(reciever);				
			}

		}
		
		public static function processMessage(msg:XML):void
		{

			var fseq:String;
			var node:XML;
			for each(node in msg.MESSAGE)
			{
				if(node.ARGUMENT[0] && node.ARGUMENT[0].@VALUE == "fseq")
					fseq = node.ARGUMENT[1].@VALUE;					
			}
///			trace("fseq = " + fseq);

			for each(node in msg.MESSAGE)
			{
				if(node.ARGUMENT[0] && node.ARGUMENT[0].@VALUE == "alive")
				{
					for each (var obj1:TUIOObject in objectArray)
					{
						obj1.isAlive = false;
					}
					
					var newIdArray:Array = new Array();					
					for each(var aliveItem:XML in node.ARGUMENT.(@VALUE != "alive"))
					{
						if(getObjectById(aliveItem.@VALUE))
							getObjectById(aliveItem.@VALUE).isAlive = true;

					}   
					
					//trace(idArray);

					idArray = newIdArray;
				}

			}			
			
							
			for each(node in msg.MESSAGE)
			{
				if(node.ARGUMENT[0])
				{
					var type:String;
					
					if(node.@NAME == "/tuio/2Dobj")
					{
						type = node.ARGUMENT[0].@VALUE;				
						if(type == "set")
						{
							var sID:int = node.ARGUMENT[1].@VALUE;
							var id:int = node.ARGUMENT[2].@VALUE;
							var x:Number = Number(node.ARGUMENT[3].@VALUE) * stagewidth;
							var y:Number = Number(node.ARGUMENT[4].@VALUE) * stageheight;
							var a:Number = Number(node.ARGUMENT[5].@VALUE);
							var X:Number = Number(node.ARGUMENT[6].@VALUE);
							var Y:Number = Number(node.ARGUMENT[7].@VALUE);
							var A:Number = Number(node.ARGUMENT[8].@VALUE);
							var m:Number = node.ARGUMENT[9].@VALUE;
							var r:Number = node.ARGUMENT[10].@VALUE;
							
							// send object update event..
							
							var objArray:Array = thestage.stage.getObjectsUnderPoint(new Point(x, y));
							var stagePoint:Point = new Point(x,y);					
							var displayObjArray:Array = thestage.stage.getObjectsUnderPoint(stagePoint);							
							var dobj = null;
							
//							if(displayObjArray.length > 0)								
//								dobj = displayObjArray[displayObjArray.length-1];										

							
						
							var tuioobj = getObjectById(id);
							if(tuioobj == null)
							{
								tuioobj = new TUIOObject("2Dobj", id, x, y, X, Y, sID, a, dobj);
								thestage.addChild(tuioobj.spr);								
								objectArray.push(tuioobj);
								tuioobj.notifyCreated();								
							} else {
								tuioobj.spr.x = x;
								tuioobj.spr.y = y;								
								tuioobj.x = x;
								tuioobj.y = y;
								tuioobj.dX = X;
								tuioobj.dY = Y;
								
								tuioobj.setObjOver(dobj);
								tuioobj.notifyMoved();								
							}
							
							try
							{
								if(tuioobj.obj && tuioobj.obj.parent)
								{							
									
									var localPoint:Point = tuioobj.obj.parent.globalToLocal(stagePoint);							
									tuioobj.obj.dispatchEvent(new TUIOEvent(TUIOEvent.MoveEvent, true, false, x, y, localPoint.x, localPoint.y, tuioobj.obj, false,false,false, true, m, "2Dobj", id, sID, a));
								}
							} catch (e:Event)
							{
							}

		
						}
						
					} else if(node.@NAME == "/tuio/2Dcur")
					{
//						trace("2dcur");
						type = node.ARGUMENT[0].@VALUE;				
						if(type == "set")
						{
							var id:int = node.ARGUMENT[1].@VALUE;
							var x:Number = Number(node.ARGUMENT[2].@VALUE) * stagewidth;
							var y:Number = Number(node.ARGUMENT[3].@VALUE) * stageheight;
							var X:Number = Number(node.ARGUMENT[4].@VALUE);
							var Y:Number = Number(node.ARGUMENT[5].@VALUE);
							var m:Number = node.ARGUMENT[6].@VALUE;
							//var area = node.ARGUMENT[7].@VALUE;							
							
							var stagePoint:Point = new Point(x,y);					
							var displayObjArray:Array = thestage.stage.getObjectsUnderPoint(stagePoint);
							var dobj = null;
							if(displayObjArray.length > 0)								
								dobj = displayObjArray[displayObjArray.length-1];							
														
								
							//var sztmp:String="";
//							for(var i=0; i<displayObjArray.length; i++)
//								sztmp += (displayObjArray[i] is InteractiveObject) + ",";
//							trace(sztmp);

							var tuioobj = getObjectById(id);
							if(tuioobj == null)
							{
								tuioobj = new TUIOObject("2Dcur", id, x, y, X, Y, -1, 0, dobj);
								//tuioobj.area = area;
								thestage.addChild(tuioobj.spr);								
								objectArray.push(tuioobj);
								tuioobj.notifyCreated();
							} else {
								tuioobj.spr.x = x;
								tuioobj.spr.y = y;
								tuioobj.x = x;
								tuioobj.y = y;
								//tuioobj.area = area;								
								tuioobj.dX = X;
								tuioobj.dY = Y;
								
								tuioobj.setObjOver(dobj);
								tuioobj.notifyMoved();
							}

							try
							{
								if(tuioobj.obj && tuioobj.obj.parent)
								{							
									var localPoint:Point = tuioobj.obj.parent.globalToLocal(stagePoint);							
									tuioobj.obj.dispatchEvent(new TUIOEvent(TUIOEvent.MoveEvent, true, false, x, y, localPoint.x, localPoint.y, tuioobj.obj, false,false,false, true, m, "2Dobj", id, sID, a));
								}
							} catch (e:Event)
							{
								trace("Dispatch event failed " + tuioobj.name);
							}

	
						}
					}
				}
			}
			

			if(debugMode)
				DEBUG_TEXT.text = "";
				DEBUG_TEXT.y = -2000;
				DEBUG_TEXT.x = -2000;		
			for (var i:int=0; i<objectArray.length; i++ )
			{
				if(objectArray[i].isAlive == false)
				{
					objectArray[i].removeObject();
					
					thestage.removeChild(objectArray[i].spr);
					objectArray.splice(i, 1);
					i--;

				} else {
					if(debugMode)
						DEBUG_TEXT.appendText("  " + (i + 1) +" - " +objectArray[i].ID + "  X:" + int(objectArray[i].x) + "  Y:" + int(objectArray[i].y) + "  \n");
						DEBUG_TEXT.x = stagewidth-160;
						DEBUG_TEXT.y = 8;	
				}
			}
		}
		

		
		private static function stopRecording(e:TUIOEvent):void
		{
			// show XML
			//bRecording = false;
			//debugMode = false;
			//trace(recordedXML.toString());
			
		}
		private static function DebugToggle(e:TUIOEvent):void
			{
				if (debugMode) {
				debugMode = false; 
				trace("debug mode off");
				}
				else {debugMode = true;
				}				
			}
			
        private static function closeHandler(event:Event):void {
            //trace("closeHandler: " + event);
        }

        private static function connectHandler(event:Event):void {

            trace("TUIO Socket Enabled: " + event);
        }

        private static function dataHandler(event:DataEvent):void {
			
            //trace("dataHandler: " + event);
			
			if(bRecording)
				recordedXML.appendChild( XML(event.data) );
			
			processMessage(XML(event.data));
        }

        private static function ioErrorHandler(event:IOErrorEvent):void {
//			thestage.tfDebug.appendText("ioError: " + event + "\n");			
            trace("ioErrorHandler: " + event);
        }

        private static function progressHandler(event:ProgressEvent):void {
            //trace("progressHandler loaded:" + event.bytesLoaded + " total: " + event.bytesTotal);
        }
        private static function securityErrorHandler(event:SecurityErrorEvent):void {
            trace("securityErrorHandler: " + event);
//			thestage.tfDebug.appendText("securityError: " + event + "\n");			
        }	
		
		
	}
}