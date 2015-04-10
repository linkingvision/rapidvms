
/// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=--=-=-=-=-=-
/// cppkit - http://www.cppkit.org
/// Copyright (c) 2013, Tony Di Croce
/// All rights reserved.
///
/// Redistribution and use in source and binary forms, with or without modification, are permitted
/// provided that the following conditions are met:
///
/// 1. Redistributions of source code must retain the above copyright notice, this list of conditions and
///    the following disclaimer.
/// 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions
///    and the following disclaimer in the documentation and/or other materials provided with the
///    distribution.
///
/// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
/// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
/// PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
/// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
/// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
/// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
/// TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
/// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
///
/// The views and conclusions contained in the software and documentation are those of the authors and
/// should not be interpreted as representing official policies, either expressed or implied, of the cppkit
/// Project.
/// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=--=-=-=-=-=-

#ifndef cppkit_byte_ptr_h
#define cppkit_byte_ptr_h

#include "cppkit/os/ck_exports.h"
#include "cppkit/ck_exception.h"
#include "cppkit/ck_string.h"

#include <cstddef>

namespace cppkit
{

/// Tests whether pointer 'pos' is within the buffer bounds for 'len' bytes.
static bool _in_bounds(const uint8_t* start, size_t blen, const uint8_t* pos, size_t len)
{
    return (pos >= start) && ((pos + len) <= (start + blen));
}

/// casts the pointer to the specified pointer type.
/// @note Only works on pointer types (i.e. not concrete types like int, char, etc).
template<typename T>
struct _ck_byte_ptr_helper
{
    static T cast(const uint8_t* start, const uint8_t* current, size_t length)
    {
        T v = 0; // Required so we can get the size of the value *pointed* to
        if (!_in_bounds(start, length, current, sizeof(*v)))
        {
            CK_STHROW(ck_exception, (ck_string::format("casting to the specified type could access out-of-bounds memory. Start (%p), End (%p), Ptr (%p), Size (%llu)",
                                                  start, start + length, current, sizeof(*v))));
        }
        return (T)current;
    }
};

/// Specializes on void*, b/c we can't do a bounds check on this type.
template<>
struct _ck_byte_ptr_helper<const void*>
{
    static const void* cast(const uint8_t*, const uint8_t* current, size_t)
    {
        return (const void*)current;
    }
};

template<>
struct _ck_byte_ptr_helper<void*>
{
   static void* cast(const uint8_t*, const uint8_t* current, size_t)
   {
        return (void*)current;
   }
};

/// ck_byte_ptr provides a simple way to interact with and parse buffer data. It
/// behaves similar to a raw byte pointer, except it throws an exception if
/// it is dereferenced passed the bounds of the buffer (to prevent buffer
/// overruns when parsing).
///
/// Sample usage:
///
/// ck_byte_ptr p(buffer, length);
/// uint8_t hdr = *p++;
///
/// uint32_t ts = p.read<uint32_t>(); // Doesn't advance pointer
/// ts = p.consume<uint32_t>();       // Advances pointer
///
/// uint32_t ssrc = *(uint32_t*)p;
/// p += 4;
///
/// *(int16_t*)p = -42;
/// *p.cast<int16_t*>() = -42;
///
/// uint16_t size = *(uint16_t)&p[37];
///
/// p.write<uint32_t>(42);
///
class ck_byte_ptr
{
public:
    /// Initializes pointer to 0
    CK_API ck_byte_ptr() :
        _start(0),
        _current(0),
        _length(0)
    {
    }

    /// Copies the current state of the other pointer
    CK_API ck_byte_ptr(const ck_byte_ptr& other) :
        _start(other._start),
        _current(other._current),
        _length(other._length)
    {
    }

    CK_API ck_byte_ptr(ck_byte_ptr&& other ) noexcept :
        _start(std::move(other._start)),
        _current(std::move(other._current)),
        _length(std::move(other._length))
    {
    }

    /// Initializes the pointer to point to the beginning of the specified buffer
    CK_API ck_byte_ptr(const void* data, size_t length) :
        _start((uint8_t*)data),
        _current((uint8_t*)data),
        _length(length)
    {
    }

    CK_API ~ck_byte_ptr() throw()
    {
    }

    CK_API ck_byte_ptr& operator=(const ck_byte_ptr& other)
    {
        return set_ptr(other._start, other._length);
    }

    CK_API ck_byte_ptr& operator=(ck_byte_ptr&& other) noexcept
    {
        _start = std::move(other._start);
        _current = std::move(other._current);
        _length = std::move(other._length);

        return *this;
    }

    /// Resets the pointer to point to the beginning of the specified buffer
    CK_API ck_byte_ptr& set_ptr(const void* data, size_t length)
    {
        _start = _current = (uint8_t*)data;
        _length = length;
        return *this;
    }

    /// Current pointer value.
    CK_API uint8_t* get_ptr() const
    {
        return _current;
    }

    /// Returns the length of the buffer pointed to.
    CK_API size_t length() const
    {
        return _length;
    }

