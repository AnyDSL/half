// half - IEEE 754-based half-precision floating point library.
//
// Copyright (c) 2012-2013 Christian Rau <rauy@users.sourceforge.net>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation 
// files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, 
// modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the 
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE 
// WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

// Version 1.7.1

/// \file
/// Main header file for half precision functionality.

#ifndef HALF_HALF_H
#define HALF_HALF_H

//check C++11 language features
#define HALF_GNUC_VERSION (__GNUC__*100+__GNUC_MINOR__)
#if defined(__clang__)										//clang
	#if __has_feature(cxx_static_assert) && !defined(HALF_ENABLE_CPP11_STATIC_ASSERT)
		#define HALF_ENABLE_CPP11_STATIC_ASSERT 1
	#endif
	#if __has_feature(cxx_constexpr) && !defined(HALF_ENABLE_CPP11_CONSTEXPR)
		#define HALF_ENABLE_CPP11_CONSTEXPR 1
	#endif
	#if __has_feature(cxx_noexcept) && !defined(HALF_ENABLE_CPP11_NOEXCEPT)
		#define HALF_ENABLE_CPP11_NOEXCEPT 1
	#endif
	#if __has_feature(cxx_user_literals) && !defined(HALF_ENABLE_CPP11_USER_LITERALS)
		#define HALF_ENABLE_CPP11_USER_LITERALS 1
	#endif
	#if !defined(HALF_ENABLE_CPP11_LONG_LONG)
		#define HALF_ENABLE_CPP11_LONG_LONG 1
	#endif
/*#elif defined(__INTEL_COMPILER)								//Intel C++
	#if __INTEL_COMPILER >= 1100 && !defined(HALF_ENABLE_CPP11_STATIC_ASSERT)
		#define HALF_ENABLE_CPP11_STATIC_ASSERT 1
	#endif
	#if __INTEL_COMPILER >= 1300 && !defined(HALF_ENABLE_CPP11_CONSTEXPR)
		#define HALF_ENABLE_CPP11_CONSTEXPR 1
	#endif
	#if !defined(HALF_ENABLE_CPP11_LONG_LONG)
		#define HALF_ENABLE_CPP11_LONG_LONG 1
	#endif*/
#elif defined(__GNUC__)										//gcc
	#if defined(__GXX_EXPERIMENTAL_CXX0X__) || __cplusplus >= 201103L
		#if HALF_GNUC_VERSION >= 403 && !defined(HALF_ENABLE_CPP11_STATIC_ASSERT)
			#define HALF_ENABLE_CPP11_STATIC_ASSERT 1
		#endif
		#if HALF_GNUC_VERSION >= 406 && !defined(HALF_ENABLE_CPP11_CONSTEXPR)
			#define HALF_ENABLE_CPP11_CONSTEXPR 1
		#endif
		#if HALF_GNUC_VERSION >= 406 && !defined(HALF_ENABLE_CPP11_NOEXCEPT)
			#define HALF_ENABLE_CPP11_NOEXCEPT 1
		#endif
		#if HALF_GNUC_VERSION >= 407 && !defined(HALF_ENABLE_CPP11_USER_LITERALS)
			#define HALF_ENABLE_CPP11_USER_LITERALS 1
		#endif
		#if !defined(HALF_ENABLE_CPP11_LONG_LONG)
			#define HALF_ENABLE_CPP11_LONG_LONG 1
		#endif
	#endif
#elif defined(_MSC_VER)										//Visual C++
	#if _MSC_VER >= 1600 && !defined(HALF_ENABLE_CPP11_STATIC_ASSERT)
		#define HALF_ENABLE_CPP11_STATIC_ASSERT 1
	#endif
	#if _MSC_VER >= 1310 && !defined(HALF_ENABLE_CPP11_LONG_LONG)
		#define HALF_ENABLE_CPP11_LONG_LONG 1
	#endif
#endif

//check C++11 library features
#include <utility>
#if defined(_LIBCPP_VERSION)								//libc++
	#if defined(__GXX_EXPERIMENTAL_CXX0X__) || __cplusplus >= 201103
		#ifndef HALF_ENABLE_CPP11_CSTDINT
			#define HALF_ENABLE_CPP11_CSTDINT 1
		#endif
		#ifndef HALF_ENABLE_CPP11_CMATH
			#define HALF_ENABLE_CPP11_CMATH 1
		#endif
		#ifndef HALF_ENABLE_CPP11_HASH
			#define HALF_ENABLE_CPP11_HASH 1
		#endif
	#endif
#elif defined(__GLIBCXX__)									//libstdc++
	#if defined(__GXX_EXPERIMENTAL_CXX0X__) || __cplusplus >= 201103
		#ifdef __clang__
			#if __GLIBCXX__ >= 20080606 && !defined(HALF_ENABLE_CPP11_CSTDINT)
				#define HALF_ENABLE_CPP11_CSTDINT 1
			#endif
			#if __GLIBCXX__ >= 20080606 && !defined(HALF_ENABLE_CPP11_CMATH)
				#define HALF_ENABLE_CPP11_CMATH 1
			#endif
			#if __GLIBCXX__ >= 20080606 && !defined(HALF_ENABLE_CPP11_HASH)
				#define HALF_ENABLE_CPP11_HASH 1
			#endif
		#else
			#if HALF_GNUC_VERSION >= 403 && !defined(HALF_ENABLE_CPP11_CSTDINT)
				#define HALF_ENABLE_CPP11_CSTDINT 1
			#endif
			#if HALF_GNUC_VERSION >= 403 && !defined(HALF_ENABLE_CPP11_CMATH)
				#define HALF_ENABLE_CPP11_CMATH 1
			#endif
			#if HALF_GNUC_VERSION >= 403 && !defined(HALF_ENABLE_CPP11_HASH)
				#define HALF_ENABLE_CPP11_HASH 1
			#endif
		#endif
	#endif
#elif defined(_CPPLIB_VER)									//Dinkumware/Visual C++
	#if _CPPLIB_VER >= 520
		#ifndef HALF_ENABLE_CPP11_CSTDINT
			#define HALF_ENABLE_CPP11_CSTDINT 1
		#endif
		#ifndef HALF_ENABLE_CPP11_HASH
			#define HALF_ENABLE_CPP11_HASH 1
		#endif
	#endif
#endif
#undef HALF_GNUC_VERSION

//support constexpr
#if HALF_ENABLE_CPP11_CONSTEXPR
	#define HALF_CONSTEXPR			constexpr
	#define HALF_CONSTEXPR_CONST	constexpr
#else
	#define HALF_CONSTEXPR
	#define HALF_CONSTEXPR_CONST	const
#endif

//support noexcept
#if HALF_ENABLE_CPP11_NOEXCEPT
	#define HALF_NOEXCEPT	noexcept
	#define HALF_NOTHROW	noexcept
#else
	#define HALF_NOEXCEPT
	#define HALF_NOTHROW	throw()
#endif

#include <algorithm>
#include <iostream>
#include <limits>
#include <climits>
#include <cfloat>
#include <cmath>
#include <cstring>
#if HALF_ENABLE_CPP11_HASH
	#include <functional>
#endif
#if HALF_ENABLE_CPP11_CSTDINT
	#include <cstdint>
#endif


/// Value signaling overflow.
/// In correspondence with `HUGE_VAL[F|L]` from `<cmath>` this symbol expands to a positive value signaling the overflow of an 
/// operation, in particular it just evaluates to positive infinity.
#define HUGE_VALH	std::numeric_limits<half_float::half>::infinity()

/// Fast half-precision fma function.
/// This symbol is only defined if the fma() function generally executes as fast as, or faster than, a separate 
/// half-precision multiplication followed by an addition. Due to the internal single-precision implementation of all 
/// arithmetic operations, this is usually always the case.
#define FP_FAST_FMAH	1

#ifndef FP_ILOGB0
	#define FP_ILOGB0		INT_MIN
#endif
#ifndef FP_ILOGBNAN
	#define FP_ILOGBNAN		INT_MAX
#endif
#ifndef FP_SUBNORMAL
	#define FP_SUBNORMAL	0
#endif
#ifndef FP_ZERO
	#define FP_ZERO			1
#endif
#ifndef FP_NAN
	#define FP_NAN			2
#endif
#ifndef FP_INFINITE
	#define FP_INFINITE		3
#endif
#ifndef FP_NORMAL
	#define FP_NORMAL		4
#endif


/// Main namespace for half precision functionality.
/// This namespace contains all the functionality provided by the library.
namespace half_float
{
	class half;

#if HALF_ENABLE_CPP11_USER_LITERALS
	/// Library-defined half-precision literals.
	/// Import this namespace to enable half-precision floating point literals:
	/// ~~~~{.cpp}
	/// using namespace half_float::literal;
	/// half_float::half = 4.2_h;
	/// ~~~~
	namespace literal
	{
		half operator "" _h(long double d);
	}
#endif

	/// \internal
	/// \brief Implementation details.
	namespace detail
	{
	#if HALF_ENABLE_CPP11_CSTDINT
		/// Unsigned integer of (at least) 16 bits width.
		typedef std::uint_least16_t uint16;

		/// Unsigned integer of (at least) 32 bits width.
		typedef std::uint_least32_t uint32;

		/// Fastest signed integer capable of holding all values of type uint16.
		typedef std::int_fast32_t int17;
	#else
		/// Unsigned integer of (at least) 16 bits width.
		typedef unsigned short uint16;

		/// Conditional type.
		template<bool B,typename T,typename F> struct conditional { typedef T type; };

		/// Conditional type.
		template<typename T,typename F> struct conditional<false,T,F> { typedef F type; };

		/// Unsigned integer of (at least) 32 bits width.
		typedef conditional<std::numeric_limits<unsigned int>::digits>=32,unsigned int,unsigned long>::type uint32;

		/// Fastest signed integer capable of holding all values of type uint16.
		typedef conditional<std::numeric_limits<int>::digits>=16,int,long>::type int17;
	#endif
/*
		/// Helper for tag dispatching.
		template<bool> struct booltype {};
*/
		/// Generic half expression.
		/// This class represents the base class for expressions of half-precision type, convertible to single precision.
		/// \tparam E concrete expression type
		template<typename E> struct half_expr
		{
			/// Conversion to single-precision.
			/// \return single precision value representing expression value
			operator float() const { return static_cast<float>(*static_cast<const E*>(this)); }
		};

		/// Temporary half expression with internal float.
		/// This class represents a half-precision expression which just stores a single-precision value internally.
		struct float_expr : public half_expr<float_expr>
		{
			/// Conversion constructor.
			/// \param f single-precision value to convert
			explicit float_expr(float f) : value(f) {}

			/// Conversion to single-precision.
			/// \return single precision value representing expression value
			operator float() const { return value; }

			/// Internal expression value stored in single-precision.
			float value;
		};

		struct noarg_functions
		{
			template<typename charT,typename traits> static std::basic_istream<charT,traits>& 
				read(std::basic_istream<charT,traits> &in, half &h);
			static half nanh(const char *arg);
		};

		template<typename T> struct unary_functions
		{
			static half negate(half arg);
			template<typename charT,typename traits> static std::basic_ostream<charT,traits>& 
				write(std::basic_ostream<charT,traits> &out, float arg);
			static half fabs(half arg);
			static float_expr exp(float arg);
			static float_expr log(float arg);
			static float_expr log10(float arg);
			static float_expr sqrt(float arg);
			static float_expr sin(float arg);
			static float_expr cos(float arg);
			static float_expr tan(float arg);
			static float_expr asin(float arg);
			static float_expr acos(float arg);
			static float_expr atan(float arg);
			static float_expr sinh(float arg);
			static float_expr cosh(float arg);
			static float_expr tanh(float arg);
			static half floor(half arg);
			static half ceil(half arg);
			static half trunc(half arg);
			static half round(half arg);
			static long lround(half arg);
			static long long llround(half arg);
			static half frexp(half arg, int *exp);
			static half modf(half x, half *iptr);
			static half scalbln(half x, long exp);
			static int ilogb(half arg);
			static half logb(half arg);
			static half nexttoward(half from, long double to);
			static int fpclassify(half arg);
			static bool isfinite(half arg);
			static bool isinf(half arg);
			static bool isnan(half arg);
			static bool isnormal(half arg);
			static bool signbit(half arg);
		#if HALF_ENABLE_CPP11_CMATH
			static float_expr exp2(float arg);
			static float_expr expm1(float arg);
			static float_expr log1p(float arg);
			static float_expr log2(float arg);
			static float_expr cbrt(float arg);
			static float_expr asinh(float arg);
			static float_expr acosh(float arg);
			static float_expr atanh(float arg);
			static float_expr erf(float arg);
			static float_expr erfc(float arg);
			static float_expr lgamma(float arg);
			static float_expr tgamma(float arg);
			static float_expr nearbyint(float arg);
			static float_expr rint(float arg);
			static long lrint(float arg);
			static long long llrint(float arg);
		#endif
		};

		template<typename T,typename U> struct binary_functions
		{
			static float_expr plus(float x, float y);
			static float_expr minus(float x, float y);
			static float_expr multiplies(float x, float y);
			static float_expr divides(float x, float y);
			static half fmin(half x, half y);
			static half fmax(half x, half y);
			static float_expr fmod(float x, float y);
			static float_expr fdim(float x, float y);
			static float_expr pow(float base, float exp);
			static float_expr atan2(float x, float y);
			static half nextafter(half from, half to);
			static half copysign(half x, half y);
			static bool isequal(half x, half y);
			static bool isnotequal(half x, half y);
			static bool isgreater(half x, half y);
			static bool isgreaterequal(half x, half y);
			static bool isless(half x, half y);
			static bool islessequal(half x, half y);
			static bool islessgreater(half x, half y);
			static bool isunordered(half x, half y);
		#if HALF_ENABLE_CPP11_CMATH
			static float_expr remainder(float x, float y);
			static float_expr remquo(float x, float y, int *quo);
			static float_expr hypot(float x, float y);
		#endif
		};

		template<typename T,typename U,typename V> struct ternary_functions
		{
			static float_expr fma(float x, float y, float z);
		};

		template<typename T=void,typename U=void,typename V=void> struct functions : noarg_functions, 
																					 unary_functions<T>, 
																					 binary_functions<T,U>, 
																					 ternary_functions<T,U,V> {};

		namespace function
		{
			enum type
			{
				identity, negate, plus, minus, multiplies, divides, write, read,
				fabs, fmod, remainder, remquo, fma, fmin, fmax, fdim,
				exp, log, log10, exp2, expm1, log1p, log2,
				sqrt, cbrt, hypot, pow,
				sin, cos, tan, asin, acos, atan, atan2,
				sinh, cosh, tanh, asinh, acosh, atanh,
				erf, erfc, lgamma, tgamma,
				floor, ceil, trunc, round, lround, llround, nearbyint, rint, lrint, llrint,
				frexp, modf, scalbln, ilogb, logb, nextafter, nexttoward, copysign,
				fpclassify, isfinite, isinf, isnan, isnormal, signbit,
				isequal, isnotequal, isgreater, isgreaterequal, isless, islessequal, islessgreater, isunordered
			};
		}

		template<typename,typename> struct unary_enable {};
		template<typename T> struct unary_enable<half,T> { typedef T type; };
		template<typename T> struct unary_enable<float_expr,T> { typedef T type; };

