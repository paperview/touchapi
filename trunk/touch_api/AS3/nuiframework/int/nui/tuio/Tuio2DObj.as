package nui.tuio
{
	/**
	 * 
	 * @author Ignacio Delgado
	 * @see http://code.google.com/p/tuio-as3-lib
	 * @see http://mtg.upf.es/reactable/?software
	 * 
	 */
	public class Tuio2DObj extends AbstractTuio
	{
		/**
		* ClassID, fiducial ID number
		*/
		public var i:int;
		
		/**
		* @copy Tuio2DCur#x
		*/
		public var x:Number;
		
	    /**
	    *  @copy Tuio2DCur#y
	    */
	    public var y:Number;
	    
	    /**
	    * angle
	    */
	    public var a:Number;
	    
	    /**
	    * @copy Tuio2DCur#X
	    */
	    public var X:Number;
	    
	    /**
	    * @copy Tuio2DCur#Y
	    */
	    public var Y:Number;
	    
	    /**
	    * A component of the rotation vector (motion speed & direction)
	    */
	    public var A:Number;
	    
	    /**
	    * @copy Tuio2DCur#m
	    */
	    public var m:Number;
	    	    
		/**
		* rotation acceleration
		*/
		public var r:Number;
		
		/**
		 * Creates a new Tuio2DObj data unit.
		 * 
		 * @param s @copy AbstractTuio#s
		 * @param i @copy #i
		 * @param x @copy #x
		 * @param y @copy #y
		 * @param a @copy #a
		 * @param X @copy #X
		 * @param Y @copy #Y
		 * @param A @copy #A
		 * @param m @copy #m
		 * @param r @copy #r
		 * 
		 */
		public function Tuio2DObj(s:int, i:int, x:Number, y:Number, a:Number, X:Number, Y:Number, A:Number, m:Number, r:Number)
		{
			this.s = s;
			this.i = i;
	    	this.x = x;
	    	this.y = y;
	    	this.a = a;
	    	this.X = X;
	    	this.Y = Y;
	    	this.A = A;
	    	this.m = m;
			this.r = r;
		}
		
		/**
		 * Updates the tuio2DObj data.
		 * 
		 * @param x @copy #x
		 * @param y @copy #y
		 * @param a @copy #a
		 * @param X @copy #X
		 * @param Y @copy #Y
		 * @param A @copy #A
		 * @param m @copy #m
		 * @param r @copy #r
		 * 
		 */
		public function update(x:Number, y:Number, a:Number, X:Number, Y:Number, A:Number, m:Number, r:Number) : void
		{
			this.x = x;
	    	this.y = y;
	    	this.a = a;
	    	this.X = X;
	    	this.Y = Y;
	    	this.A = A;
	    	this.m = m;
			this.r = r;
		}
	}
}