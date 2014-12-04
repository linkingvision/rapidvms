
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

#ifndef cppkit_string_h
#define cppkit_string_h

#include "cppkit/os/ck_exports.h"
#include "cppkit/ck_exception.h"

#include <cstring>
#include <list>
#include <string>
#include <vector>
#include <memory>

#ifdef IS_WINDOWS
#pragma warning(push)
#pragma warning(disable:4127)
#endif

namespace cppkit
{

static const char base64_encoding_table[64] =
{
    'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P',
    'Q','R','S','T','U','V','W','X','Y','Z','a','b','c','d','e','f',
    'g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v',
    'w','x','y','z','0','1','2','3','4','5','6','7','8','9','+','/'
};

class ck_exception;
class ck_memory;

class ck_string
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
    CK_API ck_string()
        : _storage()
    {}

    CK_API ck_string(const ck_string& other)
        : _storage(other._storage)
    {}

    CK_API ck_string(ck_string&& other) noexcept
        : _storage(std::move(other._storage))
    {}

    CK_API ck_string(const std::string& stdstr)
        : _storage(stdstr)
    {}

    CK_API ck_string(const char* cstr)
        : _storage(cstr)
    {}

    CK_API ck_string(char c)
        : _storage(1, c)
    {}

    CK_API ck_string(const ck_string& other, size_type index, size_type length)
        : _storage(other._storage, index, length)
    {}

    CK_API ck_string(const std::string& stdstr, size_type index, size_type length)
        : _storage(stdstr, index, length)
    {}

    CK_API ck_string(const char* cstr, size_type length)
        : _storage(cstr, length)
    {}

    CK_API ck_string(std::string::size_type length, char ch)
        : _storage(length, ch)
    {}

    template <typename InputIterator>
    CK_API ck_string(InputIterator begin, InputIterator end)
        : _storage(begin, end)
    {}

    CK_API ck_string(const wchar_t* wcstr)
        : _storage(convert_wide_string_to_multi_byte_string(wcstr))
    {}

    CK_API ck_string(const wchar_t* wcstr, size_t length)
        : _storage(convert_wide_string_to_multi_byte_string(wcstr, length))
    {}

    CK_API ck_string(wchar_t wc)
        : _storage()
    {
        wchar_t value[] = {wc,'\0'};
        _storage = convert_wide_string_to_multi_byte_string(value);
    }

    CK_API ~ck_string() noexcept {}

    /// cppkit::ck_string implicitly casts to std::string& so that
    /// it can be used as a std::string without overhead.
    CK_API operator std::string&() {return _storage;}

    /// cppkit::ck_string implicitly casts to const std::string& so
    /// that it can be used as a std::string without overhead.
    CK_API operator const std::string&() const {return _storage;}

    CK_API ck_string& operator=(const ck_string& other)
    {
        _storage = other._storage;
        return *this;
    }

    CK_API ck_string& operator=(ck_string&& other) noexcept
    {
        _storage = std::move(other._storage);
        return *this;
    }

    CK_API ck_string& operator=(const std::string& stdstr)
    {
        _storage = stdstr;
        return *this;
    }

    CK_API ck_string& operator=(const char* cstr)
    {
        _storage = cstr;
        return *this;
    }

    CK_API ck_string& operator=(char c)
    {
        _storage = c;
        return *this;
    }

    CK_API ck_string& operator=(const wchar_t* wcstr)
    {
        _storage = convert_wide_string_to_multi_byte_string(wcstr);
        return *this;
    }

    CK_API ck_string& operator=(wchar_t wc)
    {
        _storage = convert_wide_string_to_multi_byte_string(&wc);
        return *this;
    }

    /// For getting a std::string* instead of a std::string& like the implicit
    /// conversion operator does.
    CK_API std::string* std_str() {return &_storage;}

    /// For getting a const std::string* instead of a const std::string& like the
    ///implicit conversion operator does.
    CK_API const std::string* std_str() const {return &_storage;}

    /// \brief Created an ck_string from a formatted string.
    ///
    /// I64 and I32 flags are accepted (e.g. %I64d and %I32u). l and ll flags
    /// should be generally avoided just like long and long long should be
    /// generally avoided.
    ///@{
    CK_API static ck_string format(const char* fmt, ...);
    CK_API static ck_string format(const char* fmt, va_list& args);

    CK_API static ck_string from_int(int val, int base = 10);
    CK_API static ck_string from_uint(unsigned int val, int base = 10);
    CK_API static ck_string from_int8(int8_t val, int base = 10);
    CK_API static ck_string from_uint8(uint8_t val, int base = 10);
    CK_API static ck_string from_int16(int16_t val, int base = 10);
    CK_API static ck_string from_uint16(uint16_t val, int base = 10);
    CK_API static ck_string from_int32(int32_t val, int base = 10);
    CK_API static ck_string from_uint32(uint32_t val, int base = 10);
    CK_API static ck_string from_int64(int64_t val, int base = 10);
    CK_API static ck_string from_uint64(uint64_t val, int base = 10);
    CK_API static ck_string from_double(double val) {return format("%lf", val);}

