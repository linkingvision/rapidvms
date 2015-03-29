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

#ifndef AUTOJSONCXX_TO_JSON_HPP_29A4C106C1B1
#define AUTOJSONCXX_TO_JSON_HPP_29A4C106C1B1

#include <autojsoncxx/utility.hpp>
#include <autojsoncxx/base.hpp>
#include <autojsoncxx/error.hpp>

#include <rapidjson/writer.h>
#include <rapidjson/filewritestream.h>
#include <rapidjson/prettywriter.h>

#include <cstdio>

namespace autojsoncxx {

template <class Writer, class ValueType>
inline void write_json(Writer& w, const ValueType& v)
{
    Serializer<Writer, ValueType>()(w, v);
}

template <class OutputStream, class ValueType>
inline void to_json(OutputStream& os, const ValueType& v)
{
    rapidjson::Writer<OutputStream> w(os);
    write_json(w, v);
}

template <class ValueType>
inline void to_json_string(std::string& str, const ValueType& v,
                           std::size_t BufferSize = utility::default_buffer_size)
{
    typedef rapidjson::StringBuffer OutputStreamType;
    OutputStreamType os(0, BufferSize);
    to_json(os, v);
    str.assign(os.GetString(), os.GetSize());
}

template <class ValueType>
inline std::string to_json_string(const ValueType& v)
{
    std::string str;
    to_json_string(str, v);
    return str;
}

template <class ValueType>
inline bool to_json_file(std::FILE* fp, const ValueType& v)
{
    char buffer[utility::default_buffer_size];
    rapidjson::FileWriteStream os(fp, buffer, sizeof(buffer));
    to_json(os, v);
    return !std::ferror(fp);
}

template <class ValueType>
inline bool to_json_file(const char* file_name, const ValueType& v)
{
    typedef utility::scoped_ptr<std::FILE, utility::file_closer> guard_type;

    guard_type file_guard(std::fopen(file_name, "w"));

    if (file_guard.empty())
        return false;

    return to_json_file(file_guard.get(), v);
}

template <class ValueType>
inline bool to_json_file(const std::string& file_name, const ValueType& v)
{
    return to_json_file(file_name.c_str(), v);
}

template <class OutputStream, class ValueType>
inline void to_pretty_json(OutputStream& os, const ValueType& v)
{
    rapidjson::PrettyWriter<OutputStream> w(os);
    write_json(w, v);
}

template <class ValueType>
inline void to_pretty_json_string(std::string& str, const ValueType& v,
                                  std::size_t BufferSize = utility::default_buffer_size)
{
    typedef rapidjson::StringBuffer OutputStreamType;
    OutputStreamType os(0, BufferSize);
    to_pretty_json(os, v);
    str.assign(os.GetString(), os.GetSize());
}

template <class ValueType>
inline std::string to_pretty_json_string(const ValueType& v)
{
    std::string str;
    to_pretty_json_string(str, v);
    return str;
}

template <class ValueType>
inline bool to_pretty_json_file(std::FILE* fp, const ValueType& v)
{
    char buffer[utility::default_buffer_size];
    rapidjson::FileWriteStream os(fp, buffer, sizeof(buffer));
    to_pretty_json(os, v);
    return !std::ferror(fp);
}

template <class ValueType>
inline bool to_pretty_json_file(const char* file_name, const ValueType& v)
{
    typedef utility::scoped_ptr<std::FILE, utility::file_closer> guard_type;

    guard_type file_guard(std::fopen(file_name, "w"));

    if (file_guard.empty())
        return false;

    return to_pretty_json_file(file_guard.get(), v);
}

template <class ValueType>
inline bool to_pretty_json_file(const std::string& file_name, const ValueType& v)
{
    return to_pretty_json_file(file_name.c_str(), v);
}
}

#endif
