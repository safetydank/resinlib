#pragma once

#include "resin/core/Object3D.h"

namespace RESIN {

typedef std::shared_ptr<class BufferGeometry> BufferGeometryRef;
typedef std::shared_ptr<class Material> MaterialRef;

typedef std::shared_ptr<class ParticleSystem> ParticleSystemRef;

class ParticleSystem : public Object3D
{
  public:
    static ParticleSystemRef create(BufferGeometryRef buffer=BufferGeometryRef(),
            MaterialRef material=MaterialRef());

  protected:
    ParticleSystem(BufferGeometryRef buffer, MaterialRef material);
};

}