		template<function::type,typename> struct unary_result {};
		template<typename T> struct unary_result<function::negate,T> : unary_enable<T,half> {};
		template<typename T> struct unary_result<function::fabs,T> : unary_enable<T,half> {};
		template<typename T> struct unary_result<function::exp,T> : unary_enable<T,float_expr> {};
		template<typename T> struct unary_result<function::log,T> : unary_enable<T,float_expr> {};
		template<typename T> struct unary_result<function::log10,T> : unary_enable<T,float_expr> {};
		template<typename T> struct unary_result<function::sqrt,T> : unary_enable<T,float_expr> {};
		template<typename T> struct unary_result<function::sin,T> : unary_enable<T,float_expr> {};
		template<typename T> struct unary_result<function::cos,T> : unary_enable<T,float_expr> {};
		template<typename T> struct unary_result<function::tan,T> : unary_enable<T,float_expr> {};
		template<typename T> struct unary_result<function::asin,T> : unary_enable<T,float_expr> {};
		template<typename T> struct unary_result<function::acos,T> : unary_enable<T,float_expr> {};
		template<typename T> struct unary_result<function::atan,T> : unary_enable<T,float_expr> {};
		template<typename T> struct unary_result<function::sinh,T> : unary_enable<T,float_expr> {};
		template<typename T> struct unary_result<function::cosh,T> : unary_enable<T,float_expr> {};
		template<typename T> struct unary_result<function::tanh,T> : unary_enable<T,float_expr> {};
		template<typename T> struct unary_result<function::floor,T> : unary_enable<T,half> {};
		template<typename T> struct unary_result<function::ceil,T> : unary_enable<T,half> {};
		template<typename T> struct unary_result<function::trunc,T> : unary_enable<T,half> {};
		template<typename T> struct unary_result<function::round,T> : unary_enable<T,half> {};
		template<typename T> struct unary_result<function::frexp,T> : unary_enable<T,half> {};
		template<typename T> struct unary_result<function::modf,T> : unary_enable<T,half> {};
		template<typename T> struct unary_result<function::scalbln,T> : unary_enable<T,half> {};
		template<typename T> struct unary_result<function::logb,T> : unary_enable<T,half> {};
		template<typename T> struct unary_result<function::nexttoward,T> : unary_enable<T,half> {};
	#if HALF_ENABLE_CPP11_CMATH
		template<typename T> struct unary_result<function::exp2,T> : unary_enable<T,float_expr> {};
		template<typename T> struct unary_result<function::expm1,T> : unary_enable<T,float_expr> {};
		template<typename T> struct unary_result<function::log1p,T> : unary_enable<T,float_expr> {};
		template<typename T> struct unary_result<function::log2,T> : unary_enable<T,float_expr> {};
		template<typename T> struct unary_result<function::cbrt,T> : unary_enable<T,float_expr> {};
		template<typename T> struct unary_result<function::asinh,T> : unary_enable<T,float_expr> {};
		template<typename T> struct unary_result<function::acosh,T> : unary_enable<T,float_expr> {};
		template<typename T> struct unary_result<function::atanh,T> : unary_enable<T,float_expr> {};
		template<typename T> struct unary_result<function::erf,T> : unary_enable<T,float_expr> {};
		template<typename T> struct unary_result<function::erfc,T> : unary_enable<T,float_expr> {};
		template<typename T> struct unary_result<function::lgamma,T> : unary_enable<T,float_expr> {};
		template<typename T> struct unary_result<function::tgamma,T> : unary_enable<T,float_expr> {};
		template<typename T> struct unary_result<function::nearbyint,T> : unary_enable<T,float_expr> {};
		template<typename T> struct unary_result<function::rint,T> : unary_enable<T,float_expr> {};
	#endif

		template<typename,typename,typename> struct binary_enable {};
		template<typename T> struct binary_enable<half,half,T> { typedef T type; };
		template<typename T> struct binary_enable<half,float_expr,T> { typedef T type; };
		template<typename T> struct binary_enable<float_expr,half,T> { typedef T type; };
		template<typename T> struct binary_enable<float_expr,float_expr,T> { typedef T type; };

		template<function::type,typename,typename> struct binary_result {};
		template<typename T,typename U> struct binary_result<function::plus,T,U> : binary_enable<T,U,float_expr> {};
		template<typename T,typename U> struct binary_result<function::minus,T,U> : binary_enable<T,U,float_expr> {};
		template<typename T,typename U> struct binary_result<function::multiplies,T,U> : binary_enable<T,U,float_expr> {};
		template<typename T,typename U> struct binary_result<function::divides,T,U> : binary_enable<T,U,float_expr> {};
		template<typename T,typename U> struct binary_result<function::fmin,T,U> : binary_enable<T,U,half> {};
		template<typename T,typename U> struct binary_result<function::fmax,T,U> : binary_enable<T,U,half> {};
		template<typename T,typename U> struct binary_result<function::fmod,T,U> : binary_enable<T,U,float_expr> {};
		template<typename T,typename U> struct binary_result<function::fdim,T,U> : binary_enable<T,U,float_expr> {};
		template<typename T,typename U> struct binary_result<function::pow,T,U> : binary_enable<T,U,float_expr> {};
		template<typename T,typename U> struct binary_result<function::atan2,T,U> : binary_enable<T,U,float_expr> {};
		template<typename T,typename U> struct binary_result<function::nextafter,T,U> : binary_enable<T,U,half> {};
		template<typename T,typename U> struct binary_result<function::copysign,T,U> : binary_enable<T,U,half> {};
	#if HALF_ENABLE_CPP11_CMATH
		template<typename T,typename U> struct binary_result<function::remainder,T,U> : binary_enable<T,U,float_expr> {};
		template<typename T,typename U> struct binary_result<function::remquo,T,U> : binary_enable<T,U,float_expr> {};
		template<typename T,typename U> struct binary_result<function::hypot,T,U> : binary_enable<T,U,float_expr> {};
	#endif

		template<typename,typename,typename,typename> struct ternary_enable {};
		template<typename T> struct ternary_enable<half,half,half,T> { typedef T type; };
		template<typename T> struct ternary_enable<half,half,float_expr,T> { typedef T type; };
		template<typename T> struct ternary_enable<half,float_expr,half,T> { typedef T type; };
		template<typename T> struct ternary_enable<half,float_expr,float_expr,T> { typedef T type; };
		template<typename T> struct ternary_enable<float_expr,half,half,T> { typedef T type; };
		template<typename T> struct ternary_enable<float_expr,half,float_expr,T> { typedef T type; };
		template<typename T> struct ternary_enable<float_expr,float_expr,half,T> { typedef T type; };
		template<typename T> struct ternary_enable<float_expr,float_expr,float_expr,T> { typedef T type; };

		template<function::type,typename,typename,typename> struct ternary_result {};
		template<typename T,typename U,typename V> struct ternary_result<function::fma,T,U,V> : ternary_enable<T,U,V,float_expr> {};

		/// Helper class for half casts.
		/// \tparam T destination type
		/// \tparam U source type
		/// \tparam R rounding mode to use
		template<typename T,typename U,std::float_round_style R> struct half_caster;

		/// Helper class for half casts specialized for casting to half.
		/// \tparam U source type
		/// \tparam R rounding mode to use
		template<typename U,std::float_round_style R> struct half_caster<half,U,R>
		{
			static half cast(const U &arg);
		};

		/// Helper class for half casts specialized for casting from half.
		/// \tparam T destination type
		/// \tparam R rounding mode to use
		template<typename T,std::float_round_style R> struct half_caster<T,half,R>
		{
			template<typename E> static T cast(const half_expr<E> &arg);
		};

		/// Helper class for half casts specialized for casting from half-precision expressions.
		/// \tparam T destination type
		/// \tparam R rounding mode to use
		template<typename T,std::float_round_style R> struct half_caster<T,float_expr,R> : public half_caster<T,half,R> {};

		/// Helper class for half casts specialized for casting between halfs.
		/// \tparam R rounding mode to use
		template<std::float_round_style R> struct half_caster<half,half,R>
		{
			template<typename E> static half cast(const half_expr<E> &arg);
		};

		/// Helper class for half casts specialized for casting half-precision expressions to halfs.
		/// \tparam R rounding mode to use
		template<std::float_round_style R> struct half_caster<half,float_expr,R> : public half_caster<half,half,R> {};

		/// \name Classification helpers
		/// \{
		template<typename T> bool builtin_isnan(T arg);
		template<typename T> bool builtin_isinf(T arg);
		template<typename T> bool builtin_signbit(T arg);
		/// \}

		/// \name Conversion
		/// \{
		template<std::float_round_style R> uint16 float2half(float value);
		float half2float(uint16 value);
		template<typename T,std::float_round_style R> T half2int(uint16 value);
		/// \}

		template<typename T,typename U> typename binary_enable<T,U,bool>::type operator==(T x, U y);
		template<typename T,typename U> typename binary_enable<T,U,bool>::type operator!=(T x, U y);
		template<typename T,typename U> typename binary_enable<T,U,bool>::type operator<(T x, U y);
		template<typename T,typename U> typename binary_enable<T,U,bool>::type operator>(T x, U y);
		template<typename T,typename U> typename binary_enable<T,U,bool>::type operator<=(T x, U y);
		template<typename T,typename U> typename binary_enable<T,U,bool>::type operator>=(T x, U y);

		template<typename T,typename U> typename binary_result<function::plus,T,U>::type operator+(T x, U y);
		template<typename T,typename U> typename binary_result<function::minus,T,U>::type operator-(T x, U y);
		template<typename T,typename U> typename binary_result<function::multiplies,T,U>::type operator*(T x, U y);
		template<typename T,typename U> typename binary_result<function::divides,T,U>::type operator/(T x, U y);
		template<typename T> typename unary_enable<T,T>::type operator+(T arg);
		template<typename T> typename unary_result<function::negate,T>::type operator-(T arg);

		template<typename T,typename charT,typename traits> typename unary_enable<T,std::basic_ostream<charT,traits>&>::type operator<<(std::basic_ostream<charT,traits> &out, T arg);
		template<typename charT,typename traits> std::basic_istream<charT,traits>& operator>>(std::basic_istream<charT,traits> &in, half &arg);

		template<typename T> typename unary_result<function::fabs,T>::type abs(T arg);
		template<typename T> typename unary_result<function::fabs,T>::type fabs(T arg);
		template<typename T,typename U> typename binary_result<function::fmod,T,U>::type fmod(T x, U y);
		template<typename T,typename U> typename binary_result<function::remainder,T,U>::type remainder(T x, U y);
		template<typename T,typename U> typename binary_result<function::remquo,T,U>::type remquo(T x, U y, int *quo);
		template<typename T,typename U,typename V> typename ternary_result<function::fma,T,U,V>::type fma(T x, U y, V z);
		template<typename T,typename U> typename binary_result<function::fmax,T,U>::type fmax(T x, U y);
		template<typename T,typename U> typename binary_result<function::fmin,T,U>::type fmin(T x, U y);
		template<typename T,typename U> typename binary_result<function::fdim,T,U>::type fdim(T x, U y);
		half nanh(const char *arg);

		template<typename T> typename unary_result<function::exp,T>::type exp(T arg);
		template<typename T> typename unary_result<function::exp2,T>::type exp2(T arg);
		template<typename T> typename unary_result<function::expm1,T>::type expm1(T arg);
		template<typename T> typename unary_result<function::log,T>::type log(T arg);
		template<typename T> typename unary_result<function::log10,T>::type log10(T arg);
		template<typename T> typename unary_result<function::log1p,T>::type log1p(T arg);
		template<typename T> typename unary_result<function::log2,T>::type log2(T arg);

		template<typename T> typename unary_result<function::sqrt,T>::type sqrt(T arg);
		template<typename T> typename unary_result<function::cbrt,T>::type cbrt(T arg);
		template<typename T,typename U> typename binary_result<function::hypot,T,U>::type hypot(T x, U y);
		template<typename T,typename U> typename binary_result<function::pow,T,U>::type pow(T base, U exp);

		template<typename T> typename unary_result<function::sin,T>::type sin(T arg);
		template<typename T> typename unary_result<function::cos,T>::type cos(T arg);
		template<typename T> typename unary_result<function::tan,T>::type tan(T arg);
		template<typename T> typename unary_result<function::asin,T>::type asin(T arg);
		template<typename T> typename unary_result<function::acos,T>::type acos(T arg);
		template<typename T> typename unary_result<function::atan,T>::type atan(T arg);
		template<typename T,typename U> typename binary_result<function::atan2,T,U>::type atan2(T x, U y);

		template<typename T> typename unary_result<function::sinh,T>::type sinh(T arg);
		template<typename T> typename unary_result<function::cosh,T>::type cosh(T arg);
		template<typename T> typename unary_result<function::tanh,T>::type tanh(T arg);
		template<typename T> typename unary_result<function::asinh,T>::type asinh(T arg);
		template<typename T> typename unary_result<function::acosh,T>::type acosh(T arg);
		template<typename T> typename unary_result<function::atanh,T>::type atanh(T arg);

		template<typename T> typename unary_result<function::erf,T>::type erf(T arg);
		template<typename T> typename unary_result<function::erfc,T>::type erfc(T arg);
		template<typename T> typename unary_result<function::lgamma,T>::type lgamma(T arg);
		template<typename T> typename unary_result<function::tgamma,T>::type tgamma(T arg);

		template<typename T> typename unary_result<function::ceil,T>::type ceil(T arg);
		template<typename T> typename unary_result<function::floor,T>::type floor(T arg);
		template<typename T> typename unary_result<function::trunc,T>::type trunc(T arg);
		template<typename T> typename unary_result<function::round,T>::type round(T arg);
		template<typename T> typename unary_enable<T,long>::type lround(T arg);
		template<typename T> typename unary_enable<T,long long>::type llround(T arg);
		template<typename T> typename unary_result<function::nearbyint,T>::type nearbyint(T arg);
		template<typename T> typename unary_result<function::rint,T>::type rint(T arg);
		template<typename T> typename unary_enable<T,long>::type lrint(T arg);
		template<typename T> typename unary_enable<T,long long>::type llrint(T arg);

		template<typename T> typename unary_result<function::frexp,T>::type frexp(T arg, int *exp);
		template<typename T> typename unary_result<function::scalbln,T>::type ldexp(T arg, int exp);
		template<typename T> typename unary_result<function::modf,T>::type modf(T arg, half *iptr);
		template<typename T> typename unary_result<function::scalbln,T>::type scalbn(T arg, int exp);
		template<typename T> typename unary_result<function::scalbln,T>::type scalbln(T arg, long exp);
		template<typename T> typename unary_enable<T,int>::type ilogb(T arg);
		template<typename T> typename unary_result<function::logb,T>::type logb(T arg);
		template<typename T,typename U> typename binary_result<function::nextafter,T,U>::type nextafter(T from, U to);
		template<typename T> typename unary_result<function::nexttoward,T>::type nexttoward(T from, long double to);
		template<typename T,typename U> typename binary_result<function::copysign,T,U>::type copysign(T x, U y);

		template<typename T> typename unary_enable<T,int>::type fpclassify(T arg);
		template<typename T> typename unary_enable<T,bool>::type isfinite(T arg);
		template<typename T> typename unary_enable<T,bool>::type isinf(T arg);
		template<typename T> typename unary_enable<T,bool>::type isnan(T arg);
		template<typename T> typename unary_enable<T,bool>::type isnormal(T arg);
		template<typename T> typename unary_enable<T,bool>::type signbit(T arg);

		template<typename T,typename U> typename binary_enable<T,U,bool>::type isgreater(T x, U y);
		template<typename T,typename U> typename binary_enable<T,U,bool>::type isgreaterequal(T x, U y);
		template<typename T,typename U> typename binary_enable<T,U,bool>::type isless(T x, U y);
		template<typename T,typename U> typename binary_enable<T,U,bool>::type islessequal(T x, U y);
		template<typename T,typename U> typename binary_enable<T,U,bool>::type islessgreater(T x, U y);
		template<typename T,typename U> typename binary_enable<T,U,bool>::type isunordered(T x, U y);

		/// \name Casting
		/// \{
		template<typename T,typename U> T half_cast(const U &arg);
		template<typename T,std::float_round_style R,typename U> T half_cast(const U &arg);
		/// \}
	}

