#pragma once

#include "resin/RESIN.h"
#include "resin/core/Property.h"
#include "resin/math/Vector2.h"

#include <memory>
#include <string>
#include <vector>

namespace RESIN {

typedef std::shared_ptr<class Image> ImageRef;
typedef std::shared_ptr<class Texture> TextureRef;

class Texture
{
  public:
    static TextureRef create( ImageRef image, void* mapping=0,
        int32_t wrapS=kClampToEdgeWrapping, int32_t wrapT=kClampToEdgeWrapping,
        int32_t magFilter=kLinearFilter, int32_t minFilter=kLinearMipMapLinearFilter,
        int32_t format=kRGBAFormat, int32_t type=kUnsignedByteType, int32_t anisotropy=1 );

    Property<int> id;
    Property<std::string> uuid;

    Property<std::string> name;

    Property<ImageRef> image;
    std::vector<ImageRef> mipmaps;

    // Property<> mapping = mapping !== undefined ? mapping : new THREE.UVMapping();

    Property<int32_t> wrapS;
    Property<int32_t> wrapT;

    Property<int32_t> magFilter;
    Property<int32_t> minFilter;

    Property<int32_t> anisotropy;

    Property<int32_t> format;
    Property<int32_t> type;

    Property<Vector2> offset;
    Property<Vector2> repeat;

    Property<bool> generateMipmaps;
    Property<bool> premultiplyAlpha;
    Property<bool> flipY;
    Property<uint32_t> unpackAlignment;

    Property<bool> needsUpdate;
    // Property<> onUpdate = null;

  protected:
    Texture( ImageRef image, void* mapping, int32_t wrapS, int32_t wrapT,
        int32_t magFilter, int32_t minFilter,
        int32_t format, int32_t type, int32_t anisotropy );

    static int TextureIdCount;

  public:
    bool __webglInit;
    unsigned int __webglTexture;

};

}
