/**
 * @author mrdoob / http://mrdoob.com/
 * @author alteredq / http://alteredqualia.com/
 * @author szimek / https://github.com/szimek/
 */

#include "resin/core/Image.h"
#include "resin/math/Math.h"
#include "resin/textures/Texture.h"

namespace RESIN {

int Texture::TextureIdCount = 1;

TextureRef Texture::create( ImageRef image, void* mapping,
        int32_t wrapS, int32_t wrapT,
        int32_t magFilter, int32_t minFilter,
        int32_t format, int32_t type, int32_t anisotropy )
{
    return TextureRef(new Texture(image, mapping, wrapS, wrapT, magFilter, minFilter,
        format, type, anisotropy));
}

Texture::Texture( ImageRef image, void* mapping,
        int32_t wrapS, int32_t wrapT,
        int32_t magFilter, int32_t minFilter,
        int32_t format, int32_t type, int32_t anisotropy )
{

    id() = Texture::TextureIdCount ++;
    uuid() = Math::generateUUID();

    // name = "";

    this->image() = image;
    // mipmaps = [];

    // mapping = mapping !== undefined ? mapping : new THREE.UVMapping();

    this->wrapS() = wrapS;
    this->wrapT() = wrapT;

    this->magFilter() = magFilter;
    this->minFilter() = minFilter;

    this->anisotropy() = anisotropy;

    this->format() = format;
    this->type() = type;

    offset().set(0, 0);
    repeat().set(1, 1);

    generateMipmaps() = true;
    premultiplyAlpha() = false;
    flipY() = true;
    unpackAlignment() = 4; // valid values: 1, 2, 4, 8 (see http://www.khronos.org/opengles/sdk/docs/man/xhtml/glPixelStorei.xml)

    needsUpdate() = true;
    // onUpdate() = null;

}

}
