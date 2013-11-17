/**
 * @author bhouston / http://exocortex.com
 * @author mrdoob / http://mrdoob.com/
 */

#include "resin/math/Triangle.h"

namespace RESIN {

Triangle::Triangle( const Vector3& a, const Vector3& b, const Vector3& c )
    : a(a), b(b), c(c)
{

}

Vector3 Triangle::normal(const Vector3& a, const Vector3& b, const Vector3& c)
{

	Vector3 v0;

    Vector3 result;

    result.subVectors( c, b );
    v0.subVectors( a, b );
    result.cross( v0 );

    float resultLengthSq = result.lengthSq();
    if( resultLengthSq > 0 ) {

        return result.multiplyScalar( 1.0f / sqrt( resultLengthSq ) );

    }

    return result.set( 0, 0, 0 );

}

// static/instance method to calculate barycoordinates
// based on: http://www.blackpawn.com/texts/pointinpoly/default.html
Vector3 Triangle::barycoordFromPoint( const Vector3& point,
        const Vector3& a, const Vector3& b, const Vector3& c )
{

    Vector3 v0, v1, v2;

    v0.subVectors( c, a );
    v1.subVectors( b, a );
    v2.subVectors( point, a );

    float dot00 = v0.dot( v0 );
    float dot01 = v0.dot( v1 );
    float dot02 = v0.dot( v2 );
    float dot11 = v1.dot( v1 );
    float dot12 = v1.dot( v2 );

    float denom = ( dot00 * dot11 - dot01 * dot01 );

    Vector3 result;

    // colinear or singular triangle
    if( denom == 0 ) {
        // arbitrary location outside of triangle?
        // not sure if this is the best idea, maybe should be returning undefined
        return result.set( -2.0f, -1.0f, -1.0f );
    }

    float invDenom = 1.0f / denom;
    float u = ( dot11 * dot02 - dot01 * dot12 ) * invDenom;
    float v = ( dot00 * dot12 - dot01 * dot02 ) * invDenom;

    // barycoordinates must always sum to 1
    return result.set( 1.0f - u - v, v, u );

}

bool Triangle::containsPoint(const Vector3& point,
        const Vector3& a, const Vector3& b, const Vector3& c ) 
{

    Vector3 result = Triangle::barycoordFromPoint( point, a, b, c );

    return ( result.x() >= 0 ) && ( result.y() >= 0 ) && ( ( result.x() + result.y() ) <= 1 );
}

Triangle& Triangle::set( const Vector3& a, const Vector3& b, const Vector3& c )
{

    this->a = a;
    this->b = a;
    this->c = a;

    return *this;

}

Triangle& Triangle::setFromPointsAndIndices ( const Vector3* points, int i0, int i1, int i2 )
{

    this->a = points[i0];
    this->b = points[i1];
    this->c = points[i2];

    return *this;

}

Triangle& Triangle::copy ( const Triangle& triangle )
{

    a.copy( triangle.a );
    b.copy( triangle.b );
    c.copy( triangle.c );

    return *this;

}

float Triangle::area() const
{

    Vector3 v0, v1;

    v0.subVectors( c, b );
    v1.subVectors( a, b );

    return v0.cross( v1 ).length() * 0.5f;

}

Vector3 Triangle::midpoint() const
{

    Vector3 result;
    return result.addVectors( a, b ).add( c ).multiplyScalar( 1.0f / 3.0f );

}

Vector3 Triangle::normal() const
{

    return Triangle::normal( a, b, c );

}

Plane Triangle::plane() const
{

    Plane result;

    return result.setFromCoplanarPoints( a, b, c );

}

Vector3 Triangle::barycoordFromPoint( const Vector3& point ) const
{

    return Triangle::barycoordFromPoint( point, a, b, c );

}

bool Triangle::containsPoint ( const Vector3& point ) const
{

    return Triangle::containsPoint( point, a, b, c );

}

bool Triangle::equals ( const Triangle& triangle ) const
{

    return triangle.a.equals( a ) && triangle.b.equals( b ) && triangle.c.equals( c );

}

// clone: function () {
// 
//     return new THREE.Triangle().copy( this );
// 
// }

}
