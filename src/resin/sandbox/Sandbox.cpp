#include "resin/RESIN.h"
#include "resin/app/App.h"
#include "resin/app/ResinSDL.h"

#include "resin/cameras/PerspectiveCamera.h"
#include "resin/cameras/OrthographicCamera.h"
#include "resin/core/BufferGeometry.h"
#include "resin/core/Image.h"
#include "resin/core/Stream.h"
#include "resin/lights/AmbientLight.h"
#include "resin/lights/DirectionalLight.h"
#include "resin/lights/PointLight.h"
#include "resin/materials/LineBasicMaterial.h"
#include "resin/materials/MeshBasicMaterial.h"
#include "resin/materials/MeshPhongMaterial.h"
#include "resin/materials/ShaderMaterial.h"
#include "resin/materials/Uniform.h"
#include "resin/math/Math.h"
#include "resin/math/Vector3.h"
#include "resin/math/Matrix4.h"
#include "resin/objects/Line.h"
#include "resin/objects/Mesh.h"
#include "resin/renderers/GLES2Renderer.h"
#include "resin/scenes/Scene.h"

#include "D_png.h"

using namespace RESIN;

//#define BASELINE

#include <cstdio>
#include <iostream>

const char* vertex_src2 =
"                                        \n\
   void main()                           \n\
   {                                     \n\
      gl_Position = projectionMatrix * modelViewMatrix * vec4(position, 1.0);\n\
   }                                     \n\
";
 
 
const char* fragment_src2 =
"                                                \n\
   uniform vec3 fillColor;                       \n\
   void  main()                                  \n\
   {                                             \n\
      gl_FragColor  =  vec4(fillColor.r, fillColor.g, fillColor.b, 1.0); \n\
   }                                             \n\
";

class SandboxApp : public App
{
  protected:
    SceneRef scene;
    CameraRef camera;
    GLES2RendererRef renderer;
    MeshRef mesh;
    LineRef line;
    
  public:
    SandboxApp() : App()
    {
        printf("SandboxApp() %p\n", this);
    }

    Config config()
    {
        return Config().depth(32);
    }

    void test1()
    {
        printf("SETUP\n");

        int w = width();
        int h = height();

        scene = Scene::create();

        auto dstream = MemoryBufferStream::create(D_png, D_png_len);
        auto dimage  = Image::create(dstream);
        printf("Read D image: %d x %d format %d\n",
                dimage->width(), dimage->height(), int(dimage->format()));

        auto tex = Texture::create(dimage);

        // camera = OrthographicCamera::create(-1.0f, 1.0f, 1.0f, -1.0f);
        camera = PerspectiveCamera::create(45, float(w) / float(h), 1.0f, 3500.0f );
        camera->position().set(0, 0, 3.3333);
        camera->lookAt(Vector3(0, 0, 0));

        // scene->fog = new Fog( 0x050505, 2000, 3500 );
        // scene->add( new AmbientLight( 0x444444 ) );

        // auto light1 = new DirectionalLight( 0xffffff, 0.5f );
        auto geometry = BufferGeometry::create();
        geometry->attributes = {
            { "index",    newAttribute<uint16_t>(6, 1) },
            { "position", newAttribute<'v3'>(4) },
            { "normal",   newAttribute<'v3'>(4) },
            { "uv",       newAttribute<'v2'>(4) },
        };

        geometry->offsets() = {
            { newOffset(0, 0, 6) }
        };

        Vector3* positions = geometry->attribute("position")->dataPtr<Vector3>();
        positions[0] = Vector3(-0.5f, -0.5f, 0);
        positions[1] = Vector3(-0.5f,  0.5f, 0);
        positions[2] = Vector3( 0.5f,  0.5f, 0);
        positions[3] = Vector3( 0.5f, -0.5f, 0);

        Vector3* normals = geometry->attribute("normal")->dataPtr<Vector3>();
        normals[0] = Vector3(0, 0, 1.0f);
        normals[1] = Vector3(0, 0, 1.0f);
        normals[2] = Vector3(0, 0, 1.0f);
        normals[3] = Vector3(0, 0, 1.0f);

        //  Default winding is FRONT_FACE CCW
        uint16_t* indexes = geometry->attribute("index")->dataPtr<uint16_t>();
        indexes[0] = 2; indexes[3] = 3;		
        indexes[1] = 1; indexes[4] = 2;
        indexes[2] = 0; indexes[5] = 0;

        Vector2* uv = geometry->attribute("uv")->dataPtr<Vector2>();
        uv[0] = Vector2(0.0f, 0.0f);
        uv[1] = Vector2(0.0f, 1.0f);
        uv[2] = Vector2(1.0f, 1.0f);
        uv[3] = Vector2(1.0f, 0.0f);

#if 0
        ShaderMaterialRef material = ShaderMaterial::create();
        material -> vertexShader(vertex_src2)
                   .fragmentShader(fragment_src2);

        material->uniforms = {
            { "fillColor", newUniform<'c'>(Color().setRGB(1.0f, 0.1f, 0.1f)) }
        };

        auto fillColor = material->uniform("fillColor");
        fillColor->value<Color>().setRGB(0.2f, 0.8f, 0.2f);
#endif
        // MeshBasicMaterialRef material = MeshBasicMaterial::create();
        MeshPhongMaterialRef material = MeshPhongMaterial::create();
        material->color().setRGB(0.7f, 0.3f, 0.4f);
        material->ambient().setRGB(0.2f, 0.5f, 0.8f);
        material->specular().setHex(0xffffff);
        material->shininess() = 250.0f;
        material->map() = tex;

        mesh = Mesh::create(geometry, material);
        scene->add(mesh);

        DirectionalLightRef light1 = DirectionalLight::create( 0xffffff, 0.5f );
        light1->position().set(0, 0, 3.0f);
        scene->add(light1);
    }

