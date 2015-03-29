# autojsoncxx

A header-only library and a code generator to **automagically** translate between **JSON** and **C++** types.

## Overview

JSON is a popular format for data exchange. Reading and writing JSON in C++, however, is nontrivial. Even with the help of libraries, one still needs to write lots of boilerplate code, and it is extremely hard to guard against all possible errors, since JSON is dynamically typed while C++ employs static typing.

More importantly, manually writing such code is a violation of **DRY** principle. When manually written, the class definition, parsing and serialization code can easily become out of sync, leading to brittle code and subtle bugs.

*autojsoncxx* is an attempt to solve this problem by automating such process.

### Dependency

* [RapidJSON](https://github.com/miloyip/rapidjson)
* [Python](https://www.python.org) (2.7 or 3.3+)
* (optional) [Parsimonious](https://github.com/erikrose/parsimonious)
* (optional) [Catch](https://github.com/philsquared/Catch)
* (optional) [Boost](http://www.boost.org)

## Features

* The parsing/serializing code are **automagically** generated.
* **Detailed error message**. Unlike ordinary JSON libraries, *autojsoncxx* will detect not only invalid JSON errors, but also mismatch between JSON value and C++ type specification.
* **Ease of use**. A single function call is enough for most use cases. The library has no complicated build setup, since it's header only.
* **Fast**. Based on the streaming API of `rapidjson` and C++ templates, this library is efficient both in space and time.
* **Flexible**. Whenever the support of a certain class `Foo` is added to the library, all the combination of types, like `std::vector<Foo>`, `std::shared_ptr<Foo>`, `std::deque<std::map<std::string, Foo>>>` is automatically supported as well, thanks to the use of template specialization.
* **Liberal license**. Both the library and its dependency are licensed liberally (MIT or BSD-like). Anyone is free to copy, distribute, modify or include in their own projects, be it open source or commercial.

## Testing

[![Build Status](https://travis-ci.org/netheril96/autojsoncxx.svg?branch=master)](https://travis-ci.org/netheril96/autojsoncxx)

To build the test, you need a sufficiently new compiler because the goal is to test all the type support, including many ones only introduced in c++11.

First clone the repository, and pull the dependency

```bash
git clone https://github.com/netheril96/autojsoncxx.git
git submodule init
git submodule update
```

*UNIX/Linux/Mac users*:

```bash
make
make test
```

*Windows users*:

Generate the `test/userdef.hpp` file from the definition `examples/userdef.json`. Then open the solution file under `test/mscvXX_test/` to build and run the test.

If too many tests fail, make sure your work directory points to the `test` directory.

### Currently tested compilers

* Clang 3.4/3.5 on Mac OS X (11.9)
* GCC 4.9 (Homebrew) on Mac OS X (11.9)
* Clang 3.0 on Ubuntu 12.04 (x64)
* GCC 4.8 on Ubuntu 14.04.1 (x86/x64)
* MSVC 10 (x86) on Windows 7
* MSVC 11/12 (x86/x64) on Windows 7

## Quick start

The code generator reads a JSON file that defines the class structure. An example definition is like this
```javascript
{
    "name": "Person",
    "members":
    [
        ["unsigned long long", "ID", {"required": true}],
        ["std::string", "name", {"default": "anonymous"}],
        ["double", "height"],
        ["double", "weight"],
        ["std::vector<unsigned long long>", "known_associates"]
    ]
}
```

Run the script *autojsoncxx.py* (requires Python 2.7+, including version 3+) on this definition file, and a header file will be generated. It includes a definition for `Person` as well as some helper classes.

```bash
python autojsoncxx.py --input=persondef.json --output=person.hpp
```

### Serialization

```c++
Person p;
autojsoncxx::to_pretty_json_file("person.json", p);
autojsoncxx::to_json_file(stdout, p);
autojsoncxx::to_json_string(std::make_shared<Person>(p));
```

### Parsing

```c++
autojsoncxx::ParsingResult result;
Person p;
if (!autojsoncxx::from_json_file("person.json", p, result)) {
    std::cerr << result << '\n';
    return -1;
}
```

### Error handling

If the JSON file is malformed, any decent JSON library will detect it and tell you what goes wrong. But what if the JSON value is perfectly valid, but not laid out the way you expected? Usually you have to manually check the DOM tree against your specification, but this library will automatically generates the necessary code.

Here is valid JSON file

```javascript
{
    "name": "Mike",
    "ID": 8940220481904,
    "height": 1.77,
    "weight": 70.0,
    "known_associates": [
        "Jack", "Mary"
    ]
}
```

Running through the parsing code, and you will get an error output:

```
Parsing failed at offset 127 with error code 16:
Terminate parsing due to Handler error.

Trace back (last call first):
(*) Type mismatch between expected type "uint64_t" and actual type "string"
(*) Error at array element with index 0
(*) Error at object member with name "known_associates"
```

One can also [query the errors programmingly](https://netheril96.github.io/autojsoncxx/user_guide/error_handling/).

## Documentation

Read more [here](https://netheril96.github.io/autojsoncxx/tutorial).

## Credit

This project is inspired by [google protobuf](https://developers.google.com/protocol-buffers/). It adopts the same approach in protobuf (compiler + definition file), and borrows various parse options from it.
