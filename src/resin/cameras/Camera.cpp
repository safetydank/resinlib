#include "resin/math/Math.h"
#include "resin/math/Matrix4.h"
#include "resin/cameras/Camera.h"

namespace RESIN {

CameraRef Camera::create()
{
    return CameraRef(new Camera());
}

Camera::Camera() : Object3D(kCamera)
{
}

Camera::~Camera()
{
}

void Camera::lookAt(const Vector3& v)
{
    Matrix4 m1;
    m1.lookAt(mPosition, v, mUp);
    quaternion().setFromRotationMatrix( m1 );
}

}
