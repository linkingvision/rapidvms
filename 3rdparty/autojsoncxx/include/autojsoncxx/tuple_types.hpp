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

#ifndef AUTOJSONCXX_TUPLE_TYPES_HPP_29A4C106C1B1
#define AUTOJSONCXX_TUPLE_TYPES_HPP_29A4C106C1B1

#include <autojsoncxx/utility.hpp>
#include <autojsoncxx/base.hpp>
#include <autojsoncxx/error.hpp>

#if AUTOJSONCXX_HAS_MODERN_TYPES && AUTOJSONCXX_HAS_VARIADIC_TEMPLATE
#include <tuple>

namespace autojsoncxx {

template <class Writer, class Tuple, std::size_t i>
struct TupleSerializer {
    void operator()(Writer& w, const Tuple& t) const
    {
        TupleSerializer<Writer, Tuple, i - 1>()(w, t);
        Serializer<Writer, typename std::tuple_element<i, Tuple>::type>()(w, std::get<i>(t));
    }
};

template <class Writer, class Tuple>
struct TupleSerializer<Writer, Tuple, 0> {
    void operator()(Writer& w, const Tuple& t) const
    {
        Serializer<Writer, typename std::tuple_element<0, Tuple>::type>()(w, std::get<0>(t));
    }
};

template <class Writer, class... Args>
struct Serializer<Writer, std::tuple<Args...> > {
    void operator()(Writer& w, const std::tuple<Args...>& t) const
    {
        w.StartArray();
        TupleSerializer<Writer, std::tuple<Args...>, sizeof...(Args)-1>()(w, t);
        w.EndArray();
    }
};

template <class Tuple, std::size_t TupleSize, std::size_t index>
class TupleSAXEventHandler : public TupleSAXEventHandler<Tuple, TupleSize, index + 1> {

public:
    typedef typename std::tuple_element<index, Tuple>::type element_type;
    typedef TupleSAXEventHandler<Tuple, TupleSize, index + 1> base_type;

private:
    SAXEventHandler<element_type> internal_handler;
    int total_depth;
    bool finished;

    bool finish_when_depth_is_right(bool success)
    {
        if (success && total_depth == 0)
            finished = true;
        return success;
    }

public:
    explicit TupleSAXEventHandler(Tuple* t)
        : base_type(t)
        , internal_handler(&std::get<index>(*t))
        , total_depth(0)
        , finished(false)
    {
    }

    bool Null()
    {
        if (finished)
            return static_cast<base_type*>(this)->Null();
        return finish_when_depth_is_right(internal_handler.Null());
    }

    bool Bool(bool b)
    {
        if (finished)
            return static_cast<base_type*>(this)->Bool(b);
        return finish_when_depth_is_right(internal_handler.Bool(b));
    }

    bool Int(int i)
    {
        if (finished)
            return static_cast<base_type*>(this)->Int(i);
        return finish_when_depth_is_right(internal_handler.Int(i));
    }

    bool Uint(unsigned i)
    {
        if (finished)
            return static_cast<base_type*>(this)->Uint(i);
        return finish_when_depth_is_right(internal_handler.Uint(i));
    }

    bool Int64(utility::int64_t i)
    {
        if (finished)
            return static_cast<base_type*>(this)->Int64(i);
        return finish_when_depth_is_right(internal_handler.Int64(i));
    }

    bool Uint64(utility::uint64_t i)
    {
        if (finished)
            return static_cast<base_type*>(this)->Uint64(i);
        return finish_when_depth_is_right(internal_handler.Uint64(i));
    }

    bool Double(double d)
    {
        if (finished)
            return static_cast<base_type*>(this)->Double(d);
        return finish_when_depth_is_right(internal_handler.Double(d));
    }

    bool String(const char* str, SizeType len, bool copy)
    {
        if (finished)
            return static_cast<base_type*>(this)->String(str, len, copy);
        return finish_when_depth_is_right(internal_handler.String(str, len, copy));
    }

