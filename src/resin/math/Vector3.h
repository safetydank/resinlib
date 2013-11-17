#pragma once

#include "glm/glm.hpp"

namespace RESIN {

class Matrix3;
class Matrix4;
class Quaternion;

class Vector3
{
  public:
    glm::vec3 v;

    Vector3();
    Vector3(float x, float y, float z);
    Vector3(const Vector3& vec);
    Vector3(const glm::vec3& vec);

    float& x();
    float& y();
    float& z();

    const float& x() const;
    const float& y() const;
    const float& z() const;

    Vector3& set(float x, float y, float z);
    Vector3& setX(float x);
    Vector3& setY(float y);
    Vector3& setZ(float z);
	void setComponent(int index, float value);
    float getComponent(int index, float value);

    Vector3& copy(const Vector3& vec);
    Vector3& add(const Vector3& vec);
    Vector3& addScalar(float s);
    Vector3& addVectors(const Vector3& a, const Vector3& b);
    Vector3& sub(const Vector3& vec);
    Vector3& subVectors(const Vector3& a, const Vector3& b);
    Vector3& multiply(const Vector3& vec);
    Vector3& multiplyScalar(float s);
    Vector3& multiplyVectors(const Vector3& a, const Vector3& b);
    Vector3& applyMatrix3(const Matrix3& m);
    Vector3& applyMatrix4(const Matrix4& m);
    Vector3& applyProjection(const Matrix4& m);
    Vector3& applyQuaternion(const Quaternion& q);
    Vector3& transformDirection(const Matrix4& m);
    Vector3& divide(const Vector3& vec);
    Vector3& divideScalar(float scalar);

    Vector3& min(const Vector3& vec);
    Vector3& max(const Vector3& vec);
    Vector3& clamp(const Vector3& min, const Vector3& max);

    Vector3& negate();
    float dot(const Vector3& vec) const;

    float length() const;
    float lengthSq() const;
    float lengthManhattan() const;

    Vector3& normalize();

    Vector3& lerp(const Vector3& v, float alpha);
    Vector3& cross(const Vector3& vec);
    Vector3& crossVectors(const Vector3& a, const Vector3& b);

    float angleTo(const Vector3& vec) const;
    float distanceTo(const Vector3& vec) const;
    float distanceToSquared(const Vector3& vec) const;

    Vector3& getPositionFromMatrix( const Matrix4& m );
    Vector3& getScaleFromMatrix( const Matrix4& m );
    Vector3& getColumnFromMatrix( int index, const Matrix4& matrix );

    bool equals(const Vector3& vec) const;

    Vector3 clone() const;
};

}
