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

#ifndef AUTOJSONCXX_ARRAY_TYPES_HPP_29A4C106C1B1
#define AUTOJSONCXX_ARRAY_TYPES_HPP_29A4C106C1B1

#include <autojsoncxx/base.hpp>
#include <autojsoncxx/error.hpp>

#include <vector>
#include <deque>
#include <stack>
#include <cassert>
#include <iterator>

#if AUTOJSONCXX_HAS_MODERN_TYPES
#include <array>
#endif

namespace autojsoncxx {

template <class ElementType, class Derived>
class VectorBaseSAXEventHandler {
private:
    ElementType current;
    SAXEventHandler<ElementType> internal_handler;
    utility::scoped_ptr<error::ErrorBase> the_error;
    utility::stack<signed char, 32> state;
    // A stack of StartArray() and StartObject() event
    // must be recorded, so we know when the current
    // element has been fully parsed, and needs to be
    // pushed back into the container

    void push_when_time_is_right()
    {
        if (state.size() == 1 && state.top() == internal::ARRAY) {
            static_cast<Derived*>(this)->Push(AUTOJSONCXX_MOVE_IF_NOEXCEPT(current));
            current = ElementType();
            internal_handler.PrepareForReuse();
        }
    }

    bool check_array_length(SizeType length)
    {
        if (state.size() == 1 && state.top() == internal::ARRAY
            && !static_cast<Derived*>(this)->CheckLength(length)) {
            this->the_error.reset(new error::ArrayLengthMismatchError(static_cast<Derived*>(this)->ExpectedLength(), length));
            return false;
        }
        return true;
    }

    bool check_depth(const char* type)
    {
        if (state.empty()) {
            the_error.reset(new error::TypeMismatchError("array", type));
            return false;
        }
        return true;
    }

    bool checked_event_forwarding(bool success)
    {
        if (success) {
            push_when_time_is_right();
            return true;
        }

        set_element_error();
        return false;
    }

    void set_element_error()
    {
        this->the_error.reset(new error::ArrayElementError(static_cast<Derived*>(this)->GetCurrentSize()));
    }

public:
    explicit VectorBaseSAXEventHandler()
        : current()
        , internal_handler(&current)
    {
    }

    bool Null()
    {
        return check_depth("null") && checked_event_forwarding(internal_handler.Null());
    }

    bool Bool(bool b)
    {
        return check_depth("bool") && checked_event_forwarding(internal_handler.Bool(b));
    }

    bool Int(int i)
    {
        return check_depth("int") && checked_event_forwarding(internal_handler.Int(i));
    }

    bool Uint(unsigned i)
    {
        return check_depth("unsigned") && checked_event_forwarding(internal_handler.Uint(i));
    }

    bool Int64(utility::int64_t i)
    {
        return check_depth("int64_t") && checked_event_forwarding(internal_handler.Int64(i));
    }

    bool Uint64(utility::uint64_t i)
    {
        return check_depth("uint64_t") && checked_event_forwarding(internal_handler.Uint64(i));
    }

    bool Double(double d)
    {
        return check_depth("double") && checked_event_forwarding(internal_handler.Double(d));
    }

    bool String(const char* str, SizeType length, bool copy)
    {
        return check_depth("string") && checked_event_forwarding(internal_handler.String(str, length, copy));
    }

    bool Key(const char* str, SizeType length, bool copy)
    {
        return check_depth("object") && checked_event_forwarding(internal_handler.Key(str, length, copy));
    }

    bool StartArray()
    {
        state.push(internal::ARRAY);
        if (state.size() > 1 && !internal_handler.StartArray()) {
            set_element_error();
            return false;
        }
        return true;
    }

    bool EndArray(SizeType length)
    {
        assert(state.top() == internal::ARRAY);

        // When depth > 1, this event should be forwarded to the element
        if (state.size() > 1 && !internal_handler.EndArray(length)) {
            set_element_error();
            return false;
        }

        if (!check_array_length(length))
            return false;

        // When depth == 2, this event will close the underlying element
        // and it should be pushed backed into the container
        state.pop();
        push_when_time_is_right();

        return true;
    }

    bool StartObject()
    {
        if (!check_depth("object"))
            return false;
        state.push(internal::OBJECT);
        return checked_event_forwarding(internal_handler.StartObject());
    }

    bool EndObject(SizeType length)
    {
        assert(state.top() == internal::OBJECT);
        state.pop();
        return check_depth("object") && checked_event_forwarding(internal_handler.EndObject(length));
    }

    bool HasError() const
    {
        return !this->the_error.empty();
    }

    bool ReapError(error::ErrorStack& errs)
    {
        if (this->the_error.empty())
            return false;

        errs.push(this->the_error.release());
        internal_handler.ReapError(errs);
        return true;
    }

