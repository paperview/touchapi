package pong
{
	import flash.display.MovieClip;
	import flash.text.TextField;
	import flash.text.TextFormat;
	import flash.text.TextFieldAutoSize;
	
	/**
	 * An add for the developer of this application
	 */
	public class Ad extends MovieClip
	{
		/**
		 * Construct by making a TextField with the advertisement as a child
		 */
		function Ad()
		{
			var textField:TextField = new TextField();
			textField.autoSize = TextFieldAutoSize.LEFT;
			textField.htmlText = "~wong";
			addChild(textField);
			
			var format:TextFormat = new TextFormat();
			format.font = "_typewriter";
			format.size = 16;
			format.color = 0xffffff;
			textField.setTextFormat(format);
		}
	}
}