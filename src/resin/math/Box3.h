#pragma once

#include "resin/math/Vector3.h"
#include <vector>

namespace RESIN {

class Matrix4;
class Sphere;

class Box3
{
  public:
    Box3( const Vector3& min=Vector3(), const Vector3& max=Vector3() );

    const Vector3& min() const;
    const Vector3& max() const;

    Vector3& min();
    Vector3& max();

    Box3& set( const Vector3& min, const Vector3& max );
    void addPoint( const Vector3& point );
    Box3& setFromPoints( const std::vector<Vector3>& points );
    Box3& setFromCenterAndSize(const Vector3& center, const Vector3& size);
    // Box3& setFromObject(Object3D& object);
    Box3& copy( const Box3& box );
    Box3& makeEmpty();
    bool empty() const;
    Vector3 center() const;
    Vector3 size() const;
    Box3& expandByPoint( const Vector3& point );
    Box3& expandByVector( const Vector3& vector );
    Box3& expandByScalar( float scalar );
    bool containsPoint( const Vector3& point ) const;
    bool containsBox( const Box3& box ) const;
    Vector3 getParameter( const Vector3& point ) const;
    bool isIntersectionBox( const Box3& box ) const;
    Vector3 clampPoint( const Vector3& point ) const;
    float distanceToPoint( const Vector3& point ) const;
    Sphere getBoundingSphere() const;
    Box3& intersect( const Box3& box );
    Box3& union_( const Box3& box );
    Box3& applyMatrix4(const Matrix4& matrix);
    Box3& translate( const Vector3& offset );
    bool equals( const Box3& box ) const;
      
  protected:
    Vector3 mMin;
    Vector3 mMax;
};

}