    void buffer_geometry_example()
    {
        renderer->gammaInput = true;
        renderer->gammaOutput = true;
        renderer->physicallyBasedShading = true;

        scene = Scene::create();
        scene->add( AmbientLight::create( 0x444444 ) );

        camera = PerspectiveCamera::create(27, aspectRatio(), 1.0f, 3500.0f );
        camera->position().z() = 2750.0f;
        camera->lookAt(Vector3(0, 0, 0));
        
        auto light1 = DirectionalLight::create( 0xffffff, 0.5f );
        light1->position().set( 1.0f, 1.0f, 1.0f );
        scene->add( light1 );

        auto light2 = DirectionalLight::create( 0xffffff, 1.5f );
        light2->position().set( 0, -1.0f, 0 );
        scene->add( light2 );

        int triangles = 160000;
        
        auto geometry = BufferGeometry::create();
        geometry->attributes = {
            { "index",    newAttribute<uint16_t>(triangles * 3, 1) },
            { "position", newAttribute<'v3'>(triangles * 3) },
            { "normal",   newAttribute<'v3'>(triangles * 3) },
            { "color",   newAttribute<'c'>(triangles * 3) },
        };

        // break geometry into
        // chunks of 21,845 triangles (3 unique vertices per triangle)
        // for indices to fit into 16 bit integer number
        // floor(2^16 / 3) = 21845

        int chunkSize = 21845;

        uint16_t* indices = geometry->attributes["index"]->dataPtr<uint16_t>();

        for ( int i = 0; i < triangles * 3; i ++ ) {

            indices[ i ] = i % ( 3 * chunkSize );

        }

        Vector3* positions = geometry->attribute("position")->dataPtr<Vector3>();
        Vector3* normals   = geometry->attribute("normal")->dataPtr<Vector3>();
        Color* colors      = geometry->attribute("color")->dataPtr<Color>();

        Color color;

        float n = 800.0f, n2 = n * 0.5f;
        float d = 12.0f,  d2 = d * 0.5f;

        Vector3 pA, pB, pC, cb, ab;

        for ( int i = 0; i < triangles * 3; i += 3 ) {

            // positions

            float x = Math::random() * n - n2;
            float y = Math::random() * n - n2;
            float z = Math::random() * n - n2;

            float ax = x + Math::random() * d - d2;
            float ay = y + Math::random() * d - d2;
            float az = z + Math::random() * d - d2;

            float bx = x + Math::random() * d - d2;
            float by = y + Math::random() * d - d2;
            float bz = z + Math::random() * d - d2;

            float cx = x + Math::random() * d - d2;
            float cy = y + Math::random() * d - d2;
            float cz = z + Math::random() * d - d2;

            positions[ i ].set(ax, ay, az);
            positions[ i + 1 ].set(bx, by, bz);
            positions[ i + 2 ].set(cx, cy, cz);

            // flat face normals

            pA.set( ax, ay, az );
            pB.set( bx, by, bz );
            pC.set( cx, cy, cz );

            cb.subVectors( pC, pB );
            ab.subVectors( pA, pB );
            cb.cross( ab );

            cb.normalize();

            float nx = cb.x();
            float ny = cb.y();
            float nz = cb.z();

            normals[ i ].set(nx, ny, nz);
            normals[ i + 1 ].set(nx, ny, nz);
            normals[ i + 2 ].set(nx, ny, nz);

            // colors

            float vx = ( x / n ) + 0.5f;
            float vy = ( y / n ) + 0.5f;
            float vz = ( z / n ) + 0.5f;

            // color.setRGB( vx, vy, vz );

            colors[ i ].setRGB( vx, vy, vz );
            colors[ i + 1 ].setRGB( vx, vy, vz );
            colors[ i + 2 ].setRGB( vx, vy, vz );

        }

        int offsets = triangles / chunkSize;
        if (triangles % chunkSize)
            ++offsets;
        
        for ( int i = 0; i < offsets; i ++ ) {
            int start = i * chunkSize * 3;
            int index = i * chunkSize * 3;
            int count = Math::min( triangles - ( i * chunkSize ), chunkSize ) * 3;
            geometry->offsets().push_back(newOffset(start, index, count));
        }

        geometry->computeBoundingSphere();

        auto material = MeshPhongMaterial::create();
        material->color() = 0xaaaaaa;
        material->ambient() = 0xaaaaaa;
        material->specular() = 0xffffff;
        material->shininess() = 250;
        material->side() = kDoubleSide;
        material->vertexColors() = kVertexColors;

        mesh = Mesh::create( geometry, material );
        scene->add( mesh );

    }

