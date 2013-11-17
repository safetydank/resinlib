#pragma once

#if defined(EMSCRIPTEN)
  //  Version 1.3.x
  #define RESIN_SDL1
#else
  // Version 2.x
  #define RESIN_SDL2
#endif

#if defined( RESIN_SDL1 )
  #include <SDL/SDL.h>
#elif defined( RESIN_SDL2 )
  #include <SDL.h>
#endif

#include "resin/app/Config.h"

namespace RESIN {

class App;

class ResinSDL
{
  public:
    ResinSDL(App* app);
    int launch();
    void cleanup();

    //  internal loop
    void loop();

  protected:
    App* mApp;
    Config mConfig;

    uint32_t mStartTime;

#if defined(RESIN_SDL1)
    SDL_Surface* mScreen;
#elif defined(RESIN_SDL2)
    SDL_Window* mScreen;
    SDL_GLContext mGLContext;
#endif
};

}

#define RESIN_APP_SDL( APP, ... )                   \
extern "C" {                                        \
  int main(int argc, char* argv[]) {                \
    RESIN::App *app = new APP(__VA_ARGS__);         \
    RESIN::ResinSDL resin(app);                     \
    int err = resin.launch();                       \
    resin.cleanup();                                \
    return err;                                     \
  }                                                 \
}
