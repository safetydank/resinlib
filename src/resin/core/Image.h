#pragma once

#include <memory>

namespace RESIN {

typedef std::shared_ptr<class Image> ImageRef;
typedef std::shared_ptr<class IStream> IStreamRef;

class Image
{
  public:
    enum Format {
        kAuto = 0,
        kLuminance = 1,
        kLuminanceAlpha = 2,
        kRGB = 3,
        kRGBA = 4
    };

    static ImageRef create(int width, int height, Format format);
    static ImageRef create(IStreamRef stream, Format format=kAuto);

    Format format() const;
    int width() const;
    int height() const;

    ~Image();

    unsigned char* data();

  protected:
    Image(int width, int height, Format format);
    Image(IStreamRef stream, Format format);

    unsigned char* mData;

    int mWidth;
    int mHeight;
    Format mFormat;
};

}
