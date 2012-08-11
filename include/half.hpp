// half - IEEE 754-based half-precision floating point library.
//
// Copyright (c) 2012 Christian Rau <rauy@users.sourceforge.net>
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

// Version 1.3.1

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
	#if __has_feature(cxx_user_literals) && !defined(HALF_ENABLE_CPP11_USER_LITERALS)
		#define HALF_ENABLE_CPP11_USER_LITERALS 1
	#endif
#elif defined(__INTEL_COMPILER)								//Intel C++
	#if __INTEL_COMPILER >= 1100 && !defined(HALF_ENABLE_CPP11_STATIC_ASSERT)
		#define HALF_ENABLE_CPP11_STATIC_ASSERT 1
	#endif
	#if __INTEL_COMPILER >= 1300 && !defined(HALF_ENABLE_CPP11_CONSTEXPR)
		#define HALF_ENABLE_CPP11_CONSTEXPR 1
	#endif
#elif defined(__GNUC__)										//gcc
	#if defined(__GXX_EXPERIMENTAL_CXX0X__) || __cplusplus >= 201103L
		#if HALF_GNUC_VERSION >= 403 && !defined(HALF_ENABLE_CPP11_STATIC_ASSERT)
			#define HALF_ENABLE_CPP11_STATIC_ASSERT 1
		#endif
		#if HALF_GNUC_VERSION >= 406 && !defined(HALF_ENABLE_CPP11_CONSTEXPR)
			#define HALF_ENABLE_CPP11_CONSTEXPR 1
		#endif
		#if HALF_GNUC_VERSION >= 407 && !defined(HALF_ENABLE_CPP11_USER_LITERALS)
			#define HALF_ENABLE_CPP11_USER_LITERALS 1
		#endif
	#endif
#elif defined(_MSC_VER)										//Visual C++
	#if _MSC_VER >= 1600 && !defined(HALF_ENABLE_CPP11_STATIC_ASSERT)
		#define HALF_ENABLE_CPP11_STATIC_ASSERT 1
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

#ifdef FP_FAST_FMAF
	/// Fast half-precision fma function.
	/// This symbol is only defined if the fma() function generally executes as fast as, or faster than, a separate 
	/// half-precision multiplication followed by an addition. Due to the internal single-precision implementation of this 
	/// function, it is only defined if the corresponding `FP_FAST_FMAF` symbol from `cmath` is defined, too.
	#define FP_FAST_FMAH	1
#endif

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

	/// \name Comparison operators
	/// \{
	bool operator==(half a, half b);
	bool operator!=(half a, half b);
	bool operator<(half a, half b);
	bool operator>(half a, half b);
	bool operator<=(half a, half b);
	bool operator>=(half a, half b);
	/// \}

	/// \name Unary arithmetic operators
	/// \{
	HALF_CONSTEXPR half operator+(half h);
	HALF_CONSTEXPR half operator-(half h);
	/// \}

	/// \name Input and output
	/// \{
	template<typename charT,typename traits>
	std::basic_istream<charT,traits>& operator>>(std::basic_istream<charT,traits> &in, half &h);
	/// \}

	/// \name Basic mathematical operations
	/// \{
	half abs(half arg);
	half fabs(half arg);
	half fmin(half x, half y);
	half fmax(half x, half y);
	half nanh(const char*);
	/// \}

	/// \name Rounding
	/// \{
	half ceil(half arg);
	half floor(half arg);
	half trunc(half arg);
	half round(half arg);
	long lround(half arg);
	long long llround(half arg);
	/// \}

	/// \name Floating point manipulation
	/// \{
	half frexp(half arg, int *exp);
	half ldexp(half arg, int exp);
	half modf(half x, half *iptr);
	half scalbn(half x, int exp);
	half scalbln(half x, long exp);
	int ilogb(half arg);
	half logb(half arg);
	half nextafter(half from, half to);
	half nexttoward(half from, long double to);
	half copysign(half x, half y);
	/// \}

	/// \name Floating point classification
	/// \{
	int fpclassify(half arg);
	bool isfinite(half arg);
	bool isinf(half arg);
	bool isnan(half arg);
	bool isnormal(half arg);
	bool signbit(half arg);
	/// \}

	/// \name Comparison
	/// \{
	bool isgreater(half x, half y);
	bool isgreaterequal(half x, half y);
	bool isless(half x, half y);
	bool islessequal(half x, half y);
	bool islessgreater(half x, half y);
	bool isunordered(half x, half y);
	/// \}

#if HALF_ENABLE_CPP11_USER_LITERALS
	/// User-defined literals.
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
		template<typename T,typename F,bool> struct conditional { typedef T type; };

		/// Conditional type.
		template<typename T,typename F> struct conditional<T,F,false> { typedef F type; };

		/// Unsigned integer of (at least) 32 bits width.
		typedef conditional<unsigned int,unsigned long,std::numeric_limits<unsigned int>::digits>=32>::type uint32;

		/// Fastest signed integer capable of holding all values of type uint16.
		typedef conditional<int,long,std::numeric_limits<int>::digits>=16>::type int17;
	#endif

		/// Generic half expression.
		/// This class represents the base class for expressions of half-precision value, convertible to single precision.
		/// \tparam E concrete expression type
		template<typename E> struct half_expr
		{
			operator float() const;
		};

		/// Temporary half expression with internal float.
		/// This class represents a half-precision expression which just stores a single-precision value internally.
		struct float_half_expr : public half_expr<float_half_expr>
		{
			explicit float_half_expr(float f);
			operator float() const;

			/// Internal expression value stored in single-precision.
			float value;
		};

		/// \name Classification helpers
		/// \{
		template<typename T> bool isnan(T arg);
		template<typename T> bool signbit(T arg);
		/// \}

		/// \name Conversion
		/// \{
		uint16 float2half(float value);
		float half2float(uint16 value);
		/// \}

		/// \name Arithmetic operators
		/// \{
		template<typename L,typename R> float_half_expr operator+(const half_expr<L> &a, const half_expr<R> &b);
		template<typename L,typename R> float_half_expr operator-(const half_expr<L> &a, const half_expr<R> &b);
		template<typename L,typename R> float_half_expr operator*(const half_expr<L> &a, const half_expr<R> &b);
		template<typename L,typename R> float_half_expr operator/(const half_expr<L> &a, const half_expr<R> &b);
		template<typename E> const half_expr<E>& operator+(const detail::half_expr<E> &e);
		template<typename E> float_half_expr operator-(const detail::half_expr<E> &e);
		/// \}

		/// \name Streaming operators
		/// \{
		template<typename charT,typename traits,typename E> std::basic_ostream<charT,traits>& operator<<(std::basic_ostream<charT,traits> &out, const half_expr<E> &e);
		/// \}

		/// \name Basic mathematical operations
		/// \{
		template<typename E> float_half_expr abs(const half_expr<E> &arg);
		template<typename E> float_half_expr fabs(const half_expr<E> &arg);
		template<typename X,typename Y> float_half_expr fmod(const half_expr<X> &x, const half_expr<Y> &y);
		template<typename X,typename Y> float_half_expr remainder(const half_expr<X> &x, const half_expr<Y> &y);
		template<typename X,typename Y> float_half_expr remquo(const half_expr<X> &x, const half_expr<Y> &y, int *quo);
		template<typename X,typename Y,typename Z> float_half_expr fma(const half_expr<X> &x, const half_expr<Y> &y, const half_expr<Y> &z);
		template<typename X,typename Y> float_half_expr fdim(const half_expr<X> &x, const half_expr<Y> &y);
