/**
 * @author mrdoob / http://mrdoob.com/
 * @author alteredq / http://alteredqualia.com/
 */
 
#include "resin/lights/Light.h"

namespace RESIN {

Light::Light( uint32_t hex, Light_t type )
	: Object3D(kLight), color( hex ), type( type ),
      intensity(0), distance(0), onlyShadow(false)
{
}

}
