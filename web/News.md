News														{#news}
====

October 26, 2012 - Release 1.7.0
--------------------------------

[Version 1.7.0](http://sourceforge.net/projects/half/files/half/1.7.0) of the library has been released. It adds support for C++11 `noexcept` specifications. But due to the ongoing discussions about their appropriate usage in practice (and the author's limited experience with them) they are at the moment only used to provide a C++11-conformant `std::numeric_limits` specialization, with all its functions returning half-preicision constants now properly being `noexcept`. Furthermore, the automatic support for C++11 `long long` and the corresponding mathematical functions on *Microsoft* compilers has been extended all the way down to *VC++ 2003*.

----------------------------------
September 13, 2012 - Release 1.6.1
----------------------------------

[Version 1.6.1](http://sourceforge.net/projects/half/files/half/1.6.1) of the library has been released. It makes the fma() and fdim() functions available even if the corresponding single-precision functions from C++11 `<cmath>` are not supported.

----------------------------------
September 12, 2012 - Release 1.6.0
----------------------------------

[Version 1.6.0](http://sourceforge.net/projects/half/files/half/1.6.0) of the library has been released. It fixes a major bug in the C++98 compatibility for non-VC compilers, which required C++11 `<cmath>` functions even without C++11 support enabled. Furthermore, an additional preprocessor flag `HALF_ENABLE_CPP11_LONG_LONG` has been added, which controls the support for C++11 `long long` integers and the corresponding mathematical functions (actually only llround()).

-------------------------------
August 17, 2012 - Release 1.5.1
-------------------------------

[Version 1.5.1](http://sourceforge.net/projects/half/files/half/1.5.1) of the library has been released. It fixes the `std::numeric_limits` specialization to always return `std::round_indeterminate` as `round_style`, even if the `float` version returns `std::round_toward_zero`. This is neccessary as the single-to-half conversion is not exactly round-toward-zero, since it maps overflows to +/-infinity instead of the maximum/minimum finite value.

-------------------------------
August 16, 2012 - Release 1.5.0
-------------------------------

[Version 1.5.0](http://sourceforge.net/projects/half/files/half/1.5.0) of the library has been released. It adds a special half_cast() that can be used to cast between [half](\ref half_float::half) and any type convertible to/from `float`. In addition to doing an explicit conversion via `float` and thus avoiding any possible warnings about precision loss, it allows the explicit specification of the rounding mode to use for the single-to-half conversion (if any). This way you can choose a specific rounding mode if needed and can even synchronize it with the rounding mode of the underlying single-precision implementation (which is usually `std::round_to_nearest`). But this is more of an expert feature to be used when you are sure you really need it (which should rarely be the case) and most of the time the default rounding from single- to half-precision (`std::round_indeterminate`), which is the fastest, will be sufficient. Furthermore, the library has been tested successfully with Visual C++ 2008.

-------------------------------
August 12, 2012 - Release 1.4.0
-------------------------------

[Version 1.4.0](http://sourceforge.net/projects/half/files/half/1.4.0) of the library has been released. It adds support for C++11 generalized constant expressions (`constexpr`). But due to the not constantly expressable half-single-conversions most arithmetic operators as well as the half-precision literals cannot be made `constexpr`. The only `constexpr` operators are the unary plus and minus as well as the zero-initializing default constructor. But now the `std::numeric_limits` specialization is C++11-conformant, with all its special half-precision constants being actual constant expressions.

-------------------------------
August 11, 2012 - Release 1.3.1
-------------------------------

[Version 1.3.1](http://sourceforge.net/projects/half/files/half/1.3.1) of the library has been released. It fixes a bug with non-VC compilers that required the presence of `std::isnan` and `std::signbit` even if C++11 `<cmath>` functions are unsupported/disabled. Furthermore the library has been tested successfully with Visual C++ 2005 and 2012.

-------------------------------
August 10, 2012 - Release 1.3.0
-------------------------------

[Version 1.3.0](http://sourceforge.net/projects/half/files/half/1.3.0) of the library has been released. It makes the requirement for C++11's `<cstdint>` and `static_assert` optional and thus is now comletely C++98-compatible. Furthermore, all the C++11 features used by the library and detected automatically are now overridable by the user, by just defining the corresponding preprocessor symbols to 0 or 1, which is useful in cases where the automatic detection fails or where the support for a feature should be explicitly disabled.

------------------------------
August 7, 2012 - Release 1.2.0
------------------------------

[Version 1.2.0](http://sourceforge.net/projects/half/files/half/1.2.0) of the library has been released. It adds preprocessor definitions for [HUGE_VALH](\ref HUGE_VALH) and (maybe) [FP_FAST_FMAH](\ref FP_FAST_FMAH) in correspondence with their respective single-precision counterparts from `<cmath>`. It also fixes the preprocessor symbols it uses internally to be undefined properly after their use. Furthermore, the library has been tested successfully with *clang 3.1*.

------------------------------
August 7, 2012 - Release 1.1.2
------------------------------

[Version 1.1.2](http://sourceforge.net/projects/half/files/half/1.1.2) of the library has been released. It fixes the `std::numeric_limits` specialization further, enabling it to return `std::round_toward_zero` instead of `std::round_indeterminate` when possible (though this is very unlikely). Additionally the `round_error()` has been corrected to reflect the worst-case round-toward-zero behaviour. Furthermore, the library has been tested successfully with *gcc 4.5*.

------------------------------
August 6, 2012 - Release 1.1.1
------------------------------

[Version 1.1.1](http://sourceforge.net/projects/half/files/half/1.1.1) of the library has been released. It includes a minor fix to the `std::numeric_limits` specialization, which now returns a normalized number for `min()` according to the C++ standard. Additionally the `round_style` is now `std::round_indeterminate` due to the mixture of internal single-precision rounding with float-to-half truncation.

------------------------------
August 6, 2012 - Release 1.1.0
------------------------------

[Version 1.1.0](http://sourceforge.net/projects/half/files/half/1.1.0) of the library has been released. It provides support for explicit half-precision literals using C++11's user-defined literals. It has been tested successfully with *gcc 4.7*.

------------------------------------
August 5, 2012 - First release 1.0.0
------------------------------------

The first official release, [version 1.0.0](http://sourceforge.net/projects/half/files/half/1.0.0), of the library is finally out.

---------------------------------------------
August 1, 2012 - Code and Project-Web release
---------------------------------------------

The first version of the library has been checked into source control and is thus available to the public through the [SVN repository](http://sourceforge.net/p/half/code/), though still waiting for an explicit file release. Additionally the project web containing the library's documentation went online.

--------------------------------
July 30, 2012 - Project Kick-off
--------------------------------

The **half** project has finally been registered on [Sourceforge.net](http://sourceforge.net/projects/half/).
