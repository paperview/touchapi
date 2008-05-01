package nui.surface
{
	import flash.display.Sprite;
	import nui.tuio.Tuio2DCurClient;
	import nui.tuio.events.TuioEvent;
	import nui.tuio.events.Tuio2DCurEvent;
	import flash.utils.Dictionary;
	import flash.display.Shape;

	public class VisualFeedback extends Sprite
	{
		private var _tuioClient:Tuio2DCurClient;
		private var _cursors:Dictionary = new Dictionary();
		
		
		public function VisualFeedback()
		{
			//TODO: implement function
			super();
		}
		
		public function addTUIOListener(tuioClient:Tuio2DCurClient):void{
			_tuioClient = tuioClient;
			_tuioClient.addEventListener(Tuio2DCurEvent.ADD_TUIO_2D_CUR, addTuioCur);
			_tuioClient.addEventListener(Tuio2DCurEvent.REMOVE_TUIO_2D_CUR, removeTuioCur);
			_tuioClient.addEventListener(Tuio2DCurEvent.UPDATE_TUIO_2D_CUR, updateTuioCur);
		}
		
		public function removeListener():void{
			_tuioClient.removeEventListener(Tuio2DCurEvent.ADD_TUIO_2D_CUR,addTuioCur);
			_tuioClient.removeEventListener(Tuio2DCurEvent.REMOVE_TUIO_2D_CUR,removeTuioCur);
			_tuioClient.removeEventListener(Tuio2DCurEvent.UPDATE_TUIO_2D_CUR, updateTuioCur);	
		}
		
		
		private function addTuioCur(event:Tuio2DCurEvent):void{
			var a:Shape = new Shape();
		//	a.graphics.beginFill(0xFFFFFF , 0.5);					
		//	a.graphics.drawCircle(0,0,5);
		//	a.graphics.endFill();
		//	a.graphics.lineStyle(1, 0x000000, 1);	
		//	a.graphics.drawCircle(0,0,10);		
		//	a.graphics.drawCircle(0,0,11);		
			a.graphics.lineStyle(2, 0xFFFFFF, 1);			
			a.graphics.drawCircle(0,0,12);
			
			_cursors[event.data.s]=a;
			addChild(a);	
			
			this.blendMode = 'invert';
		}
		
		private function removeTuioCur(event:Tuio2DCurEvent):void{
			removeChild(_cursors[event.data.s]);
			_cursors[event.data.s]=null;
			delete(_cursors[event.data]);
		}
		
		private function updateTuioCur(event:Tuio2DCurEvent):void{
			var a:Shape;
			if(_cursors[event.data.s])
			{
				a = _cursors[event.data.s];
				a.x=event.data.x*stage.stageWidth;
				a.y=event.data.y*stage.stageHeight;
			}
			/*else
			{
				a = new Shape();
				a.graphics.beginFill(0xff0000);
				a.graphics.drawCircle(-5,-5,5);
				a.graphics.endFill();
				_cursors[event.data.s_id]=a;
				addChild(a);
			}*/
			
		}
		
	}
}