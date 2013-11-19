#pragma once

/**
 * @author mrdoob / http://mrdoob.com/
 * @author alteredq / http://alteredqualia.com/
 *
 * parameters = {
 *  color: <hex>,
 *  opacity: <float>,
 *  map: new THREE.Texture( <Image> ),
 *
 *  size: <float>,
 *
 *  blending: THREE.NormalBlending,
 *  depthTest: <bool>,
 *  depthWrite: <bool>,
 *
 *  vertexColors: <bool>,
 *
 *  fog: <bool>
 * }
 */


#include "resin/materials/Material.h"

namespace RESIN {

typedef std::shared_ptr<class ParticleBasicMaterial> ParticleBasicMaterialRef;

class ParticleBasicMaterial : public Material
{
  public:
    static ParticleBasicMaterialRef create();

    Property<float> size;
    // Property<bool> sizeAttenuation;

  protected:
    ParticleBasicMaterial();

    static const std::string kVertexShader;
    static const std::string kFragmentShader;

    static void initUniforms(UniformMap& um);
};

}

