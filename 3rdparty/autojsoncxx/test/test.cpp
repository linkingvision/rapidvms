// The MIT License (MIT)
//
// Copyright (c) 2014 Siyuan Ren (netheril96@gmail.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <catch.hpp>

#define AUTOJSONCXX_HAS_MODERN_TYPES 1
#define AUTOJSONCXX_HAS_RVALUE 1

// Uncomment the next line if you are adventurous
// #define AUTOJSONCXX_HAS_VARIADIC_TEMPLATE 1

#ifndef AUTOJSONCXX_ROOT_DIRECTORY
#define AUTOJSONCXX_ROOT_DIRECTORY ".."
#endif

#include "userdef.hpp"

#include <fstream>
#include <sstream>
#include <stack>

using namespace autojsoncxx;
using namespace config;
using namespace config::event;

inline bool operator==(Date d1, Date d2)
{
    return d1.year == d2.year && d1.month == d2.month && d1.day == d2.day;
}

inline bool operator!=(Date d1, Date d2)
{
    return !(d1 == d2);
}

inline std::string read_all(const char* file_name)
{
    std::ifstream input(file_name);
    std::ostringstream buffer;
    buffer << input.rdbuf();
    return buffer.str();
}

inline Date create_date(int year, int month, int day)
{
    Date d;
    d.year = year;
    d.month = month;
    d.day = day;
    return d;
}

template <class T, std::size_t num_elements_per_node>
inline bool is_consistent(const std::stack<T>& stk1, const utility::stack<T, num_elements_per_node>& stk2)
{
    if (stk1.empty() != stk2.empty())
        return false;

    if (stk1.size() != stk2.size())
        return false;

    if (!stk2.empty())
        return stk1.top() == stk2.top();

    return true;
}

TEST_CASE("Test for internal implementations", "[internal]")
{
    std::stack<std::string> standard_stack;
    utility::stack<std::string, 2> test_stack;

    enum operation_type {
        PUSH,
        POP
    };

    struct operation {
        operation_type type;
        std::string arg;

        operation(operation_type type_)
            : type(type_)
        {
        }

        operation(operation_type type_, std::string arg_)
            : type(type_)
        {
            arg.swap(arg_);
        }
    };

    operation ops[] = {
        operation(PUSH, "1"),
        operation(POP),
        operation(PUSH, "2"),
        operation(PUSH, "3"),
        operation(PUSH, "4"),
        operation(PUSH, "5"),
        operation(POP),
        operation(POP),
        operation(PUSH, "10"),
        operation(POP),
        operation(PUSH, "9")
    };

    for (std::size_t i = 0; i < sizeof(ops) / sizeof(*ops); ++i) {
        switch (ops[i].type) {
        case PUSH:
            standard_stack.push(ops[i].arg);
            test_stack.push(ops[i].arg);
            break;

        case POP:
            standard_stack.pop();
            test_stack.pop();
            break;

        default:
            break;
        }
        REQUIRE(is_consistent(standard_stack, test_stack));
    }
}

// If most of the cases fail, you probably set the work directory wrong.
// Point the work directory to the `test/` subdirectory
// or redefine the macro AUTOJSONCXX_ROOT_DIRECTORY.

TEST_CASE("Test for the constructor of generated class", "[code generator]")
{
    User user;

    REQUIRE(user.ID == 0ULL);

    // Do not use string literal because MSVC will mess up the encoding
    static const char default_nickname[] = { char(0xe2), char(0x9d), char(0xb6), char(0xe2),
                                             char(0x9d), char(0xb7), char(0xe2), char(0x9d),
                                             char(0xb8) };

    REQUIRE(user.nickname.size() == sizeof(default_nickname));
    REQUIRE(std::equal(user.nickname.begin(), user.nickname.end(), default_nickname));

    REQUIRE(user.birthday == create_date(0, 0, 0));
    REQUIRE(user.dark_history.empty());
    REQUIRE(user.optional_attributes.empty());
    REQUIRE(!user.block_event);

    BlockEvent event;

    REQUIRE(event.admin_ID == 255ULL);
    REQUIRE(event.date == create_date(1970, 1, 1));
    REQUIRE(event.serial_number == 0ULL);
    REQUIRE(event.details.empty());
}

