#include "resin/materials/ParticleBasicMaterial.h"

namespace RESIN {

ParticleBasicMaterialRef ParticleBasicMaterial::create()
{
    return ParticleBasicMaterialRef(new ParticleBasicMaterial());
}

ParticleBasicMaterial::ParticleBasicMaterial() : Material(kParticleBasicMaterial)
{
	color() = Color( 0xffffff );

	// map = null;

	size() = 1.0f;
	sizeAttenuation() = true;

	vertexColors() = false;

	fog() = true;
}

// THREE.ParticleBasicMaterial.prototype.clone = function () {
// 
// 	var material = new THREE.ParticleBasicMaterial();
// 
// 	THREE.Material.prototype.clone.call( this, material );
// 
// 	material.color.copy( this.color );
// 
// 	material.map = this.map;
// 
// 	material.size = this.size;
// 	material.sizeAttenuation = this.sizeAttenuation;
// 
// 	material.vertexColors = this.vertexColors;
// 
// 	material.fog = this.fog;
// 
// 	return material;
// 
// };

}
