#include "resin/lights/Light.h"
#include "resin/scenes/Scene.h"

#include <algorithm>

using std::static_pointer_cast;

namespace RESIN {

SceneRef Scene::create()
{
    return SceneRef(new Scene());
}

Scene::Scene() : Object3D(kScene)
{
    overrideMaterial().reset();

    autoUpdate() = true; // checked by the renderer
    mMatrixAutoUpdate = false;
}

void Scene::__addObject( Object3DRef object )
{
    if ( object->tag() == kLight ) {

        LightRef light = static_pointer_cast<Light>(object);

        auto it = std::find( __lights.begin(), __lights.end(), light );

        if ( it == __lights.end() ) {

            __lights.push_back( light );

        }

        if ( light->target() && !(light->target()->parent()) ) {

            add( light->target() );

        }

    } else if ( !( object->tag() == kCamera || object->tag() == kBone ) ) {

        __objectsAdded.push_back( object );

        // check if previously removed

        auto it = std::find( __objectsRemoved.begin(), __objectsRemoved.end(), object );
        if (it != __objectsRemoved.end()) {
            __objectsRemoved.erase(it);
        }
    }

    for (Object3DRef child : object->children()) {
        __addObject(child);
    }
}

void Scene::__removeObject( Object3DRef object )
{
    if ( object->tag() == kLight ) {

        LightRef light = static_pointer_cast<Light>(object);

        auto it = std::find( __lights.begin(), __lights.end(), light );

        if (it != __lights.end()) {

            __lights.erase(it);

        }

//        if ( object.shadowCascadeArray ) {
//
//            for ( var x = 0; x < object.shadowCascadeArray.length; x ++ ) {
//
//                this.__removeObject( object.shadowCascadeArray[ x ] );
//
//            }
//
//        }

    } else if ( !( object->tag() == kCamera ) ) {

        __objectsRemoved.push_back( object );

        // check if previously added

        auto it = std::find(__objectsAdded.begin(), __objectsAdded.end(), object);
        if (it != __objectsAdded.end()) {
            __objectsAdded.erase(it);
        }
    }


    for (Object3DRef child : object->children()) {
        __removeObject(child);
    }
}

Scene::GLES2Object::GLES2Object()
    : id(-1), buffer(0), object(0), opaque(0), transparent(0), z(0), render(false)
{
}

}