TEST_CASE("Test for correct parsing", "[parsing]")
{
    SECTION("Test for an array of user", "[parsing]")
    {
        std::vector<User> users;
        ParsingResult err;

        bool success = from_json_file(AUTOJSONCXX_ROOT_DIRECTORY "/examples/success/user_array.json", users, err);
        {
            CAPTURE(err.description());
            REQUIRE(success);
        }
        REQUIRE(users.size() == 2);

        {
            const User& u = users.front();
            REQUIRE(u.ID == 7947402710862746952ULL);
            REQUIRE(u.nickname == "bigger than bigger");
            REQUIRE(u.birthday == create_date(1984, 9, 2));

            REQUIRE(u.block_event.get() != 0);
            const BlockEvent& e = *u.block_event;

            REQUIRE(e.admin_ID > 0ULL);
            REQUIRE(e.date == create_date(1970, 12, 31));
            REQUIRE(e.description == "advertisement");
            REQUIRE(e.details.size() > 0ULL);

            REQUIRE(u.dark_history.empty());
            REQUIRE(u.optional_attributes.empty());
        }

        {
            const User& u = users.back();
            REQUIRE(u.ID == 13478355757133566847ULL);
            REQUIRE(u.nickname.size() == 15);
            REQUIRE(!u.block_event);
            REQUIRE(u.optional_attributes.size() == 3);
            REQUIRE(u.optional_attributes.find("Self description") != u.optional_attributes.end());
        }
    }

    SECTION("Test for a map of user", "[parsing]")
    {
        std::unordered_map<std::string, User> users;
        ParsingResult err;

        bool success = from_json_file(AUTOJSONCXX_ROOT_DIRECTORY "/examples/success/user_map.json", users, err);
        {
            CAPTURE(err.description());
            REQUIRE(success);
        }
        REQUIRE(users.size() == 2);

        {
            const User& u = users["First"];
            REQUIRE(u.ID == 7947402710862746952ULL);
            REQUIRE(u.nickname == "bigger than bigger");
            REQUIRE(u.birthday == create_date(1984, 9, 2));

            REQUIRE(u.block_event.get() != 0);
            const BlockEvent& e = *u.block_event;

            REQUIRE(e.admin_ID > 0ULL);
            REQUIRE(e.date == create_date(1970, 12, 31));
            REQUIRE(e.description == "advertisement");
            REQUIRE(e.details.size() > 0ULL);

            REQUIRE(u.dark_history.empty());
            REQUIRE(u.optional_attributes.empty());
        }

        {
            const User& u = users["Second"];
            REQUIRE(u.ID == 13478355757133566847ULL);
            REQUIRE(u.nickname.size() == 15);
            REQUIRE(!u.block_event);
            REQUIRE(u.optional_attributes.size() == 3);
            REQUIRE(u.optional_attributes.find("Self description") != u.optional_attributes.end());
        }
    }
}

