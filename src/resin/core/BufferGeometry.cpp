/**
 * @author alteredq / http://alteredqualia.com/
 */

#include "resin/core/BufferGeometry.h"
#include "resin/core/MemoryBuffer.h"
#include "resin/math/Math.h"
#include "resin/math/Matrix3.h"
#include "resin/math/Matrix4.h"

namespace RESIN {

int BufferGeometry::GeometryIdCount = 1;

BufferGeometryRef BufferGeometry::create()
{
    return BufferGeometryRef(new BufferGeometry);
}

BufferGeometry::BufferGeometry()
{
	mId = GeometryIdCount ++;
	mUuid = Math::generateUUID();

	mName = "";

	// attributes typed arrays are kept only if dynamic flag is set
	dynamic() = true;

	// offsets for chunks when using indexed elements
	// offsets = [];

	mHasTangents = false;

	// for compatibility
	// this.morphTargets = [];

    mBoundingSphereNeedsUpdate = true;

    mHasTangents = false;

    mVerticesNeedUpdate = false;
    mNormalsNeedUpdate = false;

    __webglInit = false;
}

const int BufferGeometry::id() const
{
    return mId;
}

AttributeRef BufferGeometry::attribute(const std::string& name)
{
    auto it = attributes.find(name);
    return (it == attributes.end()) ? AttributeRef() : it->second;
}

void BufferGeometry::applyMatrix( const Matrix4& matrix )
{
    MemoryBufferRef positionArray;
    MemoryBufferRef normalArray;

    if ( attributes[ "position" ] ) positionArray = attributes[ "position" ];
    if ( attributes[ "normal" ] ) normalArray = attributes[ "normal" ];

    if ( positionArray ) {

        matrix.multiplyVector3Array( positionArray->dataPtr<Vector3>(), positionArray->dataSize<Vector3>() );
        mVerticesNeedUpdate = true;

    }

    if ( normalArray ) {

        Matrix3 normalMatrix;

        normalMatrix.getNormalMatrix( matrix );
        normalMatrix.multiplyVector3Array( normalArray->dataPtr<Vector3>(), positionArray->dataSize<Vector3>() );

        normalizeNormals();

        mNormalsNeedUpdate = true;

    }
}

void BufferGeometry::computeBoundingBox()
{
    MemoryBufferRef positions = attributes[ "position" ];

    if ( positions ) {

        Box3& bb = mBoundingBox;
        // float x, y, z;

        if ( positions->dataSize<float>() >= 3 ) {
            Vector3& v = *(positions->dataPtr<Vector3>());
            bb.min().x() = bb.max().x() = v.x();
            bb.min().y() = bb.max().y() = v.y();
            bb.min().z() = bb.max().z() = v.z();
        }

        for ( int i = 1, il = positions->dataSize<Vector3>(); i < il; ++i ) {

            // bounding box
            Vector3& v = positions->at<Vector3>(i);
            float x = v.x();
            float y = v.y();
            float z = v.z();

            if ( x < bb.min().x() ) {

                bb.min().x() = x;

            } else if ( x > bb.max().x() ) {

                bb.max().x() = x;

            }

            if ( y < bb.min().y() ) {

                bb.min().y() = y;

            } else if ( y > bb.max().y() ) {

                bb.max().y() = y;

            }

            if ( z < bb.min().z() ) {

                bb.min().z() = z;

            } else if ( z > bb.max().z() ) {

                bb.max().z() = z;

            }

        }

    }

    if ( !positions || (positions && positions->size() == 0) ) {

        mBoundingBox.min().set( 0, 0, 0 );
        mBoundingBox.max().set( 0, 0, 0 );

    }
}

const Sphere& BufferGeometry::boundingSphere()
{
    if (mBoundingSphereNeedsUpdate) {
        computeBoundingSphere();
        mBoundingSphereNeedsUpdate = false;
    }

    return mBoundingSphere;
}

Offset newOffset(int start, int index, int count)
{
    Offset offset;
    offset.start = start;
    offset.index = index;
    offset.count = count;

    return offset;
}

void BufferGeometry::computeBoundingSphere()
{

    Box3 box;
    // Vector3 vector;

    MemoryBufferRef positions = attributes[ "position" ];

    if ( positions ) {

        Vector3 center = mBoundingSphere.center();

        Vector3* positionsPtr = positions->dataPtr<Vector3>();

        for ( int i = 0, il = positions->dataSize<Vector3>(); i < il; ++i ) {
            Vector3 v = positionsPtr[i];
            box.addPoint( v );
        }

        for ( int i = 0, il = positions->dataSize<Vector3>(); i < il; ++i ) {
            Vector3 v = positionsPtr[i];
            box.addPoint( v );
        }

        center = box.center();

        float maxRadiusSq = 0;

        for ( int i = 0, il = positions->dataSize<Vector3>(); i < il; ++i ) {
            Vector3 v = positionsPtr[i];
            maxRadiusSq = Math::max( maxRadiusSq, center.distanceToSquared( v ) );
        }

        mBoundingSphere.radius() = sqrt(maxRadiusSq);

    }

}

void BufferGeometry::computeVertexNormals()
{
//    if ( mAttributes[ "position" ] ) {
//
//        int i, il;
//        int j, jl;
//
//        int nVertexElements = this.attributes[ "position" ].array.length;
//
//        if ( this.attributes[ "normal" ] === undefined ) {
//
//            this.attributes[ "normal" ] = {
//
//                itemSize: 3,
//                array: new Float32Array( nVertexElements )
//
//            };
//
//        } else {
//
//            // reset existing normals to zero
//
//            for ( i = 0, il = this.attributes[ "normal" ].array.length; i < il; i ++ ) {
//
//                this.attributes[ "normal" ].array[ i ] = 0;
//
//            }
//
//        }
//
//        var positions = this.attributes[ "position" ].array;
//        var normals = this.attributes[ "normal" ].array;
//
//        var vA, vB, vC, x, y, z,
//
//        pA = new RESIN.Vector3(),
//        pB = new RESIN.Vector3(),
//        pC = new RESIN.Vector3(),
//
//        cb = new RESIN.Vector3(),
//        ab = new RESIN.Vector3();
//
//        // indexed elements
//
//        if ( this.attributes[ "index" ] ) {
//
//            var indices = this.attributes[ "index" ].array;
//
//            var offsets = this.offsets;
//
//            for ( j = 0, jl = offsets.length; j < jl; ++ j ) {
//
//                var start = offsets[ j ].start;
//                var count = offsets[ j ].count;
//                var index = offsets[ j ].index;
//
//                for ( i = start, il = start + count; i < il; i += 3 ) {
//
//                    vA = index + indices[ i ];
//                    vB = index + indices[ i + 1 ];
//                    vC = index + indices[ i + 2 ];
//
//                    x = positions[ vA * 3 ];
//                    y = positions[ vA * 3 + 1 ];
//                    z = positions[ vA * 3 + 2 ];
//                    pA.set( x, y, z );
//
//                    x = positions[ vB * 3 ];
//                    y = positions[ vB * 3 + 1 ];
//                    z = positions[ vB * 3 + 2 ];
//                    pB.set( x, y, z );
//
//                    x = positions[ vC * 3 ];
//                    y = positions[ vC * 3 + 1 ];
//                    z = positions[ vC * 3 + 2 ];
//                    pC.set( x, y, z );
//
//                    cb.subVectors( pC, pB );
//                    ab.subVectors( pA, pB );
//                    cb.cross( ab );
//
//                    normals[ vA * 3 ]     += cb.x;
//                    normals[ vA * 3 + 1 ] += cb.y;
//                    normals[ vA * 3 + 2 ] += cb.z;
//
//                    normals[ vB * 3 ]     += cb.x;
//                    normals[ vB * 3 + 1 ] += cb.y;
//                    normals[ vB * 3 + 2 ] += cb.z;
//
//                    normals[ vC * 3 ]     += cb.x;
//                    normals[ vC * 3 + 1 ] += cb.y;
//                    normals[ vC * 3 + 2 ] += cb.z;
//
//                }
//
//            }
//
//        // non-indexed elements (unconnected triangle soup)
//
//        } else {
//
//            for ( i = 0, il = positions.length; i < il; i += 9 ) {
//
//                x = positions[ i ];
//                y = positions[ i + 1 ];
//                z = positions[ i + 2 ];
//                pA.set( x, y, z );
//
//                x = positions[ i + 3 ];
//                y = positions[ i + 4 ];
//                z = positions[ i + 5 ];
//                pB.set( x, y, z );
//
//                x = positions[ i + 6 ];
//                y = positions[ i + 7 ];
//                z = positions[ i + 8 ];
//                pC.set( x, y, z );
//
//                cb.subVectors( pC, pB );
//                ab.subVectors( pA, pB );
//                cb.cross( ab );
//
//                normals[ i ] 	 = cb.x;
//                normals[ i + 1 ] = cb.y;
//                normals[ i + 2 ] = cb.z;
//
//                normals[ i + 3 ] = cb.x;
//                normals[ i + 4 ] = cb.y;
//                normals[ i + 5 ] = cb.z;
//
//                normals[ i + 6 ] = cb.x;
//                normals[ i + 7 ] = cb.y;
//                normals[ i + 8 ] = cb.z;
//
//            }
//
//        }
//
//        this.normalizeNormals();
//
//        this.normalsNeedUpdate = true;
//
//    }
//
}

void BufferGeometry::normalizeNormals()
{
    AttributeRef normals = attributes[ "normal" ];
    if (!normals) {
        return;
    }

    Vector3* vv = normals->dataPtr<Vector3>();
    for (int i=0, il = normals->dataSize<Vector3>(); i < il; ++i, ++vv) {
        vv->normalize();
    }
}

void BufferGeometry::computeTangents()
{
//    // based on http://www.terathon.com/code/tangent.html
//    // (per vertex tangents)
//
//    if ( this.attributes[ "index" ] === undefined ||
//         this.attributes[ "position" ] === undefined ||
//         this.attributes[ "normal" ] === undefined ||
//         this.attributes[ "uv" ] === undefined ) {
//
//        console.warn( "Missing required attributes (index, position, normal or uv) in BufferGeometry.computeTangents()" );
//        return;
//
//    }
//
//    var indices = this.attributes[ "index" ].array;
//    var positions = this.attributes[ "position" ].array;
//    var normals = this.attributes[ "normal" ].array;
//    var uvs = this.attributes[ "uv" ].array;
//
//    var nVertices = positions.length / 3;
//
//    if ( this.attributes[ "tangent" ] === undefined ) {
//
//        var nTangentElements = 4 * nVertices;
//
//        this.attributes[ "tangent" ] = {
//
//            itemSize: 4,
//            array: new Float32Array( nTangentElements )
//
//        };
//
//    }
//
//    var tangents = this.attributes[ "tangent" ].array;
//
//    var tan1 = [], tan2 = [];
//
//    for ( var k = 0; k < nVertices; k ++ ) {
//
//        tan1[ k ] = new RESIN.Vector3();
//        tan2[ k ] = new RESIN.Vector3();
//
//    }
//
//    var xA, yA, zA,
//        xB, yB, zB,
//        xC, yC, zC,
//
//        uA, vA,
//        uB, vB,
//        uC, vC,
//
//        x1, x2, y1, y2, z1, z2,
//        s1, s2, t1, t2, r;
//
//    var sdir = new RESIN.Vector3(), tdir = new RESIN.Vector3();
//
//    function handleTriangle( a, b, c ) {
//
//        xA = positions[ a * 3 ];
//        yA = positions[ a * 3 + 1 ];
//        zA = positions[ a * 3 + 2 ];
//
//        xB = positions[ b * 3 ];
//        yB = positions[ b * 3 + 1 ];
//        zB = positions[ b * 3 + 2 ];
//
//        xC = positions[ c * 3 ];
//        yC = positions[ c * 3 + 1 ];
//        zC = positions[ c * 3 + 2 ];
//
//        uA = uvs[ a * 2 ];
//        vA = uvs[ a * 2 + 1 ];
//
//        uB = uvs[ b * 2 ];
//        vB = uvs[ b * 2 + 1 ];
//
//        uC = uvs[ c * 2 ];
//        vC = uvs[ c * 2 + 1 ];
//
//        x1 = xB - xA;
//        x2 = xC - xA;
//
//        y1 = yB - yA;
//        y2 = yC - yA;
//
//        z1 = zB - zA;
//        z2 = zC - zA;
//
//        s1 = uB - uA;
//        s2 = uC - uA;
//
//        t1 = vB - vA;
//        t2 = vC - vA;
//
//        r = 1.0 / ( s1 * t2 - s2 * t1 );
//
//        sdir.set(
//            ( t2 * x1 - t1 * x2 ) * r,
//            ( t2 * y1 - t1 * y2 ) * r,
//            ( t2 * z1 - t1 * z2 ) * r
//        );
//
//        tdir.set(
//            ( s1 * x2 - s2 * x1 ) * r,
//            ( s1 * y2 - s2 * y1 ) * r,
//            ( s1 * z2 - s2 * z1 ) * r
//        );
//
//        tan1[ a ].add( sdir );
//        tan1[ b ].add( sdir );
//        tan1[ c ].add( sdir );
//
//        tan2[ a ].add( tdir );
//        tan2[ b ].add( tdir );
//        tan2[ c ].add( tdir );
//
//    }
//
//    var i, il;
//    var j, jl;
//    var iA, iB, iC;
//
//    var offsets = this.offsets;
//
//    for ( j = 0, jl = offsets.length; j < jl; ++ j ) {
//
//        var start = offsets[ j ].start;
//        var count = offsets[ j ].count;
//        var index = offsets[ j ].index;
//
//        for ( i = start, il = start + count; i < il; i += 3 ) {
//
//            iA = index + indices[ i ];
//            iB = index + indices[ i + 1 ];
//            iC = index + indices[ i + 2 ];
//
//            handleTriangle( iA, iB, iC );
//
//        }
//
//    }
//
//    var tmp = new RESIN.Vector3(), tmp2 = new RESIN.Vector3();
//    var n = new RESIN.Vector3(), n2 = new RESIN.Vector3();
//    var w, t, test;
//
//    function handleVertex( v ) {
//
//        n.x = normals[ v * 3 ];
//        n.y = normals[ v * 3 + 1 ];
//        n.z = normals[ v * 3 + 2 ];
//
//        n2.copy( n );
//
//        t = tan1[ v ];
//
//        // Gram-Schmidt orthogonalize
//
//        tmp.copy( t );
//        tmp.sub( n.multiplyScalar( n.dot( t ) ) ).normalize();
//
//        // Calculate handedness
//
//        tmp2.crossVectors( n2, t );
//        test = tmp2.dot( tan2[ v ] );
//        w = ( test < 0.0 ) ? -1.0 : 1.0;
//
//        tangents[ v * 4 ]     = tmp.x;
//        tangents[ v * 4 + 1 ] = tmp.y;
//        tangents[ v * 4 + 2 ] = tmp.z;
//        tangents[ v * 4 + 3 ] = w;
//
//    }
//
//    for ( j = 0, jl = offsets.length; j < jl; ++ j ) {
//
//        var start = offsets[ j ].start;
//        var count = offsets[ j ].count;
//        var index = offsets[ j ].index;
//
//        for ( i = start, il = start + count; i < il; i += 3 ) {
//
//            iA = index + indices[ i ];
//            iB = index + indices[ i + 1 ];
//            iC = index + indices[ i + 2 ];
//
//            handleVertex( iA );
//            handleVertex( iB );
//            handleVertex( iC );
//
//        }
//
//    }
//
//    this.hasTangents = true;
//    this.tangentsNeedUpdate = true;
//
}

}

