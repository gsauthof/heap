This repository contains a C/C++ [binary heap][heap] implementation which
can be used for a priority-queue, for example.

Besides the standard operations, this implementation also
provides a remove operation and an interface that allows to store
and maintain an element's heap position in a referenced object
(i.e. to allow efficient removal).

## Design

The core of this small library is implemented in pure C. There is
also a thin C++ wrapper that provides some syntactic sugar and
convenience when C++ is available. Even the C API can be used in
a header-only fashion such that a standard compiler is able to
inline all user-provided specializations.

With the C API, specializations are provided via function
pointers which are converted into direct calls and further
inlined by a standard compiler when it's used in header-only
mode.

The C++ API is just a thin wrapper around the C API to avoid
repeating the code. The wrapper provides some useful overloads,
some typesafe templates and some generic function pointer
wrappers.

## Motivation

Besides providing a flexible heap implementation, this project is
also a playground to experiment with and evaluate:

- how to allow for efficient customizations in a plain C library
  (e.g. with function pointers that can be converted into
  inlineable direct calls)
- writing a core library in C and wrapping it for C++ users instead of
  the other way around. Thus, simplifying the usage in C
  programs (e.g. no linking against C++ runtime libraries
  necessary) or even in other languages which provide only a C
  ABI FFI.
- speed up the development of unittests by making the C/C++ library
  available as a python package via a modern and convenient
  foreign package creation library - i.e. [pybind11][pyb] - such that
  the tests can be written in Python using the ultra-convenient
  [pytest][pyt] package
- how to avoid namespace and symbol clashes/pollution in a plain
  C library - i.e. by prefixing all macros and symbols

## Dependencies

For the tests: [python3-pybind11][pyb]

## License

[Boost Software License 1.0](https://choosealicense.com/licenses/bsl-1.0/)


[pyb]: https://github.com/pybind/pybind11
[pyt]: https://docs.pytest.org/en/stable/
[heap]: https://en.wikipedia.org/wiki/Binary_heap
