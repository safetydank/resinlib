#include "resin/math/Euler.h"
#include "resin/math/Matrix4.h"
#include "resin/math/Quaternion.h"
#include "resin/math/Vector3.h"

#include "glm/gtc/quaternion.hpp"

#include <cmath>

using namespace glm;

namespace RESIN {

Quaternion::Quaternion() : _euler(NULL)
{
}

Quaternion::Quaternion(const Quaternion& qt) : q(qt.q), _euler(NULL)
{
}

Quaternion::Quaternion(const quat& qt) : q(qt), _euler(NULL)
{
}

void Quaternion::_updateEuler()
{
    if (_euler) {
        _euler->setFromQuaternion(*this, 0, false);
    }
}

const float& Quaternion::x() const
{
    return q.x;
}

const float& Quaternion::y() const
{
    return q.y;
}

const float& Quaternion::z() const
{
    return q.z;
}

const float& Quaternion::w() const
{
    return q.w;
}

void Quaternion::setX(float x)
{
    q.x = x;
    _updateEuler();
}

void Quaternion::setY(float y)
{
    q.y = y;
    _updateEuler();
}

void Quaternion::setZ(float z)
{
    q.z = z;
    _updateEuler();
}

void Quaternion::setW(float w)
{
    q.w = w;
    _updateEuler();
}

Quaternion& Quaternion::copy(const Quaternion& qt)
{
    q = qt.q;
    return *this;
}

Quaternion& Quaternion::set(float x, float y, float z, float w)
{
    q = quat(w, x, y, z);
    _updateEuler();

    return *this;
}

Quaternion& Quaternion::setFromEuler(const Euler& euler, bool update)
{
    // http://www.mathworks.com/matlabcentral/fileexchange/
    // 	20696-function-to-convert-between-dcm-euler-angles-quaternions-and-euler-vectors/
    //	content/SpinCalc.m

    float c1 = cos( euler.x() / 2 );
    float c2 = cos( euler.y() / 2 );
    float c3 = cos( euler.z() / 2 );
    float s1 = sin( euler.x() / 2 );
    float s2 = sin( euler.y() / 2 );
    float s3 = sin( euler.z() / 2 );

    if ( euler.order() == 'XYZ' ) {

        q.x = s1 * c2 * c3 + c1 * s2 * s3;
        q.y = c1 * s2 * c3 - s1 * c2 * s3;
        q.z = c1 * c2 * s3 + s1 * s2 * c3;
        q.w = c1 * c2 * c3 - s1 * s2 * s3;

    } else if ( euler.order() == 'YXZ' ) {

        q.x = s1 * c2 * c3 + c1 * s2 * s3;
        q.y = c1 * s2 * c3 - s1 * c2 * s3;
        q.z = c1 * c2 * s3 - s1 * s2 * c3;
        q.w = c1 * c2 * c3 + s1 * s2 * s3;

    } else if ( euler.order() == 'ZXY' ) {

        q.x = s1 * c2 * c3 - c1 * s2 * s3;
        q.y = c1 * s2 * c3 + s1 * c2 * s3;
        q.z = c1 * c2 * s3 + s1 * s2 * c3;
        q.w = c1 * c2 * c3 - s1 * s2 * s3;

    } else if ( euler.order() == 'ZYX' ) {

        q.x = s1 * c2 * c3 - c1 * s2 * s3;
        q.y = c1 * s2 * c3 + s1 * c2 * s3;
        q.z = c1 * c2 * s3 - s1 * s2 * c3;
        q.w = c1 * c2 * c3 + s1 * s2 * s3;

    } else if ( euler.order() == 'YZX' ) {

        q.x = s1 * c2 * c3 + c1 * s2 * s3;
        q.y = c1 * s2 * c3 + s1 * c2 * s3;
        q.z = c1 * c2 * s3 - s1 * s2 * c3;
        q.w = c1 * c2 * c3 - s1 * s2 * s3;

    } else if ( euler.order() == 'XZY' ) {

        q.x = s1 * c2 * c3 - c1 * s2 * s3;
        q.y = c1 * s2 * c3 - s1 * c2 * s3;
        q.z = c1 * c2 * s3 + s1 * s2 * c3;
        q.w = c1 * c2 * c3 + s1 * s2 * s3;

    }

    if (update)
        _updateEuler();

    return *this;
}

Quaternion& Quaternion::setFromAxisAngle(const Vector3& axis, float angle)
{
    q = glm::angleAxis(angle, axis.v);
    _updateEuler();

    return *this;
}

Quaternion& Quaternion::setFromRotationMatrix(const Matrix4& mtx)
{
    q = glm::quat_cast(mtx.m);
    _updateEuler();

    return *this;
}

Quaternion& Quaternion::inverse()
{
    conjugate().normalize();

    return *this;
}

Quaternion& Quaternion::conjugate()
{
    q.x *= -1;
    q.y *= -1;
    q.z *= -1;

    _updateEuler();

    return *this;
}

float Quaternion::lengthSq() const
{
    return q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
}

float Quaternion::length() const
{
    return glm::length(q);
}

Quaternion& Quaternion::normalize()
{
    q = glm::normalize(q);

    return *this;
}

Quaternion& Quaternion::multiply(const Quaternion& qt)
{
    q = q * qt.q;

    return *this;
}

Quaternion& Quaternion::multiplyQuaternions(const Quaternion& a, const Quaternion& b)
{
    q = a.q * b.q;
    _updateEuler();

    return *this;
}

Quaternion& Quaternion::slerp(const Quaternion& qb, float t)
{
    q = glm::slerp(q, qb.q, t);
    _updateEuler();

    return *this;
}

bool Quaternion::equals(const Quaternion& qt)
{
    return q == qt.q; 
}

Quaternion Quaternion::clone()
{
    return Quaternion(q);
}

}
