



# Installing Half

The library in its most recent version can be obtained from here, see the [Release Notes](changelog.html) for further information:

<ul class="tablist"><li>[Download half 1.12.0 (.zip)](http://sourceforge.net/projects/half/files/latest/download)</li></ul>

If you are interested in previous versions of the library, see the [SourceForge download page](http://sourceforge.net/projects/half/files/half).

Comfortably enough, the library consists of just a single header file containing all the functionality, which can be directly included by your projects, without the neccessity to build anything or link to anything.

Whereas this library is fully C++98-compatible, it can profit from certain C++11 features. Support for those features is checked and enabled automatically at compile (or rather preprocessing) time, but can be explicitly enabled or disabled by defining the corresponding preprocessor symbols to either 1 or 0 yourself. This is useful when the automatic detection fails (for more exotic implementations) or when a feature should be explicitly disabled:

C++11 feature                        | Used for                           | Enabled for (and newer)                     | Override with
-------------------------------------|------------------------------------|---------------------------------------------|----------------------------------
`long long` integer type             | functions returning `long long`    | *VC++ 2003*, *gcc*, *clang*                 | `HALF_ENABLE_CPP11_LONG_LONG`
static assertions                    | extended compile-time checks       | *VC++ 2010*, *gcc 4.3*, *clang 2.9*         | `HALF_ENABLE_CPP11_STATIC_ASSERT`
generalized constant expressions     | constant operations                | *VC++ 2015*, *gcc 4.6*, *clang 3.1*         | `HALF_ENABLE_CPP11_CONSTEXPR`
`noexcept` specifications            | proper `noexcept` functions        | *VC++ 2015*, *gcc 4.6*, *clang 3.0*         | `HALF_ENABLE_CPP11_NOEXCEPT`
user-defined literals                | half-precision literals            | *VC++ 2015*, *gcc 4.7*, *clang 3.1*         | `HALF_ENABLE_CPP11_USER_LITERALS`
type traits from `<type_traits>`     | TMP and extended checks            | *VC++ 2010*, *libstdc++ 4.3*, <i>libc++</i> | `HALF_ENABLE_CPP11_TYPE_TRAITS`
sized integer types from `<cstdint>` | more flexible type sizes           | *VC++ 2010*, *libstdc++ 4.3*, <i>libc++</i> | `HALF_ENABLE_CPP11_CSTDINT`
certain new `<cmath>` functions      | corresponding half implementations | *VC++ 2013*, *libstdc++ 4.3*, <i>libc++</i> | `HALF_ENABLE_CPP11_CMATH`
`std::hash` from `<functional>`      | hash function for halfs            | *VC++ 2010*, *libstdc++ 4.3*, <i>libc++</i> | `HALF_ENABLE_CPP11_HASH`


