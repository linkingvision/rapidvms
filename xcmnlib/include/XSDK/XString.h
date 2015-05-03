
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef _XSDK_STRING_H_
#define _XSDK_STRING_H_

#include <cstring>
#include <list>
#include <string>
#include <vector>

#include "XSDK/XSharedLib.h"
#include "XSDK/XIRef.h"
#include "XSDK/Module.h"
#include "XSDK/XException.h"

#ifdef WIN32 // Disabling overzealous / dumb visual studio warning...

#pragma warning(push)

#pragma warning(disable:4127)

#endif

///@todo 7/18/11, remove this define and replace uses of it in the wild with a
///call to the static method on the class.
#define XSTRING_IS_SPACE(a) XString::IsSpace(a)



namespace XSDK
{

static const char encodingTable [64] =
{
    'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P',
    'Q','R','S','T','U','V','W','X','Y','Z','a','b','c','d','e','f',
    'g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v',
    'w','x','y','z','0','1','2','3','4','5','6','7','8','9','+','/'
};

class XMemory;

class XString
{
    enum CHAR_CLASS
    {
        LOWER,
        UPPER,
        DIGIT,
        PLUS,
        EQUAL,
        SLASH,
        NEWLINE,
        CR,
        OTHER
    };

public:
    typedef std::string::size_type size_type;
    typedef std::string::iterator iterator;
    typedef std::string::const_iterator const_iterator;
    typedef std::string::reverse_iterator reverse_iterator;
    typedef std::string::const_reverse_iterator const_reverse_iterator;

//==============================================================================
// Constructors and Conversion Operators
//==============================================================================
    X_API XString()
        : _storage()
    {}

    X_API XString(const XString& other)
        : _storage(other._storage)
    {}

    X_API XString(const std::string& stdstr)
        : _storage(stdstr)
    {}

    X_API XString(const char* cstr)
        : _storage(cstr)
    {}

    X_API XString(char c)
        : _storage(1, c)
    {}

    X_API XString(const XString& other, size_type index, size_type length)
        : _storage(other._storage, index, length)
    {}

    X_API XString(const std::string& stdstr, size_type index, size_type length)
        : _storage(stdstr, index, length)
    {}

    X_API XString(const char* cstr, size_type length)
        : _storage(cstr, length)
    {}

    X_API XString(std::string::size_type length, char ch)
        : _storage(length, ch)
    {}

    template <typename InputIterator>
    X_API XString(InputIterator begin, InputIterator end)
        : _storage(begin, end)
    {}

    X_API XString(const wchar_t* wcstr)
        : _storage(ConvertWideStringToMultiByteString(wcstr))
    {}

    X_API XString(const wchar_t* wcstr, size_t length)
        : _storage(ConvertWideStringToMultiByteString(wcstr, length))
    {}

    X_API XString(wchar_t wc)
        : _storage()
    {
        wchar_t value[] = {wc,'\0'};
        _storage = ConvertWideStringToMultiByteString(value);
    }


    X_API ~XString() throw() {}

    /// XSDK::XString implicitly casts to std::string& so that
    /// it can be used as a std::string without overhead.
    X_API operator std::string&() {return _storage;}

    /// XSDK::XString implicitly casts to const std::string& so
    /// that it can be used as a std::string without overhead.
    X_API operator const std::string&() const {return _storage;}

    X_API XString& operator=(const XString& other)
    {
        _storage = other._storage;
        return *this;
    }

    X_API XString& operator=(const std::string& stdstr)
    {
        _storage = stdstr;
        return *this;
    }

    X_API XString& operator=(const char* cstr)
    {
        _storage = cstr;
        return *this;
    }

    X_API XString& operator=(char c)
    {
        _storage = c;
        return *this;
    }


    X_API XString& operator=(const wchar_t* wcstr)
    {
        _storage = ConvertWideStringToMultiByteString(wcstr);
        return *this;
    }

    X_API XString& operator=(wchar_t wc)
    {
        _storage = ConvertWideStringToMultiByteString(&wc);
        return *this;
    }


//==============================================================================
// Functions not from std::string
//==============================================================================



//------------------------------------------------------------------------------
// std_str()
//------------------------------------------------------------------------------

    /// For getting a std::string* instead of a std::string& like the implicit
    /// conversion operator does.
    X_API std::string* std_str() {return &_storage;}

    /// For getting a const std::string* instead of a const std::string& like the
    ///implicit conversion operator does.
    X_API const std::string* std_str() const {return &_storage;}

//------------------------------------------------------------------------------
// Format()
//------------------------------------------------------------------------------

    /// \brief Created an XString from a formatted string.
    ///
    /// I64 and I32 flags are accepted (e.g. %I64d and %I32u). l and ll flags
    /// should be generally avoided just like long and long long should be
    /// generally avoided.
    ///@{
    static X_API XString Format(const char* format, ...);
    static X_API XString Format(const char* format, va_list& args);

    X_API XString& FormatInPlace(const char* format, ...);
    ///@}

//------------------------------------------------------------------------------
// Numeric Conversion Functions
//------------------------------------------------------------------------------

    static X_API XString FromInt(int val, int base = 10);
    static X_API XString FromUInt(unsigned int val, int base = 10);
    static X_API XString FromInt8(int8_t val, int base = 10);
    static X_API XString FromUInt8(uint8_t val, int base = 10);
    static X_API XString FromInt16(int16_t val, int base = 10);
    static X_API XString FromUInt16(uint16_t val, int base = 10);
    static X_API XString FromInt32(int32_t val, int base = 10);
    static X_API XString FromUInt32(uint32_t val, int base = 10);
    static X_API XString FromInt64(int64_t val, int base = 10);
    static X_API XString FromUInt64(uint64_t val, int base = 10);
    static X_API XString FromDouble(double val) {return Format("%lf", val);}

    ///
    X_API int ToInt(bool truncate = false) const;
    ///
    X_API int ToInt(int base) const;

    ///
    X_API unsigned int ToUInt(bool truncate = false) const;
    ///
    X_API unsigned int ToUInt(int base) const;

    ///
    X_API int8_t ToInt8(bool truncate = false) const;
    ///
    X_API int8_t ToInt8(int base) const;

    ///
    X_API uint8_t ToUInt8(bool truncate = false) const;
    ///
    X_API uint8_t ToUInt8(int base) const;

