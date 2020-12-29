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
#define HALF_ROUND_STYLE 1
#include <half.hpp>
using half_float::half;

#include <utility>
#include <vector>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <functional>
#include <random>
#include <chrono>


#define UNARY_PERFORMANCE_TEST(func, x, N) { \
	auto start = std::chrono::high_resolution_clock::now(); \
	for(unsigned int i=0; i<N; ++i) for(unsigned int h=0; h<x.size(); ++h) results[h] = func(x[h]); \
	auto diff = std::chrono::high_resolution_clock::now() - start; auto tm = std::chrono::duration_cast<std::chrono::milliseconds>(diff).count(); \
	auto ops = double(x.size() * N) / std::chrono::duration_cast<std::chrono::microseconds>(diff).count(); \
	out << #func << "\tx " << N << ":\t" << tm << " ms \t-> " << ops << " MFLOPS\n\n"; if(csv) *csv << #func << ';' << ops << '\n'; }

#define BINARY_PERFORMANCE_TEST(func, x, y, N) { \
	auto start = std::chrono::high_resolution_clock::now(); \
	for(unsigned int i=0; i<x.size(); i+=N) for(unsigned int j=0; j<y.size(); j+=N) results[j] = func(x[i], y[j]); \
	auto diff = std::chrono::high_resolution_clock::now() - start; auto tm = std::chrono::duration_cast<std::chrono::milliseconds>(diff).count(); \
	auto ops = double(x.size() / N) * double(y.size() / N) / std::chrono::duration_cast<std::chrono::microseconds>(diff).count(); \
	out << #func << "\t@ 1/" << (N*N) << ":\t" << tm << " ms \t-> " << ops << " MFLOPS\n\n"; if(csv) *csv << #func << ';' << ops << '\n'; }

#define OPERATOR_PERFORMANCE_TEST(op, x, y, N) { \
	auto start = std::chrono::high_resolution_clock::now(); \
	for(unsigned int i=0; i<x.size(); i+=N) for(unsigned int j=0; j<y.size(); j+=N) results[j] = x[i] op y[j]; \
	auto diff = std::chrono::high_resolution_clock::now() - start; auto tm = std::chrono::duration_cast<std::chrono::milliseconds>(diff).count(); \
	auto ops = double(x.size() / N) * double(y.size() / N) / std::chrono::duration_cast<std::chrono::microseconds>(diff).count(); \
	out << #op << "\t@ 1/" << (N*N) << ":\t" << tm << " ms \t-> " << ops << " MFLOPS\n\n"; if(csv) *csv << #op << ';' << ops << '\n'; }

#define TERNARY_PERFORMANCE_TEST(func, x, y, z, N) { \
	auto start = std::chrono::high_resolution_clock::now(); \
	for(unsigned int i=0; i<x.size(); i+=N) for(unsigned int j=0; j<y.size(); j+=N) for(unsigned int k=0; k<z.size(); k+=N) results[k] = func(x[i], y[j], z[k]); \
	auto diff = std::chrono::high_resolution_clock::now() - start; auto tm = std::chrono::duration_cast<std::chrono::milliseconds>(diff).count(); \
	auto ops = double(x.size() / N) * double(y.size() / N) * double(z.size() / N) / std::chrono::duration_cast<std::chrono::microseconds>(diff).count(); \
	out << #func << "\t@ 1/" << (N*N*N) << ":\t" << tm << " ms \t-> " << ops << " MFLOPS\n\n"; if(csv) *csv << #func << ';' << ops << '\n'; }


void performance_test(std::ostream &out = std::cout, std::ostream *csv = nullptr)
{
	std::vector<half> finite, positive, one2one, one2inf, neg2inf;
	for(std::uint16_t u=0; u<0x7C00; ++u)
	{
		half pos = *reinterpret_cast<half*>(&u), neg = -pos;
		finite.push_back(pos);
		finite.push_back(neg);
		positive.push_back(pos);
		neg2inf.push_back(pos);
		if(u <= 0x3C00)
		{
			one2one.push_back(pos);
			one2one.push_back(neg);
			neg2inf.push_back(neg);
		}
		else
			one2inf.push_back(pos);
	}
	std::vector<half> xs(finite), ys(finite), zs(finite), results(finite.size());
	std::default_random_engine g;
	std::shuffle(finite.begin(), finite.end(), g);
	std::shuffle(positive.begin(), positive.end(), g);
	std::shuffle(one2one.begin(), one2one.end(), g);
	std::shuffle(one2inf.begin(), one2inf.end(), g);
	std::shuffle(neg2inf.begin(), neg2inf.end(), g);
	std::shuffle(xs.begin(), xs.end(), g);
	std::shuffle(ys.begin(), ys.end(), g);
	std::shuffle(zs.begin(), zs.end(), g);

	if( csv )
		*csv << std::fixed << std::setprecision(3);

	OPERATOR_PERFORMANCE_TEST(+, xs, ys, 4);
	OPERATOR_PERFORMANCE_TEST(-, xs, ys, 4);
	OPERATOR_PERFORMANCE_TEST(*, xs, ys, 4);
	OPERATOR_PERFORMANCE_TEST(/, xs, ys, 4);

	BINARY_PERFORMANCE_TEST(fdim, xs, ys, 8);
	TERNARY_PERFORMANCE_TEST(fma, xs, ys, zs, 64);

	UNARY_PERFORMANCE_TEST(exp, finite, 1000);
	UNARY_PERFORMANCE_TEST(exp2, finite, 1000);
	UNARY_PERFORMANCE_TEST(expm1, finite, 1000);
	UNARY_PERFORMANCE_TEST(log, positive, 1000);
	UNARY_PERFORMANCE_TEST(log10, positive, 1000);
	UNARY_PERFORMANCE_TEST(log2, positive, 1000);
	UNARY_PERFORMANCE_TEST(log1p, neg2inf, 1000);

	UNARY_PERFORMANCE_TEST(sqrt, positive, 1000);
	UNARY_PERFORMANCE_TEST(rsqrt, positive, 1000);
	UNARY_PERFORMANCE_TEST(cbrt, finite, 1000);
	BINARY_PERFORMANCE_TEST(pow, xs, ys, 8);
	BINARY_PERFORMANCE_TEST(hypot, xs, ys, 8);

	UNARY_PERFORMANCE_TEST(sin, finite, 1000);
	UNARY_PERFORMANCE_TEST(cos, finite, 1000);
	UNARY_PERFORMANCE_TEST(tan, finite, 1000);
	UNARY_PERFORMANCE_TEST(asin, one2one, 1000);
	UNARY_PERFORMANCE_TEST(acos, one2one, 1000);
	UNARY_PERFORMANCE_TEST(atan, finite, 1000);
	BINARY_PERFORMANCE_TEST(atan2, xs, ys, 8);

	UNARY_PERFORMANCE_TEST(sinh, finite, 1000);
	UNARY_PERFORMANCE_TEST(cosh, finite, 1000);
	UNARY_PERFORMANCE_TEST(tanh, finite, 1000);
	UNARY_PERFORMANCE_TEST(asinh, finite, 1000);
	UNARY_PERFORMANCE_TEST(acosh, one2inf, 1000);
	UNARY_PERFORMANCE_TEST(atanh, one2one, 1000);

	UNARY_PERFORMANCE_TEST(erf, finite, 1000);
	UNARY_PERFORMANCE_TEST(erfc, finite, 1000);
	UNARY_PERFORMANCE_TEST(lgamma, finite, 1000);
	UNARY_PERFORMANCE_TEST(tgamma, finite, 1000);

	if( csv )
		*csv << std::defaultfloat << std::setprecision(6);
}