    CK_API int to_int(bool truncate = false) const;
    CK_API int to_int(int base) const;
    CK_API unsigned int to_uint(bool truncate = false) const;
    CK_API unsigned int to_uint(int base) const;
    CK_API int8_t to_int8(bool truncate = false) const;
    CK_API int8_t to_int8(int base) const;
    CK_API uint8_t to_uint8(bool truncate = false) const;
    CK_API uint8_t to_uint8(int base) const;
    CK_API int16_t to_int16(bool truncate = false) const;
    CK_API int16_t to_int16(int base) const;
    CK_API uint16_t to_uint16(bool truncate = false) const;
    CK_API uint16_t to_uint16(int base) const;
    CK_API int32_t to_int32(bool truncate = false) const;
    CK_API int32_t to_int32(int base) const;
    CK_API uint32_t to_uint32(bool truncate = false) const;
    CK_API uint32_t to_uint32(int base) const;
    CK_API int64_t to_int64(bool truncate = false) const;
    CK_API int64_t to_int64(int base) const;
    CK_API uint64_t to_uint64(bool truncate = false) const;
    CK_API uint64_t to_uint64(int base) const;
    CK_API double to_double() const;

    /// base64 functionality
    CK_API static ck_string to_base64( const void* source, size_t length );
    CK_API std::shared_ptr<ck_memory> from_base64() const;

    // See RFC 3986
    CK_API ck_string uri_encode() const;
    CK_API ck_string uri_decode() const;

    CK_API ck_string to_lower() const;///@todo make UTF-8 safe
    CK_API ck_string to_upper() const;///@todo make UTF-8 safe

    CK_API bool starts_with(const ck_string& other) const;
    CK_API bool starts_with(const std::string& stdstr) const;
    CK_API bool starts_with(const char* cstr) const;
    CK_API bool starts_with(const wchar_t* wcstr) const;

    CK_API bool ends_with(const ck_string& other) const;
    CK_API bool ends_with(const std::string& stdstr) const;
    CK_API bool ends_with(const char* cstr) const;
    CK_API bool ends_with(const wchar_t* wcstr) const;

    CK_API bool contains(const ck_string& other) const {return _contains(other);}
    CK_API bool contains(const std::string& stdstr) const {return _contains(stdstr);}

    CK_API bool contains(const char* cstr) const
    {
        if(NULL == cstr)
            CK_THROW(("NULL string passed to ck_string::contains()."));
        return _contains(cstr);
    }

    CK_API bool contains(const wchar_t* wcstr) const
    {
        if(NULL == wcstr)
            CK_THROW(("NULL string passed to ck_string::contains(const wchar_t* wcstr)."));
        return _contains(convert_wide_string_to_multi_byte_string(wcstr));
    }

    CK_API bool is_integer(bool canHaveSign = true) const;
    CK_API bool is_hex(bool canHaveSign = true) const;
    CK_API bool is_double(bool canHaveSign = true) const;

    CK_API ck_string lstrip() const;
    CK_API ck_string rstrip() const;
    CK_API ck_string strip() const;

    CK_API ck_string strip_eol() const;

    CK_API std::vector<ck_string> split(char delim) const
    {
        std::vector<ck_string> parts;
        _split(delim, 1, parts);
        return parts;
    }

    CK_API std::vector<ck_string> split(const ck_string& delim) const
    {
        std::vector<ck_string> parts;
        _split(delim, delim._storage.size(), parts);
        return parts;
    }

    CK_API std::vector<ck_string> split(const std::string& delim) const
    {
        std::vector<ck_string> parts;
        _split(delim, delim.size(), parts);
        return parts;
    }

    CK_API std::vector<ck_string> split(const char* delim) const
    {
        if(!delim)
            CK_THROW(("NULL string passed to ck_string::split()."));
        std::vector<ck_string> parts;
        _split(delim, strlen(delim), parts);
        return parts;
    }

    CK_API std::vector<ck_string> split(wchar_t delim) const
    {
      wchar_t delimeter[] = {delim,'\0'};
      return split(convert_wide_string_to_multi_byte_string((wchar_t*)delimeter));
    }

    CK_API std::vector<ck_string> split(const wchar_t* delim) const
    {
        if(delim == NULL)
            CK_THROW(("NULL string passed to ck_string::split()."));
        return split(convert_wide_string_to_multi_byte_string(delim));
    }

    CK_API ck_string replace_all(char toBeReplaced, char toReplaceWith) const;
    CK_API ck_string replace_all(wchar_t toBeReplaced, wchar_t toReplaceWith) const;
    CK_API ck_string replace_all(const ck_string& toBeReplaced, const ck_string& toReplaceWith) const;

    CK_API ck_string erase_all(char toBeErased) const {return _erase_all(toBeErased);}
    CK_API ck_string erase_all(wchar_t toBeErased) const {return _erase_all(toBeErased);}
    CK_API ck_string erase_all(const ck_string& toBeErased) const {return _erase_all(toBeErased);}

    CK_API iterator begin() {return _storage.begin();}
    CK_API const const_iterator begin() const {return _storage.begin();}

    CK_API iterator end() {return _storage.end();}
    CK_API const const_iterator end() const {return _storage.end();}

    CK_API reverse_iterator rbegin() {return _storage.rbegin();}
    CK_API const const_reverse_iterator rbegin() const {return _storage.rbegin();}

    CK_API reverse_iterator rend() {return _storage.rend();}
    CK_API const const_reverse_iterator rend() const {return _storage.rend();}