    bool Key(const char* str, SizeType len, bool copy)
    {
        if (finished)
            return static_cast<base_type*>(this)->Key(str, len, copy);
        return finish_when_depth_is_right(internal_handler.Key(str, len, copy));
    }

    bool StartObject()
    {
        if (finished)
            return static_cast<base_type*>(this)->StartObject();
        ++total_depth;
        return internal_handler.StartObject();
    }

    bool StartArray()
    {
        if (finished)
            return static_cast<base_type*>(this)->StartArray();
        ++total_depth;
        return internal_handler.StartArray();
    }

    bool EndObject(SizeType len)
    {
        if (finished)
            return static_cast<base_type*>(this)->EndObject(len);
        --total_depth;
        return finish_when_depth_is_right(internal_handler.EndObject(len));
    }

    bool EndArray(SizeType len)
    {
        if (finished)
            return static_cast<base_type*>(this)->EndArray(len);
        --total_depth;
        return finish_when_depth_is_right(internal_handler.EndArray(len));
    }

    bool HasError() const
    {
        if (finished)
            return static_cast<base_type*>(this)->HasError();
        return internal_handler.HasError();
    }

    bool ReapError(error::ErrorStack& errs)
    {
        if (finished)
            return static_cast<base_type*>(this)->ReapError(errs);

        if (internal_handler.HasError()) {
            utility::scoped_ptr<error::ErrorBase> guard(new error::ArrayElementError(index));
            errs.push(guard.release());
            internal_handler.ReapError(errs);
            return true;
        }
        return false;
    }

    void PrepareForReuse()
    {
        total_depth = 0;
        finished = false;
        internal_handler.PrepareForReuse();
        static_cast<base_type*>(this)->PrepareForReuse();
    }
};

template <class Tuple, std::size_t TupleSize>
class TupleSAXEventHandler<Tuple, TupleSize, TupleSize> {
protected:
public:
    explicit TupleSAXEventHandler(Tuple*)
    {
    }

    bool Null()
    {
        return true;
    }

    bool Bool(bool)
    {
        return true;
    }

    bool Int(int)
    {
        return true;
    }

    bool Uint(unsigned)
    {
        return true;
    }

    bool Int64(utility::int64_t)
    {
        return true;
    }

    bool Uint64(utility::uint64_t)
    {
        return true;
    }

    bool Double(double)
    {
        return true;
    }

    bool String(const char*, SizeType, bool)
    {
        return true;
    }

    bool Key(const char*, SizeType, bool)
    {
        return true;
    }

    bool StartObject()
    {
        return true;
    }

    bool StartArray()
    {
        return true;
    }

    bool EndObject(SizeType)
    {
        return true;
    }

    bool EndArray(SizeType)
    {
        return true;
    }

    bool HasError() const
    {
        return false;
    }

    bool ReapError(error::ErrorStack&)
    {
        return false;
    }

    void PrepareForReuse()
    {
    }
};

template <class... Args>
class SAXEventHandler<std::tuple<Args...> >
    : public TupleSAXEventHandler<std::tuple<Args...>, sizeof...(Args), 0> {
private:
    int array_depth;

public:
    typedef std::tuple<Args...> tuple_type;
    typedef TupleSAXEventHandler<std::tuple<Args...>, sizeof...(Args), 0> base_type;

    explicit SAXEventHandler(tuple_type* t)
        : base_type(t)
        , array_depth(0)
    {
    }

    bool StartArray()
    {
        if (++array_depth > 1)
            return static_cast<base_type*>(this)->StartArray();
        return true;
    }

    bool EndArray(SizeType len)
    {
        if (array_depth-- > 1)
            return static_cast<base_type*>(this)->EndArray(len);
        return true;
    }

    void PrepareForReuse()
    {
        array_depth = 0;
        static_cast<base_type*>(this)->PrepareForReuse();
    }
};
}
#endif
#endif