    /// Indicates if the current pointer value points to a valid position
    /// within the buffer.
    CK_API bool in_bounds() const
    {
        return _in_bounds(_start, _length, _current, 1);
    }

    /// The original pointer value set on the object (i.e. the beginning of the buffer).
    /// @note This value is not affected by operations on the pointer value.
    CK_API const void* original_ptr() const
    {
        return _start;
    }

    /// Adjusts the current pointer position to point to the specified position in the buffer.
    CK_API ck_byte_ptr& seek(size_t offset)
    {
        if (!_in_bounds(_start, _length, _start + offset, 1))
            CK_STHROW(ck_exception, (ck_string::format("Attempt to seek outside the buffer bounds. Offset (%llu), length(%llu)", offset, _length)));
        _current = (uint8_t*)_start + offset;
        return *this;
    }

    /// Returns the current offest (in bytes) from the beginning of the buffer
    /// to the current pointer position.
    CK_API ptrdiff_t offset() const
    {
        return _current - _start;
    }

    /// Returns the number of bytes left that are available to read from the buffer.
    CK_API ptrdiff_t remaining() const
    {
        return (_start + _length) - _current;
    }

    /// Returns a reference to the byte value at the current pointer position.
    CK_API uint8_t& operator*()
    {
        if (!in_bounds())
            CK_STHROW(ck_exception, ("Attempt to deference a pointer outside the buffer bounds."));
        return *_current;
    }

    /// Returns a reference to the byte value at the specified index in the buffer.
    CK_API uint8_t& operator[](size_t index)
    {
        if (!_in_bounds(_start, _length, _start + index, 1))
            CK_STHROW(ck_exception, ("Array index outside the buffer bounds."));
        return *_current;
    }

    /// Returns the specified data type value at the current pointer position.
    template<typename T>
    CK_API T read() const
    {
        if (!_in_bounds(_start, _length, _current, sizeof(T)))
        {
            CK_STHROW(ck_exception, (ck_string::format("Attempt to read outside the buffer bounds. Start (%p), End (%p), Ptr (%p), Size (%llu)",
                                                  _start, _start + _length, _current, sizeof(T))));
        }
        return *(T*)_current;
    }

    /// Returns the specified data type value at the current pointer position AND increments
    /// the pointer past the data read.
    template<typename T>
    CK_API T consume()
    {
        T value = read<T>();
        _current += sizeof(T);
        return value;
    }

    /// writes the specified data value to the current position in the buffer and increments
    /// the pointer past the data written.
    template<typename T>
    CK_API T write(T value)
    {
        if (!_in_bounds(_start, _length, _current, sizeof(T)))
        {
            CK_STHROW(ck_exception, (ck_string::format("Attempt to write outside the buffer bounds. Start (%p), End (%p), Ptr (%p), Size (%llu)",
                                                  _start, _start + _length, _current, sizeof(T))));
        }
        *(T*)_current = value;
        _current += sizeof(T);
        return value;
    }

    CK_API void bulk_write( void* src, uint32_t len )
    {
        if (!_in_bounds(_start, _length, _current, len))
        {
            CK_STHROW(ck_exception, (ck_string::format("Attempt to write outside the buffer bounds. Start (%p), End (%p), Ptr (%p), Size (%llu)",
                                                  _start, _start + _length, _current, len)));
        }

        memcpy( _current, src, len );
        _current += len;
    }

    CK_API uint8_t* operator++()
    {
        return ++_current;
    }

    CK_API uint8_t* operator++(int)
    {
        return _current++;
    }

    CK_API uint8_t* operator--()
    {
        return --_current;
    }

    CK_API uint8_t* operator--(int)
    {
        return _current--;
    }

    CK_API uint8_t* operator+(size_t step) const
    {
        return _current + step;
    }

    CK_API uint8_t* operator-(size_t step) const
    {
        return _current - step;
    }

    CK_API uint8_t* operator+=(size_t step)
    {
        return _current += step;
    }

    CK_API uint8_t* operator-=(size_t step)
    {
        return _current -= step;
    }

    CK_API bool operator==(const ck_byte_ptr& other) const
    {
        return other._current == _current;
    }

    CK_API bool operator!=(const ck_byte_ptr& other) const
    {
        return other._current != _current;
    }

    CK_API bool operator<=(const ck_byte_ptr& other) const
    {
        return _current <= other._current;
    }

    CK_API bool operator>=(const ck_byte_ptr& other) const
    {
        return _current >= other._current;
    }

    CK_API bool operator<(const ck_byte_ptr& other) const
    {
        return _current < other._current;
    }

    CK_API bool operator>(const ck_byte_ptr& other) const
    {
        return _current > other._current;
    }

    /// Allows pointer to be cast to other pointer types only (not concrete types).
    template<typename to>
    CK_API operator to() const
    {
        return _ck_byte_ptr_helper<to>::cast(_start, _current, _length);
    }

private:

    ///< The pointer to the start of the buffer.
    const uint8_t* _start;

    ///< The current pointer value.
    uint8_t* _current;

    ///< The length of the buffer pointed to by '_start'
    size_t _length;
};

}

#endif

