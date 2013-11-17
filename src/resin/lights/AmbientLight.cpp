#include "AmbientLight.h"

namespace RESIN {

AmbientLightRef AmbientLight::create(uint32_t hex)
{
    return AmbientLightRef(new AmbientLight(hex));
}

AmbientLight::AmbientLight(uint32_t hex) : Light(hex, kAmbient)
{
}

}