    ///
    X_API int16_t ToInt16(bool truncate = false) const;
    ///
    X_API int16_t ToInt16(int base) const;

    ///
    X_API uint16_t ToUInt16(bool truncate = false) const;
    ///
    X_API uint16_t ToUInt16(int base) const;

    ///
    X_API int32_t ToInt32(bool truncate = false) const;
    ///
    X_API int32_t ToInt32(int base) const;

    ///
    X_API uint32_t ToUInt32(bool truncate = false) const;
    ///
    X_API uint32_t ToUInt32(int base) const;

    ///
    X_API int64_t ToInt64(bool truncate = false) const;
    ///
    X_API int64_t ToInt64(int base) const;

    ///
    X_API uint64_t ToUInt64(bool truncate = false) const;
    ///
    X_API uint64_t ToUInt64(int base) const;

    ///
    X_API double ToDouble() const;

//------------------------------------------------------------------------------
// Base64 Functions
//------------------------------------------------------------------------------

    X_API static XString ToBase64(XIRef<XMemory> data);
    X_API static XString Base64Encode( const void* source, size_t length );

    X_API XIRef<XMemory> FromBase64() const;

//------------------------------------------------------------------------------
// URI Functions
//------------------------------------------------------------------------------

    // See RFC 3986
    X_API XString URIEncode() const;
    X_API XString URIDecode() const;

    X_API XString& URIEncodeInPlace() {*this = URIEncode(); return *this;}
    X_API XString& URIDecodeInPlace() {*this = URIDecode(); return *this;}

//------------------------------------------------------------------------------
// Case Transformations
//------------------------------------------------------------------------------

    X_API XString ToLower() const;///@todo make UTF-8 safe
    X_API XString ToUpper() const;///@todo make UTF-8 safe

    X_API XString& ToLowerInPlace();///@todo make UTF-8 safe
    X_API XString& ToUpperInPlace();///@todo make UTF-8 safe

//------------------------------------------------------------------------------
// Query Functions
//------------------------------------------------------------------------------

    X_API bool StartsWith(const XString& other) const;
    X_API bool StartsWith(const std::string& stdstr) const;
    X_API bool StartsWith(const char* cstr) const;
    X_API bool StartsWith(const wchar_t* wcstr) const;

    X_API bool EndsWith(const XString& other) const;
    X_API bool EndsWith(const std::string& stdstr) const;
    X_API bool EndsWith(const char* cstr) const;
    X_API bool EndsWith(const wchar_t* wcstr) const;

    X_API bool Contains(const XString& other) const {return _Contains(other);}
    X_API bool Contains(const std::string& stdstr) const {return _Contains(stdstr);}
    X_API bool Contains(const char* cstr) const
    {
        if(NULL == cstr)
            X_THROW(("NULL string passed to XString::Contains()."));
        return _Contains(cstr);
    }
    X_API bool Contains(const wchar_t* wcstr) const
    {
        if(NULL == wcstr)
            X_THROW(("NULL string passed to XString::Contains(const wchar_t* wcstr)."));
        return _Contains(ConvertWideStringToMultiByteString(wcstr));
    }

    /// \brief Whether the string contains an integral value (and contains no extraneous characters).
    ///
    /// \param canHaveSign  Whether the integral value can start with '-'. If false, then
    //                      the return value with be false if the string starts with '-'.
    X_API bool IsInteger(bool canHaveSign = true) const;

    /// \brief Whether the string contains an integral value in hex (and contains no extraneous characters).
    ///
    /// \param canHaveSign  Whether the integral value can start with '-'. If false, then
    //                      the return value with be false if the string starts with '-'.
    X_API bool IsHex(bool canHaveSign = true) const;

    /// \brief Whether the string contains a floating point value of some kind (and nothing else).
    ///
    /// \param canHaveSign  Whether the floating point value can start with '-'. If false, then
    //                      the return value with be false if the string starts with '-'.
    X_API bool IsDouble(bool canHaveSign = true) const;

//------------------------------------------------------------------------------
// Strip()
//------------------------------------------------------------------------------

    X_API XString LStrip() const;
    X_API XString RStrip() const;
    X_API XString Strip() const;

    X_API XString& LStripInPlace();
    X_API XString& RStripInPlace();
    X_API XString& StripInPlace();

    /// \brief Returns the string without the last \r\n or \n if it ends with
    ///        either of those. It does _not_ strip \r by itself.
    X_API XString StripEOL() const;

//------------------------------------------------------------------------------
// Split()
//------------------------------------------------------------------------------

    X_API void Split(char delim, OUT std::vector<XString>& parts) const
    {
        _Split(delim, 1, parts);
    }

    X_API void Split(wchar_t delim, OUT std::vector<XString>& parts) const
    {
        wchar_t delimeter[] = {delim,'\0'};
        Split(delimeter, parts);
    }

    X_API void Split(const XString& delim, OUT std::vector<XString>& parts) const
    {
        _Split(delim, delim._storage.size(), parts);
    }

    X_API void Split(const std::string& delim, OUT std::vector<XString>& parts) const
    {
        _Split(delim, delim.size(), parts);
    }

    X_API void Split(const char* delim, OUT std::vector<XString>& parts) const
    {
        if(!delim)
            X_THROW(("NULL string passed to XString::Split()."));
        _Split(delim, strlen(delim), parts);
    }

    X_API void Split(const wchar_t* delim, OUT std::vector<XString>& parts) const
    {
        if(delim==NULL)
            X_THROW(("NULL string passed to XString::Split()."));
        Split(ConvertWideStringToMultiByteString(delim), parts);
    }


    X_API std::vector<XString> Split(char delim) const
    {
        std::vector<XString> parts;
        _Split(delim, 1, parts);
        return parts;
    }

    X_API std::vector<XString> Split(const XString& delim) const
    {
        std::vector<XString> parts;
        _Split(delim, delim._storage.size(), parts);
        return parts;
    }

    X_API std::vector<XString> Split(const std::string& delim) const
    {
        std::vector<XString> parts;
        _Split(delim, delim.size(), parts);
        return parts;
    }

    X_API std::vector<XString> Split(const char* delim) const
    {
        if(!delim)
            X_THROW(("NULL string passed to XString::Split()."));
        std::vector<XString> parts;
        _Split(delim, strlen(delim), parts);
        return parts;
    }