TEST_CASE("Test for mismatch between JSON and C++ class std::vector<config::User>", "[parsing], [error]")
{
    std::vector<User> users;
    ParsingResult err;

    SECTION("Mismatch between array and object", "[parsing], [error], [type mismatch]")
    {
        REQUIRE(!from_json_file(AUTOJSONCXX_ROOT_DIRECTORY "/examples/failure/single_object.json", users, err));
        CAPTURE(err.description());
        REQUIRE(!err.error_stack().empty());

        REQUIRE(err.begin()->type() == error::TYPE_MISMATCH);
        REQUIRE(std::distance(err.begin(), err.end()) == 1);

        auto&& e = static_cast<const error::TypeMismatchError&>(*err.begin());

        REQUIRE(e.expected_type() == "array");

        REQUIRE(e.actual_type() == "object");
    }

    SECTION("Required field not present; test the path as well", "[parsing], [error], [missing required]")
    {
        REQUIRE(!from_json_file(AUTOJSONCXX_ROOT_DIRECTORY "/examples/failure/missing_required.json", users, err));
        CAPTURE(err.description());
        REQUIRE(!err.error_stack().empty());

        REQUIRE(std::distance(err.begin(), err.end()) == 5);

        auto it = err.begin();
        REQUIRE(it->type() == error::MISSING_REQUIRED);

        ++it;
        REQUIRE(it->type() == error::OBJECT_MEMBER);
        REQUIRE(static_cast<const error::ObjectMemberError&>(*it).member_name() == "date");

        ++it;
        REQUIRE(it->type() == error::ARRAY_ELEMENT);
        REQUIRE(static_cast<const error::ArrayElementError&>(*it).index() == 0);

        ++it;
        REQUIRE(it->type() == error::OBJECT_MEMBER);
        REQUIRE(static_cast<const error::ObjectMemberError&>(*it).member_name() == "dark_history");
    }

    SECTION("Unknown field in strict parsed class Date", "[parsing], [error], [unknown field]")
    {
        REQUIRE(!from_json_file(AUTOJSONCXX_ROOT_DIRECTORY "/examples/failure/unknown_field.json", users, err));
        CAPTURE(err.description());
        REQUIRE(!err.error_stack().empty());

        REQUIRE(err.begin()->type() == error::UNKNOWN_FIELD);

        REQUIRE(static_cast<const error::UnknownFieldError&>(*err.begin()).field_name() == "hour");
    }

    SECTION("Duplicate key", "[parsing], [error], [duplicate key]")
    {
        REQUIRE(!from_json_file(AUTOJSONCXX_ROOT_DIRECTORY "/examples/failure/duplicate_key.json", users, err));
        CAPTURE(err.description());
        REQUIRE(!err.error_stack().empty());

        REQUIRE(err.begin()->type() == error::DUPLICATE_KEYS);

        REQUIRE(static_cast<const error::DuplicateKeyError&>(*err.begin()).key() == "Auth-Token");
    }

    SECTION("Out of range", "[parsing], [error], [out of range]")
    {
        REQUIRE(!from_json_file(AUTOJSONCXX_ROOT_DIRECTORY "/examples/failure/out_of_range.json", users, err));
        CAPTURE(err.description());
        REQUIRE(!err.error_stack().empty());

        REQUIRE(err.begin()->type() == error::NUMBER_OUT_OF_RANGE);
    }

    SECTION("Mismatch between integer and string", "[parsing], [error], [type mismatch]")
    {
        REQUIRE(!from_json_file(AUTOJSONCXX_ROOT_DIRECTORY "/examples/failure/integer_string.json", users, err));
        CAPTURE(err.description());
        REQUIRE(!err.error_stack().empty());

        REQUIRE(err.begin()->type() == error::TYPE_MISMATCH);
    }

    SECTION("Null character in key", "[parsing], [error], [null character]")
    {
        REQUIRE(!from_json_file(AUTOJSONCXX_ROOT_DIRECTORY "/examples/failure/null_in_key.json", users, err));
        CAPTURE(err.description());
        REQUIRE(!err.error_stack().empty());

        REQUIRE(err.begin()->type() == error::UNKNOWN_FIELD);
    }

    SECTION("Array length not match the fixed C++ type", "[parsing], [error], [length mismatch]")
    {
        std::array<User, 3> trinity;

        REQUIRE(!from_json_file(AUTOJSONCXX_ROOT_DIRECTORY "/examples/success/user_array.json", trinity, err));
        CAPTURE(err.description());
        REQUIRE(!err.error_stack().empty());
        REQUIRE(err.begin()->type() == error::ARRAY_LENGTH_MISMATCH);
    }
}

TEST_CASE("Test for mismatch between JSON and C++ class std::map<std::string, config::User>", "[parsing], [error]")
{
    std::map<std::string, config::User> users;
    ParsingResult err;

    SECTION("Mismatch between object and array", "[parsing], [error], [type mismatch]")
    {
        REQUIRE(!from_json_file(AUTOJSONCXX_ROOT_DIRECTORY "/examples/success/user_array.json", users, err));
        CAPTURE(err.description());
        REQUIRE(!err.error_stack().empty());

        REQUIRE(err.begin()->type() == error::TYPE_MISMATCH);

        auto&& e = static_cast<const error::TypeMismatchError&>(*err.begin());
        REQUIRE(e.expected_type() == "object");
        REQUIRE(e.actual_type() == "array");
    }

    SECTION("Mismatch in mapped element", "[parsing], [error], [type mismatch]")
    {
        REQUIRE(!from_json_file(AUTOJSONCXX_ROOT_DIRECTORY "/examples/failure/map_element_mismatch.json", users, err));
        CAPTURE(err.description());
        REQUIRE(!err.error_stack().empty());
        {
            REQUIRE(err.begin()->type() == error::TYPE_MISMATCH);

            auto&& e = static_cast<const error::TypeMismatchError&>(*err.begin());
        }
        {
            auto it = ++err.begin();
            REQUIRE(it != err.end());
            REQUIRE(it->type() == error::OBJECT_MEMBER);
            auto&& e = static_cast<const error::ObjectMemberError&>(*it);
            REQUIRE(e.member_name() == "Third");
        }
    }
}

