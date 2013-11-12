// test - Test application for half-precision floating point functionality.
//
// Copyright (c) 2012-2013 Christian Rau <rauy@users.sourceforge.net>
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

#define HALF_ROUND_STYLE -1
#define HALF_ROUND_TIES_TO_EVEN 0
#include <half.hpp>

#include <utility>
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <iomanip>
#include <memory>
#include <algorithm>
#include <iterator>
#include <functional>
#include <fstream>
#include <random>
#include <bitset>
#include <limits>
#include <typeinfo>
#include <cstdint>
#include <cmath>
#if HALF_ENABLE_CPP11_HASH
	#include <unordered_map>
#endif


using half_float::half;
using half_float::half_cast;

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
	return (isnan(a) && isnan(b)) || a == b;
}


class half_test
{
public:
	half_test(std::ostream &log)
		: tests_(0), log_(log)
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
		for(; u<0x7E00; ++u)
			batch.push_back(b2h(u));
		halfs_.insert(std::make_pair("positive sNaN", std::move(batch)));
		batch.clear();
		for(; u<0x8000; ++u)
			batch.push_back(b2h(u));
		halfs_.insert(std::make_pair("positive qNaN", std::move(batch)));
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
		for(; u<0xFE00; ++u)
			batch.push_back(b2h(u));
		halfs_.insert(std::make_pair("negative sNaN", std::move(batch)));
		batch.clear();
		for(; u!=0; ++u)
			batch.push_back(b2h(u));
		halfs_.insert(std::make_pair("negative qNaN", std::move(batch)));

		//set classes
		classes_["positive zero"] = FP_ZERO;
		classes_["positive subn"] = FP_SUBNORMAL;
		classes_["positive norm"] = FP_NORMAL;
		classes_["positive inft"] = FP_INFINITE;
		classes_["positive sNaN"] = FP_NAN;
		classes_["positive qNaN"] = FP_NAN;
		classes_["negative zero"] = FP_ZERO;
		classes_["negative subn"] = FP_SUBNORMAL;
		classes_["negative norm"] = FP_NORMAL;
		classes_["negative inft"] = FP_INFINITE;
		classes_["negative sNaN"] = FP_NAN;
		classes_["negative qNaN"] = FP_NAN;

