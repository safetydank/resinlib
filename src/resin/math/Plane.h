#pragma once

#include "resin/core/Property.h"
#include "resin/math/Vector3.h"

namespace RESIN { 

class Line3;
class Matrix4;
class Sphere;

class Plane
{
  public:
    Plane(const Vector3& normal=Vector3(1, 0, 0), const float constant=0);

    Property<Vector3> normal;
    Property<float> constant;

    Plane& set( const Vector3& normal, float constant );
    Plane& setComponents( float x, float y, float z, float w );
    Plane& setFromNormalAndCoplanarPoint( const Vector3& normal, const Vector3& point );
    Plane& setFromCoplanarPoints(const Vector3& a, const Vector3& b, const Vector3& c);
    Plane& copy( const Plane& plane );
    Plane& normalize();
    Plane& negate();
    float distanceToPoint( const Vector3& point ) const;
    float distanceToSphere( const Sphere& sphere ) const;
    Vector3 projectPoint( const Vector3& point ) const;
    Vector3 orthoPoint( const Vector3& point ) const;
    bool isIntersectionLine( const Line3& line ) const;
    bool intersectLine(const Line3& line, Vector3* result) const;
    Vector3 coplanarPoint() const;
    Plane& applyMatrix4(const Matrix4& matrix, const Matrix3* optionalNormalMatrix=NULL);
    Plane& translate( const Vector3& offset );
    bool equals( const Plane& plane ) const;

  protected:
    Vector3 mNormal;
    float mConstant;
};

}
