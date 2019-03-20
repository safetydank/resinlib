RESIN
=====

#### A retargetable rendering library  ####

RESIN is a C++11 rendering library based on the popular
[three.js](http://threejs.org). RESIN is written to a ES2 compatible subset of
OpenGL, and has been designed to target multiple platforms (desktop, mobile,
and web via Emscripten/Native Client).

### Status ###

_Unstable, pre-alpha, proof-of-concept_

* Based on three.js r61

### Requirements ###

RESIN requires CMake and SDL2 to build, and a supported platform toolchain.

### Quickstart ###

#### OS X ####

With [homebrew](http://brew.sh) and Xcode 5 installed on Mavericks:

```
  % brew install SDL2 cmake
  % cd $RESINDIR/xcode
  % ./configure.sh
  % open Resin.xcodeproj
```

#### Emscripten ####

```
  % export EMSDK_PATH=/path/to/emscripten/1.5.x
  % cd $RESINDIR/emscripten/release
  % ./configure.sh
  % make
  % open ../sandbox.html
```

#### Android ####

NB: Currently builds as a static library only

```
  % cd $RESINDIR/android
  % ndk-build
```

#### Samples ####

<img src="https://raw.github.com/safetydank/resinlib/master/docs/images/buffer_geometry.png">
<img src="https://raw.github.com/safetydank/resinlib/master/docs/images/buffer_geometry_lines.png">
<img src="https://raw.github.com/safetydank/resinlib/master/docs/images/shader.png">

* [Emscripten buffer geometry example](http://resin.arp.io/buffer_geometry.html)
* [Emscripten buffer geometry lines example](http://resin.arp.io/buffer_geometry_lines.html)
* [Emscripten buffer geometry particles example](http://resin.arp.io/buffer_geometry_particles.html)
* [Emscripten shader example](http://resin.arp.io/shader.html)

## Links ##

* [three_cpp](https://github.com/jdduke/three_cpp) - an alternative C++ port of three.js

