#pragma once

/**
 * @author mrdoob / http://mrdoob.com/
 * @author alteredq / http://alteredqualia.com/
 *
 * parameters = {
 *  color: <hex>,
 *  ambient: <hex>,
 *  emissive: <hex>,
 *  specular: <hex>,
 *  shininess: <float>,
 *  opacity: <float>,
 *
 *  map: new THREE.Texture( <Image> ),
 *
 *  lightMap: new THREE.Texture( <Image> ),
 *
 *  bumpMap: new THREE.Texture( <Image> ),
 *  bumpScale: <float>,
 *
 *  normalMap: new THREE.Texture( <Image> ),
 *  normalScale: <Vector2>,
 *
 *  specularMap: new THREE.Texture( <Image> ),
 *
 *  envMap: new THREE.TextureCube( [posx, negx, posy, negy, posz, negz] ),
 *  combine: THREE.Multiply,
 *  reflectivity: <float>,
 *  refractionRatio: <float>,
 *
 *  shading: THREE.SmoothShading,
 *  blending: THREE.NormalBlending,
 *  depthTest: <bool>,
 *  depthWrite: <bool>,
 *
 *  wireframe: <boolean>,
 *  wireframeLinewidth: <float>,
 *
 *  vertexColors: THREE.NoColors / THREE.VertexColors / THREE.FaceColors,
 *
 *  skinning: <bool>,
 *  morphTargets: <bool>,
 *  morphNormals: <bool>,
 *
 *  fog: <bool>
 * }
 */

#include "resin/core/Property.h"
#include "resin/materials/Material.h"

namespace RESIN {

typedef std::shared_ptr<class MeshPhongMaterial> MeshPhongMaterialRef;

class MeshPhongMaterial : public Material
{
  public:
    static MeshPhongMaterialRef create();

    virtual std::string shaderID() const;

    Property<float> shininess;
    Property<Color> specular;

  protected:
    MeshPhongMaterial();

  protected:
    static const std::string kVertexShader;
    static const std::string kFragmentShader;

    static void initUniforms(UniformMap& um);
};

}
