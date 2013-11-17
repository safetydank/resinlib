#pragma once

#include "resin/core/CoreFwd.h"
#include "resin/core/GL.h"
#include "resin/core/MemoryBuffer.h"
#include "resin/core/Property.h"

#include <string>
#include <unordered_map>

namespace RESIN {

class Attribute : public MemoryBuffer
{
  public:
    Property<GLuint> buffer;
    Property<bool> needsUpdate;
    Property<bool> dynamic;

    uint32_t type;
    int itemSize;

  public:
    Attribute(size_t size, bool dynamic);
    ~Attribute();

    const std::string& name() const;
};

typedef std::shared_ptr<class Attribute> AttributeRef;
typedef std::unordered_map<std::string, AttributeRef> AttributeMap;

template <uint32_t A>
struct AttributeTrait
{
    static const int byteSize = 0;
    static const int itemSize = 0;
};

template<>
struct AttributeTrait<'v2'>
{
    static const int byteSize = 2 * sizeof(float);
    static const int itemSize = 2;
};

template<>
struct AttributeTrait<'v3'>
{
    static const int byteSize = 3 * sizeof(float);
    static const int itemSize = 3;
};

template<>
struct AttributeTrait<'v4'>
{
    static const int byteSize = 4 * sizeof(float);
    static const int itemSize = 4;
};

template<>
struct AttributeTrait<'c'>
{
    static const int byteSize = 3 * sizeof(float);
    static const int itemSize = 3;
};

template<>
struct AttributeTrait<'f'>
{
    static const int byteSize = sizeof(float);
    static const int itemSize = 1;
};

template<>
struct AttributeTrait<'i'>
{
    static const int byteSize = sizeof(int32_t);
    static const int itemSize = 1;
};

template<>
struct AttributeTrait<'m3'>
{
    static const int byteSize = 3 * 3 * sizeof(float);
    static const int itemSize = 9;
};

template<>
struct AttributeTrait<'m4'>
{
    static const int byteSize = 4 * 4 * sizeof(float);
    static const int itemSize = 16;
};

template <typename T>
AttributeRef newAttribute(size_t count, int itemSize, bool dynamic=false)
{
    AttributeRef attr = AttributeRef(new Attribute(sizeof(T) * count, dynamic));
    attr->itemSize = itemSize;
    return attr;
}

template <uint32_t A>
AttributeRef newAttribute(size_t count, bool dynamic=false)
{
    typedef AttributeTrait<A> Trait;
    AttributeRef attr = AttributeRef(new Attribute(Trait::byteSize * count, dynamic));
    attr->type = A;
    attr->itemSize = Trait::itemSize;
    return attr;
}

}
