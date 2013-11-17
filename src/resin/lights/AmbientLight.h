#pragma once

#include "resin/lights/Light.h"

namespace RESIN {

typedef std::shared_ptr<class AmbientLight> AmbientLightRef;

class AmbientLight : public Light
{
  public:
    static AmbientLightRef create(uint32_t hex);

  protected:
    AmbientLight(uint32_t hex);
};

}
