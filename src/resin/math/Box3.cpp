/**
 * @author bhouston / http://exocortex.com
 * @author WestLangley / http://github.com/WestLangley
 */

#include "resin/math/Box3.h"
#include "resin/math/Matrix4.h"
#include "resin/math/Sphere.h"
#include "resin/math/Vector3.h"

using namespace std;

namespace RESIN {

Box3::Box3( const Vector3& min, const Vector3& max )
{
    mMin = min;
    mMax = max;
}

Box3& Box3::set( const Vector3& min, const Vector3& max )
{
    mMin.copy( min );
    mMax.copy( max );

    return *this;
}

const Vector3& Box3::min() const
{
    return mMin;
}

const Vector3& Box3::max() const
{
    return mMax;
}

Vector3& Box3::min()
{
    return mMin;
}

Vector3& Box3::max()
{
    return mMax;
}

void Box3::addPoint( const Vector3& point )
{
    if ( point.v.x < mMin.v.x ) {

        mMin.v.x = point.v.x;

    } else if ( point.v.x > mMax.v.x ) {

        mMax.v.x = point.v.x;

    }

    if ( point.v.y < mMin.v.y ) {

        mMin.v.y = point.v.y;

    } else if ( point.v.y > mMax.v.y ) {

        mMax.v.y = point.v.y;

    }

    if ( point.v.z < mMin.v.z ) {

        mMin.v.z = point.v.z;

    } else if ( point.v.z > mMax.v.z ) {

        mMax.v.z = point.v.z;

    }
}

Box3& Box3::setFromPoints( const vector<Vector3>& points )
{
    if ( !points.empty() ) {

        auto pointIt = points.begin();

        mMin.copy( *pointIt );
        mMax.copy( *pointIt );

        for ( ++pointIt; pointIt != points.end(); ++pointIt ) {

            addPoint( *pointIt );

        }

    } else {

        makeEmpty();

    }

    return *this;
}


Box3& Box3::setFromCenterAndSize(const Vector3& center, const Vector3& size)
{
    Vector3 halfSize(size.v * 0.5f);

    mMin.copy( center ).sub( halfSize );
    mMax.copy( center ).add( halfSize );

    return *this;
}

// Box3& Box3::setFromObject(Object3D& object)
// {
//     // Computes the world-axis-aligned bounding box of an object (including its children),
//     // accounting for both the object's, and childrens', world transforms
// 
//     Vector3 v1;
// 
//     var scope = this;
// 
//     object.updateMatrixWorld( true );
// 
//     this.makeEmpty();
// 
//     traverse( object, [](Object3D& node){
// 
//         if ( node.geometry !== undefined && node.geometry.vertices !== undefined ) {
// 
//             var vertices = node.geometry.vertices;
// 
//             for ( var i = 0, il = vertices.length; i < il; i++ ) {
// 
//                 v1.copy( vertices[ i ] );
// 
//                 v1.applyMatrix4( node.matrixWorld );
// 
//                 expandByPoint( v1 );
// 
//             }
// 
//         }
// 
//     });
// 
//     return *this;
// }

Box3& Box3::copy( const Box3& box ) {

    mMin.copy( box.min() );
    mMax.copy( box.max() );

    return *this;

}

Box3& Box3::makeEmpty()
{
    mMin.v.x = mMin.v.y = mMin.v.z = 0;
    mMax.v.x = mMax.v.y = mMax.v.z = 0;

    return *this;
}

bool Box3::empty() const
{
    // this is a more robust check for empty than ( volume <= 0 ) because volume can get positive with two negative axes

    return ( mMax.x() < mMin.x() ) || ( mMax.y() < mMin.y() ) || ( mMax.z() < mMin.z() );
}

Vector3 Box3::center() const
{
    return Vector3().addVectors( mMin, mMax ).multiplyScalar( 0.5f );
}

Vector3 Box3::size() const
{
    return Vector3().subVectors( mMax, mMin );

}

Box3& Box3::expandByPoint( const Vector3& point )
{
    mMin.min( point );
    mMax.max( point );

    return *this;
}

Box3& Box3::expandByVector( const Vector3& vector )
{
    mMin.sub( vector );
    mMax.add( vector );

    return *this;
}

Box3& Box3::expandByScalar( float scalar )
{
    mMin.addScalar( -scalar );
    mMax.addScalar( scalar );

    return *this;
}

bool Box3::containsPoint( const Vector3& point ) const
{

    if ( point.x() < mMin.x() || point.x() > mMax.x() ||
         point.y() < mMin.y() || point.y() > mMax.y() ||
         point.z() < mMin.z() || point.z() > mMax.z() ) {

        return false;

    }

    return true;

}

bool Box3::containsBox( const Box3& box ) const
{
    if ( ( mMin.x() <= box.min().x() ) && ( box.max().x() <= mMax.x() ) &&
         ( mMin.y() <= box.min().y() ) && ( box.max().y() <= mMax.y() ) &&
         ( mMin.z() <= box.min().z() ) && ( box.max().z() <= mMax.z() ) ) {

        return true;

    }

    return false;
}

Vector3 Box3::getParameter( const Vector3& point ) const
{
    // This can potentially have a divide by zero if the box
    // has a size dimension of 0.

    return Vector3(
        ( point.x() - mMin.x() ) / ( mMax.x() - mMin.x() ),
        ( point.y() - mMin.y() ) / ( mMax.y() - mMin.y() ),
        ( point.z() - mMin.z() ) / ( mMax.z() - mMin.z() )
    );

}

bool Box3::isIntersectionBox( const Box3& box ) const
{
    // using 6 splitting planes to rule out intersections.

    if ( box.max().x() < mMin.x() || box.min().x() > mMax.x() ||
         box.max().y() < mMin.y() || box.min().y() > mMax.y() ||
         box.max().z() < mMin.z() || box.min().z() > mMax.z() ) {

        return false;

    }

    return true;
}

Vector3 Box3::clampPoint( const Vector3& point ) const
{
    return Vector3().copy( point ).clamp( mMin, mMax );
}

float Box3::distanceToPoint( const Vector3& point ) const
{
    Vector3 clampedPoint = point;
    clampedPoint.clamp( mMin, mMax );
    return clampedPoint.sub( point ).length();
}

Sphere Box3::getBoundingSphere() const
{
    Sphere result;
    Vector3 v1 = size();

    result.set(center(), v1.length() * 0.5);

    return result;
}

Box3& Box3::intersect( const Box3& box )
{
    mMin.max( box.min() );
    mMax.min( box.max() );

    return *this;
}

Box3& Box3::union_( const Box3& box )
{
    mMin.min( box.min() );
    mMax.max( box.max() );

    return *this;
}

Box3& Box3::applyMatrix4(const Matrix4& matrix)
{
    vector<Vector3> points(8);

    // NOTE: I am using a binary pattern to specify all 2^3 combinations below
    points[0].set( mMin.x(), mMin.y(), mMin.z() ).applyMatrix4( matrix ); // 000
    points[1].set( mMin.x(), mMin.y(), mMax.z() ).applyMatrix4( matrix ); // 001
    points[2].set( mMin.x(), mMax.y(), mMin.z() ).applyMatrix4( matrix ); // 010
    points[3].set( mMin.x(), mMax.y(), mMax.z() ).applyMatrix4( matrix ); // 011
    points[4].set( mMax.x(), mMin.y(), mMin.z() ).applyMatrix4( matrix ); // 100
    points[5].set( mMax.x(), mMin.y(), mMax.z() ).applyMatrix4( matrix ); // 101
    points[6].set( mMax.x(), mMax.y(), mMin.z() ).applyMatrix4( matrix ); // 110
    points[7].set( mMax.x(), mMax.y(), mMax.z() ).applyMatrix4( matrix );  // 111

    makeEmpty();
    setFromPoints( points );

    return *this;
}

Box3& Box3::translate( const Vector3& offset )
{
    mMin.add( offset );
    mMax.add( offset );

    return *this;
}

bool Box3::equals( const Box3& box ) const
{
    return box.min().equals( mMin ) && box.max().equals( mMax );
}

}
