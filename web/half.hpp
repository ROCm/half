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

// This is a dummy header file to provide a clean interface documentation to Doxygen, without any ugly internals.

/// \file
/// Main header file of the library.

#ifndef HALF_HALF_H
#define HALF_HALF_H

#include <limits>
#include <functional>
#include <iosfwd>
#include <cstdint>


/// Value signaling overflow.
/// In correspondence with `HUGE_VAL[F|L]` from `<cmath>` this symbol expands to a positive value signaling the overflow of an 
/// operation, in particular it just evaluates to positive infinity.
#define HUGE_VALH		std::numeric_limits<half_float::half>::infinity()

/// Fast half-precision fma function.
/// This symbol is only defined if the fma() function generally executes as fast as, or faster than, a separate half-precision 
/// multiplication followed by an addition. Due to the internal single-precision implementation of this function, it is only 
/// defined if the corresponding `FP_FAST_FMAF` symbol from `<cmath>` is defined, too.
#define FP_FAST_FMAH	1


/// Main namespace for half precision functionality.
/// This namespace contains all the functionality provided by the library.
namespace half_float
{
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
	class half
	{
	public:
		/// \name Construction and assignment
		/// \{

		/// Default constructor.
		/// This initializes the half to 0. Although this does not match the builtin types' default-initialization semantics 
		/// and may be less efficient than no initialization, it is needed to provide proper value-initialization semantics.
		constexpr half();

		/// Conversion constructor.
		/// \param rhs float to convert
		explicit half(float rhs);

		/// Conversion to single-precision.
		/// \return single precision value
		operator float() const;

		/// Single-precision assignment.
		/// \param rhs single-precision value to copy from
		/// \return reference to this half
		half& operator=(float rhs);

		/// \}
		/// \name Arithmetic updates
		/// \{

		/// Arithmetic assignment.
		/// This operation uses the underlying single-precision implementation.
		/// \tparam E type of concrete half expression
		/// \param rhs half to add
		/// \return reference to this half
		half& operator+=(half rhs);

		/// Arithmetic assignment.
		/// This operation uses the underlying single-precision implementation.
		/// \tparam E type of concrete half expression
		/// \param rhs half to subtract
		/// \return reference to this half
		half& operator-=(half rhs);

		/// Arithmetic assignment.
		/// This operation uses the underlying single-precision implementation.
		/// \tparam E type of concrete half expression
		/// \param rhs half to multiply with
		/// \return reference to this half
		half& operator*=(half rhs);

		/// Arithmetic assignment.
		/// This operation uses the underlying single-precision implementation.
		/// \tparam E type of concrete half expression
		/// \param rhs half to divide by
		/// \return reference to this half
		half& operator/=(half rhs);

		/// Arithmetic assignment.
		/// This operation uses the underlying single-precision implementation.
		/// \param rhs single-precision value to add
		/// \return reference to this half
		half& operator+=(float rhs);

		/// Arithmetic assignment.
		/// This operation uses the underlying single-precision implementation.
		/// \param rhs single-precision value to subtract
		/// \return reference to this half
		half& operator-=(float rhs);

		/// Arithmetic assignment.
		/// This operation uses the underlying single-precision implementation.
		/// \param rhs single-precision value to multiply with
		/// \return reference to this half
		half& operator*=(float rhs);

		/// Arithmetic assignment.
		/// This operation uses the underlying single-precision implementation.
		/// \param rhs single-precision value to divide by
		/// \return reference to this half
		half& operator/=(float rhs);

		/// \}
		/// \name Increment and decrement
		/// \{

		/// Prefix increment.
		/// This operation uses the underlying single-precision implementation.
		/// \return incremented half value
		half& operator++();

		/// Prefix decrement.
		/// This operation uses the underlying single-precision implementation.
		/// \return decremented half value
		half& operator--();

		/// Postfix increment.
		/// This operation uses the underlying single-precision implementation.
		/// \return non-incremented half value
		half operator++(int);

		/// Postfix decrement.
		/// This operation uses the underlying single-precision implementation.
		/// \return non-decremented half value
		half operator--(int);
		/// \}

