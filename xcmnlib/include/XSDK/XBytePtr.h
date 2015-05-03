
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef XBytePtr_h
#define XBytePtr_h

#include "XSDK/XSharedLib.h"
#include "XSDK/XException.h"
#include "XSDK/XString.h"

#ifdef IS_LINUX
#include <stddef.h>
#endif

namespace XSDK
{

    /// Tests whether pointer 'pos' is within the buffer bounds for 'len' bytes.
    static bool _InBounds(const uint8_t* start, size_t blen, const uint8_t* pos, size_t len)
    {
        return (pos >= start) && ((pos + len) <= (start + blen));
    }

    /// Casts the pointer to the specified pointer type.
    /// @note Only works on pointer types (i.e. not concrete types like int, char, etc).
    template<typename T>
    struct _XBytePtrHelper {
        static T Cast(const uint8_t* start, const uint8_t* current, size_t length)
        {
            T v = 0; // Required so we can get the size of the value *pointed* to
            if (!_InBounds(start, length, current, sizeof(*v)))
            {
                X_STHROW(XException, (XString::Format("Casting to the specified type could access out-of-bounds memory. Start (%p), End (%p), Ptr (%p), Size (%llu)",
                                                      start, start + length, current, sizeof(*v))));
            }
            return (T)current;
        }
    };

    /// Specializes on void*, b/c we can't do a bounds check on this type.
    template<>
    struct _XBytePtrHelper<const void*> {
        static const void* Cast(const uint8_t*, const uint8_t* current, size_t)
        {
            return (const void*)current;
        }
    };
    template<>
    struct _XBytePtrHelper<void*> {
        static void* Cast(const uint8_t*, const uint8_t* current, size_t)
        {
            return (void*)current;
        }
    };

/// XBytePtr provides a simple way to interact with and parse buffer data. It
/// behaves similar to a raw byte pointer, except it throws an exception if
/// it is dereferenced passed the bounds of the buffer (to prevent buffer
/// overruns when parsing).
///
/// Sample usage:
///
/// XBytePtr p(buffer, length);
/// uint8_t hdr = *p++;
///
/// uint32_t ts = p.Read<uint32_t>(); // Doesn't advance pointer
/// ts = p.Consume<uint32_t>();       // Advances pointer
///
/// uint32_t ssrc = *(uint32_t*)p;
/// p += 4;
///
/// *(int16_t*)p = -42;
/// *p.Cast<int16_t*>() = -42;
///
/// uint16_t size = *(uint16_t)&p[37];
///
/// p.Write<uint32_t>(42);
///
class XBytePtr
{
public:
    /// Initializes pointer to 0
    X_API XBytePtr() : _start(0), _current(0), _length(0) {}

    /// Copies the current state of the other pointer
    X_API XBytePtr(const XBytePtr& other) : _start(other._start), _current(other._current), _length(other._length) {}

    /// Initializes the pointer to point to the beginning of the specified buffer
    X_API XBytePtr(const void* data, size_t length) : _start((uint8_t*)data), _current((uint8_t*)data), _length(length) {}

    X_API ~XBytePtr() throw() {}

    X_API XBytePtr& operator=(const XBytePtr& other) { return SetPtr(other._start, other._length); }

    /// Resets the pointer to point to the beginning of the specified buffer
    X_API XBytePtr& SetPtr(const void* data, size_t length) { _start = _current = (uint8_t*)data; _length = length; return *this; }

    /// Current pointer value.
    X_API uint8_t* GetPtr() const { return _current; }

    /// Returns the length of the buffer pointed to.
    X_API size_t Length() const { return _length; }

    /// Indicates if the current pointer value points to a valid position
    /// within the buffer.
    X_API bool InBounds() const { return _InBounds(_start, _length, _current, 1); }

    /// The original pointer value set on the object (i.e. the beginning of the buffer).
    /// @note This value is not affected by operations on the pointer value.
    X_API const void* OriginalPtr() const { return _start; }

    /// Adjusts the current pointer position to point to the specified position in the buffer.
    X_API XBytePtr& Seek(size_t offset)
    {
        if (!_InBounds(_start, _length, _start + offset, 1))
            X_STHROW(XException, (XString::Format("Attempt to seek outside the buffer bounds. Offset (%llu), Length(%llu)", offset, _length)));
        _current = (uint8_t*)_start + offset;
        return *this;
    }

