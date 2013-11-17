#pragma once

#include "resin/math/Matrix4.h"
#include "resin/core/Object3D.h"
#include "resin/core/Property.h"

namespace RESIN
{

typedef std::shared_ptr<class Camera> CameraRef;

class Camera : public Object3D
{
  public:
    virtual ~Camera();
    void lookAt(const Vector3& v);

    static CameraRef create();

    Property<Matrix4> matrixWorldInverse;
    Property<Matrix4> projectionMatrix;
    Property<Matrix4> projectionMatrixInverse;

  protected:
    Camera();
};

}
