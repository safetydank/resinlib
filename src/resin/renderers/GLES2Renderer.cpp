#include "resin/RESIN.h"
#include "resin/cameras/Camera.h"
#include "resin/core/BufferGeometry.h"
#include "resin/core/Image.h"
#include "resin/lights/HemisphereLight.h"
#include "resin/lights/Light.h"
#include "resin/lights/SpotLight.h"
#include "resin/materials/LineBasicMaterial.h"
#include "resin/materials/Material.h"
#include "resin/materials/MeshPhongMaterial.h"
#include "resin/materials/ParticleBasicMaterial.h"
#include "resin/math/Math.h"
#include "resin/objects/Line.h"
#include "resin/renderers/GLES2Renderer.h"
#include "resin/scenes/Fog.h"
#include "resin/scenes/Scene.h"

#include <algorithm>

#if defined(RESIN_GLES2)
  #include <GLES2/gl2.h>
#elif defined(RESIN_GL)
  #include <OpenGL/gl.h>
  #define glClearDepthf glClearDepth
#endif

#include <sstream>

using std::map;
using std::remove_if;
using std::static_pointer_cast;
using std::string;
using std::stringstream;
using std::vector;

namespace RESIN {

GLES2Renderer::Parameters::Parameters() :
    map(this, false),
    envMap(this, false),
    lightMap(this, false),
    bumpMap(this, false),
    normalMap(this, false),
    specularMap(this, false),

    vertexColors(this, false),

    fog(this),
    useFog(this, false),
    fogExp(this, false),

    sizeAttenuation(this, true),

    skinning(this),
    maxBones(this),
    useVertexTexture(this),

    morphTargets(this, false),
    // morphNormals: material.morphNormals,
    // maxMorphTargets: this.maxMorphTargets,
    // maxMorphNormals: this.maxMorphNormals,

    maxDirLights(this, 0),
    maxPointLights(this, 0),
    maxSpotLights(this, 0),
    maxHemiLights(this, 0),

    maxShadows(this),
    shadowMapEnabled(this),
    // shadowMapType: this.shadowMapType,
    // shadowMapDebug: this.shadowMapDebug,
    // shadowMapCascade: this.shadowMapCascade,

