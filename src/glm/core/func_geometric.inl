///////////////////////////////////////////////////////////////////////////////////
/// OpenGL Mathematics (glm.g-truc.net)
///
/// Copyright (c) 2005 - 2013 G-Truc Creation (www.g-truc.net)
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
/// 
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
/// 
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
/// THE SOFTWARE.
///
/// @ref core
/// @file glm/core/func_geometric.inl
/// @date 2008-08-03 / 2011-06-15
/// @author Christophe Riccio
///////////////////////////////////////////////////////////////////////////////////

namespace glm
{
	// length
	template <typename genType>
	GLM_FUNC_QUALIFIER genType length
	(
		genType const & x
	)
	{
		GLM_STATIC_ASSERT(detail::type<genType>::is_float, "'length' only accept floating-point inputs");

		genType sqr = x * x;
		return sqrt(sqr);
	}

	template <typename T, precision P>
	GLM_FUNC_QUALIFIER T length(detail::tvec2<T, P> const & v)
	{
		GLM_STATIC_ASSERT(detail::type<T>::is_float, "'length' only accept floating-point inputs");

		T sqr = v.x * v.x + v.y * v.y;
		return sqrt(sqr);
	}

	template <typename T, precision P>
	GLM_FUNC_QUALIFIER T length(detail::tvec3<T, P> const & v)
	{
		GLM_STATIC_ASSERT(detail::type<T>::is_float, "'length' only accept floating-point inputs");

		T sqr = v.x * v.x + v.y * v.y + v.z * v.z;
		return sqrt(sqr);
	}

	template <typename T, precision P>
	GLM_FUNC_QUALIFIER T length(detail::tvec4<T, P> const & v)
	{
		GLM_STATIC_ASSERT(detail::type<T>::is_float, "'length' only accept floating-point inputs");

		T sqr = v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w;
		return sqrt(sqr);
	}

	// distance
	template <typename genType>
	GLM_FUNC_QUALIFIER genType distance
	(
		genType const & p0,
		genType const & p1
	)
	{
		GLM_STATIC_ASSERT(detail::type<genType>::is_float, "'distance' only accept floating-point inputs");

		return length(p1 - p0);
	}

	template <typename T, precision P>
	GLM_FUNC_QUALIFIER T distance
	(
		detail::tvec2<T, P> const & p0,
		detail::tvec2<T, P> const & p1
	)
	{
		GLM_STATIC_ASSERT(detail::type<T>::is_float, "'distance' only accept floating-point inputs");

		return length(p1 - p0);
	}

	template <typename T, precision P>
	GLM_FUNC_QUALIFIER T distance
	(
		detail::tvec3<T, P> const & p0,
		detail::tvec3<T, P> const & p1
	)
	{
		GLM_STATIC_ASSERT(detail::type<T>::is_float, "'distance' only accept floating-point inputs");

		return length(p1 - p0);
	}

	template <typename T, precision P>
	GLM_FUNC_QUALIFIER T distance
	(
		detail::tvec4<T, P> const & p0,
		detail::tvec4<T, P> const & p1
	)
	{
		GLM_STATIC_ASSERT(detail::type<T>::is_float, "'distance' only accept floating-point inputs");

		return length(p1 - p0);
	}

	// dot
	template <typename genType>
	GLM_FUNC_QUALIFIER genType dot
	(
		genType const & x,
		genType const & y
	)
	{
		GLM_STATIC_ASSERT(detail::type<genType>::is_float, "'dot' only accept floating-point inputs");

		return x * y;
	}

	template <typename T, precision P>
	GLM_FUNC_QUALIFIER T dot
	(
		detail::tvec2<T, P> const & x,
		detail::tvec2<T, P> const & y
	)
	{
		GLM_STATIC_ASSERT(detail::type<T>::is_float, "'dot' only accept floating-point inputs");

		return x.x * y.x + x.y * y.y;
	}

	template <typename T, precision P>
	GLM_FUNC_QUALIFIER T dot
	(
		detail::tvec3<T, P> const & x,
		detail::tvec3<T, P> const & y
	)
	{
		GLM_STATIC_ASSERT(detail::type<T>::is_float, "'dot' only accept floating-point inputs");

		return x.x * y.x + x.y * y.y + x.z * y.z;
	}
/* // SSE3
	GLM_FUNC_QUALIFIER float dot(const tvec4<float>& x, const tvec4<float>& y)
	{
		float Result;
		__asm
		{
			mov		esi, x
			mov		edi, y
			movaps	xmm0, [esi]
			mulps	xmm0, [edi]
			haddps(	_xmm0, _xmm0 )
			haddps(	_xmm0, _xmm0 )
			movss	Result, xmm0
		}
		return Result;
	}
*/
	template <typename T, precision P>
	GLM_FUNC_QUALIFIER T dot
	(
		detail::tvec4<T, P> const & x,
		detail::tvec4<T, P> const & y
	)
	{
		GLM_STATIC_ASSERT(detail::type<T>::is_float, "'dot' only accept floating-point inputs");

		return x.x * y.x + x.y * y.y + x.z * y.z + x.w * y.w;
	}

