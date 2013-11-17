/**
 * @author bhouston / http://exocortex.com
 */

#include "resin/math/Line3.h"
#include "resin/math/Matrix3.h"
#include "resin/math/Matrix4.h"
#include "resin/math/Plane.h"
#include "resin/math/Sphere.h"

#include "glm/glm.hpp"

namespace RESIN {

Plane::Plane( const Vector3& normal, float constant ) : normal(normal), constant(constant)
{
}

Plane& Plane::set( const Vector3& normal, float constant )
{
    this->normal().copy( normal );
    this->constant() = constant;

    return *this;
}

Plane& Plane::setComponents( float x, float y, float z, float w )
{

    normal().set( x, y, z );
    constant() = w;

    return *this;

}

Plane& Plane::setFromNormalAndCoplanarPoint( const Vector3& normal, const Vector3& point )
{
    this->normal().copy( normal );
    constant() = - point.dot( this->normal() ); // must be normal(), not normal, as normal() is normalized

    return *this;
}

Plane& Plane::setFromCoplanarPoints(const Vector3& a, const Vector3& b, const Vector3& c)
{
    Vector3 v1;
    Vector3 v2;

    Vector3 normal = v1.subVectors( c, b ).cross( v2.subVectors( a, b ) ).normalize();

    // Q: should an error be thrown if normal is zero (e.g. degenerate plane)?

    setFromNormalAndCoplanarPoint( normal, a );

    return *this;
}

Plane& Plane::copy( const Plane& plane )
{
    normal().copy( plane.normal() );
    constant() = plane.constant();

    return *this;
}

Plane& Plane::normalize()
{
    // Note: will lead to a divide by zero if the plane is invalid.

    float inverseNormalLength = 1.0f / normal().length();
    normal().multiplyScalar( inverseNormalLength );
    constant() *= inverseNormalLength;

    return *this;
}

Plane& Plane::negate()
{
    constant() *= -1;
    normal().negate();

    return *this;
}

float Plane::distanceToPoint( const Vector3& point ) const
{
    return normal().dot( point ) + constant();
}

float Plane::distanceToSphere( const Sphere& sphere ) const
{
    return distanceToPoint( sphere.center() ) - sphere.radius();
}

Vector3 Plane::projectPoint( const Vector3& point ) const
{

    return orthoPoint( point ).sub( point ).negate();

}

Vector3 Plane::orthoPoint( const Vector3& point ) const
{
    float perpendicularMagnitude = distanceToPoint( point );

    Vector3 result;
    return result.copy( normal() ).multiplyScalar( perpendicularMagnitude );
}

bool Plane::isIntersectionLine( const Line3& line ) const
{
    // Note: this tests if a line intersects the plane, not whether it (or its end-points) are coplanar with it.

    float startSign = distanceToPoint( line.start() );
    float endSign = distanceToPoint( line.end() );

    return ( startSign < 0 && endSign > 0 ) || ( endSign < 0 && startSign > 0 );
}

bool Plane::intersectLine(const Line3& line, Vector3* result) const
{
    const Vector3 direction = line.delta();

    float denominator = normal().dot( direction );

    if ( denominator == 0 ) {
        // line is coplanar, return origin
        if ( distanceToPoint( line.start() ) == 0 ) {
            if (result) {
                result->copy( line.start() );
            }
            return true;
        }

        // Unsure if this is the correct method to handle this case.
        return false;
    }

    float t = - ( line.start().dot( normal() ) + constant() ) / denominator;

    if( (t < 0 || t > 1) ) {
        return false;
    }

    if (result)
        result->copy( direction ).multiplyScalar( t ).add( line.start() );

    return true;
}

Vector3 Plane::coplanarPoint() const
{
    return Vector3().copy( normal() ).multiplyScalar( - constant() );
}

Plane& Plane::applyMatrix4(const Matrix4& matrix, const Matrix3* optionalNormalMatrix)
{
    Vector3 v1;
    Vector3 v2;

    // compute new normal based on theory here:
    // http://www.songho.ca/opengl/gl_normaltransform.html
    Matrix3 normalMatrix;
    if (!optionalNormalMatrix) {
        optionalNormalMatrix = &(normalMatrix.getNormalMatrix(matrix));
    }
    Vector3 newNormal = v1.copy( normal() ).applyMatrix3( *optionalNormalMatrix );

    Vector3 newCoplanarPoint = coplanarPoint();
    newCoplanarPoint.applyMatrix4( matrix );

    setFromNormalAndCoplanarPoint( newNormal, newCoplanarPoint );

    return *this;
}

Plane& Plane::translate( const Vector3& offset )
{
    constant() = constant() - offset.dot( normal() );

    return *this;
}

bool Plane::equals( const Plane& plane ) const
{
    return plane.normal().equals( normal() ) && ( plane.constant() == constant() );
}

// clone: function () {
// 
//     return new THREE.Plane().copy( this );
// 
// }

}

