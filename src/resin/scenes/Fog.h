#pragma once

#include "resin/core/Property.h"
#include "resin/math/Color.h"

#include <memory>
#include <string>

namespace RESIN {
    
typedef std::shared_ptr<class Fog> FogRef;

class Fog
{
  public:
    static FogRef create( uint32_t hex, float near=1.0f, float far=1000.0f );

    Property<std::string> name;
    Property<Color> color;
    Property<float> near;
    Property<float> far;

  protected:
    Fog ( uint32_t hex, float near=1.0f, float far=1000.0f );
};

}
