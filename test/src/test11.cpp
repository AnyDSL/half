// test - Test application for half-precision floating point functionality.
//
// Copyright (c) 2012-2020 Christian Rau <rauy@users.sourceforge.net>
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

//#define HALF_ENABLE_F16C_INTRINSICS 1
//#define HALF_ARITHMETIC_TYPE double
#define HALF_ERRHANDLING_FLAGS 1
#define HALF_ERRHANDLING_OVERFLOW_TO_INEXACT 1
#define HALF_ERRHANDLING_UNDERFLOW_TO_INEXACT 1
#define HALF_ROUND_STYLE 1
#include <half.hpp>

#define CHECK_EXCEPT 0//(FE_INVALID|FE_DIVBYZERO|FE_OVERFLOW|FE_UNDERFLOW|FE_INEXACT)

#include <utility>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <iostream>
#include <iomanip>
#include <memory>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <functional>
#include <fstream>
#include <random>
#include <bitset>
#include <limits>
#include <chrono>
#include <typeinfo>
#include <stdexcept>
#include <cstdint>
#include <cmath>
#if HALF_ENABLE_CPP11_HASH
	#include <unordered_map>
#endif

#if HALF_ENABLE_CPP11_CFENV && !defined(HALF_ARITHMETIC_TYPE)
	#include <cfenv>
	#pragma STDC FENV_ACCESS ON
#endif

int ilog2(int i)
{
	unsigned int l = 0;
	for(; i>0; i>>=1,++l) ;
	return l;
}

using half_float::half;
using half_float::half_cast;
#if HALF_ENABLE_CPP11_USER_LITERALS
	using namespace half_float::literal;
#endif

half b2h(std::uint16_t bits)
{
	return *reinterpret_cast<half*>(&bits);
}

std::uint16_t h2b(half h)
{
	return *reinterpret_cast<std::uint16_t*>(&h);
}

bool comp(half a, half b)
{
	return (isnan(a) && isnan(b)) || h2b(a) == h2b(b);
}

bool compz(half a, half b)
{
	return (isnan(a) && isnan(b)) || a == b;
}

class results
{
public:
	void read(std::istream &in)
	{
		in.read(reinterpret_cast<char*>(&value_), sizeof(value_));
		in.read(reinterpret_cast<char*>(&flags_), sizeof(flags_));
	}

	half value() const
	{
		switch(std::numeric_limits<half>::round_style)
		{
			case std::round_toward_zero:			return b2h(h2b(value_)-((flags_>>6)&1));
			case std::round_toward_infinity:		return b2h(h2b(value_)+((flags_>>7)&!signbit(value_))-((flags_>>6)&signbit(value_)));
			case std::round_toward_neg_infinity:	return b2h(h2b(value_)+((flags_>>7)&signbit(value_))-((flags_>>6)&!signbit(value_)));
			default:								return value_;
		}
	}

	int except(int flags = FE_ALL_EXCEPT) const { return static_cast<int>(flags_&flags); }

private:
	half value_;
	unsigned char flags_;
};

template<std::float_round_style R> half select(const std::pair<half,half> &hh)
{
	return	(R==std::round_toward_zero && abs(hh.first)>abs(hh.second)) ||
			(R==std::round_toward_infinity && hh.second>hh.first) ||
			(R==std::round_toward_neg_infinity && hh.second<=hh.first) ? 
			hh.second : hh.first;
}

void performance_test(std::ostream &out = std::cout, std::ostream *csv = nullptr);


class half_test
{
public:
	half_test(std::ostream &log, std::ostream *csv, bool fast, bool rough)
		: tests_(0), log_(log), csv_(csv), fast_(fast), rough_(rough)
	{
		//prepare halfs
		half_vector batch;
		std::uint16_t u = 0;
		halfs_.insert(std::make_pair("positive zero", half_vector(1, b2h(u++))));
		for(; u<0x400; ++u)
			batch.push_back(b2h(u));
		halfs_.insert(std::make_pair("positive subn", std::move(batch)));
		batch.clear();
		for(; u<0x7C00; ++u)
			batch.push_back(b2h(u));
		halfs_.insert(std::make_pair("positive norm", std::move(batch)));
		batch.clear();
		halfs_.insert(std::make_pair("positive inft", half_vector(1, b2h(u++))));
		for(; u<0x8000; ++u)
			batch.push_back(b2h(u));
		halfs_.insert(std::make_pair("positive  NaN", std::move(batch)));
		batch.clear();
		halfs_.insert(std::make_pair("negative zero", half_vector(1, b2h(u++))));
		for(; u<0x8400; ++u)
			batch.push_back(b2h(u));
		halfs_.insert(std::make_pair("negative subn", std::move(batch)));
		batch.clear();
		for(; u<0xFC00; ++u)
			batch.push_back(b2h(u));
		halfs_.insert(std::make_pair("negative norm", std::move(batch)));
		batch.clear();
		halfs_.insert(std::make_pair("negative inft", half_vector(1, b2h(u++))));
		for(; u!=0; ++u)
			batch.push_back(b2h(u));
		halfs_.insert(std::make_pair("negative  NaN", std::move(batch)));

		//set classes
		classes_["positive zero"] = FP_ZERO;
		classes_["positive subn"] = FP_SUBNORMAL;
		classes_["positive norm"] = FP_NORMAL;
		classes_["positive inft"] = FP_INFINITE;
		classes_["positive  NaN"] = FP_NAN;
		classes_["negative zero"] = FP_ZERO;
		classes_["negative subn"] = FP_SUBNORMAL;
		classes_["negative norm"] = FP_NORMAL;
		classes_["negative inft"] = FP_INFINITE;
		classes_["negative  NaN"] = FP_NAN;
	}

