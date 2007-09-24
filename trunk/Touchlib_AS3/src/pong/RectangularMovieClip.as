package pong
{
	import flash.display.MovieClip;
	
	/**
	 * A movie clip that is represented by a rectangle
	 */
	public class RectangularMovieClip extends MovieClip
	{
		/**
		 * Setup the rectangular movie clip with given dimensions
		 * @param width Width of the wall
		 * @param height Height of the number
		 */
		function setDimensions(width:Number, height:Number)
		{
			// Be a rectangle
			this.graphics.beginFill(0x000000, 1.0);
			this.graphics.drawRect(0, 0, width, height);
			this.graphics.endFill();
		}
	}
}