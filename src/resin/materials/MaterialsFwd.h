#pragma once

#include <memory>

namespace RESIN {

class Material;
typedef std::shared_ptr<Material> MaterialRef;
class ShaderMaterial;
typedef std::shared_ptr<ShaderMaterial> ShaderMaterialRef;

}
