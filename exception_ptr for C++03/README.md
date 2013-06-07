exception_ptr lite
==================

This code is part of the Blog post [exception_ptr lite for C++03](http://martin-moene.blogspot.com/2013/06/exceptionptr-for-c03.html)

`exception_ptr-lite.h` provides:

* `std11::exception_ptr`
* `std11::make_exception_ptr()`
* `std11::rethrow_exception()`

exception_ptr lite is created to propagate exceptions via Andrei Alexandrescu's excpected<T> with compilers that lack C++11's exception propagation facility and doing so without adding a dependency on e.g. Boost.

exception_ptr lite is not a full replacement of C++11's exception propagation facility. Specifically std::current_exception() is missing, so that you cannot intercept an exception in it's original guise.