#if HALF_ENABLE_CPP11_CMATH
		template<typename X,typename Y> float_half_expr fmin(const half_expr<X> &x, const half_expr<Y> &y);
		template<typename X,typename Y> float_half_expr fmax(const half_expr<X> &x, const half_expr<Y> &y);
#endif
		/// \}

		/// \name Exponential functions
		/// \{
		template<typename E> float_half_expr exp(const half_expr<E> &arg);
		template<typename E> float_half_expr exp2(const half_expr<E> &arg);
		template<typename E> float_half_expr expm1(const half_expr<E> &arg);
		template<typename E> float_half_expr log(const half_expr<E> &arg);
		template<typename E> float_half_expr log10(const half_expr<E> &arg);
		template<typename E> float_half_expr log1p(const half_expr<E> &arg);
		template<typename E> float_half_expr log2(const half_expr<E> &arg);
		/// \}

		/// \name Power functions
		/// \{
		template<typename E> float_half_expr sqrt(const half_expr<E> &arg);
		template<typename E> float_half_expr cbrt(const half_expr<E> &arg);
		template<typename X,typename Y> float_half_expr hypot(const half_expr<X> &x, const half_expr<Y> &y);
		template<typename X,typename Y> float_half_expr pow(const half_expr<X> &base, const half_expr<Y> &exp);
		/// \}

		/// \name Trigonometric functions
		/// \{
		template<typename E> float_half_expr sin(const half_expr<E> &arg);
		template<typename E> float_half_expr cos(const half_expr<E> &arg);
		template<typename E> float_half_expr tan(const half_expr<E> &arg);
		template<typename E> float_half_expr asin(const half_expr<E> &arg);
		template<typename E> float_half_expr acos(const half_expr<E> &arg);
		template<typename E> float_half_expr atan(const half_expr<E> &arg);
		template<typename X,typename Y> float_half_expr atan2(const half_expr<X> &x, const half_expr<Y> &y);
		/// \}

		/// \name Hyperbolic functions
		/// \{
		template<typename E> float_half_expr sinh(const half_expr<E> &arg);
		template<typename E> float_half_expr cosh(const half_expr<E> &arg);
		template<typename E> float_half_expr tanh(const half_expr<E> &arg);
		template<typename E> float_half_expr asinh(const half_expr<E> &arg);
		template<typename E> float_half_expr acosh(const half_expr<E> &arg);
		template<typename E> float_half_expr atanh(const half_expr<E> &arg);
		/// \}

		/// \name Error and gamma functions
		/// \{	
		template<typename E> float_half_expr erf(const half_expr<E> &arg);
		template<typename E> float_half_expr erfc(const half_expr<E> &arg);
		template<typename E> float_half_expr lgamma(const half_expr<E> &arg);
		template<typename E> float_half_expr tgamma(const half_expr<E> &arg);
		/// \}

		/// \name Rounding
		/// \{
#if HALF_ENABLE_CPP11_CMATH
		template<typename E> float_half_expr ceil(const half_expr<E> &arg);
		template<typename E> float_half_expr floor(const half_expr<E> &arg);
		template<typename E> float_half_expr trunc(const half_expr<E> &arg);
		template<typename E> float_half_expr round(const half_expr<E> &arg);
		template<typename E> long lround(const half_expr<E> &arg);
		template<typename E> long long llround(const half_expr<E> &arg);
#endif
		template<typename E> float_half_expr nearbyint(const half_expr<E> &arg);
		template<typename E> float_half_expr rint(const half_expr<E> &arg);
		template<typename E> long lrint(const half_expr<E> &arg);
		template<typename E> long long llrint(const half_expr<E> &arg);
		/// \}
	}

	using detail::operator+;
	using detail::operator-;
	using detail::operator*;
	using detail::operator/;
	using detail::operator<<;
	using detail::abs;
	using detail::fabs;
	using detail::fmod;
	using detail::remainder;
	using detail::remquo;
	using detail::fma;
	using detail::fdim;
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
	using detail::nearbyint;
	using detail::rint;
	using detail::lrint;
	using detail::llrint;
#if HALF_ENABLE_CPP11_CMATH
	using detail::fmin;
	using detail::fmax;
	using detail::ceil;
	using detail::floor;
	using detail::trunc;
	using detail::round;
	using detail::lround;
	using detail::llround;