	unsigned int test()
	{
/*
		//test size
		simple_test("size", []() { return sizeof(half)*CHAR_BIT >= 16; });

		//test conversion
		unary_test("float conversion", [](half arg) { return comp(half_cast<half>(half_cast<float>(arg)), arg); });
		unary_test("double conversion", [](half arg) { return comp(half_cast<half>(half_cast<double>(arg)), arg); });
		unary_test("long double conversion", [](half arg) { return comp(half_cast<half>(half_cast<long double>(arg)), arg); });

		//test classification functions
		class_test("fpclassify", [](half arg, int cls) { return fpclassify(arg) == cls; });
		class_test("isfinite", [](half arg, int cls) { return isfinite(arg) == (cls!=FP_INFINITE&&cls!=FP_NAN); });
		class_test("isinf", [](half arg, int cls) { return isinf(arg) == (cls==FP_INFINITE); });
		class_test("isnan", [](half arg, int cls) { return isnan(arg) == (cls==FP_NAN); });
		class_test("isnormal", [](half arg, int cls) { return isnormal(arg) == (cls==FP_NORMAL); });
		unary_test("signbit", [](half arg) -> bool { double f = arg; return isnan(arg) || f==0.0 || (signbit(arg)==(f<0.0)); });
	#if HALF_ENABLE_CPP11_CMATH
		unary_except_test("fpclassify", [](half arg) -> bool { int ch=fpclassify(arg), cf=std::fpclassify(
			half_cast<double>(arg)); return ch==cf || (ch==FP_SUBNORMAL && cf==FP_NORMAL); });
		unary_except_test("isfinite", [](half arg) { return isfinite(arg) == std::isfinite(half_cast<double>(arg)); });
		unary_except_test("isinf", [](half arg) { return isinf(arg) == std::isinf(half_cast<double>(arg)); });
		unary_except_test("isnan", [](half arg) { return isnan(arg) == std::isnan(half_cast<double>(arg)); });
		unary_except_test("isnormal", [](half arg) { return isnormal(arg) == std::isnormal(half_cast<double>(arg)) ||
			(!isnormal(arg) && fpclassify(arg)==FP_SUBNORMAL); });
		unary_except_test("signbit", [](half arg) { return signbit(arg) == std::signbit(half_cast<double>(arg)); });

		//test comparison functions
		binary_test("isgreater", [](half a, half b) { return isgreater(a, b) == std::isgreater(half_cast<double>(a), half_cast<double>(b)); });
		binary_test("isgreaterequal", [](half a, half b) { return isgreaterequal(a, b) == std::isgreaterequal(half_cast<double>(a), half_cast<double>(b)); });
		binary_test("isless", [](half a, half b) { return isless(a, b) == std::isless(half_cast<double>(a), half_cast<double>(b)); });
		binary_test("islessequal", [](half a, half b) { return islessequal(a, b) == std::islessequal(half_cast<double>(a), half_cast<double>(b)); });
		binary_test("islessgreater", [](half a, half b) { return islessgreater(a, b) == std::islessgreater(half_cast<double>(a), half_cast<double>(b)); });
		binary_test("isunordered", [](half a, half b) { return isunordered(a, b) == std::isunordered(half_cast<double>(a), half_cast<double>(b)); });
	#endif

		//test unary operators
		unary_double_test("unary plus", [](half arg) { return +arg; }, [](double arg) { return +arg; });
		unary_double_test("unary minus", [](half arg) { return -arg; }, [](double arg) { return -arg; });
		unary_double_test("prefix increment", [](half arg) { return ++arg; }, [](double arg) { return ++arg; });
		unary_double_test("prefix decrement", [](half arg) { return --arg; }, [](double arg) { return --arg; });
		unary_double_test("postfix increment", [](half arg) { return arg++; }, [](double arg) { return arg++; });
		unary_double_test("postfix decrement", [](half arg) { return arg--; }, [](double arg) { return arg--; });

		//test binary operators
		binary_double_test("addition", [](half a, half b) { return a+b; }, [](double a, double b) { return a+b; });
		binary_double_test("subtraction", [](half a, half b) { return a-b; }, [](double a, double b) { return a-b; });
		binary_double_test("multiplication", [](half a, half b) { return a*b; }, [](double a, double b) { return a*b; });
		binary_double_test("division", [](half a, half b) { return a/b; }, [](double a, double b) { return a/b; });

		//test comparison operators
		binary_except_test("equal", [](half a, half b) { return (a==b) == (half_cast<double>(a)==half_cast<double>(b)); });
		binary_except_test("not equal", [](half a, half b) { return (a!=b) == (half_cast<double>(a)!=half_cast<double>(b)); });
		binary_except_test("less", [](half a, half b) { return (a<b) == (half_cast<double>(a)<half_cast<double>(b)); });
		binary_except_test("greater", [](half a, half b) { return (a>b) == (half_cast<double>(a)>half_cast<double>(b)); });
		binary_except_test("less equal", [](half a, half b) { return (a<=b) == (half_cast<double>(a)<=half_cast<double>(b)); });
		binary_except_test("greater equal", [](half a, half b) { return (a>=b) == (half_cast<double>(a)>=half_cast<double>(b)); });

		//test basic functions
		unary_double_test("abs", half_float::abs, [](double arg) { return std::abs(arg); });
		unary_double_test("fabs", half_float::fabs, [](double arg) { return std::fabs(arg); });
		binary_double_test("fmod", half_float::fmod, [](double x, double y) { return std::fmod(x, y); });
		ternary_double_test("fma", half_float::fma, [](double x, double y, double z) { return x*y + z; });
	#if HALF_ENABLE_CPP11_CMATH
		binary_double_test("remainder", half_float::remainder, [](double x, double y) { return std::remainder(x, y); });
		binary_except_test("remquo", [](half a, half b) -> bool { int qh = 0, qf = 0; remquo(a, b, &qh); std::remquo(half_cast<double>(a), half_cast<double>(b), &qf); return (qh&7)==(qf&7); });
		binary_double_test("fmin", half_float::fmin, [](double x, double y) { return std::fmin(x, y); });
		binary_double_test("fmax", half_float::fmax, [](double x, double y) { return std::fmax(x, y); });
		binary_double_test("fdim", half_float::fdim, [](double x, double y) { return std::fdim(x, y); });
		ternary_double_test("fma", half_float::fma, [](double x, double y, double z) { return std::fma(x, y, z); });
	#endif

		//test exponential functions
		unary_reference_test("exp", half_float::exp);
		unary_reference_test("exp2", half_float::exp2);
		unary_reference_test("expm1", half_float::expm1);
		unary_reference_test("log", half_float::log);
		unary_reference_test("log10", half_float::log10);
		unary_reference_test("log2", half_float::log2);
		unary_reference_test("log1p", half_float::log1p);

		//test power functions
		unary_reference_test("sqrt", half_float::sqrt);
		unary_reference_test("rsqrt", half_float::rsqrt);
		unary_reference_test("cbrt", half_float::cbrt);
		binary_reference_test("pow", half_float::pow);
		binary_reference_test<half(half,half)>("hypot", half_float::hypot);

		//test trigonometric functions
		unary_reference_test("sin", half_float::sin);
		unary_reference_test("cos", half_float::cos);
		unary_reference_test("tan", half_float::tan);
		unary_reference_test("asin", half_float::asin);
		unary_reference_test("acos", half_float::acos);
		unary_reference_test("atan", half_float::atan);
		binary_reference_test("atan2", half_float::atan2);

		//test hyperbolic functions
		unary_reference_test("sinh", half_float::sinh);
		unary_reference_test("cosh", half_float::cosh);
		unary_reference_test("tanh", half_float::tanh);
		unary_reference_test("asinh", half_float::asinh);
		unary_reference_test("acosh", half_float::acosh);
		unary_reference_test("atanh", half_float::atanh);

		//test error and gamma functions
		unary_reference_test("erf", half_float::erf);
		unary_reference_test("erfc", half_float::erfc);
		unary_reference_test("lgamma", half_float::lgamma);
		unary_reference_test("tgamma", half_float::tgamma);

		unary_test("rsqrt", [](half arg) { half a = rsqrt(arg), b = half(1.0f) / sqrt(arg), c = half_cast<half>(1.0/sqrt(half_cast<double>(arg)));
			return comp(a, b) || std::abs(h2b(a)-h2b(c)) <= std::abs(h2b(b)-h2b(c)); });

	#if HALF_ENABLE_CPP11_CMATH
		//test binary functions
		auto isgeneral = [](double x, double y) { return std::isfinite(x) && std::isfinite(y) && x != 0.0 && y != 0.0; };
		binary_double_test("fdim", half_float::fdim, [&](double x, double y) { return std::fdim(x, y); });
		binary_double_test("pow", half_float::pow, [&](double x, double y) -> double { double d = std::pow(x, y);
			return (isgeneral(x, y) && d==0.0) ? std::copysign(std::numeric_limits<double>::min(), d) :
			(isgeneral(x, y) && std::isinf(d)) ? std::copysign(std::numeric_limits<double>::max(), d) : d; });
//		binary_double_test("hypot", [](half x, half y) { return hypot(x, y); }, [&](double x, double y) { return std::hypot(x, y); });
		binary_double_test("atan2", half_float::atan2, [&](double y, double x) -> double { double d = std::atan2(y, x);
			return (isgeneral(x, y) && d==0.0) ? std::copysign(std::numeric_limits<double>::min(), y) : d; });
	#endif

		//test rounding functions
		unary_double_test("ceil", half_float::ceil, [](double arg) { return std::ceil(arg); });
		unary_double_test("floor", half_float::floor, [](double arg) { return std::floor(arg); });
		unary_test("trunc", [](half arg) { return !isfinite(arg) || compz(trunc(arg), half_cast<half>(static_cast<int>(arg))); });
		unary_test("round", [](half arg) { return !isfinite(arg) || compz(round(arg), half_cast<half>(static_cast<int>(static_cast<double>(arg)+(signbit(arg) ? -0.5 : 0.5)))); });
		unary_test("lround", [](half arg) { return !isfinite(arg) || lround(arg) == static_cast<long>(static_cast<double>(arg)+(signbit(arg) ? -0.5 : 0.5)); });
		unary_test("nearbyint", [](half arg) { return !isfinite(arg) || compz(nearbyint(arg), half_cast<half>(half_cast<int>(arg))); });
		unary_test("rint", [](half arg) { return !isfinite(arg) || compz(rint(arg), half_cast<half>(half_cast<int>(arg))); });
		unary_test("lrint", [](half arg) { return !isfinite(arg) || lrint(arg) == half_cast<long>(arg); });
		unary_test("llround", [](half arg) { return !isfinite(arg) || llround(arg) == 
			static_cast<long long>(static_cast<double>(arg)+(signbit(arg) ? -0.5 : 0.5)); });
		unary_test("llrint", [](half arg) { return !isfinite(arg) || llrint(arg) == half_cast<long long>(arg); });
	#if HALF_ENABLE_CPP11_CMATH
		unary_double_test("trunc", half_float::trunc, [](double arg) { return std::trunc(arg); });
		unary_double_test("round", half_float::round, [](double arg) { return std::round(arg); });
		unary_except_test("lround", [](half arg) { return !isfinite(arg) || lround(arg) == std::lround(static_cast<double>(arg)); });
		unary_except_test("llround", [](half arg) { return !isfinite(arg) || llround(arg) == std::llround(static_cast<double>(arg)); });
	#if HALF_ROUND_STYLE == 1
		unary_double_test("nearbyint", half_float::nearbyint, [](double arg) { return std::nearbyint(arg); });
		unary_double_test("rint", half_float::rint, [](double arg) { return std::rint(arg); });
		unary_except_test("lrint", [](half arg) { return !isfinite(arg) || half_float::lrint(arg) == std::lrint(static_cast<double>(arg)); });
		unary_except_test("llrint", [](half arg) { return !isfinite(arg) || llrint(arg) == std::llrint(static_cast<double>(arg)); });
	#endif
	#endif

		//test float functions
		unary_double_test("frexp", [](half arg) -> half { int i; return frexp(arg, &i); }, [](double arg) -> double { int i; return std::frexp(arg, &i); });
		unary_test("frexp", [](half arg) -> bool { int eh, ef; bool eq = comp(frexp(arg, &eh), 
			static_cast<half>(std::frexp(static_cast<double>(arg), &ef))); return eq && (!isfinite(arg) || eh==ef); });
		unary_test("ldexp", [](half arg) -> bool { unsigned int passed = 0; for(int i=-50; i<50; ++i) passed += 
			comp(ldexp(arg, i), static_cast<half>(std::ldexp(static_cast<double>(arg), i))); return passed==100; });
		unary_double_test("modf", [](half arg) -> half { half i; return modf(arg, &i); }, [](double arg) -> double { double i; return std::modf(arg, &i); });
		unary_double_test("modf", [](half arg) -> half { half i; modf(arg, &i); return i; }, [](double arg) -> double { double i; std::modf(arg, &i); return i; });
		binary_test("nextafter", [](half a, half b) -> bool { half c = nextafter(a, b); std::int16_t d = std::abs(
			static_cast<std::int16_t>(h2b(a)-h2b(c))); return ((isnan(a) || isnan(b)) && isnan(c)) || 
			(compz(a, b) && compz(b, c)) || ((d==1||d==0x7FFF) && (a<b)==(a<c)); });
		binary_test("nexttoward", [](half a, half b) -> bool { half c = nexttoward(a, static_cast<long double>(b)); std::int16_t d = std::abs(
			static_cast<std::int16_t>(h2b(a)-h2b(c))); return ((isnan(a) || isnan(b)) && isnan(c)) || 
			(compz(a, b) && compz(b, c)) || ((d==1||d==0x7FFF) && (a<b)==(a<c)); });
		binary_test("copysign", [](half a, half b) -> bool { half h = copysign(a, b); return comp(abs(h), abs(a)) && signbit(h)==signbit(b); });
	#if HALF_ENABLE_CPP11_CMATH
		unary_test("scalbn", [](half arg) -> bool { unsigned int passed = 0; for(int i=-50; i<50; ++i) passed += 
			comp(scalbn(arg, i), static_cast<half>(std::scalbn(static_cast<double>(arg), i))); return passed==100; });
		unary_test("scalbln", [](half arg) -> bool { unsigned int passed = 0; for(long i=-50; i<50; ++i) passed += 
			comp(scalbln(arg, i), static_cast<half>(std::scalbln(static_cast<double>(arg), i))); return passed==100; });
		unary_except_test("ilogb", [](half arg) { return ilogb(arg) == std::ilogb(static_cast<double>(arg)); });
		unary_double_test("logb", half_float::logb, [](double arg) { return std::logb(arg); });
		binary_double_test("copysign", half_float::copysign, [](double x, double y) { return std::copysign(x, y); });
	#endif

		//test rounding
		float_test("round_to_nearest", [](float f) -> bool { half a = half_cast<half,std::round_indeterminate>(f), 
			b(nextafter(a, copysign(std::numeric_limits<half>::infinity(), a))), h = half_cast<half,std::round_to_nearest>(f);
			float af(a), bf(b), hf(h); return half_float::detail::builtin_isnan(f) || (std::abs(hf)>std::abs(f)&&comp(h, b)&&((std::abs(f-af)>std::abs(bf-f) || 
			(std::abs(f-af)==std::abs(bf-f)&&!(h2b(h)&1)))||isinf(h))) || (std::abs(hf)<=std::abs(f)&&comp(h, a)&&((std::abs(f-af)<std::abs(bf-f) || 
			(std::abs(f-af)==std::abs(bf-f)&&!(h2b(h)&1)))||isinf(h))); });
		float_test("round_toward_zero", [](float f) -> bool { half a = half_cast<half,std::round_indeterminate>(f),
			h = half_cast<half,std::round_toward_zero>(f); float af(a), hf(h); return half_float::detail::builtin_isnan(f) || isinf(a) || af == hf; });
		float_test("round_toward_infinity", [](float f) -> bool { half a = half_cast<half,std::round_toward_zero>(f),
			b(nextafter(a, copysign(std::numeric_limits<half>::infinity(), a))), h = half_cast<half,std::round_toward_infinity>(f);
			float hf(h); return half_float::detail::builtin_isnan(f) || (comp(h, a)&&(signbit(h)||hf==f)) || (comp(h, b)&&!signbit(h)&&hf>f); });
		float_test("round_toward_neg_infinity", [](float f) -> bool { half a = half_cast<half,std::round_toward_zero>(f),
			b(nextafter(a, copysign(std::numeric_limits<half>::infinity(), a))), h = half_cast<half,std::round_toward_neg_infinity>(f);
			float hf(h); return half_float::detail::builtin_isnan(f) || (comp(h, a)&&(!signbit(h)||hf==f)) || (comp(h, b)&&signbit(h)&&hf<f); });

		//test float casting
		auto rand23 = std::bind(std::uniform_int_distribution<std::uint32_t>(0, (1<<23)-1), std::default_random_engine());
		unary_test("half_cast<float>", [](half arg) -> bool { float a = half_cast<float>(arg), b = static_cast<float>(arg); 
			return *reinterpret_cast<std::uint32_t*>(&a) == *reinterpret_cast<std::uint32_t*>(&b); });
		unary_test("half_cast<round_to_nearest>(float)", [&rand23](half arg) -> bool { float f = half_cast<float>(arg); 
			std::uint32_t n=rand23(), m=1<<13; if(fpclassify(arg)==FP_SUBNORMAL) m <<= std::min(std::max(-ilogb(arg)-14, 0), 10);
			*reinterpret_cast<std::uint32_t*>(&f) |= n&(m-1)&-isfinite(arg); return fpclassify(arg)==FP_ZERO || 
			comp(half_cast<half,std::round_to_nearest>(f), ((n&(m>>1)) && ((n&((m>>1)-1)) || (h2b(arg)&1)))
			? nextafter(arg, copysign(std::numeric_limits<half>::infinity(), arg)) : arg); });
		unary_test("half_cast<round_toward_zero>(float)", [&rand23](half arg) -> bool { float f = half_cast<float>(arg);
			std::uint32_t n=rand23(), m=1<<13; if(fpclassify(arg)==FP_SUBNORMAL) m <<= std::min(std::max(-ilogb(arg)-14, 0), 10);
			*reinterpret_cast<std::uint32_t*>(&f) |= n&(m-1)&-isfinite(arg); return comp(half_cast<half,std::round_toward_zero>(f), arg); });
		unary_test("half_cast<round_toward_infinity>(float)", [&rand23](half arg) -> bool { float f = half_cast<float>(arg);
			std::uint32_t n=rand23(), m=1<<13; if(fpclassify(arg)==FP_SUBNORMAL) m <<= std::min(std::max(-ilogb(arg)-14, 0), 10);
			*reinterpret_cast<std::uint32_t*>(&f) |= n&(m-1)&-isfinite(arg); return comp(half_cast<half,std::round_toward_infinity>(f), 
			(!signbit(arg)&&(n&(m-1))) ? nextafter(arg, copysign(std::numeric_limits<half>::infinity(), arg)) : arg); });
		unary_test("half_cast<round_toward_neg_infinity>(float)", [&rand23](half arg) -> bool { float f = half_cast<float>(arg);
			std::uint32_t n=rand23(), m=1<<13; if(fpclassify(arg)==FP_SUBNORMAL) m <<= std::min(std::max(-ilogb(arg)-14, 0), 10);
			*reinterpret_cast<std::uint32_t*>(&f) |= n&(m-1)&-isfinite(arg); return comp(half_cast<half,std::round_toward_neg_infinity>(f), 
			(signbit(arg)&&(n&(m-1))) ? nextafter(arg, copysign(std::numeric_limits<half>::infinity(), arg)) : arg); });

		//test double casting
		auto rand52 = std::bind(std::uniform_int_distribution<std::uint64_t>(0, (1ULL<<52)-1), std::default_random_engine());
		unary_test("half_cast<double>", [](half arg) -> bool { double a = half_cast<double>(arg), b = static_cast<float>(arg); 
			return isnan(arg) || *reinterpret_cast<std::uint64_t*>(&a) == *reinterpret_cast<std::uint64_t*>(&b); });
		unary_test("half_cast<round_to_nearest>(double)", [&rand52](half arg) -> bool { double f = half_cast<double>(arg); 
			std::uint64_t n=rand52(), m=1ULL<<42; if(fpclassify(arg)==FP_SUBNORMAL) m <<= std::min(std::max(-ilogb(arg)-14, 0), 10);
			*reinterpret_cast<std::uint64_t*>(&f) |= n&(m-1)&-isfinite(arg); return fpclassify(arg)==FP_ZERO || 
			comp(half_cast<half,std::round_to_nearest>(f), ((n&(m>>1)) && ((n&((m>>1)-1)) || (h2b(arg)&1)))
			? nextafter(arg, copysign(std::numeric_limits<half>::infinity(), arg)) : arg); });
		unary_test("half_cast<round_toward_zero>(double)", [&rand52](half arg) -> bool { double f = half_cast<double>(arg);
			std::uint64_t n=rand52(), m=1ULL<<42; if(fpclassify(arg)==FP_SUBNORMAL) m <<= std::min(std::max(-ilogb(arg)-14, 0), 10);
			*reinterpret_cast<std::uint64_t*>(&f) |= n&(m-1)&-isfinite(arg); return comp(half_cast<half,std::round_toward_zero>(f), arg); });
		unary_test("half_cast<round_toward_infinity>(double)", [&rand52](half arg) -> bool { double f = half_cast<double>(arg);
			std::uint64_t n=rand52(), m=1ULL<<42; if(fpclassify(arg)==FP_SUBNORMAL) m <<= std::min(std::max(-ilogb(arg)-14, 0), 10);
			*reinterpret_cast<std::uint64_t*>(&f) |= n&(m-1)&-isfinite(arg); return comp(half_cast<half,std::round_toward_infinity>(f), 
			(!signbit(arg)&&(n&(m-1))) ? nextafter(arg, copysign(std::numeric_limits<half>::infinity(), arg)) : arg); });
		unary_test("half_cast<round_toward_neg_infinity>(double)", [&rand52](half arg) -> bool { double f = half_cast<double>(arg);
			std::uint64_t n=rand52(), m=1ULL<<42; if(fpclassify(arg)==FP_SUBNORMAL) m <<= std::min(std::max(-ilogb(arg)-14, 0), 10);
			*reinterpret_cast<std::uint64_t*>(&f) |= n&(m-1)&-isfinite(arg); return comp(half_cast<half,std::round_toward_neg_infinity>(f), 
			(signbit(arg)&&(n&(m-1))) ? nextafter(arg, copysign(std::numeric_limits<half>::infinity(), arg)) : arg); });

		//test casting to int
	#if HALF_ENABLE_CPP11_CMATH
		unary_test("half_cast<int>", [](half arg) -> bool { return !isfinite(arg) || half_cast<int>(arg) == static_cast<int>(nearbyint(arg)); });
	#endif
		unary_test("half_cast<int,round_to_nearest>", [](half arg) -> bool { float fi, ff = std::abs(std::modf(static_cast<float>(arg), &fi));
			int i = static_cast<int>(fi); i += (-2*signbit(arg)+1) * (ff>0.5f || (ff==0.5f && i&1));
			return !isfinite(arg) || half_cast<int,std::round_to_nearest>(arg) == i;
		});
		unary_test("half_cast<int,round_toward_zero>", [](half arg) -> bool { return !isfinite(arg) || half_cast<int,std::round_toward_zero>(arg) == static_cast<int>(arg); });
		unary_test("half_cast<int,round_toward_infinity>", [](half arg) -> bool { float fi, ff = std::modf(static_cast<float>(arg), &fi);
			return !isfinite(arg) || half_cast<int,std::round_toward_infinity>(arg) == (static_cast<int>(fi)+(ff>0.0f)); });
		unary_test("half_cast<int,round_toward_neg_infinity>", [](half arg) -> bool { float fi, ff = std::modf(static_cast<float>(arg), &fi);
			return !isfinite(arg) || half_cast<int,std::round_toward_neg_infinity>(arg) == (static_cast<int>(fi)-(ff<0.0f)); });

		//test casting from int
		int_test("half_cast<>(int)", [](int i) -> bool { return comp(half_cast<half>(i), half_cast<half>(static_cast<float>(i))); });
		int_test("half_cast<round_to_nearest>(int)", [](int i) -> bool { 
			return comp(half_cast<half,std::round_to_nearest>(i), half_cast<half,std::round_to_nearest>(static_cast<float>(i))); });
		int_test("half_cast<round_toward_zero>(int)", [](int i) -> bool { 
			return comp(half_cast<half,std::round_toward_zero>(i), half_cast<half,std::round_toward_zero>(static_cast<float>(i))); });
		int_test("half_cast<round_toward_infinity>(int)", [](int i) -> bool { 
			return comp(half_cast<half,std::round_toward_infinity>(i), half_cast<half,std::round_toward_infinity>(static_cast<float>(i))); });
		int_test("half_cast<round_toward_neg_infinity>(int)", [](int i) -> bool { 
			return comp(half_cast<half,std::round_toward_neg_infinity>(i), half_cast<half,std::round_toward_neg_infinity>(static_cast<float>(i))); });

		//test numeric limits
		unary_test("numeric_limits::min", [](half arg) { return !isnormal(arg) || signbit(arg) || arg>=std::numeric_limits<half>::min(); });
		unary_test("numeric_limits::lowest", [](half arg) { return !isfinite(arg) || arg>=std::numeric_limits<half>::lowest(); });
		unary_test("numeric_limits::max", [](half arg) { return !isfinite(arg) || arg<=std::numeric_limits<half>::max(); });
		unary_test("numeric_limits::denorm_min", [](half arg) { return !isfinite(arg) || 
			signbit(arg) || arg==static_cast<half>(0.0f) || arg>=std::numeric_limits<half>::denorm_min(); });
		simple_test("numeric_limits::infinity", []() { return isinf(std::numeric_limits<half>::infinity()) && 
			!signbit(std::numeric_limits<half>::infinity()); });
		simple_test("numeric_limits::quiet_NaN", []() { return isnan(std::numeric_limits<half>::quiet_NaN()); });
		simple_test("numeric_limits::signaling_NaN", []() { return isnan(std::numeric_limits<half>::signaling_NaN()); });
		simple_test("numeric_limits::epsilon", []() { return nextafter(static_cast<half>(1.0f), 
			std::numeric_limits<half>::infinity())-static_cast<half>(1.0f) == std::numeric_limits<half>::epsilon(); });
		binary_test("numeric_limits::round_error", [](half a, half b) -> bool { double c = static_cast<double>(a) + 
			static_cast<double>(b); return !isfinite(a) || !isfinite(b) || c>static_cast<double>(std::numeric_limits<half>::max()) || 
			c<static_cast<double>(std::numeric_limits<half>::lowest()) || std::abs(c-static_cast<double>(
			static_cast<half>(c)))<=std::ldexp(static_cast<double>(std::numeric_limits<half>::round_error()), 
			ilogb(static_cast<half>(c))-std::numeric_limits<half>::digits+1); });

	#if HALF_ENABLE_CPP11_HASH
		//test hash
		binary_test("hash function", [](half a, half b) { return a != b || std::hash<half>()(a) == std::hash<half>()(b); });
		struct { bool operator()(half a, half b) const { return h2b(a) == h2b(b); } } bincomp;
		std::unordered_map<half,const half*,std::hash<half>,decltype(bincomp)> map(65536, std::hash<half>(), bincomp);
		unary_test("hash insert", [&map](const half &arg) { return map.insert(std::make_pair(arg, &arg)).second; });
		unary_test("hash retrieve", [&map](const half &arg) { return map[arg] == &arg; });
	#endif

	#if HALF_ENABLE_CPP11_USER_LITERALS
		//test literals
		simple_test("literals", []() -> bool { using namespace half_float::literal; return comp(0.0_h, half(0.0f)) && comp(-1.0_h, half(-1.0f)) && 
			comp(+3.14159265359_h, half(3.14159265359f)) && comp(1e-2_h, half(1e-2f)) && comp(-4.2e3_h, half(-4.2e3f)); });
	#endif
*/
		if(failed_.empty())
			log_ << "all tests passed\n";
		else
		{
			log_ << (failed_.size()) << " OF " << tests_ << " FAILED:\n    ";
			std::copy(failed_.begin(), failed_.end(), std::ostream_iterator<std::string>(log_, "\n    "));
			log_ << '\n';
		}
		return failed_.size();
	}

private:
	typedef std::vector<half> half_vector;
	typedef std::map<std::string,half_vector> test_map;
	typedef std::map<std::string,int> class_map;

