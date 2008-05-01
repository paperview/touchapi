package nui.surface
{
	import flash.display.Sprite;
	import flash.events.MouseEvent;

	public class Item extends Sprite
	{
		public function Item()
		{
			graphics.lineStyle(1);
			graphics.beginFill(0xFFFFFF);
			graphics.drawRect(-200,-150,400,300);
			graphics.endFill();
								
			addEventListener(MouseEvent.MOUSE_OVER, mouseOver);
			addEventListener(MouseEvent.MOUSE_OUT, mouseOut);
		}
		
		private function mouseOver(event:MouseEvent) : void
		{
			alpha = 0.35;
		}
		
		private function mouseOut(event:MouseEvent) : void
		{
			alpha = 1;
		}
	}
}