    alphaTest(this),
    metal(this),
    perPixel(this),
    // wrapAround: material.wrapAround,
    doubleSided(this),
    flipSided(this)
{
}

// Map three.js constants to WebGL constants

int paramThreeToGL ( int p ) {

    if ( p == kRepeatWrapping ) return GL_REPEAT;
    if ( p == kClampToEdgeWrapping ) return GL_CLAMP_TO_EDGE;
    if ( p == kMirroredRepeatWrapping ) return GL_MIRRORED_REPEAT;

    if ( p == kNearestFilter ) return GL_NEAREST;
    if ( p == kNearestMipMapNearestFilter ) return GL_NEAREST_MIPMAP_NEAREST;
    if ( p == kNearestMipMapLinearFilter ) return GL_NEAREST_MIPMAP_LINEAR;

    if ( p == kLinearFilter ) return GL_LINEAR;
    if ( p == kLinearMipMapNearestFilter ) return GL_LINEAR_MIPMAP_NEAREST;
    if ( p == kLinearMipMapLinearFilter ) return GL_LINEAR_MIPMAP_LINEAR;

    if ( p == kUnsignedByteType ) return GL_UNSIGNED_BYTE;
    if ( p == kUnsignedShort4444Type ) return GL_UNSIGNED_SHORT_4_4_4_4;
    if ( p == kUnsignedShort5551Type ) return GL_UNSIGNED_SHORT_5_5_5_1;
    if ( p == kUnsignedShort565Type ) return GL_UNSIGNED_SHORT_5_6_5;

    if ( p == kByteType ) return GL_BYTE;
    if ( p == kShortType ) return GL_SHORT;
    if ( p == kUnsignedShortType ) return GL_UNSIGNED_SHORT;
    if ( p == kIntType ) return GL_INT;
    if ( p == kUnsignedIntType ) return GL_UNSIGNED_INT;
    if ( p == kFloatType ) return GL_FLOAT;

    if ( p == kAlphaFormat ) return GL_ALPHA;
    if ( p == kRGBFormat ) return GL_RGB;
    if ( p == kRGBAFormat ) return GL_RGBA;
    if ( p == kLuminanceFormat ) return GL_LUMINANCE;
    if ( p == kLuminanceAlphaFormat ) return GL_LUMINANCE_ALPHA;

    if ( p == kAddEquation ) return GL_FUNC_ADD;
    if ( p == kSubtractEquation ) return GL_FUNC_SUBTRACT;
    if ( p == kReverseSubtractEquation ) return GL_FUNC_REVERSE_SUBTRACT;

    if ( p == kZeroFactor ) return GL_ZERO;
    if ( p == kOneFactor ) return GL_ONE;
    if ( p == kSrcColorFactor ) return GL_SRC_COLOR;
    if ( p == kOneMinusSrcColorFactor ) return GL_ONE_MINUS_SRC_COLOR;
    if ( p == kSrcAlphaFactor ) return GL_SRC_ALPHA;
    if ( p == kOneMinusSrcAlphaFactor ) return GL_ONE_MINUS_SRC_ALPHA;
    if ( p == kDstAlphaFactor ) return GL_DST_ALPHA;
    if ( p == kOneMinusDstAlphaFactor ) return GL_ONE_MINUS_DST_ALPHA;

    if ( p == kDstColorFactor ) return GL_DST_COLOR;
    if ( p == kOneMinusDstColorFactor ) return GL_ONE_MINUS_DST_COLOR;
    if ( p == kSrcAlphaSaturateFactor ) return GL_SRC_ALPHA_SATURATE;

    // if ( _glExtensionCompressedTextureS3TC !== undefined ) {

    //     if ( p == kRGB_S3TC_DXT1_Format ) return _glExtensionCompressedTextureS3TC.COMPRESSED_RGB_S3TC_DXT1_EXT;
    //     if ( p == kRGBA_S3TC_DXT1_Format ) return _glExtensionCompressedTextureS3TC.COMPRESSED_RGBA_S3TC_DXT1_EXT;
    //     if ( p == kRGBA_S3TC_DXT3_Format ) return _glExtensionCompressedTextureS3TC.COMPRESSED_RGBA_S3TC_DXT3_EXT;
    //     if ( p == kRGBA_S3TC_DXT5_Format ) return _glExtensionCompressedTextureS3TC.COMPRESSED_RGBA_S3TC_DXT5_EXT;

    // }

    return 0;

}


void initDirectBuffers( BufferGeometryRef geometry )
{
    for (auto& kv : geometry->attributes) {
        GLenum type = (kv.first == "index") ? GL_ELEMENT_ARRAY_BUFFER : GL_ARRAY_BUFFER;
        auto attribute = kv.second;

        GLuint& buffer = attribute->buffer();
        MemoryBuffer& data = *attribute;

        glGenBuffers(1, &buffer);
        glBindBuffer(type, buffer);
        glBufferData(type, data.size(), data.bytePtr(), GL_STATIC_DRAW);
        printf("Initialized buffer %s %d size %d ptr %p\n", kv.first.c_str(), buffer, (int) data.size(), data.bytePtr());
    }
}

void addBuffer( vector<Scene::GLES2Object>& objlist, BufferGeometryRef buffer, Object3DRef object )
{
    Scene::GLES2Object go;
    go.buffer = buffer;
    go.object = object;

    printf("Adding GLES2Object buffer %p object %p\n", &buffer, object.get());
    objlist.push_back(go);
}

void addBufferImmediate( vector<Scene::GLES2Object>& objlist, Object3DRef object )
{
    Scene::GLES2Object go;
    go.object = object;

    objlist.push_back(go);
}


// Objects adding
void addObject( Object3DRef object, SceneRef scene )
{
    if ( !object->__webglInit ) {

        object->__webglInit = true;

        auto geometry = object->geometry();

        if ( geometry && !geometry->__webglInit ) {
            geometry->__webglInit = true;
            // object->geometry->addEventListener( 'dispose', onGeometryDispose );
        }

        if (geometry) {
            initDirectBuffers(geometry);
        }

        if (!object->__webglActive) {
            if (object->tag() == kMesh) {
                addBuffer(scene->__gles2Objects, geometry, object);
            }
            else if (object->tag() == kLine || object->tag() == kParticleSystem) {
                addBuffer(scene->__gles2Objects, geometry, object);
            }
            // XXX add handlers for other object types

            object->__webglActive = true;
        }
    }

}

//  used for custom dynamic attributes
void setDirectBuffers ( BufferGeometryRef geometry, GLenum hint, bool dispose )
{

    for ( auto& kv : geometry->attributes ) {

        const string& attributeName = kv.first;
        AttributeRef attributeItem = kv.second;

        if (attributeItem->needsUpdate()) {

            GLenum type = ( attributeName == "index" ) ? GL_ELEMENT_ARRAY_BUFFER : GL_ARRAY_BUFFER;
            glBindBuffer(type, attributeItem->buffer());
            glBufferData(type, attributeItem->size(), attributeItem->bytePtr(), hint);

            attributeItem->needsUpdate() = false;

        }

        if ( dispose && ! attributeItem->dynamic() ) {

            attributeItem->free();

        }

    }

}

// Objects updates

void updateObject( Object3DRef object )
{
    // XXX support lines, particle systems
    BufferGeometryRef geometry = object->geometry();
    if (geometry) {
        setDirectBuffers( geometry, GL_DYNAMIC_DRAW, !geometry->dynamic() );
    }
}

// Objects updates - custom attributes check

bool areCustomAttributesDirty( MaterialRef material ) {

    for ( auto& kv : material->attributes ) {

        if ( (kv.second)->needsUpdate() ) return true;

    }

    return false;

}

void clearCustomAttributes( MaterialRef material ) {

    for ( auto& kv : material->attributes ) {

        (kv.second)->needsUpdate() = false;

    }

}

// Objects removal


void removeInstances( vector<Scene::GLES2Object>& objlist, Object3DRef object )
{
    objlist.erase(remove_if(objlist.begin(), objlist.end(),
                [object](Scene::GLES2Object& go) { return object == go.object; }), objlist.end());
}

void removeObject( Object3DRef object, SceneRef scene )
{

    auto tag = object->tag();
    if ( tag == kMesh  ||
         tag == kParticleSystem ||
         tag == kLine ) {

        removeInstances( scene->__gles2Objects, object );

    } /* else if ( object instanceof THREE.Sprite ) {
//
//         removeInstancesDirect( scene.__webglSprites, object );
//
//     } else if ( object instanceof THREE.LensFlare ) {
//
//         removeInstancesDirect( scene.__webglFlares, object );
//
//     } else if ( object instanceof THREE.ImmediateRenderObject || object.immediateRenderCallback ) {
//
//         removeInstances( scene.__gles2Objects, object );
//
//     }
//     */

     object->__webglActive = false;
}


// function removeInstancesDirect( objlist, object ) {
//
//     for ( var o = objlist.length - 1; o >= 0; o -- ) {
//
//         if ( objlist[ o ] === object ) {
//
//             objlist.splice( o, 1 );
//
//         }
//
//     }
//
// };

void setupMatrices ( Object3DRef object, CameraRef camera ) {

    object->_modelViewMatrix.multiplyMatrices( camera->matrixWorldInverse(), object->matrixWorld() );
    object->_normalMatrix.getNormalMatrix( object->_modelViewMatrix );

}

void setColorGamma( Color* dst, Color& color, float intensitySq )
{

    dst->r = color.r * color.r * intensitySq;
    dst->g = color.g * color.g * intensitySq;
    dst->b = color.b * color.b * intensitySq;

}

void setColorLinear( Color* dst, Color& color, float intensity )
{

    dst->r = color.r * intensity;
    dst->g = color.g * intensity;
    dst->b = color.b * intensity;

}


void GLES2Renderer::setupLights ( ProgramRef program, vector<LightRef>& lights )
{
//    var l, ll, light, n,
    float r = 0, g = 0, b = 0;
//    color, skyColor, groundColor,
//    intensity,  intensitySq,
//    position,
//    distance,
//
    auto& zlights = _lights;

    auto& dirColors = zlights.directional.colors;
    auto& dirPositions = zlights.directional.positions;

    auto& pointColors = zlights.point.colors;
    auto& pointPositions = zlights.point.positions;
    auto& pointDistances = zlights.point.distances;

    auto& spotColors = zlights.spot.colors;
    auto& spotPositions = zlights.spot.positions;
    auto& spotDistances = zlights.spot.distances;
    auto& spotDirections = zlights.spot.directions;
    auto& spotAnglesCos = zlights.spot.anglesCos;
    auto& spotExponents = zlights.spot.exponents;

    auto& hemiSkyColors = zlights.hemi.skyColors;
    auto& hemiGroundColors = zlights.hemi.groundColors;
    auto& hemiPositions = zlights.hemi.positions;

    float dirLength = 0;
    float pointLength = 0;
    float spotLength = 0;
    float hemiLength = 0;

    uint32_t dirCount = 0;
    uint32_t pointCount = 0;
    uint32_t spotCount = 0;
    uint32_t hemiCount = 0;

    dirPositions.clear();
    dirColors.clear();

    pointPositions.clear();
    pointColors.clear();
    pointDistances.clear();

    spotColors.clear();
    spotPositions.clear();
    spotDistances.clear();
    spotDirections.clear();
    spotAnglesCos.clear();
    spotExponents.clear();

    hemiPositions.clear();
    hemiSkyColors.clear();
    hemiGroundColors.clear();

    for (LightRef light : lights ) {

        if ( light->onlyShadow() ) continue;

        Color color = light->color();
        float intensity = light->intensity();
        float distance = light->distance();

        if ( light->type() == kAmbient ) {

            if ( ! light->visible() ) continue;

            if ( gammaInput ) {

                r += color.r * color.r;
                g += color.g * color.g;
                b += color.b * color.b;

            } else {

                r += color.r;
                g += color.g;
                b += color.b;

            }

        } else if ( light->type() == kDirectional ) {

            dirCount += 1;

            if ( ! light->visible() ) continue;

            _direction.getPositionFromMatrix( light->matrixWorld() );
            _vector3.getPositionFromMatrix( light->target()->matrixWorld() );
            _direction.sub( _vector3 );
            _direction.normalize();

            // skip lights with undefined direction
            // these create troubles in OpenGL (making pixel black)

            if ( _direction.x() == 0 && _direction.y() == 0 && _direction.z() == 0 ) continue;

            dirPositions.push_back(_direction);

            Color dirColor;

            if ( gammaInput ) {

                setColorGamma( &dirColor, color, intensity * intensity );

            } else {

                setColorLinear( &dirColor, color, intensity );

            }

            dirColors.push_back(dirColor);

            dirLength += 1;

        } else if ( light->type() == kPoint ) {

            pointCount += 1;

            if ( ! light->visible() ) continue;

            Color pointColor;

            if ( gammaInput ) {

                setColorGamma( &pointColor, color, intensity * intensity );

            } else {

                setColorLinear( &pointColor, color, intensity );

            }

            pointColors.push_back( pointColor );

            _vector3.getPositionFromMatrix( light->matrixWorld() );

            pointDistances.push_back(distance);

            pointLength += 1;

        } else if ( light->type() == kSpot ) {

            SpotLightRef spotlight = static_pointer_cast<SpotLight>(light);

            spotCount += 1;

            if ( ! spotlight->visible() ) continue;

            Color spotColor;

            if ( gammaInput ) {

                setColorGamma( &spotColor, color, intensity * intensity );

            } else {

                setColorLinear( &spotColor, color, intensity );

            }

            spotColors.push_back( spotColor );

            _vector3.getPositionFromMatrix( spotlight->matrixWorld() );

            spotPositions.push_back( _vector3 );

            spotDistances.push_back( distance );

            _direction.copy( _vector3 );
            _vector3.getPositionFromMatrix( spotlight->target()->matrixWorld() );
            _direction.sub( _vector3 );
            _direction.normalize();

            spotDirections.push_back( _direction );

            spotAnglesCos.push_back( cos( spotlight->angle() ) );
            spotExponents.push_back( spotlight->exponent() );

            spotLength += 1;

        } else if ( light->type() == kHemisphere ) {

            hemiCount += 1;

            HemisphereLightRef hlight = static_pointer_cast<HemisphereLight>(light);

            if ( ! hlight->visible() ) continue;

            _direction.getPositionFromMatrix( hlight->matrixWorld() );
            _direction.normalize();

            // skip lights with undefined direction
            // these create troubles in OpenGL (making pixel black)

            if ( _direction.x() == 0 && _direction.y() == 0 && _direction.z() == 0 ) continue;

            hemiPositions.push_back(_direction);

            auto& skyColor = hlight->color();
            auto& groundColor = hlight->groundColor();

            if ( gammaInput ) {

                float intensitySq = intensity * intensity;

                Color c;
                setColorGamma( &c, skyColor, intensitySq );
                hemiSkyColors.push_back(c);
                setColorGamma( &c, groundColor, intensitySq );
                hemiGroundColors.push_back(c);

            } else {

                Color c;
                setColorLinear( &c, skyColor, intensity );
                hemiSkyColors.push_back(c);
                setColorLinear( &c, groundColor, intensity );
                hemiGroundColors.push_back(c);

            }

            hemiLength += 1;
        }

    }

    // null eventual remains from removed lights
    // (this is to avoid if in shader)

    for ( int l = dirLength, ll = Math::max( uint32_t(dirColors.size()), dirCount ); l < ll; l ++ )
        dirColors[ l ] = Color(0);
    for ( int l = pointLength, ll = Math::max( uint32_t(pointColors.size()), pointCount ); l < ll; l ++ )
        pointColors[ l ] = Color(0);
    for ( int l = spotLength, ll = Math::max( uint32_t(spotColors.size()), spotCount ); l < ll; l ++ )
        spotColors[ l ] = 0.0;
    for ( int l = hemiLength, ll = Math::max( uint32_t(hemiSkyColors.size()), hemiCount ); l < ll; l ++ )
        hemiSkyColors[ l ] = Color(0);
    for ( int l = hemiLength, ll = Math::max( uint32_t(hemiGroundColors.size()), hemiCount ); l < ll; l ++ )
        hemiGroundColors[ l ] = Color(0);

    zlights.directional.length = dirLength;
    zlights.point.length = pointLength;
    zlights.spot.length = spotLength;
    zlights.hemi.length = hemiLength;

    zlights.ambient[0] = r;
    zlights.ambient[1] = g;
    zlights.ambient[2] = b;

}

void GLES2Renderer::enableAttribute( GLuint attribute ) {
    // printf("enableAttribute %d\n", attribute);

    if ( ! _enabledAttributes[ attribute ] ) {

        glEnableVertexAttribArray( attribute );
        _enabledAttributes[ attribute ] = true;

    }

}

void GLES2Renderer::disableAttributes() {
    printf("disableAttributes");

    for ( auto& kv : _enabledAttributes ) {

        bool enabled = kv.second;

        if ( enabled ) {

            GLuint attribute = kv.first;
            glDisableVertexAttribArray( attribute );
            _enabledAttributes[ attribute ] = false;

        }

    }

}


void GLES2Renderer::renderBufferDirect( CameraRef camera, vector<LightRef>& lights, FogRef fog,
        MaterialRef material, BufferGeometryRef geometry, Object3DRef object )
{
    if ( !material->visible() ) return;

    // var linewidth, a, attribute;
    // var attributeItem, attributeName, attributePointer, attributeSize;

    ProgramRef program = setProgram( camera, lights, fog, material, object );

    bool updateBuffers = false;
    uint32_t wireframeBit = material->wireframe() ? 1 : 0;
    uint32_t geometryHash = ( geometry->id() * 0xffffff ) + ( program->id * 2 ) + wireframeBit;

    if ( geometryHash != _currentGeometryGroupHash ) {

        _currentGeometryGroupHash = geometryHash;
        updateBuffers = true;

    }

    if ( updateBuffers ) {

        disableAttributes();

    }

    // render mesh

    if ( object->tag() == kMesh ) {

        auto index = geometry->attribute( "index" );

        // indexed triangles

        if ( index ) {

            auto& offsets = geometry->offsets();

            // if there is more than 1 chunk
            // must set attribute pointers to use new offsets for each chunk
            // even if geometry and materials didn't change

            if ( offsets.size() > 1 ) updateBuffers = true;

            for ( auto& offset : offsets ) {

                int startIndex = offset.index;

                if ( updateBuffers ) {

                    for ( auto& kv : program->attributes ) {

                        GLint attributePointer = kv.second;

                        const string& attributeName = kv.first;
                        AttributeRef attributeItem = geometry->attribute(attributeName);

                        // printf("Update buffers: name %s attrib pointer %d item %p\n",
                        //         attributeName.c_str(), attributePointer, attributeItem.get());

                        if ( attributePointer >= 0 ) {

                            if ( attributeItem ) {

                                int attributeSize = attributeItem->itemSize;
                                glBindBuffer( GL_ARRAY_BUFFER, attributeItem->buffer());
                                enableAttribute( attributePointer );
                                glVertexAttribPointer( attributePointer, attributeSize, GL_FLOAT,
                                        false, 0, (GLvoid*) (startIndex * attributeSize * sizeof(float)) );

                            } /* else if ( material->defaultAttributeValues ) {

                                if ( material->defaultAttributeValues[ attributeName ].length === 2 ) {

                                    _gl.vertexAttrib2fv( attributePointer, material->defaultAttributeValues[ attributeName ] );

                                } else if ( material->defaultAttributeValues[ attributeName ].length === 3 ) {

                                    _gl.vertexAttrib3fv( attributePointer, material->defaultAttributeValues[ attributeName ] );

                                }

                            }
                                */

                        }

                    }

                    // indices

                    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, index->buffer() );

                }

                // render indexed triangles

                glDrawElements( GL_TRIANGLES, offset.count, GL_UNSIGNED_SHORT, (const GLvoid *) (offset.start * 2) ); // 2 bytes per Uint16

                info.render.calls ++;
                info.render.vertices += offset.count; // not really true, here vertices can be shared
                info.render.faces += offset.count / 3;

            }

        // non-indexed triangles

        } else {

            if ( updateBuffers ) {

                for ( auto& kv : program->attributes ) {

                    const string& attributeName = kv.first;

                    if ( attributeName == "index") continue;

                    GLint attributePointer = kv.second;

                    if ( attributePointer >= 0 ) {

                        AttributeRef attributeItem = geometry->attribute( attributeName );

                        if ( attributeItem ) {

                            int attributeSize = attributeItem->itemSize;
                            glBindBuffer( GL_ARRAY_BUFFER, attributeItem->buffer());
                            enableAttribute( attributePointer );
                            glVertexAttribPointer( attributePointer, attributeSize, GL_FLOAT, false, 0, 0 );

                        } /* else if ( material->defaultAttributeValues && material->defaultAttributeValues[ attributeName ] ) {

                            if ( material->defaultAttributeValues[ attributeName ].length === 2 ) {

                                _gl.vertexAttrib2fv( attributePointer, material->defaultAttributeValues[ attributeName ] );

                            } else if ( material->defaultAttributeValues[ attributeName ].length === 3 ) {

                                _gl.vertexAttrib3fv( attributePointer, material->defaultAttributeValues[ attributeName ] );

                            }

                        } */

                    }

                }

            }

            auto position = geometry->attribute( "position" );

            // render non-indexed triangles

            glDrawArrays( GL_TRIANGLES, 0, position->dataSize<Vector3>() );

            info.render.calls ++;
            info.render.vertices += position->dataSize<Vector3>();
            info.render.faces += position->dataSize<Vector3>() / 3;

        }

    // render particles

    } else if ( object->tag() == kParticleSystem ) {

        if ( updateBuffers ) {

            for ( auto& kv : program->attributes ) {

                const string& attributeName = kv.first;
                GLint attributePointer = kv.second;
                AttributeRef attributeItem = geometry->attribute( attributeName );

                if ( attributePointer >= 0 ) {

                    if ( attributeItem ) {

                        int attributeSize = attributeItem->itemSize;
                        glBindBuffer( GL_ARRAY_BUFFER, attributeItem->buffer() );
                        enableAttribute( attributePointer );
                        glVertexAttribPointer( attributePointer, attributeSize, GL_FLOAT, false, 0, 0 );

                    }/* else if ( material->defaultAttributeValues && material->defaultAttributeValues[ attributeName ] ) {

                        if ( material->defaultAttributeValues[ attributeName ].length === 2 ) {

                            _gl.vertexAttrib2fv( attributePointer, material->defaultAttributeValues[ attributeName ] );

                        } else if ( material->defaultAttributeValues[ attributeName ].length === 3 ) {

                            _gl.vertexAttrib3fv( attributePointer, material->defaultAttributeValues[ attributeName ] );

                        }

                    } */

                }

            }

        }

        // render particles

        AttributeRef position = geometry->attribute( "position" );

        if ( position ) {
            int count = position->dataSize<Vector3>();
            glDrawArrays( GL_POINTS, 0, count );

            info.render.calls ++;
            info.render.points += count;
        }


    } else if ( object->tag() == kLine ) {
        LineRef line = static_pointer_cast<Line>(object);

        if ( updateBuffers ) {

            // for ( attributeName in programAttributes ) {
            for ( auto& kv : program->attributes ) {

                const string& attributeName = kv.first;
                GLint attributePointer = kv.second;
                AttributeRef attributeItem = geometry->attribute( attributeName );

                if ( attributePointer >= 0 ) {

                    if ( attributeItem ) {

                        int attributeSize = attributeItem->itemSize;
                        glBindBuffer( GL_ARRAY_BUFFER, attributeItem->buffer() );
                        enableAttribute( attributePointer );
                        glVertexAttribPointer( attributePointer, attributeSize, GL_FLOAT, false, 0, 0 );

                    } /* else if ( material->defaultAttributeValues && material->defaultAttributeValues[ attributeName ] ) {

                        if ( material->defaultAttributeValues[ attributeName ].length === 2 ) {

                            _gl.vertexAttrib2fv( attributePointer, material->defaultAttributeValues[ attributeName ] );

                        } else if ( material->defaultAttributeValues[ attributeName ].length === 3 ) {

                            _gl.vertexAttrib3fv( attributePointer, material->defaultAttributeValues[ attributeName ] );

                        }

                    } */

                }

            }

        }

        // render lines

        auto primitives = ( line->type() == Line::kLineStrip ) ? GL_LINE_STRIP : GL_LINES;

        setLineWidth( material->linewidth() );

        AttributeRef position = geometry->attribute( "position" );

        int count = position->dataSize<Vector3>();
        glDrawArrays( primitives, 0, count );

        info.render.calls ++;
        info.render.points += count;

    }

}

