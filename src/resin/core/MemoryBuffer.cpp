#include "resin/core/MemoryBuffer.h"
#include <cstdlib>
#include <cstdio>

namespace RESIN {

MemoryBuffer::MemoryBuffer() : mSize(0), mData(0), mOwnsData(true)
{
}

MemoryBuffer::MemoryBuffer(const MemoryBuffer& buf) : mOwnsData(true)
{
    copy(buf);
}

MemoryBuffer::MemoryBuffer(size_t size) : mOwnsData(true)
{
    // printf("Creating buffer size %d\n", size);
    mSize = size;
    mData = calloc(mSize, sizeof(uint8_t));
}

MemoryBuffer::MemoryBuffer(void *buffer, size_t size) : mData(0), mOwnsData(false)
{
    reference(buffer, size);
}

MemoryBuffer::~MemoryBuffer() 
{
    free();
}

bool MemoryBuffer::copy(const MemoryBuffer& buf)
{
    bool ret;
    if ((ret = resize(buf.size()))) {
        copyData(buf.dataPtr<uint8_t>(), buf.size());
    }

    return ret;
}

void MemoryBuffer::reference(void* buffer, size_t size)
{
    free();

    mData = (void*) buffer;
    mSize = size;
    mOwnsData = false;
}

bool MemoryBuffer::resize(size_t size) 
{
    if (mOwnsData) {
        if (!mData) {
            mData = calloc(size, sizeof(uint8_t));
        }
        else if (size != mSize) {
            mData = realloc(mData, size);
        }
        mSize = size;
        return true;
    }

    return false;
}

void MemoryBuffer::free()
{
    if (mOwnsData) {
        ::free(mData);
        mData = 0;
        mSize = 0;
    }
}

size_t MemoryBuffer::size() const
{
    return mSize;
}

void* MemoryBuffer::bytePtr()
{
    return mData;
}

bool MemoryBuffer::ownsData() const
{
    return mOwnsData;
}

}
