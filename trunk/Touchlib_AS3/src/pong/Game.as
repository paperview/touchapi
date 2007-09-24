package pong
{
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.geom.Rectangle;
	
	public class Game extends MovieClip
	{
		/** The area in which the ball is in play */
		static var playArea:Rectangle;
		
		/** Game over display */
		private var gameOver:GameOver = new GameOver();
		
		/** Score of the game */
		private var scores:Score = new Score();
		
		/** The AI's paddle */
		private var aiPaddle:AIPaddle = new AIPaddle();
		
		/** The user's paddle */
		private var userPaddle:UserPaddle = new UserPaddle();
		
		/** The ball the players bounce back and forth */
		private var ball:Ball = new Ball();
		
		/** The top wall of the play area */
		private var wallTop:Wall = new Wall();
		
		/** The bottom wall of the play area */
		private var wallBottom:Wall = new Wall();
		
		/** The ad for the developer */
		private var ad:Ad = new Ad();
		
		/** The net in the middle of the play area */
		private var net:Net = new Net();
		
		/**
		 * Construct the game. This is done once at the start of the movie.
		 */
		function Game()
		{
			// The height of a wall
			var wallHeight:Number = 20;
			
			// Define the play area
			Game.playArea = new Rectangle(
				0,
				wallHeight,
				this.stage.stageWidth,
				this.stage.stageHeight - wallHeight
			);
			
			// Setup the graphical elements on the stage
			this.ball.setDimensions(20, 20);
			this.wallTop.setDimensions(this.stage.stageWidth, wallHeight);
			this.wallBottom.setDimensions(Game.playArea.width, wallHeight);
			this.userPaddle.setDimensions(15, 75);
			this.aiPaddle.setDimensions(15, 75);
			this.wallBottom.y = this.stage.stageHeight - this.wallBottom.height;
			this.scores.y = this.wallTop.y + this.wallTop.height;
			ad.x = (this.stage.stageWidth - ad.width) / 2;
			ad.y = this.stage.stageHeight - ad.height;
			this.userPaddle.x = 18;
			this.userPaddle.y = 161.9;
			this.aiPaddle.x = 507;
			this.aiPaddle.y = 162.5;
			this.net.setup(Game.playArea.height*0.85, 7, 4);
			this.net.x = this.stage.stageWidth / 2;
			this.net.y = Game.playArea.y + ((Game.playArea.height - this.net.height) / 2);
			
			// Add all the graphical elements to the stage
			addChild(this.scores);
			addChild(this.gameOver);
			addChild(this.aiPaddle);
			addChild(this.ball);
			addChild(this.userPaddle);
			addChild(this.wallTop);
			addChild(this.wallBottom);
			addChild(this.ad);
			addChild(this.net);

			// Bound the paddles top to the bottom of the top wall and bottom to the top of
			// the bottom wall
			Paddle.topBound = this.wallTop.y + this.wallTop.height;
			Paddle.bottomBound = this.wallBottom.y - this.userPaddle.height;
			
			// Inform the AI paddle about the ball so it can try to hit it
			this.aiPaddle.ball = this.ball;

			// Set up the ball to bounce off the paddles and walls
			this.ball.bouncers = [
				this.userPaddle,
				this.aiPaddle,
				this.wallTop,
				this.wallBottom
			];
			
			// Reset the game to initialize it
			reset(true);
			
			// Lock the player's paddle to the mouse to give them control
			this.userPaddle.lockToMouse();
			
			// Update the game every time a frame is played
			addEventListener(Event.ENTER_FRAME, update);
		}
		
		/**
		 * Reset the game. The game should be initialized first.
		 * @param clearScores If scores should be cleared as well
		 */
		private function reset(clearScores:Boolean):void
		{
			// Hide the game over
			this.gameOver.visible = false;

			// Set the ball's initial position to be the center of the stage
			this.ball.x = (this.stage.stageWidth - this.ball.width)/2;
			this.ball.y = (this.stage.stageHeight - this.ball.height)/2;
			
			// Serve the ball by giving it a random initial velocity
			this.ball.vX = (Math.random() % 2 == 0 ? 1 : -1) * 10;
			this.ball.vY = Math.random() % 5;
			
			// Reset the scores
			if (clearScores)
			{
				setGameScore(0, 0);
			}
		}
	
		/**
		 * End the game and wait for the user to restart it
		 * @param msg Message to display to the player while the game is over
		 */
		private function endGame(msg:String):void
		{
			// Show the requested message
			this.gameOver.setMessage(msg);
			this.gameOver.x = (this.stage.stageWidth - this.gameOver.width) / 2;
			this.gameOver.y = (this.stage.stageHeight - this.gameOver.height) / 2;
			this.gameOver.visible = true;
			
			// Stop allowing the player to control the paddle
			this.userPaddle.unlockFromMouse();
			
			// Stop the ball
			this.ball.vX = this.ball.vY = 0;
			
			// Listen for the user pressing the mouse. When they do, reset and play again
			this.stage.addEventListener(MouseEvent.CLICK, gameOverMouseListener);
			
			// Wait for the user to press the mouse
			removeEventListener(Event.ENTER_FRAME, update);
		}
		
		/**
		 * Listen for mouse clicks on the game over screen
		 * @param event Mouse click event to respond to
		 */
		private function gameOverMouseListener(event:MouseEvent):void
		{
			// Reset the game and stop listening for the user to do so again
			reset(true);
			this.stage.removeEventListener(MouseEvent.CLICK, gameOverMouseListener);

			// Play the game again
			this.userPaddle.lockToMouse();
			addEventListener(Event.ENTER_FRAME, update);
		}
		
		/**
		 * Update the game
		 * @param event Event we are updating for
		 */
		private function update(event:Event):void
		{
			// Update the AI's paddle
			this.aiPaddle.update();
			
			// Update the ball's movement and check for the game over condition
			var winnerIndex:Number = this.ball.update();
			switch (winnerIndex)
			{
				case 0:
				case 1:
					// Award a point to the player who scored it
					var oldScores:Array = this.scores.getScores();
					oldScores[winnerIndex]++;
					setGameScore(oldScores[0], oldScores[1]);
			
					// If the player who scored won, end the game
					var newScores:Array = this.scores.getScores();
					if (newScores[winnerIndex] == 5)
					{
						endGame(winnerIndex == 0 ? "You Win!" : "You Lose");
					}
					// Otherwise, start a new round after a little bit
					else
					{
						reset(false);
					}
					break;
			}
			
			// If the game is not over, keep running the game by going to our own frame
			// to form the main loop
			gotoAndPlay(1);
		}
		
		/**
		 * Set the score of the game and update the score display
		 * @param player The player's score
		 * @param ai The AI's score
		 */
		private function setGameScore(player:Number, ai:Number)
		{
			this.scores.setScore(player, ai);
			this.scores.x = (this.stage.stageWidth - this.scores.width) / 2;
		}
	}
}