GLuint GLES2Renderer::getTextureUnit()
{

    GLuint textureUnit = _usedTextureUnits;

    if ( textureUnit >= _maxTextures ) {

        printf( "WebGLRenderer: trying to use %d texture units while this GPU supports only %d\n",
                textureUnit, _maxTextures );

    }

    _usedTextureUnits += 1;

    return textureUnit;

}


void GLES2Renderer::loadUniformsGeneric ( Program& program, UniformMap& uniforms )
{
    // var uniform, value, type, location, texture, textureUnit, i, il, j, jl, offset;

    // for ( j = 0, jl = uniforms.length; j < jl; j ++ ) {
    for ( auto& kv : uniforms ) {

        const string& name = kv.first;
        UniformRef uniform = kv.second;

        GLint location = program.uniform(name);
        if ( location < 0 ) continue;

        uint32_t type = uniform->type();

        if ( type == 'i' ) { // single integer

            glUniform1i( location, uniform->value<GLint>() );

        } else if ( type == 'f' ) { // single float

            glUniform1f( location, uniform->value<GLfloat>() );

        } else if ( type == 'v2' ) { // single THREE.Vector2

            auto& value = uniform->value<Vector2>();
            glUniform2f( location, value.x(), value.y() );

        } else if ( type == 'v3' ) { // single THREE.Vector3

            auto& value = uniform->value<Vector3>();
            glUniform3f( location, value.x(), value.y(), value.z() );

        } else if ( type == 'v4' ) { // single THREE.Vector4

            auto& value = uniform->value<Vector4>();
            glUniform4f( location, value.x(), value.y(), value.z(), value.w() );

        } else if ( type == 'c' ) { // single THREE.Color

            auto& value = uniform->value<Color>();
            glUniform3f( location, value.r, value.g, value.b );

        } /* else if ( type == 'iv1' ) { // flat array of integers (JS or typed array)

            glUniform1iv( location, value );

        } else if ( type == 'iv' ) { // flat array of integers with 3 x N size (JS or typed array)

            glUniform3iv( location, value );

        } else if ( type == 'fv1' ) { // flat array of floats (JS or typed array)

            glUniform1fv( location, value );

        }*/ else if ( type == 'fv' ) { // flat array of floats with 3 x N size (JS or typed array)

            auto& value = uniform->array<Vector3>();
            if (value.size() > 0) {
                glUniform3fv( location, value.size(), (GLfloat*) &value[0] );
                // printf("Updated fv array location %d size %d pointer %p [0] (%f,%f,%f)\n",
                //         location, value.size(), &value[0], value[0].x(), value[0].y(), value[0].z());
            }

        } /* else if ( type == 'v2v' ) { // array of THREE.Vector2

            if ( uniform._array == undefined ) {

                uniform._array = new Float32Array( 2 * value.length );

            }

            for ( i = 0, il = value.length; i < il; i ++ ) {

                offset = i * 2;

                uniform._array[ offset ]     = value[ i ].x;
                uniform._array[ offset + 1 ] = value[ i ].y;

            }

            _gl.uniform2fv( location, uniform._array );

        } else if ( type == 'v3v' ) { // array of THREE.Vector3

            if ( uniform._array == undefined ) {

                uniform._array = new Float32Array( 3 * value.length );

            }

            for ( i = 0, il = value.length; i < il; i ++ ) {

                offset = i * 3;

                uniform._array[ offset ]     = value[ i ].x;
                uniform._array[ offset + 1 ] = value[ i ].y;
                uniform._array[ offset + 2 ] = value[ i ].z;

            }

            _gl.uniform3fv( location, uniform._array );

        } else if ( type == 'v4v' ) { // array of THREE.Vector4

            if ( uniform._array == undefined ) {

                uniform._array = new Float32Array( 4 * value.length );

            }

            for ( i = 0, il = value.length; i < il; i ++ ) {

                offset = i * 4;

                uniform._array[ offset ]     = value[ i ].x;
                uniform._array[ offset + 1 ] = value[ i ].y;
                uniform._array[ offset + 2 ] = value[ i ].z;
                uniform._array[ offset + 3 ] = value[ i ].w;

            }

            _gl.uniform4fv( location, uniform._array );

        } else if ( type == 'm4') { // single THREE.Matrix4

            if ( uniform._array == undefined ) {

                uniform._array = new Float32Array( 16 );

            }

            value.flattenToArray( uniform._array );
            _gl.uniformMatrix4fv( location, false, uniform._array );

        } else if ( type == 'm4v' ) { // array of THREE.Matrix4

            if ( uniform._array == undefined ) {

                uniform._array = new Float32Array( 16 * value.length );

            }

            for ( i = 0, il = value.length; i < il; i ++ ) {

                value[ i ].flattenToArrayOffset( uniform._array, i * 16 );

            }

            _gl.uniformMatrix4fv( location, false, uniform._array );

        } */ else if ( type == 't' ) { // single THREE.Texture (2d or cube)

            TextureRef texture = uniform->ref<Texture>();
            GLuint textureUnit = getTextureUnit();

            glUniform1i( location, textureUnit );

            if ( !texture ) continue;

            /* if ( texture.image instanceof Array && texture.image.length == 6 ) {

                setCubeTexture( texture, textureUnit );

            } else if ( texture instanceof THREE.WebGLRenderTargetCube ) {

                setCubeTextureDynamic( texture, textureUnit );

            } else { */

                setTexture( texture, textureUnit );

                /*
            }
            */

        } else if ( type == 'tv' ) { // array of THREE.Texture (2d)

            GLint texUnits[16];

            auto textures = uniform->refArray<Texture>();
            for( int i = 0, il = textures.size(); i < il; i ++ ) {

                texUnits[i] = getTextureUnit();

            }

            glUniform1iv( location, textures.size(), texUnits );

            for( int i = 0, il = textures.size(); i < il; i ++ ) {

                auto texture = textures[ i ];
                auto textureUnit = texUnits[ i ];

                if ( !texture ) continue;

                setTexture( texture, textureUnit );

            }

        } else {

            printf( "RESIN::GLES2Renderer: Unknown uniform type: %x", type );

        }

    }
};

void loadUniformsMatrices ( LocationMap uniforms, Object3DRef object ) {

    glUniformMatrix4fv( uniforms["modelViewMatrix"], 1, false, (float*) &(object->_modelViewMatrix.m) );

    auto nm = uniforms.find("normalMatrix");
    if ( nm != uniforms.end() ) {

        glUniformMatrix3fv( nm->second, 1, false, (float*) &(object->_normalMatrix.m) );

    }

};

// Uniforms (refresh uniforms objects)

