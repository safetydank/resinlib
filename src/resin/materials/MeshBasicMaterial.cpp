#include "MeshBasicMaterial.h"

namespace RESIN {

//  Generated code
#include "resin/renderers/shaders/MeshBasicMaterial-shader.cpp"

MeshBasicMaterialRef MeshBasicMaterial::create()
{
    return MeshBasicMaterialRef(new MeshBasicMaterial);
}

std::string MeshBasicMaterial::shaderID() const
{
    return "basic";
}

MeshBasicMaterial::MeshBasicMaterial()
  : Material(kMeshBasicMaterial)
{
    //  equivalent to three.js setMaterialShaders
    initUniforms(uniforms);
    vertexShader(MeshBasicMaterial::kVertexShader);
    fragmentShader(MeshBasicMaterial::kFragmentShader);

    color(Color( 0xffffff )); // emissive
    reflectivity(1.0f);
    refractionRatio(0.98f); 
    // mCombine = THREE.MultiplyOperation;

    fog(true);

    // mShading = THREE.SmoothShading;

    wireframe(false);
    wireframeLinewidth(1.0f);
    // mWireframeLinecap = 'round';
    // mWireframeLinejoin = 'round';

    // mVertexColors = THREE.NoColors;

    skinning(false);
    morphTargets(false);
}

}