#endif


	/// Half-precision floating point type.
	/// This class implements an IEEE-conformant half-precision floating point type with the usual arithmetic operators and 
	/// conversions. It is implicitly convertible to single-precision floating point, which makes artihmetic expressions and 
	/// functions with mixed-type operands to be of the most precise operand type. Additionally all arithmetic operations 
	/// (and many mathematical functions) are carried out in single-precision internally. All conversions from single- to 
	/// half-precision are done using truncation (round towards zero), but temporary results inside chained arithmetic 
	/// expressions are kept in single-precision as long as possible (while of course still maintaining a strong half-precision type).
	///
	/// One more word about the size of half. Although the half is representing an IEEE 16-bit type, it only has an actual size 
	/// of 16 bits if your C++ implementation supports unsigned integers of exactly 16 bits width, which should be the case on 
	/// nearly any reasonable platform. Nevertheless on platforms not supporting 16-bit unsigned integers, a half will have an 
	/// actual size larger than 16 bits in memory.
	class half : public detail::half_expr<half>
	{
		friend class std::numeric_limits<half>;
#if HALF_ENABLE_CPP11_HASH
		friend struct std::hash<half>;
#endif

		friend bool operator==(half, half);
		friend bool operator!=(half, half);
		friend bool operator<(half, half);
		friend bool operator>(half, half);
		friend bool operator<=(half, half);
		friend bool operator>=(half, half);
		friend HALF_CONSTEXPR half operator-(half h);
		friend half fabs(half);
		friend half nanh(const char*);
		friend half ceil(half);
		friend half floor(half);
		friend half trunc(half);
		friend half round(half);
		friend long lround(half);
		friend long long llround(half);
		friend half frexp(half, int*);
		friend half modf(half, half*);
		friend half scalbln(half, long);
		friend int ilogb(half);
		friend half logb(half);
		friend half nextafter(half, half);
		friend half nexttoward(half, long double);
		friend half copysign(half, half);
		friend int fpclassify(half);
		friend bool isfinite(half);
		friend bool isinf(half);
		friend bool isnan(half);
		friend bool isnormal(half);
		friend bool signbit(half);

	public:
		/// Default constructor.
		/// This initializes the half to 0.
		HALF_CONSTEXPR half()
			: data_()
		{
		}

		/// Conversion constructor.
		/// \param rhs float to convert
		explicit half(float rhs)
			: data_(detail::float2half(rhs))
		{
		}

		/// Copy constructor.
		/// \tparam E type of concrete half expression
		/// \param rhs half expression to copy from
		template<typename E>
		half(const detail::half_expr<E> &rhs)
			: data_(detail::float2half(static_cast<float>(rhs)))
		{
		}
	
		/// Conversion to single-precision.
		/// \return single precision value representing expression value
		operator float() const
		{
			return detail::half2float(data_);
		}

		/// Assignment operator.
		/// \tparam E type of concrete half expression
		/// \param rhs half expression to copy from
		/// \return reference to this half
		template<typename E> half& operator=(const detail::half_expr<E> &rhs)
		{
			data_ = detail::float2half(static_cast<float>(rhs));
			return *this;
		}

		/// Arithmetic assignment.
		/// \tparam E type of concrete half expression
		/// \param rhs half expression to add
		/// \return reference to this half
		template<typename E> half& operator+=(const detail::half_expr<E> &rhs)
		{
			data_ = detail::float2half(detail::half2float(data_)+static_cast<float>(rhs));
			return *this;
		}

		/// Arithmetic assignment.
		/// \tparam E type of concrete half expression
		/// \param rhs half expression to subtract
		/// \return reference to this half
		template<typename E> half& operator-=(const detail::half_expr<E> &rhs)
		{
			data_ = detail::float2half(detail::half2float(data_)-static_cast<float>(rhs));
			return *this;
		}

		/// Arithmetic assignment.
		/// \tparam E type of concrete half expression
		/// \param rhs half expression to multiply with
		/// \return reference to this half
		template<typename E> half& operator*=(const detail::half_expr<E> &rhs)
		{
			data_ = detail::float2half(detail::half2float(data_)*static_cast<float>(rhs));
			return *this;
		}

		/// Arithmetic assignment.
		/// \tparam E type of concrete half expression
		/// \param rhs half expression to divide by
		/// \return reference to this half
		template<typename E> half& operator/=(const detail::half_expr<E> &rhs)
		{
			data_ = detail::float2half(detail::half2float(data_)/static_cast<float>(rhs));
			return *this;
		}

		/// Assignment operator.
		/// \param rhs single-precision value to copy from
		/// \return reference to this half
		half& operator=(float rhs)
		{
			data_ = detail::float2half(rhs);
			return *this;
		}

		/// Arithmetic assignment.
		/// \param rhs single-precision value to add
		/// \return reference to this half
		half& operator+=(float rhs)
		{
			data_ = detail::float2half(detail::half2float(data_)+rhs);
			return *this;
		}

		/// Arithmetic assignment.
		/// \param rhs single-precision value to subtract
		/// \return reference to this half
		half& operator-=(float rhs)
		{
			data_ = detail::float2half(detail::half2float(data_)-rhs);
			return *this;
		}

		/// Arithmetic assignment.
		/// \param rhs single-precision value to multiply with
		/// \return reference to this half
		half& operator*=(float rhs)
		{
			data_ = detail::float2half(detail::half2float(data_)*rhs);
			return *this;
		}

		/// Arithmetic assignment.
		/// \param rhs single-precision value to divide by
		/// \return reference to this half
		half& operator/=(float rhs)
		{
			data_ = detail::float2half(detail::half2float(data_)/rhs);
			return *this;
		}

		/// Prefix increment.
		/// \return incremented half value
		half& operator++()
		{
			data_ = detail::float2half(detail::half2float(data_)+1.0f);
			return *this;
		}

		/// Prefix decrement.
		/// \return decremented half value
		half& operator--()
		{
			data_ = detail::float2half(detail::half2float(data_)-1.0f);
			return *this;
		}

		/// Postfix increment.
		/// \return non-incremented half value
		half operator++(int)
		{
			half out(*this);
			++*this;
			return out;
		}

		/// Postfix decrement.
		/// \return non-decremented half value
		half operator--(int)
		{
			half out(*this);
			--*this;
			return out;
		}
	
	private:
		/// Constructor.
		/// \param bits binary representation to set half to
		HALF_CONSTEXPR half(detail::uint16 bits, bool)
			: data_(bits)
		{
		}

		/// Internal binary representation
		detail::uint16 data_;
	};


	/// Comparison for equality.
	/// \param a first operand
	/// \param b second operand
	/// \retval true if operands equal
	/// \retval false else
	inline bool operator==(half a, half b)
	{
		return (a.data_==b.data_ || !((a.data_|b.data_)&0x7FFF)) && !isnan(a);
	}

	/// Comparison for inequality.
	/// \param a first operand
	/// \param b second operand
	/// \retval true if operands not equal
	/// \retval false else
	inline bool operator!=(half a, half b)
	{
		return (a.data_!=b.data_ && ((a.data_|b.data_)&0x7FFF)) || isnan(a);
	}

	/// Comparison for less than.
	/// \param a first operand
	/// \param b second operand
	/// \retval true if \a a less than \a b
	/// \retval false else
	inline bool operator<(half a, half b)
	{
		if(isnan(a) || isnan(b))
			return false;
		return (signbit(a) ? (static_cast<detail::int17>(0x8000)-a.data_) : static_cast<detail::int17>(a.data_)) < 
			(signbit(b) ? (static_cast<detail::int17>(0x8000)-b.data_) : static_cast<detail::int17>(b.data_));
	}

	/// Comparison for greater than.
	/// \param a first operand
	/// \param b second operand
	/// \retval true if \a a greater than \a b
	/// \retval false else
	inline bool operator>(half a, half b)
	{
		if(isnan(a) || isnan(b))
			return false;
		return (signbit(a) ? (static_cast<detail::int17>(0x8000)-a.data_) : static_cast<detail::int17>(a.data_)) > 
			(signbit(b) ? (static_cast<detail::int17>(0x8000)-b.data_) : static_cast<detail::int17>(b.data_));
	}

	/// Comparison for less equal.
	/// \param a first operand
	/// \param b second operand
	/// \retval true if \a a less equal \a b
	/// \retval false else
	inline bool operator<=(half a, half b)
	{
		if(isnan(a) || isnan(b))
			return false;
		return (signbit(a) ? (static_cast<detail::int17>(0x8000)-a.data_) : static_cast<detail::int17>(a.data_)) <= 
			(signbit(b) ? (static_cast<detail::int17>(0x8000)-b.data_) : static_cast<detail::int17>(b.data_));
	}

	/// Comparison for greater equal.
	/// \param a first operand
	/// \param b second operand
	/// \retval true if \a a greater equal \a b
	/// \retval false else
	inline bool operator>=(half a, half b)
	{
		if(isnan(a) || isnan(b))
			return false;
		return (signbit(a) ? (static_cast<detail::int17>(0x8000)-a.data_) : static_cast<detail::int17>(a.data_)) >= 
			(signbit(b) ? (static_cast<detail::int17>(0x8000)-b.data_) : static_cast<detail::int17>(b.data_));
	}

	/// Identity.
	/// \param h operand
	/// \return uncahnged operand
	inline HALF_CONSTEXPR half operator+(half h)
	{
		return h;
	}

	/// Negation.
	/// \param h operand
	/// \return negated operand
	inline HALF_CONSTEXPR half operator-(half h)
	{
		return half(h.data_^0x8000, true);
	}

	/// Input operator.
	/// \tparam charT character type
	/// \tparam traits character traits
	/// \param in input stream to read from
	/// \param h half to read into
	/// \return reference to input stream
	template<typename charT,typename traits> std::basic_istream<charT,traits>& operator>>(std::basic_istream<charT,traits> &in, half &h)
	{
		float f;
		if(in >> f)
			h = f;
		return in;
	}

	/// Absolute value.
	/// \param arg operand
	/// \return absolute value of \a arg
	inline half abs(half arg)
	{
		return fabs(arg);
	}

	/// Absolute value.
	/// \param arg operand
	/// \return absolute value of \a arg
	inline half fabs(half arg)
	{
		return half(arg.data_&0x7FFF, true);
	}

	/// Minimum of halfs.
	/// \param x first operand
	/// \param y second operand
	/// \return minimum of operands
	inline half fmin(half x, half y)
	{
		return std::min(x, y);
	}

	/// Maximum of halfs.
	/// \param x first operand
	/// \param y second operand
	/// \return maximum of operands
	inline half fmax(half x, half y)
	{
		return std::max(x, y);
	}

	/// Get NaN value.
	/// \return quiet NaN
	inline half nanh(const char*)
	{
		return half(0x7FFF, true);
	}

	/// Nearest integer not less than half value.
	/// \param arg half to round
	/// \return nearest integer not less than \a arg
	inline half ceil(half arg)
	{
		unsigned int e = arg.data_ & 0x7C00;
		if(e > 0x6000)
			return arg;
		if(e < 0x3C00)
			return half((arg.data_&0x8000)|(0x3C00&-static_cast<detail::uint16>(~(arg.data_>>15)&((arg.data_&0x7FFF)!=0))), true);
		e = 25 - (e>>10);
		unsigned int mask = (1<<e) - 1;
		return half((arg.data_&~mask)+((~(arg.data_>>15)&((arg.data_&mask)!=0))<<e), true);
	}

	/// Nearest integer not greater than half value.
	/// \param arg half to round
	/// \return nearest integer not greater than \a arg
	inline half floor(half arg)
	{
		unsigned int e = arg.data_ & 0x7C00;
		if(e > 0x6000)
			return arg;
		if(e < 0x3C00)
			return half((arg.data_&0x8000)|(0x3C00&-static_cast<detail::uint16>((arg.data_>>15)&((arg.data_&0x7FFF)!=0))), true);
		e = 25 - (e>>10);
		unsigned int mask = (1<<e) - 1;
		return half((arg.data_&~mask)+(((arg.data_>>15)&((arg.data_&mask)!=0))<<e), true);
	}

	/// Nearest integer not greater in magnitude than half value.
	/// \param arg half to round
	/// \return nearest integer not greater in magnitude than \a arg
	inline half trunc(half arg)
	{
		unsigned int e = arg.data_ & 0x7C00;
		if(e > 0x6000)
			return arg;
		if(e < 0x3C00)
			return half(arg.data_&0x8000, true);
		return half(arg.data_&~((1<<(25-(e>>10)))-1), true);
	}

	/// Nearest integer.
	/// \param arg half to round
	/// \return nearest integer, rounded away from zero in half-way cases
	inline half round(half arg)
	{
		unsigned int e = arg.data_ & 0x7C00;
		if(e > 0x6000)
			return arg;
		if(e < 0x3C00)
			return half((arg.data_&0x8000)|(0x3C00&-static_cast<detail::uint16>((arg.data_&0x7FFF)>=0x3800)), true);
		e >>= 10;
		return half((arg.data_+(1<<(24-e)))&~((1<<(25-e))-1), true);
	}

	/// Nearest integer.
	/// \param arg half to round
	/// \return nearest integer, rounded away from zero in half-way cases
	inline long lround(half arg)
	{
		unsigned int e = arg.data_ & 0x7C00;
		if(e == 0x7C00)
			return signbit(arg) ? std::numeric_limits<long>::min() : std::numeric_limits<long>::max();
		if(e < 0x3800)
			return 0L;
		unsigned int m = (arg.data_&0x3FF) | 0x400;
		e >>= 10;
		if(e > 25)
			m <<= e - 25;
		else if(e < 25)
			m = ((m>>(24-e))+1) >> 1;
		return signbit(arg) ? -static_cast<long>(m) : static_cast<long>(m);
	}

	/// Nearest integer.
	/// \param arg half to round
	/// \return nearest integer, rounded away from zero in half-way cases
	inline long long llround(half arg)
	{
		long l = lround(arg);
		return (l>std::numeric_limits<long>::min()) ? ((l<std::numeric_limits<long>::max()) ? 
			static_cast<long long>(l) : std::numeric_limits<long long>::max()) : std::numeric_limits<long long>::min();
	}

	/// Decompress floating point number.
	/// \param arg number to decompress
	/// \param exp address to store exponent at
	/// \return significant in range [0.5, 1)
	inline half frexp(half arg, int *exp)
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

	/// Multiply by power of two.
	/// \param arg number to modify
	/// \param exp power of two to multiply with
	/// \return \a arg multplied by 2 raised to \a exp
	inline half ldexp(half arg, int exp)
	{
		return scalbln(arg, exp);
	}

	/// Extract integer and fractional parts.
	/// \param x number to decompress
	/// \param iptr address to store integer part at
	/// \return fractional part
	inline half modf(half x, half *iptr)
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

	/// Multiply by power of two.
	/// \param x number to modify
	/// \param exp power of two to multiply with
	/// \return \a arg multplied by 2 raised to \a exp
	inline half scalbn(half x, int exp)
	{
		return scalbln(x, exp);
	}

	/// Multiply by power of two.
	/// \param x number to modify
	/// \param exp power of two to multiply with
	/// \return \a arg multplied by 2 raised to \a exp	
	inline half scalbln(half x, long exp)
	{
		int e = x.data_ & 0x7C00;
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
		unsigned int sign = x.data_ & 0x8000;
		e += exp;
		return (e>30) ? half(sign|0x7C00, true) : half((e>0) ? (sign|(e<<10)|(m&0x3FF)) : ((e<-9) ? sign : (sign|(m>>(1-e)))), true);
	}

	/// Extract exponent.
	/// \param arg number to query
	/// \return floating point exponent
	/// \retval FP_ILOGB0 for zero
	/// \retval FP_ILOGBNAN for NaN
	/// \retval MAX_INT for infinity
	inline int ilogb(half arg)
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

	/// Extract exponent.
	/// \param arg number to query
	/// \return floating point exponent
	inline half logb(half arg)
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

	/// Next representable value.
	/// \param from value to compute next representable value for
	/// \param to direction towards which to compute next value
	/// \return next representable value after \a from in direction towards \a to
	inline half nextafter(half from, half to)
	{
		detail::uint16 fabs = from.data_ & 0x7FFF, tabs = to.data_ & 0x7FFF;
		if(fabs > 0x7C00)
			return from;
		if(tabs > 0x7C00 || from.data_==to.data_ || !(fabs|tabs))
			return to;
		if(!fabs)
			return half((to.data_&0x8000)+1, true);
		bool lt = (signbit(from) ? (static_cast<detail::int17>(0x8000)-from.data_) : static_cast<detail::int17>(from.data_)) < 
			(signbit(to) ? (static_cast<detail::int17>(0x8000)-to.data_) : static_cast<detail::int17>(to.data_));
		return half((from.data_+(((from.data_>>15)^static_cast<detail::uint16>(lt))<<1))-1, true);
	}

	/// Next representable value.
	/// \param from value to compute next representable value for
	/// \param to direction towards which to compute next value
	/// \return next representable value after \a from in direction towards \a to
	inline half nexttoward(half from, long double to)
	{
		if(isnan(from))
			return from;
		long double lfrom = static_cast<long double>(from);
		if(detail::isnan(to) || lfrom == to)
			return half(static_cast<float>(to));
		if(!(from.data_&0x7FFF))
			return half((static_cast<detail::uint16>(detail::signbit(to))<<15)+1, true);
		return half((from.data_+(((from.data_>>15)^static_cast<detail::uint16>(lfrom<to))<<1))-1, true);
	}

	/// Take sign.
	/// \param x value to change sign for
	/// \param y value to take sign from
	/// \return value equal to \a x in magnitude and to \a y in sign
	inline half copysign(half x, half y)
	{
		return half(x.data_^((x.data_^y.data_)&0x8000), true);
	}

	/// Classify floating point value.
	/// \param arg number to classify
	/// \retval FP_ZERO for positive and negative zero
	/// \retval FP_SUBNORMAL for subnormal numbers
	/// \retval FP_INFINITY for positive and negative infinity
	/// \retval FP_NAN for NaNs
	/// \retval FP_NORMAL for all other (normal) values
	inline int fpclassify(half arg)
	{
		unsigned int e = arg.data_ & 0x7C00;
		if(e == 0)
			return (arg.data_&0x3FF) ? FP_SUBNORMAL : FP_ZERO;
		if(e == 0x7C00)
			return (arg.data_&0x3FF) ? FP_NAN : FP_INFINITE;
		return FP_NORMAL;
	}

	/// Check if finite number.
	/// \param arg number to check
	/// \retval true if neither infinity nor NaN
	/// \retval false else
	inline bool isfinite(half arg)
	{
		return (arg.data_&0x7C00) != 0x7C00;
	}

	/// Check for infinity.
	/// \param arg number to check
	/// \retval true for positive or negative infinity
	/// \retval false else
	inline bool isinf(half arg)
	{
		return (arg.data_&0x7FFF) == 0x7C00;
	}

	/// Check for NaN.
	/// \param arg number to check
	/// \retval true for NaNs
	/// \retval false else
	inline bool isnan(half arg)
	{
		return (arg.data_&0x7FFF) > 0x7C00;
	}

	/// Check if normal number.
	/// \param arg number to check
	/// \retval true if normal number
	/// \retval false if either subnormal, zero, infinity or NaN
	inline bool isnormal(half arg)
	{
		return ((arg.data_&0x7C00)!=0) & ((arg.data_&0x7C00)!=0x7C00);
	}

	/// Check sign.
	/// \param arg number to check
	/// \retval true for negative number
	/// \retval false for positive number
	inline bool signbit(half arg)
	{
		return (arg.data_&0x8000) != 0;
	}

	/// Comparison for greater than.
	/// \param x first operand
	/// \param y second operand
	/// \retval true if \a x greater than \a y
	/// \retval false else
	inline bool isgreater(half x, half y)
	{
		return x > y;
	}

	/// Comparison for greater equal.
	/// \param x first operand
	/// \param y second operand
	/// \retval true if \a x greater equal \a y
	/// \retval false else
	inline bool isgreaterequal(half x, half y)
	{
		return x >= y;
	}

	/// Comparison for less than.
	/// \param x first operand
	/// \param y second operand
	/// \retval true if \a x less than \a y
	/// \retval false else
	inline bool isless(half x, half y)
	{
		return x < y;
	}

	/// Comparison for less equal.
	/// \param x first operand
	/// \param y second operand
	/// \retval true if \a x less equal \a y
	/// \retval false else
	inline bool islessequal(half x, half y)
	{
		return x <= y;
	}

	/// Comarison for less or greater.
	/// \param x first operand
	/// \param y second operand
	/// \retval true if either less or greater
	/// \retval false else
	inline bool islessgreater(half x, half y)
	{
		return x < y || x > y;
	}

	/// Check if unordered.
	/// \param x first operand
	/// \param y second operand
	/// \retval true if unordered (one or two NaN operands)
	/// \retval false else
	inline bool isunordered(half x, half y)
	{
		return isnan(x) || isnan(y);
	}

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
		template<typename T> bool isnan(T arg)
		{
		#if HALF_ENABLE_CPP11_CMATH
			return std::isnan(arg);
		#elif defined(_MSC_VER)
			return _isnan(arg) != 0;
		#else
			return arg != arg;
		#endif
		}

		/// Check sign.
		/// \tparam T argument type (builtin floating point type)
		/// \param arg value to query
		/// \retval true if signbit set
		/// \retval false else
		template<typename T> bool signbit(T arg)
		{
		#if HALF_ENABLE_CPP11_CMATH
			return std::signbit(arg);
		#else
			return arg < T();
		#endif
		}

		/// Convert IEEE single-precision to half-precision.
		/// \param value single-precision value
		/// \return binary representation of half-precision value
		inline uint16 float2half(float value)
		{
		#if HALF_ENABLE_CPP11_STATIC_ASSERT
			static_assert(std::numeric_limits<float>::is_iec559, "float to half conversion needs IEEE 754 conformant 'float' type");
			static_assert(sizeof(uint32)==sizeof(float), "float to half conversion needs unsigned integer type of exactly 32 bits width");
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
			return base_table[bits>>23] + ((bits&0x7FFFFF)>>shift_table[bits>>23]);
		}
