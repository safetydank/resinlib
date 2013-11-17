#pragma once

#include "resin/core/Attribute.h"
#include "resin/core/CoreFwd.h"
#include "resin/core/Property.h"
#include "resin/math/Box3.h"
#include "resin/math/MathFwd.h"
#include "resin/math/Sphere.h"

#include <memory>
#include <vector>
#include <string>

namespace RESIN {

/*  Index offset */
struct Offset
{
    //  Attribute start index
    int start;
    //  Index start
    int index;
    //  Index count
    int count;
};

Offset newOffset(int start, int index, int count);

typedef std::shared_ptr<class BufferGeometry> BufferGeometryRef;
class BufferGeometry
{
  public:
    static BufferGeometryRef create();

    const int id() const;

    AttributeMap attributes;
    AttributeRef attribute(const std::string& name);

    void applyMatrix(const Matrix4& matrix);
    void computeBoundingBox();
    void computeBoundingSphere();
    void computeVertexNormals();
    void normalizeNormals();
    void computeTangents();

    const Sphere& boundingSphere();

  public:
    Property<std::vector<Offset>> offsets;
    Property<bool> dynamic;

  protected:
    BufferGeometry();

    int mId;

    bool mBoundingSphereNeedsUpdate;
    Sphere mBoundingSphere;

    Box3 mBoundingBox;

    std::string mUuid;
    std::string mName;
    bool mHasTangents;

    bool mVerticesNeedUpdate;
    bool mNormalsNeedUpdate;

    static int GeometryIdCount;
  public:
    /* Renderer interface */
    bool __webglInit;

};

}
