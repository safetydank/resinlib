
#include "resin/materials/SpriteMaterial.h"

namespace RESIN {

Vector2 SpriteAlignment::topLeft      = Vector2( 1, -1 );
Vector2 SpriteAlignment::topCenter    = Vector2( 0, -1);
Vector2 SpriteAlignment::topRight     = Vector2( -1, -1 );
Vector2 SpriteAlignment::centerLeft   = Vector2( 1, 0 );
Vector2 SpriteAlignment::center       = Vector2( 0, 0 );
Vector2 SpriteAlignment::centerRight  = Vector2( -1, 0 );
Vector2 SpriteAlignment::bottomLeft   = Vector2( 1, 1 );
Vector2 SpriteAlignment::bottomCenter = Vector2( 0, 1 );
Vector2 SpriteAlignment::bottomRight  = Vector2( -1, 1 );

SpriteMaterialRef SpriteMaterial::create()
{
    return SpriteMaterialRef( new SpriteMaterial() );
}

SpriteMaterial::SpriteMaterial() : Material( kSpriteMaterial )
{
   	// defaults
   
   	color() = Color( 0xffffff );
   	// map = new THREE.Texture();
   
   	useScreenCoordinates() = true;
   	depthTest() = !useScreenCoordinates();
   	sizeAttenuation() = !useScreenCoordinates();
   	scaleByViewport() = !sizeAttenuation();
   	alignment() = SpriteAlignment::center;
   
   	fog() = false;
   
   	uvOffset = Vector2( 0, 0 );
   	uvScale  = Vector2( 1, 1 );
   
   	// override coupled defaults if not specified explicitly by parameters
   
   	// parameters = parameters || {};
   
   	// if ( parameters.depthTest === undefined ) depthTest = !useScreenCoordinates;
   	// if ( parameters.sizeAttenuation === undefined ) sizeAttenuation = !useScreenCoordinates;
   	// if ( parameters.scaleByViewport === undefined ) scaleByViewport = !sizeAttenuation;
}

// THREE.SpriteMaterial = function ( parameters ) {
// 
// 	THREE.Material.call( this );
// 
// 	// defaults
// 
// 	this.color = new THREE.Color( 0xffffff );
// 	this.map = new THREE.Texture();
// 
// 	this.useScreenCoordinates = true;
// 	this.depthTest = !this.useScreenCoordinates;
// 	this.sizeAttenuation = !this.useScreenCoordinates;
// 	this.scaleByViewport = !this.sizeAttenuation;
// 	this.alignment = THREE.SpriteAlignment.center.clone();
// 
// 	this.fog = false;
// 
// 	this.uvOffset = new THREE.Vector2( 0, 0 );
// 	this.uvScale  = new THREE.Vector2( 1, 1 );
// 
// 	// set parameters
// 
// 	this.setValues( parameters );
// 
// 	// override coupled defaults if not specified explicitly by parameters
// 
// 	parameters = parameters || {};
// 
// 	if ( parameters.depthTest === undefined ) this.depthTest = !this.useScreenCoordinates;
// 	if ( parameters.sizeAttenuation === undefined ) this.sizeAttenuation = !this.useScreenCoordinates;
// 	if ( parameters.scaleByViewport === undefined ) this.scaleByViewport = !this.sizeAttenuation;
// 
// }

// THREE.SpriteMaterial.prototype.clone = function () {
// 
// 	var material = new THREE.SpriteMaterial();
// 
// 	THREE.Material.prototype.clone.call( this, material );
// 
// 	material.color.copy( this.color );
// 	material.map = this.map;
// 
// 	material.useScreenCoordinates = this.useScreenCoordinates;
// 	material.sizeAttenuation = this.sizeAttenuation;
// 	material.scaleByViewport = this.scaleByViewport;
// 	material.alignment.copy( this.alignment );
// 
// 	material.uvOffset.copy( this.uvOffset );
// 	material.uvScale.copy( this.uvScale );
// 
// 	material.fog = this.fog;
// 
// 	return material;
// 
// };

// Alignment enums

// THREE.SpriteAlignment = {};
// THREE.SpriteAlignment.topLeft = new THREE.Vector2( 1, -1 );
// THREE.SpriteAlignment.topCenter = new THREE.Vector2( 0, -1 );
// THREE.SpriteAlignment.topRight = new THREE.Vector2( -1, -1 );
// THREE.SpriteAlignment.centerLeft = new THREE.Vector2( 1, 0 );
// THREE.SpriteAlignment.center = new THREE.Vector2( 0, 0 );
// THREE.SpriteAlignment.centerRight = new THREE.Vector2( -1, 0 );
// THREE.SpriteAlignment.bottomLeft = new THREE.Vector2( 1, 1 );
// THREE.SpriteAlignment.bottomCenter = new THREE.Vector2( 0, 1 );
// THREE.SpriteAlignment.bottomRight = new THREE.Vector2( -1, 1 );

}