    X_API std::vector<XString> Split(wchar_t delim) const
    {
      wchar_t delimeter[] = {delim,'\0'};
      return Split(ConvertWideStringToMultiByteString((wchar_t*)delimeter));
    }

    X_API std::vector<XString> Split(const wchar_t* delim) const
    {
        if(delim == NULL)
            X_THROW(("NULL string passed to XString::Split()."));
        return Split(ConvertWideStringToMultiByteString(delim));
    }



//------------------------------------------------------------------------------
// ReplaceAll()
//------------------------------------------------------------------------------

    ///@brief Replace all instances of a given character with another given character.
    ///
    ///@param toBeReplaced The character to replace.
    ///@param toReplaceWith The character to replace toBeReplaced with.
    X_API XString ReplaceAll(char toBeReplaced, char toReplaceWith) const
    {
        XString dup = *this;
        dup.ReplaceAllInPlace(toBeReplaced, toReplaceWith);
        return dup;
    }

    ///@brief Replace all instances of a given character with another given character.
    ///
    ///@param toBeReplaced The character to replace.
    ///@param toReplaceWith The character to replace toBeReplaced with.
    X_API XString ReplaceAll(wchar_t toBeReplaced, wchar_t toReplaceWith) const;

    ///@brief Replace all instances of a given substring with another given substring.
    ///
    ///@param toBeReplaced The substring to replace.
    ///@param toReplaceWith The substring to replace toBeReplaced with.
    X_API XString ReplaceAll(const XString& toBeReplaced, const XString& toReplaceWith) const;


    ///@brief Replace all instances of a given character with another given character.
    ///
    ///@param toBeReplaced The character to replace.
    ///@param toReplaceWith The character to replace toBeReplaced with.
    X_API XString& ReplaceAllInPlace(char toBeReplaced, char toReplaceWith);

    ///@brief Replace all instances of a given character with another given character.
    ///
    ///@param toBeReplaced The character to replace.
    ///@param toReplaceWith The character to replace toBeReplaced with.
    X_API XString& ReplaceAllInPlace(wchar_t toBeReplaced, wchar_t toReplaceWith)
    {
        return *this = ReplaceAll(toBeReplaced, toReplaceWith);
    }

    ///@brief Replace all instances of a given substring with another given substring.
    ///
    ///@param toBeReplaced The substring to replace.
    ///@param toReplaceWith The substring to replace toBeReplaced with.
    X_API XString& ReplaceAllInPlace(const XString& toBeReplaced, const XString& toReplaceWith)
    {
        return *this = ReplaceAll(toBeReplaced, toReplaceWith);
    }

//------------------------------------------------------------------------------
// EraseAll()
//------------------------------------------------------------------------------

    ///@brief Removes all occurrences of the given character from the string.
    X_API XString EraseAll(char toBeErased) const {return _EraseAll(toBeErased);}

    ///@brief Removes all occurrences of the given character from the string.
    X_API XString& EraseAllInPlace(char toBeErased)
    {
        *this = _EraseAll(toBeErased);
        return *this;
    }

    ///@brief Removes all occurrences of the given character from the string.
    X_API XString EraseAll(wchar_t toBeErased) const {return _EraseAll(toBeErased);}

    ///@brief Removes all occurrences of the given character from the string.
    X_API XString& EraseAllInPlace(wchar_t toBeErased)
    {
        return *this = _EraseAll(toBeErased);
    }

    ///@brief Removes all occurrences of the given substring from the string.
    X_API XString EraseAll(const XString& toBeErased) const {return _EraseAll(toBeErased);}

    ///@brief Removes all occurrences of the given substring from the string.
    X_API XString& EraseAllInPlace(const XString& toBeErased)
    {
        *this = _EraseAll(toBeErased);
        return *this;
    }


//==============================================================================
// Functions from std::string
//==============================================================================

//------------------------------------------------------------------------------
// Iterator Functions
//------------------------------------------------------------------------------

    X_API iterator begin() {return _storage.begin();}
    X_API const const_iterator begin() const {return _storage.begin();}

    X_API iterator end() {return _storage.end();}
    X_API const const_iterator end() const {return _storage.end();}

    X_API reverse_iterator rbegin() {return _storage.rbegin();}
    X_API const const_reverse_iterator rbegin() const {return _storage.rbegin();}

    X_API reverse_iterator rend() {return _storage.rend();}
    X_API const const_reverse_iterator rend() const {return _storage.rend();}

//------------------------------------------------------------------------------
// Capacity Functions
//------------------------------------------------------------------------------

    X_API size_t size() const {return _storage.size();}
    X_API size_t length() const {return _storage.length();}

    X_API size_t max_size() const {return _storage.max_size();}

    X_API void resize(size_t size, char c) {_storage.resize(size, c);}
    X_API void resize(size_t size) {_storage.resize(size);}

    X_API size_t capacity() const {return _storage.capacity();}

    X_API void reserve(size_t res_arg = 0) {_storage.reserve(res_arg);}

    X_API void clear() {_storage.clear();}

    X_API bool empty() const {return _storage.empty();}

//------------------------------------------------------------------------------
// Element Access
//------------------------------------------------------------------------------

    X_API const char& operator[](size_t pos) const {return _storage[pos];}
    X_API char& operator[](size_t pos) {return _storage[pos];}

    X_API const char& at(size_t pos) const {return _storage.at(pos);}
    X_API char& at(size_t pos) {return _storage.at(pos);}

//------------------------------------------------------------------------------
// operator<<()
//------------------------------------------------------------------------------

    X_API friend std::ostream& operator<<(std::ostream& os, const XString& str) {return os << str._storage;}
    X_API friend std::istream& operator>>(std::istream& is, XString& str) {return is >> str._storage;}

//------------------------------------------------------------------------------
// operator==()
//------------------------------------------------------------------------------

    X_API friend bool operator ==(const XString& left, const XString& right) {return left._storage == right._storage;}
    X_API friend bool operator ==(const XString& left, const std::string& right) {return left._storage == right;}
    X_API friend bool operator ==(const std::string& left, const XString& right) {return left == right._storage;}
    X_API friend bool operator ==(const XString& left, const char* right) {return left._storage == std::string(right);}
    X_API friend bool operator ==(const char* left, const XString& right) {return std::string(left) == right._storage;}
    X_API friend bool operator ==(const wchar_t* left, const XString& right) {return ConvertWideStringToMultiByteString(left) == right._storage;}

//------------------------------------------------------------------------------
// operator!=()
//------------------------------------------------------------------------------