    CK_API size_t size() const {return _storage.size();}
    CK_API size_t length() const {return _storage.length();}

    CK_API size_t max_size() const {return _storage.max_size();}

    CK_API void resize(size_t size, char c) {_storage.resize(size, c);}
    CK_API void resize(size_t size) {_storage.resize(size);}

    CK_API size_t capacity() const {return _storage.capacity();}

    CK_API void reserve(size_t res_arg = 0) {_storage.reserve(res_arg);}

    CK_API void clear() {_storage.clear();}

    CK_API bool empty() const {return _storage.empty();}

    CK_API const char& operator[](size_t pos) const {return _storage[pos];}
    CK_API char& operator[](size_t pos) {return _storage[pos];}

    CK_API const char& at(size_t pos) const {return _storage.at(pos);}
    CK_API char& at(size_t pos) {return _storage.at(pos);}

    CK_API friend std::ostream& operator<<(std::ostream& os, const ck_string& str) {
        return os << str._storage;
    }
    CK_API friend std::istream& operator>>(std::istream& is, ck_string& str) {
        return is >> str._storage;
    }
    CK_API friend bool operator ==(const ck_string& left, const ck_string& right) {
        return left._storage == right._storage;
    }
    CK_API friend bool operator ==(const ck_string& left, const std::string& right) {
        return left._storage == right;
    }
    CK_API friend bool operator ==(const std::string& left, const ck_string& right) {
        return left == right._storage;
    }
    CK_API friend bool operator ==(const ck_string& left, const char* right) {
        return left._storage == std::string(right);
    }
    CK_API friend bool operator ==(const char* left, const ck_string& right) {
        return std::string(left) == right._storage;
    }
    CK_API friend bool operator ==(const wchar_t* left, const ck_string& right) {
        return convert_wide_string_to_multi_byte_string(left) == right._storage;
    }

    CK_API friend bool operator !=(const ck_string& left, const ck_string& right) {
        return left._storage != right._storage;
    }
    CK_API friend bool operator !=(const ck_string& left, const std::string& right) {
        return left._storage != right;
    }
    CK_API friend bool operator !=(const std::string& left, const ck_string& right) {
        return left != right._storage;
    }
    CK_API friend bool operator !=(const ck_string& left, const char* right) {
        return left._storage != std::string(right);
    }
    CK_API friend bool operator !=(const char* left, const ck_string& right) {
        return std::string(left) != right._storage;
    }
    CK_API friend bool operator !=(const wchar_t* left, const ck_string& right) {
        return convert_wide_string_to_multi_byte_string(left) != right._storage;
    }

    CK_API friend bool operator <(const ck_string& left, const ck_string& right) {
        return left._storage < right._storage;
    }
    CK_API friend bool operator <(const ck_string& left, const std::string& right) {
        return left._storage < right;
    }
    CK_API friend bool operator <(const std::string& left, const ck_string& right) {
        return left < right._storage;
    }
    CK_API friend bool operator <(const ck_string& left, const char* right) {
        return left._storage < std::string(right);
    }
    CK_API friend bool operator <(const char* left, const ck_string& right) {
        return std::string(left) < right._storage;
    }
    CK_API friend bool operator <(const wchar_t* left, const ck_string& right) {
        return convert_wide_string_to_multi_byte_string(left) < right._storage;
    }

    CK_API friend bool operator >(const ck_string& left, const ck_string& right) {
        return left._storage > right._storage;
    }
    CK_API friend bool operator >(const ck_string& left, const std::string& right) {
        return left._storage > right;
    }
    CK_API friend bool operator >(const std::string& left, const ck_string& right) {
        return left > right._storage;
    }
    CK_API friend bool operator >(const ck_string& left, const char* right) {
        return left._storage > std::string(right);
    }
    CK_API friend bool operator >(const char* left, const ck_string& right) {
        return std::string(left) > right._storage;
    }
    CK_API friend bool operator >(const wchar_t* left, const ck_string& right) {
        return convert_wide_string_to_multi_byte_string(left) > right._storage;
    }

    CK_API friend bool operator <=(const ck_string& left, const ck_string& right) {
        return left._storage <= right._storage;
    }
    CK_API friend bool operator <=(const ck_string& left, const std::string& right) {
        return left._storage <= right;
    }
    CK_API friend bool operator <=(const std::string& left, const ck_string& right) {
        return left <= right._storage;
    }
    CK_API friend bool operator <=(const ck_string& left, const char* right) {
        return left._storage <= std::string(right);
    }
    CK_API friend bool operator <=(const char* left, const ck_string& right) {
        return std::string(left) <= right._storage;
    }
    CK_API friend bool operator <=(const wchar_t* left, const ck_string& right) {
        return convert_wide_string_to_multi_byte_string(left) <= right._storage;
    }

    CK_API friend bool operator >=(const ck_string& left, const ck_string& right) {
        return left._storage >= right._storage;
    }
    CK_API friend bool operator >=(const ck_string& left, const std::string& right) {
        return left._storage >= right;
    }
    CK_API friend bool operator >=(const std::string& left, const ck_string& right) {
        return left >= right._storage;
    }
    CK_API friend bool operator >=(const ck_string& left, const char* right) {
        return left._storage >= std::string(right);
    }
    CK_API friend bool operator >=(const char* left, const ck_string& right) {
        return std::string(left) >= right._storage;
    }
    CK_API friend bool operator >=(const wchar_t* left, const ck_string& right) {
        return convert_wide_string_to_multi_byte_string(left) >= right._storage;
    }

