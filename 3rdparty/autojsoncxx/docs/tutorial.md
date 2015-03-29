# Tutorial

## Code generation

The code generator reads a JSON file that defines the class structure. An example definition is like this (remember to fully qualify the type name with its namespace)

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

Run the script *autojsoncxx.py* (requires Python 2.7+, including version 3+) on this definition file, and a header file will be generated. It includes a definition for `Person` as well as some helper classes. The `Person` is a `struct` with all members public, meant as a data holder without any additional functionalities. It can be used with free functions, or [wrapped up in another class to provide encapsulation and polymorphism](https://en.wikipedia.org/wiki/Composition_over_inheritance).

```bash
python autojsoncxx.py --input=persondef.json --output=person.hpp
```

Remember to add the include directory of *autojsoncxx* and *rapidjson* to your project header search path (no linking is required).

The below examples uses c++11 features, but the library also works with c++03 compilers (provided you do not use new classes from c++11).

## Serialization

```c++
#define AUTOJSONCXX_MODERN_COMPILER 1 // Turn on all the c++11 features of the library
#include <iostream>
#include "person.hpp"

int main()
{
    Person p;
    p.name = "Mike";
    p.ID = 8940220481904ULL;
    p.weight = 70;
    p.height = 1.77;
    p.known_associates = { 149977889346362, 90000134866608, 44412567664 };
    // Use successive push_back() if your compiler is not c++11 ready

    autojsoncxx::to_pretty_json_file("person.json", p);
    return 0;
}
```

This will generate a file `person.json` with contents below:

```javascript
{
    "name": "Mike",
    "ID": 8940220481904,
    "height": 1.77,
    "weight": 70.0,
    "known_associates": [
        149977889346362,
        90000134866608,
        44412567664
    ]
}
```

## Parsing
Now let's try read that back

```c++
#define AUTOJSONCXX_MODERN_COMPILER 1
#include <iostream>
#include "person.hpp"

int main()
{
    autojsoncxx::ParsingResult result;
    Person p;
    if (!autojsoncxx::from_json_file("person.json", p, result)) {
        std::cerr << result << '\n';
        return -1;
    }

    std::cout << "ID: " << p.ID << '\n'
              << "name:  " << p.name << '\n'
              << "height: " << p.height << '\n'
              << "weight: " << p.weight << '\n';

    std::cout << "known associates: ";
    for (auto&& id : p.known_associates)
        std::cout << id << '\t';
    std::cout << '\n';
    return 0;
}
```

## Error handling

Any decent JSON library can detect and report errors resulting from a invalid JSON. In addition, *autojsoncxx* will detect and stop processing once the JSON value does not match the C++ type specification. This enforces the static typing of C++ on the dynamic JSON, and can be seen as a limited form of JSON schema.

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

You can also [programmingly examine the error](user_guide/error_handling.md).

## More complex types

The design of `autojsoncxx` makes it possible to read/write not only the class generated, but also the combination of all supported types. That is, you can also call the `to_json` and `from_json` family of functions on `std::vector<Person>`, `std::map<std::string, Person>`, `std::shared_ptr<Person>`, or even more complicated ones like `std::tuple<std::string, double, Person, std::deque<int>, std::vector<std::unique_ptr<Person>>, bool>`.

This also makes it possible to build more complex types. For example, the definition file can be written as

```javascript
[
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
    },
    {
        "name": "Group",
        "members":
        [
            ["unsigned long long", "GroupID", {"required": true}],
            ["std::string", "name"],
            ["std::vector<std::shared_ptr<Person>>", "persons"]
        ]
    }
]
```

So you have another struct that contains `Person` as a member.

## DOM support

*autojsoncxx* is based on streams, which is efficient both in time and space. However, maybe sometimes Document Object Model is desired. One can convert types supported by *autojsoncxx* to/from `rapidjson::Document` directly through free functions `to_document` and `from_document`.

In addition, `rapidjson::Document` is supported the same as `int` or `std::map`, so it can be the type of class members. This makes it possible to intermix static typing and dynamic typing.
