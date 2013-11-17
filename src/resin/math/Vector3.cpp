#include "resin/math/Math.h"
#include "resin/math/Matrix3.h"
#include "resin/math/Matrix4.h"
#include "resin/math/Vector3.h"

using namespace glm;

namespace RESIN {

Vector3::Vector3()
{
}

Vector3::Vector3(float x, float y, float z) : v(x, y, z)
{
}

Vector3::Vector3(const Vector3& vec) : v(vec.v)
{
}

Vector3::Vector3(const vec3& vec) : v(vec)
{
}


float& Vector3::x()
{
    return v.x;
}

float& Vector3::y()
{
    return v.y;
}

float& Vector3::z()
{
    return v.z;
}

const float& Vector3::x() const
{
    return v.x;
}

const float& Vector3::y() const
{
    return v.y;
}

const float& Vector3::z() const
{
    return v.z;
}


Vector3& Vector3::set(float x, float y, float z)
{
    v.x = x;
    v.y = y;
    v.z = z;

    return *this;
}


Vector3& Vector3::setX(float x)
{
    v.x = x;
    return *this;
}


Vector3& Vector3::setY(float y)
{
    v.y = y;
    return *this;
}


Vector3& Vector3::setZ(float z)
{
    v.z = z;
    return *this;
}

void Vector3::setComponent(int index, float value)
{
    v[index] = value;
}

float Vector3::getComponent(int index, float value)
{
    return v[index];
}

Vector3& Vector3::copy(const Vector3& vec)
{
    v = vec.v;
    return *this;
}


Vector3& Vector3::add(const Vector3& vec)
{
    v += vec.v;
    return *this;
}

Vector3& Vector3::addScalar(float s)
{
    v += s;
    return *this;
}

Vector3& Vector3::addVectors(const Vector3& a, const Vector3& b)
{
    v = a.v + b.v;
    return *this;
}


Vector3& Vector3::sub(const Vector3& vec)
{
    v -= vec.v;
    return *this;
}


Vector3& Vector3::subVectors(const Vector3& a, const Vector3& b)
{
    v = a.v - b.v;
    return *this;
}


Vector3& Vector3::multiply(const Vector3& vec)
{
    v *= vec.v;
    return *this;
}


Vector3& Vector3::multiplyScalar(float s)
{
    v *= s;
    return *this;
}


Vector3& Vector3::multiplyVectors(const Vector3& a, const Vector3& b)
{
    v = a.v * b.v;
    return *this;
}

Vector3& Vector3::applyMatrix3(const Matrix3& mtx)
{
    v = mtx.m * v;

    return *this;
}

Vector3& Vector3::applyMatrix4(const Matrix4& m)
{
    float x = v.x, y = v.y, z = v.z;

    v.x = m[0] * x + m[4] * y + m[8]  * z + m[12];
    v.y = m[1] * x + m[5] * y + m[9]  * z + m[13];
    v.z = m[2] * x + m[6] * y + m[10] * z + m[14];

    return *this;
}


Vector3& Vector3::applyProjection(const Matrix4& m)
{
    float x = v.x, y = v.y, z = v.z;

    float d = 1.0f / ( m[3] * x + m[7] * y + m[11] * z + m[15] ); // perspective divide

    v.x = ( m[0] * x + m[4] * y + m[8]  * z + m[12] ) * d;
    v.y = ( m[1] * x + m[5] * y + m[9]  * z + m[13] ) * d;
    v.z = ( m[2] * x + m[6] * y + m[10] * z + m[14] ) * d;

    return *this;
}


Vector3& Vector3::applyQuaternion(const Quaternion& q)
{
    return *this;
}


Vector3& Vector3::transformDirection(const Matrix4& m)
{
    return *this;
}


Vector3& Vector3::divide(const Vector3& vec)
{
    v /= vec.v;
    return *this;
}


Vector3& Vector3::divideScalar(float scalar)
{
    v /= scalar;
    return *this;
}

Vector3& Vector3::min(const Vector3& vec)
{
    return *this;
}


Vector3& Vector3::max(const Vector3& vec)
{
    return *this;
}


Vector3& Vector3::clamp(const Vector3& min, const Vector3& max)
{
    return *this;
}

Vector3& Vector3::negate()
{
    v = -v;
    return *this;
}

float Vector3::dot(const Vector3& vec) const
{
    return glm::dot(v, vec.v);
}

float Vector3::length() const
{
    return glm::length(v);
}

float Vector3::lengthSq() const
{
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

float Vector3::lengthManhattan() const
{
    return fabs( v.x ) + fabs( v.y ) + fabs( v.z );
}

Vector3& Vector3::normalize()
{
    v = glm::normalize(v);
    return *this;
}

Vector3& Vector3::lerp(const Vector3& vec, float alpha)
{
    v = glm::mix(v, vec.v, alpha);
    return *this;
}

Vector3& Vector3::cross(const Vector3& vec)
{
    v = glm::cross(v, vec.v);
    return *this;
}

Vector3& Vector3::crossVectors(const Vector3& a, const Vector3& b)
{
    v = glm::cross(a.v, b.v);
    return *this;
}

float Vector3::angleTo(const Vector3& vec) const
{
    float theta = dot(vec) / (length() * vec.length());

    // clamp, to handle numerical problems
    return acos(Math::clamp(theta, -1.0f, 1.0f));
}

float Vector3::distanceTo(const Vector3& vec) const
{
    return sqrt(distanceToSquared(vec));
}

float Vector3::distanceToSquared(const Vector3& vec) const
{
    float dx = v.x - vec.v.x;
    float dy = v.y - vec.v.y;
    float dz = v.z - vec.v.z;

    return dx * dx + dy * dy + dz * dz;
}

Vector3& Vector3::getPositionFromMatrix( const Matrix4& m )
{
    v.x = m[12];
    v.y = m[13];
    v.z = m[14];

    return *this;
}

Vector3& Vector3::getScaleFromMatrix( const Matrix4& m )
{
    float sx = Vector3( m[0], m[1], m[2] ).length();
    float sy = Vector3( m[4], m[5], m[6] ).length();
    float sz = Vector3( m[8], m[9], m[10] ).length();

    v.x = sx;
    v.y = sy;
    v.z = sz;

    return *this;
}

Vector3& Vector3::getColumnFromMatrix( int index, const Matrix4& matrix )
{
    int offset = index * 4;

    const Matrix4& me = matrix.elements();

    v.x = me[ offset ];
    v.y = me[ offset + 1 ];
    v.z = me[ offset + 2 ];

    return *this;
}

bool Vector3::equals(const Vector3& vec) const
{
    return v == vec.v;
}

Vector3 Vector3::clone() const
{
    return Vector3(v);
}

}