    CK_API friend ck_string operator +(const ck_string& left, const ck_string& right) {
        return left._storage + right._storage;
    }
    CK_API friend ck_string operator +(const ck_string& left, const std::string& right) {
        return left._storage + right;
    }
    CK_API friend ck_string operator +(const std::string& left, const ck_string& right) {
        return left + right._storage;
    }
    CK_API friend ck_string operator +(const ck_string& left, const char* right) {
        return left._storage + right;
    }
    CK_API friend ck_string operator +(const char* left, const ck_string& right) {
        return left + right._storage;
    }
    CK_API friend ck_string operator +(char left, const ck_string& right) {
        return left + right._storage;
    }
    CK_API friend ck_string operator +(const ck_string& left, char right) {
        return left._storage + right;
    }
    CK_API friend ck_string operator +(const wchar_t* left, const ck_string& right) {
        return convert_wide_string_to_multi_byte_string(left) + right._storage;
    }

    CK_API friend ck_string operator +(wchar_t left, const ck_string& right)
    {
        wchar_t value[] = {left,'\0'};
        return convert_wide_string_to_multi_byte_string(value) + right._storage;
    }

    CK_API ck_string& operator +=(const ck_string& other)
    {
        _storage += other._storage;
        return *this;
    }

    CK_API ck_string& operator +=(const std::string& stdstr)
    {
        _storage += stdstr;
        return *this;
    }

    CK_API ck_string& operator +=(const char* cstr)
    {
        _storage += cstr;
        return *this;
    }

    CK_API ck_string& operator +=(char c)
    {
        _storage += c;
        return *this;
    }

    CK_API ck_string& operator +=(const wchar_t* wcstr)
    {
        _storage += convert_wide_string_to_multi_byte_string(wcstr);
        return *this;
    }

    CK_API ck_string& operator +=(wchar_t wc)
    {
        wchar_t value[] = {wc,'\0'};
        _storage += convert_wide_string_to_multi_byte_string(value);
        return *this;
    }

    CK_API ck_string& append(const ck_string& other)
    {
        _storage.append(other._storage);
        return *this;
    }

    CK_API ck_string& append(const std::string& stdstr)
    {
        _storage.append(stdstr);
        return *this;
    }

    CK_API ck_string& append(const ck_string& other, size_t pos, size_t length)
    {
        _storage.append(other._storage, pos, length);
        return *this;
    }

    CK_API ck_string& append(const std::string& stdstr, size_t pos, size_t length)
    {
        _storage.append(stdstr, pos, length);
        return *this;
    }

    CK_API ck_string& append(const char* cstr, size_t length)
    {
        _storage.append(cstr, length);
        return *this;
    }

    CK_API ck_string& append(const char* cstr)
    {
        _storage.append(cstr);
        return *this;
    }

    CK_API ck_string& append(size_t length, char c)
    {
        _storage.append(length, c);
        return *this;
    }

    CK_API ck_string& append(const wchar_t* wstr)
    {
      _storage.append(convert_wide_string_to_multi_byte_string(wstr));
      return *this;
    }

    CK_API ck_string& append(wchar_t wc)
    {
      wchar_t value[] = {wc,'\0'};
      return append(value);
    }

    template <typename InputIterator>
    CK_API ck_string& append(InputIterator begin, InputIterator end)
    {
        _storage.append(begin, end);
        return *this;
    }

    CK_API void push_back(char c) {_storage.push_back(c);}

    CK_API void push_back(wchar_t wc)
    {
        wchar_t value[] = {wc,'\0'};
        std::string temp = convert_wide_string_to_multi_byte_string(value);
        for ( size_t i = 0; i < temp.length(); ++i )
            _storage.push_back(temp.c_str()[i]);
    }

    CK_API ck_string& assign(const ck_string& other)
    {
        _storage.assign(other._storage);
        return *this;
    }

    CK_API ck_string& assign(const std::string& stdstr)
    {
        _storage.assign(stdstr);
        return *this;
    }

    CK_API ck_string& assign(const ck_string& other, size_t pos, size_t length)
    {
        _storage.assign(other._storage, pos, length);
        return *this;
    }

    CK_API ck_string& assign(const std::string& stdstr, size_t pos, size_t length)
    {
        _storage.assign(stdstr, pos, length);
        return *this;
    }

    CK_API ck_string& assign(const char* cstr, size_t length)
    {
        _storage.assign(cstr, length);
        return *this;
    }

    CK_API ck_string& assign(const char* cstr)
    {
        _storage.assign(cstr);
        return *this;
    }

    CK_API ck_string& assign(size_t length, char c)
    {
        _storage.assign(length, c);
        return *this;
    }

    CK_API ck_string& assign(const wchar_t* wcstr)
    {
        _storage.assign(convert_wide_string_to_multi_byte_string(wcstr));
        return *this;
    }

    CK_API ck_string& assign( wchar_t wc)
    {
        wchar_t value[] = {wc,'\0'};
        return assign(convert_wide_string_to_multi_byte_string(value));
    }

