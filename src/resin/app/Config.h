#pragma once

#include "resin/core/Property.h"

namespace RESIN {

class Config
{
  public:
    Config();
    ~Config();

    typedef Config C;

    CProperty<C, int> depth;
    CProperty<C, int> zDepth;
    CProperty<C, int> windows;
    CProperty<C, int> fsaa;
    CProperty<C, bool> translucent;
};

}