/*
		/// Convert non-IEEE single-precision to half-precision.
		/// \param value single-precision value
		/// \return binary representation of half-precision value
		inline uint16 float2half_impl(float value, const std::false_type&)
		{
			unsigned int sign = signbit(value) << 15;
			if(value == 0.0f)
				return sign;
			if(isnan(value))
				return sign | 0x7FFF;
//			if(std::isinf(value))
//				return sign | 0x7C00;
			if(value == std::numeric_limits<float>::infinity())
				return 0x7C00;
			if(value == -std::numeric_limits<float>::infinity())
				return 0xFC00;
			int exp;
			float mantf = std::frexp(value, &exp);
			if(exp > 16)
				return sign | 0x7C00;
			if(exp < -23)
				return sign;
			unsigned int mant = std::abs(mantf*2048.0f);
			if(exp < -13)
				return sign | ((mant>>(-13-exp))&0x3FF);
			return sign | ((exp+14)<<10) | (mant&0x3FF);
		}

		/// Convert single-precision to half-precision.
		/// \param value single-precision value
		/// \return binary representation of half-precision value
		inline uint16 float2half(float value)
		{
			return float2half_impl(value, std::integral_constant<bool,std::numeric_limits<float>::is_iec559>());
		}
*/
		/// Convert half-precision to IEEE single-precision.
		/// \param value binary representation of half-precision value
		/// \return single-precision value
		inline float half2float(uint16 value)
		{
		#if HALF_ENABLE_CPP11_STATIC_ASSERT
			static_assert(std::numeric_limits<float>::is_iec559, "half to float conversion needs IEEE 754 conformant 'float' type");
			static_assert(sizeof(uint32)==sizeof(float), "half to float conversion needs unsigned integer type of exactly 32 bits width");
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
		inline float half2float_impl(uint16 value, const std::false_type&)
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
					if(mant == 0)
						return sign ? -0.0f : 0.0f;
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
			return half2float_impl(value, std::integral_constant<bool,std::numeric_limits<float>::is_iec559>());
		}
