#include "resin/core/BufferGeometry.h"
#include "resin/objects/Mesh.h"
#include "resin/materials/Material.h"
#include "resin/materials/MeshBasicMaterial.h"

namespace RESIN {

MeshRef Mesh::create(BufferGeometryRef geometry, MaterialRef material)
{
    return MeshRef(new Mesh(geometry, material));
}

Mesh::Mesh(BufferGeometryRef geometry, MaterialRef material) : Object3D(kMesh)
{
    this->geometry() = geometry;
    this->material() = material ? material : MeshBasicMaterial::create();

    // updateMorphTargets();
}

}
