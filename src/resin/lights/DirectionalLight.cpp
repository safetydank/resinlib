/**
 * @author mrdoob / http://mrdoob.com/
 * @author alteredq / http://alteredqualia.com/
 */

#include "resin/lights/DirectionalLight.h"

namespace RESIN {

DirectionalLightRef DirectionalLight::create( uint32_t hex, float intensity )
{
    return DirectionalLightRef(new DirectionalLight(hex, intensity));
}

DirectionalLight::DirectionalLight( uint32_t hex, float intensity )
    : Light( hex, kDirectional )
{
    position().set( 0, 1, 0 );
    target() = Object3D::create();

    this->intensity() = intensity;

    castShadow() = false;
    onlyShadow() = false;

    //

    shadowCameraNear() = 50.0f;
    shadowCameraFar() = 5000.0f;

    shadowCameraLeft() = -500.0f;
    shadowCameraRight() = 500.0f;
    shadowCameraTop() = 500.0f;
    shadowCameraBottom() = -500.0f;

    shadowCameraVisible() = false;

    shadowBias() = 0.0f;
    shadowDarkness() = 0.5f;

    shadowMapWidth() = 512.0f;
    shadowMapHeight() = 512.0f;

    //

    // shadowCascade = false;

    // shadowCascadeOffset = new THREE.Vector3( 0, 0, -1000 );
    // shadowCascadeCount = 2;

    // shadowCascadeBias = [ 0, 0, 0 ];
    // shadowCascadeWidth = [ 512, 512, 512 ];
    // shadowCascadeHeight = [ 512, 512, 512 ];

    // shadowCascadeNearZ = [ -1.000, 0.990, 0.998 ];
    // shadowCascadeFarZ  = [  0.990, 0.998, 1.000 ];

    // shadowCascadeArray = [];

    // //

    // shadowMap = null;
    // shadowMapSize = null;
    // shadowCamera = null;
    // shadowMatrix = null;

}

}
