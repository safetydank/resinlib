#pragma once

#include "glm/glm.hpp"

namespace RESIN {

class Matrix4;
class Quaternion;

class Vector4
{
  public:
    glm::vec4 v;

    Vector4( float x=0.0f, float y=0.0f, float z=0.0f, float w=1.0f );
    Vector4( const Vector4& vec );

    const float& x() const;
    const float& y() const;
    const float& z() const;
    const float& w() const;
    float& x();
    float& y();
    float& z();
    float& w();

    Vector4& set( float x, float y, float z, float w );
    Vector4& setX( float x );
    Vector4& setY( float y );
    Vector4& setZ( float z );
    Vector4& setW( float w );
    void setComponent( int index, float value );
    float getComponent( int index ) const;
    Vector4& copy( const Vector4& vec );
    Vector4& add( const Vector4& vec );
    Vector4& addScalar( float s );
    Vector4& addVectors( const Vector4& a, const Vector4& b );
    Vector4& sub( const Vector4& vec );
    Vector4& subVectors( const Vector4& a, const Vector4& b );
    Vector4& multiplyScalar( float scalar );
    Vector4& applyMatrix4( const Matrix4& m );
    Vector4& divideScalar( float scalar );
    Vector4& setAxisAngleFromQuaternion( const Quaternion& q );
    Vector4& setAxisAngleFromRotationMatrix( const Matrix4& m );
    Vector4& min( const Vector4& vec );
    Vector4& max( const Vector4& vec );
    Vector4& clamp( const Vector4& min, const Vector4& max );
    Vector4& negate();
    float dot( const Vector4& vec ) const;
    float lengthSq() const;
    float length() const;
    float lengthManhattan() const;
    Vector4& normalize();
    Vector4& setLength( float l );
    Vector4& lerp( const Vector4& vec, float alpha );
    bool equals( const Vector4& vec ) const;
};

}
