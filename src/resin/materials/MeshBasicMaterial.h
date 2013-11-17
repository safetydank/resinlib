/**
 * @author mrdoob / http://mrdoob.com/
 * @author alteredq / http://alteredqualia.com/
 *
 * parameters = {
 *  color: <hex>,
 *  opacity: <float>,
 *  map: new THREE.Texture( <Image> ),
 *
 *  lightMap: new THREE.Texture( <Image> ),
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
 *
 *  fog: <bool>
 * }
 */

#pragma once

#include "resin/math/Color.h"
#include "resin/materials/Material.h"

namespace RESIN {

typedef std::shared_ptr<class MeshBasicMaterial> MeshBasicMaterialRef;

class MeshBasicMaterial : public Material
{
  public:
    static MeshBasicMaterialRef create();

    virtual std::string shaderID() const;

    // Property<Color> color;

    // Property<TextureRef> map;

    // Property<TextureRef> lightMap;

    // Property<TextureRef> specularMap;

    // Property<TextureRef> envMap;
    // Property<int32_t> combine;
    // Property<float> reflectivity;
    // Property<float> refractionRatio;

    // Property<bool> fog; 

    Property<int32_t> shading;

    // Property<bool> wireframe;
    // Property<float> wireframeLinewidth;
    // Property<int32_t> wireframeLinecap;
    // Property<int32_t> wireframeLinejoin;

    Property<int32_t> vertexColors;

    // Property<bool> skinning;
    // Property<bool> morphTargets;
 
  protected:
    MeshBasicMaterial();

  public:
    static const std::string kVertexShader;
    static const std::string kFragmentShader;

    static void initUniforms(UniformMap& um);
};

}

