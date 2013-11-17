#pragma once

#include "resin/core/CoreFwd.h"
#include "glm/glm.hpp"

namespace RESIN {

class Matrix4;
class Vector3;
class Quaternion;

class Matrix3
{
  public:
    glm::mat3 m;

    Matrix3();
    Matrix3(const Matrix3& mtx);
    Matrix3(const glm::mat3& mtx);

    Matrix3& elements();
    const Matrix3& elements() const;
    float& operator[](std::size_t i);
    const float& operator[](std::size_t i) const;

    Matrix3& set(float n11, float n12, float n13,
                 float n21, float n22, float n23,
                 float n31, float n32, float n33);

    Matrix3& identity();
	Matrix3& copy(const Matrix3& mtx);
    Vector3* multiplyVector3Array(Vector3* v, size_t count) const;
    Matrix3& multiplyScalar(float s);

    float determinant() const;
    Matrix3& transpose();

    Matrix3& getInverse(const Matrix4& mtx, bool* result = NULL);
    Matrix3& getNormalMatrix(const Matrix4& mtx);

    Matrix3 clone();
};

}
