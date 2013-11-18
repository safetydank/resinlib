#pragma once

#include "resin/core/Property.h"
#include "resin/lights/Light.h"

#include <cmath>
#include <memory>

namespace RESIN {

typedef std::shared_ptr<class SpotLight> SpotLightRef;

class SpotLight : public Light
{
  public:
    SpotLightRef create( uint32_t hex, float intensity=1.0f, float distance=0.0f, float angle=M_PI / 3.0f, float exponent=10.0f );

    Property<float> angle;
    Property<float> exponent;

    Property<float> shadowCameraNear;
    Property<float> shadowCameraFar;
    Property<float> shadowCameraFov;

    Property<bool> shadowCameraVisible;

    Property<float> shadowBias;
    Property<float> shadowDarkness;

    Property<int> shadowMapWidth;
    Property<int> shadowMapHeight;

	// Property<> shadowMap = null;
	// Property<> shadowMapSize = null;
	// Property<> shadowCamera = null;
	// Property<> shadowMatrix = null;

  protected:
    SpotLight ( uint32_t hex, float intensity=1.0f, float distance=0.0f, float angle=M_PI / 3.0f, float exponent=10.0f );
};

}

