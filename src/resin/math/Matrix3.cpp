#include "resin/core/MemoryBuffer.h"
#include "resin/math/Matrix3.h"
#include "resin/math/Matrix4.h"
#include "resin/math/Vector3.h"

using namespace glm;

namespace RESIN { 

Matrix3::Matrix3() : m(1.0f)
{
}

Matrix3::Matrix3(const Matrix3& mtx) : m(mtx.m)
{
}

Matrix3::Matrix3(const glm::mat3& mtx) : m(mtx)
{
}

Matrix3& Matrix3::elements()
{
    return *this;
}

const Matrix3& Matrix3::elements() const
{
    return *this;
}

float& Matrix3::operator[](std::size_t i)
{
    return m[i / 3][i % 3];
}

const float& Matrix3::operator[](std::size_t i) const
{
    return m[i / 3][i % 3];
}

Matrix3& Matrix3::set(float n11, float n12, float n13,
                      float n21, float n22, float n23,
                      float n31, float n32, float n33)
{
    Matrix3& te = elements();

    te[0] = n11; te[3] = n12; te[6] = n13;
    te[1] = n21; te[4] = n22; te[7] = n23;
    te[2] = n31; te[5] = n32; te[8] = n33;

    return *this;
}

Matrix3& Matrix3::identity()
{
    m = mat3(1.0f);
    return *this;
}

Matrix3& Matrix3::copy(const Matrix3& mtx)
{
    m = mtx.m;
    return *this;
}

Vector3* Matrix3::multiplyVector3Array(Vector3* v, size_t count) const
{
    for (int i=0; i < count; ++i) {
        v[i].applyMatrix3(*this);
    }

    return v;
}

Matrix3& Matrix3::multiplyScalar(float s)
{
    m *= s;
    return *this;
}

float Matrix3::determinant() const
{
    return glm::determinant(m);
}

Matrix3& Matrix3::transpose()
{
    m = glm::transpose(m);
    return *this;
}

Matrix3& Matrix3::getInverse(const Matrix4& mtx, bool* result)
{
    // input: THREE.Matrix4
    // ( based on http://code.google.com/p/webgl-mjs/ )

    Matrix3& te = elements();

    te[ 0 ] =   mtx[10] * mtx[5] - mtx[6] * mtx[9];
    te[ 1 ] = - mtx[10] * mtx[1] + mtx[2] * mtx[9];
    te[ 2 ] =   mtx[6] * mtx[1] - mtx[2] * mtx[5];
    te[ 3 ] = - mtx[10] * mtx[4] + mtx[6] * mtx[8];
    te[ 4 ] =   mtx[10] * mtx[0] - mtx[2] * mtx[8];
    te[ 5 ] = - mtx[6] * mtx[0] + mtx[2] * mtx[4];
    te[ 6 ] =   mtx[9] * mtx[4] - mtx[5] * mtx[8];
    te[ 7 ] = - mtx[9] * mtx[0] + mtx[1] * mtx[8];
    te[ 8 ] =   mtx[5] * mtx[0] - mtx[1] * mtx[4];

    float det = mtx[ 0 ] * te[ 0 ] + mtx[ 1 ] * te[ 3 ] + mtx[ 2 ] * te[ 6 ];

    // no inverse

    if ( det == 0 && result ) {

        *result = false;
        identity();

        return *this;

    }
    else if (result) {

        *result = true;

    }

    multiplyScalar( 1.0f / det );

    return *this;
}

Matrix3& Matrix3::getNormalMatrix(const Matrix4& mtx)
{
    return getInverse(mtx).transpose();
}

Matrix3 Matrix3::clone()
{
    return Matrix3(*this);
}

}
