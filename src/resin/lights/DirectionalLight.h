#pragma once

#include "resin/core/Property.h"
#include "resin/lights/Light.h"

namespace RESIN {

typedef std::shared_ptr<class DirectionalLight> DirectionalLightRef;

class DirectionalLight : public Light
{
  public:
    static DirectionalLightRef create( uint32_t hex, float intensity=1.0f );

	// Property<Vector3> position;
	// Property<Object3DRef> target;

	// Property<float> intensity;

	// Property<bool> castShadow;
	// Property<bool> onlyShadow;

	Property<float> shadowCameraNear;
	Property<float> shadowCameraFar;

	Property<float> shadowCameraLeft;
	Property<float> shadowCameraRight;
	Property<float> shadowCameraTop;
	Property<float> shadowCameraBottom;

	Property<bool> shadowCameraVisible;

	Property<float> shadowBias;
	Property<float> shadowDarkness;

	Property<float> shadowMapWidth;
	Property<float> shadowMapHeight;

	// //

	// Property<bool> shadowCascade;

	// Property<Vector3> shadowCascadeOffset;
	// Property<int> shadowCascadeCount = 2;

	// Property<> shadowCascadeBias = [ 0, 0, 0 ];
	// Property<> shadowCascadeWidth = [ 512, 512, 512 ];
	// Property<> shadowCascadeHeight = [ 512, 512, 512 ];

	// Property<> shadowCascadeNearZ = [ -1.000, 0.990, 0.998 ];
	// Property<> shadowCascadeFarZ  = [  0.990, 0.998, 1.000 ];

	// Property<> shadowCascadeArray = [];

	// //

	// Property<> shadowMap = null;
	// Property<> shadowMapSize = null;
	// Property<> shadowCamera = null;
	// Property<> shadowMatrix = null;

  protected:
    DirectionalLight( uint32_t hex, float intensity );

};

}
