.. meta::
  :description: Half documentation 
  :keywords: Half, APIs, ROCm, documentation


Understanding Half library
----------------------------

The library has been tested successfully with *Visual C++ 2005* - *2015*, *gcc 4.4* - *4.8* and *clang 3.1*. Please [contact me](#contact) if you have any problems, suggestions or even just success testing it on other platforms.

The Half library consists of a single header file containing all the functionality, which can be directly included by your projects without the neccessity to build anything or link to anything.

While this library is fully C++98-compatible, it can profit from certain C++11 features. Support for those features is checked automatically at compile (or rather preprocessing) time, but can be explicitly enabled or disabled by defining the corresponding preprocessor symbols to either 1 or 0 yourself. This is useful when the automatic detection fails (for more exotic implementations) or when a feature should be explicitly disabled:

- 'long long' integer type for mathematical functions returning 'long long' results (enabled for VC++ 2003 and newer, gcc and clang, overridable with  'HALF_ENABLE_CPP11_LONG_LONG').

- Static assertions for extended compile-time checks (enabled for VC++ 2010, gcc 4.3, clang 2.9 and newer, overridable with 'HALF_ENABLE_CPP11_STATIC_ASSERT').

- Generalized constant expressions (enabled for VC++ 2015, gcc 4.6, clang 3.1 and newer, overridable with 'HALF_ENABLE_CPP11_CONSTEXPR').

- noexcept exception specifications (enabled for VC++ 2015, gcc 4.6, clang 3.0 and newer, overridable with 'HALF_ENABLE_CPP11_NOEXCEPT').

- User-defined literals for half-precision literals to work (enabled for VC++ 2015, gcc 4.7, clang 3.1 and newer, overridable with 'HALF_ENABLE_CPP11_USER_LITERALS').

- Type traits and template meta-programming features from <type_traits> (enabled for VC++ 2010, libstdc++ 4.3, libc++ and newer, overridable with 'HALF_ENABLE_CPP11_TYPE_TRAITS').

- Special integer types from <cstdint> (enabled for VC++ 2010, libstdc++ 4.3, libc++ and newer, overridable with 'HALF_ENABLE_CPP11_CSTDINT').

- Certain C++11 single-precision mathematical functions from <cmath> for an improved implementation of their half-precision counterparts to work (enabled for VC++ 2013, libstdc++ 4.3, libc++ and newer, overridable with 'HALF_ENABLE_CPP11_CMATH').

- Hash functor 'std::hash' from <functional> (enabled for VC++ 2010, libstdc++ 4.3, libc++ and newer, overridable with 'HALF_ENABLE_CPP11_HASH').

The library has been tested successfully with Visual C++ 2005-2015, gcc 4.4-4.8 and clang 3.1. 
