package pong
{	
	/**
	 * A bouncer, which bounces movie clips off of it
	 */
	interface Bouncer
	{
		/**
		 * Bounce a ball off of the bouncer
		 * @param ball Ball to bounce
		 */
		function bounce(ball:Ball):void;
	}
}