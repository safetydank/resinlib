/*
 * @author mrdoob / http://mrdoob.com/
 * @author alteredq / http://alteredqualia.com/
 * @author bhouston / http://exocortex.com
 */

#include "resin/core/BufferGeometry.h"
#include "resin/core/Object3D.h"
#include "resin/math/Frustum.h"
#include "resin/math/Matrix4.h"
#include "resin/math/Sphere.h"
#include "resin/math/Vector3.h"

namespace RESIN {

Frustum::Frustum()
{
}

Frustum::Frustum(const Plane& p0, const Plane& p1, const Plane& p2, const Plane& p3, const Plane& p4, const Plane& p5)
{
    set(p0, p1, p2, p3, p4, p5);
}

const Plane* Frustum::planes() const
{
    return mPlanes;
}

const Plane& Frustum::plane(int index) const
{
    //  assert(index >= 0 && index < 6);
    return mPlanes[index];
}

Frustum& Frustum::set(const Plane& p0, const Plane& p1, const Plane& p2, const Plane& p3, const Plane& p4, const Plane& p5)
{
    mPlanes[0].copy(p0);
    mPlanes[1].copy(p1);
    mPlanes[2].copy(p2);
    mPlanes[3].copy(p3);
    mPlanes[4].copy(p4);
    mPlanes[5].copy(p5);

    return *this;
}

Frustum& Frustum::copy( const Frustum& frustum )
{
    Plane* planes = mPlanes;

    for( int i = 0; i < 6; i ++ ) {
        planes[i].copy( frustum.plane(i) );
    }

    return *this;
}

Frustum& Frustum::setFromMatrix( const Matrix4& m )
{
    float me0 = m[0], me1 = m[1], me2 = m[2], me3 = m[3];
    float me4 = m[4], me5 = m[5], me6 = m[6], me7 = m[7];
    float me8 = m[8], me9 = m[9], me10 = m[10], me11 = m[11];
    float me12 = m[12], me13 = m[13], me14 = m[14], me15 = m[15];

    mPlanes[ 0 ].setComponents( me3 - me0, me7 - me4, me11 - me8, me15 - me12 ).normalize();
    mPlanes[ 1 ].setComponents( me3 + me0, me7 + me4, me11 + me8, me15 + me12 ).normalize();
    mPlanes[ 2 ].setComponents( me3 + me1, me7 + me5, me11 + me9, me15 + me13 ).normalize();
    mPlanes[ 3 ].setComponents( me3 - me1, me7 - me5, me11 - me9, me15 - me13 ).normalize();
    mPlanes[ 4 ].setComponents( me3 - me2, me7 - me6, me11 - me10, me15 - me14 ).normalize();
    mPlanes[ 5 ].setComponents( me3 + me2, me7 + me6, me11 + me10, me15 + me14 ).normalize();

    return *this;
}

bool Frustum::intersectsObject(const Object3D& object)
{
    Sphere sphere;

    BufferGeometryRef geometry = object.geometry();
    if (geometry) {
        sphere.copy(geometry->boundingSphere());
        sphere.applyMatrix4(object.matrixWorld());
    }

    return intersectsSphere( sphere );
}

bool Frustum::intersectsSphere( const Sphere& sphere )
{
    const Vector3& center = sphere.center();
    float negRadius = -sphere.radius();

    for ( int i = 0; i < 6; i ++ ) {

        float distance = mPlanes[ i ].distanceToPoint( center );

        if ( distance < negRadius ) {

            return false;

        }

    }

    return true;
}

bool Frustum::intersectsBox(const Box3& box)
{
    Vector3 p1, p2;

    Plane* planes = mPlanes;

    for ( int i = 0; i < 6 ; i ++ ) {

        Plane& plane = (planes[i]);

        p1.x() = plane.normal().x() > 0 ? box.min().x() : box.max().x();
        p2.x() = plane.normal().x() > 0 ? box.max().x() : box.min().x();
        p1.y() = plane.normal().y() > 0 ? box.min().y() : box.max().y();
        p2.y() = plane.normal().y() > 0 ? box.max().y() : box.min().y();
        p1.z() = plane.normal().z() > 0 ? box.min().z() : box.max().z();
        p2.z() = plane.normal().z() > 0 ? box.max().z() : box.min().z();

        float d1 = plane.distanceToPoint( p1 );
        float d2 = plane.distanceToPoint( p2 );

        // if both outside plane, no intersection

        if ( d1 < 0 && d2 < 0 ) {

            return false;

        }
    }

    return true;
}

bool Frustum::containsPoint( const Vector3& point )
{
    for ( int i = 0; i < 6; i ++ ) {

        if ( mPlanes[ i ].distanceToPoint( point ) < 0 ) {

            return false;

        }

    }

    return true;
}

//     clone: function () {
// 
//         return new THREE.Frustum().copy( this );
// 
//     }

}
