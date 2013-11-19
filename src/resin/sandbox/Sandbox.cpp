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
#include "resin/materials/ParticleBasicMaterial.h"
#include "resin/materials/ShaderMaterial.h"
#include "resin/materials/Uniform.h"
#include "resin/math/Math.h"
#include "resin/math/Vector3.h"
#include "resin/math/Matrix4.h"
#include "resin/objects/Line.h"
#include "resin/objects/Mesh.h"
#include "resin/objects/ParticleSystem.h"
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


const char* shader_vert =
"void main()    {\n"
"\n"
"    gl_Position = vec4( position, 1.0 );\n"
"\n"
"}\n";

const char* shader_frag =
"uniform vec2 resolution;\n"
"uniform float time;\n"
"\n"
"void main()    {\n"
"\n"
"    vec2 p = -1.0 + 2.0 * gl_FragCoord.xy / resolution.xy;\n"
"    float a = time*40.0;\n"
"    float d,e,f,g=1.0/40.0,h,i,r,q;\n"
"    e=400.0*(p.x*0.5+0.5);\n"
"    f=400.0*(p.y*0.5+0.5);\n"
"    i=200.0+sin(e*g+a/150.0)*20.0;\n"
"    d=200.0+cos(f*g/2.0)*18.0+cos(e*g)*7.0;\n"
"    r=sqrt(pow(i-e,2.0)+pow(d-f,2.0));\n"
"    q=f/r;\n"
"    e=(r*cos(q))-a/2.0;f=(r*sin(q))-a/2.0;\n"
"    d=sin(e*g)*176.0+sin(e*g)*164.0+r;\n"
"    h=((f+d)+a/2.0)*g;\n"
"    i=cos(h+r*p.x/1.3)*(e+e+a)+cos(q*g*6.0)*(r+h/3.0);\n"
"    h=sin(f*g)*144.0-sin(e*g)*212.0*p.x;\n"
"    h=(h+(f-e)*q+sin(r-(a+h)/7.0)*10.0+i/4.0)*g;\n"
"    i+=cos(h*2.3*sin(a/350.0-q))*184.0*sin(q-(r*4.3+a/12.0)*g)+tan(r*g+h)*184.0*cos(r*g+h);\n"
"    i=mod(i/5.6,256.0)/64.0;\n"
"    if(i<0.0) i+=4.0;\n"
"    if(i>=2.0) i=4.0-i;\n"
"    d=r/350.0;\n"
"    d+=sin(d*d*8.0)*0.52;\n"
"    f=(sin(a*g)+1.0)/2.0;\n"
"    gl_FragColor=vec4(vec3(f*i/1.6,i/2.0+d/13.0,i)*d*p.x+vec3(i/1.3+d/8.0,i/2.0+d/18.0,i)*d*(1.0-p.x),1.0);\n"
"\n"
"}\n";

class SandboxApp : public App
{
  protected:
    SceneRef scene;
    CameraRef camera;
    GLES2RendererRef renderer;
    MeshRef mesh;
    LineRef line;
    MaterialRef material;

    MaterialRef shaderMaterial;

    ParticleSystemRef particleSystem;
    
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

    void shader_example()
    {
        camera = Camera::create();
        camera->position().z() = 1;
        camera->lookAt(Vector3(0, 0, 0));

        scene = Scene::create();

        shaderMaterial = ShaderMaterial::create();
        shaderMaterial -> vertexShader(shader_vert)
                         .fragmentShader(shader_frag);

        shaderMaterial->uniforms = {
            { "time", newUniform<'f'>(1.0f) },
            { "resolution", newUniform<'v2'>(Vector2(width(), height())) }
        };

        auto geometry = BufferGeometry::create();
        geometry->attributes = {
            { "position", newAttribute<'v3'>(4) },
            { "index",    newAttribute<uint16_t>(6, 1) },
        };

        geometry->offsets() = {
            { newOffset(0, 0, 6) }
        };

        Vector3* positions = geometry->attribute("position")->dataPtr<Vector3>();
        positions[0] = Vector3(-1.0f, -1.0f, 0);
        positions[1] = Vector3(-1.0f,  1.0f, 0);
        positions[2] = Vector3( 1.0f,  1.0f, 0);
        positions[3] = Vector3( 1.0f, -1.0f, 0);

        //  Default winding is FRONT_FACE CCW
        uint16_t* indexes = geometry->attribute("index")->dataPtr<uint16_t>();
        indexes[0] = 2; indexes[3] = 3;     
        indexes[1] = 1; indexes[4] = 2;
        indexes[2] = 0; indexes[5] = 0;

        auto shaderMesh = Mesh::create( geometry, shaderMaterial );
        scene->add( shaderMesh );
    }

    void buffer_geometry_particles_example()
    {
        camera = PerspectiveCamera::create( 27, aspectRatio(), 5.0f, 3500.0f );
        camera->position().z() = 2750;

        scene = Scene::create();
        // scene.fog = new THREE.Fog( 0x050505, 2000, 3500 );

        //

        int particles = 500000;

        BufferGeometryRef geometry = BufferGeometry::create();
        geometry->attributes = {

            { "position", newAttribute<'v3'>(particles) },
            { "color", newAttribute<'c'>(particles) },

        };


        Vector3* positions = geometry->attribute("position")->dataPtr<Vector3>();
        Color* colors = geometry->attribute("color")->dataPtr<Color>();

        // var color = new THREE.Color();

        int n = 1000, n2 = n / 2; // particles spread in the cube

        for ( int i = 0; i < particles; ++i ) {

            // positions
            Vector3& pos = positions[i];

            float x = pos.v.x = Math::random() * n - n2;
            float y = pos.v.y = Math::random() * n - n2;
            float z = pos.v.z = Math::random() * n - n2;

            // colors

            float vx = ( x / n ) + 0.5;
            float vy = ( y / n ) + 0.5;
            float vz = ( z / n ) + 0.5;

            colors[i].setRGB( vx, vy, vz );

        }

        geometry->computeBoundingSphere();

        //

        auto pm = ParticleBasicMaterial::create();
        pm->size() = 15.0f;
        pm->vertexColors() = true;

        particleSystem = ParticleSystem::create( geometry, pm );
        scene->add( particleSystem );
    }

    void setup()
    {
        renderer = GLES2Renderer::create();
        renderer->setSize(width(), height());

        // test1();
        // buffer_geometry_example();
        // buffer_geometry_lines_example();
        buffer_geometry_particles_example();
        // shader_example();
    }

    void update()
    {
        // printf("UPDATE\n");
        double time = elapsed();

        if (mesh) {
            mesh->rotation().setX(time * 0.25);
            mesh->rotation().setY(time * 0.5);
        }

        if (line) {
            line->rotation().setX(time * 0.25);
            line->rotation().setY(time * 0.5);
        }

        if (particleSystem) {
            particleSystem->rotation().setX(time * 0.25);
            particleSystem->rotation().setY(time * 0.5);
        }

        if (material) {
            auto fillColor = material->uniform("fillColor");
            fillColor->value<Color>().setRGB(0.2f, 0.8f, 0.2f);
        }

        if (shaderMaterial) {
            // auto fillColor = material->uniform("fillColor");
            shaderMaterial
                ->uniform("time")
                ->value<float>() = elapsed() * 3.0f;
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

