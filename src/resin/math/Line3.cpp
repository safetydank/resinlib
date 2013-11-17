/**
 * @author bhouston / http://exocortex.com
 */

#include "resin/math/Line3.h"
#include "resin/math/Math.h"
#include "resin/math/Matrix4.h"

namespace RESIN {

Line3::Line3(const Vector3& start, const Vector3& end)
{
    set(start, end);
}

Line3& Line3::set( const Vector3& start, const Vector3& end )
{
    this->start().copy( start );
    this->end().copy( end );

    return *this;
}

Line3& Line3::copy( const Line3& line )
{
    start().copy( line.start() );
    end().copy( line.end() );

    return *this;
}

Vector3 Line3::center() const
{
    return Vector3().addVectors( start(), end() ).multiplyScalar( 0.5f );
}

Vector3 Line3::delta() const
{
    return Vector3().subVectors( end(), start() );
}

float Line3::distanceSq() const
{
    return start().distanceToSquared( end() );
}

float Line3::distance() const
{
    return start().distanceTo( end() );
}

Vector3 Line3::at( float t ) const
{
    return delta().multiplyScalar( t ).add( start() );
}

float Line3::closestPointToPointParameter(const Vector3& point, bool clampToLine) const
{
    Vector3 startP;
    Vector3 startEnd;

    startP.subVectors( point, start() );
    startEnd.subVectors( end(), start() );

    float startEnd2 = startEnd.dot( startEnd );
    float startEnd_startP = startEnd.dot( startP );

    float t = startEnd_startP / startEnd2;

    if ( clampToLine ) {
        t = Math::clamp( t, 0.0f, 1.0f );
    }

    return t;
}

Vector3 Line3::closestPointToPoint( const Vector3& point, bool clampToLine ) const
{
    float t = closestPointToPointParameter( point, clampToLine );

    return delta().multiplyScalar( t ).add( start() );
}

Line3& Line3::applyMatrix4( const Matrix4& matrix )
{
    start().applyMatrix4( matrix );
    end().applyMatrix4( matrix );

    return *this;
}

bool Line3::equals( const Line3& line ) const
{
    return line.start().equals( start() ) && line.end().equals( end() );
}

//	clone: function () {
//
//		return new THREE.Line3().copy( this );
//
//	}

}

