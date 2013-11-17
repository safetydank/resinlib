#!/usr/bin/env bash
PLATFORM_DIR=$EMSDK_PATH/cmake/Platform
if [ -f $PLATFORM_DIR/Emscripten_unix.cmake ]; then
    TOOLCHAIN_FILE=$PLATFORM_DIR/Emscripten_unix.cmake
else
    TOOLCHAIN_FILE=$PLATFORM_DIR/Emscripten.cmake
fi

cmake -DEMSCRIPTEN=1 \
      -DCMAKE_TOOLCHAIN_FILE=$TOOLCHAIN_FILE \
      -DCMAKE_MODULE_PATH=$EMSDK_PATH/cmake \
      -DCMAKE_BUILD_TYPE=Debug \
      -G "Unix Makefiles" \
      ../..

