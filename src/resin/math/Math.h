#pragma once

#include <cmath>
#include <string>

namespace RESIN { namespace Math {

const double PI  = 3.14159265358979323846;
const double PI2 = M_PI * 2.0f;

std::string generateUUID();

template<typename T>
T clamp(T x, T a, T b)
{
    return (x < a) ? a : (( x > b ) ? b : x);
}

template<typename T>
T clampBottom(T x, T a)
{
    return x < a ? a : x;
}

template <typename T>
T mapLinear(T x, T a1, T a2, T b1, T b2)
{
    return b1 + (x - a1) * (b2 - b1) / (a2 - a1);
}

// http://en.wikipedia.org/wiki/Smoothstep
template <typename T>
T smoothstep(T x, T min, T max)
{
    if ( x <= min ) return 0;
    if ( x >= max ) return 1;

    x = ( x - min )/( max - min );

    return x*x*(3 - 2*x);
}

template <typename T>
T smootherstep(T x, T min, T max)
{
    if ( x <= min ) return 0;
    if ( x >= max ) return 1;

    x = (x - min)/(max - min);

    return x*x*x*(x*(x*6 - 15) + 10);
}

double random();

// // Random float from <0, 1> with 16 bits of randomness
// // (standard Math.random() creates repetitive patterns when applied over larger space)
// 
// random16: function () {
// 
//     return ( 65280 * Math.random() + 255 * Math.random() ) / 65535;
// 
// },
// 
// // Random integer from <low, high> interval
// 
// randInt: function ( low, high ) {
// 
//     return low + Math.floor( Math.random() * ( high - low + 1 ) );
// 
// },
// 
// // Random float from <low, high> interval
// 
// randFloat: function ( low, high ) {
// 
//     return low + Math.random() * ( high - low );
// 
// },
// 
// // Random float from <-range/2, range/2> interval
// 
// randFloatSpread: function ( range ) {
// 
//     return range * ( 0.5 - Math.random() );
// 
// },
// 

template <typename T>
T sign(T x)
{
    return ( x < 0 ) ? -1 : ( ( x > 0 ) ? 1 : 0 );
}

template <typename T>
T degToRad(T degrees)
{
    const T degreeToRadiansFactor = M_PI / 180.0;
    return degrees * degreeToRadiansFactor;
}

template <typename T>
T radToDeg(T radians)
{
    const T radianToDegreesFactor = 180.0 / M_PI;
    return radians * radianToDegreesFactor;
}

template <typename T>
T max(const T& a, const T& b)
{
    return (a > b ? a : b);
}

template <typename T>
T min(const T& a, const T& b)
{
    return (a < b ? a : b);
}


} }  // namespace RESIN::math