    X_API friend bool operator !=(const XString& left, const XString& right) {return left._storage != right._storage;}
    X_API friend bool operator !=(const XString& left, const std::string& right) {return left._storage != right;}
    X_API friend bool operator !=(const std::string& left, const XString& right) {return left != right._storage;}
    X_API friend bool operator !=(const XString& left, const char* right) {return left._storage != std::string(right);}
    X_API friend bool operator !=(const char* left, const XString& right) {return std::string(left) != right._storage;}
    X_API friend bool operator !=(const wchar_t* left, const XString& right) {return ConvertWideStringToMultiByteString(left) != right._storage;}

//------------------------------------------------------------------------------
// operator<()
//------------------------------------------------------------------------------

    X_API friend bool operator <(const XString& left, const XString& right) {return left._storage < right._storage;}
    X_API friend bool operator <(const XString& left, const std::string& right) {return left._storage < right;}
    X_API friend bool operator <(const std::string& left, const XString& right) {return left < right._storage;}
    X_API friend bool operator <(const XString& left, const char* right) {return left._storage < std::string(right);}
    X_API friend bool operator <(const char* left, const XString& right) {return std::string(left) < right._storage;}
    X_API friend bool operator <(const wchar_t* left, const XString& right) {return ConvertWideStringToMultiByteString(left) < right._storage;}

//------------------------------------------------------------------------------
// operator>()
//------------------------------------------------------------------------------

    X_API friend bool operator >(const XString& left, const XString& right) {return left._storage > right._storage;}
    X_API friend bool operator >(const XString& left, const std::string& right) {return left._storage > right;}
    X_API friend bool operator >(const std::string& left, const XString& right) {return left > right._storage;}
    X_API friend bool operator >(const XString& left, const char* right) {return left._storage > std::string(right);}
    X_API friend bool operator >(const char* left, const XString& right) {return std::string(left) > right._storage;}
    X_API friend bool operator >(const wchar_t* left, const XString& right) {return ConvertWideStringToMultiByteString(left) > right._storage;}

//------------------------------------------------------------------------------
// operator<=()
//------------------------------------------------------------------------------

    X_API friend bool operator <=(const XString& left, const XString& right) {return left._storage <= right._storage;}
    X_API friend bool operator <=(const XString& left, const std::string& right) {return left._storage <= right;}
    X_API friend bool operator <=(const std::string& left, const XString& right) {return left <= right._storage;}
    X_API friend bool operator <=(const XString& left, const char* right) {return left._storage <= std::string(right);}
    X_API friend bool operator <=(const char* left, const XString& right) {return std::string(left) <= right._storage;}
    X_API friend bool operator <=(const wchar_t* left, const XString& right) {return ConvertWideStringToMultiByteString(left) <= right._storage;}

//------------------------------------------------------------------------------
// operator>=()
//------------------------------------------------------------------------------

    X_API friend bool operator >=(const XString& left, const XString& right) {return left._storage >= right._storage;}
    X_API friend bool operator >=(const XString& left, const std::string& right) {return left._storage >= right;}
    X_API friend bool operator >=(const std::string& left, const XString& right) {return left >= right._storage;}
    X_API friend bool operator >=(const XString& left, const char* right) {return left._storage >= std::string(right);}
    X_API friend bool operator >=(const char* left, const XString& right) {return std::string(left) >= right._storage;}
    X_API friend bool operator >=(const wchar_t* left, const XString& right) {return ConvertWideStringToMultiByteString(left) >= right._storage;}

//------------------------------------------------------------------------------
// operator+()
//------------------------------------------------------------------------------

    X_API friend XString operator +(const XString& left, const XString& right) {return left._storage + right._storage;}
    X_API friend XString operator +(const XString& left, const std::string& right) {return left._storage + right;}
    X_API friend XString operator +(const std::string& left, const XString& right) {return left + right._storage;}
    X_API friend XString operator +(const XString& left, const char* right) {return left._storage + right;}
    X_API friend XString operator +(const char* left, const XString& right) {return left + right._storage;}
    X_API friend XString operator +(char left, const XString& right) {return left + right._storage;}
    X_API friend XString operator +(const XString& left, char right) {return left._storage + right;}

    X_API friend XString operator +(const wchar_t* left, const XString& right){return ConvertWideStringToMultiByteString(left) + right._storage;}
    X_API friend XString operator +(wchar_t left, const XString& right)
    {
      wchar_t value[] = {left,'\0'};
      return ConvertWideStringToMultiByteString(value) + right._storage;
    }



//------------------------------------------------------------------------------
// operator+=()
//------------------------------------------------------------------------------

    X_API XString& operator +=(const XString& other)
    {
        _storage += other._storage;
        return *this;
    }

    X_API XString& operator +=(const std::string& stdstr)
    {
        _storage += stdstr;
        return *this;
    }

    X_API XString& operator +=(const char* cstr)
    {
        _storage += cstr;
        return *this;
    }

    X_API XString& operator +=(char c)
    {
        _storage += c;
        return *this;
    }

    X_API XString& operator +=(const wchar_t* wcstr)
    {
        _storage += ConvertWideStringToMultiByteString(wcstr);
        return *this;
    }

    X_API XString& operator +=(wchar_t wc)
    {
      wchar_t value[] = {wc,'\0'};
      _storage += ConvertWideStringToMultiByteString(value);
      return *this;
    }

//------------------------------------------------------------------------------
// append()
//------------------------------------------------------------------------------

    X_API XString& append(const XString& other)
    {
        _storage.append(other._storage);
        return *this;
    }

    X_API XString& append(const std::string& stdstr)
    {
        _storage.append(stdstr);
        return *this;
    }

    X_API XString& append(const XString& other, size_t pos, size_t length)
    {
        _storage.append(other._storage, pos, length);
        return *this;
    }

    X_API XString& append(const std::string& stdstr, size_t pos, size_t length)
    {
        _storage.append(stdstr, pos, length);
        return *this;
    }

    X_API XString& append(const char* cstr, size_t length)
    {
        _storage.append(cstr, length);
        return *this;
    }

    X_API XString& append(const char* cstr)
    {
        _storage.append(cstr);
        return *this;
    }

    X_API XString& append(size_t length, char c)
    {
        _storage.append(length, c);
        return *this;
    }