	using detail::operator==;
	using detail::operator!=;
	using detail::operator<;
	using detail::operator>;
	using detail::operator<=;
	using detail::operator>=;

	using detail::operator+;
	using detail::operator-;
	using detail::operator*;
	using detail::operator/;

	using detail::operator<<;
	using detail::operator>>;

	using detail::abs;
	using detail::fabs;
	using detail::fmod;
	using detail::remainder;
	using detail::remquo;
	using detail::fma;
	using detail::fmax;
	using detail::fmin;
	using detail::fdim;
	using detail::nanh;

	using detail::exp;
	using detail::exp2;
	using detail::expm1;
	using detail::log;
	using detail::log10;
	using detail::log1p;
	using detail::log2;

	using detail::sqrt;
	using detail::cbrt;
	using detail::hypot;
	using detail::pow;

	using detail::sin;
	using detail::cos;
	using detail::tan;
	using detail::asin;
	using detail::acos;
	using detail::atan;
	using detail::atan2;

	using detail::sinh;
	using detail::cosh;
	using detail::tanh;
	using detail::asinh;
	using detail::acosh;
	using detail::atanh;

	using detail::erf;
	using detail::erfc;
	using detail::lgamma;
	using detail::tgamma;

	using detail::ceil;
	using detail::floor;
	using detail::trunc;
	using detail::round;
	using detail::lround;
	using detail::llround;
	using detail::nearbyint;
	using detail::rint;
	using detail::lrint;
	using detail::llrint;

	using detail::frexp;
	using detail::ldexp;
	using detail::modf;
	using detail::scalbn;
	using detail::scalbln;
	using detail::ilogb;
	using detail::logb;
	using detail::nextafter;
	using detail::nexttoward;
	using detail::copysign;

	using detail::fpclassify;
	using detail::isfinite;
	using detail::isinf;
	using detail::isnan;
	using detail::isnormal;
	using detail::signbit;

	using detail::isgreater;
	using detail::isgreaterequal;
	using detail::isless;
	using detail::islessequal;
	using detail::islessgreater;
	using detail::isunordered;

	using detail::half_cast;

	/// Half-precision floating point type.
	/// This class implements an IEEE-conformant half-precision floating point type with the usual arithmetic operators and 
	/// conversions. It is implicitly convertible to single-precision floating point, which makes artihmetic expressions and 
	/// functions with mixed-type operands to be of the most precise operand type. Additionally all arithmetic operations 
	/// (and many mathematical functions) are carried out in single-precision internally. All conversions from single- to 
	/// half-precision are done using truncation (round towards zero), but temporary results inside chained arithmetic 
	/// expressions are kept in single-precision as long as possible (while of course still maintaining a strong half-precision type).
	///
	/// According to the C++98/03 definition, the half type is not a POD type. But according to C++11's less strict and 
	/// extended definitions it is both a standard layout type and a trivially copyable type (even if not a POD type), which 
	/// means it can be standard-conformantly copied using raw binary copies. But in this context some more words about the 
	/// actual size of the type. Although the half is representing an IEEE 16-bit type, it does not neccessarily have to be of 
	/// exactly 16-bits size. But on any reasonable implementation the actual binary representation of this type will most 
	/// probably not ivolve any additional "magic" or padding beyond the simple binary representation of the underlying 16-bit 
	/// IEEE number, even if not strictly guaranteed by the standard. But even then it only has an actual size of 16 bits if 
	/// your C++ implementation supports an unsigned integer type of exactly 16 bits width. But this should be the case on 
	/// nearly any reasonable platform.
	///
	/// So if your C++ implementation is not totally exotic or imposes special alignment requirements, it is a reasonable 
	/// assumption that the data of a half is just comprised of the 2 bytes of the underlying IEEE representation.
	class half : public detail::half_expr<half>
	{
		friend struct detail::noarg_functions;
		template<typename> friend struct detail::unary_functions;
		template<typename,typename> friend struct detail::binary_functions;
		template<typename,typename,typename> friend struct detail::ternary_functions;
		template<typename,typename,std::float_round_style> friend struct detail::half_caster;
		friend class std::numeric_limits<half>;
	#if HALF_ENABLE_CPP11_HASH
		friend struct std::hash<half>;
	#endif

	public:
		/// Default constructor.
		/// This initializes the half to 0. Although this does not match the builtin types' default-initialization semantics 
		/// and may be less efficient than no initialization, it is needed to provide proper value-initialization semantics.
		HALF_CONSTEXPR half() : data_() {}

		/// Conversion constructor.
		/// \param rhs float to convert
		explicit half(float rhs) : data_(detail::float2half<round_style>(rhs)) {}

		/// Copy constructor.
		/// \tparam E type of concrete half expression
		/// \param rhs half expression to copy from
		template<typename E> half(const detail::half_expr<E> &rhs) : data_(detail::float2half<round_style>(static_cast<float>(rhs))) {}
	
		/// Conversion to single-precision.
		/// \return single precision value representing expression value
		operator float() const { return detail::half2float(data_); }

		/// Assignment operator.
		/// \tparam E type of concrete half expression
		/// \param rhs half expression to copy from
		/// \return reference to this half
		template<typename E> half& operator=(const detail::half_expr<E> &rhs)
		{
			data_ = detail::float2half<round_style>(static_cast<float>(rhs));
			return *this;
		}

		/// Arithmetic assignment.
		/// \tparam E type of concrete half expression
		/// \param rhs half expression to add
		/// \return reference to this half
		template<typename E> half& operator+=(const detail::half_expr<E> &rhs)
		{
			data_ = detail::float2half<round_style>(detail::half2float(data_)+static_cast<float>(rhs));
			return *this;
		}

		/// Arithmetic assignment.
		/// \tparam E type of concrete half expression
		/// \param rhs half expression to subtract
		/// \return reference to this half
		template<typename E> half& operator-=(const detail::half_expr<E> &rhs)
		{
			data_ = detail::float2half<round_style>(detail::half2float(data_)-static_cast<float>(rhs));
			return *this;
		}

		/// Arithmetic assignment.
		/// \tparam E type of concrete half expression
		/// \param rhs half expression to multiply with
		/// \return reference to this half
		template<typename E> half& operator*=(const detail::half_expr<E> &rhs)
		{
			data_ = detail::float2half<round_style>(detail::half2float(data_)*static_cast<float>(rhs));
			return *this;
		}

		/// Arithmetic assignment.
		/// \tparam E type of concrete half expression
		/// \param rhs half expression to divide by
		/// \return reference to this half
		template<typename E> half& operator/=(const detail::half_expr<E> &rhs)
		{
			data_ = detail::float2half<round_style>(detail::half2float(data_)/static_cast<float>(rhs));
			return *this;
		}

		/// Assignment operator.
		/// \param rhs single-precision value to copy from
		/// \return reference to this half
		half& operator=(float rhs)
		{
			data_ = detail::float2half<round_style>(rhs);
			return *this;
		}

		/// Arithmetic assignment.
		/// \param rhs single-precision value to add
		/// \return reference to this half
		half& operator+=(float rhs)
		{
			data_ = detail::float2half<round_style>(detail::half2float(data_)+rhs);
			return *this;
		}

		/// Arithmetic assignment.
		/// \param rhs single-precision value to subtract
		/// \return reference to this half
		half& operator-=(float rhs)
		{
			data_ = detail::float2half<round_style>(detail::half2float(data_)-rhs);
			return *this;
		}

		/// Arithmetic assignment.
		/// \param rhs single-precision value to multiply with
		/// \return reference to this half
		half& operator*=(float rhs)
		{
			data_ = detail::float2half<round_style>(detail::half2float(data_)*rhs);
			return *this;
		}

		/// Arithmetic assignment.
		/// \param rhs single-precision value to divide by
		/// \return reference to this half
		half& operator/=(float rhs)
		{
			data_ = detail::float2half<round_style>(detail::half2float(data_)/rhs);
			return *this;
		}

		/// Prefix increment.
		/// \return incremented half value
		half& operator++()
		{
			data_ = detail::float2half<round_style>(detail::half2float(data_)+1.0f);
			return *this;
		}

		/// Prefix decrement.
		/// \return decremented half value
		half& operator--()
		{
			data_ = detail::float2half<round_style>(detail::half2float(data_)-1.0f);
			return *this;
		}

		/// Postfix increment.
		/// \return non-incremented half value
		half operator++(int)
		{
			detail::uint16 out = data_;
			data_ = detail::float2half<round_style>(detail::half2float(data_)+1.0f);
			return half(out, true);
		}

		/// Postfix decrement.
		/// \return non-decremented half value
		half operator--(int)
		{
			detail::uint16 out = data_;
			data_ = detail::float2half<round_style>(detail::half2float(data_)-1.0f);
			return half(out, true);
		}
	
	private:
		/// Rounding mode to use (always `std::round_indeterminate`)
		static const std::float_round_style round_style = std::round_indeterminate;

		/// Constructor.
		/// \param bits binary representation to set half to
		HALF_CONSTEXPR half(detail::uint16 bits, bool) : data_(bits) {}

		/// Internal binary representation
		detail::uint16 data_;
	};

#if HALF_ENABLE_CPP11_USER_LITERALS
	namespace literal
	{
		/// Half literal.
		/// \param d literal value
		/// \return half with given value (if representable)
		inline half operator "" _h(long double d)
		{
			return half(static_cast<float>(d));
		}
	}
#endif

	namespace detail
	{
		/// Check for NaN.
		/// \tparam T argument type (builtin floating point type)
		/// \param arg value to query
		/// \retval true if not a number
		/// \retval false else
		template<typename T> bool builtin_isnan(T arg)
		{
		#if HALF_ENABLE_CPP11_CMATH
			return std::isnan(arg);
		#elif defined(_MSC_VER)
			return _isnan(arg) != 0;
		#else
			return arg != arg;
		#endif
		}

		/// Check for infinity.
		/// \tparam T argument type (builtin floating point type)
		/// \param arg value to query
		/// \retval true if infinity
		/// \retval false else
		template<typename T> bool builtin_isinf(T arg)
		{
		#if HALF_ENABLE_CPP11_CMATH
			return std::isinf(arg);
		#elif defined(_MSC_VER)
			return !_finite(arg) && !_isnan(arg);
		#else
			return arg == std::numeric_limits<T>::infinity() || arg == -std::numeric_limits<T>::infinity();
		#endif
		}

		/// Check sign.
		/// \tparam T argument type (builtin floating point type)
		/// \param arg value to query
		/// \retval true if signbit set
		/// \retval false else
		template<typename T> bool builtin_signbit(T arg)
		{
		#if HALF_ENABLE_CPP11_CMATH
			return std::signbit(arg);
		#else
			return arg < T();
		#endif
		}

