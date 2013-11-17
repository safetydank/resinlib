/**
 * @author mrdoob / http://mrdoob.com/
 * @author alteredq / http://alteredqualia.com/
 */

#include "resin/RESIN.h"
#include "resin/materials/Material.h"
#include "resin/math/Math.h"

using std::string;

namespace RESIN {

MaterialRef Material::create(Material_t tag)
{
    return MaterialRef(new Material(tag));
}

Material::Material(Material_t tag)
    : tag(tag),
      vertexShader(this),
      fragmentShader(this),
      index0AttributeName(this)
{
    mId = Material::MaterialIdCount++;

    mUuid = Math::generateUUID();

    mName = "";

    side() = kFrontSide;

    opacity() = 1.0f;
    transparent() = false;

    blending() = kNormalBlending;

    blendSrc() = kSrcAlphaFactor;
    blendDst() = kOneMinusSrcAlphaFactor;
    blendEquation() = kAddEquation;

    depthTest() = true;
    depthWrite() = true;

    polygonOffset() = false;
    polygonOffsetFactor() = 0;
    polygonOffsetUnits() = 0;

    // mAlphaTest = 0;

    overdraw() = 0; // Overdrawn pixels (typically between 0 and 1) for fixing antialiasing gaps in CanvasRenderer

    visible() = true;

    needsUpdate() = true;

    // By default, bind position to attribute index 0. In WebGL, attribute 0
    // should always be used to avoid potentially expensive emulation.
    index0AttributeName("position");

    linewidth() = 1.0f;
    metal() = false;
    perPixel() = true;
    shading() = kNoShading;
	vertexColors() = kNoColors;
    wireframe() = false;
    wireframeLinewidth() = 1.0f;
}

const int Material::id() const
{
    return mId;
}

// clone: function ( material ) {
// 
//     if ( material === undefined ) material = new RESIN::Material();
// 
//     material.name = this.name;
// 
//     material.side = this.side;
// 
//     material.opacity = this.opacity;
//     material.transparent = this.transparent;
// 
//     material.blending = this.blending;
// 
//     material.blendSrc = this.blendSrc;
//     material.blendDst = this.blendDst;
//     material.blendEquation = this.blendEquation;
// 
//     material.depthTest = this.depthTest;
//     material.depthWrite = this.depthWrite;
// 
//     material.polygonOffset = this.polygonOffset;
//     material.polygonOffsetFactor = this.polygonOffsetFactor;
//     material.polygonOffsetUnits = this.polygonOffsetUnits;
// 
//     material.alphaTest = this.alphaTest;
// 
//     material.overdraw = this.overdraw;
// 
//     material.visible = this.visible;
// 
//     return material;
// 
// },
// 
// dispose: function () {
// 
//     this.dispatchEvent( { type: 'dispose' } );
// 
// }

string Material::shaderID() const
{
    return "";
}

UniformRef Material::uniform(const std::string& name)
{
    auto it = uniforms.find(name);
    return (it == uniforms.end()) ? UniformRef() : it->second;
}

AttributeRef Material::attribute(const std::string& name)
{
    auto it = attributes.find(name);
    return (it == attributes.end()) ? AttributeRef() : it->second;
}

int Material::MaterialIdCount = 1;

}
