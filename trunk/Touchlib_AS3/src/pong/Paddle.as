package pong
{
	/**
	 * A paddle used to hit the ball
	 */
	public class Paddle extends RectangularMovieClip implements Bouncer
	{
		/** Top bound of movement */
		static var topBound:Number;
		
		/** Bottom bound of movement */
		static var bottomBound:Number;
		
		/** Maximum amount the ball can bounce off in the Y direction */
		private static var maxBounceVYSkew:Number = 12;
		
		/**
		 * Request that the paddle be moved on the Y. This request will be capped to
		 * the bounds of the paddle (see topBound and bottomBound properties).
		 * @param y Requested Y to move to
		 */
		function requestMove(y:Number):void
		{
			this.y = Math.max(Paddle.topBound, Math.min(Paddle.bottomBound, y));
		}
		
		/**
		 * Bounce a ball off of the paddle.
		 * @param ball Ball to bounce
		 */
		public function bounce(ball:Ball):void
		{
			// Reverse X direction and bounce horizontally
			ball.vX = -ball.vX;
			ball.x += ball.vX;
			
			// Calculate the relative position where the ball hit from the paddle's center
			var hitPos:Number = ball.y - (this.y+(this.height/2)-ball.height);
			
			// Calculate the percentage from the paddle's center that the ball hit
			var percentFromCenter:Number = hitPos/this.height/2;
			
			// Compute the amount to perturb the ball's VY
			var vyAdjust:Number = percentFromCenter * Paddle.maxBounceVYSkew;
			
			// Perturb VY based on the hit
			ball.vY += vyAdjust
		}
	}
}