*/
		/// Conversion to single-precision.
		/// \return single precision value representing expression value
		template<typename E> half_expr<E>::operator float() const
		{
				return static_cast<float>(*static_cast<const E*>(this));
		}

		/// Conversion constructor.
		/// \param f single-precision value to convert
		inline float_half_expr::float_half_expr(float f)
				: value(f)
		{
		}

		/// Conversion to single-precision.
		/// \return single precision value representing expression value
		inline float_half_expr::operator float() const
		{
			return value;
		}

		/// Add halfs.
		/// \tparam L type of left half expression
		/// \tparam R type of right half expression
		/// \param a left operand
		/// \param b right operand
		/// \return sum of half expressions
		template<typename L,typename R> float_half_expr operator+(const half_expr<L> &a, const half_expr<R> &b)
		{
			return float_half_expr(static_cast<float>(a)+static_cast<float>(b));
		}

		/// Subtract halfs.
		/// \tparam L type of left half expression
		/// \tparam R type of right half expression
		/// \param a left operand
		/// \param b right operand
		/// \return difference of half expressions
		template<typename L,typename R> float_half_expr operator-(const half_expr<L> &a, const half_expr<R> &b)
		{
			return float_half_expr(static_cast<float>(a)-static_cast<float>(b));
		}

		/// Multiply halfs.
		/// \tparam L type of left half expression
		/// \tparam R type of right half expression
		/// \param a left operand
		/// \param b right operand
		/// \return product of half expressions
		template<typename L,typename R> float_half_expr operator*(const half_expr<L> &a, const half_expr<R> &b)
		{
			return float_half_expr(static_cast<float>(a)*static_cast<float>(b));
		}

		/// Divide halfs.
		/// \tparam L type of left half expression
		/// \tparam R type of right half expression
		/// \param a left operand
		/// \param b right operand
		/// \return quotient of half expressions
		template<typename L,typename R> float_half_expr operator/(const half_expr<L> &a, const half_expr<R> &b)
		{
			return float_half_expr(static_cast<float>(a)/static_cast<float>(b));
		}

		/// Unary plus.
		/// \tparam E type of half expression
		/// \param e half expression
		/// \return identical value
		template<typename E> const half_expr<E>& operator+(const detail::half_expr<E> &e)
		{
			return e;
		}

		/// Negation.
		/// \tparam E type of half expression
		/// \param e half expression
		/// \return negated half expression
		template<typename E> float_half_expr operator-(const detail::half_expr<E> &e)
		{
			return float_half_expr(-static_cast<float>(e));
		}

		/// Output operator.
		/// \tparam charT character type
		/// \tparam traits character traits
		/// \tparam E type of half expression
		/// \param out output stream to write into
		/// \param e half expression to write
		/// \return reference to output stream
		template<typename charT,typename traits,typename E> std::basic_ostream<charT,traits>& operator<<(std::basic_ostream<charT,traits> &out, const half_expr<E> &e)
		{
			return out << static_cast<float>(e);
		}

		/// Absolute value.
		/// \tparam E type of half expression
		/// \param arg operand
		/// \return absolute value of \a arg
		template<typename E> float_half_expr abs(const half_expr<E> &arg)
		{
			return float_half_expr(std::abs(static_cast<float>(arg)));
		}

		/// Absolute value.
		/// \tparam E type of half expression
		/// \param arg operand
		/// \return absolute value of \a arg
		template<typename E> float_half_expr fabs(const half_expr<E> &arg)
		{
			return float_half_expr(std::fabs(static_cast<float>(arg)));
		}

		/// Remainder of division.
		/// \tparam X type of first expression
		/// \tparam Y type of second expression
		/// \param x first operand
		/// \param y second operand
		/// \return remainder of floating point division.
		template<typename X,typename Y> float_half_expr fmod(const half_expr<X> &x, const half_expr<Y> &y)
		{
			return float_half_expr(std::fmod(static_cast<float>(x), static_cast<float>(y)));
		}

		/// Remainder of division.
		/// \tparam X type of first expression
		/// \tparam Y type of second expression
		/// \param x first operand
		/// \param y second operand
		/// \return remainder of floating point division.
		template<typename X,typename Y> float_half_expr remainder(const half_expr<X> &x, const half_expr<Y> &y)
		{
			return float_half_expr(std::remainder(static_cast<float>(x), static_cast<float>(y)));
		}

		/// Remainder of division.
		/// \tparam X type of first expression
		/// \tparam Y type of second expression
		/// \param x first operand
		/// \param y second operand
		/// \param quo address to store some bits of quotient at
		/// \return remainder of floating point division.
		template<typename X,typename Y> float_half_expr remquo(const half_expr<X> &x, const half_expr<Y> &y, int *quo)
		{
			return float_half_expr(std::remquo(static_cast<float>(x), static_cast<float>(y), quo));
		}

		/// Fused multiply add.
		/// \tparam X type of first expression
		/// \tparam Y type of second expression
		/// \tparam Z type of third expression
		/// \param x first operand
		/// \param y second operand
		/// \param z third operand
		/// \return ( \a x * \a y ) + \a z rounded as one operation.
		template<typename X,typename Y,typename Z> float_half_expr fma(const half_expr<X> &x, const half_expr<Y> &y, const half_expr<Y> &z)
		{
			return float_half_expr(std::fma(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)));
		}
