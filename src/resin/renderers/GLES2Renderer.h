#pragma once

#include "resin/math/Color.h"
#include "resin/math/Frustum.h"
#include "resin/math/Matrix4.h"
#include "resin/math/Vector3.h"
#include "resin/renderers/Program.h"
#include "resin/scenes/Scene.h"

#include <map>
#include <string>
#include <vector>

namespace RESIN {

class Camera;
class Scene;
typedef std::shared_ptr<class Fog> FogRef;
typedef std::shared_ptr<class LineBasicMaterial> LineBasicMaterialRef;
typedef std::shared_ptr<class MeshPhongMaterial> MeshPhongMaterialRef;

struct RendererLights {

    std::vector<float> ambient;

    struct {
        int length;
        std::vector<Color> colors;
        std::vector<Vector3> positions;
    } directional;

    struct {
        int length;
        std::vector<Color> colors;
        std::vector<Vector3> positions;
        std::vector<float> distances;
    } point;

    struct {
        int length;
        std::vector<Color> colors;
        std::vector<Vector3> positions;
        std::vector<float> distances;
        std::vector<Vector3> directions;
        std::vector<float> anglesCos;
        std::vector<float> exponents;
    } spot;

    struct {
        int length;
        std::vector<Color> skyColors;
        std::vector<Color> groundColors;
        std::vector<Vector3> positions;
    } hemi;

}; 

typedef std::shared_ptr<class GLES2Renderer> GLES2RendererRef;

class GLES2Renderer
{
  public:
    struct Parameters;
    typedef Parameters R;

    struct Parameters
    {
        CProperty<R, bool> map;
        CProperty<R, bool> envMap;
        CProperty<R, bool> lightMap;
        CProperty<R, bool> bumpMap;
        CProperty<R, bool> normalMap;
        CProperty<R, bool> specularMap;

        CProperty<R, int32_t> vertexColors;

        CProperty<R, FogRef> fog;
        CProperty<R, bool> useFog;
        CProperty<R, bool> fogExp;

        // sizeAttenuation: material.sizeAttenuation,

        CProperty<R, bool> skinning;
        CProperty<R, int> maxBones;
        CProperty<R, bool> useVertexTexture;

        CProperty<R, bool> morphTargets;
        // morphNormals: material.morphNormals,
        // maxMorphTargets: this.maxMorphTargets,
        // maxMorphNormals: this.maxMorphNormals,

        CProperty<R, int> maxDirLights;
        CProperty<R, int> maxPointLights;
        CProperty<R, int> maxSpotLights;
        CProperty<R, int> maxHemiLights;

        CProperty<R, int> maxShadows;
        CProperty<R, bool> shadowMapEnabled;
        // shadowMapType: this.shadowMapType,
        // shadowMapDebug: this.shadowMapDebug,
        // shadowMapCascade: this.shadowMapCascade,

        CProperty<R, int32_t> alphaTest;
        CProperty<R, bool> metal;
        CProperty<R, bool> perPixel;
        // wrapAround: material.wrapAround,
        CProperty<R, bool> doubleSided;
        CProperty<R, bool> flipSided;

        Parameters();
    };

  public:
    static GLES2RendererRef create();

    void setSize(int width, int height);
    void setViewport(int x=0, int y=0, int width=0, int height=0);
    void render(SceneRef scene, CameraRef camera, bool forceClear=false);

    void setClearColor(const Color& color, float alpha=1.0f);
    Color getClearColor() const;
    float getClearAlpha() const;

    void setFaceCulling ( int32_t cullFace, int32_t frontFaceDirection );
    void setMaterialFaces ( MaterialRef material );
    void setDepthTest(bool depthTest);
    void setDepthWrite(bool depthWrite);
    void setLineWidth ( float width );
    void setPolygonOffset ( bool polygonoffset, float factor, float units, bool force=false );
    void setTexture(TextureRef texture, int slot);

    void clear(bool color=true, bool depth=true, bool stencil=true);

  protected:
    void initGLES2Objects(SceneRef scene);

    ProgramRef setProgram( CameraRef camera, std::vector<LightRef>& lights, FogRef fog, MaterialRef material, Object3DRef object );
    ProgramRef buildProgram ( const std::string& shaderID,
            const std::string& fragmentShader, const std::string& vertexShader,
            UniformMap& uniforms,
            AttributeMap& attributes,
            DefineMap& defines,
            const Parameters& parameters,
            const std::string& index0AttributeName );
    void renderObjects ( std::vector<Scene::GLES2Object> renderList, bool reverse,
            MaterialType materialType, CameraRef camera, std::vector<LightRef>& lights, FogRef fog,
            bool useBlending, MaterialRef overrideMaterial );
    void renderBufferDirect( CameraRef camera, std::vector<LightRef>& lights, FogRef fog, MaterialRef material,
            BufferGeometryRef geometry, Object3DRef object );