void GLES2Renderer::refreshUniformsCommon ( UniformMap& uniforms, MaterialRef material )
{

    // XXX refactor, use Uniform->set XXX
    uniforms["opacity"]->value<float>() = material->opacity();

    if ( gammaInput ) {

        uniforms["diffuse"]->value<Color>().copyGammaToLinear( material->color() );

    } else {

        uniforms["diffuse"]->value<Color>() = material->color();

    }

    uniforms["map"]->ref<Texture>() = material->map();
    uniforms["lightMap"]->ref<Texture>() = material->lightMap();
    uniforms["specularMap"]->ref<Texture>() = material->specularMap();

    // if ( material.bumpMap ) {

    //     uniforms.bumpMap.value = material.bumpMap;
    //     uniforms.bumpScale.value = material.bumpScale;

    // }

    // if ( material.normalMap ) {

    //     uniforms.normalMap.value = material.normalMap;
    //     uniforms.normalScale.value.copy( material.normalScale );

    // }

    // uv repeat and offset setting priorities
    //  1. color map
    //  2. specular map
    //  3. normal map
    //  4. bump map

    // var uvScaleMap;

    // if ( material.map ) {

    //     uvScaleMap = material.map;

    // } else if ( material.specularMap ) {

    //     uvScaleMap = material.specularMap;

    // } else if ( material.normalMap ) {

    //     uvScaleMap = material.normalMap;

    // } else if ( material.bumpMap ) {

    //     uvScaleMap = material.bumpMap;

    // }

    // if ( uvScaleMap !== undefined ) {

    //     var offset = uvScaleMap.offset;
    //     var repeat = uvScaleMap.repeat;

    //     uniforms.offsetRepeat.value.set( offset.x, offset.y, repeat.x, repeat.y );

    // }

    // uniforms.envMap.value = material.envMap;
    // uniforms.flipEnvMap.value = ( material.envMap instanceof THREE.WebGLRenderTargetCube ) ? 1 : -1;

    if ( gammaInput ) {

        //uniforms.reflectivity.value = material.reflectivity * material.reflectivity;
        uniforms["reflectivity"]->value<float>() = material->reflectivity();

    } else {

        uniforms["reflectivity"]->value<float>() = material->reflectivity();

    }

    uniforms["refractionRatio"]->value<float>() = material->refractionRatio();
    uniforms["combine"]->value<int32_t>() = material->combine();
    uniforms["useRefract"]->value<int32_t>() = 0;
    // uniforms["useRefract"]->value<int32_t> = int32_t(material->envMap && material->envMap.mapping instanceof THREE.CubeRefractionMapping);

};

void GLES2Renderer::refreshUniformsPhong ( UniformMap& uniforms, MeshPhongMaterialRef material )
{

    uniforms["shininess"]->value<float>() = material->shininess();

    if ( gammaInput ) {

        uniforms["ambient"]->value<Color>().copyGammaToLinear( material->ambient() );
        uniforms["emissive"]->value<Color>().copyGammaToLinear( material->emissive() );
        uniforms["specular"]->value<Color>().copyGammaToLinear( material->specular() );

    } else {

        uniforms["ambient"]->value<Color>() = material->ambient();
        uniforms["emissive"]->value<Color>() = material->emissive();
        uniforms["specular"]->value<Color>() = material->specular();

    }

    if ( material->wrapAround() ) {

        uniforms["wrapRGB"]->value<Vector3>().copy( material->wrapRGB() );

    }

}

void GLES2Renderer::refreshUniformsLine ( UniformMap& uniforms, LineBasicMaterialRef material )
{

    uniforms["diffuse"]->value<Color>() = material->color();
    uniforms["opacity"]->value<float>() = material->opacity();

}

void refreshUniformsLights ( UniformMap& uniforms, RendererLights& lights )
{

    uniforms["ambientLightColor"]->array<float>() = lights.ambient;

    uniforms["directionalLightColor"]->array<Color>() = lights.directional.colors;
    uniforms["directionalLightDirection"]->array<Vector3>() = lights.directional.positions;

    uniforms["pointLightColor"]->array<Color>() = lights.point.colors;
    uniforms["pointLightPosition"]->array<Vector3>() = lights.point.positions;
    uniforms["pointLightDistance"]->array<float>() = lights.point.distances;

    uniforms["spotLightColor"]->array<Color>() = lights.spot.colors;
    uniforms["spotLightPosition"]->array<Vector3>() = lights.spot.positions;
    uniforms["spotLightDistance"]->array<float>() = lights.spot.distances;
    uniforms["spotLightDirection"]->array<Vector3>() = lights.spot.directions;
    uniforms["spotLightAngleCos"]->array<float>() = lights.spot.anglesCos;
    uniforms["spotLightExponent"]->array<float>() = lights.spot.exponents;

    uniforms["hemisphereLightSkyColor"]->array<Color>() = lights.hemi.skyColors;
    uniforms["hemisphereLightGroundColor"]->array<Color>() = lights.hemi.groundColors;
    uniforms["hemisphereLightDirection"]->array<Vector3>() = lights.hemi.positions;

}

void GLES2Renderer::refreshUniformsParticle ( UniformMap& uniforms, ParticleBasicMaterialRef material )
{
    uniforms["psColor"]->value<Color>() = material->color();
    uniforms["opacity"]->value<float>() = material->opacity();
    float psize = material->size();
    uniforms["size"]->value<float>() = material->size();
    // XXX was _canvas.height
    uniforms["scale"]->value<float>() = _height / 2.0; // TODO: Cache this.

    uniforms["map"]->ref<Texture>() = material->map();
}

struct LightAlloc {
    int directional;
    int point;
    int spot;
    int hemi;

    LightAlloc() : directional(0), point(0), spot(0), hemi(0)
    {}
};

LightAlloc allocateLights( vector<LightRef>& lights )
{
    LightAlloc la;

    for ( LightRef light : lights ) {

        if ( light->onlyShadow() ) continue;

        if ( light->type() == kDirectional ) la.directional ++;
        if ( light->type() == kPoint ) la.point ++;
        if ( light->type() == kSpot ) la.spot ++;
        if ( light->type() == kHemisphere ) la.hemi ++;

    }

    return la;
}


ProgramRef GLES2Renderer::setProgram( CameraRef camera, vector<LightRef>& lights, FogRef fog,
        MaterialRef material, Object3DRef object )
{
    _usedTextureUnits = 0;

    if ( material->needsUpdate() ) {

        if ( material->program() ) deallocateMaterial( material );

        initMaterial( material, lights, fog, object );
        material->needsUpdate() = false;

    }

//     if ( material->morphTargets ) {
//
//         if ( ! object.__webglMorphTargetInfluences ) {
//
//             object.__webglMorphTargetInfluences = new Float32Array( _this.maxMorphTargets );
//
//         }
//
//     }
//
    bool refreshMaterial = false;

    ProgramRef program = material->program();
    LocationMap& p_uniforms = program->uniforms;
    UniformMap& m_uniforms = material->uniforms;

    if ( program != _currentProgram ) {

        glUseProgram( program->object );
        _currentProgram = program;

        refreshMaterial = true;

    }

    if ( material->id() != _currentMaterialId ) {

        _currentMaterialId = material->id();
        refreshMaterial = true;

    }

    if ( refreshMaterial || camera.get() != _currentCamera ) {

        glUniformMatrix4fv(p_uniforms["projectionMatrix"], 1, false, (float*) &(camera->projectionMatrix().m));

        if ( camera.get() != _currentCamera ) _currentCamera = camera.get();

    }
//
//     // skinning uniforms must be set even if material didn't change
//     // auto-setting of texture unit for bone texture must go before other textures
//     // not sure why, but otherwise weird things happen
//
//     if ( material->skinning ) {
//
//         if ( _supportsBoneTextures && object.useVertexTexture ) {
//
//             if ( p_uniforms.boneTexture !== null ) {
//
//                 var textureUnit = getTextureUnit();
//
//                 _gl.uniform1i( p_uniforms.boneTexture, textureUnit );
//                 _this.setTexture( object.boneTexture, textureUnit );
//
//             }
//
//             if ( p_uniforms.boneTextureWidth !== null ) {
//
//                 _gl.uniform1i( p_uniforms.boneTextureWidth, object.boneTextureWidth );
//
//             }
//
//             if ( p_uniforms.boneTextureHeight !== null ) {
//
//                 _gl.uniform1i( p_uniforms.boneTextureHeight, object.boneTextureHeight );
//
//             }
//
//         } else {
//
//             if ( p_uniforms.boneGlobalMatrices !== null ) {
//
//                 _gl.uniformMatrix4fv( p_uniforms.boneGlobalMatrices, false, object.boneMatrices );
//
//             }
//
//         }
//
//     }
//
    if ( refreshMaterial ) {

        // refresh uniforms common to several materials

        if ( fog && material->fog() ) {

            // refreshUniformsFog( m_uniforms, fog );

        }

        if ( material->tag() == kMeshPhongMaterial ||
             material->tag() == kMeshLambertMaterial /* ||
             material->lights() */ ) {

            if ( _lightsNeedUpdate ) {

                setupLights( program, lights );
                _lightsNeedUpdate = false;

            }

            refreshUniformsLights( m_uniforms, _lights );

        }

        if ( material->tag() == kMeshBasicMaterial ||
             material->tag() == kMeshLambertMaterial ||
             material->tag() == kMeshPhongMaterial ) {

            refreshUniformsCommon( m_uniforms, material );

        }

        // refresh single material specific uniforms

        if ( material->tag() == kLineBasicMaterial ) {

            refreshUniformsLine( m_uniforms, static_pointer_cast<LineBasicMaterial>(material) );

        } else if ( material->tag() == kMeshPhongMaterial ) {

            refreshUniformsPhong( m_uniforms, static_pointer_cast<MeshPhongMaterial>(material) );

        } else if ( material->tag() == kParticleBasicMaterial ) {

            refreshUniformsParticle( m_uniforms, static_pointer_cast<ParticleBasicMaterial>(material) );

        }

//
//         if ( material instanceof THREE.LineBasicMaterial ) {
//
//             refreshUniformsLine( m_uniforms, material );
//
//         } else if ( material instanceof THREE.LineDashedMaterial ) {
//
//             refreshUniformsLine( m_uniforms, material );
//             refreshUniformsDash( m_uniforms, material );
//
//         } else if ( material instanceof THREE.ParticleBasicMaterial ) {
//
//             refreshUniformsParticle( m_uniforms, material );
//
//         } else if ( material instanceof THREE.MeshPhongMaterial ) {
//
//             refreshUniformsPhong( m_uniforms, material );
//
//         } else if ( material instanceof THREE.MeshLambertMaterial ) {
//
//             refreshUniformsLambert( m_uniforms, material );
//
//         } else if ( material instanceof THREE.MeshDepthMaterial ) {
//
//             m_uniforms.mNear.value = camera->near;
//             m_uniforms.mFar.value = camera->far;
//             m_uniforms.opacity.value = material->opacity;
//
//         } else if ( material instanceof THREE.MeshNormalMaterial ) {
//
//             m_uniforms.opacity.value = material->opacity;
//
//         }
//
//         if ( object.receiveShadow && ! material->_shadowPass ) {
//
//             refreshUniformsShadow( m_uniforms, lights );
//
//         }
//
        // load common uniforms

        loadUniformsGeneric( *program, material->uniforms );

        // load material specific uniforms
        // (shader material also gets them for the sake of genericity)

        if ( material->tag() == kShaderMaterial ||
             material->tag() == kMeshPhongMaterial ||
             material->envMap() ) {

            auto loc = p_uniforms.find("cameraPosition");
            if ( loc != p_uniforms.end() ) {

                Vector3 v;
                v.getPositionFromMatrix( camera->matrixWorld() );
                glUniform3f( loc->second, v.x(), v.y(), v.z() );

            }

        }

        if ( material->tag() == kMeshPhongMaterial ||
             material->tag() == kMeshLambertMaterial ||
             material->tag() == kShaderMaterial ||
             material->skinning() ) {

            auto loc = p_uniforms.find("viewMatrix");
            if ( loc != p_uniforms.end() ) {

                glUniformMatrix4fv( loc->second, 1, false, (float*) &(camera->matrixWorldInverse().m) );

            }

        }

    }

    loadUniformsMatrices( p_uniforms, object );

    auto mm = p_uniforms.find("modelMatrix");
    if ( mm != p_uniforms.end() ) {

        glUniformMatrix4fv( mm->second, 1, false, (float*) &(object->matrixWorld().m) );

    }

    return program;
}