	private:
		/// Constructor.
		/// \param bits binary representation to set half to
		half(std::uint16_t bits, bool);

		/// Internal binary representation
		std::uint16_t data_;
	};


	/// \name Comparison operators
	/// \{

	/// Comparison for equality.
	/// \param a first operand
	/// \param b second operand
	/// \retval true if operands equal
	/// \retval false else
	bool operator==(half a, half b);

	/// Comparison for inequality.
	/// \param a first operand
	/// \param b second operand
	/// \retval true if operands not equal
	/// \retval false else
	bool operator!=(half a, half b);

	/// Comparison for less than.
	/// \param a first operand
	/// \param b second operand
	/// \retval true if \a a less than \a b
	/// \retval false else
	bool operator<(half a, half b);

	/// Comparison for greater than.
	/// \param a first operand
	/// \param b second operand
	/// \retval true if \a a greater than \a b
	/// \retval false else
	bool operator>(half a, half b);

	/// Comparison for less equal.
	/// \param a first operand
	/// \param b second operand
	/// \retval true if \a a less equal \a b
	/// \retval false else
	bool operator<=(half a, half b);

	/// Comparison for greater equal.
	/// \param a first operand
	/// \param b second operand
	/// \retval true if \a a greater equal \a b
	/// \retval false else
	bool operator>=(half a, half b);

	/// \}
	/// \name Arithmetic operators
	/// \{

	/// Add halfs.
	/// This operation uses the underlying single-precision implementation.
	/// \param a left operand
	/// \param b right operand
	/// \return sum of halfs
	half operator+(half a, half b);

	/// Subtract halfs.
	/// This operation uses the underlying single-precision implementation.
	/// \param a left operand
	/// \param b right operand
	/// \return difference of halfs
	half operator-(half a, half b);

	/// Multiply halfs.
	/// This operation uses the underlying single-precision implementation.
	/// \param a left operand
	/// \param b right operand
	/// \return product of halfs
	half operator*(half a, half b);

	/// Divide halfs.
	/// This operation uses the underlying single-precision implementation.
	/// \param a left operand
	/// \param b right operand
	/// \return quotient of halfs
	half operator/(half a, half b);

	/// Identity.
	/// \param h operand
	/// \return uncahnged operand
	half constexpr operator+(half h);

	/// Negation.
	/// \param h operand
	/// \return negated operand
	half constexpr operator-(half h);

	/// \}
	/// \name Input and output
	/// \{

	/// Output operator.
	/// \tparam charT character type
	/// \tparam traits character traits
	/// \param out output stream to write into
	/// \param h half to write
	/// \return reference to output stream
	template<typename charT,typename traits> std::basic_ostream<charT,traits>& operator<<(std::basic_ostream<charT,traits> &out, half h);

	/// Input operator.
	/// \tparam charT character type
	/// \tparam traits character traits
	/// \param in input stream to read from
	/// \param h half to read into
	/// \return reference to input stream
	template<typename charT,typename traits> std::basic_istream<charT,traits>& operator>>(std::basic_istream<charT,traits> &in, half &h);

	/// \}
	/// \name Basic mathematical operations
	/// \{

	/// Absolute value.
	/// \param arg operand
	/// \return absolute value of \a arg
	half abs(half arg);

	/// Absolute value.
	/// \param arg operand
	/// \return absolute value of \a arg
	half fabs(half arg);

	/// Remainder of division.
	/// This function uses the underlying single-precision implementation.
	/// \param x first operand
	/// \param y second operand
	/// \return remainder of floating point division.
	half fmod(half x, half y);

	/// Remainder of division.
	/// This function uses the underlying single-precision implementation and requires support for C++11 `<cmath>` functions.
	/// \param x first operand
	/// \param y second operand
	/// \return remainder of floating point division.
	half remainder(half x, half y);

	/// Remainder of division.
	/// This function uses the underlying single-precision implementation and requires support for C++11 `<cmath>` functions.
	/// \param x first operand
	/// \param y second operand
	/// \param quo address to store some bits of quotient at
	/// \return remainder of floating point division.
	half remquo(half x, half y, int *quo);

