#pragma once

#include "resin/math/Plane.h"

namespace RESIN {

class Matrix4;
class Object3D;
class Sphere;
class Vector3;

class Frustum
{
  public:
    Frustum();
    Frustum(const Plane& p0, const Plane& p1, const Plane& p2, const Plane& p3, const Plane& p4, const Plane& p5);

    const Plane* planes() const;
    const Plane& plane(int index) const;

    Frustum& set(const Plane& p0, const Plane& p1, const Plane& p2, const Plane& p3, const Plane& p4, const Plane& p5);
    Frustum& copy( const Frustum& frustum );
    Frustum& setFromMatrix( const Matrix4& m );
    bool intersectsObject( const Object3D& obj );
    bool intersectsSphere( const Sphere& sphere );
    bool intersectsBox(const Box3& box);
    bool containsPoint( const Vector3& point );

  protected:
    Plane mPlanes[6];
};

}
