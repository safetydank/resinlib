/**
 * @author supereggbert / http://www.paulbrunt.co.uk/
 * @author philogb / http://blog.thejit.org/
 * @author mikael emtinger / http://gomo.se/
 * @author egraether / http://egraether.com/
 * @author WestLangley / http://github.com/WestLangley
 */

#include "resin/math/Matrix4.h"
#include "resin/math/Quaternion.h"
#include "resin/math/Vector4.h"

namespace RESIN {

Vector4::Vector4( float x, float y, float z, float w )
{

    v.x = x;
    v.y = y;
    v.z = z;
    v.w = w;

}

Vector4::Vector4( const Vector4& vec ) : v(vec.v)
{
}

const float& Vector4::x() const
{
    return v.x;
}

const float& Vector4::y() const
{
    return v.y;
}

const float& Vector4::z() const
{
    return v.z;
}

const float& Vector4::w() const
{
    return v.w;
}

float& Vector4::x()
{
    return v.x;
}

float& Vector4::y()
{
    return v.y;
}

float& Vector4::z()
{
    return v.z;
}

float& Vector4::w()
{
    return v.w;
}

Vector4& Vector4::set( float x, float y, float z, float w )
{

    v.x = x;
    v.y = y;
    v.z = z;
    v.w = w;

    return *this;

}

Vector4& Vector4::setX( float x )
{

    v.x = x;

    return *this;

}

Vector4& Vector4::setY( float y )
{

    v.y = y;

    return *this;

}

Vector4& Vector4::setZ( float z )
{

    v.z = z;

    return *this;

}

Vector4& Vector4::setW( float w )
{

    v.w = w;

    return *this;

}

void Vector4::setComponent( int index, float value )
{

    switch ( index )
    {
        case 0: v.x = value; break;
        case 1: v.y = value; break;
        case 2: v.z = value; break;
        case 3: v.w = value; break;
        default: /* throw new Error( "index is out of range: " + index ) */ break;

    }

}

float Vector4::getComponent( int index ) const
{

    switch ( index )
    {
        case 0: return v.x;
        case 1: return v.y;
        case 2: return v.z;
        case 3: return v.w;
        default: /* throw new Error( "index is out of range: " + index ) */ break;

    }

    return 0;
}

Vector4& Vector4::copy( const Vector4& vec )
{

    v.x = vec.v.x;
    v.y = vec.v.y;
    v.z = vec.v.z;
    v.w = vec.v.w;

    return *this;

}

Vector4& Vector4::add( const Vector4& vec )
{
    v += vec.v;
    return *this;
}

Vector4& Vector4::addScalar( float s )
{
    v += s;
    return *this;

}

Vector4& Vector4::addVectors( const Vector4& a, const Vector4& b )
{
    v = a.v + b.v;
    return *this;

}

Vector4& Vector4::sub( const Vector4& vec )
{
    v -= vec.v;
    return *this;
}

Vector4& Vector4::subVectors( const Vector4& a, const Vector4& b )
{
    v = a.v - b.v;
    return *this;
}

Vector4& Vector4::multiplyScalar( float scalar )
{
    v *= scalar;
    return *this;
}

Vector4& Vector4::applyMatrix4( const Matrix4& m )
{
    v = m.m * v;
    //float x = v.x;
    //float y = v.y;
    //float z = v.z;
    //float w = v.w;

    //v.x = m[0] * x + m[4] * y + m[8] * z + m[12] * w;
    //v.y = m[1] * x + m[5] * y + m[9] * z + m[13] * w;
    //v.z = m[2] * x + m[6] * y + m[10] * z + m[14] * w;
    //v.w = m[3] * x + m[7] * y + m[11] * z + m[15] * w;

    return *this;
}

Vector4& Vector4::divideScalar( float scalar )
{
    if ( scalar != 0.0f ) {
        float invScalar = 1.0f / scalar;
        v *= invScalar;
    }
    else {
        v.x = 0;
        v.y = 0;
        v.z = 0;
        v.w = 1;
    }

    return *this;
}

Vector4& Vector4::setAxisAngleFromQuaternion( const Quaternion& q )
{
    // http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToAngle/index.htm

    // q is assumed to be normalized

    v.w = 2 * acos( q.w() );

    float s = sqrt( 1.0f - q.w() * q.w() );

    if ( s < 0.0001f ) {
        v.x = 1.0f;
        v.y = 0.0f;
        v.z = 0.0f;
    }
    else {
        v.x = q.x() / s;
        v.y = q.y() / s;
        v.z = q.z() / s;
    }

    return *this;
}

Vector4& Vector4::setAxisAngleFromRotationMatrix( const Matrix4& m )
{
//    // http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToAngle/index.htm
//
//    // assumes the upper 3x3 of m is a pure rotation matrix (i.e, unscaled)
//
//    float angle, x, y, z,     // variables for result
//        epsilon = 0.01,     // margin to allow for rounding errors
//        epsilon2 = 0.1;     // margin to distinguish between 0 and 180 degrees
//
//    te = m.elements,
//
//   m11 = te[0], m12 = te[4], m13 = te[8],
//   m21 = te[1], m22 = te[5], m23 = te[9],
//   m31 = te[2], m32 = te[6], m33 = te[10];
//
//    if ( ( Math.abs( m12 - m21 ) < epsilon )
//            && ( Math.abs( m13 - m31 ) < epsilon )
//            && ( Math.abs( m23 - m32 ) < epsilon ) )
//    {
//        // singularity found
//        // first check for identity matrix which must have +1 for all terms
//        // in leading diagonal and zero in other terms
//
//        if ( ( Math.abs( m12 + m21 ) < epsilon2 )
//                && ( Math.abs( m13 + m31 ) < epsilon2 )
//                && ( Math.abs( m23 + m32 ) < epsilon2 )
//                && ( Math.abs( m11 + m22 + m33 - 3 ) < epsilon2 ) )
//        {
//            // this singularity is identity matrix so angle = 0
//
//            v.set( 1, 0, 0, 0 );
//
//            return *this; // zero angle, arbitrary axis
//
//        }
//
//        // otherwise this singularity is angle = 180
//
//        angle = Math.PI;
//
//        var xx = ( m11 + 1 ) / 2;
//        var yy = ( m22 + 1 ) / 2;
//        var zz = ( m33 + 1 ) / 2;
//        var xy = ( m12 + m21 ) / 4;
//        var xz = ( m13 + m31 ) / 4;
//        var yz = ( m23 + m32 ) / 4;
//
//        if ( ( xx > yy ) && ( xx > zz ) ) { // m11 is the largest diagonal term
//
//            if ( xx < epsilon )
//            {
//                x = 0;
//                y = 0.707106781;
//                z = 0.707106781;
//
//            } else
//            {
//                x = Math.sqrt( xx );
//                y = xy / x;
//                z = xz / x;
//
//            }
//
//        } else if ( yy > zz ) { // m22 is the largest diagonal term
//
//            if ( yy < epsilon )
//            {
//                x = 0.707106781;
//                y = 0;
//                z = 0.707106781;
//
//            } else
//            {
//                y = Math.sqrt( yy );
//                x = xy / y;
//                z = yz / y;
//
//            }
//
//        } else { // m33 is the largest diagonal term so base result on this
//
//            if ( zz < epsilon )
//            {
//                x = 0.707106781;
//                y = 0.707106781;
//                z = 0;
//
//            } else
//            {
//                z = Math.sqrt( zz );
//                x = xz / z;
//                y = yz / z;
//
//            }
//
//        }
//
//        v.set( x, y, z, angle );
//
//        return *this; // return 180 deg rotation
//
//    }
//
//    // as we have reached here there are no singularities so we can handle normally
//
//    var s = Math.sqrt( ( m32 - m23 ) * ( m32 - m23 )
//            + ( m13 - m31 ) * ( m13 - m31 )
//            + ( m21 - m12 ) * ( m21 - m12 ) ); // used to normalize
//
//    if ( Math.abs( s ) < 0.001 ) s = 1;
//
//    // prevent divide by zero, should not happen if matrix is orthogonal and should be
//    // caught by singularity test above, but I've left it in just in case
//
//    v.x = ( m32 - m23 ) / s;
//    v.y = ( m13 - m31 ) / s;
//    v.z = ( m21 - m12 ) / s;
//    v.w = Math.acos( ( m11 + m22 + m33 - 1 ) / 2 );

    return *this;

}

Vector4& Vector4::min( const Vector4& vec )
{
    if ( v.x > vec.v.x ) {
        v.x = vec.v.x;
    }

    if ( v.y > vec.v.y ) {
        v.y = vec.v.y;
    }

    if ( v.z > vec.v.z ) {
        v.z = vec.v.z;
    }

    if ( v.w > vec.v.w ) {
        v.w = vec.v.w;
    }

    return *this;

}

Vector4& Vector4::max( const Vector4& vec )
{
    if ( v.x < vec.v.x ) {
        v.x = vec.v.x;
    }

    if ( v.y < vec.v.y ) {
        v.y = vec.v.y;
    }

    if ( v.z < vec.v.z ) {
        v.z = vec.v.z;
    }

    if ( v.w < vec.v.w ) {
        v.w = vec.v.w;
    }

    return *this;

}

Vector4& Vector4::clamp( const Vector4& min, const Vector4& max )
{
    // This function assumes min < max, if this assumption isn't true it will not operate correctly

    if ( v.x < min.v.x ) {
        v.x = min.v.x;
    }
    else if ( v.x > max.v.x ) {
        v.x = max.v.x;
    }

    if ( v.y < min.v.y ) {
        v.y = min.v.y;
    }
    else if ( v.y > max.v.y ) {
        v.y = max.v.y;
    }

    if ( v.z < min.v.z ) {
        v.z = min.v.z;
    }
    else if ( v.z > max.v.z ) {
        v.z = max.v.z;
    }

    if ( v.w < min.v.w ) {
        v.w = min.v.w;
    }
    else if ( v.w > max.v.w ) {
        v.w = max.v.w;
    }

    return *this;
}

Vector4& Vector4::negate()
{
    return multiplyScalar( -1.0f );
}

float Vector4::dot( const Vector4& vec ) const
{
    return glm::dot(v, vec.v);
}

float Vector4::lengthSq() const
{
    return v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w;
}

float Vector4::length() const
{
    return glm::length(v);
}

float Vector4::lengthManhattan() const
{
    return fabs( v.x ) + fabs( v.y ) + fabs( v.z ) + fabs( v.w );
}

Vector4& Vector4::normalize()
{
    v = glm::normalize(v);
    return *this;
}

Vector4& Vector4::setLength( float l )
{
    float oldLength = v.length();

    if ( oldLength != 0.0f && l != oldLength ) {
        multiplyScalar( l / oldLength );
    }

    return *this;
}

Vector4& Vector4::lerp( const Vector4& vec, float alpha )
{
    v = glm::mix(v, vec.v, alpha);

    return *this;
}

bool Vector4::equals( const Vector4& vec ) const
{
    return ( ( vec.v.x == v.x ) && ( vec.v.y == v.y ) && ( vec.v.z == v.z ) && ( vec.v.w == v.w ) );
}

// Vector4& Vector4::fromArray( array )
// {
//     v.x = array[ 0 ];
//     v.y = array[ 1 ];
//     v.z = array[ 2 ];
//     v.w = array[ 3 ];
// 
//     return *this;
// 
// }

// toArray()
// {
//     return [ v.x, v.y, v.z, v.w ];
// 
// }

// Vector4::clone()
// {
//     return new THREE.Vector4( v.x, v.y, v.z, v.w );
// }

}

