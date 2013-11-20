#pragma once

#include "resin/core/Object3D.h"
#include "resin/core/Property.h"

namespace RESIN {

typedef std::shared_ptr<class Material> MaterialRef;
typedef std::shared_ptr<class Sprite> SpriteRef;

class Sprite : public Object3D
{
  public:
    static SpriteRef create( MaterialRef material=MaterialRef() );
    virtual void updateMatrix();
    
    Property<float> angle;

  protected:
    Sprite( MaterialRef material );
};

}
