#include <algorithm>

#include "resin/core/Object3D.h"
#include "resin/math/Math.h"
#include "resin/scenes/Scene.h"

using std::static_pointer_cast;
using std::string;
using std::vector;
using namespace glm;

namespace RESIN {

int Object3D::Object3DIdCount = 1;

Object3DRef Object3D::create(Object3D_t tag)
{
    return Object3DRef(new Object3D(tag));
}

Object3D::Object3D(Object3D_t tag) : tag(tag)
{
    mId = Object3D::Object3DIdCount++;
    mUuid = Math::generateUUID();

    mName = string();

    mParent = NULL;

    mUp = vec3(0, 1, 0);

    mPosition = vec3(0, 0, 0);
    mRotation = Euler();
    mQuaternion = quat();
    mScale = vec3(1, 1, 1);

    // keep rotation and quaternion in sync

    mRotation._quaternion = &mQuaternion;
    mQuaternion._euler = &mRotation;

    // mRenderDepth = null;

    mRotationAutoUpdate = true;

    matrix() = mat4(1.0f);
    mMatrixWorld = mat4(1.0f);

    mMatrixAutoUpdate = true;
    mMatrixWorldNeedsUpdate = true;

    visible() = true;

    mCastShadow = false;
    mReceiveShadow = false;

    frustumCulled() = true;
    sortParticles() = false;

    mUserData = NULL;

    //  Renderer settings
    __webglInit = __webglActive = false;
}

const std::string& Object3D::uuid() const
{
    return mUuid;
}

const std::string& Object3D::name() const
{
    return mName;
}

const Vector3& Object3D::up() const
{
    return mUp;
}

const Vector3& Object3D::position() const
{
    return mPosition;
}

const Euler& Object3D::rotation() const
{
    return mRotation;
}

const Quaternion& Object3D::quaternion() const
{
    return mQuaternion;
}

const Vector3& Object3D::scale() const
{
    return mScale;
}

Vector3& Object3D::position()
{
    return mPosition;
}

Euler& Object3D::rotation()
{
    return mRotation;
}

Quaternion& Object3D::quaternion()
{
    return mQuaternion;
}

Object3DRef Object3D::parent() const
{
    return mParent ? mParent->shared_from_this() : Object3DRef();
}

void Object3D::setParent(Object3DRef object)
{
    mParent = object.get();
}

void Object3D::setParent(Object3D* object)
{
    mParent = object;
}

const vector<Object3DRef>& Object3D::children() const
{
    return mChildren;
}

const Matrix4& Object3D::matrixWorld() const
{
    return mMatrixWorld;
}

bool Object3D::matrixAutoUpdate() const
{
    return mMatrixAutoUpdate;
}

void Object3D::applyMatrix(const Matrix4& mat)
{
    Matrix4 m1;

    matrix().multiplyMatrices( mat, matrix() );

    mPosition.getPositionFromMatrix( matrix() );

    mScale.getScaleFromMatrix( matrix() );

    m1.extractRotation( matrix() );

    mQuaternion.setFromRotationMatrix( m1 );
}

void Object3D::setRotationFromAxisAngle(const Vector3& axis, float angle)
{
    // assumes axis is normalized

    mQuaternion.setFromAxisAngle( axis, angle );

}

void Object3D::setRotationFromEuler(const Euler& euler )
{
    mQuaternion.setFromEuler( euler, true );
}

void Object3D::setRotationFromMatrix(const Matrix4& m)
{
    // assumes the upper 3x3 of m is a pure rotation matrix (i.e, unscaled)

    mQuaternion.setFromRotationMatrix( m );
}

void Object3D::setRotationFromQuaternion(const Quaternion& q)
{
    // assumes q is normalized

    mQuaternion.copy( q );
}

Object3D& Object3D::rotateOnAxis(const Vector3& axis, float angle)
{
    // rotate object on axis in object space
    // axis is assumed to be normalized

    Quaternion q1;

    q1.setFromAxisAngle( axis, angle );

    mQuaternion.multiply( q1 );

    return *this;
}

Object3D& Object3D::rotateX(float angle)
{
    Vector3 v1(1.0f, 0.0f, 0.0f);
    return rotateOnAxis( v1, angle );
}

Object3D& Object3D::rotateY(float angle)
{
    Vector3 v1(0.0f, 1.0f, 0.0f);
    return rotateOnAxis( v1, angle );
}

Object3D& Object3D::rotateZ(float angle)
{
    Vector3 v1(0.0f, 0.0f, 1.0f);
    return rotateOnAxis( v1, angle );
}

Object3D& Object3D::translateOnAxis(const Vector3& axis, float distance)
{
    Vector3 v1;

    v1.copy( axis );
    v1.applyQuaternion( mQuaternion );
    mPosition.add( v1.multiplyScalar( distance ) );

    return *this;
}

Object3D& Object3D::translateX(float distance)
{
    Vector3 v1( 1.0f, 0.0f, 0.0f );

    return translateOnAxis( v1, distance );
}

Object3D& Object3D::translateY(float distance)
{
    Vector3 v1( 0.0f, 1.0f, 0.0f );

    return translateOnAxis( v1, distance );
}

Object3D& Object3D::translateZ(float distance)
{
    Vector3 v1( 0.0f, 0.0f, 1.0f );

    return translateOnAxis( v1, distance );
}

Vector3& Object3D::localToWorld( Vector3& vector )
{
    return vector.applyMatrix4( mMatrixWorld );
}

Vector3& Object3D::worldToLocal( Vector3& vector )
{
    Matrix4 m1;

    return vector.applyMatrix4( m1.getInverse( mMatrixWorld ) );
}

void Object3D::lookAt(const Vector3& vector)
{
    // This routine does not support objects with rotated and/or translated parent(s)

    Matrix4 m1;
    m1.lookAt( vector, mPosition, mUp );

    mQuaternion.setFromRotationMatrix( m1 );
}

void Object3D::add( Object3DRef object )
{
    if (object.get() == this) {
        printf( "RESIN::Object3D::add: An object can\'t be added as a child of itself." );
        return;
    }

    if ( object->parent() ) {

        object->parent()->remove( object );

    }

    object->setParent( this );
    // object.dispatchEvent( { type: 'added' } );

    mChildren.push_back( object );

    // add to scene

    Object3DRef scene = shared_from_this();

    while ( scene->parent() ) {
        scene = scene->parent();
    }

    if ( scene && scene->tag() == kScene )  {

        static_pointer_cast<Scene>(scene)->__addObject( object );

    }
}

void Object3D::remove( Object3DRef object )
{
    auto it = std::find(mChildren.begin(), mChildren.end(), object);
    if (it != mChildren.end()) {
        object->setParent(NULL);
        mChildren.erase(it);

        // remove from scene
    
        Object3DRef scene = shared_from_this();
    
        while ( scene->parent() ) {
            scene = scene->parent();
        }
    
        if ( scene && scene->tag() == kScene ) {
    
            static_pointer_cast<Scene>(scene)->__removeObject( object );
    
        }
    }
}

// getObjectById: function ( id, recursive ) {
// 
//     for ( var i = 0, l = this.children.length; i < l; i ++ ) {
// 
//         var child = this.children[ i ];
// 
//         if ( child.id === id ) {
// 
//             return child;
// 
//         }
// 
//         if ( recursive === true ) {
// 
//             child = child.getObjectById( id, recursive );
// 
//             if ( child !== undefined ) {
// 
//                 return child;
// 
//             }
// 
//         }
// 
//     }
// 
//     return undefined;
// 
// },
// 
// getObjectByName: function ( name, recursive ) {
// 
//     for ( var i = 0, l = this.children.length; i < l; i ++ ) {
// 
//         var child = this.children[ i ];
// 
//         if ( child.name === name ) {
// 
//             return child;
// 
//         }
// 
//         if ( recursive === true ) {
// 
//             child = child.getObjectByName( name, recursive );
// 
//             if ( child !== undefined ) {
// 
//                 return child;
// 
//             }
// 
//         }
// 
//     }
// 
//     return undefined;
// 
// },
// 
// getDescendants: function ( array ) {
// 
//     if ( array === undefined ) array = [];
// 
//     Array.prototype.push.apply( array, this.children );
// 
//     for ( var i = 0, l = this.children.length; i < l; i ++ ) {
// 
//         this.children[ i ].getDescendants( array );
// 
//     }
// 
//     return array;
// 
// },
// 
void Object3D::updateMatrix()
{
    matrix().compose( mPosition, mQuaternion, mScale );
    mMatrixWorldNeedsUpdate = true;
}

void Object3D::updateMatrixWorld( bool force )
{
    if ( mMatrixAutoUpdate ) updateMatrix();

    if ( mMatrixWorldNeedsUpdate || force ) {

        if ( !mParent ) {
            mMatrixWorld.copy( matrix() );
        }
        else {
            mMatrixWorld.multiplyMatrices( mParent->matrixWorld(), matrix() );
        }

        mMatrixWorldNeedsUpdate = false;

        force = true;
    }

    // update children

    for (Object3DRef obj : mChildren) {
        obj->updateMatrixWorld(force);
    }
}

// 
// clone: function ( object, recursive ) {
// 
//     if ( object === undefined ) object = new THREE.Object3D();
//     if ( recursive === undefined ) recursive = true;
// 
//     object.name = this.name;
// 
//     object.up.copy( this.up );
// 
//     object.position.copy( this.position );
//     object.quaternion.copy( this.quaternion );
//     object.scale.copy( this.scale );
// 
//     object.renderDepth = this.renderDepth;
// 
//     object.rotationAutoUpdate = this.rotationAutoUpdate;
// 
//     object.matrix.copy( this.matrix );
//     object.matrixWorld.copy( this.matrixWorld );
// 
//     object.matrixAutoUpdate = this.matrixAutoUpdate;
//     object.matrixWorldNeedsUpdate = this.matrixWorldNeedsUpdate;
// 
//     object.visible = this.visible;
// 
//     object.castShadow = this.castShadow;
//     object.receiveShadow = this.receiveShadow;
// 
//     object.frustumCulled = this.frustumCulled;
// 
//     object.userData = JSON.parse( JSON.stringify( this.userData ) );
// 
//     if ( recursive === true ) {
// 
//         for ( var i = 0; i < this.children.length; i ++ ) {
// 
//             var child = this.children[ i ];
//             object.add( child.clone() );
// 
//         }
// 
//     }
// 
//     return object;
// 
// }
// 
// };

}

