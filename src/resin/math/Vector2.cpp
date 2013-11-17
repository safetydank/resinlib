/**
 * @author mrdoob / http://mrdoob.com/
 * @author philogb / http://blog.thejit.org/
 * @author egraether / http://egraether.com/
 * @author zz85 / http://www.lab4games.net/zz85/blog
 */

#include "resin/math/Vector2.h"

using namespace glm;

namespace RESIN {

Vector2::Vector2( float x, float y ) : v(x, y)
{
}

Vector2::Vector2(const glm::vec2& vec) : v(vec)
{
}

float& Vector2::x()
{
    return v.x;
}

float& Vector2::y()
{
    return v.y;
}

const float& Vector2::x() const
{
    return v.x;
}

const float& Vector2::y() const
{
    return v.y;
}

Vector2& Vector2::set( float x, float y )
{

    v.x = x;
    v.y = y;

    return *this;

}

Vector2& Vector2::setX( float x )
{

    v.x = x;

    return *this;

}

Vector2& Vector2::setY( float y )
{

    v.y = y;

    return *this;

}


void Vector2::setComponent( int index, float value )
{

    switch ( index ) {

        case 0: v.x = value; break;
        case 1: v.y = value; break;
        default: // throw new Error( "index is out of range: " + index );
            break;

    }

}

float Vector2::getComponent( int index )
{

    switch ( index ) {

        case 0: return v.x;
        case 1: return v.y;
        default:
            return 0;
            // throw new Error( "index is out of range: " + index );
            break;
    }

}

Vector2& Vector2::copy( const Vector2& vec )
{

    v = vec.v;

    return *this;

}

Vector2& Vector2::add( const Vector2& vec )
{

    v += vec.v;

    return *this;

}

Vector2& Vector2::addVectors( const Vector2& a, const Vector2& b )
{

    v = a.v + b.v;

    return *this;

}

Vector2& Vector2::addScalar( float s )
{

    v.x += s;
    v.y += s;

    return *this;

}

Vector2& Vector2::sub( const Vector2& vec )
{

    v -= vec.v;

    return *this;

}

Vector2& Vector2::subVectors( const Vector2& a, const Vector2& b )
{

    v = a.v - b.v;

    return *this;

}

Vector2& Vector2::multiplyScalar( float s )
{

    v *= s;

    return *this;

}

Vector2& Vector2::divideScalar( float scalar )
{

    if ( scalar != 0 ) {

        float invScalar = 1.0f / scalar;

        v *= invScalar;

    } else {

        v.x = 0;
        v.y = 0;

    }

    return *this;

}

Vector2& Vector2::min( const Vector2& vec )
{

    if ( v.x > vec.v.x ) {

        v.x = vec.v.x;

    }

    if ( v.y > vec.v.y ) {

        v.y = vec.v.y;

    }

    return *this;

}

Vector2& Vector2::max( const Vector2& vec )
{

    if ( v.x < vec.v.x ) {

        v.x = vec.v.x;

    }

    if ( v.y < vec.v.y ) {

        v.y = vec.v.y;

    }

    return *this;

}

Vector2& Vector2::clamp( const Vector2& min, const Vector2& max )
{

    // This function assumes min < max, if this assumption isn't true it will not operate correctly

    if ( v.x < min.v.x ) {

        v.x = min.v.x;

    } else if ( v.x > max.v.x ) {

        v.x = max.v.x;

    }

    if ( v.y < min.v.y ) {

        v.y = min.v.y;

    } else if ( v.y > max.v.y ) {

        v.y = max.v.y;

    }

    return *this;

}

Vector2& Vector2::negate()
{

    return multiplyScalar( - 1 );

}

float Vector2::dot( const Vector2& vec )
{

    return v.x * vec.v.x + v.y * vec.v.y;

}

float Vector2::lengthSq()
{

    return v.x * v.x + v.y * v.y;

}

float Vector2::length()
{

    return sqrt( v.x * v.x + v.y * v.y );

}

Vector2& Vector2::normalize()
{

    return divideScalar( length() );

}

float Vector2::distanceTo( const Vector2& vec )
{

    return sqrt( distanceToSquared( vec ) );

}

float Vector2::distanceToSquared( const Vector2& vec )
{

    float dx = v.x - vec.v.x, dy = v.y - vec.v.y;
    return dx * dx + dy * dy;

}

Vector2& Vector2::setLength( float l )
{

    float oldLength = length();

    if ( oldLength != 0 && l != oldLength ) {

        multiplyScalar( l / oldLength );
    }

    return *this;

}

Vector2& Vector2::lerp( const Vector2& vec, float alpha )
{

    v.x += ( vec.v.x - v.x ) * alpha;
    v.y += ( vec.v.y - v.y ) * alpha;

    return *this;

}

bool Vector2::equals( const Vector2& vec )
{

    return ( ( vec.v.x == v.x ) && ( vec.v.y == v.y ) );

}

Vector2& Vector2::fromArray( float* array )
{

    v.x = array[ 0 ];
    v.y = array[ 1 ];

    return *this;

}

void Vector2::toArray( float* array ) const
{

    array[0] = v.x;
    array[1] = v.y;

}

Vector2 Vector2::clone()
{

    return Vector2(v);

}

}
