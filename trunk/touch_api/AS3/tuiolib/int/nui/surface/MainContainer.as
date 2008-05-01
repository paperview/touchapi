package nui.surface
{
	import flash.display.Sprite;
	import nui.tuio.TuioClient;
	import nui.tuio.TuioEvent;
	import flash.geom.Point;
	import flash.utils.Dictionary;

	public class MainContainer extends Sprite
	{
		private var _tuioClient:TuioClient;
		
		private var _activeItems:ActiveItems = new ActiveItems();
		private var _inactiveItems:InactiveItems = new InactiveItems();
		
		private var _cursors:Dictionary = new Dictionary();
		
		
		public function MainContainer()
		{
			//TODO: implement function
			super();
			
			addChild(_inactiveItems);
			addChild(_activeItems);
		}
		
		public function addTUIOListener(tuioClient:TuioClient):void{
			_tuioClient = tuioClient;
			_tuioClient.addEventListener(TuioEvent.ADD_TUIO_CUR, addTuioCur);
			_tuioClient.addEventListener(TuioEvent.REMOVE_TUIO_CUR, removeTuioCur);
			_tuioClient.addEventListener(TuioEvent.UPDATE_TUIO_CUR, updateTuioCur);
			_tuioClient.addEventListener(TuioEvent.REFRESH, refresh);
		}
		
		public function removeListener():void{
			_tuioClient.removeEventListener(TuioEvent.ADD_TUIO_CUR,addTuioCur);
			_tuioClient.removeEventListener(TuioEvent.REMOVE_TUIO_CUR,removeTuioCur);
			_tuioClient.removeEventListener(TuioEvent.UPDATE_TUIO_CUR, updateTuioCur);
			_tuioClient.removeEventListener(TuioEvent.REFRESH, refresh);
				
		}
		
		private function addTuioCur(event:TuioEvent):void{
			
			
		}
		
		private function removeTuioCur(event:TuioEvent):void{
			
			// TODO check there's an active item with that cursor
				var item:Item = _activeItems.itemHasCursor((event.data as uint));
				// if true remove cursor from Item
					if(item!=null)
					{
						//if after removing cursor from item there're no more cursor move it from active to inactive
						if(item.removeCursor((uint)(event.data))==0){
							_activeItems.removeItem(item);
							_inactiveItems.addItem(item);
							
							
							item.activeMe(false);
						}
					}
					else if(_cursors[event.data as uint])
					{
						_cursors[event.data as uint] = null;
						delete(_cursors[event.data as uint]);
					}
		}
		
		private function updateTuioCur(event:TuioEvent):void{
			
			// TODO check there's an active item with that cursor id
			var item:Item = _activeItems.itemHasCursor((event.data.s_id as uint));
				//if true update values
				if(item!=null)
				{
					item.updateCursor((event.data.s_id as uint),(event.data.x as Number),(event.data.y as Number), event.data.X as Number, event.data.Y as Number);
				}
				//else check "background" has a cursor before check for inactive items!!
				
				else if(_cursors[event.data.s_id as uint])
				{
					_cursors[event.data.s_id as uint] = event.data;
				}
				
				//else check there's an active item under point
				else {
					var objects:Array = _activeItems.getObjectsUnderPoint(new Point((event.data.x as Number)*800,(event.data.y as Number)*600));
					if( (objects.length > 0) && (objects[0] is Item) )
					{
						//if true add cursor to item and update values
						var item:Item = objects[0] as Item;
						item.updateCursor(event.data.s_id as uint, event.data.x as Number, event.data.y as Number, event.data.X as Number, event.data.Y as Number);	
					}
					else{
						//else check there's an inactive item under point
						objects = _inactiveItems.getObjectsUnderPoint(new Point((event.data.x as Number)*800, (event.data.y as Number)*600));
						if( (objects.length > 0) && (objects[0] is Item) )
						{
							//if true move item from inactive to active, add cursor to item and update values
							var item:Item = objects[0] as Item;
							_inactiveItems.removeItem(item);
							_activeItems.addItem(item);
							item.updateCursor(event.data.s_id as uint, event.data.x as Number, event.data.y as Number, event.data.X as Number, event.data.Y as Number);			
							
							
							item.activeMe(true);
						}
						// else there's no inactive item so cursor is pointing background
						else
						{
							_cursors[event.data.s_id as uint] = event.data;
						}
						
					}
					
					
					
						
				}	
			
		}
		
		private function refresh(event:TuioEvent):void{
			
			// call refresh function to all ACTIVE Items
			_activeItems.refresh();
		}
		
	}
}