#pragma once

#include "resin/core/Property.h"
#include "resin/materials/MaterialsFwd.h"
#include "resin/math/Euler.h"
#include "resin/math/Matrix3.h"
#include "resin/math/Matrix4.h"
#include "resin/math/Quaternion.h"
#include "resin/math/Vector3.h"

#include <memory>
#include <string>
#include <vector>

namespace RESIN
{

//  Object3D tags
enum Object3D_t {
    kNone = 0,
    kMesh,
    kLine,
    kLight,
    kCamera,
    kBone,
    kParticleSystem,
    kScene,
    kSprite,
    kMax = 100
};

class BufferGeometry;

typedef std::shared_ptr<Object3D> Object3DRef;

class Object3D : public std::enable_shared_from_this<Object3D>
{
  public:
    static Object3DRef create(Object3D_t tag=kNone);

    Property<Object3D_t> tag;

    const std::string& uuid() const;
    const std::string& name() const;
    const Vector3& up() const;
    const Vector3& position() const;
    const Euler& rotation() const;
    const Quaternion& quaternion() const;
    const Vector3& scale() const;

    bool matrixAutoUpdate() const;

    Vector3& position();
    Euler& rotation();
    Quaternion& quaternion();

    Object3DRef parent() const;
    void setParent(Object3DRef object=Object3DRef());
    void setParent(Object3D* object=NULL);

    Property<Matrix4> matrix;
    const Matrix4& matrixWorld() const;
    const std::vector<Object3DRef>& children() const;

    void applyMatrix(const Matrix4& mat);
    void setRotationFromAxisAngle(const Vector3& axis, float angle);
    void setRotationFromEuler(const Euler& euler);
    void setRotationFromMatrix(const Matrix4& m);
    void setRotationFromQuaternion(const Quaternion& q);
    Object3D& rotateOnAxis(const Vector3& axis, float angle);
    Object3D& rotateX(float angle);
    Object3D& rotateY(float angle);
    Object3D& rotateZ(float angle);
    Object3D& translateOnAxis(const Vector3& axis, float distance);
    Object3D& translateX(float distance);
    Object3D& translateY(float distance);
    Object3D& translateZ(float distance);
    Vector3& localToWorld( Vector3& vector );
    Vector3& worldToLocal( Vector3& vector );
    void lookAt(const Vector3& vector);
    void add( Object3DRef object );
    void remove( Object3DRef object );

    virtual void updateMatrix();
    void updateMatrixWorld( bool force=false );

    template <typename T>
    void traverse(T& callback)
    {
        callback(*this);

        for (Object3DRef child : children()) {
            child->traverse(callback);
        }
    }

  protected:
    static int Object3DIdCount;

    Object3D(Object3D_t tag);

    int mId;
    std::string mUuid;

    std::string mName;

    Object3D* mParent;
    std::vector<Object3DRef> mChildren;

    Vector3 mUp;

    Vector3 mPosition;
    Euler mRotation;
    Quaternion mQuaternion;
    Vector3 mScale;

    // mRenderDepth = null;

    bool mRotationAutoUpdate;

    Matrix4 mMatrix;
    Matrix4 mMatrixWorld;

    bool mMatrixAutoUpdate;
    bool mMatrixWorldNeedsUpdate;

    // bool mVisible;

    bool mCastShadow;
    bool mReceiveShadow;

    // bool mFrustumCulled;

    void* mUserData;

  public:
    /* Properties */
    Property<bool> visible;
    Property<bool> frustumCulled;
    Property<bool> sortParticles;

  public:
    /*  Renderer interface  */
    bool __webglInit;
    bool __webglActive;

    Matrix4 _modelViewMatrix;
    Matrix3 _normalMatrix;

    Property<BufferGeometryRef> geometry;
    Property<MaterialRef> material;
};

}

