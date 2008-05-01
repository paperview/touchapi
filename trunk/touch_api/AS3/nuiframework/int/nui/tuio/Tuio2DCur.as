package nui.tuio
{
	/**
	 * 
	 * @author Ignacio Delgado
	 * @see http://code.google.com/p/tuio-as3-lib
	 * @see http://mtg.upf.es/reactable/?software
	 * 
	 */
	public class Tuio2DCur extends AbstractTuio
	{
		/**
		* x position
		*/
		public var x:Number;
		
	    /**
	    *  y position
	    */
	    public var y:Number;
	    
	    /**
	    * x component of the movement vector (motion speed & direction)
	    */
	    public var X:Number;
	    
	    /**
	    * y component of the movement vector (motion speed & direction)
	    */
	    public var Y:Number;
	    
	    /**
	    * motion acceleration
	    */
	    public var m:Number;
	    
	    /**
	     * Creates a new Tuio2DCur data unit.
	     * 
	     * @param s @copy AbstractTuio#s
	     * @param x @copy #x
	     * @param y @copy #y
	     * @param X @copy #X
	     * @param Y @copy #Y
	     * @param m @copy #m
	     * 
	     */
	    public function Tuio2DCur(s:int, x:Number, y:Number, X:Number, Y:Number, m:Number)
	    {
	    	this.s = s;
	    	this.x = x;
	    	this.y = y;
	    	this.X = X;
	    	this.Y = Y;
	    	this.m = m;
	    }
	    
	    /**
	     * Updates the Tuio2DCur data.
	     * 
	     * @param x @copy #x
	     * @param y @copy #y
	     * @param X @copy #X
	     * @param Y @copy #Y
	     * @param m @copy #m
	     * 
	     */
	    public function update(x:Number, y:Number, X:Number, Y:Number, m:Number) : void
	    {
	    	this.x = x;
	    	this.y = y;
	    	this.X = X;
	    	this.Y = Y;
	    	this.m = m;
	    }
	}
}