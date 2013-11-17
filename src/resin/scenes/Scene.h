#pragma once

#include "resin/core/Object3D.h"
#include "resin/core/Property.h"

namespace RESIN {

class BufferGeometry;
class Material;

typedef std::shared_ptr<class Fog> FogRef;
typedef std::shared_ptr<class Light> LightRef;
typedef std::shared_ptr<class Scene> SceneRef;

class Scene : public Object3D
{
  public:
    static SceneRef create();

    Property<bool> autoUpdate;

    void __addObject( Object3DRef object );
    void __removeObject( Object3DRef object );

    Property<MaterialRef> overrideMaterial;
    Property<FogRef> fog;

  protected:
    Scene();

  public:
    /*  Used by the renderer  */

    struct GLES2Object {
        GLES2Object();
        int id;
        BufferGeometryRef buffer;
        Object3DRef object;
        MaterialRef opaque;
        MaterialRef transparent;
        float z;
        bool render;

        MaterialRef opaqueMaterial;
        MaterialRef transparentMaterial;
    };

    std::vector<LightRef> __lights;
    std::vector<Object3DRef> __objectsAdded;
    std::vector<Object3DRef> __objectsRemoved;

    std::vector<GLES2Object> __gles2Objects;
    std::vector<GLES2Object> __gles2ObjectsImmediate;
    // std::vector<Object3DRef> __gles2Sprites;
    // std::vector<Object3DRef> __gles2Flares;

};

}