void GLES2Renderer::initMaterial( MaterialRef material, vector<LightRef>& lights, FogRef fog, Object3DRef object )
{
    string shaderID = material->shaderID();

//    material->addEventListener( 'dispose', onMaterialDispose );
//
//    var u, a, identifiers, i, parameters, maxLightCount, maxBones, maxShadows, shaderID;
//
//    if ( material instanceof THREE.MeshDepthMaterial ) {
//
//        shaderID = 'depth';
//
//    } else if ( material instanceof THREE.MeshNormalMaterial ) {
//
//        shaderID = 'normal';
//
//    } else if ( material instanceof THREE.MeshBasicMaterial ) {
//
//        shaderID = 'basic';
//
//    } else if ( material instanceof THREE.MeshLambertMaterial ) {
//
//        shaderID = 'lambert';
//
//    } else if ( material instanceof THREE.MeshPhongMaterial ) {
//
//        shaderID = 'phong';
//
//    } else if ( material instanceof THREE.LineBasicMaterial ) {
//
//        shaderID = 'basic';
//
//    } else if ( material instanceof THREE.LineDashedMaterial ) {
//
//        shaderID = 'dashed';
//
//    } else if ( material instanceof THREE.ParticleBasicMaterial ) {
//
//        shaderID = 'particle_basic';
//
//    }
//
//   if ( !shaderID.empty() ) {
//
//       setMaterialShaders( material, THREE.ShaderLib[ shaderID ] );
//
//   }

    // heuristics to create shader parameters according to lights in the scene
    // (not to blow over maxLights budget)

    auto maxLightCount = allocateLights( lights );

    // maxShadows = allocateShadows( lights );

    // maxBones = allocateBones( object );


    auto parameters = Parameters()
        .map(bool(material->map()))
        .envMap(bool(material->envMap()))
        .lightMap(bool(material->lightMap()))
        .bumpMap(false)

        .vertexColors(material->vertexColors())

        .fog(fog)
        .useFog(material->fog())
        .fogExp(false)

        .sizeAttenuation(material->sizeAttenuation())

        .skinning(material->skinning())

        .morphTargets(material->morphTargets())

        .maxDirLights(maxLightCount.directional)
        .maxPointLights(maxLightCount.point)
        .maxSpotLights(maxLightCount.spot)
        .maxHemiLights(maxLightCount.hemi)

        .alphaTest(material->alphaTest())
        .metal(material->metal())
        .perPixel(material->perPixel())
        .doubleSided(material->side() == kDoubleSide)
        .flipSided(material->side() == kBackSide)
        ;

//    parameters = {
//
//     map: !!material->map,
//     envMap: !!material->envMap,
//     lightMap: !!material->lightMap,
//     bumpMap: !!material->bumpMap,
//     normalMap: !!material->normalMap,
//     specularMap: !!material->specularMap,
//
//     vertexColors: material->vertexColors,
//
//     fog: fog,
//     useFog: material->fog,
//     fogExp: fog instanceof THREE.FogExp2,
//
//     sizeAttenuation: material->sizeAttenuation,
//
//     skinning: material->skinning,
//     maxBones: maxBones,
//     useVertexTexture: _supportsBoneTextures && object && object.useVertexTexture,
//
//     morphTargets: material->morphTargets,
//     morphNormals: material->morphNormals,
//     maxMorphTargets: this.maxMorphTargets,
//     maxMorphNormals: this.maxMorphNormals,
//
//     maxDirLights: maxLightCount.directional,
//     maxPointLights: maxLightCount.point,
//     maxSpotLights: maxLightCount.spot,
//     maxHemiLights: maxLightCount.hemi,
//
//     maxShadows: maxShadows,
//     shadowMapEnabled: this.shadowMapEnabled && object.receiveShadow,
//     shadowMapType: this.shadowMapType,
//     shadowMapDebug: this.shadowMapDebug,
//     shadowMapCascade: this.shadowMapCascade,
//
//     alphaTest: material->alphaTest,
//     metal: material->metal,
//     perPixel: material->perPixel,
//     wrapAround: material->wrapAround,
//     doubleSided: material->side === THREE.DoubleSide,
//     flipSided: material->side === THREE.BackSide
//
//    };
//

    material->program() = buildProgram( shaderID,
            material->fragmentShader(), material->vertexShader(),
            material->uniforms, material->attributes,
            material->defines(),
            parameters,
            material->index0AttributeName() );

//    var attributes = material->program.attributes;
//
//    if ( material->morphTargets ) {
//
//        material->numSupportedMorphTargets = 0;
//
//        var id, base = "morphTarget";
//
//        for ( i = 0; i < this.maxMorphTargets; i ++ ) {
//
//            id = base + i;
//
//            if ( attributes[ id ] >= 0 ) {
//
//                material->numSupportedMorphTargets ++;
//
//            }
//
//        }
//
//    }
//
//    if ( material->morphNormals ) {
//
//        material->numSupportedMorphNormals = 0;
//
//        var id, base = "morphNormal";
//
//        for ( i = 0; i < this.maxMorphNormals; i ++ ) {
//
//            id = base + i;
//
//            if ( attributes[ id ] >= 0 ) {
//
//                material->numSupportedMorphNormals ++;
//
//            }
//
//        }
//
//    }
//
//    material->uniformsList = [];
//
//    for ( u in material->uniforms ) {
//
//        material->uniformsList.push( [ material->uniforms[ u ], u ] );
//
//    }
}

GLuint getShader ( const string& type, const string& str )
{
    GLuint shader = 0;

    if ( type == "fragment" ) {

        shader = glCreateShader( GL_FRAGMENT_SHADER );

    } else if ( type == "vertex" ) {

        shader = glCreateShader( GL_VERTEX_SHADER );

    }

    auto src = str.c_str();
    glShaderSource( shader, 1, &src, 0 );
    glCompileShader( shader );

    GLint success;
    glGetShaderiv( shader, GL_COMPILE_STATUS, &success );
    if (success != GL_TRUE) {
        GLint  length;
        glGetShaderiv( shader , GL_INFO_LOG_LENGTH , &length );

       if ( length ) {
          char* buffer  =  new char [ length ];
          glGetShaderInfoLog ( shader , length , NULL , buffer );
          printf("Shader %d error: %s", shader, buffer);
          delete [] buffer;
        }
    }
    else {
        printf( "Shader %d compiled OK\n", shader );
    }

    return shader;
}

string joinStrings(const vector<const string*>& strings, const string& delimiter=",")
{
    if (strings.size() > 1) {
        stringstream ss;
        auto it = strings.begin();
        for (; it != strings.end() - 1; ++it) {
            ss << **it;
            ss << delimiter;
        }
        ss << *it;
        return ss.str();
    }

    return *(strings[0]);
}

void cacheUniformLocations ( Program& program, vector<string>& identifiers )
{
    for (auto& id : identifiers) {
        program.uniforms[ id ] = glGetUniformLocation( program.object, id.c_str() );
        printf("Caching uniform identifier %s location %d\n", id.c_str(), program.uniforms[id]);
    }
}

void cacheAttributeLocations ( Program& program, vector<string>& identifiers )
{
    for (auto& id : identifiers) {
        program.attributes[ id ] = glGetAttribLocation( program.object, id.c_str() );
        printf("Caching attrib identifier %s location %d\n", id.c_str(), program.attributes[id]);
    }
}

// Defines

std::string generateDefines ( const DefineMap& defines )
{

    stringstream ss;

    for ( auto& kv : defines ) {

        ss << "#define " << kv.first << " " << kv.second << '\n';

    }

    return ss.str();

}

