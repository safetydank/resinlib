#pragma once

#include "resin/core/Property.h"
#include "resin/math/Vector3.h"

namespace RESIN {

class Matrix4;

class Line3
{
  public:
    Line3(const Vector3& start, const Vector3& end);

    Property<Vector3> start;
    Property<Vector3> end;

    Line3& set( const Vector3& start, const Vector3& end );
    Line3& copy( const Line3& line );
    Vector3 center() const;
    Vector3 delta() const;
    float distanceSq() const;
    float distance() const;
    Vector3 at( float t ) const;
    float closestPointToPointParameter(const Vector3& point, bool clampToLine) const;
    Vector3 closestPointToPoint( const Vector3& point, bool clampToLine ) const;
    Line3& applyMatrix4( const Matrix4& matrix );
    bool equals( const Line3& line ) const;
    //	clone();
};

}
