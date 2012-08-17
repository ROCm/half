Half-precision floating point library						{#mainpage}
=====================================

This is a C++ header-only library to provide an [IEEE 754](http://en.wikipedia.org/wiki/IEEE_754-2008) conformant 16-bit [half-precision](http://en.wikipedia.org/wiki/Half_precision_floating-point_format) floating point type along with corresponding arithmetic operators, type conversions and common mathematical functions. It aims for both efficiency and ease of use, trying to accurately mimic the behaviour of the builtin floating point types at the best performance possible. It is hosted on [SourceForge.net](http://sourceforge.net/projects/half/).

[TOC]


News														{#new}
====

August 16, 2012 - Release 1.5.0
-------------------------------

[Version 1.5.0](http://sourceforge.net/projects/half/files/half/1.5.0) of the library has been released. It adds a special half_cast() that can be used to cast between [half](\ref half_float::half) and any type convertible to/from `float`. In addition to doing an explicit conversion via `float` and thus avoiding any possible warnings about precision loss, it allows the explicit specification of the rounding mode to use for the single-to-half conversion (if any). This way you can choose a specific rounding mode if needed and can even synchronize it with the rounding mode of the underlying single-precision implementation (which is usually `std::round_to_nearest`). But this is more of an expert feature to be used when you are sure you really need it (which should rarely be the case) and most of the time the default rounding from single- to half-precision (`std::round_indeterminate`), which is the fastest, will be sufficient. Furthermore, the library has been tested successfully with Visual C++ 2008.

[more](news.html)


-------------------------
Download and Installation									{#downloads}
=========================

The library in its most recent version can be obtained from here, see the [Release Notes](changelog.html) for further information:

<ul class="tablist"><li>[Download half 1.5.0 (.zip)](http://sourceforge.net/projects/half/files/latest/download)</li></ul>

If you are interested in previous versions of the library, see the [SourceForge download page](http://sourceforge.net/projects/half/files/half).

Comfortably enough, the library consists of just a single header file containing all the functionality, which can be directly included by your projects, without the neccessity to build anything or link to anything.

The library needs an IEEE-754-conformant single-precision `float` type, but this should be the case on most modern platforms. Whereas the library is fully C++98-compatible, it can profit from certain C++11 features. Support for those features is checked and enabled automatically at compile (or rather preprocessing) time, but can be explicitly enabled or disabled by defining the corresponding preprocessor symbols to either 1 or 0 yourself. This is useful when the automatic detection fails (for more exotic implementations) or when a feature should be explicitly disabled:

C++11 feature                        | Used for                     | Enabled for (and newer)                     | Override with
-------------------------------------|------------------------------|---------------------------------------------|----------------------------------
static assertions                    | extended compile-time checks | *VC++ 2010*, *gcc 4.3*, *clang 2.9*         | `HALF_ENABLE_CPP11_STATIC_ASSERT`
generalized constant expressions     | constant operations          | *gcc 4.6*, *clang 3.1*                      | `HALF_ENABLE_CPP11_CONSTEXPR`
user-defined literals                | half-precision literals      | *gcc 4.7*, *clang 3.1*                      | `HALF_ENABLE_CPP11_USER_LITERALS`
sized integer types from `<cstdint>` | more flexible type sizes     | *VC++ 2010*, *libstdc++ 4.3*, <i>libc++</i> | `HALF_ENABLE_CPP11_CSTDINT`
certain new `<cmath>` functions      | corresponding half functions | *libstdc++ 4.3*, <i>libc++</i>              | `HALF_ENABLE_CPP11_CMATH`
`std::hash` from `<functional>`      | hash function for halfs      | *VC++ 2010*, *libstdc++ 4.3*, <i>libc++</i> | `HALF_ENABLE_CPP11_HASH`

The library has been tested successfully with *Visual C++ 2005* - *2012*, *gcc 4.4* - *4.7* and *clang 3.1*. Please [contact me](#contact) if you have any problems, suggestions or even just success testing it on other platforms.


-------------
Documentation												{#doc}
=============

What follows are some general words about the usage of the library and its implementation. For a complete reference documentation of its iterface you should consult the [API Documentation](usergroup0.html).


Basic usage													{#usage}
-----------

To make use of the library just include its only header file half.hpp, which defines all half-precision functionality inside the half_float namespace. The actual 16-bit half-precision data type is represented by the [half](\ref half_float::half) type. This type behaves like the builtin floating point types as much as possible, supporting the usual arithmetic, comparison and streaming operators, which makes its use pretty straight-forward:

~~~~{.cpp}
using half_float::half;
half a(3.4), b(5);
half c = a * b;
c += 3;
if(c > a)
	std::cout << c << std::endl;
~~~~

Additionally the half_float namespace also defines half-precision versions for all mathematical functions of the C++ standard library, which can be used directly through ADL:

~~~~{.cpp}
half a(-3.14159);
half s = sin(abs(a));
long l = lround(s);
~~~~

Furthermore the library provides proper specializations for `std::numeric_limits`, defining various implementation properties, and `std::hash` for hashing half-precision numbers (assuming support for C++11 `std::hash`). Similar to the corresponding preprocessor symbols from `<cmath>` the library also defines the [HUGE_VALH](\ref HUGE_VALH) constant and maybe the [FP_FAST_FMAH](\ref FP_FAST_FMAH) symbol.

Conversions													{#conversions}
-----------

The [half](\ref half_float::half) is explicitly constructible/convertible from a single-precision `float` argument. Thus it is also explicitly constructible/convertible from any type implicitly convertible to `float`, but constructing it from types like `double` or `int` will involve the usual warnings arising when implicitly converting those to `float` because of the lost precision. On the one hand those warnings are intentional, because converting those types to [half](\ref half_float::half) neccessarily also reduces precision. But on the other hand they are raised for explicit conversions from those types, when the user knows what he is doing. So if those warnings keep bugging you, then you won't get around first explicitly converting to `float` before converting to [half](\ref half_float::half), or use the half_cast() described below. In addition you can also directly assign `float` values to [half](\ref half_float::half)s.

For performance reasons the conversion from `float` to [half](\ref half_float::half) uses truncation (round toward zero, but mapping overflows to infinity) for rounding values not representable exactly in half-precision. If you are in need for other rounding behaviour (though this should rarely be the case), you can use the half_cast(). In addition to performning an explicit cast between [half](\ref half_float::half) and any other type convertible to/from `float` via an explicit cast to/from `float` (and thus without any warnings due to possible precision-loss), it let's you explicitly specify the rounding mode to use for the float-to-half conversion. You can even synchronize it with the bultin single-precision implementation's rounding mode:

~~~~{.cpp}
half a = half_float::half_cast<half,std::numeric_limits<float>::round_style>(4.2);
~~~~

In contrast to the float-to-half conversion, which reduces precision, the conversion from [half](\ref half_float::half) to `float` (and thus to any other type implicitly convertible to `float`) is implicit, because all values represetable with half-precision are also representable with single-precision. This way the half-to-float conversion behaves similar to the builtin float-to-double conversion and all arithmetic expressions involving both half-precision and single-precision arguments will be of single-precision type. This way you can also directly use the mathematical functions of the C++ standard library, though in this case you will invoke the single-precision versions which will also return single-precision values, which is (even if maybe performing the exact same computation, see below) not as conceptually clean when working in a half-precision environment.

You may also specificy explicit half-precision literals, since the library provides a user-defined literal inside the half_float::literal namespace, which you just need to import (assuming support for C++11 user-defined literals):

~~~~{.cpp}
using namespace half_float::literal;
half x = 1.0_h;
~~~~

Implementation												{#implementation}
--------------

For performance reasons (and ease of implementation) many of the mathematical functions provided by the library as well as all arithmetic operations are actually carried out in single-precision under the hood, calling to the C++ standard library implementations of those functions whenever appropriate, meaning the arguments are converted to `float`s and the result back to [half](\ref half_float::half). But to reduce the conversion overhead as much as possible any temporary values inside of lengthy expressions are kept in single-precision as long as possible, while still maintaining a strong half-precision type to the outside world. Only when finally assigning the value to a [half](\ref half_float::half) or calling a function that works directly on [half](\ref half_float::half)s is the actual conversion done (or never, when further converting the result to `float`).

This approach has two implications. First of all you have to treat the documentation on this site as a simplified version, describing the behaviour of the library as if implemented this way. The actual argument and return types of functions and operators may involve other internal types (feel free to generate the exact developer documentation from the Doxygen comments in the library's header file if you really need to). But nevertheless the behaviour is exactly like specified in the documentation. The other implication is, that in the presence of rounding errors or over-/underflows arithmetic expressions may produce different results when compared to converting to half-precision after each individual operation:

~~~~{.cpp}
half a = (std::numeric_limits<half>::max() * 2.0_h) / 2.0_h;	// a = MAX
half b = std::numeric_limits<half>::max() * 2.0_h;				// b = INF
b /= 2.0_h;														// b stays INF
~~~~

But this should only be a problem in very few cases. One last word has to be said when talking about performance. Even with its efforts in reducing conversion overhead as much as possible, the software half-precision implementation can most probably not beat the direct use of single-precision computations. Usually using actual `float` values for all computations and temproraries and using [half](\ref half_float::half)s only for storage is the recommended way. On the one hand this somehow makes the provided mathematical functions obsolete (especially in light of the implicit conversion from [half](\ref half_float::half) to `float`), but nevertheless the goal of this library was to provide a complete and conceptually clean half-precision implementation, to which the standard mathematical functions belong, even if usually not needed.

IEEE conformance											{#ieee}
----------------

The [half](\ref half_float::half) type uses the standard IEEE representation with 1 sign bit, 5 exponent bits and 10 mantissa bits (11 when counting the hidden bit). It supports all types of special values, like subnormal values, infinity and NaNs. But there are some limitations to the complete conformance to the IEEE 754 standard:

-	The implementation does not differentiate between signalling and quiet NaNs, this means operations on [half](\ref half_float::half)s are not specified to trap on signalling NaNs (though they may, see last point).
-	Though arithmetic operations are internally rounded to single-precision using the underlying single-precision implementation's current rounding mode, those values are then converted to half-precision using truncation (round toward zero, but with overflows mapped to infinity). This mixture of rounding modes is also the reason why std::numeric_limits<half_float::half>::round_style actually returns `std::round_indeterminate`.
-	Because of this truncation it may also be that certain single-precision NaNs will be wrongly converted to half-precision infinity, though this is very unlikely to happen, since most single-precision implementations don't tend to only set the lowest bits of a NaN mantissa.
-	The implementation does not provide any floating point exceptions, thus arithmetic operations or mathematical functions are not specified to invoke proper floating point exceptions. But due to many functions implemented in single-precision, those may still invoke floating point exceptions of the underlying single-precision implementation.

Some of those points could have been circumvented by controlling the floating point environment using `<cfenv>` or implementing a similar exception mechanism. But this would have required excessive runtime checks giving two high an impact on performance for something that is rarely ever needed. If you really need to rely on proper floating point exceptions, it is recommended to explicitly perform computations using the builtin floating point types to be on the safe side. In the same way, if you really need to rely on a particular rounding behaviour, it is recommended to use single-precision computations and explicitly convert the result to half-precision using half_cast() and specifying the desired rounding mode. But those are really considered expert-scenarios rarely encountered in practice, since actually working with half-precision usually comes with a certain tolerance/ignorance of exactness considerations.


-------------------
Credits and Contact											{#contact}
===================

This library is developed by [Christian Rau](http://sourceforge.net/users/rauy) and released under the [MIT License](LICENSE.txt). If you have any questions or problems with it, feel free to contact me at [rauy AT users.sourceforge.net](mailto:rauy@users.sourceforge.net) or use any of the other means for [support](usergroup1.html).

Additional credit goes to **Jeroen van der Zijp** for his paper on [Fast Half Float Conversions](ftp://ftp.fox-toolkit.org/pub/fasthalffloatconversion.pdf), whose algorithms have been used in the library for converting between half-precision and single-precision values.
