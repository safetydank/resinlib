APP_PLATFORM := android-9
APP_STL      := gnustl_static
APP_MODULES  := resin
APP_CPPFLAGS := -fexceptions -frtti -Wno-format-security -std=c++11
APP_ABI      := armeabi-v7a x86

ifeq ($(NDK_TOOLCHAIN_VERSION),clang3.1)
  APP_CFLAGS += -Qunused-arguments
endif