ProgramRef GLES2Renderer::buildProgram ( const string& shaderID,
        const string& fragmentShader, const string& vertexShader,
        UniformMap& uniforms,
        AttributeMap& attributes,
        DefineMap& defines,
        const Parameters& parameters,
        const string& index0AttributeName )
{
//
//      var p, pl, d, program, code;

    // Generate code
    vector<const string*> chunks;

    if ( !shaderID.empty() ) {

        chunks.push_back( &shaderID );

    } else {

        chunks.push_back( &fragmentShader );
        chunks.push_back( &vertexShader );

    }

//      for ( d in defines ) {
//
//          chunks.push( d );
//          chunks.push( defines[ d ] );
//
//      }
//
//      for ( p in parameters ) {
//
//          chunks.push( p );
//          chunks.push( parameters[ p ] );
//
//      }

    string code = joinStrings(chunks);

    // Check if code has been already compiled

    for ( int p = 0, pl = _programs.size(); p < pl; p ++ ) {

        ProgramRef programInfo = _programs[ p ];

        if ( programInfo->code == code ) {

            // console.log( "Code already compiled." /*: \n\n" + code*/ );

            programInfo->usedTimes ++;

            return programInfo;

        }

    }

//
//      var shadowMapTypeDefine = "SHADOWMAP_TYPE_BASIC";
//
//      if ( parameters.shadowMapType === THREE.PCFShadowMap ) {
//
//          shadowMapTypeDefine = "SHADOWMAP_TYPE_PCF";
//
//      } else if ( parameters.shadowMapType === THREE.PCFSoftShadowMap ) {
//
//          shadowMapTypeDefine = "SHADOWMAP_TYPE_PCF_SOFT";
//
//      }
//
    printf( "building new program\n" );
    ProgramRef program = Program::create();

    string customDefines = generateDefines( defines );

    program->object = glCreateProgram();


    /*  Vertex shader prefix */

    stringstream prefix_vertex;
#if defined( EMSCRIPTEN )
    prefix_vertex << "precision " << _precision << " float;\n";
    prefix_vertex << "precision " << _precision << " int;\n";
#endif
    prefix_vertex << (_supportsVertexTextures ? "#define VERTEX_TEXTURES" : "") << "\n";

    prefix_vertex << customDefines;

    prefix_vertex << (gammaInput ? "#define GAMMA_INPUT" : "") << "\n";
    prefix_vertex << (gammaOutput ? "#define GAMMA_OUTPUT" : "") << "\n";
    prefix_vertex << (physicallyBasedShading ? "#define PHYSICALLY_BASED_SHADING" : "") << "\n";

    prefix_vertex << "#define MAX_DIR_LIGHTS " << parameters.maxDirLights() << "\n";
    prefix_vertex << "#define MAX_POINT_LIGHTS " << parameters.maxPointLights() << "\n";
    prefix_vertex << "#define MAX_SPOT_LIGHTS " << parameters.maxSpotLights() << "\n";
    prefix_vertex << "#define MAX_HEMI_LIGHTS " << parameters.maxHemiLights() << "\n";

    // "#define MAX_SHADOWS " + parameters.maxShadows;

    // "#define MAX_BONES " + parameters.maxBones;

    if (parameters.map())
        prefix_vertex << "#define USE_MAP" << "\n";
    // parameters.envMap ? "#define USE_ENVMAP" : "";
    // parameters.lightMap ? "#define USE_LIGHTMAP" : "";
    // parameters.bumpMap ? "#define USE_BUMPMAP" : "";
    // parameters.normalMap ? "#define USE_NORMALMAP" : "";
    // parameters.specularMap ? "#define USE_SPECULARMAP" : "";
    if (parameters.vertexColors())
        prefix_vertex << "#define USE_COLOR" << "\n";

    // parameters.skinning ? "#define USE_SKINNING" : "";
    // parameters.useVertexTexture ? "#define BONE_TEXTURE" : "";

    // parameters.morphTargets ? "#define USE_MORPHTARGETS" : "";
    // parameters.morphNormals ? "#define USE_MORPHNORMALS" : "";
    if (parameters.perPixel())
        prefix_vertex << "#define PHONG_PER_PIXEL" << "\n";
    // parameters.wrapAround ? "#define WRAP_AROUND" : "";
    if (parameters.doubleSided())
        prefix_vertex << "#define DOUBLE_SIDED" << "\n";
    if (parameters.flipSided())
        prefix_vertex << "#define FLIP_SIDED" << "\n";

    // parameters.shadowMapEnabled ? "#define USE_SHADOWMAP" : "";
    // parameters.shadowMapEnabled ? "#define " + shadowMapTypeDefine : "";
    // parameters.shadowMapDebug ? "#define SHADOWMAP_DEBUG" : "";
    // parameters.shadowMapCascade ? "#define SHADOWMAP_CASCADE" : "";

    if (parameters.sizeAttenuation()) 
        prefix_vertex << "#define USE_SIZEATTENUATION" << "\n";

    prefix_vertex << "uniform mat4 modelMatrix;" << "\n";
    prefix_vertex << "uniform mat4 modelViewMatrix;" << "\n";
    prefix_vertex << "uniform mat4 projectionMatrix;" << "\n";
    prefix_vertex << "uniform mat4 viewMatrix;" << "\n";
    prefix_vertex << "uniform mat3 normalMatrix;" << "\n";
    prefix_vertex << "uniform vec3 cameraPosition;" << "\n";

    prefix_vertex << "attribute vec3 position;" << "\n";
    prefix_vertex << "attribute vec3 normal;" << "\n";
    prefix_vertex << "attribute vec2 uv;" << "\n";
    prefix_vertex << "attribute vec2 uv2;" << "\n";

    prefix_vertex << "#ifdef USE_COLOR" << "\n";

    prefix_vertex <<     "attribute vec3 color;" << "\n";

    prefix_vertex << "#endif" << "\n";

    prefix_vertex << "#ifdef USE_MORPHTARGETS" << "\n";

    prefix_vertex <<     "attribute vec3 morphTarget0;" << "\n";
    prefix_vertex <<     "attribute vec3 morphTarget1;" << "\n";
    prefix_vertex <<     "attribute vec3 morphTarget2;" << "\n";
    prefix_vertex <<     "attribute vec3 morphTarget3;" << "\n";

    prefix_vertex <<     "#ifdef USE_MORPHNORMALS" << "\n";

    prefix_vertex <<         "attribute vec3 morphNormal0;" << "\n";
    prefix_vertex <<         "attribute vec3 morphNormal1;" << "\n";
    prefix_vertex <<         "attribute vec3 morphNormal2;" << "\n";
    prefix_vertex <<         "attribute vec3 morphNormal3;" << "\n";

    prefix_vertex <<     "#else" << "\n";

    prefix_vertex <<         "attribute vec3 morphTarget4;" << "\n";
    prefix_vertex <<         "attribute vec3 morphTarget5;" << "\n";
    prefix_vertex <<         "attribute vec3 morphTarget6;" << "\n";
    prefix_vertex <<         "attribute vec3 morphTarget7;" << "\n";

    prefix_vertex <<     "#endif" << "\n";

    prefix_vertex << "#endif" << "\n";

    prefix_vertex << "#ifdef USE_SKINNING" << "\n";

    prefix_vertex <<     "attribute vec4 skinIndex;" << "\n";
    prefix_vertex <<     "attribute vec4 skinWeight;" << "\n";

    prefix_vertex << "#endif" << "\n\n";


    /*  Fragment shader prefix */
    stringstream prefix_fragment;

#if defined( EMSCRIPTEN )
    prefix_fragment << "precision " << _precision << " float;\n";
    prefix_fragment << "precision " << _precision << " int;\n";
#endif

    if (parameters.bumpMap() || parameters.normalMap())
        prefix_fragment << "#extension GL_OES_standard_derivatives : enable\n";

    prefix_fragment << customDefines;

    prefix_fragment << "#define MAX_DIR_LIGHTS " << parameters.maxDirLights() << "\n";
    prefix_fragment << "#define MAX_POINT_LIGHTS " << parameters.maxPointLights() << "\n";
    prefix_fragment << "#define MAX_SPOT_LIGHTS " << parameters.maxSpotLights() << "\n";
    prefix_fragment << "#define MAX_HEMI_LIGHTS " << parameters.maxHemiLights() << "\n";

    // "#define MAX_SHADOWS " + parameters.maxShadows,

    // parameters.alphaTest ? "#define ALPHATEST " + parameters.alphaTest: "",

    if (gammaInput)
       prefix_fragment << "#define GAMMA_INPUT\n";
    if (gammaOutput)
       prefix_fragment << "#define GAMMA_OUTPUT\n";
    if (physicallyBasedShading)
       prefix_fragment << "#define PHYSICALLY_BASED_SHADING\n";

    // ( parameters.useFog && parameters.fog ) ? "#define USE_FOG" : "",
    // ( parameters.useFog && parameters.fogExp ) ? "#define FOG_EXP2" : "",

    if (parameters.map())
        prefix_fragment << "#define USE_MAP" << "\n";
    // parameters.envMap ? "#define USE_ENVMAP" : "",
    // parameters.lightMap ? "#define USE_LIGHTMAP" : "",
    // parameters.bumpMap ? "#define USE_BUMPMAP" : "",
    // parameters.normalMap ? "#define USE_NORMALMAP" : "",
    // parameters.specularMap ? "#define USE_SPECULARMAP" : "",
    if (parameters.vertexColors())
        prefix_fragment << "#define USE_COLOR" << "\n";

    // parameters.metal ? "#define METAL" : "",
    if (parameters.perPixel())
        prefix_fragment << "#define PHONG_PER_PIXEL" << "\n";
    // parameters.wrapAround ? "#define WRAP_AROUND" : "",
    if (parameters.doubleSided())
        prefix_fragment << "#define DOUBLE_SIDED" << "\n";
    if (parameters.flipSided())
        prefix_fragment << "#define FLIP_SIDED" << "\n";

    // parameters.shadowMapEnabled ? "#define USE_SHADOWMAP" : "",
    // parameters.shadowMapEnabled ? "#define " + shadowMapTypeDefine : "",
    // parameters.shadowMapDebug ? "#define SHADOWMAP_DEBUG" : "",
    // parameters.shadowMapCascade ? "#define SHADOWMAP_CASCADE" : "",

    prefix_fragment << "uniform mat4 viewMatrix;" << "\n";
    prefix_fragment << "uniform vec3 cameraPosition;" << "\n\n";

    stringstream vss;
    vss << prefix_vertex.str() << vertexShader;
    string vertexShaderSource = vss.str();
    printf("VERTEX SRC:\n%s\n", vertexShaderSource.c_str());
    GLuint glVertexShader = getShader( "vertex", vertexShaderSource.c_str() );

    stringstream fss;
    fss << prefix_fragment.str() << fragmentShader;
    string fragmentShaderSource = fss.str();
    printf("FRAGMENT SRC:\n%s\n", fragmentShaderSource.c_str());
    GLuint glFragmentShader = getShader( "fragment", fragmentShaderSource.c_str() );

    glAttachShader( program->object, glVertexShader );
    glAttachShader( program->object, glFragmentShader );

    // Force a particular attribute to index 0.
    // because potentially expensive emulation is done by browser if attribute 0 is disabled.
    // And, color, for example is often automatically bound to index 0 so disabling it
    if ( !index0AttributeName.empty() ) {
        glBindAttribLocation( program->object, 0, index0AttributeName.c_str() );
    }

    glLinkProgram( program->object );

//      if ( !_gl.getProgramParameter( program, _gl.LINK_STATUS ) ) {
//
//          console.error( "Could not initialise shader\n" + "VALIDATE_STATUS: " + _gl.getProgramParameter( program, _gl.VALIDATE_STATUS ) + ", gl error [" + _gl.getError() + "]" );
//          console.error( "Program Info Log: " + _gl.getProgramInfoLog( program ) );
//      }
//
    // clean up

    glDeleteShader( glFragmentShader );
    glDeleteShader( glVertexShader );

    // console.log( fragmentShaderSource );
    // console.log( vertexShaderSource );

    program->uniforms.clear();
    program->attributes.clear();


//
//      var identifiers, u, a, i;
//
    // cache uniform locations

    vector<string> identifiers{
        "viewMatrix", "modelViewMatrix", "projectionMatrix", "normalMatrix", "modelMatrix", "cameraPosition",
        "morphTargetInfluences"
    };

    if ( parameters.useVertexTexture() ) {

        identifiers.push_back( "boneTexture" );
        identifiers.push_back( "boneTextureWidth" );
        identifiers.push_back( "boneTextureHeight" );

    } else {

        identifiers.push_back( "boneGlobalMatrices" );

    }

    for ( auto& kv : uniforms ) {

        identifiers.push_back( kv.first );

    }

    cacheUniformLocations( *program, identifiers );

    // cache attributes locations

    identifiers.clear();
    identifiers = {

        "position", "normal", "uv", "uv2", "tangent", "color",
        "skinIndex", "skinWeight", "lineDistance"

    };

    // for ( i = 0; i < parameters.maxMorphTargets; i ++ ) {

    //     identifiers.push( "morphTarget" + i );

    // }

    // for ( i = 0; i < parameters.maxMorphNormals; i ++ ) {

    //     identifiers.push( "morphNormal" + i );

    // }

    for ( auto& kv : attributes ) {

        identifiers.push_back( kv.first );

    }

    cacheAttributeLocations( *program, identifiers );

//    printf("CACHED ATTRIBUTES SIZE: %d\n", program->attributes.size());

    program->id = _programs_counter ++;
    program->code = code;
    program->usedTimes = 1;

    _programs.push_back(program);

    info.memory.programs = _programs.size();

    return program;
}

void GLES2Renderer::deallocateMaterial( MaterialRef material )
{
//    var program = material->program;
//
//    if ( program === undefined ) return;
//
//    material->program = undefined;
//
//    // only deallocate GL program if this was the last use of shared program
//    // assumed there is only single copy of any program in the _programs list
//    // (that's how it's constructed)
//
//    var i, il, programInfo;
//    var deleteProgram = false;
//
//    for ( i = 0, il = _programs.length; i < il; i ++ ) {
//
//        programInfo = _programs[ i ];
//
//        if ( programInfo.program === program ) {
//
//            programInfo.usedTimes --;
//
//            if ( programInfo.usedTimes === 0 ) {
//
//                deleteProgram = true;
//
//            }
//
//            break;
//
//        }
//
//    }
//
//    if ( deleteProgram === true ) {
//
//        // avoid using array.splice, this is costlier than creating new array from scratch
//
//        var newPrograms = [];
//
//        for ( i = 0, il = _programs.length; i < il; i ++ ) {
//
//            programInfo = _programs[ i ];
//
//            if ( programInfo.program !== program ) {
//
//                newPrograms.push( programInfo );
//
//            }
//
//        }
//
//        _programs = newPrograms;
//
//        _gl.deleteProgram( program );
//
//        _this.info.memory.programs --;
//
//    }
//
}


