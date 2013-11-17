#pragma once

#include "resin/core/MemoryBuffer.h"
#include "resin/math/Color.h"
#include "resin/math/Matrix3.h"
#include "resin/math/Matrix4.h"
#include "resin/math/Vector2.h"
#include "resin/math/Vector3.h"
#include "resin/math/Vector4.h"
#include "resin/textures/Texture.h"

#include <map>
#include <vector>

namespace RESIN {

typedef std::shared_ptr<struct Uniform> UniformRef;

template<typename T> struct UniformPointer;
template<typename T> struct UniformPointerArray;

struct Uniform
{
    Property<uint32_t> type;

    Uniform(uint32_t type) : type(type) { }

    virtual void* data()
    {
        return 0;
    }

    //  UNSAFE, USE CAREFULLY!
    template <typename T>
    T& value()
    {
        return *((T*)data());
    }

    template <typename T>
    std::vector<T>& array()
    {
        return *((std::vector<T>*)data());
    }

    template <typename T>
    std::shared_ptr<T>& ref()
    {
        return static_cast<UniformPointer<T>*>(this)->ref();
    }

    template <typename T>
    std::vector<std::shared_ptr<T>>& refArray()
    {
        return static_cast<UniformPointerArray<T>*>(this)->refArray();
    }

    template <typename T>
    void set(const T& value)
    {
        this->value<T>() = value;
    }

    template <typename T>
    void set(std::shared_ptr<T> ref)
    {
        this->ref<T>() = ref;
    }

};

template <typename T>
struct UniformValue : public Uniform
{
    Property<T> value;

    UniformValue(uint32_t type, const T& value)
        : Uniform(type)
    {
        this->value = value;
    }

    virtual void* data()
    {
        return &value;
    }
};

template <typename T>
struct UniformArray : public Uniform
{
    Property<std::vector<T>> array;

    UniformArray(uint32_t type)
        : Uniform(type)
    {
    }

    UniformArray(uint32_t type, const std::vector<T>& array)
        : Uniform(type)
    {
        this->array = array;
    }

    virtual void* data()
    {
        return &array;
    }
};

template <typename T>
struct UniformPointer : public Uniform
{
    Property<std::shared_ptr<T>> ref;

    UniformPointer(uint32_t type, std::shared_ptr<T> ref)
        : Uniform(type), ref(ref)
    {
    }
};

template <typename T>
struct UniformPointerArray : public Uniform
{
    Property<std::vector<std::shared_ptr<T>>> refArray;

    UniformPointerArray(uint32_t type)
        : Uniform(type), refArray()
    {
    }
};

template <uint32_t U>
struct UniformFactory
{
    static UniformRef make()
    {
        return UniformRef();
    }
};

template <>
struct UniformFactory<'i'>
{
    static UniformRef make(const int32_t value=0)
    {
        return UniformRef(new UniformValue<int32_t>('i', value));
    }
};

template <>
struct UniformFactory<'f'>
{
    static UniformRef make(const float value=0)
    {
        return UniformRef(new UniformValue<float>('f', value));
    }
};

template <>
struct UniformFactory<'v2'>
{
    static UniformRef make(const Vector2& value=Vector2())
    {
        return UniformRef(new UniformValue<Vector2>('v2', value));
    }
};

template <>
struct UniformFactory<'v3'>
{
    static UniformRef make(const Vector3& value=Vector3())
    {
        return UniformRef(new UniformValue<Vector3>('v3', value));
    }
};

template <>
struct UniformFactory<'v4'>
{
    static UniformRef make(const Vector4& value=Vector4())
    {
        return UniformRef(new UniformValue<Vector4>('v4', value));
    }
};

template <>
struct UniformFactory<'c'>
{
    static UniformRef make(const Color& value=Color())
    {
        return UniformRef(new UniformValue<Color>('c', value));
    }
};

template <>
struct UniformFactory<'m3'>
{
    static UniformRef make(const Matrix3& value=Matrix3())
    {
        return UniformRef(new UniformValue<Matrix3>('m3', value));
    }
};

template <>
struct UniformFactory<'m4'>
{
    static UniformRef make(const Matrix4& value=Matrix4())
    {
        return UniformRef(new UniformValue<Matrix4>('m4', value));
    }
};

template <>
struct UniformFactory<'fv'>
{
    static UniformRef make()
    {
        return UniformRef(new UniformArray<Vector3>('fv'));
    }

    static UniformRef make(const std::vector<Vector3>& array)
    {
        return UniformRef(new UniformArray<Vector3>('fv', array));
    }
};

template <>
struct UniformFactory<'fv1'>
{
    static UniformRef make()
    {
        return UniformRef(new UniformArray<float>('fv1'));
    }

    static UniformRef make(const std::vector<float>& array)
    {
        return UniformRef(new UniformArray<float>('fv1', array));
    }
};

template <>
struct UniformFactory<'iv1'>
{
    static UniformRef make()
    {
        return UniformRef(new UniformArray<int32_t>('iv1'));
    }

    static UniformRef make(const std::vector<int>& array)
    {
        return UniformRef(new UniformArray<int32_t>('iv1', array));
    }
};

template <>
struct UniformFactory<'t'>
{
    static UniformRef make(TextureRef texture)
    {
        return UniformRef(new UniformPointer<Texture>('t', texture));
    }

    static UniformRef make()
    {
        return make(TextureRef());
    }
};

template <>
struct UniformFactory<'tv'>
{
    static UniformRef make()
    {
        return UniformRef(new UniformPointerArray<Texture>('tv'));
    }
};

typedef std::map<std::string, UniformRef> UniformMap;

template <uint32_t U>
UniformRef newUniform()
{
    return UniformFactory<U>::make();
}

template <uint32_t U, typename T>
UniformRef newUniform(const T& value)
{
    return UniformFactory<U>::make(value);
}

}
