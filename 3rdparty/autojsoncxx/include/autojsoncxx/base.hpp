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

#ifndef AUTOJSONCXX_BASE_HPP_29A4C106C1B1
#define AUTOJSONCXX_BASE_HPP_29A4C106C1B1

#include <autojsoncxx/utility.hpp>
#include <autojsoncxx/error.hpp>

#include <string>
#include <cstring>

namespace autojsoncxx {

using utility::SizeType;

// The core handlers for parsing
template <class T>
class SAXEventHandler;

template <class Derived, class Ch = char>
class BaseSAXEventHandler {
protected:
    utility::scoped_ptr<error::ErrorBase> the_error;

public:
    static const char* type_name()
    {
        return Derived::type_name();
    }

protected:
    bool set_out_of_range(const char* actual_type)
    {
        the_error.reset(new error::NumberOutOfRangeError(type_name(), actual_type));
        return false;
    }

    bool set_type_mismatch(const char* actual_type)
    {
        the_error.reset(new error::TypeMismatchError(type_name(), actual_type));
        return false;
    }

public:
    bool Null()
    {
        return set_type_mismatch("null");
    }

    bool Bool(bool)
    {
        return set_type_mismatch("bool");
    }

    bool Int(int)
    {
        return set_type_mismatch("int");
    }

    bool Uint(unsigned)
    {
        return set_type_mismatch("unsigned");
    }

    bool Int64(utility::int64_t)
    {
        return set_type_mismatch("int64_t");
    }

    bool Uint64(utility::uint64_t)
    {
        return set_type_mismatch("uint64_t");
    }

    bool Double(double)
    {
        return set_type_mismatch("double");
    }

    bool String(const Ch*, SizeType, bool)
    {
        return set_type_mismatch("string");
    }

    bool StartObject()
    {
        return set_type_mismatch("object");
    }

    bool Key(const Ch*, SizeType, bool)
    {
        return set_type_mismatch("object");
    }

    bool EndObject(SizeType)
    {
        return set_type_mismatch("object");
    }

    bool StartArray()
    {
        return set_type_mismatch("array");
    }

    bool EndArray(SizeType)
    {
        return set_type_mismatch("array");
    }

    bool HasError() const
    {
        return !the_error.empty();
    }

    bool ReapError(error::ErrorStack& errs)
    {
        if (the_error.empty())
            return false;
        errs.push(the_error.release());
        return true;
    }

    void PrepareForReuse()
    {
        the_error.reset();
    }
};

#if AUTOJSONCXX_HAS_MODERN_TYPES

template <>
class SAXEventHandler<std::nullptr_t> : public BaseSAXEventHandler<SAXEventHandler<std::nullptr_t> > {
private:
public:
    static const char* type_name()
    {
        return "null";
    }

    explicit SAXEventHandler(std::nullptr_t*)
    {
    }

    bool Null()
    {
        return true;
    }
};

#endif

template <>
class SAXEventHandler<bool> : public BaseSAXEventHandler<SAXEventHandler<bool> > {
private:
    bool* m_value;

public:
    static const char* type_name()
    {
        return "bool";
    }

    explicit SAXEventHandler(bool* v)
        : m_value(v)
    {
    }

    bool Bool(bool v)
    {
        *m_value = v;
        return true;
    }
};

// This is mostly an alias for `bool` type so that the ugly `std::vector<bool>` can be avoided
template <>
class SAXEventHandler<char> : public BaseSAXEventHandler<SAXEventHandler<char> > {
private:
    char* m_value;

public:
    explicit SAXEventHandler(char* v)
        : m_value(v)
    {
    }

    static const char* type_name()
    {
        return "bool";
    }

    bool Bool(bool v)
    {
        *m_value = v;
        return true;
    }
};

template <>
class SAXEventHandler<int> : public BaseSAXEventHandler<SAXEventHandler<int> > {
private:
    int* m_value;

public:
    explicit SAXEventHandler(int* v)
        : m_value(v)
    {
    }

    bool Int(int i)
    {
        *m_value = i;
        return true;
    }

    bool Uint(unsigned i)
    {
        if (i > static_cast<unsigned>(std::numeric_limits<int>::max()))
            return set_out_of_range("unsigned");
        *m_value = static_cast<int>(i);
        return true;
    }

    bool Int64(utility::int64_t i)
    {
        if (i > static_cast<utility::int64_t>(std::numeric_limits<int>::max())
            || i < static_cast<utility::int64_t>(std::numeric_limits<int>::min()))
            return set_out_of_range("int64_t");
        *m_value = static_cast<int>(i);
        return true;
    }

    bool Uint64(utility::uint64_t i)
    {
        if (i > static_cast<utility::uint64_t>(std::numeric_limits<int>::max()))
            return set_out_of_range("uint64_t");
        *m_value = static_cast<int>(i);
        return true;
    }

    static const char* type_name()
    {
        return "int";
    }
};

template <>
class SAXEventHandler<unsigned> : public BaseSAXEventHandler<SAXEventHandler<unsigned> > {
private:
    unsigned* m_value;

public:
    explicit SAXEventHandler(unsigned* v)
        : m_value(v)
    {
    }

    bool Int(int i)
    {
        if (i < 0)
            return set_out_of_range("int");

        *m_value = static_cast<unsigned>(i);
        return true;
    }

    bool Uint(unsigned i)
    {
        *m_value = i;
        return true;
    }

