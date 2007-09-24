package app
{
	import flash.display.MovieClip;

	public class BrushObject extends MovieClip
	{
		
		public function BrushObject()
		{
			graphics.beginFill(0xFFFFFF , 1);					
			graphics.drawCircle(0,0,15);
			graphics.endFill();
		}
	}
}