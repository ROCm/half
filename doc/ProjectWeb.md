Half-precision floating point library						{#mainpage}
=====================================

This is a C++ header-only library to provide an IEEE 754 conformant half-precision floating point type along with corresponding arithmetic operators, type conversions and common mathematical functions. It aims for both efficiency and ease of use, trying to accurately mimic the behaviour of the builtin floating point types at the best performance possible.

[TOC]


News														{#news}
====

August 1, 2012
--------------

The first version of the library has been checked into source control and is thus available to the public through the [SVN repository](http://sourceforge.net/p/half/code/), though still waiting for an explicit file release. Additionally the project web containing the library's documentation went online.

July 30, 2012
-------------

The **half** project has finally been registered on [Sourceforge.net](http://sourceforge.net/projects/half/).

--------------------------------------------------------------------------------


Download and Installation									{#downloads}
=========================

The library in its most recent version can be obtained from here:

-	coming soon...

If you are interested in previous versions of the library, see the [Sourceforge download page](http://sourceforge.net/projects/half/files/).

Comfortably enough, the library consists of just a single header file containing all the functionality, which can be directly included by your projects, without the neccessity to build anything or link to anything.

The library imposes some requirements on your C++ implementation:

-	IEEE 754 conformant single-precision `float` type.
-	Support for fixed-width integer types from `<cstdint>`, which comes with C++11.
-	Support for certain C++11 single-precision mathematical functions from `<cmath>` for their half-precision counter-parts to work (**optional**).
-	Support for C++11's `std::hash` from `<functional>` (**optional**, only if hashing enabled by defining `HALF_ENABLE_HASH`).

It has been tested successfully with *Visual C++ 2010*. Please [contact me](#contact) if you have any problems, suggestions or even just success testing it on other platforms.

--------------------------------------------------------------------------------


Documenation												{#doc}
============

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

Furthermore the library provides proper specializations for `std::numeric_limits`, defining various implementation properties, and `std::hash` for hashing half-precision numbers (the latter only if the preprocessor symbol `HALF_ENABLE_HASH` is defined when including half.hpp.

Conversions													{#conversions}
-----------

The [half](\ref half_float::half) is explicitly constructible/convertible from a single-precision `float` argument. Thus it is also explicitly constructible/convertible from any type implicitly convertible to `float`, but constructing it from types like `double` or `int` will involve the usual warnings arising when implicitly converting those to `float` because of the lost precision. On the one hand those warnings are intentional, because converting those types to [half](\ref half_float::half) neccessarily also reduces precision. But on the other hand they are raised for explicit conversions from those types, when the user knows what he is doing. So if those warnings keep bugging you, then you won't get around first explicitly converting to `float` before converting to [half](\ref half_float::half). In addition you can also directly assign `float` values to [half](\ref half_float::half)s.

In contrast to the float-to-half conversion, which reduces precision, the conversion from [half](\ref half_float::half) to `float` (and thus to any other type implicitly convertible to `float`) is implicit, because all values represetable with half-precision are also representable with single-precision. This way the half-to-float conversion behaves similar to the builtin float-to-double conversion and all arithmetic expressions involving both half-precision and single-precision arguments will be of single-precision type. This way you can also directly use the mathematical functions of the C++ standard library, though in this case you will invoke the single-precision versions which will also return single-precision values, which is (even if maybe performing the exact same computation, see below) not as conceptually clean when working in a half-precision environment.

Implementation												{#implementation}
--------------

For performance reasons (and ease of implementation) many of the mathematical functions provided by the library as well as all arithmetic operations are actually carried out in single-precision under the hood, calling to the C++ standard library implementations of those functions whenever appropriate, meaning the arguments are converted to `float`s and the result back to [half](\ref half_float::half). But to reduce the conversion overhead as much as possible any temporary values inside of lengthy expressions are kept in single-precision as long as possible, while still maintaining a strong half-precision type to the outside world. Only when finally assigning the value to a [half](\ref half_float::half) or calling a function that works directly on [half](\ref half_float::half)s is the actual conversion done (or never, when further converting the result to `float`).

This approach has two implications. First of all you have to treat the documentation on this site as a simplified version, describing the behaviour of the library as-if implemented this way. The actual argument and return types of functions and operators may involve other internal types (feel free to generate the exact developer documentation from the Doxygen comments in the library's header file if you really need to). But nevertheless the behaviour is exactly like specified in the documentation. The other implication is, that in the presence of rounding errors or over-/underflows arithmetic expressions may produce different results when compared to converting to half-precision after each individual operation:

~~~~{.cpp}
half a = (std::numeric_limits<half>::max() + static_cast<half>(1)) - static_cast<half>(1);	// a = MAX
half b = std::numeric_limits<half>::max() + static_cast<half>(1);							// b = INF
b -= static_cast<half>(1);																	// b stays INF
~~~~

But this should only be a problem in very few cases. One last word has to be said when talking about performance. Even with its efforts in reducing conversion overhead as much as possible, the software half-precision implementation can most probably not beat the direct use of single-precision computations. Usually using actual `float` values for all computations and temproraries and using [half](\ref half_float::half)s only for storage is the recommended way. On the one hand this somehow makes the provided mathematical functions obsolete (especially in light of the implicit conversion from [half](\ref half_float::half) to `float`), but nevertheless the goal of this library was to provide a complete and conceptually clean half-precision implementation, to which the standard mathematical functions belong, even if usually not needed.

IEEE conformance											{#ieee}
----------------

The [half](\ref half_float::half) type uses the standard IEEE representation with 1 sign bit, 5 exponent bits and 10 mantissa bits (11 when counting the hidden bit). It supports all types of special values, like subnormal values, infinity and NaNs. But there are some limitations to the complete conformance to the IEEE 754 standard:

-	The implementation does not differentiate between signalling and quiet NaNs, this means operations on [half](\ref half_float::half)s are not specified to trap on signalling NaNs (though they may, see last point).
-	Though arithmetic operations are internally rounded to single-precision using the current rounding mode, those values are then converted to half-precision using simple truncation.
-	Because of this truncation it may also be that certain single-precision NaNs will be wrongly converted to half-precision infinity, though this is very unlikely to happen, since most single-precision implementations don't tend to only set the lowest bits of a NaN mantissa.
-	The implementation does not provide any floating point exceptions, thus arithmetic operations or mathematical functions are not specified to invoke proper floating point exceptions. But due to many functions implemented in single-precision, those may still invoke floating point exceptions of the underlying single-precision implementation.

Some of those points could have been circumvented by controlling the floating point environment (through `<cfenv>`) or implementing a similar exception mechanism. But this would have required excessive runtime checks giving two high an impact on performance for something that is rarely ever needed. If you really need to rely on proper floating point exceptions, it is recommended to explicitly perform computations using the builtin floating point types to be on the safe side.


--------------------------------------------------------------------------------


Credits and Contact											{#contact}
===================

This library is developed by [Christian Rau](http://sourceforge.net/users/rauy) and released under the [MIT License](LICENSE.txt). If you have any questions or problems with it, feel free to contact me at **rauy at users.sourceforge.net**.

Additional credit goes to **Jeroen van der Zijp** for his paper on [Fast Half Float Conversions](ftp://ftp.fox-toolkit.org/pub/fasthalffloatconversion.pdf), whose algorithms have been used in the library for converting between half-precision and single-precision values.