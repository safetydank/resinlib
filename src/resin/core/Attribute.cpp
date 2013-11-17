#include "resin/core/Attribute.h"

namespace RESIN {

Attribute::Attribute(size_t size, bool dynamic)
    : MemoryBuffer(size), buffer(0), needsUpdate(false), dynamic(dynamic), type(0)
{
}

Attribute::~Attribute()
{
}

}

