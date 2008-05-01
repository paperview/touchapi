package nui.surface
{
	public class ActiveItems extends ItemsContainer
	{
		import nui.surface.Item;
		
		public function ActiveItems()
		{
			//TODO: implement function
			super();
		}
		
		
		
		
				
		public function refresh():void{
			
			for (var i:Number = 0; i < numChildren; i++)
			{
				 ((Item)(getChildAt(i))).refresh();
			} 
		}
		
	}
}