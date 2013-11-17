#include "resin/math/Euler.h"
#include "resin/math/Math.h"
#include "resin/math/Matrix4.h"
#include "resin/math/Quaternion.h"

#include <cmath>

using RESIN::Math::clamp;

namespace RESIN {

// THREE.Euler.RotationOrders = [ 'XYZ', 'YZX', 'ZXY', 'XZY', 'YXZ', 'ZYX' ];
int Euler::DefaultOrder = 'XYZ';

Euler::Euler(float x, float y, float z, int order)
    : mX(x), mY(y), mZ(z), mOrder(order)
{
}

Euler::Euler() : mX(0), mY(0), mZ(0), mOrder(Euler::DefaultOrder)
{
}

void Euler::_updateQuaternion()
{
    if (_quaternion) {
        _quaternion->setFromEuler(*this, false);
    }
}

const float& Euler::x() const
{
    return mX;
}

const float& Euler::y() const
{
    return mY;
}

const float& Euler::z() const
{
    return mZ;
}

void Euler::setX(float x)
{
    mX = x;
    _updateQuaternion();
}

void Euler::setY(float y)
{
    mY = y;
    _updateQuaternion();
}

void Euler::setZ(float z)
{
    mZ = z;
    _updateQuaternion();
}

const int& Euler::order() const
{
    return mOrder;
}

void Euler::setOrder(int order)
{
    mOrder = order;
    _updateQuaternion();
}

Euler& Euler::set(float x, float y, float z, int order)
{
    mX = x;
    mY = y;
    mZ = z;

    if (order)
        mOrder = order;

    _updateQuaternion();

    return *this;
}

Euler& Euler::copy(const Euler& euler)
{
    mX = euler.mX;
    mY = euler.mY;
    mZ = euler.mZ;
    mOrder = euler.mOrder;

    _updateQuaternion();

    return *this;
}

Euler& Euler::setFromRotationMatrix(const Matrix4& m, int order)
{
    // assumes the upper 3x3 of m is a pure rotation matrix (i.e, unscaled)

    const Matrix4& te = m.elements();
    float m11 = te[0], m12 = te[4], m13 = te[8];
    float m21 = te[1], m22 = te[5], m23 = te[9];
    float m31 = te[2], m32 = te[6], m33 = te[10];

    if (!order)
        order = mOrder;

    if ( order == 'XYZ' ) {

        mY = asin( clamp( m13, -1.0f, 1.0f ) );

        if ( fabs( m13 ) < 0.99999f ) {

            mX = atan2( - m23, m33 );
            mZ = atan2( - m12, m11 );

        } else {

            mX = atan2( m32, m22 );
            mZ = 0;

        }

    } else if ( order == 'YXZ' ) {

        mX = asin( - clamp( m23, -1.0f, 1.0f ) );

        if ( fabs( m23 ) < 0.99999f ) {

            mY = atan2( m13, m33 );
            mZ = atan2( m21, m22 );

        } else {

            mY = atan2( - m31, m11 );
            mZ = 0;

        }

    } else if ( order == 'ZXY' ) {

        mX = asin( clamp( m32, -1.0f, 1.0f ) );

        if ( fabs( m32 ) < 0.99999f ) {

            mY = atan2( - m31, m33 );
            mZ = atan2( - m12, m22 );

        } else {

            mY = 0;
            mZ = atan2( m21, m11 );

        }

    } else if ( order == 'ZYX' ) {

        mY = asin( - clamp( m31, -1.0f, 1.0f ) );

        if ( fabs( m31 ) < 0.99999f ) {

            mX = atan2( m32, m33 );
            mZ = atan2( m21, m11 );

        } else {

            mX = 0;
            mZ = atan2( - m12, m22 );

        }

    } else if ( order == 'YZX' ) {

        mZ = asin( clamp( m21, -1.0f, 1.0f ) );

        if ( fabs( m21 ) < 0.99999f ) {

            mX = atan2( - m23, m22 );
            mY = atan2( - m31, m11 );

        } else {

            mX = 0;
            mY = atan2( m13, m33 );

        }

    } else if ( order == 'XZY' ) {

        mZ = asin( - clamp( m12, -1.0f, 1.0f ) );

        if ( fabs( m12 ) < 0.99999f ) {

            mX = atan2( m32, m22 );
            mY = atan2( m13, m11 );

        } else {

            mX = atan2( - m23, m33 );
            mY = 0;

        }

    } else {

        // console.warn( 'WARNING: Euler.setFromRotationMatrix() given unsupported order: ' + order )

    }

    mOrder = order;

    _updateQuaternion();

    return *this;
}

Euler& Euler::setFromQuaternion(const Quaternion& qt, int order, bool update)
{
    // http://www.mathworks.com/matlabcentral/fileexchange/20696-function-to-convert-between-dcm-euler-angles-quaternions-and-euler-vectors/content/SpinCalc.m

    const glm::quat& q = qt.q;
    float sqx = q.x * q.x;
    float sqy = q.y * q.y;
    float sqz = q.z * q.z;
    float sqw = q.w * q.w;

    if (!order)
        order = mOrder;

    if ( order == 'XYZ' ) {

        mX = atan2( 2.0f * ( q.x * q.w - q.y * q.z ), ( sqw - sqx - sqy + sqz ) );
        mY = asin(  clamp( 2.0f * ( q.x * q.z + q.y * q.w ), -1.0f, 1.0f ) );
        mZ = atan2( 2.0f * ( q.z * q.w - q.x * q.y ), ( sqw + sqx - sqy - sqz ) );

    } else if ( order ==  'YXZ' ) {

        mX = asin(  clamp( 2.0f * ( q.x * q.w - q.y * q.z ), -1.0f, 1.0f ) );
        mY = atan2( 2.0f * ( q.x * q.z + q.y * q.w ), ( sqw - sqx - sqy + sqz ) );
        mZ = atan2( 2.0f * ( q.x * q.y + q.z * q.w ), ( sqw - sqx + sqy - sqz ) );

    } else if ( order == 'ZXY' ) {

        mX = asin(  clamp( 2.0f * ( q.x * q.w + q.y * q.z ), -1.0f, 1.0f ) );
        mY = atan2( 2.0f * ( q.y * q.w - q.z * q.x ), ( sqw - sqx - sqy + sqz ) );
        mZ = atan2( 2.0f * ( q.z * q.w - q.x * q.y ), ( sqw - sqx + sqy - sqz ) );

    } else if ( order == 'ZYX' ) {

        mX = atan2( 2.0f * ( q.x * q.w + q.z * q.y ), ( sqw - sqx - sqy + sqz ) );
        mY = asin(  clamp( 2.0f * ( q.y * q.w - q.x * q.z ), -1.0f, 1.0f ) );
        mZ = atan2( 2.0f * ( q.x * q.y + q.z * q.w ), ( sqw + sqx - sqy - sqz ) );

    } else if ( order == 'YZX' ) {

        mX = atan2( 2.0f * ( q.x * q.w - q.z * q.y ), ( sqw - sqx + sqy - sqz ) );
        mY = atan2( 2.0f * ( q.y * q.w - q.x * q.z ), ( sqw + sqx - sqy - sqz ) );
        mZ = asin(  clamp( 2.0f * ( q.x * q.y + q.z * q.w ), -1.0f, 1.0f ) );

    } else if ( order == 'XZY' ) {

        mX = atan2( 2.0f * ( q.x * q.w + q.y * q.z ), ( sqw - sqx + sqy - sqz ) );
        mY = atan2( 2.0f * ( q.x * q.z + q.y * q.w ), ( sqw + sqx - sqy - sqz ) );
        mZ = asin(  clamp( 2.0f * ( q.z * q.w - q.x * q.y ), -1.0f, 1.0f ) );

    } else {

        // console.warn( 'WARNING: Euler.setFromQuaternion() given unsupported order: ' + order )

    }

    mOrder = order;

    if (update)
        _updateQuaternion();

    return *this;
}

void Euler::reorder(int newOrder)
{
    // WARNING: this discards revolution information -bhouston

    Quaternion q;

    q.setFromEuler( *this );
    setFromQuaternion( q, newOrder );
}

bool Euler::equals(const Euler& euler)
{
    return ( euler.mX == mX ) && ( euler.mY == mY ) && ( euler.mZ == mZ ) && ( euler.mOrder == mOrder );

}

Euler Euler::clone()
{
    return Euler(mX, mY, mZ, mOrder);
}

}
