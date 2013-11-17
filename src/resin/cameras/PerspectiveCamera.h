#pragma once

#include "resin/cameras/Camera.h"

namespace RESIN
{

typedef std::shared_ptr<class PerspectiveCamera> PerspectiveCameraRef;

class PerspectiveCamera : public Camera
{
  public:
    static PerspectiveCameraRef create(float fov, float aspect, float near, float far);

    /**
     * Uses Focal Length (in mm) to estimate and set FOV
     * 35mm (fullframe) camera is used if frame size is not specified;
     * Formula based on http://www.bobatkins.com/photography/technical/field_of_view.html
     */
    void setLens(float focalLength, float frameHeight = 0);

    /**
     * Sets an offset in a larger frustum. This is useful for multi-window or
     * multi-monitor/multi-machine setups.
     *
     * For example, if you have 3x2 monitors and each monitor is 1920x1080 and
     * the monitors are in grid like this
     *
     *   +---+---+---+
     *   | A | B | C |
     *   +---+---+---+
     *   | D | E | F |
     *   +---+---+---+
     *
     * then for each monitor you would call it like this
     *
     *   var w = 1920;
     *   var h = 1080;
     *   var fullWidth = w * 3;
     *   var fullHeight = h * 2;
     *
     *   --A--
     *   camera.setOffset( fullWidth, fullHeight, w * 0, h * 0, w, h );
     *   --B--
     *   camera.setOffset( fullWidth, fullHeight, w * 1, h * 0, w, h );
     *   --C--
     *   camera.setOffset( fullWidth, fullHeight, w * 2, h * 0, w, h );
     *   --D--
     *   camera.setOffset( fullWidth, fullHeight, w * 0, h * 1, w, h );
     *   --E--
     *   camera.setOffset( fullWidth, fullHeight, w * 1, h * 1, w, h );
     *   --F--
     *   camera.setOffset( fullWidth, fullHeight, w * 2, h * 1, w, h );
     *
     *   Note there is no reason monitors have to be the same size or in a grid.
     */
    void setViewOffset(float fullWidth, float fullHeight, float x, float y, float width, float height);

  protected:
    PerspectiveCamera(float fov, float aspect, float near, float far);

    float mFov;
    float mAspect;
    float mNear;
    float mFar;

    float mFullWidth;
    float mFullHeight;
    float mX;
    float mY;
    float mWidth;
    float mHeight;

    void updateProjectionMatrix();
};

}