    template <typename InputIterator>
    CK_API ck_string& assign(InputIterator begin, InputIterator end)
    {
        _storage.assign(begin, end);
        return *this;
    }

    CK_API ck_string& insert(size_t pos, const ck_string& other)
    {
        _storage.insert(pos, other._storage);
        return *this;
    }

    CK_API ck_string& insert(size_t pos, const std::string& stdstr)
    {
        _storage.insert(pos, stdstr);
        return *this;
    }

    CK_API ck_string& insert(size_t pos1, const ck_string& other, size_t pos2, size_t length)
    {
        _storage.insert(pos1, other._storage, pos2, length);
        return *this;
    }

    CK_API ck_string& insert(size_t pos1, const std::string& stdstr, size_t pos2, size_t length)
    {
        _storage.insert(pos1, stdstr, pos2, length);
        return *this;
    }

    CK_API ck_string& insert(size_t pos, const char* cstr, size_t length)
    {
        _storage.insert(pos, cstr, length);
        return *this;
    }

    CK_API ck_string& insert(size_t pos, const char* cstr)
    {
        _storage.insert(pos, cstr);
        return *this;
    }

    CK_API ck_string& insert(size_t pos, size_t length, char c)
    {
        _storage.insert(pos, length , c);
        return *this;
    }

    CK_API iterator insert(iterator p, char c) {return _storage.insert(p, c);}
    CK_API void insert(iterator p, size_t length, char c) {_storage.insert(p, length, c);}

    CK_API ck_string& insert(size_t pos, const wchar_t* wcstr)
    {
        return insert(pos, convert_wide_string_to_multi_byte_string(wcstr));
    }

    CK_API ck_string& insert(size_t pos, wchar_t wc)
    {
        wchar_t value[] = {wc,'\0'};
        return insert(pos,value);
    }

    CK_API iterator insert(iterator p, wchar_t wc)
    {
        wchar_t value[] = {wc,'\0'};
        std::string temp = convert_wide_string_to_multi_byte_string(value);
        for ( size_t i = 9; i < temp.length(); ++i )
            insert(p,temp.c_str()[i]);
        return p;
    }

    template <typename InputIterator>
    CK_API void insert(iterator p, InputIterator begin, InputIterator end) {_storage.insert(p, begin, end);}

    CK_API ck_string& erase(size_t pos = 0, size_t length = std::string::npos)
    {
        _storage.erase(pos, length);
        return *this;
    }

    CK_API iterator erase(iterator pos) {return _storage.erase(pos);}
    CK_API iterator erase(iterator begin, iterator end) {return _storage.erase(begin, end);}

    CK_API ck_string& replace(size_t pos, size_t length, const ck_string& other)
    {
        _storage.replace(pos, length, other._storage);
        return *this;
    }

    CK_API ck_string& replace(size_t pos, size_t length, const std::string& stdstr)
    {
        _storage.replace(pos, length, stdstr);
        return *this;
    }

    CK_API ck_string& replace(iterator begin, iterator end, const ck_string& other)
    {
        _storage.replace(begin, end, other._storage);
        return *this;
    }

    CK_API ck_string& replace(iterator begin, iterator end, const std::string& stdstr)
    {
        _storage.replace(begin, end, stdstr);
        return *this;
    }

    CK_API ck_string& replace(size_t pos1, size_t n1, const ck_string& other, size_t pos2, size_t n2)
    {
        _storage.replace(pos1, n1, other._storage, pos2, n2);
        return *this;
    }

    CK_API ck_string& replace(size_t pos1, size_t n1, const std::string& stdstr, size_t pos2, size_t n2)
    {
        _storage.replace(pos1, n1, stdstr, pos2, n2);
        return *this;
    }

    CK_API ck_string& replace(size_t pos, size_t n1, const char* cstr, size_t n2)
    {
        _storage.replace(pos, n1, cstr, n2);
        return *this;
    }

    CK_API ck_string& replace(iterator begin, iterator end, const char* cstr, size_t length)
    {
        _storage.replace(begin, end, cstr, length);
        return *this;
    }

    CK_API ck_string& replace(size_t pos, size_t length, const char* cstr)
    {
        _storage.replace(pos, length, cstr);
        return *this;
    }

    CK_API ck_string& replace(iterator begin, iterator end, const char* cstr)
    {
        _storage.replace(begin, end, cstr);
        return *this;
    }

    CK_API ck_string& replace(size_t pos, size_t n1, size_t n2, char c)
    {
        _storage.replace(pos, n1, n2, c);
        return *this;
    }

    CK_API ck_string& replace(iterator begin, iterator end, size_t length, char c)
    {
        _storage.replace(begin, end, length, c);
        return *this;
    }

    CK_API ck_string& replace(size_t pos, size_t n1, const wchar_t* wcstr)
    {
        return replace(pos, n1, convert_wide_string_to_multi_byte_string(wcstr));
    }

    CK_API ck_string& replace(iterator begin, iterator end, const wchar_t* wcstr)
    {
        return replace(begin, end, convert_wide_string_to_multi_byte_string(wcstr));
    }

    CK_API ck_string& replace(size_t pos, size_t n1, size_t /*n2*/, wchar_t wc)
    {
      wchar_t value[] = {wc,'\0'};
      return replace(pos, n1, convert_wide_string_to_multi_byte_string(value));
    }

