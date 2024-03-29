#ifndef _OFX_VEC4f
#define _OFX_VEC4f

#include "ofConstants.h"



class ofxVec4f : public ofPoint {


  public:

    float w;



    ofxVec4f( float _x=0.0f,
              float _y=0.0f,
              float _z=0.0f,
              float _w=0.0f )
    {
        x = _x;
        y = _y;
        z = _z;
        w = _w;
    }




    // Getters and Setters.
    //
    //
    void set( float _x, float _y, float _z, float _w ) {
        x = _x;
        y = _y;
        z = _z;
        w = _w;
    }

    void set( const ofxVec4f& vec ) {
        x = vec.x;
        y = vec.y;
        z = vec.z;
        w = vec.w;
    }

    float &operator[]( const int& i ) {
        switch(i) {
            case 0:  return x;
            case 1:  return y;
            case 2:  return z;
            case 3:  return w;
            default: return x;
        }
    }



    // Check similarity/equality.
    //
    //
    bool operator==( const ofxVec4f& vec ) {
        return (x == vec.x) && (y == vec.y) && (z == vec.z) && (w == vec.w);
    }

    bool operator!=( const ofxVec4f& vec ) {
        return (x != vec.x) || (y != vec.y) || (z != vec.z) || (w != vec.w);
    }

    bool match( const ofxVec4f& vec, float tollerance=0.0001) {
        return (fabs(x - vec.x) < tollerance)
            && (fabs(y - vec.y) < tollerance)
            && (fabs(z - vec.z) < tollerance)
            && (fabs(w - vec.w) < tollerance);
    }




    // Additions and Subtractions.
    //
    //
    ofxVec4f operator+( const ofxVec4f& vec ) const {
        return ofxVec4f( x+vec.x, y+vec.y, z+vec.z, w+vec.w);
    }

    ofxVec4f& operator+=( const ofxVec4f& vec ) {
        x += vec.x;
        y += vec.y;
        z += vec.z;
        w += vec.w;
        return *this;
    }

    ofxVec4f operator-( const float f ) const {
        return ofxVec4f( x-f, y-f, z-f, w-f );
    }

    ofxVec4f& operator-=( const float f ) {
        x -= f;
        y -= f;
        z -= f;
        w -= f;
        return *this;
    }

    ofxVec4f operator-( const ofxVec4f& vec ) const {
        return ofxVec4f( x-vec.x, y-vec.y, z-vec.z, w-vec.w );
    }

    ofxVec4f& operator-=( const ofxVec4f& vec ) {
        x -= vec.x;
        y -= vec.y;
        z -= vec.z;
        w -= vec.w;
        return *this;
    }

    ofxVec4f operator+( const float f ) const {
        return ofxVec4f( x+f, y+f, z+f, w+f );
    }

    ofxVec4f& operator+=( const float f ) {
        x += f;
        y += f;
        z += f;
        w += f;
        return *this;
    }

    ofxVec4f operator-() const {
        return ofxVec4f( -x, -y, -z, -w );
    }


    // Scalings
    //
    //
    ofxVec4f operator*( const ofxVec4f& vec ) const {
        return ofxVec4f( x*vec.x, y*vec.y, z*vec.z, w*vec.w );
    }

    ofxVec4f& operator*=( const ofxVec4f& vec ) {
        x *= vec.x;
        y *= vec.y;
        z *= vec.z;
        w *= vec.w;
        return *this;
    }

    ofxVec4f operator*( const float f ) const {
        return ofxVec4f( x*f, y*f, z*f, w*f );
    }

    ofxVec4f& operator*=( const float f ) {
        x *= f;
        y *= f;
        z *= f;
        w *= f;
        return *this;
    }

    ofxVec4f operator/( const ofxVec4f& vec ) const {
        return ofxVec4f( vec.x!=0 ? x/vec.x : x , vec.y!=0 ? y/vec.y : y, vec.z!=0 ? z/vec.z : z, vec.w!=0 ? w/vec.w : w  );
    }

    ofxVec4f& operator/=( const ofxVec4f& vec ) {
        vec.x!=0 ? x/vec.x : x;
        vec.y!=0 ? y/vec.y : y;
        vec.z!=0 ? z/vec.z : z;
        vec.w!=0 ? w/vec.w : w;
        return *this;
    }

    ofxVec4f operator/( const float f ) const {
        if(f == 0) return ofxVec4f(x, y, z, w);

        return ofxVec4f( x/f, y/f, z/f, w/f );
    }

    ofxVec4f& operator/=( const float f ) {
        if(f == 0)return *this;

        x /= f;
        y /= f;
        z /= f;
        w /= f;
        return *this;
    }

    ofxVec4f rescaled( const float length ) const {
        float l = (float)sqrt(x*x + y*y + z*z + w*w);
        if( l > 0 )
            return ofxVec4f( (x/l)*length, (y/l)*length,
                            (z/l)*length, (w/l)*length );
        else
            return ofxVec4f();
    }

    ofxVec4f& rescale( const float length ) {
        float l = (float)sqrt(x*x + y*y + z*z + w*w);
        if (l > 0) {
            x = (x/l)*length;
            y = (y/l)*length;
            z = (z/l)*length;
            w = (w/l)*length;
        }
        return *this;
    }



    // Normalization
    //
    //
    ofxVec4f normalized() const {
        float length = (float)sqrt(x*x + y*y + z*z + w*w);
        if( length > 0 ) {
            return ofxVec4f( x/length, y/length, z/length, w/length );
        } else {
            return ofxVec4f();
        }
    }

    ofxVec4f& normalize() {
        float lenght = (float)sqrt(x*x + y*y + z*z + w*w);
        if( lenght > 0 ) {
            x /= lenght;
            y /= lenght;
            z /= lenght;
            w /= lenght;
        }
        return *this;
	}



    // Limit length.
    //
    //
	 ofxVec4f limited(float max) const {
        float length = (float)sqrt(x*x + y*y + z*z + w*w);
		if( length > max && length > 0 ) {
            return ofxVec4f( (x/length)*max, (y/length)*max,
                            (z/length)*max, (w/length)*max );
		} else {
            return ofxVec4f( x, y, z, w );
        }
	}

    ofxVec4f& limit(float max) {
        float length = (float)sqrt(x*x + y*y + z*z + w*w);
		if( length > max && length > 0 ) {
            x = (x/length)*max;
            y = (y/length)*max;
            z = (z/length)*max;
            w = (w/length)*max;
        }
        return *this;
    }



    // Length
    //
    //
    float length() const {
        return (float)sqrt( x*x + y*y + z*z + w*w );
    }

    float lengthSquared() const {
        return (float)(x*x + y*y + z*z + w*w);
    }



    /**
    * Dot Product.
    */
    float dot( const ofxVec4f& vec ) const {
        return x*vec.x + y*vec.y + z*vec.z + w*vec.w;
    }

};




// Non-Member operators
//
//
static inline ofxVec4f operator+( float f, const ofxVec4f& vec ) {
    return ofxVec4f( f+vec.x, f+vec.y, f+vec.z, f+vec.w );
}

static inline ofxVec4f operator-( float f, const ofxVec4f& vec ) {
    return ofxVec4f( f-vec.x, f-vec.y, f-vec.z, f-vec.w );
}

static inline ofxVec4f operator*( float f, const ofxVec4f& vec ) {
    return ofxVec4f( f*vec.x, f*vec.y, f*vec.z, f*vec.w );
}

static inline ofxVec4f operator/( float f, const ofxVec4f& vec ) {
    return ofxVec4f( f/vec.x, f/vec.y, f/vec.z, f/vec.w);
}


#endif
