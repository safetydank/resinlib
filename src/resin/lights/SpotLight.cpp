/**
 * @author alteredq / http://alteredqualia.com/
 */

#include "resin/lights/SpotLight.h"

namespace RESIN {

SpotLight::SpotLight ( uint32_t hex, float intensity, float distance, float angle, float exponent )
    : Light( hex, kSpot )
{
	position().set( 0, 1, 0 );
	target() = Object3D::create();

	this->intensity() = intensity;
	this->distance() = distance;
	this->angle() = angle;
	this->exponent() = exponent;

	castShadow() = false;
	onlyShadow() = false;

	//

	shadowCameraNear() = 50;
	shadowCameraFar() = 5000;
	shadowCameraFov() = 50;

	shadowCameraVisible() = false;

	shadowBias() = 0;
	shadowDarkness() = 0.5;

	shadowMapWidth() = 512;
	shadowMapHeight() = 512;

	// shadowMap = null;
	// shadowMapSize = null;
	// shadowCamera = null;
	// shadowMatrix = null;

};

// THREE.SpotLight.prototype.clone = function () {
// 
// 	var light = new THREE.SpotLight();
// 
// 	THREE.Light.prototype.clone.call( this, light );
// 
// 	light.target = target.clone();
// 
// 	light.intensity = intensity;
// 	light.distance = distance;
// 	light.angle = angle;
// 	light.exponent = exponent;
// 
// 	light.castShadow = castShadow;
// 	light.onlyShadow = onlyShadow;
// 
// 	return light;
// 
// };

}
