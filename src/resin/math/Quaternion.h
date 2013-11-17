#pragma once

#include "glm/gtc/quaternion.hpp"

namespace RESIN {

class Euler;
class Vector3;
class Matrix4;

class Quaternion
{
  public:
    glm::quat q;

    Quaternion();
    Quaternion(const Quaternion& qt);
    Quaternion(const glm::quat& qt);

    const float& x() const;
    const float& y() const;
    const float& z() const;
    const float& w() const;

    void setX(float x);
    void setY(float y);
    void setZ(float z);
    void setW(float w);

    Quaternion& copy(const Quaternion& qt);

    Quaternion& set(float x, float y, float z, float w);
    Quaternion& setFromEuler(const Euler& euler, bool update=true);
    Quaternion& setFromAxisAngle(const Vector3& axis, float angle);
    Quaternion& setFromRotationMatrix(const Matrix4& m);
    Quaternion& inverse();
    Quaternion& conjugate();
    float lengthSq() const;
    float length() const;
    Quaternion& normalize();
    Quaternion& multiply(const Quaternion& qt);
    Quaternion& multiplyQuaternions(const Quaternion& a, const Quaternion& b);
    Quaternion& slerp(const Quaternion& qb, float t);
    bool equals(const Quaternion& qt);
    Quaternion clone();

    Euler* _euler;

  protected:
    void _updateEuler();

};

}
