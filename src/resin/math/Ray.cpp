/**
 * @author bhouston / http://exocortex.com
 */

#include "resin/math/Box3.h"
#include "resin/math/Math.h"
#include "resin/math/Matrix4.h"
#include "resin/math/Plane.h"
#include "resin/math/Ray.h"
#include "resin/math/Sphere.h"

namespace RESIN {

Ray::Ray( const Vector3& origin, const Vector3& direction )
{

    this->origin() = origin;
    this->direction() = direction;

}

Ray& Ray::set( const Vector3& origin, const Vector3& direction )
{

    this->origin().copy( origin );
    this->direction().copy( direction );

    return *this;
}

Ray& Ray::copy( const Ray& ray )
{

    origin().copy( ray.origin() );
    direction().copy( ray.direction() );

    return *this;

}

Vector3 Ray::at( float t ) const
{

    Vector3 result;

    return result.copy( direction() ).multiplyScalar( t ).add( origin() );

}

Ray& Ray::recast(float t)
{

    Vector3 v1;
    origin() = at( t );
    return *this;
}

Vector3 Ray::closestPointToPoint( const Vector3& point ) const
{

    Vector3 result;
    result.subVectors( point, origin() );
    float directionDistance = result.dot( direction() );

    if ( directionDistance < 0 ) {

        return origin();

    }

    return result.copy( direction() ).multiplyScalar( directionDistance ).add( origin() );

}

float Ray::distanceToPoint(const Vector3& point) const
{
    Vector3 v1;

    float directionDistance = v1.subVectors( point, origin() ).dot( direction() );

    // point behind the ray

    if ( directionDistance < 0 ) {

        return origin().distanceTo( point );

    }

    v1.copy( direction() ).multiplyScalar( directionDistance ).add( origin() );

    return v1.distanceTo( point );

}

float Ray::distanceSqToSegment( const Vector3& v0, const Vector3& v1,
        Vector3* optionalPointOnRay, Vector3* optionalPointOnSegment ) const
{

    // from http://www.geometrictools.com/LibMathematics/Distance/Wm5DistRay3Segment3.cpp
    // It returns the min distance between the ray and the segment
    // defined by v0 and v1
    // It can also set two optional targets :
    // - The closest point on the ray
    // - The closest point on the segment

    auto segCenter = v0.clone().add( v1 ).multiplyScalar( 0.5f );
    auto segDir = v1.clone().sub( v0 ).normalize();
    auto segExtent = v0.distanceTo( v1 ) * 0.5f;
    auto diff = origin().clone().sub( segCenter );
    auto a01 = - direction().dot( segDir );
    auto b0 = diff.dot( direction() );
    auto b1 = - diff.dot( segDir );
    auto c = diff.lengthSq();
    auto det = fabs( 1 - a01 * a01 );
    float s0, s1, sqrDist, extDet;

    if ( det >= 0 ) {

        // The ray and segment are not parallel.

        s0 = a01 * b1 - b0;
        s1 = a01 * b0 - b1;
        extDet = segExtent * det;

        if ( s0 >= 0 ) {

            if ( s1 >= - extDet ) {

                if ( s1 <= extDet ) {

                    // region 0
                    // Minimum at interior points of ray and segment.

                    auto invDet = 1.0f / det;
                    s0 *= invDet;
                    s1 *= invDet;
                    sqrDist = s0 * ( s0 + a01 * s1 + 2 * b0 ) + s1 * ( a01 * s0 + s1 + 2 * b1 ) + c;

                } else {

                    // region 1

                    s1 = segExtent;
                    s0 = Math::max( 0.0f, - ( a01 * s1 + b0) );
                    sqrDist = - s0 * s0 + s1 * ( s1 + 2 * b1 ) + c;

                }

            } else {

                // region 5

                s1 = - segExtent;
                s0 = Math::max( 0.0f, - ( a01 * s1 + b0) );
                sqrDist = - s0 * s0 + s1 * ( s1 + 2 * b1 ) + c;

            }

        } else {

            if ( s1 <= - extDet) {

                // region 4

                s0 = Math::max( 0.0f, - ( - a01 * segExtent + b0 ) );
                s1 = ( s0 > 0 ) ? - segExtent : Math::min( Math::max( - segExtent, - b1 ), segExtent );
                sqrDist = - s0 * s0 + s1 * ( s1 + 2 * b1 ) + c;

            } else if ( s1 <= extDet ) {

                // region 3

                s0 = 0;
                s1 = Math::min( Math::max( - segExtent, - b1 ), segExtent );
                sqrDist = s1 * ( s1 + 2 * b1 ) + c;

            } else {

                // region 2

                s0 = Math::max( 0.0f, - ( a01 * segExtent + b0 ) );
                s1 = ( s0 > 0 ) ? segExtent : Math::min( Math::max( - segExtent, - b1 ), segExtent );
                sqrDist = - s0 * s0 + s1 * ( s1 + 2 * b1 ) + c;

            }

        }

    } else {

        // Ray and segment are parallel.

        s1 = ( a01 > 0 ) ? - segExtent : segExtent;
        s0 = Math::max( 0.0f, - ( a01 * s1 + b0 ) );
        sqrDist = - s0 * s0 + s1 * ( s1 + 2 * b1 ) + c;

    }

    if ( optionalPointOnRay ) {

        optionalPointOnRay->copy( direction().clone().multiplyScalar( s0 ).add( origin() ) );

    }

    if ( optionalPointOnSegment ) {

        optionalPointOnSegment->copy( segDir.clone().multiplyScalar( s1 ).add( segCenter ) );

    }

    return sqrDist;

}

bool Ray::isIntersectionSphere( const Sphere& sphere ) const
{

    return distanceToPoint( sphere.center() ) <= sphere.radius();

}

bool Ray::isIntersectionPlane( const Plane& plane ) const
{

    // check if the ray lies on the plane first

    float distToPoint = plane.distanceToPoint( origin() );

    if ( distToPoint == 0 ) {

        return true;

    }

    float denominator = plane.normal().dot( direction() );

    if ( denominator * distToPoint < 0 ) {

        return true;

    }

    // ray origin is behind the plane (and is pointing behind it)

    return false;

}

bool Ray::distanceToPlane( const Plane& plane, float *distance ) const
{

    float denominator = plane.normal().dot( direction() );
    if ( denominator == 0 ) {

        // line is coplanar, return origin
        if( plane.distanceToPoint( origin() ) == 0 ) {

            *distance = 0;
            return true;

        }

        // Null is preferable to undefined since undefined means.... it is undefined

        return false;

    }

    auto t = - ( origin().dot( plane.normal() ) + plane.constant() ) / denominator;

    // Return if the ray never intersects the plane

    if (t >= 0) {
        *distance = t;
        return true;
    }

    return false;

}

bool Ray::intersectPlane( const Plane& plane, Vector3* optionalTarget ) const
{

    float t;

    if ( !distanceToPlane(plane, &t) ) {

        return false;
    }

    if ( optionalTarget )
        *optionalTarget = at(t);

    return true;

}

bool Ray::isIntersectionBox( const Box3& box ) const
{

    Vector3 v;

    return intersectBox( box, &v );

}

bool Ray::intersectBox( const Box3& box , Vector3* optionalTarget ) const
{

    // http://www.scratchapixel.com/lessons/3d-basic-lessons/lesson-7-intersecting-simple-shapes/ray-box-intersection/

    float tmin,tmax,tymin,tymax,tzmin,tzmax;

    float invdirx = 1.0f / direction().x(),
          invdiry = 1.0f / direction().y(),
          invdirz = 1.0f / direction().z();

    Vector3 origin_ = origin();

    if (invdirx >= 0) {

        tmin = (box.min().x() - origin_.x()) * invdirx;
        tmax = (box.max().x() - origin_.x()) * invdirx;

    } else {

        tmin = (box.max().x() - origin_.x()) * invdirx;
        tmax = (box.min().x() - origin_.x()) * invdirx;
    }

    if (invdiry >= 0) {

        tymin = (box.min().y() - origin_.y()) * invdiry;
        tymax = (box.max().y() - origin_.y()) * invdiry;

    } else {

        tymin = (box.max().y() - origin_.y()) * invdiry;
        tymax = (box.min().y() - origin_.y()) * invdiry;
    }

    if ((tmin > tymax) || (tymin > tmax)) return false;

    // These lines also handle the case where tmin or tmax is NaN
    // (result of 0 * Infinity). x != x returns true if x is NaN

    if (tymin > tmin || tmin != tmin ) tmin = tymin;

    if (tymax < tmax || tmax != tmax ) tmax = tymax;

    if (invdirz >= 0) {

        tzmin = (box.min().z() - origin_.z()) * invdirz;
        tzmax = (box.max().z() - origin_.z()) * invdirz;

    } else {

        tzmin = (box.max().z() - origin_.z()) * invdirz;
        tzmax = (box.min().z() - origin_.z()) * invdirz;
    }

    if ((tmin > tzmax) || (tzmin > tmax)) return false;

    if (tzmin > tmin || tmin != tmin ) tmin = tzmin;

    if (tzmax < tmax || tmax != tmax ) tmax = tzmax;

    //return point closest to the ray (positive side)

    if ( tmax < 0 ) return false;

    if (optionalTarget) {
        *optionalTarget = at( tmin >= 0 ? tmin : tmax );
    }

    return true;

}

bool Ray::intersectTriangle( const Vector3& a, const Vector3& b, const Vector3& c,
        bool backfaceCulling, Vector3* optionalTarget ) const
{

    // Compute the offset origin, edges, and normal.
    Vector3 diff;
    Vector3 edge1;
    Vector3 edge2;
    Vector3 normal;

    // from http://www.geometrictools.com/LibMathematics/Intersection/Wm5IntrRay3Triangle3.cpp

    edge1.subVectors( b, a );
    edge2.subVectors( c, a );
    normal.crossVectors( edge1, edge2 );

    // Solve Q + t*D = b1*E1 + b2*E2 (Q = kDiff, D = ray direction,
    // E1 = kEdge1, E2 = kEdge2, N = Cross(E1,E2)) by
    //   |Dot(D,N)|*b1 = sign(Dot(D,N))*Dot(D,Cross(Q,E2))
    //   |Dot(D,N)|*b2 = sign(Dot(D,N))*Dot(D,Cross(E1,Q))
    //   |Dot(D,N)|*t = -sign(Dot(D,N))*Dot(Q,N)
    float DdN = direction().dot( normal );
    float sign;

    if ( DdN > 0 ) {

        if ( backfaceCulling ) return false;
        sign = 1;

    } else if ( DdN < 0 ) {

        sign = - 1;
        DdN = - DdN;

    } else {

        return false;

    }

    diff.subVectors( origin(), a );
    float DdQxE2 = sign * direction().dot( edge2.crossVectors( diff, edge2 ) );

    // b1 < 0, no intersection
    if ( DdQxE2 < 0 ) {

        return false;

    }

    float DdE1xQ = sign * direction().dot( edge1.cross( diff ) );

    // b2 < 0, no intersection
    if ( DdE1xQ < 0 ) {

        return false;

    }

    // b1+b2 > 1, no intersection
    if ( DdQxE2 + DdE1xQ > DdN ) {

        return false;

    }

    // Line intersects triangle, check if ray does.
    float QdN = - sign * diff.dot( normal );

    // t < 0, no intersection
    if ( QdN < 0 ) {

        return false;

    }

    // Ray intersects triangle.
    if (optionalTarget) {
        *optionalTarget = at(QdN / DdN);
    }

    return true;

}

Ray& Ray::applyMatrix4( const Matrix4& matrix4 )
{

    direction().add( origin() ).applyMatrix4( matrix4 );
    origin().applyMatrix4( matrix4 );
    direction().sub( origin() );
    direction().normalize();

    return *this;
}

bool Ray::equals( const Ray& ray ) const
{

    return ray.origin().equals( origin() ) && ray.direction().equals( direction() );

}

}
