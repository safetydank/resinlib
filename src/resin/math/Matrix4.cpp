#include "glm/glm.hpp"
#include "resin/math/Math.h"
#include "resin/math/Matrix4.h"
#include "resin/math/Quaternion.h"
#include "resin/math/Vector3.h"

#include "resin/core/MemoryBuffer.h"

using namespace glm;

namespace RESIN { 

Matrix4::Matrix4() : m(1.0f)
{
}

Matrix4::Matrix4(const Matrix4& matrix) : m(matrix.m)
{
}

Matrix4::Matrix4(const mat4& matrix) : m(matrix)
{
}

Matrix4& Matrix4::elements()
{
    return *this;
}

const Matrix4& Matrix4::elements() const
{
    return *this;
}

float& Matrix4::operator[](std::size_t i)
{
    size_t c = i >> 2;
    size_t r = i - (c << 2);
    return m[c][r];
}

const float& Matrix4::operator[](std::size_t i) const
{
    size_t c = i >> 2;
    size_t r = i - (c << 2);
    return m[c][r];
}

Matrix4& Matrix4::set(
        float n11, float n12, float n13, float n14,
        float n21, float n22, float n23, float n24,
        float n31, float n32, float n33, float n34,
        float n41, float n42, float n43, float n44)
{
    Matrix4& te = elements();

    te[0] = n11; te[4] = n12; te[8] = n13; te[12] = n14;
    te[1] = n21; te[5] = n22; te[9] = n23; te[13] = n24;
    te[2] = n31; te[6] = n32; te[10] = n33; te[14] = n34;
    te[3] = n41; te[7] = n42; te[11] = n43; te[15] = n44;

    return te;
}

Matrix4& Matrix4::identity()
{
    m = mat4(1.0f);
    return *this;
}

Matrix4& Matrix4::copy(const Matrix4& mtx)
{
    m = mtx.m;
    return *this;
}

Matrix4& Matrix4::copyPosition(const Matrix4& matrix)
{
    m[12] = matrix.m[12];
    m[13] = matrix.m[13];
    m[14] = matrix.m[14];

    return *this;
}

Matrix4& Matrix4::extractRotation(const Matrix4& matrix)
{
    Vector3 v1;

    Matrix4& te = elements();
    const Matrix4& me = matrix;

    float scaleX = 1.0f / v1.set( me[0], me[1], me[2] ).length();
    float scaleY = 1.0f / v1.set( me[4], me[5], me[6] ).length();
    float scaleZ = 1.0f / v1.set( me[8], me[9], me[10] ).length();

    te[0] = me[0] * scaleX;
    te[1] = me[1] * scaleX;
    te[2] = me[2] * scaleX;

    te[4] = me[4] * scaleY;
    te[5] = me[5] * scaleY;
    te[6] = me[6] * scaleY;

    te[8] = me[8] * scaleZ;
    te[9] = me[9] * scaleZ;
    te[10] = me[10] * scaleZ;

    return *this;
}

Matrix4& Matrix4::makeRotationFromQuaternion(const Quaternion& q)
{
    Matrix4& te = elements();

    float x = q.x(), y = q.y(), z = q.z(), w = q.w();
    float x2 = x + x, y2 = y + y, z2 = z + z;
    float xx = x * x2, xy = x * y2, xz = x * z2;
    float yy = y * y2, yz = y * z2, zz = z * z2;
    float wx = w * x2, wy = w * y2, wz = w * z2;

    te[0] = 1 - ( yy + zz );
    te[4] = xy - wz;
    te[8] = xz + wy;

    te[1] = xy + wz;
    te[5] = 1 - ( xx + zz );
    te[9] = yz - wx;

    te[2] = xz - wy;
    te[6] = yz + wx;
    te[10] = 1 - ( xx + yy );

    // last column
    te[3] = 0;
    te[7] = 0;
    te[11] = 0;

    // bottom row
    te[12] = 0;
    te[13] = 0;
    te[14] = 0;
    te[15] = 1;

    return *this;
}

Matrix4& Matrix4::lookAt(const Vector3& eye, const Vector3& target, const Vector3& up)
{
    Vector3 x, y, z;

    Matrix4& te = elements();

    z.subVectors( eye, target ).normalize();

    if ( z.length() == 0 ) {
        z.v.z = 1;
    }

    x.crossVectors( up, z ).normalize();

    if ( x.length() == 0 ) {
        z.v.x += 0.0001;
        x.crossVectors( up, z ).normalize();
    }

    y.crossVectors( z, x );

    te[0] = x.v.x; te[4] = y.v.x; te[8] = z.v.x;
    te[1] = x.v.y; te[5] = y.v.y; te[9] = z.v.y;
    te[2] = x.v.z; te[6] = y.v.z; te[10] = z.v.z;

    return *this;
}

Matrix4& Matrix4::multiply(const Matrix4& mtx)
{
    m *= mtx.m;
    return *this;
}

Matrix4& Matrix4::multiplyMatrices(const Matrix4& a, const Matrix4& b)
{
    m = a.m * b.m;
    return *this;
}

Matrix4& Matrix4::multiplyScalar(float s)
{
    m *= s;
    return *this;
}

Vector3* Matrix4::multiplyVector3Array(Vector3* v, size_t count) const
{
    for (int i=0; i < count; ++i) {
        v[i].applyProjection(*this);
    }

    /*
    for ( int i = 0, il = buf.dataSize<float>(); i < il; i += 3 ) {

        auto& v = v1.v;

        v.x = a[ i ];
        v.y = a[ i + 1 ];
        v.z = a[ i + 2 ];

        v1.applyProjection( *this );

        a[ i ]     = v.x;
        a[ i + 1 ] = v.y;
        a[ i + 2 ] = v.z;

    }
    */

    return v;
}


float Matrix4::determinant() const
{
    return glm::determinant(m);
}

Matrix4& Matrix4::transpose()
{
    m = glm::transpose(m);
    return *this;
}

Matrix4& Matrix4::setPosition(const Vector3& v)
{
    Matrix4& te = elements();

    te[12] = v.x();
    te[13] = v.y();
    te[14] = v.z();

    return *this;
}

Matrix4& Matrix4::getInverse(const Matrix4& mtx)
{
    m = glm::inverse(mtx.m);
    return *this;
}

Matrix4& Matrix4::scale(const Vector3& v)
{
    Matrix4& te = elements();
    float x = v.x(), y = v.y(), z = v.z();

    te[0] *= x; te[4] *= y; te[8] *= z;
    te[1] *= x; te[5] *= y; te[9] *= z;
    te[2] *= x; te[6] *= y; te[10] *= z;
    te[3] *= x; te[7] *= y; te[11] *= z;

    return *this;
}

float Matrix4::getMaxScaleOnAxis() const
{
    const Matrix4& te = elements();

    float scaleXSq = te[0] * te[0] + te[1] * te[1] + te[2] * te[2];
    float scaleYSq = te[4] * te[4] + te[5] * te[5] + te[6] * te[6];
    float scaleZSq = te[8] * te[8] + te[9] * te[9] + te[10] * te[10];

    return sqrt( Math::max( scaleXSq, Math::max( scaleYSq, scaleZSq ) ) );
}

Matrix4& Matrix4::makeTranslation( float x, float y, float z )
{
    set(

        1, 0, 0, x,
        0, 1, 0, y,
        0, 0, 1, z,
        0, 0, 0, 1

    );

    return *this;
}

Matrix4& Matrix4::makeRotationX( float theta )
{
    float c = cos( theta ), s = sin( theta );

    set(

        1, 0,  0, 0,
        0, c, -s, 0,
        0, s,  c, 0,
        0, 0,  0, 1

    );

    return *this;
}

Matrix4& Matrix4::makeRotationY( float theta )
{
    float c = cos( theta ), s = sin( theta );

    set(

         c, 0, s, 0,
         0, 1, 0, 0,
        -s, 0, c, 0,
         0, 0, 0, 1

    );

    return *this;
}

Matrix4& Matrix4::makeRotationZ( float theta ) {

    float c = cos( theta ), s = sin( theta );

    set(

        c, -s, 0, 0,
        s,  c, 0, 0,
        0,  0, 1, 0,
        0,  0, 0, 1

    );

    return *this;

}

Matrix4& Matrix4::makeRotationAxis( const Vector3& axis, float angle )
{
    // Based on http://www.gamedev.net/reference/articles/article1199.asp

    float c = cos( angle );
    float s = sin( angle );
    float t = 1 - c;
    float x = axis.x(), y = axis.y(), z = axis.z();
    float tx = t * x, ty = t * y;

    set(

        tx * x + c, tx * y - s * z, tx * z + s * y, 0,
        tx * y + s * z, ty * y + c, ty * z - s * x, 0,
        tx * z - s * y, ty * z + s * x, t * z * z + c, 0,
        0, 0, 0, 1

    );

    return *this;
}

Matrix4& Matrix4::makeScale( float x, float y, float z )
{
    set(

        x, 0, 0, 0,
        0, y, 0, 0,
        0, 0, z, 0,
        0, 0, 0, 1

    );

    return *this;
}

Matrix4& Matrix4::compose(const Vector3& position, const Quaternion& quaternion, const Vector3& scale_)
{
    makeRotationFromQuaternion( quaternion );
    scale( scale_ );
    setPosition( position );

    return *this;
}

Matrix4& Matrix4::decompose(Vector3* position, Quaternion* quaternion, Vector3* scale)
{
    Vector3 vector;
    Matrix4 matrix;

    Matrix4& te = elements();

    float sx = vector.set( te[0], te[1], te[2] ).length();
    float sy = vector.set( te[4], te[5], te[6] ).length();
    float sz = vector.set( te[8], te[9], te[10] ).length();

    position->x() = te[12];
    position->y() = te[13];
    position->z() = te[14];

    // scale the rotation part

    matrix = Matrix4(*this);
    // matrix.elements.set( this.elements ); // at this point matrix is incomplete so we can't use .copy()

    float invSX = 1.0f / sx;
    float invSY = 1.0f / sy;
    float invSZ = 1.0f / sz;

    matrix[0] *= invSX;
    matrix[1] *= invSX;
    matrix[2] *= invSX;

    matrix[4] *= invSY;
    matrix[5] *= invSY;
    matrix[6] *= invSY;

    matrix[8] *= invSZ;
    matrix[9] *= invSZ;
    matrix[10] *= invSZ;

    quaternion->setFromRotationMatrix( matrix );

    scale->x() = sx;
    scale->y() = sy;
    scale->z() = sz;

    return *this;
}

Matrix4& Matrix4::makeFrustum(float left, float right, float bottom, float top, float near, float far)
{
    Matrix4& te = elements();
    float x = 2 * near / ( right - left );
    float y = 2 * near / ( top - bottom );

    float a = ( right + left ) / ( right - left );
    float b = ( top + bottom ) / ( top - bottom );
    float c = - ( far + near ) / ( far - near );
    float d = - 2 * far * near / ( far - near );

    te[0] = x;  te[4] = 0;  te[8] = a;  te[12] = 0;
    te[1] = 0;  te[5] = y;  te[9] = b;  te[13] = 0;
    te[2] = 0;  te[6] = 0;  te[10] = c; te[14] = d;
    te[3] = 0;  te[7] = 0;  te[11] = - 1;   te[15] = 0;

    return *this;
}

Matrix4& Matrix4::makePerspective(float fov, float aspect, float near, float far)
{
    float ymax = near * tan( Math::degToRad( fov * 0.5 ) );
    float ymin = - ymax;
    float xmin = ymin * aspect;
    float xmax = ymax * aspect;

    return makeFrustum( xmin, xmax, ymin, ymax, near, far );
}

Matrix4& Matrix4::makeOrthographic(float left, float right, float top, float bottom, float near, float far)
{
    Matrix4& te = elements();
    float w = right - left;
    float h = top - bottom;
    float p = far - near;

    float x = ( right + left ) / w;
    float y = ( top + bottom ) / h;
    float z = ( far + near ) / p;

    te[0] = 2 / w;  te[4] = 0;  te[8] = 0;  te[12] = -x;
    te[1] = 0;  te[5] = 2 / h;  te[9] = 0;  te[13] = -y;
    te[2] = 0;  te[6] = 0;  te[10] = -2/p;  te[14] = -z;
    te[3] = 0;  te[7] = 0;  te[11] = 0; te[15] = 1;

    return *this;
}

Matrix4 Matrix4::clone()
{
    return Matrix4(*this);
}

}