    /// Returns the current offest (in bytes) from the beginning of the buffer
    /// to the current pointer position.
    X_API ptrdiff_t Offset() const { return _current - _start; }

    /// Returns the number of bytes left that are available to read from the buffer.
    X_API ptrdiff_t Remaining() const { return (_start + _length) - _current; }

    /// Returns a reference to the byte value at the current pointer position.
    X_API uint8_t& operator*()
    {
        if (!InBounds())
            X_STHROW(XException, ("Attempt to deference a pointer outside the buffer bounds."));
        return *_current;
    }

    /// Returns a reference to the byte value at the specified index in the buffer.
    X_API uint8_t& operator[](size_t index)
    {
        if (!_InBounds(_start, _length, _start + index, 1))
            X_STHROW(XException, ("Array index outside the buffer bounds."));
        return *_current;
    }

    /// Returns the specified data type value at the current pointer position.
    template<typename T>
    X_API T Read() const
    {
        if (!_InBounds(_start, _length, _current, sizeof(T)))
        {
            X_STHROW(XException, (XString::Format("Attempt to read outside the buffer bounds. Start (%p), End (%p), Ptr (%p), Size (%llu)",
                                                  _start, _start + _length, _current, sizeof(T))));
        }
        return *(T*)_current;
    }

    /// Returns the specified data type value at the current pointer position AND increments
    /// the pointer past the data read.
    template<typename T>
    X_API T Consume()
    {
        T value = Read<T>();
        _current += sizeof(T);
        return value;
    }

    /// Writes the specified data value to the current position in the buffer and increments
    /// the pointer past the data written.
    template<typename T>
    X_API T Write(T value)
    {
        if (!_InBounds(_start, _length, _current, sizeof(T)))
        {
            X_STHROW(XException, (XString::Format("Attempt to write outside the buffer bounds. Start (%p), End (%p), Ptr (%p), Size (%llu)",
                                                  _start, _start + _length, _current, sizeof(T))));
        }
        *(T*)_current = value;
        _current += sizeof(T);
        return value;
    }

    X_API void BulkWrite( void* src, uint32_t len )
    {
        if (!_InBounds(_start, _length, _current, len))
        {
            X_STHROW(XException, (XString::Format("Attempt to write outside the buffer bounds. Start (%p), End (%p), Ptr (%p), Size (%llu)",
                                                  _start, _start + _length, _current, len)));
        }

        memcpy( _current, src, len );
        _current += len;
    }

    /// ++Prefix
    X_API uint8_t* operator++() { return ++_current; }

    /// Postfix++
    X_API uint8_t* operator++(int) { return _current++; }

    /// --Prefix
    X_API uint8_t* operator--() { return --_current; }

    /// Postfix--
    X_API uint8_t* operator--(int) { return _current--; }

    X_API uint8_t* operator+(size_t step) const { return _current + step; }

    X_API uint8_t* operator-(size_t step) const { return _current - step; }

    X_API uint8_t* operator+=(size_t step) { return _current += step; }

    X_API uint8_t* operator-=(size_t step) { return _current -= step; }

    X_API bool operator==(const XBytePtr& other) const { return other._current == _current; }

    X_API bool operator!=(const XBytePtr& other) const { return other._current != _current; }

    X_API bool operator<=(const XBytePtr& other) const { return _current <= other._current; }

    X_API bool operator>=(const XBytePtr& other) const { return _current >= other._current; }

    X_API bool operator<(const XBytePtr& other) const { return _current < other._current; }

    X_API bool operator>(const XBytePtr& other) const { return _current > other._current; }

    /// Allows pointer to be cast to other pointer types only (not concrete types).
    template<typename To>
    X_API operator To() const
    {
        return _XBytePtrHelper<To>::Cast(_start, _current, _length);
    }

private:

    const uint8_t* _start;      ///< The pointer to the start of the buffer.
    uint8_t*       _current;    ///< The current pointer value.
    size_t         _length;     ///< The length of the buffer pointed to by '_start'
};

};

#endif