	template<typename F> bool class_test(const std::string &name, F &&test)
	{
		unsigned int count = 0;
		log_ << "testing " << name << ":\n";
		for(auto iterB=halfs_.begin(); iterB!=halfs_.end(); ++iterB)
		{
			unsigned int passed = 0;
			int fpclass = classes_[iterB->first];
			for(auto iterH=iterB->second.begin(); iterH!=iterB->second.end(); ++iterH)
				passed += test(*iterH, fpclass);
			log_ << "    " << iterB->first << ": ";
			if(passed == iterB->second.size())
			{
				log_ << "all passed\n";
				++count;
			}
			else
				log_ << (iterB->second.size()-passed) << " of " << iterB->second.size() << " FAILED\n";
		}
		log_ << '\n';
		++tests_;
		if(count == halfs_.size())
			return true;
		failed_.push_back(name);
		return false;
	}

	template<typename F> bool simple_test(const std::string &name, F &&test)
	{
		log_ << "testing " << name << ": ";
		bool passed = test();
		log_ << (passed ? "passed" : "FAILED") << "\n\n";
		++tests_;
		if(!passed)
			failed_.push_back(name);
		return passed;
	}

	template<typename F> bool unary_test(const std::string &name, F &&test)
	{
		unsigned int count = 0, failed = 0;
		log_ << "testing " << name << ":\n";
		for(auto iterB=halfs_.begin(); iterB!=halfs_.end(); ++iterB)
		{
			unsigned int passed = 0;
			for(auto iterH=iterB->second.begin(); iterH!=iterB->second.end(); ++iterH)
				passed += test(*iterH);
			log_ << "    " << iterB->first << ": ";
			if(passed == iterB->second.size())
			{
				log_ << "all passed\n";
				++count;
			}
			else
			{
				failed += iterB->second.size() - passed;
				log_ << (iterB->second.size()-passed) << " of " << iterB->second.size() << " FAILED\n";
			}
		}
		if(csv_)
			*csv_ << name << ";" << failed << '\n';
		if(failed)
			log_ << failed << " FAILED\n\n";
		else
			log_ << '\n';
		++tests_;
		if(count == halfs_.size())
			return true;
		failed_.push_back(name);
		return false;
	}