		//write halfs
		std::ofstream out("halfs.log");
		for(auto iterB=halfs_.begin(); iterB!=halfs_.end(); ++iterB)
		{
			out << iterB->first << ":\n";
			for(auto iterH=iterB->second.begin(); iterH!=iterB->second.end(); ++iterH)
				out << '\t' << *iterH << '\n';
		}
	}

	unsigned int test()
	{
		//test size
		simple_test("size", []() { return sizeof(half)*CHAR_BIT >= 16; });

		//test conversion
		unary_test("conversion", [](half arg) { return comp(static_cast<half>(static_cast<float>(arg)), arg); });

		//test classification
		class_test("fpclassify", [](half arg, int cls) { return fpclassify(arg) == cls; });
		class_test("isfinite", [](half arg, int cls) { return isfinite(arg) == (cls!=FP_INFINITE&&cls!=FP_NAN); });
		class_test("isinf", [](half arg, int cls) { return isinf(arg) == (cls==FP_INFINITE); });
		class_test("isnan", [](half arg, int cls) { return isnan(arg) == (cls==FP_NAN); });
		class_test("isnormal", [](half arg, int cls) { return isnormal(arg) == (cls==FP_NORMAL); });
		unary_test("signbit", [](half arg) -> bool { float f = arg; return isnan(arg) || f==0.0f || (signbit(arg)==(f<0.0f)); });

		//test operators
		unary_test("prefix increment", [](half arg) -> bool { float f = static_cast<float>(arg); 
			return comp(static_cast<half>(++f), ++arg) && comp(static_cast<half>(f), arg); });
		unary_test("prefix decrement", [](half arg) -> bool { float f = static_cast<float>(arg); 
			return comp(static_cast<half>(--f), --arg) && comp(static_cast<half>(f), arg); });
		unary_test("postfix increment", [](half arg) -> bool { float f = static_cast<float>(arg); 
			return comp(static_cast<half>(f++), arg++) && comp(static_cast<half>(f), arg); });
		unary_test("postfix decrement", [](half arg) -> bool { float f = static_cast<float>(arg); 
			return comp(static_cast<half>(f--), arg--) && comp(static_cast<half>(f), arg); });
		unary_test("unary plus", [](half arg) { return comp(+arg, arg); });
		unary_test("unary minus", [](half arg) { return comp(-arg, static_cast<half>(-static_cast<float>(arg))); });
		binary_test("addition", [](half a, half b) { return comp(a+b, static_cast<half>(static_cast<float>(a)+static_cast<float>(b))); });
		binary_test("subtraction", [](half a, half b) { return comp(a-b, static_cast<half>(static_cast<float>(a)-static_cast<float>(b))); });
		binary_test("multiplication", [](half a, half b) { return comp(a*b, static_cast<half>(static_cast<float>(a)*static_cast<float>(b))); });
		binary_test("division", [](half a, half b) { return comp(a/b, static_cast<half>(static_cast<float>(a)/static_cast<float>(b))); });
		binary_test("equal", [](half a, half b) { return (a==b) == (static_cast<float>(a)==static_cast<float>(b)); });
		binary_test("not equal", [](half a, half b) { return (a!=b) == (static_cast<float>(a)!=static_cast<float>(b)); });
		binary_test("less", [](half a, half b) { return (a<b) == (static_cast<float>(a)<static_cast<float>(b)); });
		binary_test("greater", [](half a, half b) { return (a>b) == (static_cast<float>(a)>static_cast<float>(b)); });
		binary_test("less equal", [](half a, half b) { return (a<=b) == (static_cast<float>(a)<=static_cast<float>(b)); });
		binary_test("greater equal", [](half a, half b) { return (a>=b) == (static_cast<float>(a)>=static_cast<float>(b)); });

		//test basic functions
		unary_test("abs", [](half arg) { return comp(abs(arg), static_cast<half>(std::abs(static_cast<float>(arg)))); });
		unary_test("fabs", [](half arg) { return comp(abs(arg), static_cast<half>(std::fabs(static_cast<float>(arg)))); });
		binary_test("fmod", [](half a, half b) { return comp(fmod(a, b), 
			static_cast<half>(std::fmod(static_cast<float>(a), static_cast<float>(b)))); });
		binary_test("fdim", [](half a, half b) -> bool { half c = fdim(a, b); return isnan(a) || isnan(b) || 
			(isinf(a) && isinf(b) && signbit(a)==signbit(b)) || ((a>b) && comp(c, a-b)) || ((a<=b) && comp(c, static_cast<half>(0.0f))); });

		//test exponential functions
		unary_test("exp", [](half arg) { return comp(exp(arg), static_cast<half>(std::exp(static_cast<float>(arg)))); });
		unary_test("log", [](half arg) { return comp(log(arg), static_cast<half>(std::log(static_cast<float>(arg)))); });
		unary_test("log10", [](half arg) { return comp(log10(arg), static_cast<half>(std::log10(static_cast<float>(arg)))); });

		//test power functions
		unary_test("sqrt", [](half arg) { return comp(sqrt(arg), static_cast<half>(std::sqrt(static_cast<float>(arg)))); });
		binary_test("pow", [](half a, half b) { return comp(pow(a, b), 
			static_cast<half>(std::pow(static_cast<float>(a), static_cast<float>(b)))); });

		//test trig functions
		unary_test("sin", [](half arg) { return comp(sin(arg), static_cast<half>(std::sin(static_cast<float>(arg)))); });
		unary_test("cos", [](half arg) { return comp(cos(arg), static_cast<half>(std::cos(static_cast<float>(arg)))); });
		unary_test("tan", [](half arg) { return comp(tan(arg), static_cast<half>(std::tan(static_cast<float>(arg)))); });
		unary_test("asin", [](half arg) { return comp(asin(arg), static_cast<half>(std::asin(static_cast<float>(arg)))); });
		unary_test("acos", [](half arg) { return comp(acos(arg), static_cast<half>(std::acos(static_cast<float>(arg)))); });
		unary_test("atan", [](half arg) { return comp(atan(arg), static_cast<half>(std::atan(static_cast<float>(arg)))); });
		binary_test("atan2", [](half a, half b) { return comp(atan2(a, b), 
			static_cast<half>(std::atan2(static_cast<float>(a), static_cast<float>(b)))); });

		//test hyp functions
		unary_test("sinh", [](half arg) { return comp(sinh(arg), static_cast<half>(std::sinh(static_cast<float>(arg)))); });
		unary_test("cosh", [](half arg) { return comp(cosh(arg), static_cast<half>(std::cosh(static_cast<float>(arg)))); });
		unary_test("tanh", [](half arg) { return comp(tanh(arg), static_cast<half>(std::tanh(static_cast<float>(arg)))); });

		//test round functions
		unary_test("ceil", [](half arg) { return comp(ceil(arg), static_cast<half>(std::ceil(static_cast<float>(arg)))); });
		unary_test("floor", [](half arg) { return comp(floor(arg), static_cast<half>(std::floor(static_cast<float>(arg)))); });
		unary_test("trunc", [](half arg) { return !isfinite(arg) || comp(trunc(arg), static_cast<half>(static_cast<int>(arg))); });
		unary_test("round", [](half arg) { return !isfinite(arg) || comp(round(arg), 
			static_cast<half>(static_cast<int>(static_cast<float>(arg)+(signbit(arg) ? -0.5f : 0.5f)))); });
		unary_test("lround", [](half arg) { return !isfinite(arg) || lround(arg) == 
			static_cast<long>(static_cast<float>(arg)+(signbit(arg) ? -0.5f : 0.5f)); });
		unary_test("nearbyint", [](half arg) { return !isfinite(arg) || comp(nearbyint(arg), static_cast<half>(half_cast<int>(arg))); });
		unary_test("rint", [](half arg) { return !isfinite(arg) || comp(rint(arg), static_cast<half>(half_cast<int>(arg))); });
		unary_test("lrint", [](half arg) { return !isfinite(arg) || lrint(arg) == half_cast<long>(arg); });
#if HALF_ENABLE_CPP11_LONG_LONG
		unary_test("llround", [](half arg) { return !isfinite(arg) || llround(arg) == 
			static_cast<long long>(static_cast<float>(arg)+(signbit(arg) ? -0.5f : 0.5f)); });
		unary_test("llrint", [](half arg) { return !isfinite(arg) || llrint(arg) == half_cast<long long>(arg); });
#endif

		//test float functions
		unary_test("frexp", [](half arg) -> bool { int eh, ef; bool eq = comp(frexp(arg, &eh), 
			static_cast<half>(std::frexp(static_cast<float>(arg), &ef))); return eq && eh==ef; });
		unary_test("ldexp", [](half arg) -> bool { unsigned int passed = 0; for(int i=-50; i<50; ++i) passed += 
			comp(ldexp(arg, i), static_cast<half>(std::ldexp(static_cast<float>(arg), i))); return passed==100; });
		unary_test("modf", [](half arg) -> bool { half h; float f; bool eq = comp(modf(arg, &h), static_cast<half>(
			std::modf(static_cast<float>(arg), &f))); return eq && comp(h, static_cast<half>(f)); });
		binary_test("nextafter", [](half a, half b) -> bool { half c = nextafter(a, b); std::int16_t d = std::abs(
			static_cast<std::int16_t>(h2b(a)-h2b(c))); return ((isnan(a) || isnan(b)) && isnan(c)) || 
			(comp(a, b) && comp(b, c)) || ((d==1||d==0x7FFF) && (a<b)==(a<c)); });
		binary_test("copysign", [](half a, half b) -> bool { half h = copysign(a, b); 
			return comp(abs(h), abs(a)) && signbit(h)==signbit(b); });

#if HALF_ENABLE_CPP11_CMATH
		//test basic functions
		binary_test("remainder", [](half a, half b) { return comp(remainder(a, b), 
			static_cast<half>(std::remainder(static_cast<float>(a), static_cast<float>(b)))); });
		binary_test("remquo", [](half a, half b) -> bool { int qh = 0, qf = 0; bool eq = comp(remquo(a, b, &qh), 
			static_cast<half>(std::remquo(static_cast<float>(a), static_cast<float>(b), &qf))); return eq && (qh&7)==(qf&7); });
		binary_test("fmin", [](half a, half b) -> bool { half c = fmin(a, b); return ((isnan(a) || isnan(b)) && isnan(c)) || 
			comp(c, static_cast<half>(std::fmin(static_cast<float>(a), static_cast<float>(b)))); });
		binary_test("fmax", [](half a, half b) -> bool { half c = fmax(a, b); return ((isnan(a) || isnan(b)) && isnan(c)) || 
			comp(c, static_cast<half>(std::fmax(static_cast<float>(a), static_cast<float>(b)))); });
		binary_test("fdim", [](half a, half b) { return comp(fdim(a, b), static_cast<half>(
			std::fdim(static_cast<float>(a), static_cast<float>(b)))); });

		//test exponential functions
		unary_test("exp2", [](half arg) { return comp(exp2(arg), static_cast<half>(std::exp2(static_cast<float>(arg)))); });
		unary_test("expm1", [](half arg) { return comp(expm1(arg), static_cast<half>(std::expm1(static_cast<float>(arg)))); });
		unary_test("log1p", [](half arg) { return comp(log1p(arg), static_cast<half>(std::log1p(static_cast<float>(arg)))); });
		unary_test("log2", [](half arg) { return comp(log2(arg), static_cast<half>(std::log2(static_cast<float>(arg)))); });

		//test power functions
		unary_test("cbrt", [](half arg) { return comp(cbrt(arg), static_cast<half>(std::cbrt(static_cast<float>(arg)))); });
		binary_test("hypot", [](half a, half b) { return comp(hypot(a, b), static_cast<half>(
			std::hypot(static_cast<float>(a), static_cast<float>(b)))); });

		//test hyp functions
		unary_test("asinh", [](half arg) { return comp(asinh(arg), static_cast<half>(std::asinh(static_cast<float>(arg)))); });
		unary_test("acosh", [](half arg) { return comp(acosh(arg), static_cast<half>(std::acosh(static_cast<float>(arg)))); });
		unary_test("atanh", [](half arg) { return comp(atanh(arg), static_cast<half>(std::atanh(static_cast<float>(arg)))); });

		//test err functions
		unary_test("erf", [](half arg) { return comp(erf(arg), static_cast<half>(std::erf(static_cast<float>(arg)))); });
		unary_test("erfc", [](half arg) { return comp(erfc(arg), static_cast<half>(std::erfc(static_cast<float>(arg)))); });
		unary_test("lgamma", [](half arg) { return comp(lgamma(arg), static_cast<half>(std::lgamma(static_cast<float>(arg)))); });
		unary_test("tgamma", [](half arg) { return comp(tgamma(arg), static_cast<half>(std::tgamma(static_cast<float>(arg)))); });

		//test round functions
		unary_test("trunc", [](half arg) { return comp(trunc(arg), static_cast<half>(std::trunc(static_cast<float>(arg)))); });
		unary_test("round", [](half arg) { return comp(round(arg), static_cast<half>(std::round(static_cast<float>(arg)))); });
		unary_test("lround", [](half arg) { return lround(arg) == std::lround(static_cast<float>(arg)); });
		unary_test("llround", [](half arg) { return llround(arg) == std::llround(static_cast<float>(arg)); });
	#if HALF_ROUND_STYLE == 1 && HALF_ROUND_TIES_TO_EVEN == 1
		unary_test("nearbyint", [](half arg) { return comp(nearbyint(arg), static_cast<half>(std::nearbyint(static_cast<float>(arg)))); });
		unary_test("rint", [](half arg) { return comp(rint(arg), static_cast<half>(std::rint(static_cast<float>(arg)))); });
		unary_test("lrint", [](half arg) { return lrint(arg) == std::lrint(static_cast<float>(arg)); });
		unary_test("llrint", [](half arg) { return llrint(arg) == std::llrint(static_cast<float>(arg)); });
	#endif

		//test float functions
		unary_test("scalbn", [](half arg) -> bool { unsigned int passed = 0; for(int i=-50; i<50; ++i) passed += 
			comp(scalbn(arg, i), static_cast<half>(std::scalbn(static_cast<float>(arg), i))); return passed==100; });
		unary_test("scalbln", [](half arg) -> bool { unsigned int passed = 0; for(long i=-50; i<50; ++i) passed += 
			comp(scalbln(arg, i), static_cast<half>(std::scalbln(static_cast<float>(arg), i))); return passed==100; });
		unary_test("ilogb", [](half arg) { return ilogb(arg) == std::ilogb(static_cast<float>(arg)); });
		unary_test("logb", [](half arg) { return comp(logb(arg), static_cast<half>(std::logb(static_cast<float>(arg)))); });
		binary_test("copysign", [](half a, half b) { return comp(copysign(a, b), 
			static_cast<half>(std::copysign(static_cast<float>(a), static_cast<float>(b)))); });

		//test classification functions
		unary_test("fpclassify", [](half arg) -> bool { int ch=fpclassify(arg), cf=std::fpclassify(
			static_cast<float>(arg)); return ch==cf || (ch==FP_SUBNORMAL && cf==FP_NORMAL); });
		unary_test("isfinite", [](half arg) { return isfinite(arg) == std::isfinite(static_cast<float>(arg)); });
		unary_test("isinf", [](half arg) { return isinf(arg) == std::isinf(static_cast<float>(arg)); });
		unary_test("isnan", [](half arg) { return isnan(arg) == std::isnan(static_cast<float>(arg)); });
		unary_test("isnormal", [](half arg) { return isnormal(arg) == std::isnormal(static_cast<float>(arg)) || 
			(!isnormal(arg) && fpclassify(arg)==FP_SUBNORMAL); });
		unary_test("signbit", [](half arg) { return signbit(arg) == std::signbit(static_cast<float>(arg)); });

		//test comparison functions
		binary_test("isgreater", [](half a, half b) { return isgreater(a, b) == 
			std::isgreater(static_cast<float>(a), static_cast<float>(b)); });
		binary_test("isgreaterequal", [](half a, half b) { return isgreaterequal(a, b) == 
			std::isgreaterequal(static_cast<float>(a), static_cast<float>(b)); });
		binary_test("isless", [](half a, half b) { return isless(a, b) == 
			std::isless(static_cast<float>(a), static_cast<float>(b)); });
		binary_test("islessequal", [](half a, half b) { return islessequal(a, b) == 
			std::islessequal(static_cast<float>(a), static_cast<float>(b)); });
		binary_test("islessgreater", [](half a, half b) { return islessgreater(a, b) == 
			std::islessgreater(static_cast<float>(a), static_cast<float>(b)); });
		binary_test("isunordered", [](half a, half b) { return isunordered(a, b) == 
			std::isunordered(static_cast<float>(a), static_cast<float>(b)); });
#endif

		//test rounding
		auto rand32 = std::bind(std::uniform_int_distribution<std::uint32_t>(0, std::numeric_limits<std::uint32_t>::max()), std::default_random_engine());
		simple_test("round_to_nearest", [&rand32]() -> bool { unsigned int passed = 0; for(unsigned int i=0; i<1e6; ++i) {
			std::uint32_t u=rand32(); float f = *reinterpret_cast<float*>(&u); half a = half_cast<half,std::round_indeterminate>(f), 
			b(nextafter(a, copysign(std::numeric_limits<half>::infinity(), a))), h = half_cast<half,std::round_to_nearest>(f);
			float af(a), bf(b), hf(h); passed += half_float::detail::builtin_isnan(f) ||
			(std::abs(hf)>std::abs(f)&&comp(h, b)&&((
			#if HALF_ROUND_TIES_TO_EVEN
				std::abs(f-af)>std::abs(bf-f) || (std::abs(f-af)==std::abs(bf-f)&&!(h2b(h)&1))
			#else
				std::abs(f-af)>=std::abs(bf-f)
			#endif
			)||isinf(h))) ||
			(std::abs(hf)<=std::abs(f)&&comp(h, a)&&((
			#if HALF_ROUND_TIES_TO_EVEN
				std::abs(f-af)<std::abs(bf-f) || (std::abs(f-af)==std::abs(bf-f)&&!(h2b(h)&1))
			#else
				std::abs(f-af)<std::abs(bf-f)
			#endif
			)||isinf(h))); } return passed == 1e6; });
		simple_test("round_toward_zero", [&rand32]() -> bool { unsigned int passed = 0; for(unsigned int i=0; i<1e6; ++i) {
			std::uint32_t u=rand32(); float f = *reinterpret_cast<float*>(&u); half a = half_cast<half,std::round_indeterminate>(f), 
			h = half_cast<half,std::round_toward_zero>(f); float af(a), hf(h); 
			passed += half_float::detail::builtin_isnan(f) || isinf(a) || af == hf; } return passed == 1e6; });
		simple_test("round_toward_infinity", [&rand32]() -> bool { unsigned int passed = 0; for(unsigned int i=0; i<1e6; ++i) {
			std::uint32_t u=rand32(); float f = *reinterpret_cast<float*>(&u); half a = half_cast<half,std::round_toward_zero>(f), 
			b(nextafter(a, copysign(std::numeric_limits<half>::infinity(), a))), h = half_cast<half,std::round_toward_infinity>(f);
			float hf(h); passed += half_float::detail::builtin_isnan(f) || (comp(h, a)&&(signbit(h)||hf==f)) || 
			(comp(h, b)&&!signbit(h)&&hf>f); } return passed == 1e6; });
		simple_test("round_toward_neg_infinity", [&rand32]() -> bool { unsigned int passed = 0; for(unsigned int i=0; i<1e6; ++i) {
			std::uint32_t u=rand32(); float f = *reinterpret_cast<float*>(&u); half a = half_cast<half,std::round_toward_zero>(f), 
			b(nextafter(a, copysign(std::numeric_limits<half>::infinity(), a))), h = half_cast<half,std::round_toward_neg_infinity>(f);
			float hf(h); passed += half_float::detail::builtin_isnan(f) || (comp(h, a)&&(!signbit(h)||hf==f)) || 
			(comp(h, b)&&signbit(h)&&hf<f); } return passed == 1e6; });

		//test casting
		auto rand23 = std::bind(std::uniform_int_distribution<std::uint32_t>(0, (1<<23)-1), std::default_random_engine());
		unary_test("half_cast<float>", [](half arg) -> bool { float a = half_cast<float>(arg), b = static_cast<float>(arg); 
			return *reinterpret_cast<std::uint32_t*>(&a) == *reinterpret_cast<std::uint32_t*>(&b); });
		unary_test("half_cast<round_to_nearest>", [&rand23](half arg) -> bool { float f = half_cast<float>(arg); 
			std::uint32_t n=rand23(), m=1<<13; if(fpclassify(arg)==FP_SUBNORMAL) m <<= std::min(std::max(-ilogb(arg)-14, 0), 10);
			*reinterpret_cast<std::uint32_t*>(&f) |= n&(m-1)&-isfinite(arg); return fpclassify(arg)==FP_ZERO || 
			comp(half_cast<half,std::round_to_nearest>(f), 
			#if HALF_ROUND_TIES_TO_EVEN
				((n&(m>>1)) && ((n&((m>>1)-1)) || (h2b(arg)&1)))
			#else
				(n&(m>>1))
			#endif
			? nextafter(arg, copysign(std::numeric_limits<half>::infinity(), arg)) : arg); });
		unary_test("half_cast<round_toward_zero>", [&rand23](half arg) -> bool { float f = half_cast<float>(arg);
			std::uint32_t n=rand23(), m=1<<13; if(fpclassify(arg)==FP_SUBNORMAL) m <<= std::min(std::max(-ilogb(arg)-14, 0), 10);
			*reinterpret_cast<std::uint32_t*>(&f) |= n&(m-1)&-isfinite(arg); return comp(half_cast<half,std::round_toward_zero>(f), arg); });
		unary_test("half_cast<round_toward_infinity>", [&rand23](half arg) -> bool { float f = half_cast<float>(arg);
			std::uint32_t n=rand23(), m=1<<13; if(fpclassify(arg)==FP_SUBNORMAL) m <<= std::min(std::max(-ilogb(arg)-14, 0), 10);
			*reinterpret_cast<std::uint32_t*>(&f) |= n&(m-1)&-isfinite(arg); return comp(half_cast<half,std::round_toward_infinity>(f), 
			(!signbit(arg)&&(n&(m-1))) ? nextafter(arg, copysign(std::numeric_limits<half>::infinity(), arg)) : arg); });
		unary_test("half_cast<round_toward_neg_infinity>", [&rand23](half arg) -> bool { float f = half_cast<float>(arg);
			std::uint32_t n=rand23(), m=1<<13; if(fpclassify(arg)==FP_SUBNORMAL) m <<= std::min(std::max(-ilogb(arg)-14, 0), 10);
			*reinterpret_cast<std::uint32_t*>(&f) |= n&(m-1)&-isfinite(arg); return comp(half_cast<half,std::round_toward_neg_infinity>(f), 
			(signbit(arg)&&(n&(m-1))) ? nextafter(arg, copysign(std::numeric_limits<half>::infinity(), arg)) : arg); });

		//test numeric limits
		unary_test("numeric_limits::min", [](half arg) { return !isnormal(arg) || 
			signbit(arg) || arg>=std::numeric_limits<half>::min(); });
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

		if(failed_.empty())
			log_ << "ALL TESTS PASSED\n";
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

	template<typename F>
	bool class_test(const std::string &name, F test)
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
				log_ << (iterB->second.size()-passed) << " of " << iterB->second.size() << " failed\n";
		}
		log_ << '\n';
		++tests_;
		if(count == halfs_.size())
			return true;
		failed_.push_back(name);
		return false;
	}

	template<typename F>
	bool simple_test(const std::string &name, F test)
	{
		log_ << "testing " << name << ": ";
		bool passed = test();
		log_ << (passed ? "passed" : "failed") << "\n\n";
		++tests_;
		if(!passed)
			failed_.push_back(name);
		return passed;
	}

	template<typename F>
	bool unary_test(const std::string &name, F test)
	{
		unsigned int count = 0;
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
				log_ << (iterB->second.size()-passed) << " of " << iterB->second.size() << " failed\n";
		}
		log_ << '\n';
		++tests_;
		if(count == halfs_.size())
			return true;
		failed_.push_back(name);
		return false;
	}

	template<typename F>
	bool binary_test(const std::string &name, F test)
	{
		auto rand = std::bind(std::uniform_int_distribution<std::size_t>(0, 63), std::default_random_engine());
		unsigned int tests = 0, count = 0;
		log_ << "testing " << name << ": ";
		for(auto iterB1=halfs_.begin(); iterB1!=halfs_.end(); ++iterB1)
		{
			for(auto iterB2=halfs_.begin(); iterB2!=halfs_.end(); ++iterB2)
			{
				for(unsigned int i=std::min(rand(), iterB1->second.size()-1); i<iterB1->second.size(); i+=64)
				{
					for(unsigned int j=std::min(rand(), iterB2->second.size()-1); j<iterB2->second.size(); j+=64)
					{
						++tests;
						count += test(iterB1->second[i], iterB2->second[j]);
					}
				}
			}
		}
		bool passed = count == tests;
		if(passed)
			log_ << "all passed\n\n";
		else
		{
			log_ << (tests-count) << " of " << tests << " failed\n\n";
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
};
/*
#include <chrono>
struct timer
{
	timer() : start_(std::chrono::high_resolution_clock::now()) {}
	~timer() { std::cout << "time: " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()-start_).count() << " ms\n"; }

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> start_;
};
*/

int main(int argc, char *argv[])
{
/*	using namespace half_float::literal;
	half a, b;
	a = (std::numeric_limits<half>::max()*2.0_h) / 2.0_h;
	b = std::numeric_limits<half>::max() * 2.0_h;
	b /= 2.0_h;
	std::cout << std::hex << a << " - " << b << std::endl;
	a = (std::numeric_limits<half>::max()+1.0_h) - 1.0_h;
	b = std::numeric_limits<half>::max() + 1.0_h;
	b -= 1.0_h;
	std::cout << a << " - " << b << std::dec << std::endl;
*/
	std::cout << std::hash<half>()(std::numeric_limits<half>::infinity()) << std::endl;

	int i = rand();
	half h = half_cast<half>(i);
	unsigned int u = half_cast<unsigned int>(h);
	half a = half_cast<half>(h), b = half_cast<half>(h+a);

	half pi = half_cast<half,std::round_to_nearest>(3.1415926535897932384626433832795L);
	std::cout << "Pi: " << pi << " - 0x" << std::hex << std::setfill('0') << std::setw(4) << h2b(pi) << std::dec 
		<< " - " << std::bitset<16>(static_cast<unsigned long long>(h2b(pi))).to_string() << std::endl;
	half e = half_cast<half,std::round_to_nearest>(std::exp(1.0L));
	std::cout << "e:  " << e << " - 0x" << std::hex << std::setfill('0') << std::setw(4) << h2b(e) << std::dec 
		<< " - " << std::bitset<16>(static_cast<unsigned long long>(h2b(e))).to_string() << std::endl;

	std::cout << ilogb(sin(a+b)) << '\n';			//ADL test

	using namespace std;
//	using namespace half_float;
	auto f = atan2(h, 3);
	auto g = h + 3LL;
	std::cout << typeid(f).name() << ", " << typeid(g).name() << '\n';
	std::cout << exp2(half(1)) << ", " << log2(half(1)) << '\n';
	std::cout << logb(h) << ilogb(h) << '\n';

	std::unique_ptr<std::ostream> file;
	if(argc > 1)
		file.reset(new std::ofstream(argv[1]));
	half_test test((argc>1) ? *file : std::cout);

//	timer time;
	return test.test();
}