void GLES2Renderer::renderObjects ( vector<Scene::GLES2Object> renderList, bool reverse,
        MaterialType materialType, CameraRef camera, vector<LightRef>& lights, FogRef fog,
        bool useBlending, MaterialRef overrideMaterial )
{
    int start, end, delta;

    if ( reverse ) {

        start = renderList.size() - 1;
        end = -1;
        delta = -1;

    } else {

        start = 0;
        end = renderList.size();
        delta = 1;
    }

    for ( int i = start; i != end; i += delta ) {

        auto& gles2Object = renderList[ i ];

        if ( gles2Object.render ) {

            Object3DRef object = gles2Object.object;
            BufferGeometryRef buffer = gles2Object.buffer;
            MaterialRef material;

            if ( overrideMaterial ) {

                material = overrideMaterial;

            } else {

                material = (materialType == kOpaque)      ? gles2Object.opaqueMaterial
                         : (materialType == kTransparent) ? gles2Object.transparentMaterial
                                                          : MaterialRef();

                if ( ! material ) continue;

                if ( useBlending )
                    setBlending( material->blending(), material->blendEquation(),
                            material->blendSrc(), material->blendDst() );

                setDepthTest( material->depthTest() );
                setDepthWrite( material->depthWrite() );
                setPolygonOffset( material->polygonOffset(), material->polygonOffsetFactor(), material->polygonOffsetUnits() );

            }

            setMaterialFaces( material );

            renderBufferDirect( camera, lights, fog, material, buffer, object );
        }

    }

}

GLES2RendererRef GLES2Renderer::create()
{
    return GLES2RendererRef(new GLES2Renderer());
}

GLES2Renderer::GLES2Renderer()
{
    _clearColor = Color(0x000000);
    _precision = "highp";

    _programs_counter = 0;

    /*
    this.domElement = _canvas;
    this.context = null;
    this.devicePixelRatio = parameters.devicePixelRatio !== undefined
                ? parameters.devicePixelRatio
                : self.devicePixelRatio !== undefined
                    ? self.devicePixelRatio
                    : 1;
    */

    // clearing

    autoClear = true;
    autoClearColor = true;
    autoClearDepth = true;
    autoClearStencil = true;

    // scene graph

    sortObjects = true;
    autoUpdateObjects = true;

    // physically based shading

    gammaInput = false;
    gammaOutput = false;
    physicallyBasedShading = false;

    // shadow map

    shadowMapEnabled = false;
    shadowMapAutoUpdate = true;
    /*
    this.shadowMapType = THREE.PCFShadowMap;
    this.shadowMapCullFace = THREE.CullFaceFront;
    */
    shadowMapDebug = false;
    shadowMapCascade = false;

    // morphs

    maxMorphTargets = 8;
    maxMorphNormals = 4;

    // flags

    autoScaleCubemaps = true;

    // custom render plugins

    /*
    this.renderPluginsPre = [];
    this.renderPluginsPost = [];
    */

    // info
    info.memory.programs = 0;
    info.memory.geometries = 0;
    info.memory.textures = 0;

    info.render.calls = 0;
    info.render.vertices = 0;
    info.render.faces = 0;
    info.render.points = 0;

    //  GL state cache

    _currentProgram = ProgramRef();
    _currentMaterialId = -1;
    _currentGeometryGroupHash = 0;
    _currentCamera = NULL;

    _oldDoubleSided = -1;
    _oldFlipSided = -1;

    _oldBlending = -1;

    _oldBlendEquation = -1;
    _oldBlendSrc = -1;
    _oldBlendDst = -1;

    _oldLineWidth = 1.0f;
    _oldPolygonOffsetFactor = -1.0f;
    _oldPolygonOffsetUnits = -1.0f;

    //  GPU capabilities
    _glExtensionTextureFloat = false;
    _glExtensionTextureFloatLinear = false;
    _glExtensionStandardDerivatives = false;
    _glExtensionTextureFilterAnisotropic = false;
    _glExtensionCompressedTextureS3TC = false;

    _lightsNeedUpdate = true;

    _lights.directional.length = 0;
    _lights.point.length = 0;
    _lights.spot.length = 0;
    _lights.hemi.length = 0;

    _lights.ambient.resize(3);

    //  initGL();
    setDefaultGLState();

    glGetIntegerv( GL_MAX_TEXTURE_IMAGE_UNITS, &_maxTextures );
    glGetIntegerv( GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &_maxVertexTextures );
    glGetIntegerv( GL_MAX_TEXTURE_SIZE, &_maxTextureSize );
    glGetIntegerv( GL_MAX_CUBE_MAP_TEXTURE_SIZE, &_maxCubemapSize );

    // var _maxAnisotropy = _glExtensionTextureFilterAnisotropic ? _gl.getParameter( _glExtensionTextureFilterAnisotropic.MAX_TEXTURE_MAX_ANISOTROPY_EXT ) : 0;

    _supportsVertexTextures = ( _maxVertexTextures > 0 );
    _supportsBoneTextures = _supportsVertexTextures && _glExtensionTextureFloat;

    // var _compressedTextureFormats = _glExtensionCompressedTextureS3TC ? _gl.getParameter( _gl.COMPRESSED_TEXTURE_FORMATS ) : [];

    // //

    // var _vertexShaderPrecisionHighpFloat = _gl.getShaderPrecisionFormat( _gl.VERTEX_SHADER, _gl.HIGH_FLOAT );
    // var _vertexShaderPrecisionMediumpFloat = _gl.getShaderPrecisionFormat( _gl.VERTEX_SHADER, _gl.MEDIUM_FLOAT );
    // var _vertexShaderPrecisionLowpFloat = _gl.getShaderPrecisionFormat( _gl.VERTEX_SHADER, _gl.LOW_FLOAT );

    // var _fragmentShaderPrecisionHighpFloat = _gl.getShaderPrecisionFormat( _gl.FRAGMENT_SHADER, _gl.HIGH_FLOAT );
    // var _fragmentShaderPrecisionMediumpFloat = _gl.getShaderPrecisionFormat( _gl.FRAGMENT_SHADER, _gl.MEDIUM_FLOAT );
    // var _fragmentShaderPrecisionLowpFloat = _gl.getShaderPrecisionFormat( _gl.FRAGMENT_SHADER, _gl.LOW_FLOAT );

    // var _vertexShaderPrecisionHighpInt = _gl.getShaderPrecisionFormat( _gl.VERTEX_SHADER, _gl.HIGH_INT );
    // var _vertexShaderPrecisionMediumpInt = _gl.getShaderPrecisionFormat( _gl.VERTEX_SHADER, _gl.MEDIUM_INT );
    // var _vertexShaderPrecisionLowpInt = _gl.getShaderPrecisionFormat( _gl.VERTEX_SHADER, _gl.LOW_INT );

    // var _fragmentShaderPrecisionHighpInt = _gl.getShaderPrecisionFormat( _gl.FRAGMENT_SHADER, _gl.HIGH_INT );
    // var _fragmentShaderPrecisionMediumpInt = _gl.getShaderPrecisionFormat( _gl.FRAGMENT_SHADER, _gl.MEDIUM_INT );
    // var _fragmentShaderPrecisionLowpInt = _gl.getShaderPrecisionFormat( _gl.FRAGMENT_SHADER, _gl.LOW_INT );

}

void GLES2Renderer::setSize(int width, int height)
{
    _width = width;
    _height = height;
    setViewport( 0, 0, _width, _height );
}

void GLES2Renderer::setViewport(int x, int y, int width, int height)
{
    _viewportX = x;
    _viewportY = y;

    _viewportWidth  = width  == 0 ? _width  : width;
    _viewportHeight = height == 0 ? _height : height;

    glViewport( _viewportX, _viewportY, _viewportWidth, _viewportHeight );
}

void GLES2Renderer::setClearColor(const Color& color, float alpha)
{
    _clearColor = color;
    _clearAlpha = alpha;

    // printf("set clear color %f %f %f %f",  _clearColor.r, _clearColor.g, _clearColor.b, _clearAlpha );
    glClearColor( _clearColor.r, _clearColor.g, _clearColor.b, _clearAlpha );
}

Color GLES2Renderer::getClearColor() const
{
    return _clearColor;
}

float GLES2Renderer::getClearAlpha() const
{
    return _clearAlpha;
}

void GLES2Renderer::clear(bool color, bool depth, bool stencil)
{
    GLbitfield bits = 0;

    if (color) bits |= GL_COLOR_BUFFER_BIT;
    if (depth) bits |= GL_DEPTH_BUFFER_BIT;
    if (stencil) bits |= GL_STENCIL_BUFFER_BIT;

    glClear(bits);
}

void unrollBufferMaterial ( Scene::GLES2Object& globject )
{
    Object3DRef object = globject.object;
    BufferGeometryRef buffer = globject.buffer;

    // XXX implement missing MeshFaceMaterial handling

    MaterialRef material = object->material();

    if ( material ) {

        if ( material->transparent() ) {

            globject.transparentMaterial = material;
            globject.opaqueMaterial.reset();

        } else {

            globject.opaqueMaterial = material;
            globject.transparentMaterial.reset();

        }

    }
}

void GLES2Renderer::render(SceneRef scene, CameraRef camera, bool forceClear)
{
    FogRef fog = scene->fog();
    vector<LightRef>& lights = scene->__lights;

    // reset caching for this frame

    _currentMaterialId = -1;
    _lightsNeedUpdate = true;

    // update scene graph

    if ( scene->autoUpdate() == true ) scene->updateMatrixWorld();

    // update camera matrices and frustum

    if ( ! camera->parent() ) camera->updateMatrixWorld();

    camera->matrixWorldInverse().getInverse( camera->matrixWorld() );

    _projScreenMatrix.multiplyMatrices( camera->projectionMatrix(), camera->matrixWorldInverse() );
    _frustum.setFromMatrix( _projScreenMatrix );

    // update WebGL objects

    if ( autoUpdateObjects ) initGLES2Objects( scene );

    // custom render plugins (pre pass)

    // renderPlugins( this.renderPluginsPre, scene, camera );

    info.render.calls = 0;
    info.render.vertices = 0;
    info.render.faces = 0;
    info.render.points = 0;

    // this.setRenderTarget( renderTarget );

    if ( autoClear || forceClear ) {

        clear( autoClearColor, autoClearDepth, autoClearStencil );

    }

    // set matrices for regular objects (frustum culled)

    auto& renderList = scene->__gles2Objects;

    for ( int i = 0, il = renderList.size(); i < il; i ++ ) {

        auto& gles2Object = renderList[ i ];
        Object3DRef object = gles2Object.object;

        gles2Object.id = i;
        gles2Object.render = false;

        if ( object->visible() ) {
            if ( (object->tag() != kMesh && object->tag() != kParticleSystem)
              || (!object->frustumCulled())
              || (_frustum.intersectsObject( *object )) ) {

                setupMatrices( object, camera );
                unrollBufferMaterial( gles2Object );
                gles2Object.render = true;

                if (sortObjects) {
                    // if (object->renderDepth) {
                    //     gles2Object.z = object->renderDepth;
                    // }
                    // else {
                    //     Vector3 v;
                    //     v.getPositionFromMatrix(object->matrixWorld());
                    //     v.applyProjection(_projScreenMatrix);
                    //
                    //     gles2Object.z = _vector3.z;
                    // }
                }
            }
        }
    }

    if (sortObjects) {
        // renderList.sort( painterSortStable );
    }

//     // set matrices for immediate objects
//
//     renderList = scene->__webglObjectsImmediate;
//
//     for ( i = 0, il = renderList.length; i < il; i ++ ) {
//
//         gles2Object = renderList[ i ];
//         object = gles2Object.object;
//
//         if ( object.visible ) {
//
//             setupMatrices( object, camera );
//
//             unrollImmediateBufferMaterial( gles2Object );
//
//         }
//
//     }
//
    /*
    if ( scene->overrideMaterial() ) {

        MaterialRef material = scene->overrideMaterial();

        // setBlending( material.blending, material.blendEquation, material.blendSrc, material.blendDst );
        // setDepthTest( material.depthTest );
        // setDepthWrite( material.depthWrite );
        // setPolygonOffset( material.polygonOffset, material.polygonOffsetFactor, material.polygonOffsetUnits );

        renderObjects( scene->__gles2Objects, false, kTransparent, camera, lights, fog, true, material );
        // renderObjectsImmediate( scene->__webglObjectsImmediate, "", camera, lights, fog, false, material );

    } else {
    */

        // opaque pass (front-to-back order)

        setBlending( kNoBlending );

        renderObjects( scene->__gles2Objects, true, kOpaque, camera, lights, fog, false, MaterialRef() );
        // renderObjectsImmediate( scene->__webglObjectsImmediate, "opaque", camera, lights, fog, false, material );

        // transparent pass (back-to-front order)

        // renderObjects( scene->__gles2Objects, false, kTransparent, camera, lights, fog, true, material );
        // renderObjectsImmediate( scene->__webglObjectsImmediate, "transparent", camera, lights, fog, true, material );

        /*
    }
    */

//     // custom render plugins (post pass)
//
//     renderPlugins( this.renderPluginsPost, scene, camera );
//
//
//     // Generate mipmap if we're using any kind of mipmap filtering
//
//     if ( renderTarget && renderTarget.generateMipmaps && renderTarget.minFilter !== THREE.NearestFilter && renderTarget.minFilter !== THREE.LinearFilter ) {
//
//         updateRenderTargetMipmap( renderTarget );
//
//     }
//
//     */
//
     // Ensure depth buffer writing is enabled so it can be cleared on next render

     setDepthTest( true );
     setDepthWrite( true );

     // glFinish();
}