	template<typename F> bool binary_test(const std::string &name, F &&test)
	{
		unsigned long long tests = 0, count = 0, step = fast_ ? 64 : 1;
		auto rand = std::bind(std::uniform_int_distribution<std::uint16_t>(0, step-1), std::default_random_engine());
		std::set<std::string> failed_tests;
		log_ << "testing " << name << (fast_ ? ": " : ":\n");
		for(auto iterB1=halfs_.begin(); iterB1!=halfs_.end(); ++iterB1)
		{
			unsigned int end1 = /*(iterB1->first.find("NaN")==std::string::npos) ?*/ iterB1->second.size() /*: 1*/;
			for(auto iterB2=halfs_.begin(); iterB2!=halfs_.end(); ++iterB2)
			{
				if(!fast_)
					std::cout << iterB1->first << " x " << iterB2->first;
				bool failed = false;
				unsigned int end2 = /*(iterB2->first.find("NaN")==std::string::npos) ?*/ iterB2->second.size() /*: 1*/;
				for(unsigned int i=0; i<end1; i+=step)
				{
					half a = iterB1->second[i];
					if(fast_ && end1 >= step)
						a = b2h(h2b(a)|rand());
					for(unsigned int j=0; j<end2; j+=step)
					{
						half b = iterB2->second[j];
						if(fast_ && end2 >= step)
							b = b2h(h2b(b)|rand());
						bool success = test(a, b);
						count += success;
						failed = failed || !success;
						++tests;
					}
				}
				if(!fast_)
					std::cout << " done\n";
				if(failed)
					failed_tests.insert(iterB1->first+" x "+iterB2->first);
			}
		}
		bool passed = count == tests;
		if(csv_)
			*csv_ << name << ";" << (tests-count) << '\n';
		if(passed)
			log_ << "all passed\n\n";
		else
		{
			log_ << (tests-count) << " of " << tests << " FAILED\n";
			for(auto &&s : failed_tests)
				log_ << s << " FAILED\n";
			log_ << '\n';
			failed_.push_back(name);
		}
		++tests_;
		return passed;
	}

