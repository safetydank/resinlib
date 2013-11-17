#include "resin/cameras/PerspectiveCamera.h"
#include "resin/math/Math.h"
#include "resin/math/Matrix4.h"

namespace RESIN {

PerspectiveCameraRef PerspectiveCamera::create(float fov, float aspect, float near, float far)
{
    return PerspectiveCameraRef(new PerspectiveCamera(fov, aspect, near, far));
}

PerspectiveCamera::PerspectiveCamera(float fov, float aspect, float near, float far)
    : Camera(), mFullWidth(0), mFov(fov), mAspect(aspect), mNear(near), mFar(far)
{
    updateProjectionMatrix();
}

void PerspectiveCamera::setLens(float focalLength, float frameHeight)
{
    if (frameHeight == 0) frameHeight = 24;

    mFullWidth = 0;
    mFov = 2 * Math::radToDeg( atan( frameHeight / ( focalLength * 2 ) ) );
    updateProjectionMatrix();
}

void PerspectiveCamera::setViewOffset(float fullWidth, float fullHeight, float x, float y, float width, float height)
{
    mFullWidth = fullWidth;
    mFullHeight = fullHeight;
    mX = x;
    mY = y;
    mWidth = width;
    mHeight = height;

    updateProjectionMatrix();
}

void PerspectiveCamera::updateProjectionMatrix()
{
    if (mFullWidth != 0) {
        float aspect = mFullWidth / mFullHeight;
        float top = tan( Math::degToRad( mFov * 0.5 ) ) * mNear;
        float bottom = -top;
        float left = aspect * bottom;
        float right = aspect * top;
        float width = fabs( right - left );
        float height = fabs( top - bottom );

        projectionMatrix().makeFrustum(
            left + mX * width / mFullWidth,
            left + ( mX + mWidth ) * width / mFullWidth,
            top - ( mY + mHeight ) * height / mFullHeight,
            top - mY * height / mFullHeight,
            mNear,
            mFar
        );
    }
    else {
        projectionMatrix().makePerspective( mFov, mAspect, mNear, mFar );
    }
}

}

