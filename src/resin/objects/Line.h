#pragma once

#include "resin/core/Object3D.h"

namespace RESIN {

typedef std::shared_ptr<class Line> LineRef;

class Line : public Object3D
{
  public:
    enum {
        kLineStrip  = 0,
        kLinePieces = 1,
    };

    static LineRef create( BufferGeometryRef geometry, MaterialRef material, int32_t type=kLineStrip );

    Property<int32_t> type;

  protected:
    Line( BufferGeometryRef geometry, MaterialRef material, int32_t type );

};

}