#if HALF_ENABLE_CPP11_CMATH
		/// Minimum of half expressions.
		/// \tparam X type of first expression
		/// \tparam Y type of second expression
		/// \param x first operand
		/// \param y second operand
		/// \return minimum of operands
		template<typename X,typename Y> float_half_expr fmin(const half_expr<X> &x, const half_expr<Y> &y)
		{
			return float_half_expr(std::fmin(static_cast<float>(x), static_cast<float>(y)));
		}

		/// Maximum of half expressions.
		/// \tparam X type of first expression
		/// \tparam Y type of second expression
		/// \param x first operand
		/// \param y second operand
		/// \return maximum of operands
		template<typename X,typename Y> float_half_expr fmax(const half_expr<X> &x, const half_expr<Y> &y)
		{
			return float_half_expr(std::fmax(static_cast<float>(x), static_cast<float>(y)));
		}
#endif
		/// Positive difference.
		/// \tparam X type of first expression
		/// \tparam Y type of second expression
		/// \param x first operand
		/// \param y second operand
		/// \return \a x - \a y or 0 if difference negative
		template<typename X,typename Y> float_half_expr fdim(const half_expr<X> &x, const half_expr<Y> &y)
		{
			return float_half_expr(std::fdim(static_cast<float>(x), static_cast<float>(y)));
		}

		/// Exponential function.
		/// \tparam E type of half expression
		/// \param arg operand
		/// \return e raised to \a arg
		template<typename E> float_half_expr exp(const half_expr<E> &arg)
		{
			return float_half_expr(std::exp(static_cast<float>(arg)));
		}

		/// Binary exponential.
		/// \tparam E type of half expression
		/// \param arg operand
		/// \return 2 raised to \a arg
		template<typename E> float_half_expr exp2(const half_expr<E> &arg)
		{
			return float_half_expr(std::exp2(static_cast<float>(arg)));
		}

		/// Exponential minus one.
		/// \tparam E type of half expression
		/// \param arg operand
		/// \return e raised to \a arg subtracted by 1
		template<typename E> float_half_expr expm1(const half_expr<E> &arg)
		{
			return float_half_expr(std::expm1(static_cast<float>(arg)));
		}

		/// Natural logorithm.
		/// \tparam E type of half expression
		/// \param arg operand
		/// \return logarithm of \a arg to base e
		template<typename E> float_half_expr log(const half_expr<E> &arg)
		{
			return float_half_expr(std::log(static_cast<float>(arg)));
		}

		/// Common logorithm.
		/// \tparam E type of half expression
		/// \param arg operand
		/// \return logarithm of \a arg to base 10
		template<typename E> float_half_expr log10(const half_expr<E> &arg)
		{
			return float_half_expr(std::log10(static_cast<float>(arg)));
		}

		/// Natural logorithm.
		/// \tparam E type of half expression
		/// \param arg operand
		/// \return logarithm of \a arg plus 1 to base e
		template<typename E> float_half_expr log1p(const half_expr<E> &arg)
		{
			return float_half_expr(std::log1p(static_cast<float>(arg)));
		}

		/// Binary logorithm.
		/// \tparam E type of half expression
		/// \param arg operand
		/// \return logarithm of \a arg to base 2
		template<typename E> float_half_expr log2(const half_expr<E> &arg)
		{
			return float_half_expr(std::log2(static_cast<float>(arg)));
		}

		/// Square root.
		/// \tparam E type of half expression
		/// \param arg operand
		/// \return square root of \a arg
		template<typename E> float_half_expr sqrt(const half_expr<E> &arg)
		{
			return float_half_expr(std::sqrt(static_cast<float>(arg)));
		}

		/// Cubic root.
		/// \tparam E type of half expression
		/// \param arg operand
		/// \return cubic root of \a arg
		template<typename E> float_half_expr cbrt(const half_expr<E> &arg)
		{
			return float_half_expr(std::cbrt(static_cast<float>(arg)));
		}

		/// Hypotenuse function.
		/// \tparam X type of first expression
		/// \tparam Y type of second expression
		/// \param x first operand
		/// \param y second operand
		/// \return square root of sum of squares
		template<typename X,typename Y> float_half_expr hypot(const half_expr<X> &x, const half_expr<Y> &y)
		{
			return float_half_expr(std::hypot(static_cast<float>(x), static_cast<float>(y)));
		}

		/// Power function.
		/// \tparam X type of first expression
		/// \tparam Y type of second expression
		/// \param base first operand
		/// \param exp second operand
		/// \return \a base raised to \a exp
		template<typename X,typename Y> float_half_expr pow(const half_expr<X> &base, const half_expr<Y> &exp)
		{
			return float_half_expr(std::pow(static_cast<float>(base), static_cast<float>(exp)));
		}

		/// Sine function.
		/// \tparam E type of half expression
		/// \param arg operand
		/// \return sine value of \a arg
		template<typename E> float_half_expr sin(const half_expr<E> &arg)
		{
			return float_half_expr(std::sin(static_cast<float>(arg)));
		}

		/// Cosine function.
		/// \tparam E type of half expression
		/// \param arg operand
		/// \return cosine value of \a arg
		template<typename E> float_half_expr cos(const half_expr<E> &arg)
		{
			return float_half_expr(std::cos(static_cast<float>(arg)));
		}

		/// Tangent function.
		/// \tparam E type of half expression
		/// \param arg operand
		/// \return tangent value of \a arg
		template<typename E> float_half_expr tan(const half_expr<E> &arg)
		{
			return float_half_expr(std::tan(static_cast<float>(arg)));
		}

		/// Arc sine.
		/// \tparam E type of half expression
		/// \param arg operand
		/// \return arc sine value of \a arg
		template<typename E> float_half_expr asin(const half_expr<E> &arg)
		{
			return float_half_expr(std::asin(static_cast<float>(arg)));
		}

		/// Arc cosine function.
		/// \tparam E type of half expression
		/// \param arg operand
		/// \return arc cosine value of \a arg
		template<typename E> float_half_expr acos(const half_expr<E> &arg)
		{
			return float_half_expr(std::acos(static_cast<float>(arg)));
		}

		/// Arc tangent function.
		/// \tparam E type of half expression
		/// \param arg operand
		/// \return arc tangent value of \a arg
		template<typename E> float_half_expr atan(const half_expr<E> &arg)
		{
			return float_half_expr(std::atan(static_cast<float>(arg)));
		}

		/// Arc tangent function.
		/// \tparam X type of first expression
		/// \tparam Y type of second expression
		/// \param x first operand
		/// \param y second operand
		/// \return arc tangent value
		template<typename X,typename Y> float_half_expr atan2(const half_expr<X> &x, const half_expr<Y> &y)
		{
			return float_half_expr(std::atan2(static_cast<float>(x), static_cast<float>(y)));
		}

		/// Hyperbolic sine.
		/// \tparam E type of half expression
		/// \param arg operand
		/// \return hyperbolic sine value of \a arg
		template<typename E> float_half_expr sinh(const half_expr<E> &arg)
		{
			return float_half_expr(std::sinh(static_cast<float>(arg)));
		}

		/// Hyperbolic cosine.
		/// \tparam E type of half expression
		/// \param arg operand
		/// \return hyperbolic cosine value of \a arg
		template<typename E> float_half_expr cosh(const half_expr<E> &arg)
		{
			return float_half_expr(std::cosh(static_cast<float>(arg)));
		}

		/// Hyperbolic tangent.
		/// \tparam E type of half expression
		/// \param arg operand
		/// \return hyperbolic tangent value of \a arg
		template<typename E> float_half_expr tanh(const half_expr<E> &arg)
		{
			return float_half_expr(std::tanh(static_cast<float>(arg)));
		}

		/// Hyperbolic arc sine.
		/// \tparam E type of half expression
		/// \param arg operand
		/// \return hyperbolic arc sine value of \a arg
		template<typename E> float_half_expr asinh(const half_expr<E> &arg)
		{
			return float_half_expr(std::asinh(static_cast<float>(arg)));
		}

		/// Hyperbolic arc cosine.
		/// \tparam E type of half expression
		/// \param arg operand
		/// \return hyperbolic arc cosine value of \a arg
		template<typename E> float_half_expr acosh(const half_expr<E> &arg)
		{
			return float_half_expr(std::acosh(static_cast<float>(arg)));
		}

		/// Hyperbolic arc tangent.
		/// \tparam E type of half expression
		/// \param arg operand
		/// \return hyperbolic arc tangent value of \a arg
		template<typename E> float_half_expr atanh(const half_expr<E> &arg)
		{
			return float_half_expr(std::atanh(static_cast<float>(arg)));
		}

		/// Error function.
		/// \tparam E type of half expression
		/// \param arg operand
		/// \return error function value of \a arg
		template<typename E> float_half_expr erf(const half_expr<E> &arg)
		{
			return float_half_expr(std::erf(static_cast<float>(arg)));
		}

		/// Complementary error function.
		/// \tparam E type of half expression
		/// \param arg operand
		/// \return 1 minus error function value of \a arg
		template<typename E> float_half_expr erfc(const half_expr<E> &arg)
		{
			return float_half_expr(std::erfc(static_cast<float>(arg)));
		}

		/// Natural logarithm of gamma function.
		/// \tparam E type of half expression
		/// \param arg operand
		/// \return natural logarith of gamma function for \a arg
		template<typename E> float_half_expr lgamma(const half_expr<E> &arg)
		{
			return float_half_expr(std::lgamma(static_cast<float>(arg)));
		}

		/// Gamma function.
		/// \tparam E type of half expression
		/// \param arg operand
		/// \return gamma function value of \a arg
		template<typename E> float_half_expr tgamma(const half_expr<E> &arg)
		{
			return float_half_expr(std::tgamma(static_cast<float>(arg)));
		}