	template<typename F> bool ternary_test(const std::string &name, F &&test)
	{
		unsigned long long tests = 0, count = 0, step = fast_ ? 256 : 1;
		auto rand = std::bind(std::uniform_int_distribution<std::uint16_t>(0, step-1), std::default_random_engine());
		std::set<std::string> failed_tests;
		log_ << "testing " << name << ": ";
		for(auto iterB1=halfs_.begin(); iterB1!=halfs_.end(); ++iterB1)
		{
			unsigned int end1 = /*(iterB1->first.find("NaN")==std::string::npos) ?*/ iterB1->second.size() /*: 1*/;
			for(auto iterB2=halfs_.begin(); iterB2!=halfs_.end(); ++iterB2)
			{
				unsigned int end2 = /*(iterB2->first.find("NaN")==std::string::npos) ?*/ iterB2->second.size() /*: 1*/;
				for(auto iterB3=halfs_.begin(); iterB3!=halfs_.end(); ++iterB3)
				{
					bool failed = false;
					unsigned int end3 = /*(iterB3->first.find("NaN")==std::string::npos) ?*/ iterB3->second.size() /*: 1*/;
					for(unsigned int i=0; i<end1; i+=step)
					{
						half a = iterB1->second[i];
						if(fast_ && end1 >= step)
							a = b2h(h2b(a)|rand());
						for(unsigned int j=0; j<end2; j+=step)
						{
							half b = iterB2->second[j];
							if(fast_ && end2 >= step)
								b = b2h(h2b(b)|rand());
							for(unsigned int k=0; k<end3; k+=step)
							{
								half c = iterB3->second[k];
								if(fast_ && end3 >= step)
									c = b2h(h2b(c)|rand());
								bool success = test(a, b, c);
								count += success;
								failed = failed || !success;
								++tests;
							}
						}
					}
					if(failed)
						failed_tests.insert(iterB1->first+" x "+iterB2->first+" x "+iterB3->first);
				}
			}
		}
		bool passed = count == tests;
		if(csv_)
			*csv_ << name << ";" << (tests-count) << '\n';
		if(passed)
			log_ << "all passed\n\n";
		else
		{
			log_ << (tests-count) << " of " << tests << " failed\n\n";
			for(auto &&s : failed_tests)
				log_ << s << " FAILED\n";
			log_ << '\n';
			failed_.push_back(name);
		}
		++tests_;
		return passed;
	}

