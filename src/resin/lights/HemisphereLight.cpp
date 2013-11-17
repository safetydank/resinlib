/**
 * @author alteredq / http://alteredqualia.com/
 */

#include "resin/lights/HemisphereLight.h"

namespace RESIN {

HemisphereLightRef HemisphereLight::create( uint32_t skyColorHex, uint32_t groundColorHex,
        float intensity )
{
    return HemisphereLightRef(new HemisphereLight(skyColorHex, groundColorHex, intensity));
}

HemisphereLight::HemisphereLight ( uint32_t skyColorHex, uint32_t groundColorHex,
        float intensity )
    : Light(skyColorHex, kHemisphere)
{
	position().set( 0, 100, 0 );

	groundColor() = groundColorHex;
	this->intensity() = intensity;
}

}
