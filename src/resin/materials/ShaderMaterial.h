/**
 * @author alteredq / http://alteredqualia.com/
 *
 * parameters = {
 *  fragmentShader: <string>,
 *  vertexShader: <string>,
 *
 *  uniforms: { "parameter1": { type: "f", value: 1.0 }, "parameter2": { type: "i" value2: 2 } },
 *
 *  defines: { "label" : "value" },
 *
 *  shading: THREE.SmoothShading,
 *  blending: THREE.NormalBlending,
 *  depthTest: <bool>,
 *  depthWrite: <bool>,
 *
 *  wireframe: <boolean>,
 *  wireframeLinewidth: <float>,
 *
 *  lights: <bool>,
 *
 *  vertexColors: THREE.NoColors / THREE.VertexColors / THREE.FaceColors,
 *
 *  skinning: <bool>,
 *  morphTargets: <bool>,
 *  morphNormals: <bool>,
 *
 *	fog: <bool>
 * }
 */

#include "resin/core/Property.h"
#include "resin/materials/Material.h"

#include <string>

namespace RESIN {

typedef std::shared_ptr<class ShaderMaterial> ShaderMaterialRef;

class ShaderMaterial : public Material
{
  public:
    static ShaderMaterialRef create();

    typedef ShaderMaterial M;
	// defines = {};

	// shading = THREE.SmoothShading;
    Property<int32_t> shading;

	Property<float> linewidth;

	Property<bool>  wireframe;
	Property<float> wireframeLinewidth;

	Property<bool> fog;

	Property<bool> lights;

	// vertexColors = THREE.NoColors; // set to use "color" attribute stream
	Property<int32_t> vertexColors;

	Property<bool> skinning;

	Property<bool> morphTargets;
	Property<bool> morphNormals;

    // AttributeMap defaultAttributeValues;

  protected:
    ShaderMaterial();
};

}
