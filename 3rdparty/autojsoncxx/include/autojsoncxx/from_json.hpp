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

#ifndef AUTOJSONCXX_FROM_JSON_HPP_29A4C106C1B1
#define AUTOJSONCXX_FROM_JSON_HPP_29A4C106C1B1

#include <autojsoncxx/utility.hpp>
#include <autojsoncxx/base.hpp>
#include <autojsoncxx/error.hpp>

#include <rapidjson/reader.h>
#include <rapidjson/filereadstream.h>

#include <cstdio>

namespace autojsoncxx {

template <class Reader, class InputStream, class ValueType>
inline bool from_json(Reader& r, InputStream& is, ValueType& value, ParsingResult& result)
{
    typedef SAXEventHandler<ValueType> handler_type;

    // If the ValueType is extremely complicated, the handler may also be extremely complicated
    // so it is safer to allocate it on the heap
    utility::scoped_ptr<handler_type> handler(new handler_type(&value));

    result.set_result(r.Parse(is, *handler));
    handler->ReapError(result.error_stack());
    return !result.has_error();
}

template <class InputStream, class ValueType>
inline bool from_json(InputStream& is, ValueType& value, ParsingResult& result)
{
    rapidjson::Reader reader;
    return from_json(reader, is, value, result);
}

template <class ValueType>
inline bool from_json_string(const char* json, ValueType& value, ParsingResult& result)
{
    rapidjson::StringStream ss(json);
    return from_json(ss, value, result);
}

template <class ValueType>
inline bool from_json_string(const std::string& json, ValueType& value, ParsingResult& result)
{
    return from_json_string(json.c_str(), value, result);
}

template <class ValueType>
inline bool from_json_file(std::FILE* file, ValueType& value, ParsingResult& result)
{
    char buffer[utility::default_buffer_size];
    rapidjson::FileReadStream fs(file, buffer, sizeof(buffer));
    return from_json(fs, value, result);
}

template <class ValueType>
inline bool from_json_file(const char* file_name, ValueType& value, ParsingResult& result)
{
    typedef utility::scoped_ptr<std::FILE, utility::file_closer> guard_type;

    guard_type file_guard(std::fopen(file_name, "r"));

    if (file_guard.empty())
        return false;

    return from_json_file(file_guard.get(), value, result);
}

template <class ValueType>
inline bool from_json_file(const std::string& file_name, ValueType& value, ParsingResult& result)
{
    return from_json_file(file_name.c_str(), value, result);
}
}

#endif
