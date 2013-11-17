/**
 * @author bhouston / http://exocortex.com
 * @author mrdoob / http://mrdoob.com/
 */

#include "resin/math/Matrix4.h"
#include "resin/math/Sphere.h"

namespace RESIN {

Sphere::Sphere( const Vector3& center, float radius )
{
	this->center() = center;
	this->radius() = radius;
}

Sphere& Sphere::set( const Vector3& center, float radius )
{
    this->center().copy( center );
    this->radius() = radius;

    return *this;
}

// setFromPoints: function () {
// 
//     var box = new THREE.Box3();
// 
//     return function ( points, optionalCenter )  {
// 
//         var center = center();
// 
//         if ( optionalCenter !== undefined ) {
// 
//             center.copy( optionalCenter );
// 
//         } else {
// 
//             box.setFromPoints( points ).center( center );
// 
//         }
// 
//         var maxRadiusSq = 0;
// 
//         for ( var i = 0, il = points.length; i < il; i ++ ) {
// 
//             maxRadiusSq = Math.max( maxRadiusSq, center.distanceToSquared( points[ i ] ) );
// 
//         }
// 
//         radius() = Math.sqrt( maxRadiusSq );
// 
//         return this;			
//     
//     };
// 
// }(),

Sphere& Sphere::copy( const Sphere& sphere )
{
    center().copy( sphere.center() );
    radius() = sphere.radius();

    return *this;
}

bool Sphere::empty() const
{
    return ( radius() <= 0 );
}

bool Sphere::containsPoint( const Vector3& point ) const
{
    return ( point.distanceToSquared( center() ) <= ( radius() * radius() ) );
}

float Sphere::distanceToPoint( const Vector3& point )
{
    return ( point.distanceTo( center() ) - radius() );
}

bool Sphere::intersectsSphere( const Sphere& sphere )
{
    float radiusSum = radius() + sphere.radius();

    return sphere.center().distanceToSquared( center() ) <= ( radiusSum * radiusSum );
}

Vector3 Sphere::clampPoint( const Vector3& point ) const
{
    float deltaLengthSq = center().distanceToSquared( point );

    Vector3 result(point);

    if ( deltaLengthSq > ( radius() * radius() ) ) {
        result.sub( center() ).normalize();
        result.multiplyScalar( radius() ).add( center() );
    }

    return result;
}

Box3 Sphere::getBoundingBox() const
{
    Box3 box;

    box.set( center(), center() );
    box.expandByScalar( radius() );

    return box;
}

Sphere& Sphere::applyMatrix4( const Matrix4& matrix )
{
    center().applyMatrix4( matrix );
    radius() = radius() * matrix.getMaxScaleOnAxis();

    return *this;
}

Sphere& Sphere::translate( const Vector3& offset )
{
    center().add( offset );

    return *this;
}

bool Sphere::equals( const Sphere& sphere ) const
{
    return sphere.center().equals( center() ) && ( sphere.radius() == radius() );
}

// clone: function () {
// 
//     return new THREE.Sphere().copy( this );
// 
// }

}