    X_API XString& append(const wchar_t* wstr)
    {
      _storage.append(ConvertWideStringToMultiByteString(wstr));
      return *this;
    }

    X_API XString& append(wchar_t wc)
    {
      wchar_t value[] = {wc,'\0'};
      return append(value);
    }

    template <typename InputIterator>
    X_API XString& append(InputIterator begin, InputIterator end)
    {
        _storage.append(begin, end);
        return *this;
    }

//------------------------------------------------------------------------------
// push_back()
//------------------------------------------------------------------------------

    X_API void push_back(char c) {_storage.push_back(c);}

    X_API void push_back(wchar_t wc)
    {
            wchar_t value[] = {wc,'\0'};
        std::string temp = ConvertWideStringToMultiByteString(value);
        for ( size_t i = 0; i < temp.length(); ++i )
            _storage.push_back(temp.c_str()[i]);
    }

//------------------------------------------------------------------------------
// assign()
//------------------------------------------------------------------------------

    X_API XString& assign(const XString& other)
    {
        _storage.assign(other._storage);
        return *this;
    }

    X_API XString& assign(const std::string& stdstr)
    {
        _storage.assign(stdstr);
        return *this;
    }

    X_API XString& assign(const XString& other, size_t pos, size_t length)
    {
        _storage.assign(other._storage, pos, length);
        return *this;
    }

    X_API XString& assign(const std::string& stdstr, size_t pos, size_t length)
    {
        _storage.assign(stdstr, pos, length);
        return *this;
    }

    X_API XString& assign(const char* cstr, size_t length)
    {
        _storage.assign(cstr, length);
        return *this;
    }

    X_API XString& assign(const char* cstr)
    {
        _storage.assign(cstr);
        return *this;
    }

    X_API XString& assign(size_t length, char c)
    {
        _storage.assign(length, c);
        return *this;
    }

    X_API XString& assign(const wchar_t* wcstr)
    {
        _storage.assign(ConvertWideStringToMultiByteString(wcstr));
        return *this;
    }

    X_API XString& assign( wchar_t wc)
    {
      wchar_t value[] = {wc,'\0'};
      return assign(ConvertWideStringToMultiByteString(value));
    }

    template <typename InputIterator>
    X_API XString& assign(InputIterator begin, InputIterator end)
    {
        _storage.assign(begin, end);
        return *this;
    }

//------------------------------------------------------------------------------
// insert()
//------------------------------------------------------------------------------

    X_API XString& insert(size_t pos, const XString& other)
    {
        _storage.insert(pos, other._storage);
        return *this;
    }

    X_API XString& insert(size_t pos, const std::string& stdstr)
    {
        _storage.insert(pos, stdstr);
        return *this;
    }

    X_API XString& insert(size_t pos1, const XString& other, size_t pos2, size_t length)
    {
        _storage.insert(pos1, other._storage, pos2, length);
        return *this;
    }

    X_API XString& insert(size_t pos1, const std::string& stdstr, size_t pos2, size_t length)
    {
        _storage.insert(pos1, stdstr, pos2, length);
        return *this;
    }

    X_API XString& insert(size_t pos, const char* cstr, size_t length)
    {
        _storage.insert(pos, cstr, length);
        return *this;
    }

    X_API XString& insert(size_t pos, const char* cstr)
    {
        _storage.insert(pos, cstr);
        return *this;
    }

    X_API XString& insert(size_t pos, size_t length, char c)
    {
        _storage.insert(pos, length , c);
        return *this;
    }

    X_API iterator insert(iterator p, char c) {return _storage.insert(p, c);}
    X_API void insert(iterator p, size_t length, char c) {_storage.insert(p, length, c);}

    X_API XString& insert(size_t pos, const wchar_t* wcstr)
    {
        return insert(pos, ConvertWideStringToMultiByteString(wcstr));
    }

    X_API XString& insert(size_t pos, wchar_t wc)
    {
      wchar_t value[] = {wc,'\0'};
      return insert(pos,value);
    }

    X_API iterator insert(iterator p, wchar_t wc)
    {
      wchar_t value[] = {wc,'\0'};
      std::string temp = ConvertWideStringToMultiByteString(value);
      for ( size_t i = 9; i < temp.length(); ++i )
    insert(p,temp.c_str()[i]);
      return p;
    }

    template <typename InputIterator>
    X_API void insert(iterator p, InputIterator begin, InputIterator end) {_storage.insert(p, begin, end);}

//------------------------------------------------------------------------------
// erase()
//------------------------------------------------------------------------------

    X_API XString& erase(size_t pos = 0, size_t length = std::string::npos)
    {
        _storage.erase(pos, length);
        return *this;
    }

    X_API iterator erase(iterator pos) {return _storage.erase(pos);}
    X_API iterator erase(iterator begin, iterator end) {return _storage.erase(begin, end);}

//------------------------------------------------------------------------------
// replace()
//------------------------------------------------------------------------------

    X_API XString& replace(size_t pos, size_t length, const XString& other)
    {
        _storage.replace(pos, length, other._storage);
        return *this;
    }

    X_API XString& replace(size_t pos, size_t length, const std::string& stdstr)
    {
        _storage.replace(pos, length, stdstr);
        return *this;
    }

    X_API XString& replace(iterator begin, iterator end, const XString& other)
    {
        _storage.replace(begin, end, other._storage);
        return *this;
    }

    X_API XString& replace(iterator begin, iterator end, const std::string& stdstr)
    {
        _storage.replace(begin, end, stdstr);
        return *this;
    }

    X_API XString& replace(size_t pos1, size_t n1, const XString& other, size_t pos2, size_t n2)
    {
        _storage.replace(pos1, n1, other._storage, pos2, n2);
        return *this;
    }

    X_API XString& replace(size_t pos1, size_t n1, const std::string& stdstr, size_t pos2, size_t n2)
    {
        _storage.replace(pos1, n1, stdstr, pos2, n2);
        return *this;
    }

    X_API XString& replace(size_t pos, size_t n1, const char* cstr, size_t n2)
    {
        _storage.replace(pos, n1, cstr, n2);
        return *this;
    }

    X_API XString& replace(iterator begin, iterator end, const char* cstr, size_t length)
    {
        _storage.replace(begin, end, cstr, length);
        return *this;
    }

