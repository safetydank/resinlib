#pragma once

#include "resin/core/Object3D.h"
#include "resin/core/Property.h"
#include "resin/math/Color.h"

namespace RESIN {

typedef std::shared_ptr<class Light> LightRef;

enum Light_t {
    kUndefinedLight = 0,
    kAmbient,
    kDirectional,
    kPoint,
    kSpot,
    kHemisphere
};

class Light : public Object3D
{
  public:
    Property<Color> color;
	Property<Object3DRef> target;
    Property<Light_t> type;
	Property<float> intensity;
	Property<float> distance;

	Property<bool> castShadow;
	Property<bool> onlyShadow;

  protected:
    Light(uint32_t hex, Light_t type);
};

}