	// cross
	template <typename T, precision P>
	GLM_FUNC_QUALIFIER detail::tvec3<T, P> cross
	(
		detail::tvec3<T, P> const & x,
		detail::tvec3<T, P> const & y
	)
	{
		GLM_STATIC_ASSERT(detail::type<T>::is_float, "'cross' only accept floating-point inputs");

		return detail::tvec3<T, P>(
			x.y * y.z - y.y * x.z,
			x.z * y.x - y.z * x.x,
			x.x * y.y - y.x * x.y);
	}

	// normalize
	template <typename genType>
	GLM_FUNC_QUALIFIER genType normalize
	(
		genType const & x
	)
	{
		GLM_STATIC_ASSERT(detail::type<genType>::is_float, "'normalize' only accept floating-point inputs");

		return x < genType(0) ? genType(-1) : genType(1);
	}

	// According to issue 10 GLSL 1.10 specification, if length(x) == 0 then result is undefine and generate an error
	template <typename T, precision P>
	GLM_FUNC_QUALIFIER detail::tvec2<T, P> normalize
	(
		detail::tvec2<T, P> const & x
	)
	{
		GLM_STATIC_ASSERT(detail::type<T>::is_float, "'normalize' only accept floating-point inputs");
		
		T sqr = x.x * x.x + x.y * x.y;
		return x * inversesqrt(sqr);
	}

	template <typename T, precision P>
	GLM_FUNC_QUALIFIER detail::tvec3<T, P> normalize
	(
		detail::tvec3<T, P> const & x
	)
	{
		GLM_STATIC_ASSERT(detail::type<T>::is_float, "'normalize' only accept floating-point inputs");

		T sqr = x.x * x.x + x.y * x.y + x.z * x.z;
		return x * inversesqrt(sqr);
	}

	template <typename T, precision P>
	GLM_FUNC_QUALIFIER detail::tvec4<T, P> normalize
	(
		detail::tvec4<T, P> const & x
	)
	{
		GLM_STATIC_ASSERT(detail::type<T>::is_float, "'normalize' only accept floating-point inputs");
		
		T sqr = x.x * x.x + x.y * x.y + x.z * x.z + x.w * x.w;
		return x * inversesqrt(sqr);
	}

	// faceforward
	template <typename genType>
	GLM_FUNC_QUALIFIER genType faceforward
	(
		genType const & N,
		genType const & I,
		genType const & Nref
	)
	{
		return dot(Nref, I) < 0 ? N : -N;
	}

	// reflect
	template <typename genType>
	GLM_FUNC_QUALIFIER genType reflect
	(
		genType const & I,
		genType const & N
	)
	{
		return I - N * dot(N, I) * genType(2);
	}

	// refract
	template <typename genType>
	GLM_FUNC_QUALIFIER genType refract
	(
		genType const & I,
		genType const & N,
		genType const & eta
	)
	{
		GLM_STATIC_ASSERT(detail::type<genType>::is_float, "'refract' only accept floating-point inputs");

		genType dotValue = dot(N, I);
		genType k = genType(1) - eta * eta * (genType(1) - dotValue * dotValue);
		if(k < genType(0))
			return genType(0);
		else
			return eta * I - (eta * dotValue + sqrt(k)) * N;
	}

	template <typename T, precision P, template <typename, precision> class vecType>
	GLM_FUNC_QUALIFIER vecType<T, P> refract
	(
		vecType<T, P> const & I,
		vecType<T, P> const & N,
		T const & eta
	)
	{
		GLM_STATIC_ASSERT(detail::type<T>::is_float, "'refract' only accept floating-point inputs");

		T dotValue = dot(N, I);
		T k = T(1) - eta * eta * (T(1) - dotValue * dotValue);
		if(k < T(0))
			return vecType<T, P>(0);
		else
			return eta * I - (eta * dotValue + sqrt(k)) * N;
	}

}//namespace glm
