LOCAL_PATH := $(call my-dir)/../..

include $(CLEAR_VARS)

LOCAL_MODULE := resin
LOCAL_C_INCLUDES := $(LOCAL_PATH)/src
LOCAL_SRC_FILES := \
	src/resin/app/App.cpp \
	src/resin/app/Config.cpp \
	src/resin/cameras/Camera.cpp \
	src/resin/cameras/OrthographicCamera.cpp \
	src/resin/cameras/PerspectiveCamera.cpp \
    src/resin/core/Attribute.cpp \
    src/resin/core/BufferGeometry.cpp \
    src/resin/core/Image.cpp \
    src/resin/core/MemoryBuffer.cpp \
	src/resin/core/Object3D.cpp \
	src/resin/core/Stream.cpp \
    src/resin/lights/AmbientLight.cpp \
    src/resin/lights/DirectionalLight.cpp \
    src/resin/lights/HemisphereLight.cpp \
    src/resin/lights/Light.cpp \
    src/resin/lights/PointLight.cpp \
    src/resin/materials/LineBasicMaterial.cpp \
    src/resin/materials/Material.cpp \
    src/resin/materials/MeshBasicMaterial.cpp \
    src/resin/materials/MeshPhongMaterial.cpp \
    src/resin/materials/ShaderMaterial.cpp \
	src/resin/math/Box3.cpp \
	src/resin/math/Color.cpp \
	src/resin/math/Euler.cpp \
	src/resin/math/Frustum.cpp \
	src/resin/math/Line3.cpp \
    src/resin/math/Math.cpp \
	src/resin/math/Matrix3.cpp \
	src/resin/math/Matrix4.cpp \
	src/resin/math/Plane.cpp \
	src/resin/math/Quaternion.cpp \
	src/resin/math/Ray.cpp \
	src/resin/math/Sphere.cpp \
	src/resin/math/Triangle.cpp \
	src/resin/math/Vector2.cpp \
	src/resin/math/Vector3.cpp \
    src/resin/math/Vector4.cpp \
    src/resin/objects/Line.cpp \
    src/resin/objects/Mesh.cpp \
    src/resin/renderers/GLES2Renderer.cpp \
    src/resin/renderers/Program.cpp \
	src/resin/scenes/Fog.cpp \
	src/resin/scenes/Scene.cpp \
	src/resin/textures/Texture.cpp \
    src/resin/thirdparty/stb_image/stb_image.c \
    src/resin/thirdparty/tinymt/tinymt32.c \
    src/resin/thirdparty/murmur/MurmurHash2.cpp

LOCAL_CFLAGS += -Wno-multichar -D__STDC_CONSTANT_MACROS

include $(BUILD_STATIC_LIBRARY)