    X_API XString& replace(size_t pos, size_t length, const char* cstr)
    {
        _storage.replace(pos, length, cstr);
        return *this;
    }

    X_API XString& replace(iterator begin, iterator end, const char* cstr)
    {
        _storage.replace(begin, end, cstr);
        return *this;
    }

    X_API XString& replace(size_t pos, size_t n1, size_t n2, char c)
    {
        _storage.replace(pos, n1, n2, c);
        return *this;
    }

    X_API XString& replace(iterator begin, iterator end, size_t length, char c)
    {
        _storage.replace(begin, end, length, c);
        return *this;
    }

    X_API XString& replace(size_t pos, size_t n1, const wchar_t* wcstr)
    {
        return replace(pos, n1, ConvertWideStringToMultiByteString(wcstr));
    }

    X_API XString& replace(iterator begin, iterator end, const wchar_t* wcstr)
    {
        return replace(begin, end, ConvertWideStringToMultiByteString(wcstr));
    }

    X_API XString& replace(size_t pos, size_t n1, size_t /*n2*/, wchar_t wc)
    {
      wchar_t value[] = {wc,'\0'};
      return replace(pos, n1, ConvertWideStringToMultiByteString(value));
    }

    X_API XString& replace(iterator begin, iterator end, size_t /*length*/, wchar_t wc)
    {
      wchar_t value[] = {wc,'\0'};
      return replace(begin, end, ConvertWideStringToMultiByteString(value));
    }

    template <typename InputIterator>
    X_API XString& replace(iterator begin1, iterator end1, InputIterator begin2, InputIterator end2)
    {
        _storage.replace(begin1, end1, begin2, end2);
        return *this;
    }

//------------------------------------------------------------------------------
// swap()
//------------------------------------------------------------------------------

    X_API void swap(XString& other) {_storage.swap(other._storage);}
    X_API void swap(std::string& stdstr) {_storage.swap(stdstr);}

//------------------------------------------------------------------------------
// c_str()
//------------------------------------------------------------------------------

    X_API const char* c_str() const {return _storage.c_str();}

//------------------------------------------------------------------------------
// get_wide_string()
//------------------------------------------------------------------------------

    X_API std::wstring get_wide_string() const {return ConvertMultiByteStringToWideString(_storage);}

//------------------------------------------------------------------------------
// data()
//------------------------------------------------------------------------------

    X_API const char* data() const {return _storage.data();}

//------------------------------------------------------------------------------
// get_allocator()
//------------------------------------------------------------------------------

    X_API std::allocator<char> get_allocator() const {return _storage.get_allocator();}

//------------------------------------------------------------------------------
// find()
//------------------------------------------------------------------------------

    X_API size_t find(const XString& other, size_t pos = 0) const
    {
        return _storage.find(other._storage, pos);
    }

    X_API size_t find(const std::string& stdstr, size_t pos = 0) const
    {
        return _storage.find(stdstr, pos);
    }

    X_API size_t find(const char* cstr, size_t pos, size_t length) const
    {
        return _storage.find(cstr, pos, length);
    }

    X_API size_t find(const char* cstr, size_t pos = 0) const
    {
        return _storage.find(cstr, pos);
    }

    X_API size_t find(char c, size_t pos = 0) const
    {
        return _storage.find(c, pos);
    }

    X_API size_t find(const wchar_t* wcstr, size_t pos, size_t length) const
    {
        return _storage.find(ConvertWideStringToMultiByteString(wcstr, length), pos);
    }

    X_API size_t find(const wchar_t* wcstr, size_t pos = 0) const
    {
        return _storage.find(ConvertWideStringToMultiByteString(wcstr), pos);
    }

    X_API size_t find(wchar_t wc, size_t pos = 0) const
    {
        wchar_t value[] = {wc,'\0'};
        return _storage.find(ConvertWideStringToMultiByteString(value), pos);
    }


//------------------------------------------------------------------------------
// rfind()
//------------------------------------------------------------------------------

    X_API size_t rfind(const XString& other, size_t pos = std::string::npos) const
    {
        return _storage.rfind(other._storage, pos);
    }

    X_API size_t rfind(const std::string& stdstr, size_t pos = std::string::npos) const
    {
        return _storage.rfind(stdstr, pos);
    }

    X_API size_t rfind(const char* cstr, size_t pos, size_t length) const
    {
        return _storage.rfind(cstr, pos, length);
    }

    X_API size_t rfind(const char* cstr, size_t pos = std::string::npos) const
    {
        return _storage.rfind(cstr, pos);
    }

    X_API size_t rfind(char c, size_t pos = std::string::npos) const
    {
        return _storage.rfind(c, pos);
    }


    X_API size_t rfind(const wchar_t* wcstr, size_t pos, size_t length) const
    {
        return _storage.rfind(ConvertWideStringToMultiByteString(wcstr, length), pos);
    }

    X_API size_t rfind(const wchar_t* wcstr, size_t pos = std::string::npos) const
    {
        return _storage.rfind(ConvertWideStringToMultiByteString(wcstr), pos);
    }

    X_API size_t rfind(wchar_t wc, size_t pos = std::string::npos) const
    {
      wchar_t value[] = {wc,'\0'};
        return _storage.rfind(ConvertWideStringToMultiByteString(value), pos);
    }


//------------------------------------------------------------------------------
// find_first_of()
//------------------------------------------------------------------------------

    X_API size_t find_first_of(const XString& other, size_t pos = 0) const
    {
        return _storage.find_first_of(other._storage, pos);
    }

    X_API size_t find_first_of(const std::string& stdstr, size_t pos = 0) const
    {
        return _storage.find_first_of(stdstr, pos);
    }

    X_API size_t find_first_of(const char* cstr, size_t pos, size_t length) const
    {
        return _storage.find_first_of(cstr, pos, length);
    }

    X_API size_t find_first_of(const char* cstr, size_t pos = 0) const
    {
        return _storage.find_first_of(cstr, pos);
    }

    X_API size_t find_first_of(char c, size_t pos = 0) const
    {
        return _storage.find_first_of(c, pos);
    }


    X_API size_t find_first_of(const wchar_t* wcstr, size_t pos, size_t length) const
    {
        return _storage.find_first_of(ConvertWideStringToMultiByteString(wcstr, length), pos);
    }