#if HALF_ENABLE_CPP11_CMATH
		/// Nearest integer not less than half value.
		/// \tparam E type of half expression
		/// \param arg half expression to round
		/// \return nearest integer not less than \a arg
		template<typename E> float_half_expr ceil(const half_expr<E> &arg)
		{
			return float_half_expr(std::ceil(static_cast<float>(arg)));
		}

		/// Nearest integer not greater than half value.
		/// \tparam E type of half expression
		/// \param arg half expression to round
		/// \return nearest integer not greater than \a arg
		template<typename E> float_half_expr floor(const half_expr<E> &arg)
		{
			return float_half_expr(std::floor(static_cast<float>(arg)));
		}

		/// Nearest integer not greater in magnitude than half value.
		/// \tparam E type of half expression
		/// \param arg half expression to round
		/// \return nearest integer not greater in magnitude than \a arg
		template<typename E> float_half_expr trunc(const half_expr<E> &arg)
		{
			return float_half_expr(std::trunc(static_cast<float>(arg)));
		}

		/// Nearest integer.
		/// \tparam E type of half expression
		/// \param arg half expression to round
		/// \return nearest integer, rounded away from zero in half-way cases
		template<typename E> float_half_expr round(const half_expr<E> &arg)
		{
			return float_half_expr(std::round(static_cast<float>(arg)));
		}

		/// Nearest integer.
		/// \tparam E type of half expression
		/// \param arg half expression to round
		/// \return nearest integer, rounded away from zero in half-way cases
		template<typename E> long lround(const half_expr<E> &arg)
		{
			return std::lround(static_cast<float>(arg));
		}

		/// Nearest integer.
		/// \tparam E type of half expression
		/// \param arg half expression to round
		/// \return nearest integer, rounded away from zero in half-way cases
		template<typename E> long long llround(const half_expr<E> &arg)
		{
			return std::llround(static_cast<float>(arg));
		}
