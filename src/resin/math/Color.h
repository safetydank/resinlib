#pragma once

#include <cstdint>
#include <string>

namespace RESIN { 

class Color
{
  public:
    Color();

    Color(uint32_t hex);
    Color(const std::string& style);

    Color& set( const Color& value );
        Color& set( uint32_t value );
    Color& set( const std::string& value );

    Color& setHex( uint32_t hex );
    Color& setRGB( float r, float g, float b );

    Color& setHSL( float h, float s, float l );
    Color& setStyle( const std::string& style );
    Color& copy( const Color& color );
    Color& copyGammaToLinear( const Color& color );
    Color& copyLinearToGamma( const Color& color );
    Color& convertGammaToLinear();
    Color& convertLinearToGamma();

    uint32_t getHex() const;
    std::string getHexString() const;

    Color& add( const Color& color );
    Color& addColors( const Color& color1, const Color& color2 );
    Color& addScalar( float s );
    Color& multiply( const Color& color );
    Color& multiplyScalar(float s);
    Color& lerp( const Color& color, float alpha );
    bool equals( const Color& c ) const;

    float r, g, b;
};

}