	template<typename F> bool unary_except_test(const std::string &name, F &&fn)
	{
		return unary_test(name, [&](half arg) -> bool {
		#if HALF_ENABLE_CPP11_CFENV && CHECK_EXCEPT
			half_float::feclearexcept(CHECK_EXCEPT);
			std::feclearexcept(CHECK_EXCEPT);
			bool equal = fn(arg);
			int eh = half_float::fetestexcept(CHECK_EXCEPT), ed = std::fetestexcept(CHECK_EXCEPT) | (eh&(FE_OVERFLOW|FE_UNDERFLOW|FE_INEXACT));
//			if(eh != ed)
//				std::cerr << arg << " = " << std::hex << eh << " vs " << ed << std::dec << '\n';
			return equal && eh == ed;
		#else
			return fn(arg);
		#endif
		});
	}

	template<typename F> bool binary_except_test(const std::string &name, F &&fn)
	{
		return binary_test(name, [&](half x, half y) -> bool {
		#if HALF_ENABLE_CPP11_CFENV && CHECK_EXCEPT
			half_float::feclearexcept(CHECK_EXCEPT);
			std::feclearexcept(CHECK_EXCEPT);
			bool equal = fn(x, y);
			int eh = half_float::fetestexcept(CHECK_EXCEPT), ed = std::fetestexcept(CHECK_EXCEPT) | (eh&(FE_OVERFLOW|FE_UNDERFLOW|FE_INEXACT));
//			if(eh != ed)
//				std::cerr << std::hex << h2b(x) << ", " << h2b(y) << " = " << eh << " vs " << ed << std::dec << '\n';
			return equal && eh == ed;
		#else
			return fn(x, y);
		#endif
		});
	}

	template<typename F> bool ternary_except_test(const std::string &name, F &&fn)
	{
		return ternary_test(name, [&](half x, half y, half z) -> bool {
		#if HALF_ENABLE_CPP11_CFENV && CHECK_EXCEPT
			half_float::feclearexcept(CHECK_EXCEPT);
			std::feclearexcept(CHECK_EXCEPT);
			bool equal = fn(x, y, z);
			int eh = half_float::fetestexcept(CHECK_EXCEPT), ed = std::fetestexcept(CHECK_EXCEPT) | (eh&(FE_OVERFLOW|FE_UNDERFLOW|FE_INEXACT));
			return equal && eh == ed;
		#else
			return fn(x, y, z);
		#endif
		});
	}

	template<typename FH,typename FD> bool unary_double_test(const std::string &name, FH &&fh, FD &&fd)
	{
		double err = 0.0, rel = 0.0; int bin = 0;
		bool success = unary_test(name, [&](half arg) -> bool {
		#if HALF_ENABLE_CPP11_CFENV && CHECK_EXCEPT
			half_float::feclearexcept(CHECK_EXCEPT);
			half a = fh(arg);
			int eh = half_float::fetestexcept(CHECK_EXCEPT);
			std::feclearexcept(CHECK_EXCEPT);
			half b = half_cast<half>(fd(half_cast<double>(arg)));
			int ed = std::fetestexcept(CHECK_EXCEPT) | (eh&(FE_OVERFLOW|FE_UNDERFLOW|FE_INEXACT));
			if(eh != ed)
				std::cerr << std::hex << h2b(arg) << " = " << eh << " vs " << ed << std::dec << '\n';
			return comp(a, b) && eh == ed;
		#else
			half a = fh(arg), b = half_cast<half>(fd(half_cast<double>(arg)));
			bool equal = comp(a, b);
			if(!equal)
			{
				double error = std::abs(static_cast<double>(a)-static_cast<double>(b));
				err = std::max(err, error); rel = std::max(rel, error/std::abs(b)); bin = std::max(bin, std::abs(h2b(a)-h2b(b)));
			}
			return equal;
		#endif
		});
		if(err != 0.0 || rel != 0.0 || bin != 0)
			std::cout << name << " max error: " << err << ", max relative error: " << rel << ", max ulp error: " << /*ilog2*/(bin) << '\n';
		return success;
	}

	template<typename FH,typename FD> bool binary_double_test(const std::string &name, FH &&fh, FD &&fd)
	{
		double err = 0.0, rel = 0.0; int bin = 0;
		bool success = binary_test(name, [&](half x, half y) -> bool {
		#if HALF_ENABLE_CPP11_CFENV && CHECK_EXCEPT
			half_float::feclearexcept(CHECK_EXCEPT);
			half a = fh(x, y);
			int eh = half_float::fetestexcept(CHECK_EXCEPT);
			std::feclearexcept(CHECK_EXCEPT);
			half b = half_cast<half>(fd(half_cast<double>(x), half_cast<double>(y)));
			int ed = std::fetestexcept(CHECK_EXCEPT) | (eh&(FE_OVERFLOW|FE_UNDERFLOW|FE_INEXACT));
//			if(eh != ed)
//				std::cerr << std::hex << h2b(x) << ", " << h2b(y) << " = " << eh << " vs " << ed << std::dec << '\n';
			return comp(a, b) && eh == ed;
		#else
			half a = fh(x, y), b = half_cast<half>(fd(half_cast<double>(x), half_cast<double>(y)));
			bool equal = comp(a, b);
			if(!equal)
			{
				double error = std::abs(static_cast<double>(a)-static_cast<double>(b));
//				if(std::abs(h2b(a)-h2b(b)) > 1)
//				std::cerr << std::hex << x << ", " << y << " = " << a << '(' << h2b(a) << "), " << b << '(' << h2b(b) << ") -> " << error << std::dec << ' ' << (ilogb(x)-ilogb(y)) << '\n';
				err = std::max(err, error); rel = std::max(rel, error/std::abs(b)); bin = std::max(bin, std::abs(h2b(a)-h2b(b)));
			}
			return equal;
		#endif
		});
		if(err != 0.0 || rel != 0.0 || bin != 0)
			std::cout << name << " max error: " << err << ", max relative error: " << rel << ", max ulp error: " << /*ilog2*/(bin) << '\n';
		return success;
	}

	template<typename FH,typename FD> bool ternary_double_test(const std::string &name, FH &&fh, FD &&fd)
	{
		double err = 0.0, rel = 0.0; int bin = 0;
		bool success = ternary_test(name, [&](half x, half y, half z) -> bool {
		#if HALF_ENABLE_CPP11_CFENV && CHECK_EXCEPT
			half_float::feclearexcept(CHECK_EXCEPT);
			half a = fh(x, y, z);
			int eh = half_float::fetestexcept(CHECK_EXCEPT);
			std::feclearexcept(CHECK_EXCEPT);
			half b = half_cast<half>(fd(half_cast<double>(x), half_cast<double>(y), half_cast<double>(z)));
			int ed = std::fetestexcept(CHECK_EXCEPT) | (eh&(FE_OVERFLOW|FE_UNDERFLOW|FE_INEXACT));
//			if(eh != ed)
//				std::cerr << std::hex << h2b(x) << ", " << h2b(y) << ", " << h2b(z) << " = " << eh << " vs " << ed << std::dec << '\n';
			return comp(a, b) && eh == ed;
		#else
			half a = fh(x, y, z), b = half_cast<half>(fd(half_cast<double>(x), half_cast<double>(y), half_cast<double>(z)));
			bool equal = comp(a, b);
			if(!equal)
			{
				double error = std::abs(static_cast<double>(a)-static_cast<double>(b));
				err = std::max(err, error); rel = std::max(rel, error/std::abs(b)); bin = std::max(bin, std::abs(h2b(a)-h2b(b)));
			}
			return equal;
		#endif
		});
		if(err != 0.0 || rel != 0.0 || bin != 0)
			std::cout << name << " max error: " << err << ", max relative error: " << rel << ", max ulp error: " << /*ilog2*/(bin) << '\n';
		return success;
	}