#endif
		/// Nearest integer.
		/// \tparam E type of half expression
		/// \param arg half expression to round
		/// \return nearest integer using current rounding mode
		template<typename E> float_half_expr nearbyint(const half_expr<E> &arg)
		{
			return float_half_expr(std::nearbyint(static_cast<float>(arg)));
		}

		/// Nearest integer.
		/// \tparam E type of half expression
		/// \param arg half expression to round
		/// \return nearest integer using current rounding mode
		template<typename E> float_half_expr rint(const half_expr<E> &arg)
		{
			return float_half_expr(std::rint(static_cast<float>(arg)));
		}

		/// Nearest integer.
		/// \tparam E type of half expression
		/// \param arg half expression to round
		/// \return nearest integer using current rounding mode
		template<typename E> long lrint(const half_expr<E> &arg)
		{
			return std::lrint(static_cast<float>(arg));
		}

		/// Nearest integer.
		/// \tparam E type of half expression
		/// \param arg half expression to round
		/// \return nearest integer using current rounding mode
		template<typename E> long long llrint(const half_expr<E> &arg)
		{
			return std::llrint(static_cast<float>(arg));
		}
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
		/// single-precision implementation) with explicit truncation of the single-to-half conversions, the rounding mode is 
		/// only round-toward-zero if the single-precision rounding mode is also round-toward-zero (which is very unlikely). In 
		/// all other cases the half-precision rounding mode is indeterminate.
		static HALF_CONSTEXPR_CONST std::float_round_style round_style = 
			(std::numeric_limits<float>::round_style==std::round_toward_zero) ? std::round_toward_zero : std::round_indeterminate;

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
		static HALF_CONSTEXPR half_float::half min() { return half_float::half(0x0400, true); }

		/// Smallest finite value.
		static HALF_CONSTEXPR half_float::half lowest() { return half_float::half(0xFBFF, true); }

		/// Largest finite value.
		static HALF_CONSTEXPR half_float::half max() { return half_float::half(0x7BFF, true); }

		/// Difference between one and next representable value.
		static HALF_CONSTEXPR half_float::half epsilon() { return half_float::half(0x1400, true); }

		/// Maximum rounding error.
		static HALF_CONSTEXPR half_float::half round_error() { return half_float::half(0x3C00, true); }

		/// Positive infinity.
		static HALF_CONSTEXPR half_float::half infinity() { return half_float::half(0x7C00, true); }

		/// Quiet NaN.
		static HALF_CONSTEXPR half_float::half quiet_NaN() { return half_float::half(0x7FFF, true); }

		/// Signalling NaN.
		static HALF_CONSTEXPR half_float::half signaling_NaN() { return half_float::half(0x7DFF, true); }

		/// Smallest positive subnormal value.
		static HALF_CONSTEXPR half_float::half denorm_min() { return half_float::half(0x0001, true); }
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
			return std::hash<half_float::detail::uint16>()(-static_cast<half_float::detail::uint16>(arg.data_!=0x8000)&arg.data_);
		}
	};
#endif
}


#undef HALF_CONSTEXPR
#undef HALF_CONSTEXPR_CONST

#endif