    X_API size_t find_first_of(const wchar_t* wcstr, size_t pos = 0) const
    {
        return _storage.find_first_of(ConvertWideStringToMultiByteString(wcstr), pos);
    }

    X_API size_t find_first_of(wchar_t wc, size_t pos = 0) const
    {
      wchar_t value[] = {wc,'\0'};
      return _storage.find_first_of(ConvertWideStringToMultiByteString(value), pos);
    }

//------------------------------------------------------------------------------
// find_last_of()
//------------------------------------------------------------------------------

    X_API size_t find_last_of(const XString& other, size_t pos = std::string::npos) const
    {
        return _storage.find_last_of(other._storage, pos);
    }

    X_API size_t find_last_of(const std::string& stdstr, size_t pos = std::string::npos) const
    {
        return _storage.find_last_of(stdstr, pos);
    }

    X_API size_t find_last_of(const char* cstr, size_t pos, size_t length) const
    {
        return _storage.find_last_of(cstr, pos, length);
    }

    X_API size_t find_last_of(const char* cstr, size_t pos = std::string::npos) const
    {
        return _storage.find_last_of(cstr, pos);
    }

    X_API size_t find_last_of(char c, size_t pos = std::string::npos) const
    {
        return _storage.find_last_of(c, pos);
    }


    X_API size_t find_last_of(const wchar_t* wcstr, size_t pos, size_t length) const
    {
        return _storage.find_last_of(ConvertWideStringToMultiByteString(wcstr, length), pos);
    }

    X_API size_t find_last_of(const wchar_t* wcstr, size_t pos = std::string::npos) const
    {
        return _storage.find_last_of(ConvertWideStringToMultiByteString(wcstr), pos);
    }

    X_API size_t find_last_of(wchar_t wc, size_t pos = std::string::npos) const
    {
      wchar_t value[] = {wc,'\0'};
      return _storage.find_last_of(ConvertWideStringToMultiByteString(value), pos);
    }

//------------------------------------------------------------------------------
// find_first_not_of()
//------------------------------------------------------------------------------

    X_API size_t find_first_not_of(const XString& other, size_t pos = 0) const
    {
        return _storage.find_first_not_of(other._storage, pos);
    }

    X_API size_t find_first_not_of(const std::string& stdstr, size_t pos = 0) const
    {
        return _storage.find_first_not_of(stdstr, pos);
    }

    X_API size_t find_first_not_of(const char* cstr, size_t pos, size_t length) const
    {
        return _storage.find_first_not_of(cstr, pos, length);
    }

    X_API size_t find_first_not_of(const char* cstr, size_t pos = 0) const
    {
        return _storage.find_first_not_of(cstr, pos);
    }

    X_API size_t find_first_not_of(char c, size_t pos = 0) const
    {
        return _storage.find_first_not_of(c, pos);
    }

    X_API size_t find_first_not_of(const wchar_t* wcstr, size_t pos, size_t length) const
    {
        return _storage.find_first_not_of(ConvertWideStringToMultiByteString(wcstr, length), pos);
    }

    X_API size_t find_first_not_of(const wchar_t* wcstr, size_t pos = 0) const
    {
        return _storage.find_first_not_of(ConvertWideStringToMultiByteString(wcstr), pos);
    }

    X_API size_t find_first_not_of(wchar_t wc, size_t pos = 0) const
    {
      wchar_t value[] = {wc,'\0'};
      return _storage.find_first_not_of(ConvertWideStringToMultiByteString(value), pos);
    }

//------------------------------------------------------------------------------
// find_last_not_of()
//------------------------------------------------------------------------------

    X_API size_t find_last_not_of(const XString& other, size_t pos = std::string::npos) const
    {
        return _storage.find_last_not_of(other._storage, pos);
    }

    X_API size_t find_last_not_of(const std::string& stdstr, size_t pos = std::string::npos) const
    {
        return _storage.find_last_not_of(stdstr, pos);
    }

    X_API size_t find_last_not_of(const char* cstr, size_t pos, size_t length) const
    {
        return _storage.find_last_not_of(cstr, pos, length);
    }

    X_API size_t find_last_not_of(const char* cstr, size_t pos = std::string::npos) const
    {
        return _storage.find_last_not_of(cstr, pos);
    }

    X_API size_t find_last_not_of(char c, size_t pos = std::string::npos) const
    {
        return _storage.find_last_not_of(c, pos);
    }

    X_API size_t find_last_not_of(const wchar_t* wcstr, size_t pos, size_t length) const
    {
        return _storage.find_last_not_of(ConvertWideStringToMultiByteString(wcstr, length), pos);
    }

    X_API size_t find_last_not_of(const wchar_t* wcstr, size_t pos = std::string::npos) const
    {
        return _storage.find_last_not_of(ConvertWideStringToMultiByteString(wcstr), pos);
    }

    X_API size_t find_last_not_of(wchar_t wc, size_t pos = std::string::npos) const
    {
      wchar_t value[] = {wc,'\0'};
      return _storage.find_last_not_of(ConvertWideStringToMultiByteString(value), pos);
    }

//------------------------------------------------------------------------------
// substr()
//------------------------------------------------------------------------------

    X_API XString substr(size_t pos = 0, size_t length = std::string::npos) const {return _storage.substr(pos, length);}

//------------------------------------------------------------------------------
// compare()
//------------------------------------------------------------------------------

    X_API int compare(const XString& other) const {return _storage.compare(other._storage);}
    X_API int compare(const std::string& stdstr) const {return _storage.compare(stdstr);}
    X_API int compare(const char* cstr) const {return _storage.compare(cstr);}
    X_API int compare(const wchar_t* wcstr) const {return _storage.compare(ConvertWideStringToMultiByteString(wcstr));}

    X_API int compare(size_t pos, size_t length, const XString& other) const
    {
        return _storage.compare(pos, length, other._storage);
    }

    X_API int compare(size_t pos, size_t length, const std::string& stdstr) const
    {
        return _storage.compare(pos, length, stdstr);
    }

    X_API int compare(size_t pos, size_t length, const char* cstr) const
    {
        return _storage.compare(pos, length, cstr);
    }

    X_API int compare(size_t pos, size_t length, const wchar_t* wcstr) const
    {
        return compare(pos, length, ConvertWideStringToMultiByteString(wcstr));
    }


    X_API int compare(size_t pos1, size_t length1, const XString& other, size_t pos2 , size_t length2) const
    {
        return _storage.compare(pos1, length1, other._storage, pos2, length2);
    }