    bool Int64(utility::int64_t i)
    {
        if (i < 0 || i > static_cast<utility::int64_t>(std::numeric_limits<unsigned>::max()))
            return set_out_of_range("int64_t");
        *m_value = static_cast<unsigned>(i);
        return true;
    }

    bool Uint64(utility::uint64_t i)
    {
        if (i > static_cast<utility::uint64_t>(std::numeric_limits<unsigned>::max()))
            return set_out_of_range("uint64_t");
        *m_value = static_cast<unsigned>(i);
        return true;
    }

    static const char* type_name()
    {
        return "unsigned";
    }
};

template <>
class SAXEventHandler<utility::int64_t> : public BaseSAXEventHandler<SAXEventHandler<utility::int64_t> > {
private:
    utility::int64_t* m_value;

public:
    explicit SAXEventHandler(utility::int64_t* v)
        : m_value(v)
    {
    }

    bool Int(int i)
    {
        *m_value = i;
        return true;
    }

    bool Uint(unsigned i)
    {
        *m_value = i;
        return true;
    }

    bool Int64(utility::int64_t i)
    {
        *m_value = i;
        return true;
    }

    bool Uint64(utility::uint64_t i)
    {
        if (i > static_cast<utility::uint64_t>(std::numeric_limits<utility::int64_t>::max()))
            return set_out_of_range("uint64_t");
        *m_value = static_cast<utility::int64_t>(i);
        return true;
    }

    static const char* type_name()
    {
        return "int64_t";
    }
};

template <>
class SAXEventHandler<utility::uint64_t> : public BaseSAXEventHandler<SAXEventHandler<utility::uint64_t> > {
private:
    utility::uint64_t* m_value;

public:
    explicit SAXEventHandler(utility::uint64_t* v)
        : m_value(v)
    {
    }

    bool Int(int i)
    {
        if (i < 0)
            return set_out_of_range("int");
        *m_value = static_cast<utility::uint64_t>(i);
        return true;
    }

    bool Uint(unsigned i)
    {
        *m_value = i;
        return true;
    }

    bool Int64(utility::int64_t i)
    {
        if (i < 0)
            return set_out_of_range("int64_t");
        *m_value = static_cast<utility::uint64_t>(i);
        return true;
    }

    bool Uint64(utility::uint64_t i)
    {
        *m_value = i;
        return true;
    }

    static const char* type_name()
    {
        return "uint64_t";
    }
};

template <>
class SAXEventHandler<double> : public BaseSAXEventHandler<SAXEventHandler<double> > {
private:
    double* m_value;

public:
    explicit SAXEventHandler(double* v)
        : m_value(v)
    {
    }

    bool Int(int i)
    {
        *m_value = i;
        return true;
    }

    bool Uint(unsigned i)
    {
        *m_value = i;
        return true;
    }

    bool Int64(utility::int64_t i)
    {
        const utility::int64_t threshold = 1LL << 53;
        if (i > threshold || i < -threshold)
            return this->set_out_of_range("int64_t");
        // the maximum value of double is much larger, but we want to prevent precision loss

        *m_value = static_cast<double>(i);
        return true;
    }

    bool Uint64(utility::uint64_t i)
    {
        const utility::uint64_t threshold = 1ULL << 53;
        if (i > threshold)
            return this->set_out_of_range("uint64_t");

        *m_value = static_cast<double>(i);
        return true;
    }

    bool Double(double d)
    {
        *m_value = d;
        return true;
    }

    static const char* type_name()
    {
        return "double";
    }
};

template <>
class SAXEventHandler<std::string> : public BaseSAXEventHandler<SAXEventHandler<std::string> > {
private:
    std::string* m_value;

public:
    explicit SAXEventHandler(std::string* v)
        : m_value(v)
    {
    }

    bool String(const char* str, SizeType length, bool)
    {
        m_value->assign(str, length);
        return true;
    }

    static const char* type_name()
    {
        return "string";
    }
};

template <class Writer, class T>
struct Serializer;

template <class Writer>
struct Serializer<Writer, int> {
    void operator()(Writer& w, int i) const
    {
        w.Int(i);
    }
};

#if AUTOJSONCXX_HAS_MODERN_TYPES

template <class Writer>
struct Serializer<Writer, std::nullptr_t> {
    void operator()(Writer& w, std::nullptr_t) const
    {
        w.Null();
    }
};

#endif

template <class Writer>
struct Serializer<Writer, bool> {
    void operator()(Writer& w, bool b) const
    {
        w.Bool(b);
    }
};

template <class Writer>
struct Serializer<Writer, char> {
    void operator()(Writer& w, char c) const
    {
        w.Bool(c);
    }
};

template <class Writer>
struct Serializer<Writer, unsigned> {
    void operator()(Writer& w, unsigned i) const
    {
        w.Uint(i);
    }
};

template <class Writer>
struct Serializer<Writer, utility::int64_t> {
    void operator()(Writer& w, utility::int64_t i) const
    {
        w.Int64(i);
    }
};

template <class Writer>
struct Serializer<Writer, utility::uint64_t> {
    void operator()(Writer& w, utility::uint64_t i) const
    {
        w.Uint64(i);
    }
};

template <class Writer>
struct Serializer<Writer, double> {
    void operator()(Writer& w, double d) const
    {
        w.Double(d);
    }
};

template <class Writer>
struct Serializer<Writer, std::string> {
    void operator()(Writer& w, const std::string& str) const
    {
        w.String(str.c_str(), static_cast<SizeType>(str.size()), true);
    }
};
}
#endif
