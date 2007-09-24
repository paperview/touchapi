// FIXME: need velocity

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
		private static var FLOSCSocket:XMLSocket;
		private static var thestage:Sprite;
		private static var objectArray:Array;
		private static var idArray:Array;
		
		public static var debugMode:Boolean;		
		private static var debug_txt:TextField;			
		public	static var recordedXML:XML;
		private	static var bRecording:Boolean;
		private	static var xmlPlaybackURL:String;  
		private	static var xmlPlaybackLoader:URLLoader;
		private	static var playbackXML:XML;
		
		private	static var stagewidth:int;
		private	static var stageheight:int;
		
		private	static var bInitialized:Boolean = false;


		public static function init (s:Sprite, host:String, port:Number, wd:int, ht:int, nXML:String, dbug:Boolean = false):void
		{	
		  	xmlPlaybackURL = nXML; 
			if(bInitialized)
				return;
		  	
		  	bInitialized = true;
			
			debugMode = dbug;
			bRecording = dbug;
			//trace(dbug);
		
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
			debug_txt = new TextField();
			if(debugMode)
			{

				var format:TextFormat = new TextFormat();
				format.font = "Verdana";
     			format.color = 0xFFFFFF;
        		format.size = 10;
        
				debug_txt.defaultTextFormat = format;
				debug_txt.autoSize = TextFieldAutoSize.LEFT;
				debug_txt.background = true;	
				debug_txt.backgroundColor = 0x000000;	
				debug_txt.border = true;	
				debug_txt.borderColor = 0x333333;	
				thestage.addChild( debug_txt );		
				//thestage.addChildAt(debug_txt, 1);	
				//thestage.setChildIndex(debug_txt,1);
				
				recordedXML = <OSCPackets></OSCPackets>;				
				
				var debug_btn:Sprite = new Sprite();
				debug_btn.graphics.beginFill(0xF80101,0.5);
				debug_btn.graphics.drawRoundRect(0, 35, 100, 100,10);				 
				debug_btn.addEventListener(TUIOEvent.DownEvent, stopRecording);				 
				thestage.addChildAt(debug_btn,1);
				//thestage.addChild(debug_btn);
				//thestage.setChildIndex(debug_btn,1);
				
				var cursor_btn:Sprite = new Sprite();
				cursor_btn.graphics.beginFill(0xFFFFFF,0.5);
				cursor_btn.graphics.drawRoundRect(0, 145, 100, 100,10);				 
				cursor_btn.addEventListener(TUIOEvent.DownEvent, DebugToggle);				 
				thestage.addChild(cursor_btn);
				//thestage.setChildIndex(cursor_btn,1);
				
				
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
				debug_txt.text = "";
				debug_txt.y = -2000;
				debug_txt.x = -2000;		
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
						debug_txt.appendText("  " + (i + 1) +" - " +objectArray[i].ID + "  X:" + int(objectArray[i].x) + "  Y:" + int(objectArray[i].y) + "  \n");
						debug_txt.x = stagewidth-160;
						debug_txt.y = 8;
				}
			}
		}
		

		
		private static function stopRecording(e:TUIOEvent):void
		{
			// show XML
			//bRecording = false;
			//debugMode = false;
			trace(recordedXML.toString());
			//debug_txt.text = recordedXML.toString();
			
		}
		private static function DebugToggle(e:TUIOEvent):void
			{
				if (debugMode) {
				debugMode = false; 		
				debug_txt.text = "";	
				trace("debug mode off");
				}
				else {debugMode = true;}	
					
			}
			
        private static function closeHandler(event:Event):void {
            //trace("closeHandler: " + event);
        }

        private static function connectHandler(event:Event):void {

            //trace("TUIO Socket Enabled: " + event);
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