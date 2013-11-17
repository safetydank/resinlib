#pragma once

#include "glm/glm.hpp"

namespace RESIN {
class MemoryBuffer;
}

namespace RESIN {

class Vector3;
class Quaternion;

class Matrix4
{
  public:
    glm::mat4 m;

    Matrix4();
    Matrix4(const Matrix4& mtx);
    Matrix4(const glm::mat4& mtx);

    Matrix4& elements();
    const Matrix4& elements() const;
    float& operator[](std::size_t i);
    const float& operator[](std::size_t i) const;

    Matrix4& set(float n11, float n12, float n13, float n14,
                 float n21, float n22, float n23, float n24,
                 float n31, float n32, float n33, float n34,
                 float n41, float n42, float n43, float n44);

    Matrix4& identity();
	Matrix4& copy(const Matrix4& mtx);
    Matrix4& copyPosition(const Matrix4& mtx);
	Matrix4& extractRotation(const Matrix4& mtx);
	// Matrix4& makeRotationFromEuler(const Euler& euler);
	Matrix4& makeRotationFromQuaternion(const Quaternion& q);
    Matrix4& lookAt(const Vector3& eye, const Vector3& target, const Vector3& up);

    Matrix4& multiply(const Matrix4& mtx);
    Matrix4& multiplyMatrices(const Matrix4& a, const Matrix4& b);
    Matrix4& multiplyScalar(float s);
    Vector3* multiplyVector3Array(Vector3* v, size_t count) const;

    float determinant() const;
    Matrix4& transpose();

    Matrix4& setPosition(const Vector3& v);
    Matrix4& getInverse(const Matrix4& mtx);

    Matrix4& scale(const Vector3& v);
    float getMaxScaleOnAxis() const;

    Matrix4& makeTranslation(float x, float y, float z);
    Matrix4& makeRotationX(float theta);
    Matrix4& makeRotationY(float theta);
    Matrix4& makeRotationZ(float theta);
    Matrix4& makeRotationAxis(const Vector3& axis, float angle);
    Matrix4& makeScale(float x, float y, float z);
    Matrix4& compose(const Vector3& position, const Quaternion& quaternion, const Vector3& scale);
    Matrix4& decompose(Vector3* position, Quaternion* quaternion, Vector3* scale);

    Matrix4& makeFrustum(float left, float right, float bottom, float top, float near, float far);
    Matrix4& makePerspective(float fov, float aspect, float near, float far);
	Matrix4& makeOrthographic(float left, float right, float top, float bottom, float near, float far);

    Matrix4 clone();
};

}
