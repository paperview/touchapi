package pong
{
	import flash.events.MouseEvent;
	
	/**
	 * A paddle that is controlled by the user
	 */
	public class UserPaddle extends Paddle
	{
		/**
		 * Lock the paddle's Y position to the mouse's Y position, within the bounds
		 * set for the paddle (see topBound and bottomBound properties).
		 */
		function lockToMouse():void
		{
			// Listen to mouse move events
			this.stage.addEventListener(MouseEvent.MOUSE_MOVE, mouseMoveListener);
		}
		
		/**
		 * Stop locking the paddle's Y position to the mouse's Y position
		 */
		function unlockFromMouse():void
		{
			this.stage.removeEventListener(MouseEvent.MOUSE_MOVE, mouseMoveListener);
		}
		
		/**
		 * Listener to mouse movements. Requests movement to sync with the mouse's Y.
		 * @param event The mouse move event
		 */
		function mouseMoveListener(event:MouseEvent)
		{
			requestMove(event.stageY-this.height/2);
		}
	}
}