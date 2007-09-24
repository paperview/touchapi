package pong
{
	/**
	 * The ball the two paddles bounce back and forth
	 */
	public class Ball extends RectangularMovieClip
	{
		/** Velocity in the X direction */
		var vX:Number = 0;
		
		/** Velocity in the Y direction */
		var vY:Number = 0;
		
		/** Bouncers the ball collides with (type: Bouncer) */
		var bouncers:Array;
		
		/**
		 * Update the movement of the ball
		 * @return -1 if no player has won,
		 *         0 if the player has won,
		 *         1 if the AI has won
		 */
		function update():Number
		{
			// Update based on velocity
			this.x += vX;
			this.y += vY;
			
			// Check if we've gone off the left side (AI wins)
			if (this.x < Game.playArea.x)
			{
				return 1;
			}

			// Check if we've gone off the right side (player wins)
			if (this.x >= Game.playArea.x + Game.playArea.width)
			{
				return 0;
			}
			
			// Check if we hit any of the bouncers. If we did, have it bounce us.
			for (var it:String in this.bouncers)
			{
				var bouncer:Bouncer = this.bouncers[it];
	
				// Check if we hit the current bouncer
			/*	if (hitTestObject(bouncer))
				{
					// Bounce off the current bouncer and stop checking
					bouncer.bounce(this);
					break;
				}
			}
			*/
			return -1;
		}
	}
}