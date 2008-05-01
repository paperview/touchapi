package {
	import flash.display.Sprite;
	import flash.display.Stage;
	import nui.tuio.Tuio2DCurClient;
	import nui.tuio.events.Tuio2DCurEvent;
	import nui.tuio.events.TuioEvent;
	import flash.utils.Dictionary;
	
	import nui.surface.*;	
	import nui.ui.VirtualMouse;
	import flash.display.DisplayObjectContainer;
	import flash.display.Shape;
	import flash.display.MovieClip;
	import flash.events.MouseEvent;

	[SWF(width='1024', height='768', backgroundColor='0x000000', frameRate='33')]
	public class TestVirtualMouse extends Sprite
	{
		private var _tuioClient:Tuio2DCurClient;
		private var _virtualMouses:Dictionary = new Dictionary();
		
		private var _stage:MovieClip;
		private var _visualFeedBack:VisualFeedback;
		
		public function TestVirtualMouse()
		{
			_tuioClient = new Tuio2DCurClient();
			_tuioClient.addEventListener(Tuio2DCurEvent.ADD_TUIO_2D_CUR, addVirtualMouse);
			_tuioClient.addEventListener(Tuio2DCurEvent.REMOVE_TUIO_2D_CUR, removeVirtualMouse);
			_tuioClient.addEventListener(Tuio2DCurEvent.UPDATE_TUIO_2D_CUR, updateVirtualMouse);
			_tuioClient.addEventListener(TuioEvent.REFRESH, refresh);
			_tuioClient.connect();
			
			_stage = new MovieClip();
			//_stage.width = 1024;//root.stage.stageWidth;
			//_stage.height = 768;//root.stage.stageHeight;
			_stage.graphics.beginFill(0x00FF00,0);
			_stage.graphics.drawRect(0,0,root.stage.stageWidth,root.stage.stageHeight);
			_stage.graphics.endFill();
			
			initStage();
			
			addChild(_stage);
			_visualFeedBack = new VisualFeedback();
			_visualFeedBack.addTUIOListener(_tuioClient);
			addChild(_visualFeedBack);
		}
		
		private function addVirtualMouse(event:Tuio2DCurEvent):void
		{
			var vMouse:VirtualMouse = new VirtualMouse(_stage,event.data.x*this.root.stage.stageWidth, event.data.y*this.root.stage.stageWidth);
			_virtualMouses[event.data.s] = vMouse;
			
		}
		
		private function removeVirtualMouse(event:Tuio2DCurEvent) : void
		{
			var vm:VirtualMouse = _virtualMouses[event.data.s] as VirtualMouse;
			vm.setLocation(-1,-1);
			_virtualMouses[event.data.s] = null;
			delete(_virtualMouses[event.data.s]);
		}
		
		private function updateVirtualMouse(event:Tuio2DCurEvent) : void
		{
			var _x:Number = event.data.x*this.root.stage.stageWidth;
			var _y:Number = event.data.y*this.root.stage.stageHeight;
			(_virtualMouses[event.data.s] as VirtualMouse).setLocation(_x,_y);
		}
		
		private function refresh(event:TuioEvent) : void
		{
			
		}
		private function traceEvent(event:MouseEvent):void
		{
		//	trace(event.target.name);
			trace(event.type);
		}
		private function initStage():void
		{
			for (var i:int; i<10; i++)
			{
				var item:Item = new Item();
				item.x = Math.random()*_stage.width;
				item.y = Math.random()*_stage.height-200;
				item.rotation = Math.random()*180 - 90;
				item.addEventListener(MouseEvent.CLICK, traceEvent);	
				item.addEventListener(MouseEvent.DOUBLE_CLICK, traceEvent);	
				item.addEventListener(MouseEvent.MOUSE_DOWN, traceEvent);
				//item.addEventListener(MouseEvent.MOUSE_MOVE, traceEvent);		
				item.addEventListener(MouseEvent.MOUSE_UP, traceEvent);
				item.addEventListener(MouseEvent.MOUSE_DOWN, traceEvent);
				item.addEventListener(MouseEvent.MOUSE_OVER, traceEvent);
				item.addEventListener(MouseEvent.MOUSE_OUT, traceEvent);	
				item.name = "item_"+i;
				
				_stage.addChild(item);
			}
		}	
	}
}
