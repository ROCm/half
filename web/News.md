News														{#news}
====

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
