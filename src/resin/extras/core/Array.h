#pragma once

#include "resin/core/MemoryBuffer.h"

namespace RESIN {

template <typename T>
class IArray
{
  public:
    typedef std::shared_ptr<IArray> Ref;
    virtual T* data() = 0;
    virtual size_t size() const = 0;

    virtual ~IArray { }
};

template <typename T>
class Array : IArray<T>
{
  protected:
    MemoryBuffer mBuffer;

  public:
    static IArray<T>::Ref create(T* data, size_t size)
    {
        return IArray<T>::Ref(new Array(data, size));
    }

    Array(T* data, size_t size) : mBuffer(data, size)
    {
    }

    virtual ~Array() { }

    virtual T* data()
    {
        return mData;
    }
    
    virtual size_t size() const
    {
        return mSize;
    }
};


template <typename T>
class VectorArray : IArray<T>
{
  protected:
    std::vector<T> mVector;

    VectorArray(std::vector<T>& v, bool swap)
    {
        if (swap) {
            v.swap(mVector);
        }
        else {
            mVector = v;
        }
    }

  public:
    static IArray<T>::Ref create(std::vector<T>& v, bool swap)
    {
        return IArray<T>::Ref(new VectorArray(v, swap));
    }

    virtual ~VectorArray() { }

    virtual T* data()
    {
        return &mVector[0];
    }

    virtual size_t size() const
    {
        return mVector.size();
    }
};

}