    CK_API ck_string& replace(iterator begin, iterator end, size_t /*length*/, wchar_t wc)
    {
      wchar_t value[] = {wc,'\0'};
      return replace(begin, end, convert_wide_string_to_multi_byte_string(value));
    }

    template <typename InputIterator>
    CK_API ck_string& replace(iterator begin1, iterator end1, InputIterator begin2, InputIterator end2)
    {
        _storage.replace(begin1, end1, begin2, end2);
        return *this;
    }

    CK_API void swap(ck_string& other) {_storage.swap(other._storage);}
    CK_API void swap(std::string& stdstr) {_storage.swap(stdstr);}

    CK_API const char* c_str() const {return _storage.c_str();}

    /**
     * @warning Returns a pointer to allocated memory that you the caller are expected to clean up.
     *
     */
    CK_API std::wstring get_wide_string() const
    {
        return convert_multi_byte_string_to_wide_string(_storage.c_str());
    }

    CK_API const char* data() const {return _storage.data();}

    CK_API std::allocator<char> get_allocator() const {return _storage.get_allocator();}

    CK_API size_t find(const ck_string& other, size_t pos = 0) const
    {
        return _storage.find(other._storage, pos);
    }

    CK_API size_t find(const std::string& stdstr, size_t pos = 0) const
    {
        return _storage.find(stdstr, pos);
    }

    CK_API size_t find(const char* cstr, size_t pos, size_t length) const
    {
        return _storage.find(cstr, pos, length);
    }

    CK_API size_t find(const char* cstr, size_t pos = 0) const
    {
        return _storage.find(cstr, pos);
    }

    CK_API size_t find(char c, size_t pos = 0) const
    {
        return _storage.find(c, pos);
    }

    CK_API size_t find(const wchar_t* wcstr, size_t pos, size_t length) const
    {
        return _storage.find(convert_wide_string_to_multi_byte_string(wcstr, length), pos);
    }

    CK_API size_t find(const wchar_t* wcstr, size_t pos = 0) const
    {
        return _storage.find(convert_wide_string_to_multi_byte_string(wcstr), pos);
    }

    CK_API size_t find(wchar_t wc, size_t pos = 0) const
    {
        wchar_t value[] = {wc,'\0'};
        return _storage.find(convert_wide_string_to_multi_byte_string(value), pos);
    }

    CK_API size_t rfind(const ck_string& other, size_t pos = std::string::npos) const
    {
        return _storage.rfind(other._storage, pos);
    }

    CK_API size_t rfind(const std::string& stdstr, size_t pos = std::string::npos) const
    {
        return _storage.rfind(stdstr, pos);
    }

    CK_API size_t rfind(const char* cstr, size_t pos, size_t length) const
    {
        return _storage.rfind(cstr, pos, length);
    }

    CK_API size_t rfind(const char* cstr, size_t pos = std::string::npos) const
    {
        return _storage.rfind(cstr, pos);
    }

    CK_API size_t rfind(char c, size_t pos = std::string::npos) const
    {
        return _storage.rfind(c, pos);
    }

    CK_API size_t rfind(const wchar_t* wcstr, size_t pos, size_t length) const
    {
        return _storage.rfind(convert_wide_string_to_multi_byte_string(wcstr, length), pos);
    }

    CK_API size_t rfind(const wchar_t* wcstr, size_t pos = std::string::npos) const
    {
        return _storage.rfind(convert_wide_string_to_multi_byte_string(wcstr), pos);
    }

    CK_API size_t rfind(wchar_t wc, size_t pos = std::string::npos) const
    {
      wchar_t value[] = {wc,'\0'};
        return _storage.rfind(convert_wide_string_to_multi_byte_string(value), pos);
    }

    CK_API size_t find_first_of(const ck_string& other, size_t pos = 0) const
    {
        return _storage.find_first_of(other._storage, pos);
    }

    CK_API size_t find_first_of(const std::string& stdstr, size_t pos = 0) const
    {
        return _storage.find_first_of(stdstr, pos);
    }

    CK_API size_t find_first_of(const char* cstr, size_t pos, size_t length) const
    {
        return _storage.find_first_of(cstr, pos, length);
    }

    CK_API size_t find_first_of(const char* cstr, size_t pos = 0) const
    {
        return _storage.find_first_of(cstr, pos);
    }

    CK_API size_t find_first_of(char c, size_t pos = 0) const
    {
        return _storage.find_first_of(c, pos);
    }

    CK_API size_t find_first_of(const wchar_t* wcstr, size_t pos, size_t length) const
    {
        return _storage.find_first_of(convert_wide_string_to_multi_byte_string(wcstr, length), pos);
    }

    CK_API size_t find_first_of(const wchar_t* wcstr, size_t pos = 0) const
    {
        return _storage.find_first_of(convert_wide_string_to_multi_byte_string(wcstr), pos);
    }

    CK_API size_t find_first_of(wchar_t wc, size_t pos = 0) const
    {
      wchar_t value[] = {wc,'\0'};
      return _storage.find_first_of(convert_wide_string_to_multi_byte_string(value), pos);
    }

