package pong
{
	/**
	 * A paddle that is controlled by AI
	 */
	public class AIPaddle extends Paddle
	{
		/** The ball we try to hit */
		var ball:Ball;
		
		/** Maximum distance the paddle can move per update */
		private var maxSpeed:Number = 9;
		
		/**
		 * Update by trying to hit the ball. We try to hit the ball at a random spot
		 * on the paddle.
		 */
		function update():void
		{
			// Compute a random spot we'd like to hit the ball at
			var hitSpot:Number = Math.random() % this.height;
			
			// Compute the middle of the ball
			var ballMiddle:Number = ball.y + (ball.height/2);
	
			// Get the distance we'd like to move to hit the ball
			var ballDelta:Number = this.y + hitSpot - ballMiddle;
	
			// Compute the amount to move to hit the ball, or the maximum we can move
			// toward hitting it
			var moveAmount:Number = Math.min(Math.abs(ballDelta), this.maxSpeed);
			
			// The ball is below us, try to go up to hit it
			if (ballDelta < 0)
			{
				requestMove(this.y + moveAmount);
			}
			// The ball is above us, try to go down to hit it
			else
			{
				requestMove(this.y - moveAmount);
			}
		}
	}
}