		/// Convert IEEE single-precision to half-precision.
		/// \tparam R rounding mode to use, `std::round_indeterminate` for fastest rounding
		/// \param value single-precision value
		/// \return binary representation of half-precision value
		template<std::float_round_style R> uint16 float2half(float value/*, booltype<true>*/)
		{
		#if HALF_ENABLE_CPP11_STATIC_ASSERT
			static_assert(std::numeric_limits<float>::is_iec559, "float to half conversion needs IEEE 754 conformant 'float' type");
			static_assert(sizeof(uint32)==sizeof(float), "float to half conversion needs unsigned integer type of exactly the size of a 'float'");
		#endif
			static const uint16 base_table[512] = { 
				0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
				0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
				0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
				0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
				0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
				0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
				0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 
				0x0200, 0x0400, 0x0800, 0x0C00, 0x1000, 0x1400, 0x1800, 0x1C00, 0x2000, 0x2400, 0x2800, 0x2C00, 0x3000, 0x3400, 0x3800, 0x3C00, 
				0x4000, 0x4400, 0x4800, 0x4C00, 0x5000, 0x5400, 0x5800, 0x5C00, 0x6000, 0x6400, 0x6800, 0x6C00, 0x7000, 0x7400, 0x7800, 0x7C00, 
				0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 
				0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 
				0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 
				0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 
				0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 
				0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 
				0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 
				0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 
				0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 
				0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 
				0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 
				0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 
				0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 
				0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8001, 0x8002, 0x8004, 0x8008, 0x8010, 0x8020, 0x8040, 0x8080, 0x8100, 
				0x8200, 0x8400, 0x8800, 0x8C00, 0x9000, 0x9400, 0x9800, 0x9C00, 0xA000, 0xA400, 0xA800, 0xAC00, 0xB000, 0xB400, 0xB800, 0xBC00, 
				0xC000, 0xC400, 0xC800, 0xCC00, 0xD000, 0xD400, 0xD800, 0xDC00, 0xE000, 0xE400, 0xE800, 0xEC00, 0xF000, 0xF400, 0xF800, 0xFC00, 
				0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 
				0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 
				0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 
				0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 
				0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 
				0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 
				0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00 };
			static const unsigned char shift_table[512] = { 
				24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 
				24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 
				24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 
				24, 24, 24, 24, 24, 24, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 
				13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 
				24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 
				24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 
				24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 13, 
				24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 
				24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 
				24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 
				24, 24, 24, 24, 24, 24, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 
				13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 
				24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 
				24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 
				24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 13 };
			uint32 bits;// = *reinterpret_cast<uint32*>(&value);
			std::memcpy(&bits, &value, sizeof(float));
			uint16 hbits = base_table[bits>>23] + ((bits&0x7FFFFF)>>shift_table[bits>>23]);
			if(R == std::round_to_nearest)
				hbits += (((bits&0x7FFFFF)>>(shift_table[bits>>23]-1))|(((bits>>23)&0xFF)==102)) & ((hbits&0x7C00)!=0x7C00);
			else if(R == std::round_toward_zero)
				hbits -= ((hbits&0x7FFF)==0x7C00) & ~shift_table[bits>>23];
			else if(R == std::round_toward_infinity)
				hbits += ((((bits&0x7FFFFF&((static_cast<uint32>(1)<<(shift_table[bits>>23]))-1))!=0)|(((bits>>23)<=102)&
					((bits>>23)!=0)))&(hbits<0x7C00)) - ((hbits==0xFC00)&((bits>>23)!=511));
			else if(R == std::round_toward_neg_infinity)
				hbits += ((((bits&0x7FFFFF&((static_cast<uint32>(1)<<(shift_table[bits>>23]))-1))!=0)|(((bits>>23)<=358)&
					((bits>>23)!=256)))&(hbits<0xFC00)&(hbits>>15)) - ((hbits==0x7C00)&((bits>>23)!=255));
			return hbits;
		}
/*
		/// Convert non-IEEE single-precision to half-precision.
		/// \param value single-precision value
		/// \return binary representation of half-precision value
		template<std::float_round_style R> uint16 float2half_impl(float value, booltype<false>)
		{
			uint16 hbits = builtin_signbit(value) << 15;
			if(value == 0.0f)
				return hbits;
			if(builtin_isnan(value))
				return hbits | 0x7FFF;
			if(builtin_isinf(value))
				return hbits | 0x7C00;
			int exp;
			std::frexp(value, &exp);
			if(exp > 16)
			{
				if(R == std::round_toward_zero)
					return hbits | 0x7BFF;
				else if(R == std::round_toward_infinity)
					return hbits | 0x7C00 - (hbits>>15);
				else if(R == std::round_toward_neg_infinity)
					return hbits | 0x7BFF + (hbits>>15);
				return hbits | 0x7C00;
			}
			if(exp < -13)
				value = std::ldexp(value, 24);
			else
			{
				value = std::ldexp(value, 11-exp);
				hbits |= ((exp+14)<<10);
			}
			int ival = static_cast<int>(value);
			hbits |= (static_cast<uint16>(std::abs(ival))&0x3FF);
			if(R == std::round_to_nearest)
				hbits += std::abs(value-static_cast<float>(ival)) >= 0.5f;
			else if(R == std::round_toward_infinity)
				hbits += value > static_cast<float>(ival);
			else if(R == std::round_toward_neg_infinity)
				hbits += value < static_cast<float>(ival);
			return hbits;
		}

		/// Convert single-precision to half-precision.
		/// \param value single-precision value
		/// \return binary representation of half-precision value
		template<std::float_round_style R> uint16 float2half(float value)
		{
			return float2half_impl<R>(value, booltype<std::numeric_limits<float>::is_iec559>());
		}
*/
		/// Convert half-precision to IEEE single-precision.
		/// \param value binary representation of half-precision value
		/// \return single-precision value
		inline float half2float(uint16 value/*, booltype<true>*/)
		{
		#if HALF_ENABLE_CPP11_STATIC_ASSERT
			static_assert(std::numeric_limits<float>::is_iec559, "half to float conversion needs IEEE 754 conformant 'float' type");
			static_assert(sizeof(uint32)==sizeof(float), "half to float conversion needs unsigned integer type of exactly the size of a 'float'");
		#endif
			static const uint32 mantissa_table[2048] = { 
				0x00000000, 0x33800000, 0x34000000, 0x34400000, 0x34800000, 0x34A00000, 0x34C00000, 0x34E00000, 0x35000000, 0x35100000, 0x35200000, 0x35300000, 0x35400000, 0x35500000, 0x35600000, 0x35700000, 
				0x35800000, 0x35880000, 0x35900000, 0x35980000, 0x35A00000, 0x35A80000, 0x35B00000, 0x35B80000, 0x35C00000, 0x35C80000, 0x35D00000, 0x35D80000, 0x35E00000, 0x35E80000, 0x35F00000, 0x35F80000, 
				0x36000000, 0x36040000, 0x36080000, 0x360C0000, 0x36100000, 0x36140000, 0x36180000, 0x361C0000, 0x36200000, 0x36240000, 0x36280000, 0x362C0000, 0x36300000, 0x36340000, 0x36380000, 0x363C0000, 
				0x36400000, 0x36440000, 0x36480000, 0x364C0000, 0x36500000, 0x36540000, 0x36580000, 0x365C0000, 0x36600000, 0x36640000, 0x36680000, 0x366C0000, 0x36700000, 0x36740000, 0x36780000, 0x367C0000, 
				0x36800000, 0x36820000, 0x36840000, 0x36860000, 0x36880000, 0x368A0000, 0x368C0000, 0x368E0000, 0x36900000, 0x36920000, 0x36940000, 0x36960000, 0x36980000, 0x369A0000, 0x369C0000, 0x369E0000, 
				0x36A00000, 0x36A20000, 0x36A40000, 0x36A60000, 0x36A80000, 0x36AA0000, 0x36AC0000, 0x36AE0000, 0x36B00000, 0x36B20000, 0x36B40000, 0x36B60000, 0x36B80000, 0x36BA0000, 0x36BC0000, 0x36BE0000, 
				0x36C00000, 0x36C20000, 0x36C40000, 0x36C60000, 0x36C80000, 0x36CA0000, 0x36CC0000, 0x36CE0000, 0x36D00000, 0x36D20000, 0x36D40000, 0x36D60000, 0x36D80000, 0x36DA0000, 0x36DC0000, 0x36DE0000, 
				0x36E00000, 0x36E20000, 0x36E40000, 0x36E60000, 0x36E80000, 0x36EA0000, 0x36EC0000, 0x36EE0000, 0x36F00000, 0x36F20000, 0x36F40000, 0x36F60000, 0x36F80000, 0x36FA0000, 0x36FC0000, 0x36FE0000, 
				0x37000000, 0x37010000, 0x37020000, 0x37030000, 0x37040000, 0x37050000, 0x37060000, 0x37070000, 0x37080000, 0x37090000, 0x370A0000, 0x370B0000, 0x370C0000, 0x370D0000, 0x370E0000, 0x370F0000, 
				0x37100000, 0x37110000, 0x37120000, 0x37130000, 0x37140000, 0x37150000, 0x37160000, 0x37170000, 0x37180000, 0x37190000, 0x371A0000, 0x371B0000, 0x371C0000, 0x371D0000, 0x371E0000, 0x371F0000, 
				0x37200000, 0x37210000, 0x37220000, 0x37230000, 0x37240000, 0x37250000, 0x37260000, 0x37270000, 0x37280000, 0x37290000, 0x372A0000, 0x372B0000, 0x372C0000, 0x372D0000, 0x372E0000, 0x372F0000, 
				0x37300000, 0x37310000, 0x37320000, 0x37330000, 0x37340000, 0x37350000, 0x37360000, 0x37370000, 0x37380000, 0x37390000, 0x373A0000, 0x373B0000, 0x373C0000, 0x373D0000, 0x373E0000, 0x373F0000, 
				0x37400000, 0x37410000, 0x37420000, 0x37430000, 0x37440000, 0x37450000, 0x37460000, 0x37470000, 0x37480000, 0x37490000, 0x374A0000, 0x374B0000, 0x374C0000, 0x374D0000, 0x374E0000, 0x374F0000, 
				0x37500000, 0x37510000, 0x37520000, 0x37530000, 0x37540000, 0x37550000, 0x37560000, 0x37570000, 0x37580000, 0x37590000, 0x375A0000, 0x375B0000, 0x375C0000, 0x375D0000, 0x375E0000, 0x375F0000, 
				0x37600000, 0x37610000, 0x37620000, 0x37630000, 0x37640000, 0x37650000, 0x37660000, 0x37670000, 0x37680000, 0x37690000, 0x376A0000, 0x376B0000, 0x376C0000, 0x376D0000, 0x376E0000, 0x376F0000, 
				0x37700000, 0x37710000, 0x37720000, 0x37730000, 0x37740000, 0x37750000, 0x37760000, 0x37770000, 0x37780000, 0x37790000, 0x377A0000, 0x377B0000, 0x377C0000, 0x377D0000, 0x377E0000, 0x377F0000, 
				0x37800000, 0x37808000, 0x37810000, 0x37818000, 0x37820000, 0x37828000, 0x37830000, 0x37838000, 0x37840000, 0x37848000, 0x37850000, 0x37858000, 0x37860000, 0x37868000, 0x37870000, 0x37878000, 
				0x37880000, 0x37888000, 0x37890000, 0x37898000, 0x378A0000, 0x378A8000, 0x378B0000, 0x378B8000, 0x378C0000, 0x378C8000, 0x378D0000, 0x378D8000, 0x378E0000, 0x378E8000, 0x378F0000, 0x378F8000, 
				0x37900000, 0x37908000, 0x37910000, 0x37918000, 0x37920000, 0x37928000, 0x37930000, 0x37938000, 0x37940000, 0x37948000, 0x37950000, 0x37958000, 0x37960000, 0x37968000, 0x37970000, 0x37978000, 
				0x37980000, 0x37988000, 0x37990000, 0x37998000, 0x379A0000, 0x379A8000, 0x379B0000, 0x379B8000, 0x379C0000, 0x379C8000, 0x379D0000, 0x379D8000, 0x379E0000, 0x379E8000, 0x379F0000, 0x379F8000, 
				0x37A00000, 0x37A08000, 0x37A10000, 0x37A18000, 0x37A20000, 0x37A28000, 0x37A30000, 0x37A38000, 0x37A40000, 0x37A48000, 0x37A50000, 0x37A58000, 0x37A60000, 0x37A68000, 0x37A70000, 0x37A78000, 
				0x37A80000, 0x37A88000, 0x37A90000, 0x37A98000, 0x37AA0000, 0x37AA8000, 0x37AB0000, 0x37AB8000, 0x37AC0000, 0x37AC8000, 0x37AD0000, 0x37AD8000, 0x37AE0000, 0x37AE8000, 0x37AF0000, 0x37AF8000, 
				0x37B00000, 0x37B08000, 0x37B10000, 0x37B18000, 0x37B20000, 0x37B28000, 0x37B30000, 0x37B38000, 0x37B40000, 0x37B48000, 0x37B50000, 0x37B58000, 0x37B60000, 0x37B68000, 0x37B70000, 0x37B78000, 
				0x37B80000, 0x37B88000, 0x37B90000, 0x37B98000, 0x37BA0000, 0x37BA8000, 0x37BB0000, 0x37BB8000, 0x37BC0000, 0x37BC8000, 0x37BD0000, 0x37BD8000, 0x37BE0000, 0x37BE8000, 0x37BF0000, 0x37BF8000, 
				0x37C00000, 0x37C08000, 0x37C10000, 0x37C18000, 0x37C20000, 0x37C28000, 0x37C30000, 0x37C38000, 0x37C40000, 0x37C48000, 0x37C50000, 0x37C58000, 0x37C60000, 0x37C68000, 0x37C70000, 0x37C78000, 
				0x37C80000, 0x37C88000, 0x37C90000, 0x37C98000, 0x37CA0000, 0x37CA8000, 0x37CB0000, 0x37CB8000, 0x37CC0000, 0x37CC8000, 0x37CD0000, 0x37CD8000, 0x37CE0000, 0x37CE8000, 0x37CF0000, 0x37CF8000, 
				0x37D00000, 0x37D08000, 0x37D10000, 0x37D18000, 0x37D20000, 0x37D28000, 0x37D30000, 0x37D38000, 0x37D40000, 0x37D48000, 0x37D50000, 0x37D58000, 0x37D60000, 0x37D68000, 0x37D70000, 0x37D78000, 
				0x37D80000, 0x37D88000, 0x37D90000, 0x37D98000, 0x37DA0000, 0x37DA8000, 0x37DB0000, 0x37DB8000, 0x37DC0000, 0x37DC8000, 0x37DD0000, 0x37DD8000, 0x37DE0000, 0x37DE8000, 0x37DF0000, 0x37DF8000, 
				0x37E00000, 0x37E08000, 0x37E10000, 0x37E18000, 0x37E20000, 0x37E28000, 0x37E30000, 0x37E38000, 0x37E40000, 0x37E48000, 0x37E50000, 0x37E58000, 0x37E60000, 0x37E68000, 0x37E70000, 0x37E78000, 
				0x37E80000, 0x37E88000, 0x37E90000, 0x37E98000, 0x37EA0000, 0x37EA8000, 0x37EB0000, 0x37EB8000, 0x37EC0000, 0x37EC8000, 0x37ED0000, 0x37ED8000, 0x37EE0000, 0x37EE8000, 0x37EF0000, 0x37EF8000, 
				0x37F00000, 0x37F08000, 0x37F10000, 0x37F18000, 0x37F20000, 0x37F28000, 0x37F30000, 0x37F38000, 0x37F40000, 0x37F48000, 0x37F50000, 0x37F58000, 0x37F60000, 0x37F68000, 0x37F70000, 0x37F78000, 
				0x37F80000, 0x37F88000, 0x37F90000, 0x37F98000, 0x37FA0000, 0x37FA8000, 0x37FB0000, 0x37FB8000, 0x37FC0000, 0x37FC8000, 0x37FD0000, 0x37FD8000, 0x37FE0000, 0x37FE8000, 0x37FF0000, 0x37FF8000, 
				0x38000000, 0x38004000, 0x38008000, 0x3800C000, 0x38010000, 0x38014000, 0x38018000, 0x3801C000, 0x38020000, 0x38024000, 0x38028000, 0x3802C000, 0x38030000, 0x38034000, 0x38038000, 0x3803C000, 
				0x38040000, 0x38044000, 0x38048000, 0x3804C000, 0x38050000, 0x38054000, 0x38058000, 0x3805C000, 0x38060000, 0x38064000, 0x38068000, 0x3806C000, 0x38070000, 0x38074000, 0x38078000, 0x3807C000, 
				0x38080000, 0x38084000, 0x38088000, 0x3808C000, 0x38090000, 0x38094000, 0x38098000, 0x3809C000, 0x380A0000, 0x380A4000, 0x380A8000, 0x380AC000, 0x380B0000, 0x380B4000, 0x380B8000, 0x380BC000, 
				0x380C0000, 0x380C4000, 0x380C8000, 0x380CC000, 0x380D0000, 0x380D4000, 0x380D8000, 0x380DC000, 0x380E0000, 0x380E4000, 0x380E8000, 0x380EC000, 0x380F0000, 0x380F4000, 0x380F8000, 0x380FC000, 
				0x38100000, 0x38104000, 0x38108000, 0x3810C000, 0x38110000, 0x38114000, 0x38118000, 0x3811C000, 0x38120000, 0x38124000, 0x38128000, 0x3812C000, 0x38130000, 0x38134000, 0x38138000, 0x3813C000, 
				0x38140000, 0x38144000, 0x38148000, 0x3814C000, 0x38150000, 0x38154000, 0x38158000, 0x3815C000, 0x38160000, 0x38164000, 0x38168000, 0x3816C000, 0x38170000, 0x38174000, 0x38178000, 0x3817C000, 
				0x38180000, 0x38184000, 0x38188000, 0x3818C000, 0x38190000, 0x38194000, 0x38198000, 0x3819C000, 0x381A0000, 0x381A4000, 0x381A8000, 0x381AC000, 0x381B0000, 0x381B4000, 0x381B8000, 0x381BC000, 
				0x381C0000, 0x381C4000, 0x381C8000, 0x381CC000, 0x381D0000, 0x381D4000, 0x381D8000, 0x381DC000, 0x381E0000, 0x381E4000, 0x381E8000, 0x381EC000, 0x381F0000, 0x381F4000, 0x381F8000, 0x381FC000, 
				0x38200000, 0x38204000, 0x38208000, 0x3820C000, 0x38210000, 0x38214000, 0x38218000, 0x3821C000, 0x38220000, 0x38224000, 0x38228000, 0x3822C000, 0x38230000, 0x38234000, 0x38238000, 0x3823C000, 
				0x38240000, 0x38244000, 0x38248000, 0x3824C000, 0x38250000, 0x38254000, 0x38258000, 0x3825C000, 0x38260000, 0x38264000, 0x38268000, 0x3826C000, 0x38270000, 0x38274000, 0x38278000, 0x3827C000, 
				0x38280000, 0x38284000, 0x38288000, 0x3828C000, 0x38290000, 0x38294000, 0x38298000, 0x3829C000, 0x382A0000, 0x382A4000, 0x382A8000, 0x382AC000, 0x382B0000, 0x382B4000, 0x382B8000, 0x382BC000, 
				0x382C0000, 0x382C4000, 0x382C8000, 0x382CC000, 0x382D0000, 0x382D4000, 0x382D8000, 0x382DC000, 0x382E0000, 0x382E4000, 0x382E8000, 0x382EC000, 0x382F0000, 0x382F4000, 0x382F8000, 0x382FC000, 
				0x38300000, 0x38304000, 0x38308000, 0x3830C000, 0x38310000, 0x38314000, 0x38318000, 0x3831C000, 0x38320000, 0x38324000, 0x38328000, 0x3832C000, 0x38330000, 0x38334000, 0x38338000, 0x3833C000, 
				0x38340000, 0x38344000, 0x38348000, 0x3834C000, 0x38350000, 0x38354000, 0x38358000, 0x3835C000, 0x38360000, 0x38364000, 0x38368000, 0x3836C000, 0x38370000, 0x38374000, 0x38378000, 0x3837C000, 
				0x38380000, 0x38384000, 0x38388000, 0x3838C000, 0x38390000, 0x38394000, 0x38398000, 0x3839C000, 0x383A0000, 0x383A4000, 0x383A8000, 0x383AC000, 0x383B0000, 0x383B4000, 0x383B8000, 0x383BC000, 
				0x383C0000, 0x383C4000, 0x383C8000, 0x383CC000, 0x383D0000, 0x383D4000, 0x383D8000, 0x383DC000, 0x383E0000, 0x383E4000, 0x383E8000, 0x383EC000, 0x383F0000, 0x383F4000, 0x383F8000, 0x383FC000, 
				0x38400000, 0x38404000, 0x38408000, 0x3840C000, 0x38410000, 0x38414000, 0x38418000, 0x3841C000, 0x38420000, 0x38424000, 0x38428000, 0x3842C000, 0x38430000, 0x38434000, 0x38438000, 0x3843C000, 
				0x38440000, 0x38444000, 0x38448000, 0x3844C000, 0x38450000, 0x38454000, 0x38458000, 0x3845C000, 0x38460000, 0x38464000, 0x38468000, 0x3846C000, 0x38470000, 0x38474000, 0x38478000, 0x3847C000, 
				0x38480000, 0x38484000, 0x38488000, 0x3848C000, 0x38490000, 0x38494000, 0x38498000, 0x3849C000, 0x384A0000, 0x384A4000, 0x384A8000, 0x384AC000, 0x384B0000, 0x384B4000, 0x384B8000, 0x384BC000, 
				0x384C0000, 0x384C4000, 0x384C8000, 0x384CC000, 0x384D0000, 0x384D4000, 0x384D8000, 0x384DC000, 0x384E0000, 0x384E4000, 0x384E8000, 0x384EC000, 0x384F0000, 0x384F4000, 0x384F8000, 0x384FC000, 
				0x38500000, 0x38504000, 0x38508000, 0x3850C000, 0x38510000, 0x38514000, 0x38518000, 0x3851C000, 0x38520000, 0x38524000, 0x38528000, 0x3852C000, 0x38530000, 0x38534000, 0x38538000, 0x3853C000, 
				0x38540000, 0x38544000, 0x38548000, 0x3854C000, 0x38550000, 0x38554000, 0x38558000, 0x3855C000, 0x38560000, 0x38564000, 0x38568000, 0x3856C000, 0x38570000, 0x38574000, 0x38578000, 0x3857C000, 
				0x38580000, 0x38584000, 0x38588000, 0x3858C000, 0x38590000, 0x38594000, 0x38598000, 0x3859C000, 0x385A0000, 0x385A4000, 0x385A8000, 0x385AC000, 0x385B0000, 0x385B4000, 0x385B8000, 0x385BC000, 
				0x385C0000, 0x385C4000, 0x385C8000, 0x385CC000, 0x385D0000, 0x385D4000, 0x385D8000, 0x385DC000, 0x385E0000, 0x385E4000, 0x385E8000, 0x385EC000, 0x385F0000, 0x385F4000, 0x385F8000, 0x385FC000, 
				0x38600000, 0x38604000, 0x38608000, 0x3860C000, 0x38610000, 0x38614000, 0x38618000, 0x3861C000, 0x38620000, 0x38624000, 0x38628000, 0x3862C000, 0x38630000, 0x38634000, 0x38638000, 0x3863C000, 
				0x38640000, 0x38644000, 0x38648000, 0x3864C000, 0x38650000, 0x38654000, 0x38658000, 0x3865C000, 0x38660000, 0x38664000, 0x38668000, 0x3866C000, 0x38670000, 0x38674000, 0x38678000, 0x3867C000, 
				0x38680000, 0x38684000, 0x38688000, 0x3868C000, 0x38690000, 0x38694000, 0x38698000, 0x3869C000, 0x386A0000, 0x386A4000, 0x386A8000, 0x386AC000, 0x386B0000, 0x386B4000, 0x386B8000, 0x386BC000, 
				0x386C0000, 0x386C4000, 0x386C8000, 0x386CC000, 0x386D0000, 0x386D4000, 0x386D8000, 0x386DC000, 0x386E0000, 0x386E4000, 0x386E8000, 0x386EC000, 0x386F0000, 0x386F4000, 0x386F8000, 0x386FC000, 
				0x38700000, 0x38704000, 0x38708000, 0x3870C000, 0x38710000, 0x38714000, 0x38718000, 0x3871C000, 0x38720000, 0x38724000, 0x38728000, 0x3872C000, 0x38730000, 0x38734000, 0x38738000, 0x3873C000, 
				0x38740000, 0x38744000, 0x38748000, 0x3874C000, 0x38750000, 0x38754000, 0x38758000, 0x3875C000, 0x38760000, 0x38764000, 0x38768000, 0x3876C000, 0x38770000, 0x38774000, 0x38778000, 0x3877C000, 
				0x38780000, 0x38784000, 0x38788000, 0x3878C000, 0x38790000, 0x38794000, 0x38798000, 0x3879C000, 0x387A0000, 0x387A4000, 0x387A8000, 0x387AC000, 0x387B0000, 0x387B4000, 0x387B8000, 0x387BC000, 
				0x387C0000, 0x387C4000, 0x387C8000, 0x387CC000, 0x387D0000, 0x387D4000, 0x387D8000, 0x387DC000, 0x387E0000, 0x387E4000, 0x387E8000, 0x387EC000, 0x387F0000, 0x387F4000, 0x387F8000, 0x387FC000, 
				0x38000000, 0x38002000, 0x38004000, 0x38006000, 0x38008000, 0x3800A000, 0x3800C000, 0x3800E000, 0x38010000, 0x38012000, 0x38014000, 0x38016000, 0x38018000, 0x3801A000, 0x3801C000, 0x3801E000, 
				0x38020000, 0x38022000, 0x38024000, 0x38026000, 0x38028000, 0x3802A000, 0x3802C000, 0x3802E000, 0x38030000, 0x38032000, 0x38034000, 0x38036000, 0x38038000, 0x3803A000, 0x3803C000, 0x3803E000, 
				0x38040000, 0x38042000, 0x38044000, 0x38046000, 0x38048000, 0x3804A000, 0x3804C000, 0x3804E000, 0x38050000, 0x38052000, 0x38054000, 0x38056000, 0x38058000, 0x3805A000, 0x3805C000, 0x3805E000, 
				0x38060000, 0x38062000, 0x38064000, 0x38066000, 0x38068000, 0x3806A000, 0x3806C000, 0x3806E000, 0x38070000, 0x38072000, 0x38074000, 0x38076000, 0x38078000, 0x3807A000, 0x3807C000, 0x3807E000, 
				0x38080000, 0x38082000, 0x38084000, 0x38086000, 0x38088000, 0x3808A000, 0x3808C000, 0x3808E000, 0x38090000, 0x38092000, 0x38094000, 0x38096000, 0x38098000, 0x3809A000, 0x3809C000, 0x3809E000, 
				0x380A0000, 0x380A2000, 0x380A4000, 0x380A6000, 0x380A8000, 0x380AA000, 0x380AC000, 0x380AE000, 0x380B0000, 0x380B2000, 0x380B4000, 0x380B6000, 0x380B8000, 0x380BA000, 0x380BC000, 0x380BE000, 
				0x380C0000, 0x380C2000, 0x380C4000, 0x380C6000, 0x380C8000, 0x380CA000, 0x380CC000, 0x380CE000, 0x380D0000, 0x380D2000, 0x380D4000, 0x380D6000, 0x380D8000, 0x380DA000, 0x380DC000, 0x380DE000, 
				0x380E0000, 0x380E2000, 0x380E4000, 0x380E6000, 0x380E8000, 0x380EA000, 0x380EC000, 0x380EE000, 0x380F0000, 0x380F2000, 0x380F4000, 0x380F6000, 0x380F8000, 0x380FA000, 0x380FC000, 0x380FE000, 
				0x38100000, 0x38102000, 0x38104000, 0x38106000, 0x38108000, 0x3810A000, 0x3810C000, 0x3810E000, 0x38110000, 0x38112000, 0x38114000, 0x38116000, 0x38118000, 0x3811A000, 0x3811C000, 0x3811E000, 
				0x38120000, 0x38122000, 0x38124000, 0x38126000, 0x38128000, 0x3812A000, 0x3812C000, 0x3812E000, 0x38130000, 0x38132000, 0x38134000, 0x38136000, 0x38138000, 0x3813A000, 0x3813C000, 0x3813E000, 
				0x38140000, 0x38142000, 0x38144000, 0x38146000, 0x38148000, 0x3814A000, 0x3814C000, 0x3814E000, 0x38150000, 0x38152000, 0x38154000, 0x38156000, 0x38158000, 0x3815A000, 0x3815C000, 0x3815E000, 
				0x38160000, 0x38162000, 0x38164000, 0x38166000, 0x38168000, 0x3816A000, 0x3816C000, 0x3816E000, 0x38170000, 0x38172000, 0x38174000, 0x38176000, 0x38178000, 0x3817A000, 0x3817C000, 0x3817E000, 
				0x38180000, 0x38182000, 0x38184000, 0x38186000, 0x38188000, 0x3818A000, 0x3818C000, 0x3818E000, 0x38190000, 0x38192000, 0x38194000, 0x38196000, 0x38198000, 0x3819A000, 0x3819C000, 0x3819E000, 
				0x381A0000, 0x381A2000, 0x381A4000, 0x381A6000, 0x381A8000, 0x381AA000, 0x381AC000, 0x381AE000, 0x381B0000, 0x381B2000, 0x381B4000, 0x381B6000, 0x381B8000, 0x381BA000, 0x381BC000, 0x381BE000, 
				0x381C0000, 0x381C2000, 0x381C4000, 0x381C6000, 0x381C8000, 0x381CA000, 0x381CC000, 0x381CE000, 0x381D0000, 0x381D2000, 0x381D4000, 0x381D6000, 0x381D8000, 0x381DA000, 0x381DC000, 0x381DE000, 
				0x381E0000, 0x381E2000, 0x381E4000, 0x381E6000, 0x381E8000, 0x381EA000, 0x381EC000, 0x381EE000, 0x381F0000, 0x381F2000, 0x381F4000, 0x381F6000, 0x381F8000, 0x381FA000, 0x381FC000, 0x381FE000, 
				0x38200000, 0x38202000, 0x38204000, 0x38206000, 0x38208000, 0x3820A000, 0x3820C000, 0x3820E000, 0x38210000, 0x38212000, 0x38214000, 0x38216000, 0x38218000, 0x3821A000, 0x3821C000, 0x3821E000, 
				0x38220000, 0x38222000, 0x38224000, 0x38226000, 0x38228000, 0x3822A000, 0x3822C000, 0x3822E000, 0x38230000, 0x38232000, 0x38234000, 0x38236000, 0x38238000, 0x3823A000, 0x3823C000, 0x3823E000, 
				0x38240000, 0x38242000, 0x38244000, 0x38246000, 0x38248000, 0x3824A000, 0x3824C000, 0x3824E000, 0x38250000, 0x38252000, 0x38254000, 0x38256000, 0x38258000, 0x3825A000, 0x3825C000, 0x3825E000, 
				0x38260000, 0x38262000, 0x38264000, 0x38266000, 0x38268000, 0x3826A000, 0x3826C000, 0x3826E000, 0x38270000, 0x38272000, 0x38274000, 0x38276000, 0x38278000, 0x3827A000, 0x3827C000, 0x3827E000, 
				0x38280000, 0x38282000, 0x38284000, 0x38286000, 0x38288000, 0x3828A000, 0x3828C000, 0x3828E000, 0x38290000, 0x38292000, 0x38294000, 0x38296000, 0x38298000, 0x3829A000, 0x3829C000, 0x3829E000, 
				0x382A0000, 0x382A2000, 0x382A4000, 0x382A6000, 0x382A8000, 0x382AA000, 0x382AC000, 0x382AE000, 0x382B0000, 0x382B2000, 0x382B4000, 0x382B6000, 0x382B8000, 0x382BA000, 0x382BC000, 0x382BE000, 
				0x382C0000, 0x382C2000, 0x382C4000, 0x382C6000, 0x382C8000, 0x382CA000, 0x382CC000, 0x382CE000, 0x382D0000, 0x382D2000, 0x382D4000, 0x382D6000, 0x382D8000, 0x382DA000, 0x382DC000, 0x382DE000, 
				0x382E0000, 0x382E2000, 0x382E4000, 0x382E6000, 0x382E8000, 0x382EA000, 0x382EC000, 0x382EE000, 0x382F0000, 0x382F2000, 0x382F4000, 0x382F6000, 0x382F8000, 0x382FA000, 0x382FC000, 0x382FE000, 
				0x38300000, 0x38302000, 0x38304000, 0x38306000, 0x38308000, 0x3830A000, 0x3830C000, 0x3830E000, 0x38310000, 0x38312000, 0x38314000, 0x38316000, 0x38318000, 0x3831A000, 0x3831C000, 0x3831E000, 
				0x38320000, 0x38322000, 0x38324000, 0x38326000, 0x38328000, 0x3832A000, 0x3832C000, 0x3832E000, 0x38330000, 0x38332000, 0x38334000, 0x38336000, 0x38338000, 0x3833A000, 0x3833C000, 0x3833E000, 
				0x38340000, 0x38342000, 0x38344000, 0x38346000, 0x38348000, 0x3834A000, 0x3834C000, 0x3834E000, 0x38350000, 0x38352000, 0x38354000, 0x38356000, 0x38358000, 0x3835A000, 0x3835C000, 0x3835E000, 
				0x38360000, 0x38362000, 0x38364000, 0x38366000, 0x38368000, 0x3836A000, 0x3836C000, 0x3836E000, 0x38370000, 0x38372000, 0x38374000, 0x38376000, 0x38378000, 0x3837A000, 0x3837C000, 0x3837E000, 
				0x38380000, 0x38382000, 0x38384000, 0x38386000, 0x38388000, 0x3838A000, 0x3838C000, 0x3838E000, 0x38390000, 0x38392000, 0x38394000, 0x38396000, 0x38398000, 0x3839A000, 0x3839C000, 0x3839E000, 
				0x383A0000, 0x383A2000, 0x383A4000, 0x383A6000, 0x383A8000, 0x383AA000, 0x383AC000, 0x383AE000, 0x383B0000, 0x383B2000, 0x383B4000, 0x383B6000, 0x383B8000, 0x383BA000, 0x383BC000, 0x383BE000, 
				0x383C0000, 0x383C2000, 0x383C4000, 0x383C6000, 0x383C8000, 0x383CA000, 0x383CC000, 0x383CE000, 0x383D0000, 0x383D2000, 0x383D4000, 0x383D6000, 0x383D8000, 0x383DA000, 0x383DC000, 0x383DE000, 
				0x383E0000, 0x383E2000, 0x383E4000, 0x383E6000, 0x383E8000, 0x383EA000, 0x383EC000, 0x383EE000, 0x383F0000, 0x383F2000, 0x383F4000, 0x383F6000, 0x383F8000, 0x383FA000, 0x383FC000, 0x383FE000, 
				0x38400000, 0x38402000, 0x38404000, 0x38406000, 0x38408000, 0x3840A000, 0x3840C000, 0x3840E000, 0x38410000, 0x38412000, 0x38414000, 0x38416000, 0x38418000, 0x3841A000, 0x3841C000, 0x3841E000, 
				0x38420000, 0x38422000, 0x38424000, 0x38426000, 0x38428000, 0x3842A000, 0x3842C000, 0x3842E000, 0x38430000, 0x38432000, 0x38434000, 0x38436000, 0x38438000, 0x3843A000, 0x3843C000, 0x3843E000, 
				0x38440000, 0x38442000, 0x38444000, 0x38446000, 0x38448000, 0x3844A000, 0x3844C000, 0x3844E000, 0x38450000, 0x38452000, 0x38454000, 0x38456000, 0x38458000, 0x3845A000, 0x3845C000, 0x3845E000, 
				0x38460000, 0x38462000, 0x38464000, 0x38466000, 0x38468000, 0x3846A000, 0x3846C000, 0x3846E000, 0x38470000, 0x38472000, 0x38474000, 0x38476000, 0x38478000, 0x3847A000, 0x3847C000, 0x3847E000, 
				0x38480000, 0x38482000, 0x38484000, 0x38486000, 0x38488000, 0x3848A000, 0x3848C000, 0x3848E000, 0x38490000, 0x38492000, 0x38494000, 0x38496000, 0x38498000, 0x3849A000, 0x3849C000, 0x3849E000, 
				0x384A0000, 0x384A2000, 0x384A4000, 0x384A6000, 0x384A8000, 0x384AA000, 0x384AC000, 0x384AE000, 0x384B0000, 0x384B2000, 0x384B4000, 0x384B6000, 0x384B8000, 0x384BA000, 0x384BC000, 0x384BE000, 
				0x384C0000, 0x384C2000, 0x384C4000, 0x384C6000, 0x384C8000, 0x384CA000, 0x384CC000, 0x384CE000, 0x384D0000, 0x384D2000, 0x384D4000, 0x384D6000, 0x384D8000, 0x384DA000, 0x384DC000, 0x384DE000, 
				0x384E0000, 0x384E2000, 0x384E4000, 0x384E6000, 0x384E8000, 0x384EA000, 0x384EC000, 0x384EE000, 0x384F0000, 0x384F2000, 0x384F4000, 0x384F6000, 0x384F8000, 0x384FA000, 0x384FC000, 0x384FE000, 
				0x38500000, 0x38502000, 0x38504000, 0x38506000, 0x38508000, 0x3850A000, 0x3850C000, 0x3850E000, 0x38510000, 0x38512000, 0x38514000, 0x38516000, 0x38518000, 0x3851A000, 0x3851C000, 0x3851E000, 
				0x38520000, 0x38522000, 0x38524000, 0x38526000, 0x38528000, 0x3852A000, 0x3852C000, 0x3852E000, 0x38530000, 0x38532000, 0x38534000, 0x38536000, 0x38538000, 0x3853A000, 0x3853C000, 0x3853E000, 
				0x38540000, 0x38542000, 0x38544000, 0x38546000, 0x38548000, 0x3854A000, 0x3854C000, 0x3854E000, 0x38550000, 0x38552000, 0x38554000, 0x38556000, 0x38558000, 0x3855A000, 0x3855C000, 0x3855E000, 
				0x38560000, 0x38562000, 0x38564000, 0x38566000, 0x38568000, 0x3856A000, 0x3856C000, 0x3856E000, 0x38570000, 0x38572000, 0x38574000, 0x38576000, 0x38578000, 0x3857A000, 0x3857C000, 0x3857E000, 
				0x38580000, 0x38582000, 0x38584000, 0x38586000, 0x38588000, 0x3858A000, 0x3858C000, 0x3858E000, 0x38590000, 0x38592000, 0x38594000, 0x38596000, 0x38598000, 0x3859A000, 0x3859C000, 0x3859E000, 
				0x385A0000, 0x385A2000, 0x385A4000, 0x385A6000, 0x385A8000, 0x385AA000, 0x385AC000, 0x385AE000, 0x385B0000, 0x385B2000, 0x385B4000, 0x385B6000, 0x385B8000, 0x385BA000, 0x385BC000, 0x385BE000, 
				0x385C0000, 0x385C2000, 0x385C4000, 0x385C6000, 0x385C8000, 0x385CA000, 0x385CC000, 0x385CE000, 0x385D0000, 0x385D2000, 0x385D4000, 0x385D6000, 0x385D8000, 0x385DA000, 0x385DC000, 0x385DE000, 
				0x385E0000, 0x385E2000, 0x385E4000, 0x385E6000, 0x385E8000, 0x385EA000, 0x385EC000, 0x385EE000, 0x385F0000, 0x385F2000, 0x385F4000, 0x385F6000, 0x385F8000, 0x385FA000, 0x385FC000, 0x385FE000, 
				0x38600000, 0x38602000, 0x38604000, 0x38606000, 0x38608000, 0x3860A000, 0x3860C000, 0x3860E000, 0x38610000, 0x38612000, 0x38614000, 0x38616000, 0x38618000, 0x3861A000, 0x3861C000, 0x3861E000, 
				0x38620000, 0x38622000, 0x38624000, 0x38626000, 0x38628000, 0x3862A000, 0x3862C000, 0x3862E000, 0x38630000, 0x38632000, 0x38634000, 0x38636000, 0x38638000, 0x3863A000, 0x3863C000, 0x3863E000, 
				0x38640000, 0x38642000, 0x38644000, 0x38646000, 0x38648000, 0x3864A000, 0x3864C000, 0x3864E000, 0x38650000, 0x38652000, 0x38654000, 0x38656000, 0x38658000, 0x3865A000, 0x3865C000, 0x3865E000, 
				0x38660000, 0x38662000, 0x38664000, 0x38666000, 0x38668000, 0x3866A000, 0x3866C000, 0x3866E000, 0x38670000, 0x38672000, 0x38674000, 0x38676000, 0x38678000, 0x3867A000, 0x3867C000, 0x3867E000, 
				0x38680000, 0x38682000, 0x38684000, 0x38686000, 0x38688000, 0x3868A000, 0x3868C000, 0x3868E000, 0x38690000, 0x38692000, 0x38694000, 0x38696000, 0x38698000, 0x3869A000, 0x3869C000, 0x3869E000, 
				0x386A0000, 0x386A2000, 0x386A4000, 0x386A6000, 0x386A8000, 0x386AA000, 0x386AC000, 0x386AE000, 0x386B0000, 0x386B2000, 0x386B4000, 0x386B6000, 0x386B8000, 0x386BA000, 0x386BC000, 0x386BE000, 
				0x386C0000, 0x386C2000, 0x386C4000, 0x386C6000, 0x386C8000, 0x386CA000, 0x386CC000, 0x386CE000, 0x386D0000, 0x386D2000, 0x386D4000, 0x386D6000, 0x386D8000, 0x386DA000, 0x386DC000, 0x386DE000, 
				0x386E0000, 0x386E2000, 0x386E4000, 0x386E6000, 0x386E8000, 0x386EA000, 0x386EC000, 0x386EE000, 0x386F0000, 0x386F2000, 0x386F4000, 0x386F6000, 0x386F8000, 0x386FA000, 0x386FC000, 0x386FE000, 
				0x38700000, 0x38702000, 0x38704000, 0x38706000, 0x38708000, 0x3870A000, 0x3870C000, 0x3870E000, 0x38710000, 0x38712000, 0x38714000, 0x38716000, 0x38718000, 0x3871A000, 0x3871C000, 0x3871E000, 
				0x38720000, 0x38722000, 0x38724000, 0x38726000, 0x38728000, 0x3872A000, 0x3872C000, 0x3872E000, 0x38730000, 0x38732000, 0x38734000, 0x38736000, 0x38738000, 0x3873A000, 0x3873C000, 0x3873E000, 
				0x38740000, 0x38742000, 0x38744000, 0x38746000, 0x38748000, 0x3874A000, 0x3874C000, 0x3874E000, 0x38750000, 0x38752000, 0x38754000, 0x38756000, 0x38758000, 0x3875A000, 0x3875C000, 0x3875E000, 
				0x38760000, 0x38762000, 0x38764000, 0x38766000, 0x38768000, 0x3876A000, 0x3876C000, 0x3876E000, 0x38770000, 0x38772000, 0x38774000, 0x38776000, 0x38778000, 0x3877A000, 0x3877C000, 0x3877E000, 
				0x38780000, 0x38782000, 0x38784000, 0x38786000, 0x38788000, 0x3878A000, 0x3878C000, 0x3878E000, 0x38790000, 0x38792000, 0x38794000, 0x38796000, 0x38798000, 0x3879A000, 0x3879C000, 0x3879E000, 
				0x387A0000, 0x387A2000, 0x387A4000, 0x387A6000, 0x387A8000, 0x387AA000, 0x387AC000, 0x387AE000, 0x387B0000, 0x387B2000, 0x387B4000, 0x387B6000, 0x387B8000, 0x387BA000, 0x387BC000, 0x387BE000, 
				0x387C0000, 0x387C2000, 0x387C4000, 0x387C6000, 0x387C8000, 0x387CA000, 0x387CC000, 0x387CE000, 0x387D0000, 0x387D2000, 0x387D4000, 0x387D6000, 0x387D8000, 0x387DA000, 0x387DC000, 0x387DE000, 
				0x387E0000, 0x387E2000, 0x387E4000, 0x387E6000, 0x387E8000, 0x387EA000, 0x387EC000, 0x387EE000, 0x387F0000, 0x387F2000, 0x387F4000, 0x387F6000, 0x387F8000, 0x387FA000, 0x387FC000, 0x387FE000 };
			static const uint32 exponent_table[64] = { 
				0x00000000, 0x00800000, 0x01000000, 0x01800000, 0x02000000, 0x02800000, 0x03000000, 0x03800000, 0x04000000, 0x04800000, 0x05000000, 0x05800000, 0x06000000, 0x06800000, 0x07000000, 0x07800000, 
				0x08000000, 0x08800000, 0x09000000, 0x09800000, 0x0A000000, 0x0A800000, 0x0B000000, 0x0B800000, 0x0C000000, 0x0C800000, 0x0D000000, 0x0D800000, 0x0E000000, 0x0E800000, 0x0F000000, 0x47800000, 
				0x80000000, 0x80800000, 0x81000000, 0x81800000, 0x82000000, 0x82800000, 0x83000000, 0x83800000, 0x84000000, 0x84800000, 0x85000000, 0x85800000, 0x86000000, 0x86800000, 0x87000000, 0x87800000, 
				0x88000000, 0x88800000, 0x89000000, 0x89800000, 0x8A000000, 0x8A800000, 0x8B000000, 0x8B800000, 0x8C000000, 0x8C800000, 0x8D000000, 0x8D800000, 0x8E000000, 0x8E800000, 0x8F000000, 0xC7800000 };
			static const unsigned short offset_table[64] = { 
				   0, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 
				   0, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024 };
			uint32 bits = mantissa_table[offset_table[value>>10]+(value&0x3FF)] + exponent_table[value>>10];
//			uint32 bits = mantissa_table[(((value&0x7C00)!=0)<<10)+(value&0x3FF)] + exponent_table[value>>10];
//			return *reinterpret_cast<float*>(&bits);
			float out;
			std::memcpy(&out, &bits, sizeof(float));
			return out;
		}
/*
		/// Convert half-precision to non-IEEE single-precision.
		/// \param value binary representation of half-precision value
		/// \return single-precision value
		inline float half2float_impl(uint16 value, booltype<false>)
		{
			int exp = value & 0x7C00;
			float out;
			if(exp == 0x7C00)
			{
				if(value & 0x3FF)
					out = std::numeric_limits<float>::has_quiet_NaN ? std::numeric_limits<float>::quiet_NaN() : 0.0f;
				else
					out = std::numeric_limits<float>::has_infinity ? std::numeric_limits<float>::infinity() : 
						std::numeric_limits<float>::max();
			}
			else
			{
				exp >>= 10;
				unsigned int mant = value & 0x3FF;
				if(!exp)
				{
					if(mant)
						for(mant<<=1; mant<0x400; mant<<=1)
							--exp;
				}
				else
					mant |= 0x400;
				out = std::ldexp(static_cast<float>(mant), exp-25);
			}
			return (value&0x8000) ? -out : out;
		}

		/// Convert half-precision to single-precision.
		/// \param value binary representation of half-precision value
		/// \return single-precision value
		inline float half2float(uint16 value)
		{
			return half2float_impl(value, booltype<std::numeric_limits<float>::is_iec559>());
		}
*/
		/// Convert half-precision floating point to integer.
		/// \tparam T type to convert to (integer type with at least 16 bits precision, excluding any implicit sign bits)
		/// \tparam R rounding mode to use, `std::round_indeterminate` for fastest rounding
		/// \param value binary representation of half-precision value
		/// \return integral value
		template<typename T,std::float_round_style R> T half2int(uint16 value)
		{
		#if HALF_ENABLE_CPP11_STATIC_ASSERT
			static_assert(std::numeric_limits<T>::digits>=16, "half only convertible to integers with at least 16 bits precision (exluding sign)");
		#endif
			unsigned int e = value & 0x7C00;
			if(e == 0x7C00)
				return (value&0x8000) ? std::numeric_limits<T>::min() : std::numeric_limits<T>::max();
			if(e < 0x3800)
				return T();
			T m = (value&0x3FF) | 0x400;
			e >>= 10;
			if(e < 25)
			{
				if(R == std::round_indeterminate || R == std::round_toward_zero)
					m >>= 25 - e;
				else
				{
					T frac = m & ((1<<(25-e))-1);
					m >>= 25 - e;
					if(R == std::round_to_nearest)
						m += frac >> (24-e);
					else if(R == std::round_toward_infinity)
						m += ~(value>>15) & (frac!=0);
					else if(R == std::round_toward_neg_infinity)
						m += (value>>15) & (frac!=0);
				}
			}
			else
				m <<= e - 25;
			return (value&0x8000) ? -m : m;
		}