    X_API int compare(size_t pos1, size_t length1, const std::string& stdstr, size_t pos2 , size_t length2) const
    {
        return _storage.compare(pos1, length1, stdstr, pos2, length2);
    }

    X_API int compare(size_t pos, size_t length1, const char* cstr, size_t length2) const
    {
        return _storage.compare(pos, length1, cstr, length2);
    }

    X_API int compare(size_t pos, size_t length1, const wchar_t* wcstr, size_t length2) const
    {
        return compare(pos, length1, ConvertWideStringToMultiByteString(wcstr, length2));
    }

    //------------------------------------------------------------------------------------------------------
    //------------------------- Static conversion methods for dealing with wchar_t -------------------------
    //------------------------------------------------------------------------------------------------------
    /**
     * This method is used by our string to convert your platform's native wide character representation
     * to UTF-8, please note that you don't need to use this directly as you can just assign wchar_t's to
     * the string, or pass them to a constructor. Also please note that you should <b>prefer UTF-8 ALWAYS!!!</b>
     */
    X_API static std::string ConvertWideStringToMultiByteString(const wchar_t* str);

    /// Overload which takes a length. str may or may not be zero-terminated. If it _is_
    /// zero-terminated, and the zero '\0' is earlier then length, then that is considered
    /// to be the end of the string.
    X_API static std::string ConvertWideStringToMultiByteString(const wchar_t* str, size_t length);

    /**
     * @note you should preferentially use this method if you want a wide string, however in most cases
     * you should prefer working in UTF-8 which can be natively represented with our string.
     */
    X_API static std::wstring ConvertMultiByteStringToWideString(const std::string& str);

    /**
     * @warning DANGER!!! You should only be using this method if you want to explicitly use UTF-16
     * instead of your platform's wchar_t, e.g. this should only be done if you are reading data writen
     * as UTF-16 and you are on a machine that is UTF-32 for wide characters, otherwise you whould
     * read it as wide data, and then pass to XString.
     */
    X_API static std::string ConvertUtf16StringToMultiByteString(const uint16_t* str);

    /// Overload which takes a length. str may or may not be zero-terminated. If it _is_
    /// zero-terminated, and the zero '\0' is earlier then length, then that is considered
    /// to be the end of the string.
    X_API static std::string ConvertUtf16StringToMultiByteString(const uint16_t* str, size_t length);

    /**
     * @warning DANGER!!! You should not be using this method unless you are required to explicity
     * output UTF-16, otherwise you should either use the wstring which is your platform's native wide
     * character type, or BEST UTF-8 which is our multi-byte representation as this is portable and
     * easiest to work with, so you would only use this if e.g. you are required to write UTF-16 to a file,
     * for a spec that some moron wrote, etc.
     */
    X_API static std::vector<uint16_t> ConvertMultiByteStringToUtf16String(const std::string& str);

    /**
     * @warning DANGER!!! You should only be using this method if you want to explicitly use UTF-32
     * instead of your platform's wchar_t, e.g. this should only be done if you are reading data writen
     * as UTF-32 and you are on a machine that is UTF-16 for wide characters, otherwise you whould
     * read it as wide data, and then pass to XString.
     */
    X_API static std::string ConvertUtf32StringToMultiByteString(const uint32_t* str);

    /// Overload which takes a length. str may or may not be zero-terminated. If it _is_
    /// zero-terminated, and the zero '\0' is earlier then length, then that is considered
    /// to be the end of the string.
    X_API static std::string ConvertUtf32StringToMultiByteString(const uint32_t* str, size_t length);

    /**
     * @warning DANGER!!! You should not be using this method unless you are required to explicity
     * output UTF-32, otherwise you should either use the wstring which is your platform's native wide
     * character type, or BEST UTF-8 which is our multi-byte representation as this is portable and
     * easiest to work with, so you would only use this if e.g. you are required to write UTF-32 to a file,
     * for a spec that some moron wrote, etc.
     */
    X_API static std::vector<uint32_t> ConvertMultiByteStringToUtf32String(const std::string& str);
    //------------------------------------------------------------------------------------------------------

    X_API static bool IsSpace(char a){return (a == ' ' || a == '\n' || a == '\t' || a == '\r');}

private:
//------------------------------------------------------------------------------
// Private Member Functions
//------------------------------------------------------------------------------

    static char _EncodeByte(uint8_t u);
    static CHAR_CLASS _GetCharClass(char c);
    static bool _IsValidURICharacter(char c);

    template <typename T>
    bool _Contains(const T& other) const;

    template <typename T>
    void _Split(const T& delim, size_t delimLen, std::vector<XString>& parts) const;

    template <typename T>
    XString _EraseAll(const T& toBeErased) const;

//------------------------------------------------------------------------------
// Private Member Variables
//------------------------------------------------------------------------------

    std::string _storage;

};

X_API int Compare( const XString& left, const XString& right, void* context );

X_API int Compare( const char* c_string, const XString& intString, void* context );

X_API int Compare( const XString& intString, const char* c_string, void* context );

//------------------------------------------------------------------------------
// Templated Member Functions (must be defined in header)
//------------------------------------------------------------------------------

template <typename T>
bool XString::_Contains(const T& other) const
{
    return _storage.find(other) != std::string::npos;
}

template <typename T>
void XString::_Split(const T& delim, size_t delimLen, OUT std::vector<XString>& parts) const
{
    size_t begin = 0;
    size_t end = 0;

    parts.clear();

    while(1)
    {
        end = _storage.find(delim, begin);
        const XString str = substr(begin, end - begin);

        if(!str.empty())
            parts.push_back(str);

        if(end == std::string::npos)
            break;

        begin = end + delimLen;
    }
}

template <typename T>
XString XString::_EraseAll(const T& toBeErased) const
{
    try
    {
        std::vector<XString> splitList;
        Split(toBeErased, splitList);

        XString str;

        for(std::vector<XString>::iterator iter = splitList.begin(),
                                           end  = splitList.end();
            iter != end;
            ++iter)
        {
            str += *iter;
        }

        return str;
    }
    catch(std::exception)
    {
        X_THROW(("std::list threw in XString::EraseAll()."));
    }
}

}

#ifdef WIN32 // Enabling overzealous / dumb visual studio warning...

#pragma warning(pop)

#endif

#endif

