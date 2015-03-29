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

#ifndef AUTOJSONCXX_DOM_HPP_29A4C106C1B1
#define AUTOJSONCXX_DOM_HPP_29A4C106C1B1

#include <autojsoncxx/base.hpp>
#include <autojsoncxx/error.hpp>

#include <rapidjson/document.h>

#include <cassert>

namespace autojsoncxx {
template <class Encoding, class Allocator, class StackAllocator>
class SAXEventHandler<rapidjson::GenericDocument<Encoding, Allocator, StackAllocator> > {
public:
    typedef rapidjson::GenericDocument<Encoding, Allocator, StackAllocator> document_type;
    typedef rapidjson::GenericValue<Encoding, Allocator> value_type;
    typedef typename value_type::Ch char_type;

private:
    utility::stack<value_type, 32> working_stack;
    utility::scoped_ptr<error::ErrorBase> the_error;
    document_type* doc;

private:
    // Implicitly treat the document as the bottom of stack
    value_type* top_value()
    {
        if (working_stack.empty())
            return doc;
        return &working_stack.top();
    }

    bool set_dom_error(const char* message)
    {
        the_error.reset(new error::CorruptedDOMError(message));
        return false;
    }

    void pre_processing()
    {
        if (top_value()->IsArray())
            working_stack.emplace();
    }

    bool post_processing()
    {
        if (working_stack.empty())
            return true;

        value_type to_be_inserted;
        to_be_inserted.Swap(working_stack.top());
        working_stack.pop();

        if (!working_stack.empty() && working_stack.top().IsString()) {
            value_type key;
            key.Swap(working_stack.top());
            working_stack.pop();

            if (!top_value()->IsObject())
                return set_dom_error("DOM corrupted: Non-object types encountered where object is expected");
            top_value()->AddMember(key, to_be_inserted, doc->GetAllocator());

        } else if (top_value()->IsArray()) {
            top_value()->PushBack(to_be_inserted, doc->GetAllocator());
        } else
            return set_dom_error("DOM corrupted: a composite type neither array nor object encountered");

        return true;
    }

public:
    explicit SAXEventHandler(document_type* value)
        : doc(value)
    {
    }

    bool Null()
    {
        pre_processing();
        top_value()->SetNull();
        return post_processing();
    }

    bool Bool(bool b)
    {
        pre_processing();
        top_value()->SetBool(b);
        return post_processing();
    }

    bool Int(int i)
    {
        pre_processing();
        top_value()->SetInt(i);
        return post_processing();
    }

    bool Uint(unsigned i)
    {
        pre_processing();
        top_value()->SetUint(i);
        return post_processing();
    }

    bool Int64(utility::int64_t i)
    {
        pre_processing();
        top_value()->SetInt64(i);
        return post_processing();
    }

    bool Uint64(utility::uint64_t i)
    {
        pre_processing();
        top_value()->SetUint64(i);
        return post_processing();
    }

    bool Double(double d)
    {
        pre_processing();
        top_value()->SetDouble(d);
        return post_processing();
    }

    bool String(const char_type* str, SizeType length, bool copy)
    {
        pre_processing();
        if (copy)
            top_value()->SetString(str, length, doc->GetAllocator());
        else
            top_value()->SetString(str, length);
        return post_processing();
    }

    bool Key(const char_type* str, SizeType length, bool copy)
    {
        if (!top_value()->IsObject())
            return set_dom_error("DOM corrupted: object types required");

        value_type key;
        if (copy)
            key.SetString(str, length, doc->GetAllocator());
        else
            key.SetString(str, length);

        working_stack.push(key);

        // push a placeholder as the corresponding value
        working_stack.emplace();
        return true;
    }

    bool StartArray()
    {
        pre_processing();
        top_value()->SetArray();
        return true;
    }

    bool EndArray(SizeType)
    {
        return post_processing();
    }

    bool StartObject()
    {
        pre_processing();
        top_value()->SetObject();
        return true;
    }

    bool EndObject(SizeType)
    {
        return post_processing();
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
        return true;
    }

    void PrepareForReuse()
    {
        the_error.reset();
        working_stack.clear();
    }
};

template <class Writer, class Encoding, class Allocator, class StackAllocator>
struct Serializer<Writer, rapidjson::GenericDocument<Encoding, Allocator, StackAllocator> > {

    void operator()(Writer& w, const rapidjson::GenericDocument<Encoding, Allocator, StackAllocator>& doc) const
    {
        doc.template Accept<Writer>(w);
    }
};

template <class T, class Encoding, class Allocator, class StackAllocator>
void to_document(const T& value, rapidjson::GenericDocument<Encoding, Allocator, StackAllocator>& doc)
{
    doc.SetNull();

    typedef rapidjson::GenericDocument<Encoding, Allocator, StackAllocator> document_type;
    typedef SAXEventHandler<document_type> handler_type;
    handler_type handler(&doc);
    Serializer<handler_type, T>()(handler, value);
}

template <class T, class Encoding, class Allocator, class StackAllocator>
bool from_document(T& value, const rapidjson::GenericDocument<Encoding, Allocator, StackAllocator>& doc,
                   error::ErrorStack& errs)
{
    typedef SAXEventHandler<T> handler_type;

    handler_type handler(&value);
    doc.template Accept<handler_type>(handler);
    handler.ReapError(errs);
    return errs.empty();
}
}

#endif