		/// Cast to half.
		/// \param arg value to cast
		/// \return \a arg converted to half-precision (via single-precision)
		template<typename U,std::float_round_style R> half half_caster<half,U,R>::cast(const U &arg)
		{
			return half(float2half<R>(static_cast<float>(arg)), true);
		}

		/// Cast from half.
		/// \param arg expression to cast
		/// \return \a arg converted to destination type (via single-precision)
		template<typename T,std::float_round_style R> template<typename E> T half_caster<T,half,R>::cast(const half_expr<E> &arg)
		{
			return static_cast<T>(static_cast<float>(arg));
		}

		/// Cast between halfs.
		/// \param arg expression to cast
		/// \return unchanged value
		template<std::float_round_style R> template<typename E> half half_caster<half,half,R>::cast(const half_expr<E> &arg)
		{
			return arg;
		}


		half noarg_functions::nanh(const char*) { return half(0x7FFF, true); }

		template<typename charT,typename traits> std::basic_istream<charT,traits>& noarg_functions::read(std::basic_istream<charT,traits> &in, half &h)
		{
			float f;
			if(in >> f)
				h = f;
			return in;
		}

		template<typename T> half unary_functions<T>::negate(half arg) { return half(arg.data_^0x8000, true); }

		template<typename T> template<typename charT,typename traits> std::basic_ostream<charT,traits>& 
			unary_functions<T>::write(std::basic_ostream<charT,traits> &out, float arg) { return out << arg; }

