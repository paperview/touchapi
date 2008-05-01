package nui.surface
{
	public class InactiveItems extends ItemsContainer
	{
		public function InactiveItems()
		{
			//TODO: implement function
			super();
			
			
			
			init();
			
		}
		
		private function init():void{
			
			var a:Item = new Item();
			a.x = Math.random()*800;//stage.stageWidth;
			a.y = Math.random()*600;//stage.stageHeight;
			addChild(a);
			
			
		}
		
	}
}