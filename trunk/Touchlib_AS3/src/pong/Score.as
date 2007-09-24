package pong
{
	import flash.display.MovieClip;
	import flash.text.TextField;
	import flash.text.TextFormat;
	import flash.text.TextFieldAutoSize;
	
	/**
	 * Represents the match score
	 */
	public class Score extends MovieClip
	{
		/** The text field displaying the score */
		private var score:TextField;
		
		/** Format of the text field */
		private var format:TextFormat;
	
		/**
		 * Constructor. Creates a text field and sets it up with the default score of
		 * zero versus zero.
		 */
		function Score()
		{
			// Create the text field and go to the next score in the count
			this.score = new TextField();
			this.score.autoSize = TextFieldAutoSize.LEFT;
			addChild(this.score);
			
			// Set up the format to use a big monospaced font
			this.format = new TextFormat();
			this.format.font = "_typewriter";
			this.format.size = 75;
	
			// The default score is zero versus zero
			setScore(0, 0);
		}
		
		/**
		 * Set the score for both players. This will update the display and possibly
		 * change the score's dimensions.
		 * @param score1 Player one's score
		 * @param score2 Player two's score
		 */
		function setScore(score1:Number, score2:Number):void
		{
			this.score.text = String(score1 + "   " + score2);
			this.score.setTextFormat(this.format);
		}
		
		/**
		 * Get the displayed scores
		 * @return The displayed scores, an Array of two Numbers
		 */
		function getScores():Array
		{
			// Split the string by the delimiter to get the two scores
			var scores:Array = this.score.text.split("   ");
			
			// Convert the score strings into Numbers
			for (var i:String in scores)
			{
				scores[i] = Number(scores[i]);
			}
			
			return scores;
		}
	}
}