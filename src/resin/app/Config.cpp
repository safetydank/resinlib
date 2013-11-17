#include "Config.h"

namespace RESIN {

#if 1 || defined(RESIN_ANDROID) || defined(RESIN_IOS)
Config::Config() :
    depth(this, 16),
    zDepth(this, 16),
    windows(this, 1),
    fsaa(this, 0),
    translucent(this, false)
#elif defined(RESIN_WINDOWS) || defined(RESIN_OSX) || defined(RESIN_LINUX)
Config::Config() :
    depth(this, 32),
    zDepth(this, 32),
    windows(this, 1),
    fsaa(this, 4),
    translucent(this, false)
#endif
{
}

Config::~Config()
{
}

}
