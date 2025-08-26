.. meta::
  :description: Half documentation 
  :keywords: Half, APIs, ROCm, documentation


Converting and rounding Half
-----------------------------

Half is explicitly constructible/convertible from a single-precision float argument. Thus it is also explicitly constructible/convertible from any type implicitly convertible to float, but constructing it from types like double or 
int will involve the usual warnings arising when implicitly converting those to float because of the lost precision. On the one hand those warnings are intentional, because converting those types to half neccessarily also reduces 
precision. But on the other hand they are raised for explicit conversions from those types, when the user knows what he is doing. So if those warnings keep bugging you, then you won't get around first explicitly converting to float 
before converting to half, or use the 'half_cast' described below. In addition you can also directly assign float values to halfs.

In contrast to the float-to-half conversion, which reduces precision, the conversion from half to float (and thus to any other type implicitly convertible from float) is implicit, because all values represetable with 
half-precision are also representable with single-precision. This way the half-to-float conversion behaves similar to the builtin float-to-double conversion and all arithmetic expressions involving both half-precision and 
single-precision arguments will be of single-precision type. This way you can also directly use the mathematical functions of the C++ standard library, though in this case you will invoke the single-precision versions which will 
also return single-precision values, which is (even if maybe performing the exact same computation, see below) not as conceptually clean when working in a half-precision environment.

The default rounding mode for conversions from float to half uses truncation (round toward zero, but mapping overflows to infinity) for rounding values not representable exactly in half-precision. This is the fastest rounding possible 
and is usually sufficient. But by redefining the 'HALF_ROUND_STYLE' preprocessor symbol (before including half.hpp) this default can be overridden with one of the other standard rounding modes using their respective constants 
or the equivalent values of 'std::float_round_style' (it can even be synchronized with the underlying single-precision implementation by defining it to 'std::numeric_limits<float>::round_style'):

- 'std::round_indeterminate' or -1 for the fastest rounding (default).

- 'std::round_toward_zero' or 0 for rounding toward zero.

- std::round_to_nearest' or 1 for rounding to the nearest value.

- std::round_toward_infinity' or 2 for rounding toward positive infinity.

- std::round_toward_neg_infinity' or 3 for rounding toward negative infinity.

In addition to changing the overall default rounding mode one can also use the 'half_cast'. This converts between half and any built-in arithmetic type using a configurable rounding mode (or the default rounding mode if none is 
specified). In addition to a configurable rounding mode, 'half_cast' has another big difference to a mere 'static_cast': Any conversions are performed directly using the given rounding mode, without any intermediate conversion 
to/from 'float'. This is especially relevant for conversions to integer types, which don't necessarily truncate anymore. But also for conversions from 'double' or 'long double' this may produce more precise results than a 
pre-conversion to 'float' using the single-precision implementation's current rounding mode would.

.. code-block:: bash

    half a = half_cast<half>(4.2);
    half b = half_cast<half,std::numeric_limits<float>::round_style>(4.2f);
    assert( half_cast<int, std::round_to_nearest>( 0.7_h )     == 1 );
    assert( half_cast<half,std::round_toward_zero>( 4097 )     == 4096.0_h );
    assert( half_cast<half,std::round_toward_infinity>( 4097 ) == 4100.0_h );
    assert( half_cast<half,std::round_toward_infinity>( std::numeric_limits<double>::min() ) > 0.0_h );

When using round to nearest (either as default or through 'half_cast') ties are by default resolved by rounding them away from zero (and thus equal to the behaviour of the 'round' function). But by redefining the 
'HALF_ROUND_TIES_TO_EVEN' preprocessor symbol to 1 (before including half.hpp) this default can be changed to the slightly slower but less biased and more IEEE-conformant behaviour of rounding half-way cases to the nearest even value.

.. code-block:: bash  

    #define HALF_ROUND_TIES_TO_EVEN 1
    #include <half.hpp>
    ...
    assert( half_cast<int,std::round_to_nearest>(3.5_h) 
         == half_cast<int,std::round_to_nearest>(4.5_h) );
