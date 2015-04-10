
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef _XSDK_XStringSlice_h
#define _XSDK_XStringSlice_h

#include "XSDK/XRef.h"
#include "XSDK/XString.h"

namespace XSDK
{

/// \brief Wrapper around XString to make parsing easier.
///
/// It holds a reference to an XString and keeps track of what part of the string
/// it refers to. So, you can alter the slice without altering the string, and
/// you can have multiple slices refering to the same string. This means that you
/// can keep popping characters from the slice as you parse it without having to
/// copy the actual string like you'd have to do with substr.
class XStringSlice
{
public:
    typedef XString::const_iterator const_iterator;
    typedef XString::const_reverse_iterator const_reverse_iterator;

    X_API XStringSlice();

    X_API XStringSlice(const XString* strPtr)
        : _strPtr(strPtr), _first(0), _length(strPtr->size())
    { X_ASSERT(_strPtr); }

    X_API bool empty() const { return _length == 0; }

    X_API size_t size() const { return _length; }
    X_API size_t length() const { return _length; }

    X_API XString ToString() const { return _strPtr->substr(_first, _length); }

    /// Returns an XString starting at beginDex and ending one character before endDex.
    ///
    /// Note that this differs from substr, which takes length as its second argument.
    X_API XString ToString(size_t beginDex, size_t endDex) const
    { return _strPtr->substr(_first + beginDex, endDex - beginDex); }

    X_API XString ToString(size_t beginDex) const { return ToString(beginDex, _length); }

    X_API XStringSlice Slice() const { return *this; }

    /// Returns an XStringSlice starting at beginDex and ending one character before endDex.
    ///
    /// Note that this differs from substr, which takes length as its second argument.
    X_API XStringSlice Slice(size_t beginDex, size_t endDex) const
    {
        X_ASSERT(beginDex <= endDex && endDex <= _length);
        return XStringSlice(_strPtr, _first + beginDex, endDex - beginDex);
    }

    X_API XStringSlice Slice(size_t beginDex) const { return Slice(beginDex, _length); }

    X_API XStringSlice Slice(const_iterator beginIter, const_iterator endIter)
    {
        X_ASSERT(beginIter <= endIter);
        return XStringSlice(_strPtr, _first + beginIter - begin(), endIter - beginIter);
    }

//------------------------------------------------------------------------------
// operator==()
//------------------------------------------------------------------------------

    X_API friend bool operator==(const XStringSlice& lhs, const XStringSlice& rhs)
    { return lhs._length == rhs._length && (lhs._strPtr == rhs._strPtr || equal(lhs.begin(), lhs.end(), rhs.begin())); }

    X_API friend bool operator==(const XStringSlice& lhs, const XString& rhs)
    { return lhs._length == rhs.size() && equal(lhs.begin(), lhs.end(), rhs.begin()); }

    X_API friend bool operator==(const XString& lhs, const XStringSlice& rhs)
    { return rhs == lhs; }

//------------------------------------------------------------------------------
// operator!=()
//------------------------------------------------------------------------------

    X_API friend bool operator!=(const XStringSlice& lhs, const XStringSlice& rhs) { return !(lhs == rhs); }
    X_API friend bool operator!=(const XStringSlice& lhs, const XString& rhs) { return !(lhs == rhs); }
    X_API friend bool operator!=(const XString& lhs, const XStringSlice& rhs) { return rhs != lhs; }

//------------------------------------------------------------------------------
// Element Access
//------------------------------------------------------------------------------

    X_API const char& front() const { return (*_strPtr)[_first]; }
    X_API const char& back() const { return (*_strPtr)[_first + _length - 1]; }

    X_API const char& operator[](size_t pos) const { return (*_strPtr)[_first + pos]; }
    X_API const char& at(size_t pos) const { return _strPtr->at(_first + pos); }

//------------------------------------------------------------------------------
// Iterator Functions
//------------------------------------------------------------------------------

    X_API const const_iterator begin() const { return _strPtr->begin() + _first; }
    X_API const const_iterator end() const { return _strPtr->begin() + _first + _length; }

    X_API const const_reverse_iterator rbegin() const
    { return _strPtr->rbegin() + (_strPtr->size() - (_first + _length)); }

    X_API const const_reverse_iterator rend() const
    { return _strPtr->rbegin() + (_strPtr->size() - _first); }


    X_API void popFront() { X_ASSERT(_length != 0); ++_first; --_length; }
    X_API void popBack() { X_ASSERT(_length != 0); --_length; }

    X_API void popFrontN(size_t n) { X_ASSERT(_length >= n); _first += n; _length -= n; }
    X_API void popBackN(size_t n) { X_ASSERT(_length >= n); _length -= n; }

//------------------------------------------------------------------------------
// Query Functions
//------------------------------------------------------------------------------

    X_API bool StartsWith(const XString& str) const;
    X_API bool StartsWith(const std::string& stdstr) const;
    X_API bool StartsWith(const char* cstr) const;
    X_API bool StartsWith(const wchar_t* wcstr) const;

    X_API bool EndsWith(const XString& str) const;
    X_API bool EndsWith(const std::string& stdstr) const;
    X_API bool EndsWith(const char* cstr) const;
    X_API bool EndsWith(const wchar_t* wcstr) const;

//------------------------------------------------------------------------------
// Find
//------------------------------------------------------------------------------

    /// Returns a slice starting at what was found - empty if not found.
    X_API XStringSlice Find(const XString& str) const;
    X_API XStringSlice Find(const std::string& stdstr) const;
    X_API XStringSlice Find(const char* cstr) const;
    X_API XStringSlice Find(char c) const;
    X_API XStringSlice Find(const wchar_t* wstr) const;

    /// Like Find except that it returns a slice to what was before what was found
    ///
    /// includeFound indicates whether what was fonud should be included in the slice
    /// which is returned.
    X_API XStringSlice FindUntil(const XString& str, bool includeFound = false) const;
    X_API XStringSlice FindUntil(const std::string& stdstr, bool includeFound = false) const;
    X_API XStringSlice FindUntil(const char* cstr, bool includeFound = false) const;
    X_API XStringSlice FindUntil(char c, bool includeFound = false) const;
    X_API XStringSlice FindUntil(const wchar_t* wstr, bool includeFound = false) const;

private:

    X_API XStringSlice(const XString* str, size_t first, size_t length)
        : _strPtr(str), _first(first), _length(length)
    {}

    X_API XStringSlice _Find(size_t indexFound) const;
    X_API XStringSlice _FindUntil(size_t indexFound, size_t lenOfFound, bool includeFound) const;

    const XString* _strPtr;
    size_t _first;
    size_t _length;
};

}

#endif