		template<typename T> half unary_functions<T>::fabs(half arg) { return half(arg.data_&0x7FFF, true); }

		template<typename T> float_expr unary_functions<T>::exp(float arg) { return float_expr(std::exp(arg)); }

		template<typename T> float_expr unary_functions<T>::log(float arg) { return float_expr(std::log(arg)); }

		template<typename T> float_expr unary_functions<T>::log10(float arg) { return float_expr(std::log10(arg)); }

		template<typename T> float_expr unary_functions<T>::sqrt(float arg) { return float_expr(std::sqrt(arg)); }

		template<typename T> float_expr unary_functions<T>::sin(float arg) { return float_expr(std::sin(arg)); }

		template<typename T> float_expr unary_functions<T>::cos(float arg) { return float_expr(std::cos(arg)); }

		template<typename T> float_expr unary_functions<T>::tan(float arg) { return float_expr(std::tan(arg)); }

		template<typename T> float_expr unary_functions<T>::asin(float arg) { return float_expr(std::asin(arg)); }

		template<typename T> float_expr unary_functions<T>::acos(float arg) { return float_expr(std::acos(arg)); }

		template<typename T> float_expr unary_functions<T>::atan(float arg) { return float_expr(std::atan(arg)); }

		template<typename T> float_expr unary_functions<T>::sinh(float arg) { return float_expr(std::sinh(arg)); }

