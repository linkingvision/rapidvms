
#ifndef cppkit_string_slice_h
#define cppkit_string_slice_h

#include "cppkit/ck_string.h"

namespace cppkit
{

/// Wrapper around ck_string to make parsing easier.
/// It holds a reference to an ck_string and keeps track of what part of the string
/// it refers to. So, you can alter the slice without altering the string, and
/// you can have multiple slices refering to the same string. This means that you
/// can keep popping characters from the slice as you parse it without having to
/// copy the actual string like you'd have to do with substr.
class ck_string_slice
{
public:
    typedef ck_string::const_iterator const_iterator;
    typedef ck_string::const_reverse_iterator const_reverse_iterator;

    CK_API ck_string_slice();

    CK_API ck_string_slice(const ck_string* strPtr)
        : _strPtr(strPtr), _first(0), _length(strPtr->size())
    {
        if( !strPtr )
            CK_THROW(("NULL ck_string pointer."));
    }

    CK_API bool empty() const { return _length == 0; }

    CK_API size_t size() const { return _length; }
    CK_API size_t length() const { return _length; }

    CK_API ck_string to_string() const { return _strPtr->substr(_first, _length); }

    /// Returns an ck_string starting at beginDex and ending one character before endDex.
    ///
    /// Note that this differs from substr, which takes length as its second argument.
    CK_API ck_string to_string(size_t beginDex, size_t endDex) const
    { return _strPtr->substr(_first + beginDex, endDex - beginDex); }

    CK_API ck_string to_string(size_t beginDex) const { return to_string(beginDex, _length); }

    CK_API ck_string_slice slice() const { return *this; }

    /// Returns an ck_string_slice starting at beginDex and ending one character before endDex.
    ///
    /// Note that this differs from substr, which takes length as its second argument.
    CK_API ck_string_slice slice(size_t beginDex, size_t endDex) const
    {
        return ck_string_slice(_strPtr, _first + beginDex, endDex - beginDex);
    }

    CK_API ck_string_slice slice(size_t beginDex) const { return slice(beginDex, _length); }

    CK_API ck_string_slice slice(const_iterator beginIter, const_iterator endIter)
    {
        return ck_string_slice(_strPtr, _first + beginIter - begin(), endIter - beginIter);
    }

//------------------------------------------------------------------------------
// operator==()
//------------------------------------------------------------------------------

    CK_API friend bool operator==(const ck_string_slice& lhs, const ck_string_slice& rhs)
    { return lhs._length == rhs._length && (lhs._strPtr == rhs._strPtr || equal(lhs.begin(), lhs.end(), rhs.begin())); }

    CK_API friend bool operator==(const ck_string_slice& lhs, const ck_string& rhs)
    { return lhs._length == rhs.size() && equal(lhs.begin(), lhs.end(), rhs.begin()); }

    CK_API friend bool operator==(const ck_string& lhs, const ck_string_slice& rhs)
    { return rhs == lhs; }

//------------------------------------------------------------------------------
// operator!=()
//------------------------------------------------------------------------------

    CK_API friend bool operator!=(const ck_string_slice& lhs, const ck_string_slice& rhs) { return !(lhs == rhs); }
    CK_API friend bool operator!=(const ck_string_slice& lhs, const ck_string& rhs) { return !(lhs == rhs); }
    CK_API friend bool operator!=(const ck_string& lhs, const ck_string_slice& rhs) { return rhs != lhs; }

//------------------------------------------------------------------------------
// Element Access
//------------------------------------------------------------------------------

    CK_API const char& front() const { return (*_strPtr)[_first]; }
    CK_API const char& back() const { return (*_strPtr)[_first + _length - 1]; }

    CK_API const char& operator[](size_t pos) const { return (*_strPtr)[_first + pos]; }
    CK_API const char& at(size_t pos) const { return _strPtr->at(_first + pos); }

//------------------------------------------------------------------------------
// Iterator Functions
//------------------------------------------------------------------------------

    CK_API const const_iterator begin() const { return _strPtr->begin() + _first; }
    CK_API const const_iterator end() const { return _strPtr->begin() + _first + _length; }

    CK_API const const_reverse_iterator rbegin() const
    { return _strPtr->rbegin() + (_strPtr->size() - (_first + _length)); }

    CK_API const const_reverse_iterator rend() const
    { return _strPtr->rbegin() + (_strPtr->size() - _first); }


    CK_API void pop_front() { if( _length == 0 ) CK_THROW(("pop_front() called on empty.")); ++_first; --_length; }
    CK_API void pop_back() { if( _length == 0 ) CK_THROW(("pop_back() called on empty.")); --_length; }

    CK_API void pop_front_n(size_t n)
    {
        if( n > _length )
            CK_THROW(("pop count greater than length."));

        _first += n;
        _length -= n;
    }

    CK_API void pop_back_n(size_t n)
    {
        if( n > _length )
            CK_THROW(("pop count greater than length."));

        _length -= n;
    }

//------------------------------------------------------------------------------
// Query Functions
//------------------------------------------------------------------------------

    CK_API bool starts_with(const ck_string& str) const;
    CK_API bool starts_with(const std::string& stdstr) const;
    CK_API bool starts_with(const char* cstr) const;
    CK_API bool starts_with(const wchar_t* wcstr) const;

    CK_API bool ends_with(const ck_string& str) const;
    CK_API bool ends_with(const std::string& stdstr) const;
    CK_API bool ends_with(const char* cstr) const;
    CK_API bool ends_with(const wchar_t* wcstr) const;

//------------------------------------------------------------------------------
// Find
//------------------------------------------------------------------------------

    /// Returns a slice starting at what was found - empty if not found.
    CK_API ck_string_slice find(const ck_string& str) const;
    CK_API ck_string_slice find(const std::string& stdstr) const;
    CK_API ck_string_slice find(const char* cstr) const;
    CK_API ck_string_slice find(char c) const;
    CK_API ck_string_slice find(const wchar_t* wstr) const;

    /// Like Find except that it returns a slice to what was before what was found
    ///
    /// includeFound indicates whether what was fonud should be included in the slice
    /// which is returned.
    CK_API ck_string_slice find_until(const ck_string& str, bool includeFound = false) const;
    CK_API ck_string_slice find_until(const std::string& stdstr, bool includeFound = false) const;
    CK_API ck_string_slice find_until(const char* cstr, bool includeFound = false) const;
    CK_API ck_string_slice find_until(char c, bool includeFound = false) const;
    CK_API ck_string_slice find_until(const wchar_t* wstr, bool includeFound = false) const;

private:

    CK_API ck_string_slice(const ck_string* str, size_t first, size_t length)
        : _strPtr(str), _first(first), _length(length)
    {}

    CK_API ck_string_slice _find(size_t indexFound) const;
    CK_API ck_string_slice _find_until(size_t indexFound, size_t lenOfFound, bool includeFound) const;

    const ck_string* _strPtr;
    size_t _first;
    size_t _length;
};

}

#endif
