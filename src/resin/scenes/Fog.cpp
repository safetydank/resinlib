/**
 * @author mrdoob / http://mrdoob.com/
 * @author alteredq / http://alteredqualia.com/
 */

#include "resin/scenes/Fog.h"

namespace RESIN {

Fog::Fog ( uint32_t hex, float near, float far )
    : color( hex ),
      near(near),
      far(far)
{
}

}
