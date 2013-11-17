#!/usr/bin/env bash
OSX_SDK_PATH=`xcodebuild -version -sdk macosx10.8 Path`
cmake -DCMAKE_BUILD_TYPE=Debug \
      -DCMAKE_OSX_SYSROOT=$OSX_SDK_PATH \
      -DCMAKE_OSX_DEPLOYMENT_TARGET=10.8 \
      -G "Xcode" \
      ..