	/// Fused multiply add.
	/// This function uses the underlying single-precision implementation and requires support for C++11 `<cmath>` functions.
	/// \param x first operand
	/// \param y second operand
	/// \param z third operand
	/// \return ( \a x * \a y ) + \a z rounded as one operation.
	half fma(half x, half y, half z);

	/// Minimum of halfs.
	/// \param x first operand
	/// \param y second operand
	/// \return minimum of operands
	half fmin(half x, half y);

	/// Maximum of halfs.
	/// \param x first operand
	/// \param y second operand
	/// \return maximum of operands
	half fmax(half x, half y);

	/// Positive difference.
	/// This function uses the underlying single-precision implementation and requires support for C++11 `<cmath>` functions.
	/// \param x first operand
	/// \param y second operand
	/// \return \a x - \a y or 0 if difference negative
	half fdim(half x, half y);

	/// Get NaN value.
	/// \return quiet NaN
	half nanh(const char*);

	/// \}
	/// \name Exponential functions
	/// \{

	/// Exponential function.
	/// This function uses the underlying single-precision implementation.
	/// \param arg operand
	/// \return e raised to \a arg
	half exp(half arg);

	/// Binary exponential.
	/// This function uses the underlying single-precision implementation and requires support for C++11 `<cmath>` functions.
	/// \param arg operand
	/// \return 2 raised to \a arg
	half exp2(half arg);

	/// Exponential minus one.
	/// This function uses the underlying single-precision implementation and requires support for C++11 `<cmath>` functions.
	/// \param arg operand
	/// \return e raised to \a arg subtracted by 1
	half expm1(half arg);

	/// Natural logorithm.
	/// This function uses the underlying single-precision implementation.
	/// \param arg operand
	/// \return logarithm of \a arg to base e
	half log(half arg);

	/// Common logorithm.
	/// This function uses the underlying single-precision implementation.
	/// \param arg operand
	/// \return logarithm of \a arg to base 10
	half log10(half arg);

	/// Natural logorithm.
	/// This function uses the underlying single-precision implementation and requires support for C++11 `<cmath>` functions.
	/// \param arg operand
	/// \return logarithm of \a arg plus 1 to base e
	half log1p(half arg);

	/// Binary logorithm.
	/// This function uses the underlying single-precision implementation and requires support for C++11 `<cmath>` functions.
	/// \param arg operand
	/// \return logarithm of \a arg to base 2
	half log2(half arg);

	/// \}
	/// \name Power functions
	/// \{

	/// Square root.
	/// This function uses the underlying single-precision implementation.
	/// \param arg operand
	/// \return square root of \a arg
	half sqrt(half arg);

	/// Cubic root.
	/// This function uses the underlying single-precision implementation and requires support for C++11 `<cmath>` functions.
	/// \param arg operand
	/// \return cubic root of \a arg
	half cbrt(half arg);

	/// Hypotenuse function.
	/// This function uses the underlying single-precision implementation and requires support for C++11 `<cmath>` functions.
	/// \param x first operand
	/// \param y second operand
	/// \return square root of sum of squares
	half hypot(half x, half y);

	/// Power function.
	/// This function uses the underlying single-precision implementation.
	/// \param base first operand
	/// \param exp second operand
	/// \return \a base raised to \a exp
	half pow(half base, half exp);

	/// \}
	/// \name Trigonometric functions
	/// \{

	/// Sine function.
	/// This function uses the underlying single-precision implementation.
	/// \param arg operand
	/// \return sine value of \a arg
	half sin(half arg);

	/// Cosine function.
	/// This function uses the underlying single-precision implementation.
	/// \param arg operand
	/// \return cosine value of \a arg
	half cos(half arg);

	/// Tangent function.
	/// This function uses the underlying single-precision implementation.
	/// \param arg operand
	/// \return tangent value of \a arg
	half tan(half arg);

	/// Arc sine.
	/// This function uses the underlying single-precision implementation.
	/// \param arg operand
	/// \return arc sine value of \a arg
	half asin(half arg);

