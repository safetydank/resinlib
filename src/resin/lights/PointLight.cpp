#include "resin/lights/PointLight.h"

namespace RESIN {

PointLightRef PointLight::create(uint32_t hex, float intensity, float distance)
{
    return PointLightRef(new PointLight(hex, intensity, distance));
}

PointLight::PointLight(uint32_t hex, float intensity, float distance) : Light(hex, kPoint)
{
    this->intensity() = intensity;
    this->distance() = distance;
}

}
