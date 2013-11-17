/**
 * @author mrdoob / http://mrdoob.com/
 */

#include "resin/objects/Line.h"
#include "resin/materials/LineBasicMaterial.h"

namespace RESIN {

LineRef Line::create( BufferGeometryRef geometry, MaterialRef material, int32_t type )
{
    return LineRef(new Line(geometry, material, type));
}

Line::Line ( BufferGeometryRef geometry, MaterialRef material, int32_t type )
    : Object3D(kLine)
{
	this->geometry() = geometry;
	this->material() = material ? material : LineBasicMaterial::create();

	this->type() = type;
}

}
