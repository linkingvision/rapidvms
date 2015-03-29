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

#ifndef AUTOJSONCXX_UTILITY_HPP_29A4C106C1B1
#define AUTOJSONCXX_UTILITY_HPP_29A4C106C1B1

#include <string>
#include <cstring>
#include <cstddef>
#include <utility>
#include <algorithm>
#include <cstdio>
#include <cctype>
#include <cassert>

#if AUTOJSONCXX_MODERN_COMPILER
#define AUTOJSONCXX_HAS_MODERN_TYPES 1
#define AUTOJSONCXX_HAS_RVALUE 1
#define AUTOJSONCXX_HAS_NOEXCEPT 1
#define AUTOJSONCXX_HAS_VARIADIC_TEMPLATE 1
#define AUTOJSONCXX_HAS_EXPLICIT_OPERATOR 1
#endif

#if AUTOJSONCXX_HAS_RVALUE
#define AUTOJSONCXX_MOVE(x) std::move(x)
#else
#define AUTOJSONCXX_MOVE(x) (x)
#endif

#if AUTOJSONCXX_HAS_NOEXCEPT
#define AUTOJSONCXX_NOEXCEPT noexcept
#define AUTOJSONCXX_MOVE_IF_NOEXCEPT(x) std::move_if_noexcept(x)
#else
#define AUTOJSONCXX_NOEXCEPT
#define AUTOJSONCXX_MOVE_IF_NOEXCEPT(x) AUTOJSONCXX_MOVE(x)
#endif

namespace autojsoncxx {
namespace utility {

    typedef unsigned int SizeType;
    typedef long long int64_t;
    typedef unsigned long long uint64_t;

    const std::size_t default_buffer_size = 256;

    namespace traits {
        struct true_type {
            static const bool value = true;
        };

        struct false_type {
            static const bool value = false;
        };

        template <class T>
        struct is_simple_type : public false_type {
        };

        template <>
        struct is_simple_type<bool> : public true_type {
        };

        template <>
        struct is_simple_type<char> : public true_type {
        };

        template <>
        struct is_simple_type<int> : public true_type {
        };

        template <>
        struct is_simple_type<unsigned> : public true_type {
        };

        template <>
        struct is_simple_type<utility::int64_t> : public true_type {
        };

        template <>
        struct is_simple_type<utility::uint64_t> : public true_type {
        };

        template <>
        struct is_simple_type<std::string> : public true_type {
        };
    }

    template <class T>
    struct default_deleter {
        void operator()(T* ptr) const
        {
            delete ptr;
        }
    };

    struct file_closer {
        void operator()(std::FILE* fp) const
        {
            if (fp)
                std::fclose(fp);
        }
    };

    template <class T, class Deleter = default_deleter<T> >
    class scoped_ptr {
    private:
        T* ptr;

        scoped_ptr(const scoped_ptr&);
        scoped_ptr& operator=(const scoped_ptr&);

#if AUTOJSONCXX_HAS_RVALUE
        scoped_ptr(scoped_ptr&&);
        scoped_ptr& operator==(scoped_ptr&&);
#endif

    public:
        typedef T element_type;
        typedef T* pointer_type;
        typedef T& reference_type;

        explicit scoped_ptr()
            : ptr(0)
        {
        }

        explicit scoped_ptr(pointer_type t)
            : ptr(t)
        {
        }

        ~scoped_ptr()
        {
            Deleter()(ptr);
        }

        pointer_type get() const AUTOJSONCXX_NOEXCEPT
        {
            return ptr;
        }

        pointer_type operator->() const AUTOJSONCXX_NOEXCEPT
        {
            return ptr;
        }

        pointer_type release() AUTOJSONCXX_NOEXCEPT
        {
            T* result = ptr;
            ptr = 0;
            return result;
        }

        reference_type operator*() const
        {
            return *ptr;
        }

        void reset(pointer_type p = 0)
        {
            Deleter()(ptr);
            ptr = p;
        }

        void swap(scoped_ptr& that) AUTOJSONCXX_NOEXCEPT
        {
            std::swap(ptr, that.ptr);
        }

        bool empty() const AUTOJSONCXX_NOEXCEPT
        {
            return ptr == 0;
        }
    };

    inline bool string_equal(const char* str1, std::size_t len1, const char* str2, std::size_t len2)
    {
        return len1 == len2 && std::equal(str1, str1 + len1, str2);
    }

    // Adapted from Jettison's implementation (http://jettison.codehaus.org/)
    // Original copyright (compatible with MIT):

