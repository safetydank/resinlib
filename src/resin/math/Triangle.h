#include "resin/math/Vector3.h"
#include "resin/math/Plane.h"

namespace RESIN {

class Triangle
{
  public:
    Triangle( const Vector3& a, const Vector3& b, const Vector3& c );

    static Vector3 normal(const Vector3& a, const Vector3& b, const Vector3& c);
    static Vector3 barycoordFromPoint( const Vector3& point,
            const Vector3& a, const Vector3& b, const Vector3& c );
    static bool containsPoint(const Vector3& point,
            const Vector3& a, const Vector3& b, const Vector3& c ) ;

    Triangle& set( const Vector3& a, const Vector3& b, const Vector3& c );
    Triangle& setFromPointsAndIndices ( const Vector3* points, int i0, int i1, int i2 );
    Triangle& copy ( const Triangle& triangle );
    float area() const;
    Vector3 midpoint() const ;
    Vector3 normal() const ;
    Plane plane() const ;
    Vector3 barycoordFromPoint( const Vector3& point ) const;
    bool containsPoint ( const Vector3& point ) const;
    bool equals ( const Triangle& triangle ) const;

    Vector3 a, b, c;
};

}
