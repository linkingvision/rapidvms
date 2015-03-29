# Miscellaneous

## Memory handling and exceptions

Exception handling (`throw`, `try`, `catch`) is not used by this library, to accommodate the needs of fake C++ programmers. It is designed, however, to be exception safe by using RAII wrappers to do all resource management. Copy, move constructor/assignment operator are disabled at certain places to avoid ownership mismanagement.

Notably, the `ParsingResult` class is not copyable. This simplifies the memory handling because it fully owns the error stack. It is movable, however, if you define `AUTOJSONCXX_HAS_RVALUE`. If you ever need to pass it around or store it somewhere, the simplest way is to use a shared pointer.

## C++11 features

A set of macros control the usage of c++11 features. Define these macros as nonzero constants *before* inclusion, or define it in your build system.

* `AUTOJSONCXX_MODERN_COMPILER`: turn on all of the below
* `AUTOJSONCXX_HAS_MODERN_TYPES`: add support for c++11 new types, such as `std::shared_ptr<>`.
* `AUTOJSONCXX_HAS_RVALUE`: enable the use of r-value references and move semantic.
* `AUTOJSONCXX_HAS_NOEXCEPT`: enable the use of keyword `noexcept` and the function `std::move_if_noexcept()`.
* `AUTOJSONCXX_HAS_VARIADIC_TEMPLATE`: enable the use of variadic templates. required if `std::tuple<>` is used.
* `AUTOJSONCXX_HAS_EXPLICIT_OPERATOR`: enable the use of `explicit operator bool()`. Otherwise no conversion to bool operator is defined.

The 64-bit integer type `long long` and `unsigned long long` is always required. Though not in C++03 standard, most compilers support it nonetheless.

## Encoding

The default encoding is `UTF-8`. If you need to read/write JSON in `UTF-16` or `UTF-32`, instantiate the class `SAXEventHandler` and/or `Serializer`, and use it in combination with RapidJSON's transcoding capability.
