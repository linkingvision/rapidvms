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

#ifndef AUTOJSONCXX_BOOST_TYPES_HPP_29A4C106C1B1
#define AUTOJSONCXX_BOOST_TYPES_HPP_29A4C106C1B1

#include <autojsoncxx/autojsoncxx.hpp>

#include <boost/container/vector.hpp>
#include <boost/container/deque.hpp>
#include <boost/array.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/optional.hpp>
#include <boost/utility/in_place_factory.hpp>
#include <boost/unordered_map.hpp>

namespace autojsoncxx {
template <class T, class Allocator>
class SAXEventHandler<boost::container::vector<T, Allocator> >
    : public VectorBaseSAXEventHandler<T, SAXEventHandler<boost::container::vector<T, Allocator> > > {

public:
    typedef boost::container::vector<T, Allocator> vector_type;
    typedef VectorBaseSAXEventHandler<T, SAXEventHandler<boost::container::vector<T, Allocator> > > base_type;

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

#ifdef AUTOJSONCXX_HAS_RVALUE

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

template <class Writer, class T, class Allocator>
struct Serializer<Writer, boost::container::vector<T, Allocator> >
    : public ContainerSerializer<Writer, boost::container::vector<T, Allocator>,
                                 typename boost::container::vector<T, Allocator>::value_type,
                                 typename boost::container::vector<T, Allocator>::const_iterator> {
};

template <class T, class Allocator>
class SAXEventHandler<boost::container::deque<T, Allocator> >
    : public VectorBaseSAXEventHandler<T, SAXEventHandler<boost::container::deque<T, Allocator> > > {

public:
    typedef boost::container::deque<T, Allocator> vector_type;
    typedef VectorBaseSAXEventHandler<T, SAXEventHandler<boost::container::deque<T, Allocator> > > base_type;

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

#ifdef AUTOJSONCXX_HAS_RVALUE

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

template <class Writer, class T, class Allocator>
struct Serializer<Writer, boost::container::deque<T, Allocator> >
    : public ContainerSerializer<Writer, boost::container::deque<T, Allocator>,
                                 typename boost::container::deque<T, Allocator>::value_type,
                                 typename boost::container::deque<T, Allocator>::const_iterator> {
};

template <class T, size_t N>
class SAXEventHandler<boost::array<T, N> >
    : public VectorBaseSAXEventHandler<T, SAXEventHandler<boost::array<T, N> > > {

public:
    typedef boost::array<T, N> vector_type;
    typedef VectorBaseSAXEventHandler<T, SAXEventHandler<boost::array<T, N> > > base_type;

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

#ifdef AUTOJSONCXX_HAS_RVALUE

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
struct Serializer<Writer, boost::array<T, N> >
    : public ContainerSerializer<Writer, boost::array<T, N>,
                                 typename boost::array<T, N>::value_type,
                                 typename boost::array<T, N>::const_iterator> {
};

template <class T>
class SAXEventHandler<boost::shared_ptr<T> >
    : public NullableBaseSAXEventHandler<T, SAXEventHandler<boost::shared_ptr<T> > > {

public:
    typedef boost::shared_ptr<T> smart_pointer_type;

private:
    smart_pointer_type* m_value;

public:
    explicit SAXEventHandler(smart_pointer_type* v)
        : m_value(v)
    {
    }

    bool IsNull() const
    {
        return m_value->get() == 0;
    }

    T* Initialize()
    {
        boost::make_shared<T>().swap(*m_value);
        return m_value->get();
    }

    void SetNull()
    {
        m_value->reset();
    }
};

template <class Writer, class T>
struct Serializer<Writer, boost::shared_ptr<T> > {
    void operator()(Writer& w, const boost::shared_ptr<T>& ptr) const
    {
        if (!ptr)
            w.Null();
        else
            Serializer<Writer, T>()(w, *ptr);
    }
};

namespace utility {
    namespace traits {
        template <class T>
        struct is_simple_type<boost::shared_ptr<T> > : public is_simple_type<T> {
        };
    }
}

template <class T>
class SAXEventHandler<boost::optional<T> > {
private:
    typedef boost::optional<T> optional_type;
    typedef boost::optional<SAXEventHandler<T> > internal_handler_type;

private:
    optional_type* m_value;

    internal_handler_type internal_handler;
    int depth;

    void initialize()
    {
        if (m_value->get_ptr() == 0) {
            *m_value = T();
            internal_handler = boost::in_place(m_value->get_ptr());
        }
    }

    bool checked_event_forwarding(bool success)
    {
        return success;
    }

public:
    explicit SAXEventHandler(optional_type* v)
        : m_value(v)
        , depth(0)
    {
    }

    bool Null()
    {
        if (depth == 0) {
            *m_value = boost::none;
            return true;
        } else {
            initialize();
            return checked_event_forwarding(internal_handler->Null());
        }
    }

    bool Bool(bool b)
    {
        initialize();
        return checked_event_forwarding(internal_handler->Bool(b));
    }

    bool Int(int i)
    {
        initialize();
        return checked_event_forwarding(internal_handler->Int(i));
    }

    bool Uint(unsigned i)
    {
        initialize();
        return checked_event_forwarding(internal_handler->Uint(i));
    }

    bool Int64(utility::int64_t i)
    {
        initialize();
        return checked_event_forwarding(internal_handler->Int64(i));
    }

    bool Uint64(utility::uint64_t i)
    {
        initialize();
        return checked_event_forwarding(internal_handler->Uint64(i));
    }

    bool Double(double i)
    {
        initialize();
        return checked_event_forwarding(internal_handler->Double(i));
    }

    bool String(const char* str, SizeType len, bool copy)
    {
        initialize();
        return checked_event_forwarding(internal_handler->String(str, len, copy));
    }

    bool Key(const char* str, SizeType len, bool copy)
    {
        initialize();
        return checked_event_forwarding(internal_handler->Key(str, len, copy));
    }

    bool StartObject()
    {
        initialize();
        ++depth;
        return internal_handler->StartObject();
    }

    bool EndObject(SizeType len)
    {
        initialize();
        --depth;
        return checked_event_forwarding(internal_handler->EndObject(len));
    }

    bool StartArray()
    {
        initialize();
        ++depth;
        return internal_handler->StartArray();
    }

    bool EndArray(SizeType len)
    {
        initialize();
        --depth;
        return checked_event_forwarding(internal_handler->EndArray(len));
    }

    bool HasError() const
    {
        return internal_handler.get_ptr() && internal_handler->HasError();
    }

    bool ReapError(error::ErrorStack& errs)
    {
        if (!internal_handler)
            return false;
        return internal_handler->ReapError(errs);
    }

    void PrepareForReuse()
    {
        internal_handler = boost::none;
        depth = 0;
    }
};

template <class Writer, class T>
struct Serializer<Writer, boost::optional<T> > {
    void operator()(Writer& w, const boost::optional<T>& value) const
    {
        if (!value)
            w.Null();
        else
            Serializer<Writer, T>()(w, *value);
    }
};

template <class ElementType, class Hash, class Equal, class Allocator>
class SAXEventHandler<boost::unordered_map<std::string, ElementType, Hash, Equal, Allocator> >
    : public MapBaseSAXEventHandler<ElementType,
                                    SAXEventHandler<boost::unordered_map<std::string, ElementType, Hash, Equal, Allocator> > > {
private:
    typedef boost::unordered_map<std::string, ElementType, Hash, Equal, Allocator> map_type;

    map_type* m_value;

public:
    explicit SAXEventHandler(map_type* v)
        : m_value(v)
    {
    }

    bool Emplace(const std::string& key, const ElementType& value)
    {
        return m_value->insert(std::make_pair(key, value)).second;
    }

#if AUTOJSONCXX_HAS_RVALUE

    bool Emplace(const std::string& key, ElementType&& value)
    {
        return m_value->insert(std::make_pair(AUTOJSONCXX_MOVE(key), AUTOJSONCXX_MOVE(value))).second;
    }

#endif
};

template <class Writer, class ElementType, class Hash, class Equal, class Allocator>
struct Serializer<Writer, boost::unordered_map<std::string, ElementType, Hash, Equal, Allocator> >
    : public MapSerializer<Writer, boost::unordered_map<std::string, ElementType, Hash, Equal, Allocator>, ElementType, typename boost::unordered_map<std::string, ElementType, Hash, Equal, Allocator>::const_iterator> {
};

template <class ElementType, class Hash, class Equal, class Allocator>
class SAXEventHandler<boost::unordered_multimap<std::string, ElementType, Hash, Equal, Allocator> >
    : public MapBaseSAXEventHandler<ElementType,
                                    SAXEventHandler<boost::unordered_multimap<std::string, ElementType, Hash, Equal, Allocator> > > {
private:
    typedef boost::unordered_multimap<std::string, ElementType, Hash, Equal, Allocator> map_type;

    map_type* m_value;

public:
    explicit SAXEventHandler(map_type* v)
        : m_value(v)
    {
    }

    bool Emplace(const std::string& key, const ElementType& value)
    {
        return m_value->insert(std::make_pair(key, value)).second;
    }

#if AUTOJSONCXX_HAS_RVALUE

    bool Emplace(const std::string& key, ElementType&& value)
    {
        return m_value->insert(std::make_pair(AUTOJSONCXX_MOVE(key), AUTOJSONCXX_MOVE(value))).second;
    }

#endif
};

template <class Writer, class ElementType, class Hash, class Equal, class Allocator>
struct Serializer<Writer, boost::unordered_multimap<std::string, ElementType, Hash, Equal, Allocator> >
    : public MapSerializer<Writer, boost::unordered_multimap<std::string, ElementType, Hash, Equal, Allocator>, ElementType, typename boost::unordered_multimap<std::string, ElementType, Hash, Equal, Allocator>::const_iterator> {
};
}

#endif
