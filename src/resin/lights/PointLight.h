#pragma once

#include "resin/lights/Light.h"

namespace RESIN {

typedef std::shared_ptr<class PointLight> PointLightRef;

class PointLight : public Light
{
  public:
    static PointLightRef create(uint32_t hex, float intensity=1.0f, float distance=0.0f);

  protected:
    PointLight(uint32_t hex, float intensity, float distance);
};

}
