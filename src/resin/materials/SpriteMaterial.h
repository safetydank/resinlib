/**
 * @author alteredq / http://alteredqualia.com/
 *
 * parameters = {
 *  color: <hex>,
 *  opacity: <float>,
 *  map: new THREE.Texture( <Image> ),
 *
 *  blending: THREE.NormalBlending,
 *  depthTest: <bool>,
 *  depthWrite: <bool>,
 *
 *  useScreenCoordinates: <bool>,
 *  sizeAttenuation: <bool>,
 *  scaleByViewport: <bool>,
 *  alignment: THREE.SpriteAlignment.center,
 *
 *	uvOffset: new THREE.Vector2(),
 *	uvScale: new THREE.Vector2(),
 *
 *  fog: <bool>
 * }
 */

#pragma once

#include "resin/materials/Material.h"

namespace RESIN {

typedef std::shared_ptr<class SpriteMaterial> SpriteMaterialRef;

class SpriteMaterial : public Material
{
  public:
    static SpriteMaterialRef create();

  protected:
    SpriteMaterial();
};

}
