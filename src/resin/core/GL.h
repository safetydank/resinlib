#if defined(EMSCRIPTEN) || defined(ANDROID)
  #include <GLES2/gl2.h>
#else
  #include <OpenGL/gl.h>
#endif

