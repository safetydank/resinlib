#pragma once

#include "resin/core/Property.h"
#include "resin/math/Color.h"
#include "resin/lights/Light.h"

namespace RESIN {

typedef std::shared_ptr<class HemisphereLight> HemisphereLightRef;

class HemisphereLight : public Light
{
  public:
    static HemisphereLightRef create( uint32_t skyColorHex, uint32_t groundColorHex, float intensity=1.0f );

    Property<Color> groundColor;

  protected:
    HemisphereLight ( uint32_t skyColorHex, uint32_t groundColorHex, float intensity=1.0f );
};

}