	/// Arc cosine function.
	/// This function uses the underlying single-precision implementation.
	/// \param arg operand
	/// \return arc cosine value of \a arg
	half acos(half arg);

	/// Arc tangent function.
	/// This function uses the underlying single-precision implementation.
	/// \param arg operand
	/// \return arc tangent value of \a arg
	half atan(half arg);

	/// Arc tangent function.
	/// This function uses the underlying single-precision implementation.
	/// \param x first operand
	/// \param y second operand
	/// \return arc tangent value
	half atan2(half x, half y);

	/// \}
	/// \name Hyperbolic functions
	/// \{

	/// Hyperbolic sine.
	/// This function uses the underlying single-precision implementation.
	/// \param arg operand
	/// \return hyperbolic sine value of \a arg
	half sinh(half arg);

	/// Hyperbolic cosine.
	/// This function uses the underlying single-precision implementation.
	/// \param arg operand
	/// \return hyperbolic cosine value of \a arg
	half cosh(half arg);

	/// Hyperbolic tangent.
	/// This function uses the underlying single-precision implementation.
	/// \param arg operand
	/// \return hyperbolic tangent value of \a arg
	half tanh(half arg);

	/// Hyperbolic arc sine.
	/// This function uses the underlying single-precision implementation and requires support for C++11 `<cmath>` functions.
	/// \param arg operand
	/// \return hyperbolic arc sine value of \a arg
	half asinh(half arg);

	/// Hyperbolic arc cosine.
	/// This function uses the underlying single-precision implementation and requires support for C++11 `<cmath>` functions.
	/// \param arg operand
	/// \return hyperbolic arc cosine value of \a arg
	half acosh(half arg);

	/// Hyperbolic arc tangent.
	/// This function uses the underlying single-precision implementation and requires support for C++11 `<cmath>` functions.
	/// \param arg operand
	/// \return hyperbolic arc tangent value of \a arg
	half atanh(half arg);

	/// \}
	/// \name Error and gamma functions
	/// \{

	/// Error function.
	/// This function uses the underlying single-precision implementation and requires support for C++11 `<cmath>` functions.
	/// \param arg operand
	/// \return error function value of \a arg
	half erf(half arg);

	/// Complementary error function.
	/// This function uses the underlying single-precision implementation and requires support for C++11 `<cmath>` functions.
	/// \param arg operand
	/// \return 1 minus error function value of \a arg
	half erfc(half arg);

	/// Natural logarithm of gamma function.
	/// This function uses the underlying single-precision implementation and requires support for C++11 `<cmath>` functions.
	/// \param arg operand
	/// \return natural logarith of gamma function for \a arg
	half lgamma(half arg);

	/// Gamma function.
	/// This function uses the underlying single-precision implementation and requires support for C++11 `<cmath>` functions.
	/// \param arg operand
	/// \return gamma function value of \a arg
	half tgamma(half arg);

	/// \}
	/// \name Rounding
	/// \{

	/// Nearest integer not less than half value.
	/// \param arg half to round
	/// \return nearest integer not less than \a arg
	half ceil(half arg);

	/// Nearest integer not greater than half value.
	/// \param arg half to round
	/// \return nearest integer not greater than \a arg
	half floor(half arg);

	/// Nearest integer not greater in magnitude than half value.
	/// \param arg half to round
	/// \return nearest integer not greater in magnitude than \a arg
	half trunc(half arg);

	/// Nearest integer.
	/// \param arg half to round
	/// \return nearest integer, rounded away from zero in half-way cases
	half round(half arg);

	/// Nearest integer.
	/// \param arg half to round
	/// \return nearest integer, rounded away from zero in half-way cases
	long lround(half arg);

	/// Nearest integer.
	/// This function requires support for C++11 `long long`.
	/// \param arg half to round
	/// \return nearest integer, rounded away from zero in half-way cases
	long long llround(half arg);

	/// Nearest integer.
	/// This function uses the underlying single-precision implementation and requires support for C++11 `<cmath>` functions.
	/// \tparam E type of half expression
	/// \param arg half expression to round
	/// \return nearest integer using current single-precision rounding mode
	half nearbyint(half arg);