	template<typename F> bool unary_reference_test(const std::string &name, F &&fn)
	{
		std::ifstream in("reference/"+name, std::ios_base::in|std::ios_base::binary);
		if(!in)
			throw std::runtime_error("cannot open reference file for "+name);
		double err = 0.0, rel = 0.0; int bin = 0;
		std::vector<results> reference(0x10000);
		for(auto &ref : reference)
			ref.read(in);
		bool success = unary_test(name, [&](half arg) -> bool {
			auto ref = reference[h2b(arg)];
			half_float::feclearexcept(CHECK_EXCEPT);
			half a = fn(arg), b = ref.value();
			int ea = half_float::fetestexcept(CHECK_EXCEPT), eb = ref.except(CHECK_EXCEPT);
			bool equal = comp(a, b);
			if(!equal)
			{
				double error = std::abs(static_cast<double>(a)-static_cast<double>(b));
//				if(std::abs(h2b(a)-h2b(b)) > 1)
//				if(std::isinf(error/std::abs(b)))
//				std::cerr << std::hex << arg << '(' << h2b(arg) << ") = " << a << '(' << h2b(a) << "), " << b << '(' << h2b(b) << ") -> " << error << std::dec << '\n';
				err = std::max(err, error); rel = std::max(rel, error/std::abs(b)); bin = std::max(bin, std::abs(h2b(a)-h2b(b)));
			}
			if(ea != eb)
			{
//				std::cerr << std::hex << arg << '(' << h2b(arg) << ") = " << a << '(' << h2b(a) << "), " << b << '(' << h2b(b) << ") @ " << ea << " vs " << eb << " -> " << (ea^eb) << '\n' << std::dec;
			}
			return equal && ea == eb;
		});
		if(err != 0.0 || rel != 0.0 || bin != 0)
			std::cout << name << " max error: " << err << ", max relative error: " << rel << ", max ulp error: " << /*ilog2*/(bin) << '\n';
		return success;
	}

	template<typename F> bool binary_reference_test(const std::string &name, F &&fn)
	{
		std::ifstream in("reference/"+name, std::ios_base::in|std::ios_base::binary|std::ios_base::ate);
		if(!in)
			throw std::runtime_error("cannot open reference file for "+name);
		unsigned int passed = 0, count = in.tellg() / 7;
		in.seekg(0, std::ios_base::beg);
		in.clear();
		double err = 0.0, rel = 0.0; int bin = 0;
		bool success = simple_test(name, [&]() -> bool {
			for(unsigned int i=0; i<count; ++i)
			{
				half x, y;
				results ref;
				in.read(reinterpret_cast<char*>(&x), sizeof(x));
				in.read(reinterpret_cast<char*>(&y), sizeof(y));
				ref.read(in);
				half_float::feclearexcept(CHECK_EXCEPT);
				half a = fn(x, y), b = ref.value();
				int ea = half_float::fetestexcept(CHECK_EXCEPT), eb = ref.except(CHECK_EXCEPT);
				bool equal = comp(a, b);
				if(!equal)
				{
					double error = std::abs(static_cast<double>(a)-static_cast<double>(b));
//					if(std::abs(h2b(a)-h2b(b)) > 1)
//					std::cerr << std::hex << x << ", " << y << " = " << a << '(' << h2b(a) << "), " << b << '(' << h2b(b) << ") -> " << error << std::dec << ' ' << (ilogb(x)-ilogb(y)) << '\n';
					err = std::max(err, error); rel = std::max(rel, error/std::abs(b)); bin = std::max(bin, std::abs(h2b(a)-h2b(b)));
				}
				if(ea != eb)
				{
//					std::cerr << std::hex << x << '(' << h2b(x) << "), " << y << '(' << h2b(y) << ") = " << a << '(' << h2b(a) << "), " << b << '(' << h2b(b) << ") @ " << ea << " vs " << eb << " -> " << (ea^eb) << '\n' << std::dec;
				}
				passed += equal && ea == eb;;
			}
			if(csv_)
				*csv_ << name << ";" << (count-passed) << '\n';
			return passed == count;
		});
		if(passed != count)
			std::cout << name << ": " << (count-passed) << " of " << count << " failed\n";
		if(err != 0.0 || rel != 0.0 || bin != 0)
			std::cout << name << " max error: " << err << ", max relative error: " << rel << ", max ulp error: " << /*ilog2*/(bin) << '\n';
		return success;
	}

	template<typename F> bool ternary_reference_test(const std::string &name, F &&fn)
	{
		struct record { half x, y, z; std::pair<half,half> result; };
		std::ifstream in("reference/"+name, std::ios_base::in|std::ios_base::binary|std::ios_base::ate);
		if(!in)
			throw std::runtime_error("cannot open reference file for "+name);
		unsigned int passed = 0, count = in.tellg() / 9;
		in.seekg(0, std::ios_base::beg);
		in.clear();
		double err = 0.0, rel = 0.0; int bin = 0;
		results ref;
		bool success = simple_test(name, [&]() -> bool {
			for(unsigned int i=0; i<count; ++i)
			{
				half x, y, z;
				results ref;
				in.read(reinterpret_cast<char*>(&x), sizeof(x));
				in.read(reinterpret_cast<char*>(&y), sizeof(y));
				in.read(reinterpret_cast<char*>(&z), sizeof(z));
				ref.read(in);
				half a = fn(x, y, z), b = ref.value();
				bool equal = comp(a, b);
				if(!equal)
				{
					double error = std::abs(static_cast<double>(a)-static_cast<double>(b));
//					std::cerr << std::hex << x << ", " << y << ", " << z << " = " << a << '(' << h2b(a) << "), " << b << '(' << h2b(b) << ") -> " << error << std::dec << '\n';
					err = std::max(err, error); rel = std::max(rel, error/std::abs(b)); bin = std::max(bin, std::abs(h2b(a)-h2b(b)));
				}
				passed += equal;
			}
			if(csv_)
				*csv_ << name << ";" << (count-passed) << '\n';
			return passed == count;
		});
		if(passed != count)
			std::cout << name << ": " << (count-passed) << " of " << count << " failed\n";
		if(err != 0.0 || rel != 0.0 || bin != 0)
			std::cout << name << " max error: " << err << ", max relative error: " << rel << ", max ulp error: " << /*ilog2*/(bin) << '\n';
		return success;
	}

	template<typename F> bool float_test(const std::string &name, F &&test)
	{
		auto rand32 = std::bind(std::uniform_int_distribution<std::uint32_t>(0, std::numeric_limits<std::uint32_t>::max()), std::default_random_engine());
		unsigned long long count = 0, tests = fast_ ? 1e6 : (1ULL<<32);
		log_ << "testing " << name << ": ";
		if(fast_)
		{
			for(unsigned long long i=0; i<tests; ++i)
			{
				std::uint32_t u = rand32();
				count += test(*reinterpret_cast<float*>(&u));
			}
		}
		else
			for(std::uint32_t i=0; i++>0; )
				count += test(*reinterpret_cast<float*>(&i));
		bool passed = count == tests;
		if(passed)
			log_ << "all passed\n\n";
		else
		{
			log_ << (tests-count) << " of " << tests << " FAILED\n\n";
			failed_.push_back(name);
		}
		++tests_;
		return passed;
	}

	template<typename F> bool int_test(const std::string &name, F &&test)
	{
		unsigned int count = 0, tests = (1<<17) + 1;
		log_ << "testing " << name << ": ";
		for(int i=-(1<<16); i<=(1<<16); ++i)
			count += test(i);
		bool passed = count == tests;
		if(passed)
			log_ << "all passed\n\n";
		else
		{
			log_ << (tests-count) << " of " << tests << " FAILED\n\n";
			failed_.push_back(name);
		}
		++tests_;
		return passed;
	}

