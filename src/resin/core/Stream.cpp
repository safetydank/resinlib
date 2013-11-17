#include "resin/core/MemoryBuffer.h"
#include "resin/core/Stream.h"
#include "resin/math/Math.h"

#include <string>

namespace RESIN {

FileStreamRef FileStream::create(const std::string& path, IStream::Mode mode)
{
    FileStreamRef fs = FileStreamRef(new FileStream(path, mode));
    return fs->mFile ? fs : FileStreamRef();
}

FileStream::FileStream(const std::string& path, IStream::Mode mode)
    : mFile(0), mMode(mode)
{
    const char* cmode = (mode == IStream::kRead)      ? "r"
                      : (mode == IStream::kWrite)     ? "w"
                      : (mode == IStream::kReadWrite) ? "w+"
                                                      : "r";

    mFile = fopen(path.c_str(), cmode);
}

FileStream::~FileStream()
{
    if (mFile) {
        fclose(mFile);
    }
}

bool FileStream::seek(long offset, Origin origin)
{
    int corigin = (origin == kBegin)   ? SEEK_SET
                : (origin == kCurrent) ? SEEK_CUR
                : (origin == kEnd)     ? SEEK_END
                                       : SEEK_SET;
    return (fseek(mFile, offset, corigin) == 0);
}

size_t FileStream::tell()
{
    return ftell(mFile);
}

bool FileStream::eof()
{
    return feof(mFile) != 0;
}

size_t FileStream::read(void* dst, size_t size)
{
    return fread(dst, 1, size, mFile);
}

size_t FileStream::write(void* src, size_t size)
{
    return fwrite(src, 1, size, mFile);
}

IStream::Mode FileStream::mode() const
{
    return mMode;
}

MemoryBufferStreamRef MemoryBufferStream::create(MemoryBufferRef buffer)
{
    return MemoryBufferStreamRef(new MemoryBufferStream(buffer));
}

MemoryBufferStreamRef MemoryBufferStream::create(void* data, size_t size)
{
    auto buffer = MemoryBuffer::create(data, size);
    return MemoryBufferStream::create(buffer);
}

MemoryBufferStream::~MemoryBufferStream()
{
}

MemoryBufferStream::MemoryBufferStream(MemoryBufferRef buffer)
    : mBuffer(buffer), mOffset(0), mEof(false)
{
}

bool MemoryBufferStream::seek(long offset, Origin origin)
{
    if (origin == IStream::kBegin) {
        mOffset = offset;
    }
    else if (origin == IStream::kCurrent) {
        mOffset += offset;
    }
    else if (origin == IStream::kEnd) {
        mOffset = mBuffer->size() - offset;
    }

    mOffset = Math::clamp(mOffset, 0, int(mBuffer->size()));

    //  Should we return an error if the offset was clamped?
    return true;
}

size_t MemoryBufferStream::tell()
{
    return mOffset;
}

bool MemoryBufferStream::eof()
{
    return (mOffset == mBuffer->size());
}

size_t MemoryBufferStream::read(void* dst, size_t size)
{
    int avail = mBuffer->size() - mOffset;
    int readSize = Math::min(int(size), avail);

    if (readSize > 0) {
        memcpy(dst, mBuffer->dataPtr<uint8_t>() + mOffset, size);
        seek(readSize, IStream::kCurrent);
    }

    return readSize;
}

size_t MemoryBufferStream::write(void* src, size_t size)
{
    int avail = mBuffer->size() - mOffset;
    int writeSize = Math::min(int(size), avail);

    if (writeSize > 0) {
        memcpy(mBuffer->dataPtr<uint8_t>() + mOffset, src, writeSize);
        seek(writeSize, IStream::kCurrent);
    }

    return writeSize;
}

IStream::Mode MemoryBufferStream::mode() const
{
    return IStream::kReadWrite;
}

}