	/// Nearest integer.
	/// This function uses the underlying single-precision implementation and requires support for C++11 `<cmath>` functions.
	/// \tparam E type of half expression
	/// \param arg half expression to round
	/// \return nearest integer using current single-precision rounding mode
	half rint(half arg);

	/// Nearest integer.
	/// This function uses the underlying single-precision implementation and requires support for C++11 `<cmath>` functions.
	/// \tparam E type of half expression
	/// \param arg half expression to round
	/// \return nearest integer using current single-precision rounding mode
	long lrint(half arg);

	/// Nearest integer.
	/// This function uses the underlying single-precision implementation and requires support for C++11 `<cmath>` functions.
	/// \tparam E type of half expression
	/// \param arg half expression to round
	/// \return nearest integer using current single-precision rounding mode
	long long llrint(half arg);

	/// \}
	/// \name Floating point manipulation
	/// \{

	/// Decompress floating point number.
	/// \param arg number to decompress
	/// \param exp address to store exponent at
	/// \return significant in range [0.5, 1)
	half frexp(half arg, int *exp);

	/// Multiply by power of two.
	/// \param arg number to modify
	/// \param exp power of two to multiply with
	/// \return \a arg multplied by 2 raised to \a exp
	half ldexp(half arg, int exp);

	/// Extract integer and fractional parts.
	/// \param x number to decompress
	/// \param iptr address to store integer part at
	/// \return fractional part
	half modf(half x, half *iptr);

	/// Multiply by power of two.
	/// \param x number to modify
	/// \param exp power of two to multiply with
	/// \return \a arg multplied by 2 raised to \a exp
	half scalbn(half x, int exp);

	/// Multiply by power of two.
	/// \param x number to modify
	/// \param exp power of two to multiply with
	/// \return \a arg multplied by 2 raised to \a exp	
	half scalbln(half x, long exp);

	/// Extract exponent.
	/// \param arg number to query
	/// \return floating point exponent
	/// \retval FP_ILOGB0 for zero
	/// \retval FP_ILOGBNAN for NaN
	/// \retval MAX_INT for infinity
	int ilogb(half arg);

	/// Extract exponent.
	/// \param arg number to query
	/// \return floating point exponent
	half logb(half arg);

	/// Next representable value.
	/// \param from value to compute next representable value for
	/// \param to direction towards which to compute next value
	/// \return next representable value after \a from in direction towards \a to
	half nextafter(half from, half to);

	/// Next representable value.
	/// \param from value to compute next representable value for
	/// \param to direction towards which to compute next value
	/// \return next representable value after \a from in direction towards \a to
	half nexttoward(half from, long double to);

	/// Take sign.
	/// \param x value to change sign for
	/// \param y value to take sign from
	/// \return value equal to \a x in magnitude and to \a y in sign
	half copysign(half x, half y);

	/// \}
	/// \name Floating point classification
	/// \{

	/// Classify floating point value.
	/// \param arg number to classify
	/// \retval FP_ZERO for positive and negative zero
	/// \retval FP_SUBNORMAL for subnormal numbers
	/// \retval FP_INFINITY for positive and negative infinity
	/// \retval FP_NAN for NaNs
	/// \retval FP_NORMAL for all other (normal) values
	int fpclassify(half arg);

	/// Check if finite number.
	/// \param arg number to check
	/// \retval true if neither infinity nor NaN
	/// \retval false else
	bool isfinite(half arg);

	/// Check for infinity.
	/// \param arg number to check
	/// \retval true for positive or negative infinity
	/// \retval false else
	bool isinf(half arg);

	/// Check for NaN.
	/// \param arg number to check
	/// \retval true for NaNs
	/// \retval false else
	bool isnan(half arg);

	/// Check if normal number.
	/// \param arg number to check
	/// \retval true if normal number
	/// \retval false if either subnormal, zero, infinity or NaN
	bool isnormal(half arg);

	/// Check sign.
	/// \param arg number to check
	/// \retval true for negative number
	/// \retval false for positive number
	bool signbit(half arg);