void GLES2Renderer::initGLES2Objects(SceneRef scene)
{
    for (Object3DRef obj : scene->__objectsAdded) {
        addObject(obj, scene);
        scene->__objectsAdded.clear();
    }

    for (Object3DRef obj : scene->__objectsRemoved) {
        removeObject(obj, scene);
        scene->__objectsRemoved.clear();
    }

    // update must be called after objects adding / removal

    for ( auto go : scene->__gles2Objects ) {
        // printf("Iterating GLES2Object buffer %p object %p\n", go.buffer, go.object);
        auto object = go.object;

        // TODO: Remove this hack (WebGLRenderer refactoring)
        if ( !object->__webglInit ) {
            if ( object->__webglActive ) {
                removeObject( object, scene );
            }

            addObject( object, scene );
        }

        updateObject( object );
    }
}

// GL state setting

void GLES2Renderer::setFaceCulling ( int32_t cullFace, int32_t frontFaceDirection )
{

    if ( cullFace == kCullFaceNone ) {

        glDisable( GL_CULL_FACE );

    } else {

        if ( frontFaceDirection == kFrontFaceDirectionCW ) {

            glFrontFace( GL_CW );

        } else {

            glFrontFace( GL_CCW );

        }

        if ( cullFace == kCullFaceBack ) {

            glCullFace( GL_BACK );

        } else if ( cullFace == kCullFaceFront ) {

            glCullFace( GL_FRONT );

        } else {

            glCullFace( GL_FRONT_AND_BACK );

        }

        glEnable( GL_CULL_FACE );

    }

}

void GLES2Renderer::setMaterialFaces ( MaterialRef material )
{

    bool doubleSided = material->side() == kDoubleSide;
    bool flipSided = material->side() == kBackSide;

    if ( _oldDoubleSided != doubleSided ) {

        if ( doubleSided ) {

            glDisable( GL_CULL_FACE );

        } else {

            glEnable( GL_CULL_FACE );

        }

        _oldDoubleSided = doubleSided;

    }

    if ( _oldFlipSided != flipSided ) {

        if ( flipSided ) {

            glFrontFace( GL_CW );

        } else {

            glFrontFace( GL_CCW );

        }

        _oldFlipSided = flipSided;

    }

};

void GLES2Renderer::setDepthTest( bool depthTest )
{
    if ( _oldDepthTest != depthTest ) {

        if ( depthTest ) {

            glEnable( GL_DEPTH_TEST );

        } else {

            glDisable( GL_DEPTH_TEST );

        }

        _oldDepthTest = depthTest;

    }
}

void GLES2Renderer::setDepthWrite( bool depthWrite )
{
    if ( _oldDepthWrite != depthWrite ) {

        glDepthMask( depthWrite );
        _oldDepthWrite = depthWrite;

    }
}

void GLES2Renderer::setLineWidth ( float width ) {

    if ( width != _oldLineWidth ) {

        glLineWidth( width );

        _oldLineWidth = width;

    }

}

void GLES2Renderer::setPolygonOffset ( bool polygonoffset, float factor, float units, bool force )
{

    if ( force || (_oldPolygonOffset != polygonoffset) ) {

        if ( polygonoffset ) {

            glEnable( GL_POLYGON_OFFSET_FILL );

        } else {

            glDisable( GL_POLYGON_OFFSET_FILL );

        }

        _oldPolygonOffset = polygonoffset;

    }

    if ( polygonoffset && ( _oldPolygonOffsetFactor != factor || _oldPolygonOffsetUnits != units ) ) {

        glPolygonOffset( factor, units );

        _oldPolygonOffsetFactor = factor;
        _oldPolygonOffsetUnits = units;

    }

};

void GLES2Renderer::setBlending( int32_t blending, int32_t blendEquation, int32_t blendSrc, int32_t blendDst )
{

    if ( blending != _oldBlending ) {

        if ( blending == kNoBlending ) {

            glDisable( GL_BLEND );

        } else if ( blending == kAdditiveBlending ) {

            glEnable( GL_BLEND );
            glBlendEquation( GL_FUNC_ADD );
            glBlendFunc( GL_SRC_ALPHA, GL_ONE );

        } else if ( blending == kSubtractiveBlending ) {

            // TODO: Find blendFuncSeparate() combination
            glEnable( GL_BLEND );
            glBlendEquation( GL_FUNC_ADD );
            glBlendFunc( GL_ZERO, GL_ONE_MINUS_SRC_COLOR );

        } else if ( blending == kMultiplyBlending ) {

            // TODO: Find blendFuncSeparate() combination
            glEnable( GL_BLEND );
            glBlendEquation( GL_FUNC_ADD );
            glBlendFunc( GL_ZERO, GL_SRC_COLOR );

        } else if ( blending == kCustomBlending ) {

            glEnable( GL_BLEND );

        } else {

            glEnable( GL_BLEND );
            glBlendEquationSeparate( GL_FUNC_ADD, GL_FUNC_ADD );
            glBlendFuncSeparate( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA );

        }

        _oldBlending = blending;

    }

    if ( blending == kCustomBlending ) {

        if ( blendEquation != _oldBlendEquation ) {

            glBlendEquation( paramThreeToGL( blendEquation ) );

            _oldBlendEquation = blendEquation;

        }

        if ( blendSrc != _oldBlendSrc || blendDst != _oldBlendDst ) {

            glBlendFunc( paramThreeToGL( blendSrc ), paramThreeToGL( blendDst ) );

            _oldBlendSrc = blendSrc;
            _oldBlendDst = blendDst;

        }

    } else {

        _oldBlendEquation = -1;
        _oldBlendSrc = -1;
        _oldBlendDst = -1;

    }

}

bool isPowerOfTwo ( int value ) {

    return ( value & ( value - 1 ) ) == 0;

}


void GLES2Renderer::setTexture(TextureRef texture, int slot)
{

    if ( texture->needsUpdate() ) {

        if ( ! texture->__webglInit ) {

            texture->__webglInit = true;

            // texture.addEventListener( 'dispose', onTextureDispose );

            glGenTextures(1, &(texture->__webglTexture));

            info.memory.textures ++;

        }

        glActiveTexture( GL_TEXTURE0 + slot );
        glBindTexture( GL_TEXTURE_2D, texture->__webglTexture );

        // glPixelStorei( GL_UNPACK_FLIP_Y_WEBGL, texture.flipY );
        // glPixelStorei( GL_UNPACK_PREMULTIPLY_ALPHA_WEBGL, texture.premultiplyAlpha );
        // glPixelStorei( GL_UNPACK_ALIGNMENT, texture.unpackAlignment );

        ImageRef image = texture->image();
        bool isImagePowerOfTwo = isPowerOfTwo( image->width() ) && isPowerOfTwo( image->height() );
        auto glFormat = paramThreeToGL( texture->format() );
        auto glType = paramThreeToGL( texture->type() );

        // setTextureParameters( GL_TEXTURE_2D, texture, isImagePowerOfTwo );

        // var mipmap, mipmaps = texture.mipmaps;
        ImageRef mipmap;
        auto& mipmaps = texture->mipmaps;

        /* if ( texture instanceof THREE.DataTexture ) {

            // use manually created mipmaps if available
            // if there are no manual mipmaps
            // set 0 level mipmap and then use GL to generate other mipmap levels

            if ( mipmaps.length > 0 && isImagePowerOfTwo ) {

                for ( var i = 0, il = mipmaps.length; i < il; i ++ ) {

                    mipmap = mipmaps[ i ];
                    glTexImage2D( GL_TEXTURE_2D, i, glFormat, mipmap.width, mipmap.height, 0, glFormat, glType, mipmap.data );

                }

                texture.generateMipmaps = false;

            } else {

                glTexImage2D( GL_TEXTURE_2D, 0, glFormat, image.width, image.height, 0, glFormat, glType, image.data );

            }

        } else if ( texture instanceof THREE.CompressedTexture ) {

            for( var i = 0, il = mipmaps.length; i < il; i ++ ) {

                mipmap = mipmaps[ i ];
                if ( texture.format!==THREE.RGBAFormat ) {
                    glCompressedTexImage2D( GL_TEXTURE_2D, i, glFormat, mipmap.width, mipmap.height, 0, mipmap.data );
                } else {
                    glTexImage2D( GL_TEXTURE_2D, i, glFormat, mipmap.width, mipmap.height, 0, glFormat, glType, mipmap.data );
                }

            }

        } else */ { // regular Texture (image, video, canvas)

            // use manually created mipmaps if available
            // if there are no manual mipmaps
            // set 0 level mipmap and then use GL to generate other mipmap levels

            if ( mipmaps.size() > 0 && isImagePowerOfTwo ) {

                for ( int i = 0, il = mipmaps.size(); i < il; i ++ ) {

                    mipmap = mipmaps[ i ];
                    glTexImage2D( GL_TEXTURE_2D, i, glFormat,
                            mipmap->width(), mipmap->height(), 0,
                            glFormat, glType, mipmap->data() );

                }

                texture->generateMipmaps() = false;

            } else {

                glTexImage2D( GL_TEXTURE_2D, 0, glFormat,
                        image->width(), image->height(), 0,
                        glFormat, glType, texture->image()->data() );

            }

        }

        if ( texture->generateMipmaps() && isImagePowerOfTwo ) glGenerateMipmap( GL_TEXTURE_2D );

        texture->needsUpdate() = false;

        // if ( texture->onUpdate ) texture->onUpdate();

    } else {

        glActiveTexture( GL_TEXTURE0 + slot );
        glBindTexture( GL_TEXTURE_2D, texture->__webglTexture );

    }

}


void GLES2Renderer::setDefaultGLState ()
{
    glClearColor( 0, 0, 0, 1 );
    glClearDepthf( 1.0f );
    glClearStencil( 0 );

    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );

    glFrontFace( GL_CCW );
    glCullFace( GL_BACK );
    glEnable( GL_CULL_FACE );

    glEnable( GL_BLEND );
    glBlendEquation( GL_FUNC_ADD );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    glClearColor( _clearColor.r, _clearColor.g, _clearColor.b, _clearAlpha );

#if defined( RESIN_GL )
    glEnable(GL_POINT_SPRITE);
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
#endif
}


}
