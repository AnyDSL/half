# Half-precision floating-point library						{#mainpage}

This is a C++ header-only library to provide an [IEEE 754](http://en.wikipedia.org/wiki/IEEE_754-2008) conformant 16-bit [half-precision](http://en.wikipedia.org/wiki/Half_precision_floating-point_format) floating-point type along with corresponding arithmetic operators, type conversions and common mathematical functions. It aims for both efficiency and ease of use, trying to accurately mimic the behaviour of the built-in floating-point types at the best performance possible. It is hosted on [SourceForge.net](http://sourceforge.net/projects/half/).

[TOC]


# News														{#new}

XXX X, 2019 - Release 2.1.0: IEEE-conformant exception handling
---------------------------------------------------------------

[Version 2.1.0](https://sourceforge.net/projects/half/files/half/2.1.0) of the library has been released. It adds proper detection of IEEE floating-point exceptions to all required operators and functions. In addition to this it also adds support for managing the exception flags and various means for additional processing of floating-point exceptions, like propagating them to the built-in floating-point platform or throwing C++ exceptions. For performance reasons all exception detection and handling is disabled by default and has to be enabled explicitly, though.

July 23, 2019 - Release 2.0.0: Clean half-precision computations
----------------------------------------------------------------

[Version 2.0.0](https://sourceforge.net/projects/half/files/half/2.0.0) of the library has been released. It marks a major change in its internal implementation by implementing all operators and mathematical functions directly in half-precision without employing the built-in single- or double-precision implementation and without keeping temporary results as part of lenghtier statements in single-precision. This makes for a much cleaner implementation giving more reliable and IEEE-conformant computation results. Furthermore, and this marks a slight deviation from the previous interface, the default rounding mode has been changed to rounding to nearest, but is of course still configurable at compile-time. What isn't configurable anymore is the tie-breaking behaviour, which now always rounds ties to even as any proper floating-point implementation does. In addition to these major cleanups there are a few new features. The `constexpr` support has been extended, primarily to comparisons, classifications and simple sign management functions (however, there are still no constant expression literals yet). The conversion functions can be accelerated by [F16C instructions](https://en.wikipedia.org/wiki/F16C) if supported. The C++11 feature detection has also been extended to Intel compilers (which hasn't been tested yet, though, so feedback is welcome).

[more](news.html)


---------------------------
# Download and Installation									{#downloads}

The library in its most recent version can be obtained from here, see the [Release Notes](changelog.html) for further information:

###[**Download half 2.0.0 (.zip)**](https://sourceforge.net/projects/half/files/latest/download)

If you are interested in previous versions of the library, see the [SourceForge download page](https://sourceforge.net/projects/half/files/half).

Conveniently, the library consists of just a single header file containing all the functionality, which can be directly included by your projects, without the neccessity to build anything or link to anything.

Whereas this library is fully C++98-compatible, it can profit from certain C++11 features. Support for those features is checked and enabled automatically at compile (or rather preprocessing) time, but can be explicitly enabled or disabled by predefining the corresponding preprocessor symbols to either 1 or 0 yourself before including half.hpp. This is useful when the automatic detection fails (for more exotic implementations) or when a feature should be explicitly disabled:

C++11 feature                         | used for                             | enabled for (and newer)                         | override with
--------------------------------------|--------------------------------------|-------------------------------------------------|----------------------------------
`long long` integer type              | functions returning `long long`      | *VC++ 2003*, *gcc*, *clang*, *icc 11.1*         | `HALF_ENABLE_CPP11_LONG_LONG`
static assertions                     | extended compile-time checks         | *VC++ 2010*, *gcc 4.3*, *clang 2.9*, *icc 11.1* | `HALF_ENABLE_CPP11_STATIC_ASSERT`
generalized constant expressions      | constant operations                  | *VC++ 2015*, *gcc 4.6*, *clang 3.1*, *icc 14.0* | `HALF_ENABLE_CPP11_CONSTEXPR`
`noexcept` specifications             | proper `noexcept` functions          | *VC++ 2015*, *gcc 4.6*, *clang 3.0*, *icc 14.0* | `HALF_ENABLE_CPP11_NOEXCEPT`
user-defined literals                 | half-precision literals              | *VC++ 2015*, *gcc 4.7*, *clang 3.1*, *icc 15.0* | `HALF_ENABLE_CPP11_USER_LITERALS`
thread-local storage                  | thread-local exception flags         | *VC++ 2015*, *gcc 4.8*, *clang 3.3*, *icc 15.0* | `HALF_ENABLE_CPP11_THREAD_LOCAL`
type traits from `<type_traits>`      | TMP and extended checks              | *VC++ 2010*, *libstdc++ 4.3*, <i>libc++</i>     | `HALF_ENABLE_CPP11_TYPE_TRAITS`
sized integer types from `<cstdint>`  | more flexible type sizes             | *VC++ 2010*, *libstdc++ 4.3*, <i>libc++</i>     | `HALF_ENABLE_CPP11_CSTDINT`
certain new `<cmath>` functions       | classifications during conversions   | *VC++ 2013*, *libstdc++ 4.3*, <i>libc++</i>     | `HALF_ENABLE_CPP11_CMATH`
floating-point control from `<cfenv>` | floating-point exception propagation | *VC++ 2013*, *libstdc++ 4.3*, <i>libc++</i>     | `HALF_ENABLE_CPP11_CFENV`
`std::hash` from `<functional>`       | hash function for halfs              | *VC++ 2010*, *libstdc++ 4.3*, <i>libc++</i>     | `HALF_ENABLE_CPP11_HASH`

The library has been tested successfully with *Visual C++ 2005* - *2015*, *gcc 4* - *8* and *clang 3* - *8* on 32- and 64-bit x86 systems. Please [contact me](#contact) if you have any problems, suggestions or even just success testing it on other platforms.


---------------
# Documentation												{#doc}

What follows are some general words about the usage of the library and its implementation. For a complete reference documentation of its interface you should consult the [API Documentation](usergroup0.html).

## Basic usage												{#usage}

To make use of the library just include its only header file half.hpp, which defines all half-precision functionality inside the half_float namespace. The actual 16-bit half-precision data type is represented by the [half](\ref half_float::half) type, which uses the standard IEEE representation with 1 sign bit, 5 exponent bits and 11 mantissa bits (including the hidden bit) and supports all types of special values, like subnormal values, infinity and NaNs. This type behaves like the builtin floating-point types as much as possible, supporting the usual arithmetic, comparison and streaming operators, which makes its use pretty straight-forward:

~~~~{.cpp}
using half_float::half;
half a(3.4), b(5);
half c = a * b;
c += 3;
if(c > a)
	std::cout << c << std::endl;
~~~~

Additionally the half_float namespace also defines half-precision versions for all mathematical functions of the [C++ standard library](http://en.cppreference.com/w/cpp/numeric/math), which can be used directly through ADL:

~~~~{.cpp}
half a(-3.14159);
half s = sin(abs(a));
long l = lround(s);
~~~~

You may also specify explicit half-precision literals, since the library provides a user-defined literal inside the half_float::literal namespace, which you just need to import (assuming support for C++11 user-defined literals):

~~~~{.cpp}
using namespace half_float::literal;
half x = 1.0_h;
~~~~

Furthermore the library provides proper specializations for [std::numeric_limits](http://en.cppreference.com/w/cpp/types/numeric_limits), defining various implementation properties, and [std::hash](http://en.cppreference.com/w/cpp/utility/hash) for hashing half-precision numbers (assuming support for C++11 `std::hash`). Similar to the corresponding preprocessor symbols from `<cmath>` the library also defines the [HUGE_VALH](\ref HUGE_VALH) constant and the [FP_FAST_FMAH](\ref FP_FAST_FMAH) symbol.

## Conversions and rounding									{#conversions}

The [half](\ref half_float::half) is explicitly constructible/convertible from a single-precision `float` argument. Thus it is also explicitly constructible/convertible from any type implicitly convertible to `float`, but constructing it from types like `double` or `int` will involve the usual warnings arising when implicitly converting those to `float` because of the lost precision. On the one hand those warnings are intentional, because converting those types to [half](\ref half_float::half) neccessarily also reduces precision. But on the other hand they are raised for explicit conversions from those types, when the user knows what he is doing. So if those warnings keep bugging you, then you won't get around first explicitly converting to `float` before converting to [half](\ref half_float::half), or use the half_cast() described below. In addition you can also directly assign `float` values to [half](\ref half_float::half)s.

In contrast to the float-to-half conversion, which reduces precision, the conversion from [half](\ref half_float::half) to `float` (and thus to any other type implicitly convertible from `float`) is implicit, because all values represetable with half-precision are also representable with single-precision. This way the half-to-float conversion behaves similar to the builtin float-to-double conversion and all arithmetic expressions involving both half-precision and single-precision arguments will be of single-precision type. This way you can also directly use the mathematical functions of the C++ standard library, though in this case you will invoke the single-precision versions which will also return single-precision values, which is (even if maybe performing the exact same computation, see below) not as conceptually clean when working in a half-precision environment.

The default rounding mode for conversions between [half](\ref half_float::half) and more precise types as well as for rounding results of arithmetic operations and mathematical functions rounds to the nearest representable value. But by predefining the [HALF_ROUND_STYLE](\ref HALF_ROUND_STYLE) preprocessor symbol this default can be overridden with one of the other standard rounding modes using their respective constants or the equivalent values of [std::float_round_style](http://en.cppreference.com/w/cpp/types/numeric_limits/float_round_style) (it can even be synchronized with the built-in single-precision implementation by defining it to [std::numeric_limits<float>::round_style](http://en.cppreference.com/w/cpp/types/numeric_limits/round_style)):

`std::float_round_style`         | value | rounding
---------------------------------|-------|-------------------------
`std::round_indeterminate`       | -1    | fastest
`std::round_toward_zero`         | 0     | toward zero
`std::round_to_nearest`          | 1     | to nearest (default)
`std::round_toward_infinity`     | 2     | toward positive infinity
`std::round_toward_neg_infinity` | 3     | toward negative infinity

In addition to changing the overall default rounding mode one can also use the half_cast(). This converts between [half](\ref half_float::half) and any built-in arithmetic type using a configurable rounding mode (or the default rounding mode if none is specified). In addition to a configurable rounding mode, half_cast() has another big difference to a mere `static_cast`: Any conversions are performed directly using the given rounding mode, without any intermediate conversion to/from `float`. This is especially relevant for conversions to integer types, which don't necessarily truncate anymore. But also for conversions from `double` or `long double` this may produce more precise results than a pre-conversion to `float` using the single-precision implementation's current rounding mode would.

~~~~{.cpp}
half a = half_cast<half>(4.2);
half b = half_cast<half,std::numeric_limits<float>::round_style>(4.2f);
assert( half_cast<int, std::round_to_nearest>( 0.7_h )     == 1 );
assert( half_cast<half,std::round_toward_zero>( 4097 )     == 4096.0_h );
assert( half_cast<half,std::round_toward_infinity>( 4097 ) == 4100.0_h );
assert( half_cast<half,std::round_toward_infinity>( std::numeric_limits<double>::min() ) > 0.0_h );
~~~~

## Accuracy and Performance									{#implementation}

From version 2.0 onward the library is implemented without employing the underlying floating-point implementation of the system (except for conversions, of course), providing an entirely self-contained half-precision implementation with results independent from the system's existing single- or double-precision implementation and its rounding behaviour.

As to accuracy, many of the operators and functions provided by this library are exact to rounding for all [rounding modes](\ref HALF_ROUND_STYLE), i.e. the error to the exact result is at most 0.5 ULP (unit in the last place) for rounding to nearest and less than 1 ULP for all other rounding modes. This holds for all the operations required by the IEEE 754 standard and many more. Specifically, the following functions might exhibit a deviation from the correctly rounded result by 1 ULP for a select few input values: expm1(), log1p(), pow(), [atan2()](\ref half_float::atan2), [erf()](\ref half_float::erf), erfc(), lgamma(), tgamma() (for more details see the documentation of the individual functions). All other functions and operators are always exact to rounding or independent of the rounding mode altogether.

The increased IEEE-conformance and cleanliness of this implementation comes with a certain performance cost compared to doing computations and mathematical functions in hardware-accelerated single-precision. On average and depending on the platform, the arithemtic operators are about 75% as fast and the mathematical functions about 50% as fast as performing the corresponding operations in single-precision and converting between the inputs and outputs. However, directly computing with half-precision values is a rather rare use-case and usually using actual `float` values for all computations and temproraries and using [half](\ref half_float::half)s only for storage is the recommended way. But nevertheless the goal of this library was to provide a complete and conceptually clean IEEE-confromant half-precision implementation and in the few cases when you do need to compute directly in half-precision you do so for a reason and want accurate results.

If necessary, this internal implementation can be overridden by predefining the [HALF_ARITHMETIC_TYPE](\ref HALF_ARITHMETIC_TYPE) preprocessor symbol to one of the built-in floating-point types (`float`, `double` or `long double`), which will cause the library to use this type for computing arithmetic operations and mathematical functions (if available). However, due to using the platform's floating-point implementation (and its rounding behaviour) internally, this might cause results to deviate from the specified half-precision rounding mode. It will of course also inhibit any of the automatic exception handling facilities described below.

The conversion operations between half-precision and single-precision types can also make use of the [F16C extension](https://en.wikipedia.org/wiki/F16C) for x86 processors by using the corresponding compiler intrinsics from `<immintrin.h>`. Support for this is checked at compile-time by looking for the `__F16C__` macro which at least *gcc* and *clang* define based on the target platform. It can also be enabled manually by predefining the [HALF_ENABLE_F16C_INTRINSICS](\ref HALF_ENABLE_F16C_INTRINSICS) preprocessor symbol to 1, or 0 for explicitly disabling it. However, this will directly use the corresponding intrinsics for conversion without checking if they are available at runtime (possibly crashing if they are not), so make sure they are supported on the target platform before enabling this.

## Exception Handling										{#exceptions}

The half-precision implementation supports all 5 required floating-point exceptions from the IEEE standard to indicate erroneous inputs or inexact results during operations. These are represented by exception flags which actually use the same values as the corresponding [flags defined in C++11's `<cfenv>` header](https://en.cppreference.com/w/cpp/numeric/fenv/FE_exceptions) if supported, specifically:

flag                                          | meaning                                           | C++ equivalent
----------------------------------------------|---------------------------------------------------|---------------
[HALF_FE_INVALID](\ref HALF_FE_INVALID)       | domain error: invalid input for operation         | `FE_INVALID`
[HALF_FE_DIVBYZERO](\ref HALF_FE_DIVBYZERO)   | pole error: finite input produced infinite result | `FE_DIVBYZERO`
[HALF_FE_OVERFLOW](\ref HALF_FE_OVERFLOW)     | result too large to represent finitely            | `FE_OVERFLOW`
[HALF_FE_UNDERFLOW](\ref HALF_FE_UNDERFLOW)   | subnormal or zero result after rounding           | `FE_UNDERFLOW`
[HALF_FE_INEXACT](\ref HALF_FE_INEXACT)       | result was rounded to be representable            | `FE_INEXACT`
[HALF_FE_ALL_EXCEPT](\ref HALF_FE_ALL_EXCEPT) | OR of all possible exception flags                | `FE_ALL_EXCEPT`

The internal exception flag state will start with all flags cleared and is maintained per thread if C++11 thread-local storage is supported, otherwise it will be maintained globally and will theoretically *not* be thread-safe (while practically being as thread-safe as a simple integer variable can be). These flags can be managed explicitly using the library's [error handling functions](\ref errors), which again try to mimic the [built-in functions for handling floating-point exceptions](https://en.cppreference.com/w/cpp/numeric/fenv) from `<cfenv>`. You can clear them with feclearexcept() (which is the only way a flag can be cleared), test them with fetestexcept(), explicitly raise errors with feraiseexcept() and save and restore their state using fegetexceptflag() and fesetexceptflag(). You can also throw corresponding C++ exceptions based on the current flag state using fethrowexcept().

However, any automatic exception detection and handling during half-precision operations and functions is **disabled by default**, since it comes with a minor performance overhead due to runtime checks, and reacting to IEEE floating-point exceptions is rarely ever needed in application code. But the library fully supports IEEE-conformant detection of floating-point exceptions and various ways for handling them, which can be enabled by predefining the corresponding preprocessor symbols to 1. They can be enabled individually or all at once and they will be processed in the order they are listed here:

- [HALF_ERRHANDLING_FLAGS](\ref HALF_ERRHANDLING_FLAGS) sets the internal exception flags described above whenever the corresponding exception occurs.
- [HALF_ERRHANDLING_ERRNO](\ref HALF_ERRHANDLING_ERRNO) sets the value of [`errno` from `<cerrno>`](https://en.cppreference.com/w/cpp/error/errno) similar to the behaviour of the built-in floating-point types when [MATH_ERRNO](https://en.cppreference.com/w/cpp/numeric/math/math_errhandling) is used.
- [HALF_ERRHANDLING_FENV](\ref HALF_ERRHANDLING_FENV) will propagate exceptions to the built-in floating-point implementation using [std::feraiseexcept](https://en.cppreference.com/w/cpp/numeric/fenv/feraiseexcept) if support for C++11 floating-point control is enabled. However, this does not synchronize exceptions: neither will clearing  propagate nor will it work in reverse.
- [HALF_ERRHANDLING_THROW_...](\ref HALF_ERRHANDLING_THROW_INVALID) can be defined to a string literal which will be used as description message for a C++ exception that is thrown whenever a `HALF_FE_...` exception occurs, similar to the behaviour of fethrowexcept().

If any of the above error handling is activated, non-quiet operations on half-precision values will also raise a [HALF_FE_INVALID](\ref HALF_FE_INVALID) exception whenever they encounter a signaling NaN value, in addition to transforming the value into a quiet NaN. If error handling is disabled, signaling NaNs will be treated like quiet NaNs (while still getting explicitly quieted if propagated to the result). There can also be additional treatment of overflow and underflow errors after they have been processed as above, which is **enabled by default** (but of course only takes effect if any other exception handling is activated) unless overridden by predefining the corresponding preprocessor symbol to 0:

- [HALF_ERRHANDLING_OVERFLOW_TO_INEXACT](\ref HALF_ERRHANDLING_OVERFLOW_TO_INEXACT) will cause overflow errors to also raise a [HALF_FE_INEXACT](\ref HALF_FE_INEXACT) exception.
- [HALF_ERRHANDLING_UNDERFLOW_TO_INEXACT](\ref HALF_ERRHANDLING_UNDERFLOW_TO_INEXACT) will cause underflow errors to also raise a [HALF_FE_INEXACT](\ref HALF_FE_INEXACT) exception. This will also slightly change the behaviour of the underflow exception, which will *only* be raised if the result is actually inexact due to underflow. If this is disabled, underflow exceptions will be raised for *any* (possibly exact) subnormal result.


---------------------
# Credits and Contact										{#contact}

This library is developed by [Christian Rau](http://sourceforge.net/users/rauy) and released under the <a href="LICENSE.txt">MIT License</a>. If you have any questions or problems with it, feel free to contact me at [rauy AT users.sourceforge.net](mailto:rauy@users.sourceforge.net) or use any of the other means for [support](usergroup1.html).

Additional credit goes to **Jeroen van der Zijp** for his paper on [Fast Half Float Conversions](ftp://ftp.fox-toolkit.org/pub/fasthalffloatconversion.pdf), whose algorithms have been used in the library for converting between half-precision and single-precision values.