    CK_API size_t find_last_of(const ck_string& other, size_t pos = std::string::npos) const
    {
        return _storage.find_last_of(other._storage, pos);
    }

    CK_API size_t find_last_of(const std::string& stdstr, size_t pos = std::string::npos) const
    {
        return _storage.find_last_of(stdstr, pos);
    }

    CK_API size_t find_last_of(const char* cstr, size_t pos, size_t length) const
    {
        return _storage.find_last_of(cstr, pos, length);
    }

    CK_API size_t find_last_of(const char* cstr, size_t pos = std::string::npos) const
    {
        return _storage.find_last_of(cstr, pos);
    }

    CK_API size_t find_last_of(char c, size_t pos = std::string::npos) const
    {
        return _storage.find_last_of(c, pos);
    }

    CK_API size_t find_last_of(const wchar_t* wcstr, size_t pos, size_t length) const
    {
        return _storage.find_last_of(convert_wide_string_to_multi_byte_string(wcstr, length), pos);
    }

    CK_API size_t find_last_of(const wchar_t* wcstr, size_t pos = std::string::npos) const
    {
        return _storage.find_last_of(convert_wide_string_to_multi_byte_string(wcstr), pos);
    }

    CK_API size_t find_last_of(wchar_t wc, size_t pos = std::string::npos) const
    {
      wchar_t value[] = {wc,'\0'};
      return _storage.find_last_of(convert_wide_string_to_multi_byte_string(value), pos);
    }

    CK_API size_t find_first_not_of(const ck_string& other, size_t pos = 0) const
    {
        return _storage.find_first_not_of(other._storage, pos);
    }

    CK_API size_t find_first_not_of(const std::string& stdstr, size_t pos = 0) const
    {
        return _storage.find_first_not_of(stdstr, pos);
    }

    CK_API size_t find_first_not_of(const char* cstr, size_t pos, size_t length) const
    {
        return _storage.find_first_not_of(cstr, pos, length);
    }

    CK_API size_t find_first_not_of(const char* cstr, size_t pos = 0) const
    {
        return _storage.find_first_not_of(cstr, pos);
    }

    CK_API size_t find_first_not_of(char c, size_t pos = 0) const
    {
        return _storage.find_first_not_of(c, pos);
    }

    CK_API size_t find_first_not_of(const wchar_t* wcstr, size_t pos, size_t length) const
    {
        return _storage.find_first_not_of(convert_wide_string_to_multi_byte_string(wcstr, length), pos);
    }

    CK_API size_t find_first_not_of(const wchar_t* wcstr, size_t pos = 0) const
    {
        return _storage.find_first_not_of(convert_wide_string_to_multi_byte_string(wcstr), pos);
    }

    CK_API size_t find_first_not_of(wchar_t wc, size_t pos = 0) const
    {
      wchar_t value[] = {wc,'\0'};
      return _storage.find_first_not_of(convert_wide_string_to_multi_byte_string(value), pos);
    }

    CK_API size_t find_last_not_of(const ck_string& other, size_t pos = std::string::npos) const
    {
        return _storage.find_last_not_of(other._storage, pos);
    }

    CK_API size_t find_last_not_of(const std::string& stdstr, size_t pos = std::string::npos) const
    {
        return _storage.find_last_not_of(stdstr, pos);
    }

    CK_API size_t find_last_not_of(const char* cstr, size_t pos, size_t length) const
    {
        return _storage.find_last_not_of(cstr, pos, length);
    }

    CK_API size_t find_last_not_of(const char* cstr, size_t pos = std::string::npos) const
    {
        return _storage.find_last_not_of(cstr, pos);
    }

    CK_API size_t find_last_not_of(char c, size_t pos = std::string::npos) const
    {
        return _storage.find_last_not_of(c, pos);
    }

    CK_API size_t find_last_not_of(const wchar_t* wcstr, size_t pos, size_t length) const
    {
        return _storage.find_last_not_of(convert_wide_string_to_multi_byte_string(wcstr, length), pos);
    }

    CK_API size_t find_last_not_of(const wchar_t* wcstr, size_t pos = std::string::npos) const
    {
        return _storage.find_last_not_of(convert_wide_string_to_multi_byte_string(wcstr), pos);
    }

    CK_API size_t find_last_not_of(wchar_t wc, size_t pos = std::string::npos) const
    {
      wchar_t value[] = {wc,'\0'};
      return _storage.find_last_not_of(convert_wide_string_to_multi_byte_string(value), pos);
    }

    CK_API ck_string substr(size_t pos = 0, size_t length = std::string::npos) const {return _storage.substr(pos, length);}

    CK_API int compare(const ck_string& other) const {return _storage.compare(other._storage);}
    CK_API int compare(const std::string& stdstr) const {return _storage.compare(stdstr);}
    CK_API int compare(const char* cstr) const {return _storage.compare(cstr);}
    CK_API int compare(const wchar_t* wcstr) const
    {
        return _storage.compare(convert_wide_string_to_multi_byte_string(wcstr));
    }

    CK_API int compare(size_t pos, size_t length, const ck_string& other) const
    {
        return _storage.compare(pos, length, other._storage);
    }

    CK_API int compare(size_t pos, size_t length, const std::string& stdstr) const
    {
        return _storage.compare(pos, length, stdstr);
    }

