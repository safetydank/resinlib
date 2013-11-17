#pragma once

#include <cstdint>
#include <cstring>
#include <memory>
#include <vector>

namespace RESIN {

typedef std::shared_ptr<class MemoryBuffer> MemoryBufferRef;

/** A simple memory buffer class with type helpers */
class MemoryBuffer
{
  public:
    /**  Create a new buffer with a given byte size */
    template<typename T>
    static MemoryBufferRef create(size_t count)
    {
        return MemoryBufferRef(new MemoryBuffer(sizeof(T) * count));
    }

    static MemoryBufferRef create(void* buffer, size_t size)
    {
        return MemoryBufferRef(new MemoryBuffer(buffer, size));
    }

    bool ownsData() const;

  public:
    /**  An empty memory buffer, can be resized */
    MemoryBuffer();

    /**  Wrap an existing byte buffer, will not free on destruction */
    MemoryBuffer(void* buffer, size_t size);

    /**  Copy constructor */
    MemoryBuffer(const MemoryBuffer& buf);

    ~MemoryBuffer();

    /** Copy from an existing buffer */
    bool copy(const MemoryBuffer& buf);

    /** Reference an existing byte buffer */
    void reference(void* buffer, size_t size);

    /** Resize to a new byte size */
    bool resize(size_t size);

    /** Free data if owned, otherwise no-op */
    void free();

    /** Size of the buffer in bytes */
    size_t size() const;

    void* bytePtr();

    //  Type-cast helpers, assumes aligned T (no padding)
    template <typename T>
    MemoryBuffer(T* ptr, size_t size) : MemoryBuffer(&ptr, size * sizeof(T))
    { }

    template <typename T>
    MemoryBuffer(std::vector<T>& v) : MemoryBuffer(&v[0], v.size())
    { }

    template <typename T>
    bool resizeData(size_t length)
    {
        return resize(sizeof(T) * length);
    }

    template <typename T>
    void copyData(const T* data, size_t length)
    {
        if (resizeData<T>(length))
            memcpy(mData, data, size());
    }

    template <typename T>
    T* dataPtr() const
    {
        return (T*)(mData);
    }

    template <typename T>
    T* dataPtr()
    {
        return (T*)(mData);
    }

    template <typename T>
    T& at(int i)
    {
        return *(dataPtr<T>()+i);
    }

    template <typename T>
    size_t dataSize()
    {
        return mSize / sizeof(T);
    }

  protected:
    size_t mSize;

    /** Byte buffer */
    void *mData;
    bool mOwnsData;

    MemoryBuffer(size_t size);
};

}
