#pragma once

#include "resin/RESIN.h"
#include "resin/core/Attribute.h"
#include "resin/core/Property.h"
#include "resin/materials/Uniform.h"
#include "resin/renderers/Program.h"

#include <map>
#include <memory>
#include <string>

namespace RESIN {

enum MaterialType {
    kOpaque = 1,
    kTransparent = 2
};

enum Material_t {
    kEmptyMaterial = 0,
    kLineBasicMaterial,
    kShaderMaterial,
    kMeshBasicMaterial,
    kMeshPhongMaterial,
    kMeshLambertMaterial,
    kParticleBasicMaterial,
    kSpriteMaterial
};

typedef std::map<std::string, std::string> DefineMap;
typedef std::shared_ptr<class Material> MaterialRef;

typedef Uniform Parameter;
typedef std::shared_ptr<Parameter> ParameterRef;

struct MaterialParameters
{
    std::map<std::string, ParameterRef> parameters;
    ParameterRef parameter();
};

class Material
{
  public:
    static int MaterialIdCount;

    static MaterialRef create(Material_t tag);

    Property<Material_t> tag;

    const int id() const;

    Property<int32_t> side;

    Property<float> opacity;
	Property<bool>  transparent;

    Property<int32_t> blending;

    Property<int32_t> blendSrc;
    Property<int32_t> blendDst;
    Property<int32_t> blendEquation;

	Property<bool> depthTest;
	Property<bool> depthWrite;

	Property<bool>  polygonOffset;
	Property<float> polygonOffsetFactor;
	Property<float> polygonOffsetUnits;

    Property<int32_t> alphaTest;

	Property<float> overdraw;

	Property<bool> visible;

    Property<bool> needsUpdate;

    /*  Properties pulled up from other materials */

    Property<Color> ambient;
    Property<Color> color;
    Property<int32_t> combine;
    Property<Color> emissive;
    Property<TextureRef> envMap;
    Property<bool> fog;
    Property<TextureRef> lightMap;
    Property<float> linewidth;
    Property<TextureRef> map;
    Property<bool> metal;
    Property<bool> morphTargets;
    Property<bool> perPixel;
    Property<float> reflectivity;
    Property<float> refractionRatio;
    Property<int32_t> shading;
    Property<bool> sizeAttenuation;
    Property<bool> skinning;
    Property<TextureRef> specularMap;
    Property<int32_t> vertexColors;
    Property<bool> wireframe;
    Property<float> wireframeLinewidth;
    Property<int32_t> wireframeLinecap;
    Property<int32_t> wireframeLinejoin;
    Property<bool> wrapAround;
    Property<Vector3> wrapRGB;

    /*  End properties */

    typedef Material M;

    CProperty<M, std::string> vertexShader;
    CProperty<M, std::string> fragmentShader;

	// By default, bind position to attribute index 0. In WebGL, attribute 0
	// should always be used to avoid potentially expensive emulation.
	CProperty<M, std::string> index0AttributeName;

    AttributeMap defaultAttributeValues;

    UniformMap uniforms;
    UniformRef uniform(const std::string& name);

    AttributeMap attributes;
    AttributeRef attribute(const std::string& name);

    Property<DefineMap> defines;

    Property<ProgramRef> program;

    //  Used by renderer to instantiate the correct type
    virtual std::string shaderID() const;
  protected:

    Material(Material_t tag);

	int mId;

    std::string mUuid;

    std::string mName;
};

}

