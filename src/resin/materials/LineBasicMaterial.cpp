/**
 * @author mrdoob / http://mrdoob.com/
 * @author alteredq / http://alteredqualia.com/
 *
 * parameters = {
 *  color: <hex>,
 *  opacity: <float>,
 *
 *  blending: THREE.NormalBlending,
 *  depthTest: <bool>,
 *  depthWrite: <bool>,
 *
 *  linewidth: <float>,
 *  linecap: "round",
 *  linejoin: "round",
 *
 *  vertexColors: <bool>
 *
 *  fog: <bool>
 * }
 */

#include "resin/materials/LineBasicMaterial.h"
#include "resin/materials/MeshBasicMaterial.h"

namespace RESIN {

LineBasicMaterialRef LineBasicMaterial::create()
{
    return LineBasicMaterialRef(new LineBasicMaterial());
}

LineBasicMaterial::LineBasicMaterial()
    : Material(kLineBasicMaterial)
{
    MeshBasicMaterial::initUniforms(uniforms);
    vertexShader(MeshBasicMaterial::kVertexShader);
    fragmentShader(MeshBasicMaterial::kFragmentShader);

	color() = Color( 0xffffff );

	linewidth() = 1.0f;
	// linecap() = 'round';
	// linejoin() = 'round';

	vertexColors() = false;

	fog() = true;
}

}
