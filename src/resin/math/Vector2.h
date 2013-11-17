#pragma once

#include "glm/glm.hpp"

namespace RESIN {

class Vector2
{
  public:
    glm::vec2 v;

    Vector2(float x=0.0f, float y=0.0f);
    Vector2(const glm::vec2& vec);

    float& x();
    float& y();

    const float& x() const;
    const float& y() const;

    Vector2& set( float x, float y );
    Vector2& setX( float x );
    Vector2& setY( float y );
    void setComponent( int index, float value );
    float getComponent( int index );
    Vector2& copy( const Vector2& vec );
    Vector2& add( const Vector2& vec );
    Vector2& addVectors( const Vector2& a, const Vector2& b );
    Vector2& addScalar( float s );
    Vector2& sub( const Vector2& vec );
    Vector2& subVectors( const Vector2& a, const Vector2& b );
    Vector2& multiplyScalar( float s );
    Vector2& divideScalar( float scalar );
    Vector2& min( const Vector2& vec );
    Vector2& max( const Vector2& vec );
    Vector2& clamp( const Vector2& min, const Vector2& max );
    Vector2& negate();
    float dot( const Vector2& vec );
    float lengthSq();
    float length();
    Vector2& normalize();
    float distanceTo( const Vector2& vec );
    float distanceToSquared( const Vector2& vec );
    Vector2& setLength( float l );
    Vector2& lerp( const Vector2& vec, float alpha );
    bool equals( const Vector2& vec );

    Vector2& fromArray( float* array );
    void toArray( float* array ) const;
    Vector2 clone();
};

}
