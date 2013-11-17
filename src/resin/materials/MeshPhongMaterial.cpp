#include "resin/materials/MeshPhongMaterial.h"

namespace RESIN {

//  Generated code
#include "resin/renderers/shaders/MeshPhongMaterial-shader.cpp"

MeshPhongMaterialRef MeshPhongMaterial::create()
{
    return MeshPhongMaterialRef(new MeshPhongMaterial);
}

std::string MeshPhongMaterial::shaderID() const
{
    return "phong";
}

MeshPhongMaterial::MeshPhongMaterial() : Material(kMeshPhongMaterial)
{
    initUniforms(uniforms);
    vertexShader(MeshPhongMaterial::kVertexShader);
    fragmentShader(MeshPhongMaterial::kFragmentShader);

    color() = Color( 0xffffff ); // diffuse
    ambient() = Color( 0xffffff );
    emissive() = Color( 0x000000 );
    specular() = Color( 0x111111 );
    shininess() = 30.0f;

    metal() = false;
    perPixel() = true;

    wrapAround() = false;
    wrapRGB() = Vector3( 1, 1, 1 );

    // this.map = null;

    // this.lightMap = null;

    // this.bumpMap = null;
    // bumpScale() = 1.0f;

    // this.normalMap = null;
    // this.normalScale = new THREE.Vector2( 1, 1 );

    // this.specularMap = null;

    // this.envMap = null;
    // this.combine = THREE.MultiplyOperation;
    // this.reflectivity = 1;
    // this.refractionRatio = 0.98;

    fog() = true;

    shading() = kSmoothShading;

    // wireframe() = false;
    // wireframeLinewidth() = 1.0f;
    // this.wireframeLinecap = 'round';
    // this.wireframeLinejoin = 'round';

    vertexColors() = kNoColors;

    skinning() = false;
    // this.morphTargets = false;
    // this.morphNormals = false;
};

}
