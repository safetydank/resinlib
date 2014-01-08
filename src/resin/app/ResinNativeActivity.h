#pragma once

#include "resin/app/Config.h"

namespace RESIN {

class App;

/** Android Native Activity implementation */
class ResinNativeActivity
{
  public:
    ResinNativeActivity(App* app);
    int launch();
    void cleanup();

    //  internal loop
    void loop();

  protected:
    App* mApp;
    Config mConfig;

    uint32_t mStartTime;

// #if defined(RESIN_SDL1)
//     SDL_Surface* mScreen;
// #elif defined(RESIN_SDL2)
//     SDL_Window* mScreen;
//     SDL_GLContext mGLContext;
// #endif
};

}

#define RESIN_APP_NATIVE_ACTIVITY( APP, ... )       \
extern "C" {                                        \
  void android_main( struct android_app* state ) {  \
    RESIN::App *app = new APP(__VA_ARGS__);         \
    RESIN::ResinNativeActivity resin(app);          \
    int err = resin.launch();                       \
    resin.cleanup();                                \
    return err;                                     \
  }                                                 \
}


