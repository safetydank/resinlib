#pragma once

namespace RESIN { 

class Matrix4;
class Quaternion;

class Euler
{
  protected:
    static int DefaultOrder;

    float mX, mY, mZ;
    int mOrder;

    void _updateQuaternion();

  public:
    Quaternion* _quaternion;

    Euler(float x, float y, float z, int order=DefaultOrder);
    Euler();

    const float& x() const;
    const float& y() const;
    const float& z() const;

    void setX(float x);
    void setY(float y);
    void setZ(float z);

    const int& order() const;
    void setOrder(int order);

    Euler& set(float x, float y, float z, int order=0);
    Euler& copy(const Euler& euler);
    Euler& setFromRotationMatrix(const Matrix4& m, int order=0);
    Euler& setFromQuaternion(const Quaternion& qt, int order=0, bool update=true);
    void reorder(int newOrder);
    bool equals(const Euler& euler);
    Euler clone();
};

}