    CK_API int compare(size_t pos, size_t length, const char* cstr) const
    {
        return _storage.compare(pos, length, cstr);
    }

    CK_API int compare(size_t pos, size_t length, const wchar_t* wcstr) const
    {
        return compare(pos, length, convert_wide_string_to_multi_byte_string(wcstr));
    }


    CK_API int compare(size_t pos1,
                       size_t length1,
                       const ck_string& other,
                       size_t pos2,
                       size_t length2) const
    {
        return _storage.compare(pos1, length1, other._storage, pos2, length2);
    }

    CK_API int compare(size_t pos1,
                       size_t length1,
                       const std::string& stdstr,
                       size_t pos2,
                       size_t length2) const
    {
        return _storage.compare(pos1, length1, stdstr, pos2, length2);
    }

    CK_API int compare(size_t pos, size_t length1, const char* cstr, size_t length2) const
    {
        return _storage.compare(pos, length1, cstr, length2);
    }

    CK_API int compare(size_t pos, size_t length1, const wchar_t* wcstr, size_t length2) const
    {
        return compare(pos, length1, convert_wide_string_to_multi_byte_string(wcstr, length2));
    }

    CK_API static std::string convert_wide_string_to_multi_byte_string(const wchar_t* str);
    CK_API static std::string convert_wide_string_to_multi_byte_string(const wchar_t* str, size_t length);
    CK_API static std::wstring convert_multi_byte_string_to_wide_string(const std::string& str);

    /**
     * @warning DANGER!!! You should only be using this method if you want to explicitly use UTF-16
     * instead of your platform's wchar_t, e.g. this should only be done if you are reading data writen
     * as UTF-16 and you are on a machine that is UTF-32 for wide characters, otherwise you whould
     * read it as wide data, and then pass to ck_string.
     */
    CK_API static std::string convert_utf16_string_to_multi_byte_string(const uint16_t* str);
    CK_API static std::string convert_utf16_string_to_multi_byte_string(const uint16_t* str,
                                                                       size_t length);

    /**
     * @warning DANGER!!! You should not be using this method unless you are required to explicity
     * output UTF-16, otherwise you should either use the wstring which is your platform's native wide
     * character type, or BEST UTF-8 which is our multi-byte representation as this is portable and
     * easiest to work with, so you would only use this if e.g. you are required to write UTF-16 to a file,
     * for a spec that some moron wrote, etc.
     */
    CK_API static std::vector<uint16_t> convert_multi_byte_string_to_utf16_string(const std::string& str);

    /**
     * @warning DANGER!!! You should only be using this method if you want to explicitly use UTF-32
     * instead of your platform's wchar_t, e.g. this should only be done if you are reading data writen
     * as UTF-32 and you are on a machine that is UTF-16 for wide characters, otherwise you whould
     * read it as wide data, and then pass to ck_string.
     */
    CK_API static std::string convert_utf32_string_to_multi_byte_string(const uint32_t* str);
    CK_API static std::string convert_utf32_string_to_multi_byte_string(const uint32_t* str,
                                                                       size_t length);

    /**
     * @warning DANGER!!! You should not be using this method unless you are required to explicity
     * output UTF-32, otherwise you should either use the wstring which is your platform's native wide
     * character type, or BEST UTF-8 which is our multi-byte representation as this is portable and
     * easiest to work with, so you would only use this if e.g. you are required to write UTF-32 to a file
     * for a spec that some moron wrote, etc.
     */
    CK_API static std::vector<uint32_t> convert_multi_byte_string_to_utf32_string(const std::string& str);
    CK_API static bool is_space(char a){return (a == ' ' || a == '\n' || a == '\t' || a == '\r');}

private:
    static char _encode_byte(uint8_t u);
    static CHAR_CLASS _get_char_class(char c);
    static bool _is_valid_uri_character(char c);

    template <typename T>
    bool _contains(const T& other) const;

    template <typename T>
    void _split(const T& delim, size_t delimLen, std::vector<ck_string>& parts) const;

    template <typename T>
    ck_string _erase_all(const T& toBeErased) const;

    std::string _storage;
};

CK_API int compare( const ck_string& left, const ck_string& right, void* context );

CK_API int compare( const char* c_string, const ck_string& intString, void* context );

CK_API int compare( const ck_string& intString, const char* c_string, void* context );

template <typename T>
bool ck_string::_contains(const T& other) const
{
    return _storage.find(other) != std::string::npos;
}

template <typename T>
void ck_string::_split(const T& delim, size_t delimLen, OUT std::vector<ck_string>& parts) const
{
    size_t begin = 0;
    size_t end = 0;

    parts.clear();

    while(1)
    {
        end = _storage.find(delim, begin);
        const ck_string str = substr(begin, end - begin);

        if(!str.empty())
            parts.push_back(str);

        if(end == std::string::npos)
            break;

        begin = end + delimLen;
    }
}

template <typename T>
ck_string ck_string::_erase_all(const T& toBeErased) const
{
    std::vector<ck_string> splitList = split( toBeErased );

    ck_string str;

    for(std::vector<ck_string>::iterator iter = splitList.begin(), end  = splitList.end();
        iter != end;
        ++iter)
    {
        str += *iter;
    }

    return str;
}

}

#ifdef IS_WINDOWS
#pragma warning(pop)
#endif

#endif