	test_map halfs_;
	class_map classes_;
	unsigned int tests_;
	std::vector<std::string> failed_;
	std::ostream &log_;
	std::ostream *csv_;
	bool fast_;
	bool rough_;
};

struct timer
{
	timer() : start_(std::chrono::high_resolution_clock::now()) {}
	~timer() { std::cout << "time: " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()-start_).count() << " ms\n"; }

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> start_;
};

int main(int argc, char *argv[]) try
{
#ifndef HALF_ARITHMETIC_TYPE
	switch(std::numeric_limits<half>::round_style)
	{
	#ifdef _WIN32
		case std::round_to_nearest: _controlfp(_MCW_RC, _RC_NEAR); break;
		case std::round_toward_zero: _controlfp(_MCW_RC, _RC_CHOP); break;
		case std::round_toward_infinity: _controlfp(_MCW_RC, _RC_UP); break;
		case std::round_toward_neg_infinity: _controlfp(_MCW_RC, _RC_DOWN); break;
	#else
		case std::round_to_nearest: std::fesetround(FE_TONEAREST); break;
		case std::round_toward_zero: std::fesetround(FE_TOWARDZERO); break;
		case std::round_toward_infinity: std::fesetround(FE_UPWARD); break;
		case std::round_toward_neg_infinity: std::fesetround(FE_DOWNWARD); break;
	#endif
	}
#endif
/*
	auto rand_abs = std::bind(std::uniform_int_distribution<std::uint32_t>(0x00000000, 0x7F100000), std::default_random_engine());
	auto rand_sign = std::bind(std::uniform_int_distribution<std::uint32_t>(0, 1), std::default_random_engine());
	std::vector<float> floats;
	for(unsigned int i=0; i<1e8; ++i)
	{
		auto bits = rand_abs() | (rand_sign()<<31);
		floats.push_back(*reinterpret_cast<float*>(&bits));
	}
	std::shuffle(floats.begin(), floats.end(), std::default_random_engine());
	std::vector<half> halfs(floats.size());
	{
		timer time;
		for(std::size_t i=0; i<floats.size(); ++i)
			halfs[i] = half_cast<half,std::round_to_nearest>(floats[i]);
	}
	return 0;

	half pi = half_cast<half>(3.1415926535897932384626433832795l);
	std::cout << "Pi: " << pi << " - 0x" << std::hex << std::setfill('0') << std::setw(4) << h2b(pi) << std::dec 
		<< " - " << std::bitset<16>(static_cast<unsigned long long>(h2b(pi))).to_string() << std::endl;
	half e = half_cast<half>(2.7182818284590452353602874713527l);
	std::cout << "e:  " << e << " - 0x" << std::hex << std::setfill('0') << std::setw(4) << h2b(e) << std::dec 
		<< " - " << std::bitset<16>(static_cast<unsigned long long>(h2b(e))).to_string() << std::endl;

	static const long double logs[] = {
		1.0000000000000000000000000000000000000000000000000000000000000000000000000000L, 0.5849625007211561814537389439478165087598144076924810604557526545410982276485L,
		0.3219280948873623478703194294893901758648313930245806120547563958159347765589L, 0.1699250014423123629074778878956330175196288153849621209115053090821964552970L,
		0.0874628412503394082540660108104043540112672823448206881266090643866965081686L, 0.0443941193584534376531019906736094674630459333742491317685543002674288465967L,
		0.0223678130284545082671320837460849094932677948156179815932199216587899627785L, 0.0112272554232541203378805844158839407281095943600297940811823651462712311786L,
		0.0056245491938781069198591026740666017211096815383520359072957784732489771013L, 0.0028150156070540381547362547502839489729507927389771959487826944878598909400L,
		0.0014081943928083889066101665016890524233311715793462235597709051792834906001L, 0.0007042690112466432585379340422201964456668872087249334581924550139514213168L,
		0.0003521774803010272377989609925281744988670304302127133979341729842842377649L, 0.0001760994864425060348637509459678580940163670081839283659942864068257522373L,
		0.0000880524301221769086378699983597183301490534085738474534831071719854721939L, 0.0000440268868273167176441087067175806394819146645511899503059774914593663365L,
		0.0000220136113603404964890728830697555571275493801909791504158295359319433723L, 0.0000110068476674814423006223021573490183469930819844945565597452748333526464L,
		0.0000055034343306486037230640321058826431606183125807276574241540303833251704L, 0.0000027517197895612831123023958331509538486493412831626219340570294203116559L,
		0.0000013758605508411382010566802834037147561973553922354232704569052932922954L, 0.0000006879304394358496786728937442939160483304056131990916985043387874690617L,
		0.0000003439652607217645360118314743718005315334062644619363447395987584138324L, 0.0000001719826406118446361936972479533123619972434705828085978955697643547921L,
		0.0000000859913228686632156462565208266682841603921494181830811515318381744650L, 0.0000000429956620750168703982940244684787907148132725669106053076409624949917L,
		0.0000000214978311976797556164155504126645192380395989504741781512309853438587L, 0.0000000107489156388827085092095702361647949603617203979413516082280717515504L,
		0.0000000053744578294520620044408178949217773318785601260677517784797554422804L, 0.0000000026872289172287079490026152352638891824761667284401180026908031182361L,
		0.0000000013436144592400232123622589569799954658536700992739887706412976115422L, 0.0000000006718072297764289157920422846078078155859484240808550018085324187007L };
	std::ofstream out("logs.txt");
	for(auto val : logs)
		out << "0x" << std::hex << std::uppercase << std::setfill('0') << std::setw(8) << std::llrint(std::ldexp(val, 27)) << ", \n";
	return 0;

	using namespace half_float::literal;
	std::cout << "0x" << std::hex << std::uppercase << std::setfill('0') << std::setw(8) << std::llrint(std::ldexp(0.6072529350088812561694l, 30)) << '\n';
	std::ofstream out("atans.txt");
	for(int i=0; i<32; ++i)
		out << "0x" << std::hex << std::uppercase << std::setfill('0') << std::setw(8) << std::llrint(std::ldexp(std::atan(std::ldexp(1.0l, -i)), 30)) << ", \n";
	return 0;

	for(std::uint16_t i=0x3C00; i<0x7C00; ++i)
	{
		half x = b2h(i), y = half_cast<half,std::round_toward_neg_infinity>(std::erfc(half_cast<double>(x)));
		std::cout << x << " (" << std::hex << std::uppercase << std::setfill('0') << std::setw(4) << i << std::dec << ")\t= " << y << '\n';
		if(y == 0.0_h)
			return 0;
	}

	std::cout << std::hex << std::uppercase << std::setfill('0') << std::setw(9) << std::llrint(std::ldexp(3.15l, 31-1)) << '\n';
	std::cout << std::hex << std::uppercase << std::setfill('0') << std::setw(9) << std::llrint(std::ldexp(3.85l, 31-1)) << '\n';
	std::cout << std::hex << std::uppercase << std::setfill('0') << std::setw(9) << std::llrint(std::ldexp(4.65l, 31-2)) << '\n';
	return 0;

	for(std::uint16_t i=0xBC00; i<0xFC00; ++i)
	{
		half x = b2h(i), y = half_cast<half, std::round_to_nearest>(std::exp2(half_cast<double>(x)));
		std::cout << x << " (" << std::hex << std::uppercase << std::setfill('0') << std::setw(4) << i << std::dec << ")\t= " << y << '\n';
		if(y == 0.0_h)
			return 0;
	}
*/
	std::vector<std::string> args(argv+1, argv+argc);
	std::unique_ptr<std::ostream> file, csv;
	bool fast = false, rough = false;
	for(auto &&arg : args)
	{
		if(arg == "-fast")
			fast = true;
		else if(arg == "-rough")
			rough = true;
		else if(arg.length() > 4 && arg.substr(arg.length()-4) == ".csv")
			csv.reset(new std::ofstream(arg));
		else
			file.reset(new std::ofstream(arg));
	}
	half_test test(file ? *file : std::cout, csv.get(), fast, rough);

//	performance_test(file ? *file : std::cout, csv.get());

	timer time;
	return test.test();
}
catch(const std::exception &e)
{
	std::cerr << "ERROR: " << e.what() << '\n';
	return -1;
}