TEST_CASE("Test for writing JSON", "[serialization]")
{
    std::vector<User> users;
    ParsingResult err;

    bool success = from_json_file(AUTOJSONCXX_ROOT_DIRECTORY "/examples/success/user_array.json", users, err);
    {
        CAPTURE(err.description());
        REQUIRE(success);
    }
    REQUIRE(users.size() == 2);

    REQUIRE(to_json_string(users) == read_all(AUTOJSONCXX_ROOT_DIRECTORY "/examples/success/user_array_compact.json"));
}

TEST_CASE("Test for DOM support", "[DOM]")
{
    rapidjson::Document doc;
    ParsingResult err;
    bool success = from_json_file(AUTOJSONCXX_ROOT_DIRECTORY "/examples/success/user_array_compact.json", doc, err);
    {
        CAPTURE(err.description());
        REQUIRE(success);
    }

    SECTION("Test for parsed result", "[DOM], [parsing]")
    {
        REQUIRE(doc.IsArray());
        REQUIRE(doc.Size() == 2);

        const rapidjson::Value& second = doc[1u];
        REQUIRE(second["ID"].IsUint64());
        REQUIRE(second["ID"].GetUint64() == 13478355757133566847ULL);
        REQUIRE(second["block_event"].IsNull());
        REQUIRE(second["dark_history"].IsArray());
        REQUIRE(second["dark_history"][0u].IsObject());
        REQUIRE(second["dark_history"][0u]["description"] == "copyright infringement");
    }

    SECTION("Test for serialization", "[DOM], [serialization]")
    {
        std::string output;
        to_json_string(output, doc);
        REQUIRE(output == read_all(AUTOJSONCXX_ROOT_DIRECTORY "/examples/success/user_array_compact.json"));
    }

    SECTION("Test for to/from DOM", "[DOM], [conversion]")
    {
        std::vector<User> users;
        error::ErrorStack errs;

        REQUIRE(from_document(users, doc, errs));

        REQUIRE(users.size() == 2);
        REQUIRE(users[0].birthday == create_date(1984, 9, 2));
        REQUIRE(users[0].block_event);
        REQUIRE(users[0].block_event->details == "most likely a troll");

        rapidjson::Document another_doc;
        to_document(users, another_doc);
        REQUIRE(doc == another_doc);
    }
}

#if AUTOJSONCXX_HAS_VARIADIC_TEMPLATE

TEST_CASE("Test for parsing tuple type", "[parsing], [tuple]")
{
    typedef std::tuple<BlockEvent, int, std::nullptr_t, double, std::unordered_map<std::string, std::shared_ptr<User> >, bool> hard_type;
    hard_type hetero_array;
    ParsingResult err;

    SECTION("Test for valid tuple", "[parsing], [tuple]")
    {
        bool success = from_json_file(AUTOJSONCXX_ROOT_DIRECTORY "/examples/success/hard.json", hetero_array, err);
        {
            CAPTURE(err.description());
            REQUIRE(success);
        }
        REQUIRE(std::get<1>(hetero_array) == -65535);
        REQUIRE(std::get<std::tuple_size<hard_type>::value - 1>(hetero_array) == false);
    }

    SECTION("Test for invalid tuple", "[parsing], [tuple], [error]")
    {
        REQUIRE(!from_json_file(AUTOJSONCXX_ROOT_DIRECTORY "/examples/failure/hard.json", hetero_array, err));
        CAPTURE(err.description());
        REQUIRE(!err.error_stack().empty());
        REQUIRE(err.begin()->type() == error::TYPE_MISMATCH);
        REQUIRE(static_cast<const error::TypeMismatchError&>(*err.begin()).actual_type() == "null");
    }
}
#endif
