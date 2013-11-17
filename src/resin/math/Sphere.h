#pragma once

#include "resin/core/Property.h"
#include "resin/math/Box3.h"
#include "resin/math/Vector3.h"

namespace RESIN {

class Sphere
{
  public:
    Sphere( const Vector3& center=Vector3(), float radius=0 );

    Property<Vector3> center;
    Property<float> radius;

    Sphere& set( const Vector3& center, float radius );
    Sphere& copy( const Sphere& sphere );
    bool empty() const;
    bool containsPoint( const Vector3& point ) const;
    float distanceToPoint( const Vector3& point );
    bool intersectsSphere( const Sphere& sphere );
    Vector3 clampPoint( const Vector3& point ) const;
    Box3 getBoundingBox() const;
    Sphere& applyMatrix4( const Matrix4& matrix );
    Sphere& translate( const Vector3& offset );
    bool equals( const Sphere& sphere ) const;
};

}
