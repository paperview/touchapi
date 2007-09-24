package pong
{
	import flash.display.MovieClip;
	import flash.text.TextField;
	import flash.text.TextFormat;
	import flash.text.TextFieldAutoSize;
	
	/**
	 * Displays the game over information
	 */
	public class GameOver extends MovieClip
	{
		/** The text field displaying the game over message */
		private var gameOver:TextField;
		
		/** Format of the text field */
		private var format:TextFormat;
		
		/**
		 * Constructor. Creates a text field for the game over message.
		 * @param where Movie clip to create the score on
		 */
		function GameOver()
		{
			// Create the text field and go to the next score in the count
			this.gameOver = new TextField();
			this.gameOver.autoSize = TextFieldAutoSize.LEFT;
			addChild(this.gameOver);
			
			// Set up the format to use a big monospaced font
			this.format = new TextFormat();
			this.format.font = "_typewriter";
			this.format.size = 50;
			this.format.align = "center";
		}
		
		/**
		 * Set the message to display under "Game Over". This will update the display.
		 * @param msg Message to display
		 */
		function setMessage(msg:String):void
		{
			this.gameOver.text = "Game Over\n" + msg;
			this.gameOver.setTextFormat(this.format);
		}
	}
}