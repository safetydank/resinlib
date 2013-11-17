#pragma once

#if defined( EMSCRIPTEN ) || defined( ANDROID )
#define RESIN_GLES2
#else
#define RESIN_GL
#endif

namespace RESIN {

// GL STATE CONSTANTS

enum {
    kCullFaceNone = 0,
    kCullFaceBack = 1,
    kCullFaceFront = 2,
    kCullFaceFrontBack = 3,
};

enum {
    kFrontFaceDirectionCW = 0,
    kFrontFaceDirectionCCW = 1,
};

// SHADOWING TYPES

enum {
    kBasicShadowMap = 0,
    kPCFShadowMap = 1,
    kPCFSoftShadowMap = 2,
};

// MATERIAL CONSTANTS

// side

enum {
    kFrontSide = 0,
    kBackSide = 1,
    kDoubleSide = 2,
};

// shading

enum {
    kNoShading = 0,
    kFlatShading = 1,
    kSmoothShading = 2,
};

// colors

enum {
    kNoColors = 0,
    kFaceColors = 1,
    kVertexColors = 2,
};

// blending modes

enum {
    kNoBlending = 0,
    kNormalBlending = 1,
    kAdditiveBlending = 2,
    kSubtractiveBlending = 3,
    kMultiplyBlending = 4,
    kCustomBlending = 5,
};

// custom blending equations
// (numbers start from 100 not to clash with other
//  mappings to OpenGL constants defined in Texture.js)

enum {
    kAddEquation = 100,
    kSubtractEquation = 101,
    kReverseSubtractEquation = 102,
};

// custom blending destination factors

enum {
    kZeroFactor = 200,
    kOneFactor = 201,
    kSrcColorFactor = 202,
    kOneMinusSrcColorFactor = 203,
    kSrcAlphaFactor = 204,
    kOneMinusSrcAlphaFactor = 205,
    kDstAlphaFactor = 206,
    kOneMinusDstAlphaFactor = 207,
};

//// custom blending source factors
//
////THREE.ZeroFactor = 200;
////THREE.OneFactor = 201;
////THREE.SrcAlphaFactor = 204;
////THREE.OneMinusSrcAlphaFactor = 205;
////THREE.DstAlphaFactor = 206;
////THREE.OneMinusDstAlphaFactor = 207;

enum {
    kDstColorFactor = 208,
    kOneMinusDstColorFactor = 209,
    kSrcAlphaSaturateFactor = 210,
};


// TEXTURE CONSTANTS

enum {
    kMultiplyOperation = 0,
    kMixOperation = 1,
    kAddOperation = 2,
};

// Mapping modes

//kUVMapping = function () {};
//
//kCubeReflectionMapping = function () {};
//kCubeRefractionMapping = function () {};
//
//kSphericalReflectionMapping = function () {};
//kSphericalRefractionMapping = function () {};


// Wrapping modes

enum {
    kRepeatWrapping = 1000,
    kClampToEdgeWrapping = 1001,
    kMirroredRepeatWrapping = 1002,
};

// Filters

enum {
    kNearestFilter = 1003,
    kNearestMipMapNearestFilter = 1004,
    kNearestMipMapLinearFilter = 1005,
    kLinearFilter = 1006,
    kLinearMipMapNearestFilter = 1007,
    kLinearMipMapLinearFilter = 1008,
};

// Data types

enum {
    kUnsignedByteType = 1009,
    kByteType = 1010,
    kShortType = 1011,
    kUnsignedShortType = 1012,
    kIntType = 1013,
    kUnsignedIntType = 1014,
    kFloatType = 1015,
};

// Pixel types

enum {
    //kUnsignedByteType = 1009,
    kUnsignedShort4444Type = 1016,
    kUnsignedShort5551Type = 1017,
    kUnsignedShort565Type = 1018,
};

// Pixel formats

enum {
    kAlphaFormat = 1019,
    kRGBFormat = 1020,
    kRGBAFormat = 1021,
    kLuminanceFormat = 1022,
    kLuminanceAlphaFormat = 1023,
};

// Compressed texture formats

enum {
    kRGB_S3TC_DXT1_Format = 2001,
    kRGBA_S3TC_DXT1_Format = 2002,
    kRGBA_S3TC_DXT3_Format = 2003,
    kRGBA_S3TC_DXT5_Format = 2004,
};

/*
// Potential future PVRTC compressed texture formats
kRGB_PVRTC_4BPPV1_Format = 2100,
kRGB_PVRTC_2BPPV1_Format = 2101,
kRGBA_PVRTC_4BPPV1_Format = 2102,
kRGBA_PVRTC_2BPPV1_Format = 2103,
*/

};