		template<typename T> float_expr unary_functions<T>::cosh(float arg) { return float_expr(std::cosh(arg)); }

		template<typename T> float_expr unary_functions<T>::tanh(float arg) { return float_expr(std::tanh(arg)); }

		template<typename T> half unary_functions<T>::floor(half arg)
		{
			unsigned int e = arg.data_ & 0x7C00;
			if(e > 0x6000)
				return arg;
			if(e < 0x3C00)
				return half((arg.data_&0x8000)|(0x3C00&-static_cast<uint16>((arg.data_>>15)&((arg.data_&0x7FFF)!=0))), true);
			e = 25 - (e>>10);
			unsigned int mask = (1<<e) - 1;
			return half((arg.data_&~mask)+(((arg.data_>>15)&((arg.data_&mask)!=0))<<e), true);
		}

		template<typename T> half unary_functions<T>::ceil(half arg)
		{
			unsigned int e = arg.data_ & 0x7C00;
			if(e > 0x6000)
				return arg;
			if(e < 0x3C00)
				return half((arg.data_&0x8000)|(0x3C00&-static_cast<uint16>(~(arg.data_>>15)&((arg.data_&0x7FFF)!=0))), true);
			e = 25 - (e>>10);
			unsigned int mask = (1<<e) - 1;
			return half((arg.data_&~mask)+((~(arg.data_>>15)&((arg.data_&mask)!=0))<<e), true);
		}

		template<typename T> half unary_functions<T>::trunc(half arg)
		{
			unsigned int e = arg.data_ & 0x7C00;
			if(e > 0x6000)
				return arg;
			if(e < 0x3C00)
				return half(arg.data_&0x8000, true);
			return half(arg.data_&~((1<<(25-(e>>10)))-1), true);
		}

		template<typename T> half unary_functions<T>::round(half arg)
		{
			unsigned int e = arg.data_ & 0x7C00;
			if(e > 0x6000)
				return arg;
			if(e < 0x3C00)
				return half((arg.data_&0x8000)|(0x3C00&-static_cast<uint16>((arg.data_&0x7FFF)>=0x3800)), true);
			e >>= 10;
			return half((arg.data_+(1<<(24-e)))&~((1<<(25-e))-1), true);
		}

		template<typename T> long unary_functions<T>::lround(half arg)
		{
			return detail::half2int<long,std::round_to_nearest>(arg.data_);
		}

		template<typename T> long long unary_functions<T>::llround(half arg)
		{
			return detail::half2int<long long,std::round_to_nearest>(arg.data_);
		}

		template<typename T> half unary_functions<T>::frexp(half arg, int *exp)
		{
			int e = arg.data_ & 0x7C00;
			if(e == 0x7C00 || !(arg.data_&0x7FFF))
				return *exp = 0, arg;
			unsigned int m = arg.data_ & 0x3FF;
			e >>= 10;
			if(!e)
			{
				for(m<<=1; m<0x400; m<<=1)
					--e;
				m &= 0x3FF;
			}
			*exp = e - 14;
			return half((arg.data_&0x8000) | 0x3800 | m, true);
		}

		template<typename T> half unary_functions<T>::modf(half x, half *iptr)
		{
			*iptr = x;
			unsigned int e = x.data_ & 0x7C00;
			if(e > 0x6000)
				return (e==0x7C00&&(x.data_&0x3FF)) ? x : half(x.data_&0x8000, true);
			if(e < 0x3C00)
				return iptr->data_ &= 0x8000, x;
			e >>= 10;
			unsigned int mask = (1<<(25-e)) - 1;
			unsigned int m = x.data_ & mask;
			iptr->data_ &= ~mask;
			if(!m)
				return half(x.data_&0x8000, true);
			for(; m<0x400; m<<=1)
				--e;
			return half((x.data_&0x8000) | (e<<10) | (m&0x3FF), true);
		}

		template<typename T> half unary_functions<T>::scalbln(half x, long exp)
		{
			long e = x.data_ & 0x7C00;
			if(e == 0x7C00)
				return x;
			unsigned int m = x.data_ & 0x3FF;
			if(e >>= 10)
				m |= 0x400;
			else
			{
				if(!m)
					return x;
				for(m<<=1; m<0x400; m<<=1)
					--e;
			}
			e += exp;
			unsigned int sign = x.data_ & 0x8000;
			return (e>30) ? half(sign|0x7C00, true) : half((e>0) ? (sign|(e<<10)|(m&0x3FF)) : ((e<-9) ? sign : (sign|(m>>(1-e)))), true);
		}

		template<typename T> int unary_functions<T>::ilogb(half arg)
		{
			if(!(arg.data_&0x7FFF))
				return FP_ILOGB0;
			int e = arg.data_ & 0x7C00;
			if(e == 0x7C00)
				return (arg.data_&0x3FF) ? FP_ILOGBNAN : INT_MAX;
			e >>= 10;
			if(!e)
				for(unsigned int m=(arg.data_&0x3FF)<<1; m<0x400; m<<=1)
					--e;
			return e - 15;
		}

		template<typename T> half unary_functions<T>::logb(half arg)
		{
			if(!(arg.data_&0x7FFF))
				return half(0xFC00, true);
			int e = arg.data_ & 0x7C00;
			if(e == 0x7C00)
				return (arg.data_&0x3FF) ? arg : half(0x7C00, true);
			e >>= 10;
			if(!e)
				for(unsigned int m=(arg.data_&0x3FF)<<1; m<0x400; m<<=1)
					--e;
			return half(static_cast<float>(e-15));
		}

		template<typename T> half unary_functions<T>::nexttoward(half from, long double to)
		{
			if(isnan(from))
				return from;
			long double lfrom = static_cast<long double>(from);
			if(builtin_isnan(to) || lfrom == to)
				return half(static_cast<float>(to));
			if(!(from.data_&0x7FFF))
				return half((static_cast<detail::uint16>(builtin_signbit(to))<<15)+1, true);
			return half(from.data_+(((from.data_>>15)^static_cast<uint16>(lfrom<to))<<1)-1, true);
		}

		template<typename T> int unary_functions<T>::fpclassify(half arg)
		{
			unsigned int e = arg.data_ & 0x7C00;
			if(e == 0)
				return (arg.data_&0x3FF) ? FP_SUBNORMAL : FP_ZERO;
			if(e == 0x7C00)
				return (arg.data_&0x3FF) ? FP_NAN : FP_INFINITE;
			return FP_NORMAL;
		}

		template<typename T> bool unary_functions<T>::isfinite(half arg) { return (arg.data_&0x7C00) != 0x7C00; }

		template<typename T> bool unary_functions<T>::isinf(half arg) { return (arg.data_&0x7FFF) == 0x7C00; }

		template<typename T> bool unary_functions<T>::isnan(half arg) { return (arg.data_&0x7FFF) > 0x7C00; }

		template<typename T> bool unary_functions<T>::isnormal(half arg) { return ((arg.data_&0x7C00)!=0) & ((arg.data_&0x7C00)!=0x7C00); }
		
		template<typename T> bool unary_functions<T>::signbit(half arg) { return (arg.data_&0x8000) != 0; }
#if HALF_ENABLE_CPP11_CMATH
		template<typename T> float_expr unary_functions<T>::exp2(float arg) { return float_expr(std::exp2(arg)); }

		template<typename T> float_expr unary_functions<T>::expm1(float arg) { return float_expr(std::expm1(arg)); }

		template<typename T> float_expr unary_functions<T>::log1p(float arg) { return float_expr(std::log1p(arg)); }

		template<typename T> float_expr unary_functions<T>::log2(float arg) { return float_expr(std::log2(arg)); }

		template<typename T> float_expr unary_functions<T>::cbrt(float arg) { return float_expr(std::cbrt(arg)); }

		template<typename T> float_expr unary_functions<T>::asinh(float arg) { return float_expr(std::asinh(arg)); }

		template<typename T> float_expr unary_functions<T>::acosh(float arg) { return float_expr(std::acosh(arg)); }

		template<typename T> float_expr unary_functions<T>::atanh(float arg) { return float_expr(std::atanh(arg)); }

		template<typename T> float_expr unary_functions<T>::erf(float arg) { return float_expr(std::erf(arg)); }

		template<typename T> float_expr unary_functions<T>::erfc(float arg) { return float_expr(std::erfc(arg)); }

		template<typename T> float_expr unary_functions<T>::lgamma(float arg) { return float_expr(std::lgamma(arg)); }

		template<typename T> float_expr unary_functions<T>::tgamma(float arg) { return float_expr(std::tgamma(arg)); }

		template<typename T> float_expr unary_functions<T>::nearbyint(float arg) { return float_expr(std::nearbyint(arg)); }

		template<typename T> float_expr unary_functions<T>::rint(float arg) { return float_expr(std::rint(arg)); }

		template<typename T> long unary_functions<T>::lrint(float arg) { return float_expr(std::lrint(arg)); }

		template<typename T> long long unary_functions<T>::llrint(float arg) { return float_expr(std::llrint(arg)); }
#endif

		template<typename T,typename U> float_expr binary_functions<T,U>::plus(float x, float y) { return float_expr(x+y); }

		template<typename T,typename U> float_expr binary_functions<T,U>::minus(float x, float y) { return float_expr(x-y); }

		template<typename T,typename U> float_expr binary_functions<T,U>::multiplies(float x, float y) { return float_expr(x*y); }

		template<typename T,typename U> float_expr binary_functions<T,U>::divides(float x, float y) { return float_expr(x/y); }

		template<typename T,typename U> half binary_functions<T,U>::fmin(half x, half y) { return std::min(x, y); }

		template<typename T,typename U> half binary_functions<T,U>::fmax(half x, half y) { return std::max(x, y); }

		template<typename T,typename U> float_expr binary_functions<T,U>::fmod(float x, float y) { return float_expr(std::fmod(x, y)); }

		template<typename T,typename U> float_expr binary_functions<T,U>::fdim(float x, float y) { return float_expr(std::max(x-y, 0.0f)); }

		template<typename T,typename U> float_expr binary_functions<T,U>::pow(float base, float exp) { return float_expr(std::pow(base, exp)); }

		template<typename T,typename U> float_expr binary_functions<T,U>::atan2(float x, float y) { return float_expr(std::atan2(x, y)); }

		template<typename T,typename U> half binary_functions<T,U>::nextafter(half from, half to)
		{
			uint16 fabs = from.data_ & 0x7FFF, tabs = to.data_ & 0x7FFF;
			if(fabs > 0x7C00)
				return from;
			if(tabs > 0x7C00 || from.data_==to.data_ || !(fabs|tabs))
				return to;
			if(!fabs)
				return half((to.data_&0x8000)+1, true);
			bool lt = (signbit(from) ? (static_cast<int17>(0x8000)-from.data_) : static_cast<int17>(from.data_)) < 
				(signbit(to) ? (static_cast<int17>(0x8000)-to.data_) : static_cast<int17>(to.data_));
			return half(from.data_+(((from.data_>>15)^static_cast<uint16>(lt))<<1)-1, true);
		}

		template<typename T,typename U> half binary_functions<T,U>::copysign(half x, half y) { return half(x.data_^((x.data_^y.data_)&0x8000), true); }

		template<typename T,typename U> bool binary_functions<T,U>::isequal(half x, half y)
		{
			return (x.data_==y.data_ || !((x.data_|y.data_)&0x7FFF)) && !isnan(x);
		}

		template<typename T,typename U> bool binary_functions<T,U>::isnotequal(half x, half y)
		{
			return (x.data_!=y.data_ && ((x.data_|y.data_)&0x7FFF)) || isnan(x);
		}

		template<typename T,typename U> bool binary_functions<T,U>::isgreater(half x, half y)
		{
			if(isnan(x) || isnan(y))
				return false;
			return (signbit(x) ? (static_cast<int17>(0x8000)-x.data_) : static_cast<int17>(x.data_)) > 
				(signbit(y) ? (static_cast<int17>(0x8000)-y.data_) : static_cast<int17>(y.data_));
		}

		template<typename T,typename U> bool binary_functions<T,U>::isgreaterequal(half x, half y)
		{
			if(isnan(x) || isnan(y))
				return false;
			return (signbit(x) ? (static_cast<int17>(0x8000)-x.data_) : static_cast<int17>(x.data_)) >= 
				(signbit(y) ? (static_cast<int17>(0x8000)-y.data_) : static_cast<int17>(y.data_));
		}

		template<typename T,typename U> bool binary_functions<T,U>::isless(half x, half y)
		{
			if(isnan(x) || isnan(y))
				return false;
			return (signbit(x) ? (static_cast<int17>(0x8000)-x.data_) : static_cast<int17>(x.data_)) < 
				(signbit(y) ? (static_cast<int17>(0x8000)-y.data_) : static_cast<int17>(y.data_));
		}

		template<typename T,typename U> bool binary_functions<T,U>::islessequal(half x, half y)
		{
			if(isnan(x) || isnan(y))
				return false;
			return (signbit(x) ? (static_cast<int17>(0x8000)-x.data_) : static_cast<int17>(x.data_)) <= 
				(signbit(y) ? (static_cast<int17>(0x8000)-y.data_) : static_cast<int17>(y.data_));
		}

		template<typename T,typename U> bool binary_functions<T,U>::islessgreater(half x, half y) { return isless(x, y) || isgreater(x, y); }

		template<typename T,typename U> bool binary_functions<T,U>::isunordered(half x, half y) { return isnan(x) || isnan(y); }
#if HALF_ENABLE_CPP11_CMATH
		template<typename T,typename U> float_expr binary_functions<T,U>::remainder(float x, float y) { return float_expr(std::remainder(x, y)); }

		template<typename T,typename U> float_expr binary_functions<T,U>::remquo(float x, float y, int *quo) { return float_expr(std::remquo(x, y, quo)); }

		template<typename T,typename U> float_expr binary_functions<T,U>::hypot(float x, float y) { return float_expr(std::hypot(x, y)); }
#endif

		template<typename T,typename U,typename V> float_expr ternary_functions<T,U,V>::fma(float x, float y, float z)
		{
		#if HALF_ENABLE_CPP11_CMATH && defined(FP_FAST_FMAF)
			return float_expr(std::fma(x, y, z));
		#else
			return float_expr(x*y+z);
		#endif
		}


		/// Cast to or from half-precision floating point number.
		/// This casts between [half](\ref half_float::half) and any type convertible to/from `float` via an explicit cast of this 
		/// type to/from `float`. It uses the fastest rounding possible when performing a float-to-half conversion (if any) and is 
		/// thus equivalent to half_cast<T,std::round_indeterminate,U>() or a simple `static_cast`, but suppressing any possible 
		/// warnings due to an otherwise implicit conversion to/from `float`.
		///
		/// Using this cast with neither of the two types being a [half](\ref half_float::half) results in a compiler error and 
		/// casting between [half](\ref half_float::half)s is just a no-op.
		/// \tparam T destination type
		/// \tparam U source type
		/// \param arg value to cast
		/// \return \a arg converted to destination type
		template<typename T,typename U> T half_cast(const U &arg)
		{
			return half_caster<T,U,std::round_indeterminate>::cast(arg);
		}

		/// Cast to or from half-precision floating point number with specified rounding.
		/// This casts between [half](\ref half_float::half) and any type convertible to/from `float` via an explicit cast of this 
		/// type to/from `float`. The rounding mode used for the internal float-to-half conversion (if any) can be specified 
		/// explicitly, or chosen to be the fastest possible rounding using `std::round_indeterminate`, which would be equivalent 
		/// to half_cast<T,U>() or a simple `static_cast`, but suppressing any possible warnings due to an otherwise implicit 
		/// conversion to/from `float`.
		///
		/// Using this cast with neither of the two types being a [half](\ref half_float::half) results in a compiler error and 
		/// casting between [half](\ref half_float::half)s is just a no-op.
		/// \tparam T destination type
		/// \tparam R rounding mode to use
		/// \tparam U source type
		/// \param arg value to cast
		/// \return \a arg converted to destination type
		template<typename T,std::float_round_style R,typename U> T half_cast(const U &arg)
		{
			return half_caster<T,U,R>::cast(arg);
		}

