include_directories (${Resin_SOURCE_DIR}/src) 

if(APPLE)
    message(STATUS "APPLE BUILD")

    include(FindPkgConfig)
    pkg_search_module(SDL2 REQUIRED sdl2)
    include_directories(${SDL2_INCLUDE_DIRS})

    find_library(OpenGL_LIBRARY OpenGL)
    mark_as_advanced(COCOA_LIBRARY OpenGL_LIBRARY ${SDL2_LIBRARIES})
    set(EXTRA_LIBS ${COCOA_LIBRARY} ${OpenGL_LIBRARY} ${SDL2_LIBRARIES})
    link_directories(${SDL2_LIBRARY_DIRS})
endif(APPLE)

if(EMSCRIPTEN)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${Resin_SOURCE_DIR}/emscripten)
endif(EMSCRIPTEN)

set(SOURCES_CXX
    resin/app/App.cpp
    resin/app/Config.cpp
    resin/app/ResinSDL.cpp
    resin/cameras/Camera.cpp
    resin/cameras/OrthographicCamera.cpp
    resin/cameras/PerspectiveCamera.cpp
    resin/core/Attribute.cpp
    resin/core/BufferGeometry.cpp
    resin/core/Image.cpp
    resin/core/MemoryBuffer.cpp
    resin/core/Object3D.cpp
    resin/core/Stream.cpp
    resin/lights/AmbientLight.cpp
    resin/lights/DirectionalLight.cpp
    resin/lights/HemisphereLight.cpp
    resin/lights/Light.cpp
    resin/lights/PointLight.cpp
    resin/lights/SpotLight.cpp
    resin/materials/LineBasicMaterial.cpp
    resin/materials/Material.cpp
    resin/materials/MeshBasicMaterial.cpp
    resin/materials/MeshPhongMaterial.cpp
    resin/materials/ParticleBasicMaterial.cpp
    resin/materials/ShaderMaterial.cpp
    resin/materials/SpriteMaterial.cpp
    resin/math/Box3.cpp
    resin/math/Color.cpp
    resin/math/Euler.cpp
    resin/math/Frustum.cpp
    resin/math/Line3.cpp
    resin/math/Math.cpp
    resin/math/Matrix3.cpp
    resin/math/Matrix4.cpp
    resin/math/Plane.cpp
    resin/math/Quaternion.cpp
    resin/math/Ray.cpp
    resin/math/Sphere.cpp
    resin/math/Triangle.cpp
    resin/math/Vector2.cpp
    resin/math/Vector3.cpp
    resin/math/Vector4.cpp
    resin/objects/Line.cpp
    resin/objects/Mesh.cpp
    resin/objects/ParticleSystem.cpp
    resin/objects/Sprite.cpp
    resin/renderers/GLES2Renderer.cpp
    resin/renderers/Program.cpp
    resin/sandbox/Sandbox.cpp
    resin/scenes/Fog.cpp
    resin/scenes/Scene.cpp
    resin/textures/Texture.cpp
    resin/thirdparty/murmur/MurmurHash2.cpp
)
set(HEADERS_CXX
    resin/app/App.h
    resin/app/Config.h
    resin/app/ResinSDL.h
    resin/cameras/Camera.h
    resin/cameras/OrthographicCamera.h
    resin/cameras/PerspectiveCamera.h
    resin/core/Attribute.h
    resin/core/BufferGeometry.h
    resin/core/Image.h
    resin/core/MemoryBuffer.h
    resin/core/Object3D.h
    resin/core/Stream.h
    resin/lights/AmbientLight.h 
    resin/lights/DirectionalLight.h
    resin/lights/HemisphereLight.h
    resin/lights/Light.h 
    resin/lights/PointLight.h
    resin/lights/SpotLight.h
    resin/materials/LineBasicMaterial.h
    resin/materials/Material.h
    resin/materials/MeshBasicMaterial.h
    resin/materials/MeshPhongMaterial.h
    resin/materials/ShaderMaterial.h
    resin/math/Box3.h
    resin/math/Color.h
    resin/math/Euler.h
    resin/math/Frustum.h
    resin/math/Line3.h
    resin/math/Math.h
    resin/math/Matrix3.h
    resin/math/Matrix4.h
    resin/math/Plane.h
    resin/math/Quaternion.h
    resin/math/Ray.h
    resin/math/Sphere.h
    resin/math/Triangle.h
    resin/math/Vector2.h
    resin/math/Vector3.h
    resin/math/Vector4.h
    resin/objects/Line.h
    resin/objects/Mesh.h
    resin/objects/Sprite.h
    resin/renderers/GLES2Renderer.h
    resin/renderers/Program.h
    resin/scenes/Fog.h
    resin/scenes/Scene.h
    resin/textures/Texture.h
    resin/thirdparty/murmur/MurmurHash2.h
)

set(SOURCES_C
    resin/thirdparty/tinymt/tinymt32.c
    resin/thirdparty/tinymt/tinymt32.h
    resin/thirdparty/stb_image/stb_image.c
    resin/thirdparty/stb_image/stb_image.h
)

set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

set_source_files_properties(${SOURCES_CXX} PROPERTIES COMPILE_FLAGS "-g -std=c++11 -stdlib=libc++ -Wno-shift-count-overflow -Wno-multichar -Wno-warn-absolute-paths")
set_source_files_properties(${SOURCES_C} PROPERTIES COMPILE_FLAGS "-g -Wno-multichar -Wno-warn-absolute-paths")

if(APPLE)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++")
    link_libraries(${EXTRA_LIBS})
    add_executable(sandbox ${SOURCES_C} ${SOURCES_CXX} ${HEADERS_CXX})
endif(APPLE)

if(EMSCRIPTEN)
    set(CMAKE_EXE_LINKER_FLAGS "-s TOTAL_MEMORY=67108864 ${CMAKE_EXE_LINKER_FLAGS}")
    add_executable(sandbox.html ${SOURCES_C} ${SOURCES_CXX} ${HEADERS_CXX})
endif(EMSCRIPTEN)

# Groups for Xcode/Visual Studio
source_group("app" REGULAR_EXPRESSION resin/app/.*)
source_group("cameras" REGULAR_EXPRESSION resin/cameras/.*)
source_group("core" REGULAR_EXPRESSION resin/core/.*)
source_group("lights" REGULAR_EXPRESSION resin/lights/.*)
source_group("materials" REGULAR_EXPRESSION resin/materials/.*)
source_group("math" REGULAR_EXPRESSION resin/math/.*)
source_group("objects" REGULAR_EXPRESSION resin/objects/.*)
source_group("renderers" REGULAR_EXPRESSION resin/renderers/.*)
source_group("scenes" REGULAR_EXPRESSION resin/scenes/.*)
source_group("thirdparty\\\\murmur" REGULAR_EXPRESSION resin/thirdparty/murmur/.*)
source_group("thirdparty\\\\stb_image" REGULAR_EXPRESSION resin/thirdparty/stb_image/.*)
source_group("thirdparty\\\\tinymt" REGULAR_EXPRESSION resin/thirdparty/tinymt/.*)

