#pragma once

#include "resin/core/Property.h"
#include "resin/math/Vector3.h"

namespace RESIN {

class Box3;
class Matrix4;
class Plane;
class Sphere;

class Ray
{
  public:
    Ray( const Vector3& origin=Vector3(), const Vector3& direction=Vector3() );
    Ray& set( const Vector3& origin, const Vector3& direction );
    Ray& copy( const Ray& ray );
    Vector3 at( float t ) const;
    Ray& recast(float t);
    Vector3 closestPointToPoint( const Vector3& point ) const;
    float distanceToPoint(const Vector3& point) const;
    float distanceSqToSegment( const Vector3& v0, const Vector3& v1,
            Vector3* optionalPointOnRay, Vector3* optionalPointOnSegment ) const;
    bool isIntersectionSphere( const Sphere& sphere ) const;
    bool isIntersectionPlane( const Plane& plane ) const;
    bool distanceToPlane( const Plane& plane, float *distance=0 ) const;
    bool intersectPlane( const Plane& plane, Vector3* optionalTarget=0 ) const;
    bool isIntersectionBox( const Box3& box ) const;
    bool intersectBox( const Box3& box , Vector3* optionalTarget=0 ) const;
    bool intersectTriangle( const Vector3& a, const Vector3& b, const Vector3& c,
            bool backfaceCulling, Vector3* optionalTarget=0 ) const;
    Ray& applyMatrix4( const Matrix4& matrix4 );
    bool equals( const Ray& ray ) const;

    Property<Vector3> origin;
    Property<Vector3> direction;
};

}
