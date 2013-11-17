#include "resin/materials/ShaderMaterial.h"

namespace RESIN {

ShaderMaterialRef ShaderMaterial::create()
{
    return ShaderMaterialRef(new ShaderMaterial);
}

ShaderMaterial::ShaderMaterial() : Material(kShaderMaterial)
{
    fragmentShader("void main() {}");
    vertexShader("void main() {}");
    // uniforms = {};
    // defines = {};
    // attributes = null;

    // shading = THREE.SmoothShading;

    linewidth(1.0f);

    wireframe(false);
    wireframeLinewidth(1.0f);

    fog(false); // set to use scene fog

    lights(false); // set to use scene lights

    // vertexColors = THREE.NoColors; // set to use "color" attribute stream

    skinning(false); // set to use skinning attribute streams

    morphTargets(false); // set to use morph targets
    morphNormals(false); // set to use morph normals

    // When rendered geometry doesn't include these attributes but the material does,
    // use these default values in WebGL. This avoids errors when buffer data is missing.
    defaultAttributeValues = {
        { "color", newAttribute<'c'>(1) },
//        { "uv" : [ 0, 0 ] },
//        { "uv2" : [ 0, 0 ] }
    };

    // Color().setRGB(1.0f, 1.0f, 1.0f);

}

}
