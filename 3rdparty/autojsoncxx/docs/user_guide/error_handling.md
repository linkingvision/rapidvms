# Error handling

## Classes

All the classes relevant to error handling are in namespace `autojsoncxx::error`.

### `ErrorBase`

This is the base of all error classes that denote a mismatch between JSON value and C++ specification. It exposes two public API, both declared `const`:

* `type()`: returns an integer denoting the runtime type of this error
* `description()`: returns a human readable description of the error.

To examine the actual payload with the error, it must be cast to one of its subclasses based on its type. The current types include:

```c++
SUCCESS = 0,
OBJECT_MEMBER = 1,
ARRAY_ELEMENT = 2,
MISSING_REQUIRED = 3,
TYPE_MISMATCH = 4,
NUMBER_OUT_OF_RANGE = 5,
ARRAY_LENGTH_MISMATCH = 6,
UNKNOWN_FIELD = 7,
DUPLICATE_KEYS = 8,
CORRUPTED_DOM = 9;
```

### `ErrorStack`

This is an intrusive stack of chained `ErrorBase` and thus noncopyable. Users should only query its const methods, which are compatible with STL classes: `begin()` and `end()` that returns an iterator to `ErrorBase`; `empty()` and `size()` for the information about the container.

### `ParsingResult`

This class is also in namespace `autojsoncxx`. It combines the error information from `rapidjson` and the `ErrorStack`. Relevant methods:

* `error_code()`
* `offset()`
* `error_stack()`
* `description()`

It can also be iterated over, a shorthand for iterating over the stack embedded within.

## Example

Call `error_code()` and `offset()` to examine it. When `error_code() == rapidjson::kParseErrorTermination`, you can also iterate over the `autojsoncxx::ParsingResult` object for any errors resulting from mapping JSON to C++ types; otherwise the error is a result of malformed JSON, such as missing coma, invalid escape sequence, etc.

```c++
if (!result.has_error())
    return;

// equivalent: if (result.error_stack().empty())
if (result.error_code() != rapidjson::kParseErrorTermination)
{
    std::cerr << "Malformed JSON: " << result.short_description() << '\n';
    return;
}

// equivalent: for (auto&& e: result.error_stack())
for (auto&& e : result) {
    using namespace autojsoncxx::error;

    switch (e.type()) {
    
    case UNKNOWN_FIELD: {
        const UnknownFieldError& err = static_cast<const UnknownFieldError&>(e);
        if (err.field_name().find("Version") != std::string::npos)
            std::cerr << "This is a definition of different protocol version\n";
    } break;

    case NUMBER_OUT_OF_RANGE:
        std::cerr << "Maybe you should use a 64-bit integer type instead?\n";
        break;

    case TYPE_MISMATCH: {
        const TypeMismatchError& err = static_cast<const TypeMismatchError&>(e);
        std::cout << "don't you dare use a " << err.actual_type()
                  << " to fool me!\n";
    } break;

    case OBJECT_MEMBER: {
        const ObjectMemberError& err = static_cast<const ObjectMemberError&>(e);
        std::cout << "The member " << err.member_name() << " is naughty!\n";
    } break;

    // Many more types of error has been defined, but not shown here for simplicity
    
    default:
        break;
    }
}
```