    // Copyright 2006 Envoi Solutions LLC
    //
    // Licensed under the Apache License, Version 2.0 (the "License");
    // you may not use this file except in compliance with the License.
    // You may obtain a copy of the License at
    //
    //     http://www.apache.org/licenses/LICENSE-2.0
    //
    // Unless required by applicable law or agreed to in writing, software
    // distributed under the License is distributed on an "AS IS" BASIS,
    // WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    // See the License for the specific language governing permissions and
    // limitations under the License.
    inline std::string quote(const std::string& str)
    {
        std::string sb;
        sb.reserve(str.size() + 8);
        sb += '\"';

        typedef std::string::const_iterator iterator;

        for (iterator it = str.begin(), end = str.end(); it != end; ++it) {
            char c = *it;
            switch (c) {
            case '\\':
            case '"':
                sb += '\\';
                sb += c;
                break;
            case '\b':
                sb.append("\\b", 2);
                break;
            case '\t':
                sb.append("\\t", 2);
                break;
            case '\n':
                sb.append("\\n", 2);
                break;
            case '\f':
                sb.append("\\f", 2);
                break;
            case '\r':
                sb.append("\\r", 2);
                break;
            case '\x00':
                sb.append("\\x00", 4);
                break;
            case '\x01':
                sb.append("\\x01", 4);
                break;
            case '\x02':
                sb.append("\\x02", 4);
                break;
            case '\x03':
                sb.append("\\x03", 4);
                break;
            case '\x04':
                sb.append("\\x04", 4);
                break;
            case '\x05':
                sb.append("\\x05", 4);
                break;
            case '\x06':
                sb.append("\\x06", 4);
                break;
            case '\x07':
                sb.append("\\x07", 4);
                break;
            case '\x0b':
                sb.append("\\x0b", 4);
                break;
            case '\x0e':
                sb.append("\\x0e", 4);
                break;
            case '\x0f':
                sb.append("\\x0f", 4);
                break;
            case '\x10':
                sb.append("\\x10", 4);
                break;
            case '\x11':
                sb.append("\\x11", 4);
                break;
            case '\x12':
                sb.append("\\x12", 4);
                break;
            case '\x13':
                sb.append("\\x13", 4);
                break;
            case '\x14':
                sb.append("\\x14", 4);
                break;
            case '\x15':
                sb.append("\\x15", 4);
                break;
            case '\x16':
                sb.append("\\x16", 4);
                break;
            case '\x17':
                sb.append("\\x17", 4);
                break;
            case '\x18':
                sb.append("\\x18", 4);
                break;
            case '\x19':
                sb.append("\\x19", 4);
                break;
            case '\x1a':
                sb.append("\\x1a", 4);
                break;
            case '\x1b':
                sb.append("\\x1b", 4);
                break;
            case '\x1c':
                sb.append("\\x1c", 4);
                break;
            case '\x1d':
                sb.append("\\x1d", 4);
                break;
            case '\x1e':
                sb.append("\\x1e", 4);
                break;
            case '\x1f':
                sb.append("\\x1f", 4);
                break;
            default:
                sb += c;
            }
        }
        sb += '\"';
        return sb;
    }

    // The standard std::stack is insufficient because it cannot handle noncopyable types in c++03
    template <class T, std::size_t num_elements_per_node>
    class stack {
    private:
        // The node is always allocated with new, so no need for alignment tuning
        struct node {
            char raw_storage[sizeof(T) * num_elements_per_node];
            node* next;
        };

        node* head;
        std::size_t current_size;
        std::size_t total_size;

    private:
        void deallocate_current_node()
        {
            for (std::size_t i = 0; i < current_size; ++i)
                reinterpret_cast<T*>(head->raw_storage + sizeof(T) * i)->~T();

            current_size = num_elements_per_node;
            node* next = head->next;
            operator delete(head);
            head = next;
        }

    private:
        stack(const stack&);
        stack& operator=(const stack&);

    public:
        explicit stack()
            : head(0)
            , current_size(num_elements_per_node)
            , total_size(0)
        {
        }

        T& emplace()
        {
            if (current_size == num_elements_per_node) {
                // operator new always return memory maximumly aligned
                node* new_node = static_cast<node*>(operator new(sizeof(*new_node)));
                new_node->next = head;
                head = new_node;
                current_size = 0;
            }
            T* result = new (head->raw_storage + sizeof(T) * current_size) T();
            ++current_size;
            ++total_size;
            return *result;
        }

        void push(const T& value)
        {
            emplace() = value;
        }

        void push(T& value)
        {
            emplace() = value;
        }

        T& top()
        {
            return const_cast<T&>(static_cast<const stack<T, num_elements_per_node>*>(this)->top());
        }

        const T& top() const
        {
            assert(!empty());

            // The below code triggers a warning about strict aliasing in some versions of gcc
            // That is a false positive, because character type is allowed to alias any type
            if (current_size > 0)
                return *reinterpret_cast<T*>(head->raw_storage + sizeof(T) * (current_size - 1));
            else
                return *reinterpret_cast<T*>(head->next->raw_storage + sizeof(T) * (num_elements_per_node - 1));
        }

        void pop()
        {
            assert(!empty());
            if (current_size == 0)
                deallocate_current_node();
            top().~T();
            --current_size;
            --total_size;
        }

        void clear()
        {
            while (head)
                deallocate_current_node();
            head = 0;
        }

        bool empty() const AUTOJSONCXX_NOEXCEPT
        {
            return total_size == 0;
        }

        std::size_t size() const AUTOJSONCXX_NOEXCEPT
        {
            return total_size;
        }

        ~stack()
        {
            clear();
        }
    };
}

namespace internal {
    const signed char ARRAY = 0;
    const signed char OBJECT = -1;
}
}

#endif