	/// \}
	/// \name Comparison
	/// \{

	/// Comparison for greater than.
	/// \param x first operand
	/// \param y second operand
	/// \retval true if \a x greater than \a y
	/// \retval false else
	bool isgreater(half x, half y);

	/// Comparison for greater equal.
	/// \param x first operand
	/// \param y second operand
	/// \retval true if \a x greater equal \a y
	/// \retval false else
	bool isgreaterequal(half x, half y);

	/// Comparison for less than.
	/// \param x first operand
	/// \param y second operand
	/// \retval true if \a x less than \a y
	/// \retval false else
	bool isless(half x, half y);

	/// Comparison for less equal.
	/// \param x first operand
	/// \param y second operand
	/// \retval true if \a x less equal \a y
	/// \retval false else
	bool islessequal(half x, half y);

	/// Comarison for less or greater.
	/// \param x first operand
	/// \param y second operand
	/// \retval true if either less or greater
	/// \retval false else
	bool islessgreater(half x, half y);

	/// Check if unordered.
	/// \param x first operand
	/// \param y second operand
	/// \retval true if unordered (one or two NaN operands)
	/// \retval false else
	bool isunordered(half x, half y);

	/// \}
	/// \name Casting
	/// \{

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
	template<typename T,typename U> T half_cast(const U &arg);

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
	template<typename T,std::float_round_style R,typename U> T half_cast(const U &arg);
	/// \}


	/// Library-defined half-precision literals.
	/// Import this namespace to enable half-precision floating point literals (of course only if C++11 user-defined literals 
	/// are supported and enabled):
	/// ~~~~{.cpp}
	/// using namespace half_float::literal;
	/// half_float::half = 4.2_h;
	/// ~~~~
	namespace literal
	{
		/// Half literal.
		/// \param d literal value
		/// \return half with given value (if representable)
		half operator "" _h(long double d);
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
		static constexpr bool is_signed = true;

		/// Is not exact.
		static constexpr bool is_exact = false;

		/// Doesn't provide modulo arithmetic.
		static constexpr bool is_modulo = false;

		/// IEEE conformant.
		static constexpr bool is_iec559 = true;

		/// Supports infinity.
		static constexpr bool has_infinity = true;

		/// Supports quiet NaNs.
		static constexpr bool has_quiet_NaN = true;

		/// Supports subnormal values.
		static constexpr std::float_denorm_style has_denorm = std::denorm_present;

		/// Rounding mode.
		/// Due to the mix of internal single-precision computations (using the rounding mode of the underlying 
		/// single-precision implementation) with explicit truncation of the single-to-half conversions, the actual rounding 
		/// mode is indeterminate.
		static constexpr std::float_round_style round_style = std::round_indeterminate;

		/// Significant digits.
		static constexpr int digits = 11;

		/// Significant decimal digits.
		static constexpr int digits10 = 3;

		/// Required decimal digits to represent all possible values.
		static constexpr int max_digits10 = 5;

		/// Number base.
		static constexpr int radix = 2;

		/// One more than smallest exponent.
		static constexpr int min_exponent = -13;

		/// Smallest normalized representable power of 10.
		static constexpr int min_exponent10 = -4;

		/// One more than largest exponent
		static constexpr int max_exponent = 16;

		/// Largest finitely representable power of 10.
		static constexpr int max_exponent10 = 4;

		/// Smallest positive normal value.
		static constexpr half_float::half min();

		/// Smallest finite value.
		static constexpr half_float::half lowest();

		/// Largest finite value.
		static constexpr half_float::half max();

		/// Difference between one and next representable value.
		static constexpr half_float::half epsilon();

		/// Maximum rounding error.
		static constexpr half_float::half round_error();

		/// Positive infinity.
		static constexpr half_float::half infinity();

		/// Quiet NaN.
		static constexpr half_float::half quiet_NaN();

		/// Signalling NaN.
		static constexpr half_float::half signaling_NaN();

		/// Smallest positive subnormal value.
		static constexpr half_float::half denorm_min();
	};

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
		std::size_t operator()(half_float::half arg) const;
	};
}

#endif
