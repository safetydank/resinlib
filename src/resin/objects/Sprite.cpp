/**
 * @author mikael emtinger / http://gomo.se/
 * @author alteredq / http://alteredqualia.com/
 */

#include "resin/objects/Sprite.h"
#include "resin/materials/Material.h"
#include "resin/materials/SpriteMaterial.h"

namespace RESIN {

SpriteRef Sprite::create( MaterialRef material )
{
    return SpriteRef( new Sprite( material ) );
}

Sprite::Sprite( MaterialRef material ) : Object3D( kSprite )
{

	this->material() = material ? material : SpriteMaterial::create();
	angle() = 0;

}

/*
 * Custom update matrix
 */

void Sprite::updateMatrix()
{

	rotation().set( 0, 0, angle(), rotation().order() );
	quaternion().setFromEuler( rotation() );
	matrix().compose( position(), quaternion(), scale() );

	mMatrixWorldNeedsUpdate = true;

}

// THREE.Sprite.prototype.clone = function ( object ) {
// 
// 	if ( object === undefined ) object = new THREE.Sprite( this.material );
// 
// 	THREE.Object3D.prototype.clone.call( this, object );
// 
// 	return object;
// 
// };

}
