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
#include "resin/math/Vector2.h"

#include <memory>

namespace RESIN {

typedef std::shared_ptr<class SpriteMaterial> SpriteMaterialRef;

struct SpriteAlignment {
    static Vector2 topLeft;
    static Vector2 topCenter;
    static Vector2 topRight;
    static Vector2 centerLeft;
    static Vector2 center;
    static Vector2 centerRight;
    static Vector2 bottomLeft;
    static Vector2 bottomCenter;
    static Vector2 bottomRight;
};

class SpriteMaterial : public Material
{
  public:
    static SpriteMaterialRef create();

    Property<bool> useScreenCoordinates;
    Property<bool> depthTest;
    Property<bool> scaleByViewport;
    Property<Vector2> alignment;
    Property<Vector2> uvOffset;
    Property<Vector2> uvScale;

  protected:
    SpriteMaterial();
};

}
