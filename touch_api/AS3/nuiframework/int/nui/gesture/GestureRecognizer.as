package nui.gesture
{
	
	/**
	 * 
	 * original idea Mouse Gesture Recognition from http://www.bytearray.org/?p=91
	 * 
	 **/
	
	import flash.geom.Point;
	import flash.geom.Rectangle;
	
	public class GestureRecognizer
	{
		static private var _instance:GestureRecognizer;
		
		
		// ------------------------------------------------
		//
		// ---o static
		//
		// ------------------------------------------------

		public static const DEFAULT_NB_SECTORS:uint=8;		// Number of sectors
		public static const DEFAULT_TIME_STEP:uint=20;		// Capture interval in ms
		public static const DEFAULT_PRECISION:uint=8;		// Precision of catpure in pixels
		public static const DEFAULT_FIABILITY:uint=30;		// Default fiability level
		
		// ------------------------------------------------
		//
		// ---o properties
		//
		// ------------------------------------------------
		
		private var moves:Array;						// Mouse gestures
		private var lastPoint:Point;					// Last mouse point
//		private var mouseZone:InteractiveObject;		// Mouse zone
		private var captureDepth:uint;					// Current capture depth 
		private var gestures:Array;						// Gestures to match
		private var rect:Object;						// Rectangle zone
		private var points:Array;						// Mouse points 
	
//		protected var timer:Timer;						// Timer
		protected var sectorRad:Number;					// Angle of one sector		
		protected var anglesMap:Array;					// Angles map 
		
		
		public function GestureRecognizer(singletonEnforcer:SingletonEnforcer)
		{
			// Build the angles map
			buildAnglesMap();
			
			// Gesture Spots
			gestures=[];
			
			addGesture("A","71");
		}
		
		
		public static function getInstance():GestureRecognizer{
			if(GestureRecognizer._instance == null)
			{
				GestureRecognizer._instance = new GestureRecognizer( new SingletonEnforcer());
			}
			return GestureRecognizer._instance;
		}
		
		/**
		*	Add a gesture
		*/
		public function addGesture(o:*,gesture:String,matchHandler:Function=null):void{
			var g:Array=[];
			for (var i:uint=0;i<gesture.length;i++){
				g.push(gesture.charAt(i)=="." ? -1 : parseInt(gesture.charAt(i),16));				
			}
			gestures.push({datas:o,moves:g,match:matchHandler});	
		}
		
		
		public function detectGesture(inPoints:Array):void
		{
			
			initMatchGesture(inPoints);
			
			matchGesture();
		}
		
		
		
		private function initMatchGesture(inPoints:Array):void
		{
			moves=[];
			points=[];
			rect={	minx:Number.POSITIVE_INFINITY,
					maxx:Number.NEGATIVE_INFINITY,
					miny:Number.POSITIVE_INFINITY,
					maxy:Number.NEGATIVE_INFINITY};
			
			inPoints.reverse();
			
			lastPoint = new Point(inPoints[0].x * 800, inPoints[0].y * 600);
			
			var msx:Number;
			var msy:Number;
			var difx:Number;
			var dify:Number;
			var sqDist:Number;
			var sqPrec:Number=DEFAULT_PRECISION*DEFAULT_PRECISION;
			
			for ( var i:int=1; i < inPoints.length; i++)
			{
				msx = inPoints[i].x * 800;
				msy = inPoints[i].y * 600;
				
				difx = msx - lastPoint.x;
				dify = msy - lastPoint.y;
				
				sqDist = difx*difx + dify*dify;
				
				if (sqDist > sqPrec)
				{
					points.push(new Point(msx,msy));
					addMove(difx, dify);
					lastPoint.x = msx;
					lastPoint.y = msy;
					
					if (msx<rect.minx)rect.minx=msx;
					if (msx>rect.maxx)rect.maxx=msx;
					if (msy<rect.miny)rect.miny=msy;
					if (msy>rect.maxy)rect.maxy=msy;
				}
			}
		
		}
		
		
		/**
		*	Add a move 
		*/
		protected function addMove(dx:int,dy:int):void{
			var angle:Number=Math.atan2(dy,dx)+sectorRad/2;
			if (angle<0)angle+=Math.PI*2;
			var no:int=Math.floor(angle/(Math.PI*2)*100);
			moves.push(anglesMap[no]);
		}
		
		
		/**
		*	Build the angles map
		*/
		protected function buildAnglesMap():void{
			
			// Angle of one sector
			sectorRad=Math.PI*2/DEFAULT_NB_SECTORS;
			
			// map containing sectors no from 0 to PI*2
			anglesMap=[];
			
			// the precision is Math.PI*2/100
			var step:Number=Math.PI*2/100;
						
			// memorize sectors
			var sector:Number;
			for (var i:Number=-sectorRad/2;i<=Math.PI*2-sectorRad/2;i+=step){
				sector=Math.floor((i+sectorRad/2)/sectorRad);
				anglesMap.push(sector);
			}
		}
		
		
		/**
		*	Match the gesture
		*/
		protected function matchGesture():void{
			
			var bestCost:uint=1000000;
			var nbGestures:uint=gestures.length;
			var cost:uint;
			var gest:Array;
			var bestGesture:Object=null;
			var infos:Object={	points:points,
								moves:moves,
								lastPoint:lastPoint,
								rect:new Rectangle(	rect.minx,
													rect.miny,
													rect.maxx-rect.minx,
													rect.maxy-rect.miny)};

			for (var i:uint=0;i<nbGestures;i++){
				
				gest=gestures[i].moves;
				
				infos.datas=gestures[i].datas;
				
				cost=costLeven(gest,moves);
				
				if (cost<=DEFAULT_FIABILITY){
					if (gestures[i].match!=null){
						infos.cost=cost;
						cost=gestures[i].match(infos);
					}
					if (cost<bestCost){
						bestCost=cost;
						bestGesture=gestures[i];
					}
				}
			}
			
			
			if (bestGesture!=null){
				/*var evt:GestureEvent=new GestureEvent(GestureEvent.GESTURE_MATCH);
				evt.datas=bestGesture.datas;
				evt.fiability=bestCost;
				dispatchEvent(evt);*/
				trace(bestGesture.datas);
			}else{
				//dispatchEvent(new GestureEvent(GestureEvent.NO_MATCH));
			}
		}
		
		
		/**
		*	dif angle
		*/
		protected function difAngle(a:uint,b:uint):uint{
			var dif:uint=Math.abs(a-b);
			if (dif>DEFAULT_NB_SECTORS/2)dif=DEFAULT_NB_SECTORS-dif;
			return dif;
		}
		
		/**
		*	return a filled 2D table
		*/
		protected function fill2DTable(w:uint,h:uint,f:*):Array{
			var o:Array=new Array(w);
			for (var x:uint=0;x<w;x++){
				o[x]=new Array(h);
				for (var y:uint=0;y<h;y++)o[x][y]=f;
			}
			return o;
		}
		
		/**
		*	cost Levenshtein
		*/
		protected function costLeven(a:Array,b:Array):uint{
			
			// point
			if (a[0]==-1){
				return b.length==0 ? 0 : 100000;
			}
			
			// precalc difangles
			var d:Array=fill2DTable(a.length+1,b.length+1,0);
			var w:Array=d.slice();
			
			for (var x:uint=1;x<=a.length;x++){
				for (var y:uint=1;y<b.length;y++){
					d[x][y]=difAngle(a[x-1],b[y-1]);
				}
			}
			
			// max cost
			for (y=1;y<=b.length;y++)w[0][y]=100000;
			for (x=1;x<=a.length;x++)w[x][0]=100000;
			w[0][0]=0;
			
			// levensthein application
			var cost:uint=0;
			var pa:uint;
			var pb:uint;
			var pc:uint;
			
			for (x=1;x<=a.length;x++){
				for (y=1;y<b.length;y++){
					cost=d[x][y];
					pa=w[x-1][y]+cost;
					pb=w[x][y-1]+cost;
					pc=w[x-1][y-1]+cost;
					w[x][y]=Math.min(Math.min(pa,pb),pc)
				}
			}
			
			return w[x-1][y-1];
		}
		
		
	}
	
}
	class SingletonEnforcer{}