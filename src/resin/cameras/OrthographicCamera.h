#pragma once

#include "resin/cameras/Camera.h"

namespace RESIN
{

typedef std::shared_ptr<class OrthographicCamera> OrthographicCameraRef;

class OrthographicCamera : public Camera
{
  public:
    static OrthographicCameraRef create(float left, float right, float top, float bottom, float near=0.1f, float far=2000.0f);
    void updateProjectionMatrix();

    float left;
    float right;
    float top;
    float bottom;
    float near;
    float far;

  protected:
    OrthographicCamera(float left, float right, float top, float bottom, float near=0.1f, float far=2000.0f);
};

}
