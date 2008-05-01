package nui.surface
{
	import flash.display.Sprite;

	public class ItemsContainer extends Sprite
	{
		public function ItemsContainer()
		{
			//TODO: implement function
			super();
		}
		
		public function addItem(item:Item):void{
			addChild(item);
		}
		
		public function removeItem(item:Item):void{
			removeChild(item);	
		}
		
		public function itemHasCursor(s_id:uint):Item{
			
			for( var i:int = 0; i < numChildren; i++)
			{
				var item:Item = (Item)(getChildAt(i));
				if(item.hasCursor(s_id))
					return item;
			}
			
			return null;
		}
		
	}
}