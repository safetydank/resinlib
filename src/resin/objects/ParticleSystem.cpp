/**
 * @author alteredq / http://alteredqualia.com/
 */

#include "resin/core/BufferGeometry.h"
#include "resin/materials/ParticleBasicMaterial.h"
#include "resin/math/Math.h"
#include "resin/objects/ParticleSystem.h"

namespace RESIN {

ParticleSystemRef ParticleSystem::create(BufferGeometryRef buffer, MaterialRef material)
{
    return ParticleSystemRef(new ParticleSystem(buffer, material));
}

ParticleSystem::ParticleSystem( BufferGeometryRef geometry, MaterialRef material )
    : Object3D(kParticleSystem)
{
    this->geometry() = geometry ? geometry : BufferGeometry::create();
    this->material() = material ? material : ParticleBasicMaterial::create();

    this->material()->color() = uint32_t(Math::random() * 0xffffff);

    sortParticles() = false;
    frustumCulled() = false;
}

// THREE.ParticleSystem.prototype.clone = function ( object )
// {
// 
//  if ( object === undefined ) object = new THREE.ParticleSystem( this.geometry, this.material );
// 
//  object.sortParticles = this.sortParticles;
// 
//  THREE.Object3D.prototype.clone.call( this, object );
// 
//  return object;
// 
// }

}