		template<typename T,typename U> typename binary_enable<T,U,bool>::type operator==(T x, U y) { return functions<T,U>::isequal(x, y); }
		template<typename T,typename U> typename binary_enable<T,U,bool>::type operator!=(T x, U y) { return functions<T,U>::isnotequal(x, y); }
		template<typename T,typename U> typename binary_enable<T,U,bool>::type operator<(T x, U y) { return functions<T,U>::isless(x, y); }
		template<typename T,typename U> typename binary_enable<T,U,bool>::type operator>(T x, U y) { return functions<T,U>::isgreater(x, y); }
		template<typename T,typename U> typename binary_enable<T,U,bool>::type operator<=(T x, U y) { return functions<T,U>::islessequal(x, y); }
		template<typename T,typename U> typename binary_enable<T,U,bool>::type operator>=(T x, U y) { return functions<T,U>::isgreaterequal(x, y); }

		template<typename T,typename U> typename binary_result<function::plus,T,U>::type operator+(T x, U y) { return functions<T,U>::plus(x, y); }
		template<typename T,typename U> typename binary_result<function::minus,T,U>::type operator-(T x, U y) { return functions<T,U>::minus(x, y); }
		template<typename T,typename U> typename binary_result<function::multiplies,T,U>::type operator*(T x, U y) { return functions<T,U>::multiplies(x, y); }
		template<typename T,typename U> typename binary_result<function::divides,T,U>::type operator/(T x, U y) { return functions<T,U>::divides(x, y); }
		template<typename T> typename unary_enable<T,T>::type operator+(T arg) { return arg; }
		template<typename T> typename unary_result<function::negate,T>::type operator-(T arg) { return functions<T>::negate(arg); }

		template<typename T,typename charT,typename traits> typename unary_enable<T,std::basic_ostream<charT,traits>&>::type operator<<(std::basic_ostream<charT,traits> &out, T arg) { return functions<T>::write(out, arg); }
		template<typename charT,typename traits> std::basic_istream<charT,traits>& operator>>(std::basic_istream<charT,traits> &in, half &arg) { return functions<>::read(in, arg); }

		template<typename T> typename unary_result<function::fabs,T>::type abs(T arg) { return functions<T>::fabs(arg); }
		template<typename T> typename unary_result<function::fabs,T>::type fabs(T arg) { return functions<T>::fabs(arg); }
		template<typename T,typename U> typename binary_result<function::fmod,T,U>::type fmod(T x, U y) { return functions<T,U>::fmod(x, y); }
		template<typename T,typename U> typename binary_result<function::remainder,T,U>::type remainder(T x, U y) { return functions<T,U>::remainder(x, y); }
		template<typename T,typename U> typename binary_result<function::remquo,T,U>::type remquo(T x, U y, int *quo) { return functions<T,U>::remquo(x, y, quo); }
		template<typename T,typename U,typename V> typename ternary_result<function::fma,T,U,V>::type fma(T x, U y, V z) { return functions<T,U,V>::fma(x, y, z); }
		template<typename T,typename U> typename binary_result<function::fmax,T,U>::type fmax(T x, U y) { return functions<T,U>::fmax(x, y); }
		template<typename T,typename U> typename binary_result<function::fmin,T,U>::type fmin(T x, U y) { return functions<T,U>::fmin(x, y); }
		template<typename T,typename U> typename binary_result<function::fdim,T,U>::type fdim(T x, U y) { return functions<T,U>::fdim(x, y); }
		half nanh(const char *arg) { return functions<>::nanh(arg); }

		template<typename T> typename unary_result<function::exp,T>::type exp(T arg) { return functions<T>::exp(arg); }
		template<typename T> typename unary_result<function::exp2,T>::type exp2(T arg) { return functions<T>::exp2(arg); }
		template<typename T> typename unary_result<function::expm1,T>::type expm1(T arg) { return functions<T>::expm1(arg); }
		template<typename T> typename unary_result<function::log,T>::type log(T arg) { return functions<T>::log(arg); }
		template<typename T> typename unary_result<function::log10,T>::type log10(T arg) { return functions<T>::log10(arg); }
		template<typename T> typename unary_result<function::log1p,T>::type log1p(T arg) { return functions<T>::log1p(arg); }
		template<typename T> typename unary_result<function::log2,T>::type log2(T arg) { return functions<T>::log2(arg); }

		template<typename T> typename unary_result<function::sqrt,T>::type sqrt(T arg) { return functions<T>::sqrt(arg); }
		template<typename T> typename unary_result<function::cbrt,T>::type cbrt(T arg) { return functions<T>::cbrt(arg); }
		template<typename T,typename U> typename binary_result<function::hypot,T,U>::type hypot(T x, U y) { return functions<T,U>::hypot(x, y); }
		template<typename T,typename U> typename binary_result<function::pow,T,U>::type pow(T base, U exp) { return functions<T,U>::pow(base, exp); }

		template<typename T> typename unary_result<function::sin,T>::type sin(T arg) { return functions<T>::sin(arg); }
		template<typename T> typename unary_result<function::cos,T>::type cos(T arg) { return functions<T>::cos(arg); }
		template<typename T> typename unary_result<function::tan,T>::type tan(T arg) { return functions<T>::tan(arg); }
		template<typename T> typename unary_result<function::asin,T>::type asin(T arg) { return functions<T>::asin(arg); }
		template<typename T> typename unary_result<function::acos,T>::type acos(T arg) { return functions<T>::acos(arg); }
		template<typename T> typename unary_result<function::atan,T>::type atan(T arg) { return functions<T>::atan(arg); }
		template<typename T,typename U> typename binary_result<function::atan2,T,U>::type atan2(T x, U y) { return functions<T,U>::atan2(x, y); }

		template<typename T> typename unary_result<function::sinh,T>::type sinh(T arg) { return functions<T>::sinh(arg); }
		template<typename T> typename unary_result<function::cosh,T>::type cosh(T arg) { return functions<T>::cosh(arg); }
		template<typename T> typename unary_result<function::tanh,T>::type tanh(T arg) { return functions<T>::tanh(arg); }
		template<typename T> typename unary_result<function::asinh,T>::type asinh(T arg) { return functions<T>::asinh(arg); }
		template<typename T> typename unary_result<function::acosh,T>::type acosh(T arg) { return functions<T>::acosh(arg); }
		template<typename T> typename unary_result<function::atanh,T>::type atanh(T arg) { return functions<T>::atanh(arg); }

		template<typename T> typename unary_result<function::erf,T>::type erf(T arg) { return functions<T>::erf(arg); }
		template<typename T> typename unary_result<function::erfc,T>::type erfc(T arg) { return functions<T>::erfc(arg); }
		template<typename T> typename unary_result<function::lgamma,T>::type lgamma(T arg) { return functions<T>::lgamma(arg); }
		template<typename T> typename unary_result<function::tgamma,T>::type tgamma(T arg) { return functions<T>::tgamma(arg); }

		template<typename T> typename unary_result<function::ceil,T>::type ceil(T arg) { return functions<T>::ceil(arg); }
		template<typename T> typename unary_result<function::floor,T>::type floor(T arg) { return functions<T>::floor(arg); }
		template<typename T> typename unary_result<function::trunc,T>::type trunc(T arg) { return functions<T>::trunc(arg); }
		template<typename T> typename unary_result<function::round,T>::type round(T arg) { return functions<T>::round(arg); }
		template<typename T> typename unary_enable<T,long>::type lround(T arg) { return functions<T>::lround(arg); }
		template<typename T> typename unary_enable<T,long long>::type llround(T arg) { return functions<T>::llround(arg); }
		template<typename T> typename unary_result<function::nearbyint,T>::type nearbyint(T arg) { return functions<T>::nearbyint(arg); }
		template<typename T> typename unary_result<function::rint,T>::type rint(T arg) { return functions<T>::rint(arg); }
		template<typename T> typename unary_enable<T,long>::type lrint(T arg) { return functions<T>::lrint(arg); }
		template<typename T> typename unary_enable<T,long long>::type llrint(T arg) { return functions<T>::llrint(arg); }

		template<typename T> typename unary_result<function::frexp,T>::type frexp(T arg, int *exp) { return functions<T>::frexp(arg, exp); }
		template<typename T> typename unary_result<function::scalbln,T>::type ldexp(T arg, int exp) { return functions<T>::scalbln(arg, exp); }
		template<typename T> typename unary_result<function::modf,T>::type modf(T arg, half *iptr) { return functions<T>::modf(arg, iptr); }
		template<typename T> typename unary_result<function::scalbln,T>::type scalbn(T arg, int exp) { return functions<T>::scalbln(arg, exp); }
		template<typename T> typename unary_result<function::scalbln,T>::type scalbln(T arg, long exp) { return functions<T>::scalbln(arg, exp); }
		template<typename T> typename unary_enable<T,int>::type ilogb(T arg) { return functions<T>::ilogb(arg); }
		template<typename T> typename unary_result<function::logb,T>::type logb(T arg) { return functions<T>::logb(arg); }
		template<typename T,typename U> typename binary_result<function::nextafter,T,U>::type nextafter(T from, U to) { return functions<T,U>::nextafter(from, to); }
		template<typename T> typename unary_result<function::nexttoward,T>::type nexttoward(T from, long double to) { return functions<T>::nexttoward(from, to); }
		template<typename T,typename U> typename binary_result<function::copysign,T,U>::type copysign(T x, U y) { return functions<T,U>::copysign(x, y); }

		template<typename T> typename unary_enable<T,int>::type fpclassify(T arg) { return functions<T>::fpclassify(arg); }
		template<typename T> typename unary_enable<T,bool>::type isfinite(T arg) { return functions<T>::isfinite(arg); }
		template<typename T> typename unary_enable<T,bool>::type isinf(T arg) { return functions<T>::isinf(arg); }
		template<typename T> typename unary_enable<T,bool>::type isnan(T arg) { return functions<T>::isnan(arg); }
		template<typename T> typename unary_enable<T,bool>::type isnormal(T arg) { return functions<T>::isnormal(arg); }
		template<typename T> typename unary_enable<T,bool>::type signbit(T arg) { return functions<T>::signbit(arg); }

		template<typename T,typename U> typename binary_enable<T,U,bool>::type isgreater(T x, U y) { return functions<T,U>::isgreater(x, y); }
		template<typename T,typename U> typename binary_enable<T,U,bool>::type isgreaterequal(T x, U y) { return functions<T,U>::isgreaterequal(x, y); }
		template<typename T,typename U> typename binary_enable<T,U,bool>::type isless(T x, U y) { return functions<T,U>::isless(x, y); }
		template<typename T,typename U> typename binary_enable<T,U,bool>::type islessequal(T x, U y) { return functions<T,U>::islessequal(x, y); }
		template<typename T,typename U> typename binary_enable<T,U,bool>::type islessgreater(T x, U y) { return functions<T,U>::islessgreater(x, y); }
		template<typename T,typename U> typename binary_enable<T,U,bool>::type isunordered(T x, U y) { return functions<T,U>::isunordered(x, y); }
	}
}


/// Extensions to the C++ standard library.
namespace std
{
	/// Numeric limits for half-precision floats.
	/// Because of the underlying single-precision implementation of many operations, it inherits some properties from 
	/// `std::numeric_limits<float>`.
	template<> class numeric_limits<half_float::half> : public std::numeric_limits<float>
	{
	public:
		/// Supports signed values.
		static HALF_CONSTEXPR_CONST bool is_signed = true;

		/// Is not exact.
		static HALF_CONSTEXPR_CONST bool is_exact = false;

		/// Doesn't provide modulo arithmetic.
		static HALF_CONSTEXPR_CONST bool is_modulo = false;

		/// IEEE conformant.
		static HALF_CONSTEXPR_CONST bool is_iec559 = true;

		/// Supports infinity.
		static HALF_CONSTEXPR_CONST bool has_infinity = true;

		/// Supports quiet NaNs.
		static HALF_CONSTEXPR_CONST bool has_quiet_NaN = true;

		/// Supports subnormal values.
		static HALF_CONSTEXPR_CONST std::float_denorm_style has_denorm = std::denorm_present;

		/// Rounding mode.
		/// Due to the mix of internal single-precision computations (using the rounding mode of the underlying 
		/// single-precision implementation) with explicit truncation of the single-to-half conversions, the actual rounding 
		/// mode is indeterminate.
		static HALF_CONSTEXPR_CONST std::float_round_style round_style = std::round_indeterminate;

		/// Significant digits.
		static HALF_CONSTEXPR_CONST int digits = 11;

		/// Significant decimal digits.
		static HALF_CONSTEXPR_CONST int digits10 = 3;

		/// Required decimal digits to represent all possible values.
		static HALF_CONSTEXPR_CONST int max_digits10 = 5;

		/// Number base.
		static HALF_CONSTEXPR_CONST int radix = 2;

		/// One more than smallest exponent.
		static HALF_CONSTEXPR_CONST int min_exponent = -13;

		/// Smallest normalized representable power of 10.
		static HALF_CONSTEXPR_CONST int min_exponent10 = -4;

		/// One more than largest exponent
		static HALF_CONSTEXPR_CONST int max_exponent = 16;

		/// Largest finitely representable power of 10.
		static HALF_CONSTEXPR_CONST int max_exponent10 = 4;

		/// Smallest positive normal value.
		static HALF_CONSTEXPR half_float::half min() HALF_NOTHROW { return half_float::half(0x0400, true); }

		/// Smallest finite value.
		static HALF_CONSTEXPR half_float::half lowest() HALF_NOTHROW { return half_float::half(0xFBFF, true); }

		/// Largest finite value.
		static HALF_CONSTEXPR half_float::half max() HALF_NOTHROW { return half_float::half(0x7BFF, true); }

		/// Difference between one and next representable value.
		static HALF_CONSTEXPR half_float::half epsilon() HALF_NOTHROW { return half_float::half(0x1400, true); }

		/// Maximum rounding error.
		static HALF_CONSTEXPR half_float::half round_error() HALF_NOTHROW { return half_float::half(0x3C00, true); }

		/// Positive infinity.
		static HALF_CONSTEXPR half_float::half infinity() HALF_NOTHROW { return half_float::half(0x7C00, true); }

		/// Quiet NaN.
		static HALF_CONSTEXPR half_float::half quiet_NaN() HALF_NOTHROW { return half_float::half(0x7FFF, true); }

		/// Signalling NaN.
		static HALF_CONSTEXPR half_float::half signaling_NaN() HALF_NOTHROW { return half_float::half(0x7DFF, true); }

		/// Smallest positive subnormal value.
		static HALF_CONSTEXPR half_float::half denorm_min() HALF_NOTHROW { return half_float::half(0x0001, true); }
	};

#if HALF_ENABLE_CPP11_HASH
	/// Hash function for half-precision floats.
	/// This is only defined if C++11 `std::hash` is supported and enabled.
	template<> struct hash<half_float::half>
	{
		/// Type of function argument.
		typedef half_float::half argument_type;

		/// Function return type.
		typedef std::size_t result_type;

		/// Compute hash function.
		/// \param arg half to hash
		/// \return hash value
		std::size_t operator()(half_float::half arg) const
		{
			return std::hash<half_float::detail::uint16>()(arg.data_&-static_cast<half_float::detail::uint16>(arg.data_!=0x8000));
		}
	};
#endif
}


#undef HALF_CONSTEXPR
#undef HALF_CONSTEXPR_CONST
#undef HALF_NOEXCEPT
#undef HALF_NOTHROW

#endif