    void PrepareForReuse()
    {
        the_error.reset();
        state.clear();
        internal_handler.PrepareForReuse();
    }
};

template <class T, class Allocator>
class SAXEventHandler<std::vector<T, Allocator> >
    : public VectorBaseSAXEventHandler<T, SAXEventHandler<std::vector<T, Allocator> > > {

public:
    typedef std::vector<T, Allocator> vector_type;
    typedef VectorBaseSAXEventHandler<T, SAXEventHandler<std::vector<T, Allocator> > > base_type;

private:
    vector_type* m_value;

public:
    explicit SAXEventHandler(vector_type* v)
        : m_value(v)
    {
    }

    void Push(const T& c)
    {
        m_value->push_back(c);
    }

#if AUTOJSONCXX_HAS_RVALUE

    void Push(T&& c)
    {
        m_value->push_back(AUTOJSONCXX_MOVE(c));
    }

#endif

    bool CheckLength(SizeType) const
    {
        return true;
    }

    size_t ExpectedLength() const
    {
        return 0;
    }

    size_t GetCurrentSize() const
    {
        return m_value->size();
    }
};

template <class T, class Allocator>
class SAXEventHandler<std::deque<T, Allocator> >
    : public VectorBaseSAXEventHandler<T, SAXEventHandler<std::deque<T, Allocator> > > {

public:
    typedef std::deque<T, Allocator> vector_type;
    typedef VectorBaseSAXEventHandler<T, SAXEventHandler<std::deque<T, Allocator> > > base_type;

private:
    vector_type* m_value;

public:
    explicit SAXEventHandler(vector_type* v)
        : m_value(v)
    {
    }

    void Push(const T& c)
    {
        m_value->push_back(c);
    }

#if AUTOJSONCXX_HAS_RVALUE

    void Push(T&& c)
    {
        m_value->push_back(AUTOJSONCXX_MOVE(c));
    }

#endif

    bool CheckLength(SizeType) const
    {
        return true;
    }

    size_t ExpectedLength() const
    {
        return 0;
    }

    size_t GetCurrentSize() const
    {
        return m_value->size();
    }
};

template <class Writer, class Container, class ValueType, class ConstIteratorType>
struct ContainerSerializer {
    void operator()(Writer& w, const Container& con) const
    {
        w.StartArray();
        for (ConstIteratorType it = con.begin(), end = con.end(); it != end; ++it)
            Serializer<Writer, ValueType>()(w, *it);
        w.EndArray(static_cast<SizeType>(std::distance(con.begin(), con.end())));
    }
};

template <class Writer, class T, class Allocator>
struct Serializer<Writer, std::vector<T, Allocator> >
    : public ContainerSerializer<Writer, std::vector<T, Allocator>,
                                 typename std::vector<T, Allocator>::value_type,
                                 typename std::vector<T, Allocator>::const_iterator> {
};

template <class Writer, class T, class Allocator>
struct Serializer<Writer, std::deque<T, Allocator> >
    : public ContainerSerializer<Writer, std::deque<T, Allocator>,
                                 typename std::deque<T, Allocator>::value_type,
                                 typename std::deque<T, Allocator>::const_iterator> {
};

#if AUTOJSONCXX_HAS_MODERN_TYPES
template <class T, size_t N>
class SAXEventHandler<std::array<T, N> >
    : public VectorBaseSAXEventHandler<T, SAXEventHandler<std::array<T, N> > > {

public:
    typedef std::array<T, N> vector_type;
    typedef VectorBaseSAXEventHandler<T, SAXEventHandler<std::array<T, N> > > base_type;

private:
    vector_type* m_value;
    size_t index;

public:
    explicit SAXEventHandler(vector_type* v)
        : m_value(v)
        , index(0)
    {
    }

    void Push(const T& c)
    {
        (*m_value)[index] = c;
        index = (index + 1) % N;
    }

#if AUTOJSONCXX_HAS_RVALUE

    void Push(T&& c)
    {
        (*m_value)[index] = AUTOJSONCXX_MOVE(c);
        index = (index + 1) % N;
    }

#endif

    bool CheckLength(SizeType length) const
    {
        return length == N;
    }

    size_t ExpectedLength() const
    {
        return N;
    }

    size_t GetCurrentSize() const
    {
        return index;
    }
};

template <class Writer, class T, std::size_t N>
struct Serializer<Writer, std::array<T, N> >
    : public ContainerSerializer<Writer, std::array<T, N>,
                                 typename std::array<T, N>::value_type,
                                 typename std::array<T, N>::const_iterator> {
};

#endif
}
#endif
