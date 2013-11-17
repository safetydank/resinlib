#pragma once

#include "resin/core/Object3D.h"

namespace RESIN {

typedef std::shared_ptr<class Mesh> MeshRef;

class Mesh : public Object3D
{
  public:
    static MeshRef create(BufferGeometryRef geometry, MaterialRef material);

  protected:
    Mesh(BufferGeometryRef geometry, MaterialRef material);

    // void updateMorphTargets();
    // int getMorphTargetIndexByName();
};

}
