/**
 * @author mrdoob / http://mrdoob.com/
 * @author alteredq / http://alteredqualia.com/
 *
 * parameters = {
 *  color: <hex>,
 *  opacity: <float>,
 *
 *  blending: THREE.NormalBlending,
 *  depthTest: <bool>,
 *  depthWrite: <bool>,
 *
 *  linewidth: <float>,
 *  linecap: "round",
 *  linejoin: "round",
 *
 *  vertexColors: <bool>
 *
 *  fog: <bool>
 * }
 */

#include "resin/materials/Material.h"

namespace RESIN {

typedef std::shared_ptr<class LineBasicMaterial> LineBasicMaterialRef;

class LineBasicMaterial : public Material
{
  public:
    static LineBasicMaterialRef create();

    // Property<float> linewidth;

  protected:
    LineBasicMaterial();
};

}
