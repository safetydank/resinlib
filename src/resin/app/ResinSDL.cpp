#include "resin/app/App.h"
#include "resin/app/ResinSDL.h"

#include <stdio.h>

#if defined(RESIN_SDL1)
  //  Emscripten
  #include <SDL/SDL_opengles2.h>
  #include <SDL/SDL_compat.h> //  why???
#elif defined(RESIN_SDL2)
  //  Desktop
  #include <SDL_opengl.h>
  #include <OpenGL/gl.h>
#endif

#if defined(EMSCRIPTEN)
  #include "emscripten.h"

  extern "C" {
    static RESIN::ResinSDL* sResinSDL = NULL;
    void staticLoop__()
    {
        if (sResinSDL)
          sResinSDL->loop();
    }
  }
#endif

namespace RESIN {

ResinSDL::ResinSDL(App* app) : mApp(app)
{
    mConfig = app->config();
}

int ResinSDL::launch()
{
    printf("!!! LAUNCH !!!\n");

    const int kWidth = 1024;
    const int kHeight = 768;

    if ( SDL_Init(SDL_INIT_VIDEO) != 0 ) {
        printf("Unable to initialize SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 ); 

#if defined(RESIN_SDL1)
    mScreen = SDL_SetVideoMode( kWidth, kHeight, mConfig.depth(), SDL_OPENGL );
#elif defined(RESIN_SDL2)
    mScreen = SDL_CreateWindow("RESIN sandbox",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            kWidth, kHeight,
            SDL_WINDOW_OPENGL);
    mGLContext = SDL_GL_CreateContext(mScreen);
#endif

    if ( !mScreen ) {
        printf("Unable to set video mode: %s\n", SDL_GetError());
        return 1;
    }
    
    glViewport( 0, 0, kWidth, kHeight );

    mApp->setDimensions__(kWidth, kHeight);
    mApp->setup();

#if defined( EMSCRIPTEN )
    const int kFPS = 60;
    sResinSDL = this;
    emscripten_set_main_loop(staticLoop__, kFPS, 1);
    // return NORMAL_EXIT; // Yield control to the browser
#else
    // 10ms delay
    // while (1) {
    //     loop();
    //     SDL_Delay(10);
    // }

    /* Main render loop */
    int frames = 0;
    mStartTime = SDL_GetTicks();
    bool running = false;

    while (!running) {
        /* Check for events */
        ++frames;

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_WINDOWEVENT:
                    switch (event.window.event) {
                    }
                    break;
                case SDL_QUIT:
                    running = true;
                    break;
            }
        }

        if (!running) {
            loop();
        }

        // while (SDL_PollEvent(&event)) {
        //     switch (event.type) {
        //       case SDL_WINDOWEVENT:
        //         switch (event.window.event) {
        //           case SDL_WINDOWEVENT_RESIZED:
        //             for (i = 0; i < state->num_windows; ++i) {
        //                 if (event.window.windowID == SDL_GetWindowID(state->windows[i])) {
        //                     status = SDL_GL_MakeCurrent(state->windows[i], mContext[i]);
        //                     if (status) {
        //                         printf("SDL_GL_MakeCurrent(): %s\n", SDL_GetError());
        //                         break;
        //                     }
        //                     /* Change view port to the new window dimensions */
        //                     glViewport(0, 0, event.window.data1, event.window.data2);
        //                     /* Update window content */
        //                     mApp->draw();
        //                     SDL_GL_SwapWindow(state->windows[i]);
        //                     break;
        //                 }
        //             }
        //             break;
        //         }
        //     }
        // }

        // for (i = 0; i < state->num_windows; ++i) {
        //     status = SDL_GL_MakeCurrent(state->windows[i], mContext[i]);
        //     if (status) {
        //         printf("SDL_GL_MakeCurrent(): %s\n", SDL_GetError());

        //         /* Continue for next window */
        //         continue;
        //     }
        //     mApp->draw();
        //     SDL_GL_SwapWindow(state->windows[i]);
        // }
    }

    /* Print out some timing information */
    uint32_t endTime = SDL_GetTicks();
    if (endTime > mStartTime) {
        printf("%2.2f frames per second\n",
               ((double) frames * 1000) / (endTime - mStartTime));
    }
#endif

    // SDL_Quit();
    return 0;
}

void ResinSDL::loop()
{
    double elapsed = (SDL_GetTicks() - mStartTime) / 1000.0;
    mApp->setElapsed__(elapsed);
    mApp->update();
    mApp->draw();
#if defined(RESIN_SDL1)
    SDL_GL_SwapBuffers();
#elif defined(RESIN_SDL2)
    SDL_GL_SwapWindow(mScreen);
#endif

    // Set the OpenGL state after creating the context with SDL_SetVideoMode
    // glClearColor( 1.0f, 0, 0, 1.0f );

    // glEnable( GL_TEXTURE_2D ); // Needed when we're using the fixed-function pipeline.

    // glMatrixMode( GL_PROJECTION );
    // glPushMatrix(); // just for testing
    // glLoadIdentity();

    // glOrtho( 0, 640, 480, 0, -1, 1 );

    // glMatrixMode( GL_MODELVIEW );
    // glLoadIdentity();

    // // Load the OpenGL texture

    // GLuint texture; // Texture object handle
    // SDL_Surface *surface; // Gives us the information to make the texture

    // if ( (surface = IMG_Load("screenshot.png")) ) {

    //     // Check that the image's width is a power of 2
    //     if ( (surface->w & (surface->w - 1)) != 0 ) {
    //         printf("warning: image.bmp's width is not a power of 2\n");
    //     }

    //     // Also check if the height is a power of 2
    //     if ( (surface->h & (surface->h - 1)) != 0 ) {
    //         printf("warning: image.bmp's height is not a power of 2\n");
    //     }

    //     // Have OpenGL generate a texture object handle for us
    //     glGenTextures( 1, &texture );

    //     // Bind the texture object
    //     glBindTexture( GL_TEXTURE_2D, texture );

    //     // Set the texture's stretching properties
    //     glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    //     glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    //     //SDL_LockSurface(surface);

    //     // Add some greyness
    //     memset(surface->pixels, 0x66, surface->w*surface->h);

    //     // Edit the texture object's image data using the information SDL_Surface gives us
    //     glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0,
    //                   GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels );

    //     //SDL_UnlockSurface(surface);
    // }
    // else {
    //     printf("SDL could not load image.bmp: %s\n", SDL_GetError());
    //     SDL_Quit();
    //     return 1;
    // }

    // // Free the SDL_Surface only if it was successfully created
    // if ( surface ) {
    //     SDL_FreeSurface( surface );
    // }

    // Clear the screen before drawing
    // glClear( GL_COLOR_BUFFER_BIT );

    // // Bind the texture to which subsequent calls refer to
    // glBindTexture( GL_TEXTURE_2D, texture );

    // glBegin( GL_QUADS );
    //     glTexCoord2i( 0, 0 ); glVertex3f( 10, 10, 0 );
    //     glTexCoord2i( 1, 0 ); glVertex3f( 300, 10, 0 );
    //     glTexCoord2i( 1, 1 ); glVertex3f( 300, 128, 0 );
    //     glTexCoord2i( 0, 1 ); glVertex3f( 10, 128, 0 );

    //     glTexCoord2f( 0, 0.5 ); glVertex3f( 410, 10, 0 );
    //     glTexCoord2f( 1, 0.5 ); glVertex3f( 600, 10, 0 );
    //     glTexCoord2f( 1, 1   ); glVertex3f( 630, 200, 0 );
    //     glTexCoord2f( 0.5, 1 ); glVertex3f( 310, 250, 0 );
    // glEnd();

    // glBegin( GL_TRIANGLE_STRIP );
    //     glTexCoord2i( 0, 0 ); glVertex3f( 100, 300, 0 );
    //     glTexCoord2i( 1, 0 ); glVertex3f( 300, 300, 0 );
    //     glTexCoord2i( 1, 1 ); glVertex3f( 300, 400, 0 );
    //     glTexCoord2i( 0, 1 ); glVertex3f( 500, 410, 0 );
    // glEnd();

    // glDisable(GL_TEXTURE_2D);

    // glColor3ub(90, 255, 255);
    // glBegin( GL_QUADS );
    //     glVertex3f( 10, 410, 0 );
    //     glVertex3f( 300, 410, 0 );
    //     glVertex3f( 300, 480, 0 );
    //     glVertex3f( 10, 470, 0 );
    // glEnd();

    // glBegin( GL_QUADS );
    //     glColor3f(1.0, 0, 1.0);   glVertex3f( 410, 410, 0 );
    //     glColor3f(0, 1.0, 0);     glVertex3f( 600, 410, 0 );
    //     glColor3f(0, 0, 1.0);     glVertex3f( 600, 480, 0 );
    //     glColor3f(1.0, 1.0, 1.0); glVertex3f( 410, 470, 0 );
    // glEnd();

    // SDL_GL_SwapBuffers();
}

// int ResinSDL::launch()
// {
//     int i, done;
//     SDL_DisplayMode mode;
//     SDL_Event event;
//     Uint32 then, now, frames;
//     int status;
// 
//     /* Initialize parameters */
//     int fsaa = 0;
//     int accel = 0;
// 
//     // for (i = 1; i < argc;) {
//     //     int consumed;
// 
//     //     consumed = SDLTest_CommonArg(state, i);
//     //     if (consumed == 0) {
//     //         if (SDL_strcasecmp(argv[i], "--fsaa") == 0) {
//     //             ++fsaa;
//     //             consumed = 1;
//     //         } else if (SDL_strcasecmp(argv[i], "--accel") == 0) {
//     //             ++accel;
//     //             consumed = 1;
//     //         } else if (SDL_strcasecmp(argv[i], "--zdepth") == 0) {
//     //             i++;
//     //             if (!argv[i]) {
//     //                 consumed = -1;
//     //             } else {
//     //                 depth = SDL_atoi(argv[i]);
//     //                 consumed = 1;
//     //             }
//     //         } else {
//     //             consumed = -1;
//     //         }
//     //     }
//     //     if (consumed < 0) {
//     //         fprintf(stderr, "Usage: %s %s [--fsaa] [--accel] [--zdepth %%d]\n", argv[0],
//     //                 SDLTest_CommonUsage(state));
//     //         quit(1);
//     //     }
//     //     i += consumed;
//     // }
// 
//     /* Set OpenGL parameters */
//     state->window_flags |= SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_BORDERLESS;
//     state->gl_red_size = 5;
//     state->gl_green_size = 5;
//     state->gl_blue_size = 5;
//     state->gl_depth_size = depth;
// 
//     if (fsaa) {
//         state->gl_multisamplebuffers=1;
//         state->gl_multisamplesamples=fsaa;
//     }
//     if (accel) {
//         state->gl_accelerated=1;
//     }
//     // if (!SDLTest_CommonInit(state)) {
//     //     return 2;
//     // }
// 
//     mContext = SDL_calloc(state->num_windows, sizeof(mContext));
//     if (mContext == NULL) {
//         fprintf(stderr, "Out of memory!\n");
//         return 2;
//     }
// 
//     /* Create OpenGL ES mContexts */
//     for (i = 0; i < state->num_windows; i++) {
//         mContext[i] = SDL_GL_CreateContext(state->windows[i]);
//         if (!mContext[i]) {
//             fprintf(stderr, "SDL_GL_CreateContext(): %s\n", SDL_GetError());
//             return 2;
//         }
//     }
// 
//     if (state->render_flags & SDL_RENDERER_PRESENTVSYNC) {
//         SDL_GL_SetSwapInterval(1);
//     } else {
//         SDL_GL_SetSwapInterval(0);
//     }
// 
//     SDL_GetCurrentDisplayMode(0, &mode);
//     printf("Screen bpp: %d\n", SDL_BITSPERPIXEL(mode.format));
//     printf("\n");
//     printf("Vendor     : %s\n", glGetString(GL_VENDOR));
//     printf("Renderer   : %s\n", glGetString(GL_RENDERER));
//     printf("Version    : %s\n", glGetString(GL_VERSION));
//     printf("Extensions : %s\n", glGetString(GL_EXTENSIONS));
//     printf("\n");
// 
//     int value;
//     status = SDL_GL_GetAttribute(SDL_GL_RED_SIZE, &value);
//     if (!status) {
//         printf("SDL_GL_RED_SIZE: requested %d, got %d\n", 5, value);
//     } else {
//         fprintf(stderr, "Failed to get SDL_GL_RED_SIZE: %s\n",
//                 SDL_GetError());
//     }
//     status = SDL_GL_GetAttribute(SDL_GL_GREEN_SIZE, &value);
//     if (!status) {
//         printf("SDL_GL_GREEN_SIZE: requested %d, got %d\n", 5, value);
//     } else {
//         fprintf(stderr, "Failed to get SDL_GL_GREEN_SIZE: %s\n",
//                 SDL_GetError());
//     }
//     status = SDL_GL_GetAttribute(SDL_GL_BLUE_SIZE, &value);
//     if (!status) {
//         printf("SDL_GL_BLUE_SIZE: requested %d, got %d\n", 5, value);
//     } else {
//         fprintf(stderr, "Failed to get SDL_GL_BLUE_SIZE: %s\n",
//                 SDL_GetError());
//     }
//     status = SDL_GL_GetAttribute(SDL_GL_DEPTH_SIZE, &value);
//     if (!status) {
//         printf("SDL_GL_DEPTH_SIZE: requested %d, got %d\n", depth, value);
//     } else {
//         fprintf(stderr, "Failed to get SDL_GL_DEPTH_SIZE: %s\n",
//                 SDL_GetError());
//     }
//     if (fsaa) {
//         status = SDL_GL_GetAttribute(SDL_GL_MULTISAMPLEBUFFERS, &value);
//         if (!status) {
//             printf("SDL_GL_MULTISAMPLEBUFFERS: requested 1, got %d\n", value);
//         } else {
//             fprintf(stderr, "Failed to get SDL_GL_MULTISAMPLEBUFFERS: %s\n",
//                     SDL_GetError());
//         }
//         status = SDL_GL_GetAttribute(SDL_GL_MULTISAMPLESAMPLES, &value);
//         if (!status) {
//             printf("SDL_GL_MULTISAMPLESAMPLES: requested %d, got %d\n", fsaa,
//                    value);
//         } else {
//             fprintf(stderr, "Failed to get SDL_GL_MULTISAMPLESAMPLES: %s\n",
//                     SDL_GetError());
//         }
//     }
//     if (accel) {
//         status = SDL_GL_GetAttribute(SDL_GL_ACCELERATED_VISUAL, &value);
//         if (!status) {
//             printf("SDL_GL_ACCELERATED_VISUAL: requested 1, got %d\n", value);
//         } else {
//             fprintf(stderr, "Failed to get SDL_GL_ACCELERATED_VISUAL: %s\n",
//                     SDL_GetError());
//         }
//     }
// 
//     /* Set rendering settings for each mContext */
//     // for (i = 0; i < state->num_windows; ++i) {
//     //     float aspectAdjust;
// 
//     //     status = SDL_GL_MakeCurrent(state->windows[i], mContext[i]);
//     //     if (status) {
//     //         printf("SDL_GL_MakeCurrent(): %s\n", SDL_GetError());
// 
//     //         /* Continue for next window */
//     //         continue;
//     //     }
// 
//     //     aspectAdjust = (4.0f / 3.0f) / ((float)state->window_w / state->window_h);
//     //     glViewport(0, 0, state->window_w, state->window_h);
//     //     glMatrixMode(GL_PROJECTION);
//     //     glLoadIdentity();
//     //     glOrthof(-2.0, 2.0, -2.0 * aspectAdjust, 2.0 * aspectAdjust, -20.0, 20.0);
//     //     glMatrixMode(GL_MODELVIEW);
//     //     glLoadIdentity();
//     //     glEnable(GL_DEPTH_TEST);
//     //     glDepthFunc(GL_LESS);
//     //     glShadeModel(GL_SMOOTH);
//     // }
// 
//     /* Main render loop */
//     frames = 0;
//     then = SDL_GetTicks();
//     done = 0;
//     while (!done) {
//         /* Check for events */
//         ++frames;
//         while (SDL_PollEvent(&event)) {
//             switch (event.type) {
//               case SDL_WINDOWEVENT:
//                 switch (event.window.event) {
//                   case SDL_WINDOWEVENT_RESIZED:
//                     for (i = 0; i < state->num_windows; ++i) {
//                         if (event.window.windowID == SDL_GetWindowID(state->windows[i])) {
//                             status = SDL_GL_MakeCurrent(state->windows[i], mContext[i]);
//                             if (status) {
//                                 printf("SDL_GL_MakeCurrent(): %s\n", SDL_GetError());
//                                 break;
//                             }
//                             /* Change view port to the new window dimensions */
//                             glViewport(0, 0, event.window.data1, event.window.data2);
//                             /* Update window content */
//                             mApp->draw();
//                             SDL_GL_SwapWindow(state->windows[i]);
//                             break;
//                         }
//                     }
//                     break;
//                 }
//             }
//         }
// 
//         for (i = 0; i < state->num_windows; ++i) {
//             status = SDL_GL_MakeCurrent(state->windows[i], mContext[i]);
//             if (status) {
//                 printf("SDL_GL_MakeCurrent(): %s\n", SDL_GetError());
// 
//                 /* Continue for next window */
//                 continue;
//             }
//             mApp->draw();
//             SDL_GL_SwapWindow(state->windows[i]);
//         }
//     }
// 
//     /* Print out some timing information */
//     now = SDL_GetTicks();
//     if (now > then) {
//         printf("%2.2f frames per second\n",
//                ((double) frames * 1000) / (now - then));
//     }
//     return 0;
// }

void ResinSDL::cleanup()
{
#if ! defined( EMSCRIPTEN )
    SDL_GL_DeleteContext(mGLContext);
    // int i;

    // if (mContext != NULL) {
    //     for (i = 0; i < state->num_windows; i++) {
    //         if (mContext[i]) {
    //             SDL_GL_DeleteContext(mContext[i]);
    //         }
    //     }

    //     SDL_free(mContext);
    // }

    // exit(0);
#endif
}

}