    void buffer_geometry_lines_example()
    {
        camera = PerspectiveCamera::create( 27, aspectRatio(), 1.0f, 4000.0f );
        camera->position().z() = 2750;
        camera->lookAt(Vector3(0, 0, 0));

        scene = Scene::create();


        int segments = 10000;

        auto geometry = BufferGeometry::create();
        auto material = LineBasicMaterial::create();
        material->vertexColors() = true;

        geometry->attributes = {
            { "position", newAttribute<'v3'>(segments) },
            { "color",   newAttribute<'c'>(segments) },
        };

        Vector3* positions = geometry->attributes["position"]->dataPtr<Vector3>();
        Color* colors = geometry->attributes["color"]->dataPtr<Color>();

        float r = 800.0f;

        for ( int i = 0; i < segments; i ++ ) {

            float x = Math::random() * r - r / 2;
            float y = Math::random() * r - r / 2;
            float z = Math::random() * r - r / 2;

            // positions

            positions[i].set(x, y, z);

            // colors

            colors[i].setRGB(( x / r ) + 0.5, ( y / r ) + 0.5, ( z / r ) + 0.5);
        }

        geometry->computeBoundingSphere();

        line = Line::create( geometry, material );
        scene->add( line );

        renderer->gammaInput = true;
        renderer->gammaOutput = true;
        renderer->physicallyBasedShading = true;
    }

    void setup()
    {
        renderer = GLES2Renderer::create();
        renderer->setSize(width(), height());

        test1();
        // buffer_geometry_example();
        // buffer_geometry_lines_example();
    }

    void update()
    {
        // printf("UPDATE\n");
        double time = elapsed();

        // if (mesh) {
        //     mesh->rotation().setX(time * 0.25);
        //     mesh->rotation().setY(time * 0.5);
        // }

        if (line) {
            line->rotation().setX(time * 0.25);
            line->rotation().setY(time * 0.5);
        }
    }

    void draw()
    {
        // printf("DRAW\n");

        // Set the OpenGL state after creating the context with SDL_SetVideoMode
        renderer->setClearColor(Color(0x111111));
        renderer->render(scene, camera);
    }
};

RESIN_APP_SDL(SandboxApp)

