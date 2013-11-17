#include "resin/cameras/OrthographicCamera.h"
#include "resin/math/Math.h"
#include "resin/math/Matrix4.h"

namespace RESIN {

OrthographicCameraRef OrthographicCamera::create(float left, float right, float top, float bottom, float near, float far)
{
    return OrthographicCameraRef(new OrthographicCamera(left, right, top, bottom, near, far));
}

OrthographicCamera::OrthographicCamera(float left, float right, float top, float bottom, float near, float far)
    : Camera(), left(left), right(right), top(top), bottom(bottom), near(near), far(far)
{
    updateProjectionMatrix();
}

void OrthographicCamera::updateProjectionMatrix()
{
    projectionMatrix().makeOrthographic(left, right, top, bottom, near, far);
}

}