    void initMaterial( MaterialRef material, std::vector<LightRef>& lights, FogRef fog, Object3DRef object );
    void deallocateMaterial( MaterialRef material );
    void enableAttribute( GLuint attribute );
    void disableAttributes();
    void setupLights ( ProgramRef program, std::vector<LightRef>& lights );
    void refreshUniformsCommon ( UniformMap& uniforms, MaterialRef material );
    void refreshUniformsPhong ( UniformMap& uniforms, MeshPhongMaterialRef material );
    void refreshUniformsLine ( UniformMap& uniforms, LineBasicMaterialRef material );
    void setDefaultGLState();
    void loadUniformsGeneric ( Program& program, UniformMap& uniforms );
    void setBlending( int32_t blending, int32_t blendEquation=-1, int32_t blendSrc=-1, int32_t blendDst=-1 );
    GLuint getTextureUnit();

  public:
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
	bool autoClear;
	bool autoClearColor;
	bool autoClearDepth;
	bool autoClearStencil;

	// scene graph
	bool sortObjects;
	bool autoUpdateObjects;

	// physically based shading

	bool gammaInput;
	bool gammaOutput;
	bool physicallyBasedShading;

	// shadow map

	bool shadowMapEnabled;
	bool shadowMapAutoUpdate;
    /*
	this.shadowMapType = THREE.PCFShadowMap;
	this.shadowMapCullFace = THREE.CullFaceFront;
    */
	bool shadowMapDebug;
	bool shadowMapCascade;

	// morphs

	int maxMorphTargets;
	int maxMorphNormals;

	// flags

	bool autoScaleCubemaps;

	// custom render plugins

	// this.renderPluginsPre = [];
	// this.renderPluginsPost = [];

	// info

    struct {
        struct {
            int programs;
            int geometries;
            int textures;
        } memory;

        struct {
            int calls;
            int vertices;
            int faces;
            int points;
        } render;
    } info;

	// this.info = {

	// 	memory: {

	// 		programs: 0,
	// 		geometries: 0,
	// 		textures: 0

	// 	},

	// 	render: {

	// 		calls: 0,
	// 		vertices: 0,
	// 		faces: 0,
	// 		points: 0

	// 	}

	// };


  protected:
    GLES2Renderer();

    int _width, _height;

    std::string _precision;

    Color _clearColor;
    float _clearAlpha;

	// internal properties

    std::vector<ProgramRef> _programs;

	// _programs = [],
	int  _programs_counter;

	// // internal state cache

	ProgramRef _currentProgram;
	// _currentFramebuffer = null,
	int _currentMaterialId;
	uint32_t _currentGeometryGroupHash;
	Camera* _currentCamera;
	// _geometryGroupCounter = 0,

	int _usedTextureUnits;

    //  GL state cache
	GLint _oldDoubleSided;
	GLint _oldFlipSided;

	int32_t _oldBlending;

	GLint _oldBlendEquation;
	GLint _oldBlendSrc;
	GLint _oldBlendDst;

    bool _oldDepthTest;
    bool _oldDepthWrite;

	bool _oldPolygonOffset;
	float _oldPolygonOffsetFactor;
	float _oldPolygonOffsetUnits;

	float _oldLineWidth;

    int _viewportX;
    int _viewportY;

    int _viewportWidth;
    int _viewportHeight;

    std::map<GLuint, bool> _enabledAttributes;

	// frustum
	Frustum _frustum;

	 // camera matrices cache
	Matrix4 _projScreenMatrix;
	Matrix4 _projScreenMatrixPS;

	Vector3 _vector3;

	// light arrays cache
	Vector3 _direction;

    RendererLights _lights;

	bool _lightsNeedUpdate;

    //  GPU capabilities
	bool _glExtensionTextureFloat;
	bool _glExtensionTextureFloatLinear;
	bool _glExtensionStandardDerivatives;
	bool _glExtensionTextureFilterAnisotropic;
	bool _glExtensionCompressedTextureS3TC;

	GLint _maxTextures;
	GLint _maxVertexTextures;
	GLint _maxTextureSize;
	GLint _maxCubemapSize;

	// GLint _maxAnisotropy;

	bool _supportsVertexTextures;
	bool _supportsBoneTextures;

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

};

}
