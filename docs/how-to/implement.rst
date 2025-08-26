.. meta::
  :description: Half documentation 
  :keywords: Half, APIs, ROCm, documentation

Implement Half
------------------

For performance reasons (and ease of implementation) many of the mathematical functions provided by the library as well as all arithmetic operations are actually carried out in single-precision under the hood, calling to the C++ standard library implementations of those functions whenever appropriate, meaning the arguments are converted to floats and the result back to half. But to reduce the conversion overhead as much as possible any temporary values inside of lengthy expressions are kept in single-precision as long as possible, while still maintaining a strong half-precision type to the outside world. Only when finally assigning the value to a half or calling a function that works directly on halfs is the actual conversion done (or never, when further converting the result to float.

This approach has two implications. First of all you have to treat the library's documentation at http://half.sourceforge.net as a simplified version, describing the behaviour of the library as if implemented this way. The actual argument and return types of functions and operators may involve other internal types (feel free to generate the exact developer documentation from the Doxygen comments in the library's header file if you really need to). But nevertheless the behaviour is exactly like specified in the documentation. The other implication is, that in the presence of rounding errors or over-/underflows arithmetic expressions may produce different results when compared to converting to half-precision after each individual operation:

.. code-block:: bash

    half a = std::numeric_limits<half>::max() * 2.0_h / 2.0_h;       // a = MAX
    half b = half(std::numeric_limits<half>::max() * 2.0_h) / 2.0_h; // b = INF
    assert( a != b );

But this should only be a problem in very few cases. One last word has to be said when talking about performance. Even with its efforts in reducing conversion overhead as much as possible, the software half-precision 
implementation can most probably not beat the direct use of single-precision computations. Usually using actual float values for all computations and temproraries and using halfs only for storage is the recommended way. On the one hand this somehow makes the provided mathematical functions obsolete (especially in light of the implicit conversion from half to float), but nevertheless the goal of this library was to provide a complete and 
conceptually clean half-precision implementation, to which the standard mathematical functions belong, even if usually not needed.

IEEE conformance
-----------------

The half type uses the standard IEEE representation with 1 sign bit, 5 exponent bits and 10 mantissa bits (11 when counting the hidden bit). It supports all types of special values, like subnormal values, infinity and NaNs. But there are some limitations to the complete conformance to the IEEE 754 standard:

- The implementation does not differentiate between signalling and quiet NaNs, this means operations on halfs are not specified to trap on signalling NaNs (though they may, see last point).

- Though arithmetic operations are internally rounded to single-precision using the underlying single-precision implementation's current rounding mode, those values are then converted to half-precision using the default 
    half-precision rounding mode (changed by defining 'HALF_ROUND_STYLE' accordingly). This mixture of rounding modes is also the reason why 'std::numeric_limits<half>::round_style' may actually return 
    'std::round_indeterminate' when half- and single-precision rounding modes  don't match.

- Because of internal truncation it may also be that certain single-precision  NaNs will be wrongly converted to half-precision infinity, though this is unlikely to happen, since most single-precision implementations don't tend to only set the lowest bits of a NaN mantissa.

- The implementation does not provide any floating point exceptions, thus arithmetic operations or mathematical functions are not specified to invoke proper floating point exceptions. But due to many functions implemented in single-precision, those may still invoke floating point exceptions of the underlying single-precision implementation.

Some of those points could have been circumvented by controlling the floating point environment using <cfenv> or implementing a similar exception mechanism. But this would have required excessive runtime checks giving two high an impact on performance for something that is rarely ever needed. If you really need to rely on proper floating point exceptions, it is recommended to explicitly perform computations using the built-in floating point types to be on the safe side. In the same way, if you really need to rely on a particular rounding behaviour, it is recommended to either use single-precision computations and explicitly convert the result to half-precision using 'half_cast' and specifying the desired rounding mode, or synchronize the default half-precision rounding mode to the rounding mode of the single-precision implementation (most likely 'HALF_ROUND_STYLE=1', 'HALF_ROUND_TIES_TO_EVEN=1'). But this is really considered an expert-scenario that should be used only when necessary, since actually working with half-precision usually comes with a certain tolerance/ignorance of exactness considerations and proper rounding comes with a certain performance cost.
