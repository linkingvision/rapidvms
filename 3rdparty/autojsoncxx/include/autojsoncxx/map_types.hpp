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

#ifndef AUTOJSONCXX_MAP_TYPES_HPP_29A4C106C1B1
#define AUTOJSONCXX_MAP_TYPES_HPP_29A4C106C1B1

#include <autojsoncxx/utility.hpp>
#include <autojsoncxx/base.hpp>
#include <autojsoncxx/error.hpp>

#include <map>
#include <stack>
#include <cassert>
#include <string>

#if AUTOJSONCXX_HAS_MODERN_TYPES
#include <unordered_map>
#endif

namespace autojsoncxx {

template <class ElementType, class Derived>
class MapBaseSAXEventHandler {
private:
    std::string key;
    ElementType value;

    SAXEventHandler<ElementType> internal_handler;
    utility::scoped_ptr<error::ErrorBase> the_error;
    utility::stack<signed char, 32> state;
    // A stack of StartArray() and StartObject() event
    // must be recorded, so we know when the current
    // element has been fully parsed, and needs to be
    // pushed back into the container

    bool emplace_when_time_is_right()
    {
        if (state.size() == 1 && state.top() == internal::OBJECT) {
            if (!static_cast<Derived*>(this)->Emplace(key, AUTOJSONCXX_MOVE_IF_NOEXCEPT(value))) {
                the_error.reset(new error::DuplicateKeyError(AUTOJSONCXX_MOVE(key)));
                return false;
            }

            value = ElementType();
            internal_handler.PrepareForReuse();
        }
        return true;
    }

    bool check_depth(const char* type)
    {
        if (state.empty()) {
            the_error.reset(new error::TypeMismatchError("object", type));
            return false;
        }
        return true;
    }

    bool checked_event_forwarding(bool success)
    {
        if (success)
            return emplace_when_time_is_right();

        set_member_error();
        return false;
    }

    void set_member_error()
    {
        this->the_error.reset(new error::ObjectMemberError(key));
    }

public:
    explicit MapBaseSAXEventHandler()
        : key()
        , value()
        , internal_handler(&value)
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
        if (state.size() > 1)
            return checked_event_forwarding(internal_handler.Key(str, length, copy));

        key.assign(str, length);
        return true;
    }

    bool StartArray()
    {
        if (!check_depth("array"))
            return false;
        state.push(internal::ARRAY);
        return checked_event_forwarding(internal_handler.StartArray());
    }

    bool EndArray(SizeType length)
    {
        assert(state.top() == internal::ARRAY);
        state.pop();
        return check_depth("array") && checked_event_forwarding(internal_handler.EndArray(length));
    }

    bool StartObject()
    {
        state.push(internal::OBJECT);
        if (state.size() > 1)
            return checked_event_forwarding(internal_handler.StartObject());
        return true;
    }

    bool EndObject(SizeType length)
    {
        assert(state.top() == internal::OBJECT);
        state.pop();
        if (!state.empty())
            return checked_event_forwarding(internal_handler.EndObject(length));
        return true;
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

template <class Writer, class MapType, class ElementType, class ConstIteratorType>
struct MapSerializer {
    void operator()(Writer& w, const MapType& map) const
    {
        w.StartObject();

        for (ConstIteratorType it = map.begin(), end = map.end(); it != end; ++it) {
            w.Key(it->first.data(), static_cast<SizeType>(it->first.size()), true);
            Serializer<Writer, ElementType>()(w, it->second);
        }

        w.EndObject(static_cast<SizeType>(map.size()));
    }
};

template <class ElementType, class Compare, class Allocator>
class SAXEventHandler<std::map<std::string, ElementType, Compare, Allocator> >
    : public MapBaseSAXEventHandler<ElementType,
                                    SAXEventHandler<std::map<std::string,
                                                             ElementType, Compare, Allocator> > > {
private:
    typedef std::map<std::string, ElementType, Compare, Allocator> map_type;

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

template <class Writer, class ElementType, class Compare, class Allocator>
struct Serializer<Writer, std::map<std::string, ElementType, Compare, Allocator> >
    : public MapSerializer<Writer, std::map<std::string, ElementType, Compare, Allocator>,
                           ElementType, typename std::map<std::string, ElementType, Compare, Allocator>::const_iterator> {
};

template <class ElementType, class Compare, class Allocator>
class SAXEventHandler<std::multimap<std::string, ElementType, Compare, Allocator> >
    : public MapBaseSAXEventHandler<ElementType,
                                    SAXEventHandler<std::multimap<std::string,
                                                                  ElementType, Compare, Allocator> > > {
private:
    typedef std::multimap<std::string, ElementType, Compare, Allocator> map_type;

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

template <class Writer, class ElementType, class Compare, class Allocator>
struct Serializer<Writer, std::multimap<std::string, ElementType, Compare, Allocator> >
    : public MapSerializer<Writer, std::multimap<std::string, ElementType, Compare, Allocator>,
                           ElementType, typename std::multimap<std::string, ElementType, Compare, Allocator>::const_iterator> {
};

#if AUTOJSONCXX_HAS_MODERN_TYPES

template <class ElementType, class Hash, class Equal, class Allocator>
class SAXEventHandler<std::unordered_map<std::string, ElementType, Hash, Equal, Allocator> >
    : public MapBaseSAXEventHandler<ElementType,
                                    SAXEventHandler<std::unordered_map<std::string, ElementType, Hash, Equal, Allocator> > > {
private:
    typedef std::unordered_map<std::string, ElementType, Hash, Equal, Allocator> map_type;

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
struct Serializer<Writer, std::unordered_map<std::string, ElementType, Hash, Equal, Allocator> >
    : public MapSerializer<Writer, std::unordered_map<std::string, ElementType, Hash, Equal, Allocator>, ElementType, typename std::unordered_map<std::string, ElementType, Hash, Equal, Allocator>::const_iterator> {
};

template <class ElementType, class Hash, class Equal, class Allocator>
class SAXEventHandler<std::unordered_multimap<std::string, ElementType, Hash, Equal, Allocator> >
    : public MapBaseSAXEventHandler<ElementType,
                                    SAXEventHandler<std::unordered_multimap<std::string, ElementType, Hash, Equal, Allocator> > > {
private:
    typedef std::unordered_multimap<std::string, ElementType, Hash, Equal, Allocator> map_type;

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
struct Serializer<Writer, std::unordered_multimap<std::string, ElementType, Hash, Equal, Allocator> >
    : public MapSerializer<Writer, std::unordered_multimap<std::string, ElementType, Hash, Equal, Allocator>, ElementType, typename std::unordered_multimap<std::string, ElementType, Hash, Equal, Allocator>::const_iterator> {
};

#endif
}

#endif
