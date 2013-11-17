#include "resin/core/Image.h"
#include "resin/core/Stream.h"
#include "resin/thirdparty/stb_image/stb_image.h"

#include <cstdlib>

namespace RESIN {

ImageRef Image::create(int width, int height, Image::Format format)
{
    return ImageRef(new Image(width, height, format));
}

ImageRef Image::create(IStreamRef stream, Format format)
{
    return stream ? ImageRef(new Image(stream, format)) : ImageRef();
}

Image::Image(int width, int height, Image::Format format)
    : mWidth(width), mHeight(height), mFormat(format)
{
    size_t componentSize = size_t(mFormat);
    size_t bitmapBytes = width * height * componentSize;
    mData = (unsigned char*) calloc(bitmapBytes, 1);
}

unsigned char* Image::data()
{
    return mData;
}

namespace {

int StreamRead(void *user, char *data, int size)
{
    IStream* stream = static_cast<IStream*>(user);
    return stream->read(data, size);
}

void StreamSkip(void *user, unsigned n)
{
    IStream* stream = static_cast<IStream*>(user);
    stream->seek(n, IStream::kCurrent);
}

int StreamEOF(void *user)
{
    IStream* stream = static_cast<IStream*>(user);
    return stream->eof();
}

}

Image::Image(IStreamRef stream, Format format)
{
    stbi_io_callbacks callbacks;

    callbacks.read = StreamRead;
    callbacks.skip = StreamSkip;
    callbacks.eof  = StreamEOF;

    int comp;
    mData = stbi_load_from_callbacks(&callbacks, stream.get(), &mWidth, &mHeight, &comp, format);
    mFormat = Image::Format(comp);
}

Image::~Image()
{
    free(mData);
}

Image::Format Image::format() const
{
    return mFormat;
}

int Image::width() const
{
    return mWidth;
}

int Image::height() const
{
    return mHeight;
}

}
