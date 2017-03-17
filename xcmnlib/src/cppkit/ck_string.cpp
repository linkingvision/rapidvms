
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

#include "cppkit/ck_string.h"
#include "cppkit/os/ck_sprintf.h"
#include "cppkit/ck_memory.h"

#include <algorithm>

using namespace std;
using namespace cppkit;

#ifdef IS_WINDOWS
const static size_t WIDE_CHAR_SIZE = 2;
#else
const static size_t WIDE_CHAR_SIZE = 4;
#endif

ck_string ck_string::format(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    const ck_string retval = format(fmt, args);
    va_end(args);
    return retval;
}

ck_string ck_string::format(const char* fmt, va_list& args)
{
    if(fmt)
    {
        va_list newargs;
        va_copy(newargs, args);
        const int chars_written = ck_vsnprintf(NULL, 0, fmt, newargs);
        const int len = chars_written + 1;

        char* str = new char[len];
        str[len - 1] = 0;

        va_copy(newargs, args);
        ck_vsnprintf(str, len, fmt, newargs);

        try
        {
            ck_string formatted(str);
            delete[] str;
            return formatted;
        }
        catch(std::exception)
        {
            CK_THROW(("std::basic_string assignment threw."));
        }
    }

    CK_THROW(("NULL string passed to ck_string::format()."));
}

ck_string ck_string::from_int(int val, int base)
{
    if(base == 10)
        return format("%d", val);
    else if(base == 16)
        return format("%x", val);

    CK_THROW(("Unsupported numeric base requested."));
}

ck_string ck_string::from_uint(unsigned int val, int base)
{
    if(base == 10)
        return format("%u", val);
    else if(base == 16)
        return format("%x", val);

    CK_THROW(("Unsupported numeric base requested."));
}

ck_string ck_string::from_int8(int8_t val, int base)
{
    int tempVal = val;

    if(base == 10)
        return format("%d", tempVal);
    else if(base == 16)
        return format("%x", tempVal);

    CK_THROW(("Unsupported numeric base requested."));
}

ck_string ck_string::from_uint8(uint8_t val, int base)
{
    unsigned int tempVal = val;

    if(base == 10)
        return format("%u", tempVal);
    else if(base == 16)
        return format("%x", tempVal);

    CK_THROW(("Unsupported numeric base requested."));
}

ck_string ck_string::from_int16(int16_t val, int base)
{
    int tempVal = val;

    if(base == 10)
        return format("%d", tempVal);
    else if(base == 16)
        return format("%x", tempVal);

    CK_THROW(("Unsupported numeric base requested."));
}

ck_string ck_string::from_uint16(uint16_t val, int base)
{
    unsigned tempVal = val;

    if(base == 10)
        return format("%u", tempVal);
    else if(base == 16)
        return format("%x", tempVal);

    CK_THROW(("Unsupported numeric base requested."));
}

ck_string ck_string::from_int32(int32_t val, int base)
{
    if(base == 10)
        return format("%I32d", val);
    else if(base == 16)
        return format("%I32x", val);

    CK_THROW(("Unsupported numeric base requested."));
}

ck_string ck_string::from_uint32(uint32_t val, int base)
{
    if(base == 10)
        return format("%I32u", val);
    else if(base == 16)
        return format("%I32x", val);

    CK_THROW(("Unsupported numeric base requested."));
}

ck_string ck_string::from_int64(int64_t val, int base)
{
    if(base == 10)
        return format("%I64d", val);
    else if(base == 16)
        return format("%I64x", val);

    CK_THROW(("Unsupported numeric base requested."));
}

ck_string ck_string::from_uint64(uint64_t val, int base)
{
    if(base == 10)
        return format("%I64u", val);
    else if(base == 16)
        return format("%I64x", val);

    CK_THROW(("Unsupported numeric base requested."));
}

int ck_string::to_int(bool truncate) const
{
    if(truncate ? !is_double() : !is_integer())
        CK_THROW(("The ck_string is not a valid integral value: %s", _storage.c_str()));

    int val = 0;

    if(ck_sscanf(_storage.c_str(), "%d", &val) != 1)
        CK_THROW(("Failed to convert the ck_string to an int: %s", _storage.c_str()));

    return val;
}

int ck_string::to_int(int base) const
{
    if(base == 10)
        return to_int(false);
    else if(base == 16)
    {
        if(!is_hex())
            CK_THROW(("The ck_string is not a valid integral value: %s", _storage.c_str()));

        int val = 0;

        if(ck_sscanf(_storage.c_str(), "%x", &val) != 1)
            CK_THROW(("Failed to convert the ck_string to an int: %s", _storage.c_str()));

        return val;
    }
    else
        CK_THROW(("Unsupported numeric base requested."));
}

unsigned int ck_string::to_uint(bool truncate) const
{
    if(truncate ? !is_double(false) : !is_integer(false))
        CK_THROW(("The ck_string is not a valid unsigned integral value: %s", _storage.c_str()));

    unsigned int val = 0;

    if(ck_sscanf(_storage.c_str(), "%u", &val) != 1)
        CK_THROW(("Unsupported to convert a string to unsigned int. "
                  "ck_string is probably not a valid unsigned int."));

    return val;
}

unsigned int ck_string::to_uint(int base) const
{
    if(base == 10)
        return to_uint(false);
    else if(base == 16)
    {
        if(!is_hex(false))
            CK_THROW(("The ck_string is not a valid unsigned hex value: %s", _storage.c_str()));

        unsigned int val = 0;

        if(ck_sscanf(_storage.c_str(), "%x", &val) != 1)
            CK_THROW(("Unsupported to convert a string to unsigned int. ck_string is probably not a "
                      "valid unsigned int."));

        return val;
    }
    else
        CK_THROW(("Unsupported numeric base requested."));
}

int8_t ck_string::to_int8(bool truncate) const
{
    if(truncate ? !is_double() : !is_integer())
        CK_THROW(("The ck_string is not a valid integral value: %s", _storage.c_str()));

    int val = 0;

    if(ck_sscanf(_storage.c_str(), "%d", &val) != 1)
        CK_THROW(("Failed to convert the ck_string to int8_t: %s", _storage.c_str()));

    return (int8_t)val;
}

int8_t ck_string::to_int8(int base) const
{
    if(base == 10)
        return to_int8(false);
    else if(base == 16)
    {
        if(!is_hex())
            CK_THROW(("The ck_string is not a valid integral value: %s", _storage.c_str()));

        int val = 0;

        if(ck_sscanf(_storage.c_str(), "%x", &val) != 1)
            CK_THROW(("Failed to convert the ck_string to int8_t: %s", _storage.c_str()));

        return (int8_t)val;
    }
    else
        CK_THROW(("Unsupported numeric base requested."));
}

uint8_t ck_string::to_uint8(bool truncate) const
{
    if(truncate ? !is_double(false) : !is_integer(false))
        CK_THROW(("The ck_string is not a valid unsigned integral value: %s", _storage.c_str()));

    unsigned int val = 0;

    if(ck_sscanf(_storage.c_str(), "%u", &val) != 1)
        CK_THROW(("Unsupported to convert a string to uint8_t. ck_string is probably not a valid "
                  "uint8_t."));

    return (uint8_t)val;
}

uint8_t ck_string::to_uint8(int base) const
{
    if(base == 10)
        return to_uint8(false);
    else if(base == 16)
    {
        if(!is_hex(false))
            CK_THROW(("The ck_string is not a valid unsigned hex value: %s", _storage.c_str()));

        unsigned int val = 0;

        if(ck_sscanf(_storage.c_str(), "%x", &val) != 1)
            CK_THROW(("Unsupported to convert a string to uint8_t. ck_string is probably not a valid "
                      "uint8_t."));

        return (uint8_t)val;
    }
    else
        CK_THROW(("Unsupported numeric base requested."));
}

int16_t ck_string::to_int16(bool truncate) const
{
    if(truncate ? !is_double() : !is_integer())
        CK_THROW(("The ck_string is not a valid integral value: %s", _storage.c_str()));

    int val = 0;

    if(ck_sscanf(_storage.c_str(), "%d", &val) != 1)
        CK_THROW(("Failed to convert the ck_string to int16_t: %s", _storage.c_str()));

    return (int16_t)val;
}

int16_t ck_string::to_int16(int base) const
{
    if(base == 10)
        return to_int16(false);
    else if(base == 16)
    {
        if(!is_hex())
            CK_THROW(("The ck_string is not a valid integral value: %s", _storage.c_str()));

        int val = 0;

        if(ck_sscanf(_storage.c_str(), "%x", &val) != 1)
            CK_THROW(("Failed to convert the ck_string to int16_t: %s", _storage.c_str()));

        return (int16_t)val;
    }
    else
        CK_THROW(("Unsupported numeric base requested."));
}

uint16_t ck_string::to_uint16(bool truncate) const
{
    if(truncate ? !is_double(false) : !is_integer(false))
        CK_THROW(("The ck_string is not a valid unsigned integral value: %s", _storage.c_str()));

    unsigned int val = 0;

    if(ck_sscanf(_storage.c_str(), "%u", &val) != 1)
        CK_THROW(("Unsupported to convert a string to uint16_t. ck_string is probably not a valid "
                  "unsigned int."));

    return (uint16_t)val;
}

uint16_t ck_string::to_uint16(int base) const
{
    if(base == 10)
        return to_uint16(false);
    else if(base == 16)
    {
        if(!is_hex(false))
            CK_THROW(("The ck_string is not a valid unsigned hex value: %s", _storage.c_str()));

        unsigned int val = 0;

        if(ck_sscanf(_storage.c_str(), "%x", &val) != 1)
            CK_THROW(("Unsupported to convert a string to uint16_t. ck_string is probably not a valid "
                      "uint16_t."));

        return (uint16_t)val;
    }
    else
        CK_THROW(("Unsupported numeric base requested."));
}

int32_t ck_string::to_int32(bool truncate) const
{
    if(truncate ? !is_double() : !is_integer())
        CK_THROW(("The ck_string is not a valid integral value: %s", _storage.c_str()));

    int32_t val = 0;

    if(ck_sscanf(_storage.c_str(), "%I32d", &val) != 1)
        CK_THROW(("Unsupported to convert a string to int32_t. ck_string is probably not a valid "
                  "int32_t."));

    return val;
}

int32_t ck_string::to_int32(int base) const
{
    if(base == 10)
        return to_int32(false);
    else if(base == 16)
    {
        if(!is_hex())
            CK_THROW(("The ck_string is not a valid hex value: %s", _storage.c_str()));

        int32_t val = 0;

        if(ck_sscanf(_storage.c_str(), "%I32x", &val) != 1)
            CK_THROW(("Unsupported to convert a string to int32_t. ck_string is probably not a valid "
                      "int32_t."));

        return val;
    }
    else
        CK_THROW(("Unsupported numeric base requested."));
}

uint32_t ck_string::to_uint32(bool truncate) const
{
    if(truncate ? !is_double(false) : !is_integer(false))
        CK_THROW(("The ck_string is not a valid unsigned integral value: %s", _storage.c_str()));

    uint32_t val = 0;

    if(ck_sscanf(_storage.c_str(), "%I32u", &val) != 1)
        CK_THROW(("Unsupported to convert a string to uint32_t. ck_string is probably not a valid "
                  "uint32_t."));

    return val;
}

uint32_t ck_string::to_uint32(int base) const
{
    if(base == 10)
        return to_uint(false);
    else if(base == 16)
    {
        if(!is_hex(false))
            CK_THROW(("The ck_string is not a valid unsigned hex value: %s", _storage.c_str()));

        uint32_t val = 0;

        if(ck_sscanf(_storage.c_str(), "%I32x", &val) != 1)
            CK_THROW(("Unsupported to convert a string to uint32_t. ck_string is probably not a valid "
                      "uint32_t."));

        return val;
    }
    else
        CK_THROW(("Unsupported numeric base requested."));
}

int64_t ck_string::to_int64(bool truncate) const
{
    if(truncate ? !is_double() : !is_integer())
        CK_THROW(("The ck_string is not a valid integral value: %s", _storage.c_str()));

    int64_t val = 0;

    if(ck_sscanf(_storage.c_str(), "%I64d", &val) != 1)
        CK_THROW(("Unsupported to convert a string to int64_t. ck_string is probably not a valid "
                  "int64_t."));

    return val;
}

int64_t ck_string::to_int64(int base) const
{
    if(base == 10)
        return to_int64(false);
    else if(base == 16)
    {
        if(!is_hex())
            CK_THROW(("The ck_string is not a valid hex value: %s", _storage.c_str()));

        int64_t val = 0;

        if(ck_sscanf(_storage.c_str(), "%I64x", &val) != 1)
            CK_THROW(("Unsupported to convert a string to int64_t. ck_string is probably not a valid "
                      "int64_t."));

        return val;
    }
    else
        CK_THROW(("Unsupported numeric base requested."));
}

uint64_t ck_string::to_uint64(bool truncate) const
{
    if(truncate ? !is_double(false) : !is_integer(false))
        CK_THROW(("The ck_string is not a valid unsigned integral value: %s", _storage.c_str()));

    uint64_t val = 0;

    if(ck_sscanf(_storage.c_str(), "%I64u", &val) != 1)
        CK_THROW(("Unsupported to convert a string to uint64_t. ck_string is probably not a valid "
                  "uint64_t."));

    return val;
}

uint64_t ck_string::to_uint64(int base) const
{
    if(base == 10)
        return to_uint64(false);
    else if(base == 16)
    {
        if(!is_hex(false))
            CK_THROW(("The ck_string is not a valid unsigned hex value: %s", _storage.c_str()));

        uint64_t val = 0;

        if(ck_sscanf(_storage.c_str(), "%I64x", &val) != 1)
            CK_THROW(("Unsupported to convert a string to uint64_t. ck_string is probably not a valid "
                      "uint64_t."));

        return val;
    }
    else
        CK_THROW(("Unsupported numeric base requested."));
}

double ck_string::to_double() const
{
    if(!is_double())
        CK_THROW(("The ck_string is not a valid floating-point value: %s", _storage.c_str()));

    double val = 0;

    if(ck_sscanf(_storage.c_str(), "%lf", &val) != 1)
        CK_THROW(("Failed to convert a string to double. ck_string is probably not a valid double."));

    return val;
}

ck_string ck_string::to_base64( const void* source, size_t length )
{
    // Calculate output buffer size
    size_t srcLen = length;
    const size_t bufferSize = (4 * ((srcLen + 2 - ((srcLen + 2) % 3)) / 3));

    shared_ptr<vector<char> > destBuffer = make_shared<vector<char> >(bufferSize);

    size_t encodeLen = 0;
    uint8_t* pData = (uint8_t*)&((*destBuffer)[0]);
    uint8_t* src = (uint8_t*)source;

    // Encode
    if( (src != NULL) && (srcLen > 0))
    {
        for(size_t i = 0;  i < (srcLen / 3); i++, src += 3)
        {
            pData[encodeLen++] = base64_encoding_table[src[0]>>2];
            pData[encodeLen++] = base64_encoding_table[((src[0]& 0x3)<<4) | (src[1]>>4)];
            pData[encodeLen++] = base64_encoding_table[((src[1]& 0xF)<<2) | (src[2]>>6)];
            pData[encodeLen++] = base64_encoding_table[(src[2]& 0x3F)];
        }

        // Add padding '=' if necessary
        switch (srcLen % 3)
        {
        case 1:
            pData[encodeLen++] = base64_encoding_table[src[0]>>2];
            pData[encodeLen++] = base64_encoding_table[(src[0]& 0x3)<<4];
            pData[encodeLen++] = '=';
            pData[encodeLen++] = '=';
            break;

        case 2:
            pData[encodeLen++] = base64_encoding_table[src[0]>>2];
            pData[encodeLen++] = base64_encoding_table[((src[0]& 0x3)<<4) | (src[1]>>4)];
            pData[encodeLen++] = base64_encoding_table[((src[1]& 0xF)<<2)];
            pData[encodeLen++] = '=';
            break;

#ifdef ANCIENT_BULLSHIT                  // This is not compatible with standard BASE64
        default:                         // But can be added for comaptibility
            // Add a trailing '='
            pData[encodeLen++] = '=';
            break;
#endif

        }
    }
    else
    {
        // no data to encode. return an empty string.
        return ck_string();
    }

    // Copy encoded data into an XString object
    ck_string encoded((char*)&((*destBuffer)[0]), destBuffer->size());

    return encoded;
}

shared_ptr<ck_memory> ck_string::from_base64() const
{
    //this might be able to be bumped up to 4 since it looks like encoding
    //a single byte should result in 4 encoded bytes.  For now just do
    //enough checking to ensure that DecodeSize doesn't return a negative
    //number
    if (_storage.size() < 2)
        return make_shared<ck_memory>();

    // This buffer size is an upper bound.
    // This value can be: N, N+1 or N+2,
    // where N is the length of the raw data.
    size_t bufferSize = ((3 * _storage.size()) / 4);

    // Allocate some memory
    shared_ptr<ck_memory> destBuffer = make_shared<ck_memory>();

    uint8_t* pData = destBuffer->extend_data(bufferSize).get_ptr();
    uint8_t* src = (uint8_t*)_storage.c_str();
    size_t cursor = 0;
    size_t decodeLen = 0;
    uint8_t byteNumber = 0;
    uint8_t c;
    bool done = false;

    while (cursor < _storage.size())
    {
        c = *src++;
        cursor++;

        switch (_get_char_class(c))
        {
        case UPPER:
            c = c - 'A';
            break;
        case DIGIT:
            c -= '0' - 52;
            break;
        case LOWER:
            c -= 'a' - 26;
            break;
        case SLASH:
            c = 63;
            break;
        case PLUS:
            c = 62;
            break;
        case NEWLINE:
            continue;  // Just skip any new lines (Base64 sometimes has \n's)
        case CR:
            continue;  // Just skip any carriage returns (Base64 sometimes has CR's)
        case EQUAL:
            done = true;
            break;
        default:
            // This should never happen. Return an empty object.
            return make_shared<ck_memory>();
        }

        // If we haven't hit an '=' sign keep going
        if (!done)
        {
            switch(byteNumber++)
            {
            case 0:
                pData[decodeLen] = c << 2;
                break;
            case 1:
                pData[decodeLen++] |= c >> 4;
                pData[decodeLen] = c << 4;
                break;
            case 2:
                pData[decodeLen++] |= c >> 2;
                pData[decodeLen] = c << 6;
                break;
            case 3:
                pData[decodeLen++] |= c;
                byteNumber = 0;
                break;
            default:
                break;
            }
        }
    }

    // Actual raw data was less than our upper bound.
    // Copy the memory into a smaller buffer so we know its actual size.
    if ( decodeLen < destBuffer->size() )
    {
        shared_ptr<ck_memory> tempBuffer = make_shared<ck_memory>();
        memcpy( tempBuffer->extend_data(decodeLen).get_ptr(),
                destBuffer->map_data().get_ptr(),
                decodeLen );
        destBuffer = tempBuffer;
    }

    return destBuffer;
}

ck_string ck_string::uri_encode() const
{
    ck_string retval;

    if(!_storage.empty())
    {
        char c;

        for(size_t i = 0, size = _storage.size(); i < size; ++i)
        {
            c = _storage[i];

            if(_is_valid_uri_character(c))
                retval += c;
            // unsafe characters
            else
                retval += format("%%%02X", c);
        }
    }

    return retval;
}

ck_string ck_string::uri_decode() const
{
    ck_string retval;

    if(!_storage.empty())
    {
        char c;

        for(size_t i = 0, size = _storage.size(); i < size; ++i)
        {
            c = _storage[i];

            // spaces
            // The current spec (RFC 3986) does not permit encoding spaces with
            // +, but older versions of the spec do, so we decode + as space but
            // do not encode space as +.
            if(c == '+')
                retval += ' ';
            // unsafe characters
            else if(c == '%')
            {
                // hex to char conversion of next 2 characters
                if(i + 2 < size)
                {
                    //Initial contents of hexStr are irrelevant. It just needs to be the right length.
                    ck_string hexStr = "XX";
                    for(size_t j = 0; j < 2; ++j)
                    {
                        if(isxdigit(_storage[i+j+1]))
                            hexStr[j] = _storage[i+j+1];
                        else
                            CK_THROW(("malformed url"));
                    }

                    retval += (char)ck_string(hexStr).to_uint(16);
                    i += 2;
                }
                else
                    CK_THROW(("malformed url"));
            }
            else
                retval += c;
        }
    }

    return retval;
}

ck_string ck_string::to_lower() const
{
    ck_string retval = *this;
    transform( retval._storage.begin(), retval._storage.end(), retval._storage.begin(), ::tolower );
    return retval;
}

ck_string ck_string::to_upper() const
{
    ck_string retval = *this;
    transform( retval._storage.begin(), retval._storage.end(), retval._storage.begin(), ::toupper );
    return retval;
}

bool ck_string::starts_with(const ck_string& other) const
{
    const size_t otherSize = other._storage.size();
    return otherSize <= _storage.size() && _storage.compare(0, otherSize, other._storage) == 0;
}

bool ck_string::starts_with(const std::string& stdstr) const
{
    const size_t otherSize = stdstr.size();
    return otherSize <= _storage.size() && _storage.compare(0, otherSize, stdstr) == 0;
}


bool ck_string::starts_with(const wchar_t* wcstr) const
{
    if(!wcstr)
        CK_THROW(("NULL string passed to ck_string::starts_with()."));
    return starts_with(convert_wide_string_to_multi_byte_string(wcstr));
}


bool ck_string::starts_with(const char* cstr) const
{
    if(!cstr)
        CK_THROW(("NULL string passed to ck_string::starts_with()."));

    const size_t otherSize = strlen(cstr);
    return otherSize <= _storage.size() && _storage.compare(0, otherSize, cstr, 0, otherSize) == 0;
}

bool ck_string::ends_with(const ck_string& other) const
{
    const size_t otherSize = other._storage.size();
    const size_t thisSize = _storage.size();

    return (otherSize <= thisSize) &&
        (_storage.compare(thisSize - otherSize, otherSize, other._storage) == 0);
}

bool ck_string::ends_with(const std::string& stdstr) const
{
    const size_t otherSize = stdstr.size();
    const size_t thisSize = _storage.size();

    return otherSize <= thisSize && _storage.compare(thisSize - otherSize, otherSize, stdstr) == 0;
}

bool ck_string::ends_with(const wchar_t* wcstr) const
{
    if(!wcstr)
        CK_THROW(("NULL string passed to ck_string::ends_with()."));
    return ends_with(convert_wide_string_to_multi_byte_string(wcstr));
}


bool ck_string::ends_with(const char* cstr) const
{
    if(!cstr)
        CK_THROW(("NULL string passed to ck_string::ends_with()."));

    const size_t otherSize = strlen(cstr);
    const size_t thisSize = _storage.size();

    return (otherSize <= thisSize) &&
        (_storage.compare(thisSize - otherSize, otherSize, cstr, 0, otherSize) == 0);
}

//For some reason, I can't get isdigit to work directly (something about templates).
static bool verify_digit(char c)
{
    //I should be able to just return the result, but Windows is stupid.
    return isdigit(c) != 0;
}

bool ck_string::is_integer(bool canHaveSign) const
{
    const size_t first = _storage.find_first_not_of(' ');

    if(first == string::npos || (!canHaveSign && _storage[first] == '-'))
        return false;

    const size_t strippedFront = _storage[first] == '-' ? first + 1 : first;
    const size_t last = _storage.find_last_not_of(' ');
    const size_t strippedBack = last == string::npos ? 0 : (size() - 1) - last;
    const int numSize = (int)(size() - (strippedFront + strippedBack));

    if(numSize == 0)
        return false;

    return count_if(_storage.begin() + strippedFront,
                    _storage.end() - strippedBack,
                    verify_digit) == numSize;
}

//For some reason, I can't get isxdigit to work directly (something about templates).
static bool verify_hex_digit(char c)
{
    //I should be able to just return the result, but Windows is stupid.
    return isxdigit(c) != 0;
}

bool ck_string::is_hex(bool canHaveSign) const
{
    const size_t first = _storage.find_first_not_of(' ');

    if(first == string::npos || (!canHaveSign && _storage[first] == '-'))
        return false;

    const size_t strippedFront = _storage[first] == '-' ? first + 1 : first;
    const size_t last = _storage.find_last_not_of(' ');
    const size_t strippedBack = last == string::npos ? 0 : (size() - 1) - last;
    const int numSize = (int)(size() - (strippedFront + strippedBack));

    if(numSize == 0)
        return false;

    return count_if(_storage.begin() + strippedFront,
                    _storage.end() - strippedBack,
                    verify_hex_digit) == numSize;
}

bool ck_string::is_double(bool canHaveSign) const
{
    const size_t decimal = _storage.find('.');

    if(decimal == string::npos)
        return is_integer();

    const size_t first = _storage.find_first_not_of(' ');

    if(first == string::npos || (!canHaveSign && _storage[first] == '-'))
        return false;

    const size_t strippedFront = _storage[first] == '-' ? first + 1 : first;
    const size_t last = _storage.find_last_not_of(' ');
    const size_t strippedBack = last == string::npos ? 0 : (size() - 1) - last;
    const int numSize = (int)(size() - (strippedFront + strippedBack) - 1);

    if(numSize == 0)
        return false;

    const int numDigits = count_if(_storage.begin() + strippedFront,
                                   _storage.begin() + decimal,
                                   verify_digit) +
        count_if(_storage.begin() + decimal + 1,
                 _storage.end() - strippedBack,
                 verify_digit);

    return numDigits == numSize;
}

ck_string ck_string::lstrip() const
{
    ck_string retval = *this;
    size_t pos = 0;
    while(pos < retval.size() && ck_string::is_space(retval[pos])) pos++;
    retval.erase(0, pos);
    return retval;
}

ck_string ck_string::rstrip() const
{
    ck_string retval = *this;
    size_t pos = retval.size();
    while(pos > 0 && ck_string::is_space(retval[pos - 1])) pos--;
    retval.erase(pos);
    return retval;
}

ck_string ck_string::strip() const
{
    ck_string retval = rstrip();
    return retval.lstrip();
}

ck_string ck_string::strip_eol() const
{
    if(ends_with("\r\n"))
        return substr(0, _storage.size() - 2);
    if(ends_with("\n"))
        return substr(0, _storage.size() - 1);
    return *this;
}

ck_string ck_string::replace_all(char toBeReplaced, char toReplaceWith) const
{
    ck_string dup = *this;
    for(size_t i = dup.find(toBeReplaced);
        i != string::npos;
        i = dup.find(toBeReplaced, i))
    {
        dup[i] = toReplaceWith;
    }
    return dup;
}

ck_string ck_string::replace_all(wchar_t toBeReplaced, wchar_t toReplaceWith) const
{
    wchar_t replace[] = {toBeReplaced, (wchar_t)NULL};
    vector<ck_string> splitList = split(toBeReplaced);
    bool endedWith = false;
    ck_string replaced = "";

    if(starts_with(replace))
        replaced += toReplaceWith;

    if(ends_with(replace))
        endedWith = true;

    bool firstPass = true;

    for(vector<ck_string>::iterator iter = splitList.begin(), end  = splitList.end();
        iter != end;
        ++iter)
    {
        if(!firstPass)
            replaced += toReplaceWith;
        else
            firstPass = false;

        replaced += *iter;
    }

    if(endedWith)
        replaced += toReplaceWith;

    return replaced;
}


ck_string ck_string::replace_all(const ck_string& toBeReplaced, const ck_string& toReplaceWith) const
{
    size_t found = _storage.find(toBeReplaced);

    if(found == string::npos)
        return *this;

    ck_string retval = _storage.substr(0, found);
    retval.append(toReplaceWith);

    while(1)
    {
        const size_t start = found + toBeReplaced.size();
        found = _storage.find(toBeReplaced, start);

        if(found != string::npos)
        {
            retval.append(_storage.substr(start, found - start));
            retval.append(toReplaceWith);
        }
        else
        {
            retval.append(_storage.substr(start));
            break;
        }
    }

    return retval;
}

char ck_string::_encode_byte(unsigned char u)
{
    if(u < 26)
        return 'A' + u;
    if(u < 52)
        return 'a' + (u - 26);
    if(u < 62)
        return '0' + (u - 52);
    if(u == 62)
        return '+';
    return '/';
}

ck_string::CHAR_CLASS ck_string::_get_char_class(char c)
{
    if (isupper(c))
        return UPPER;
    else if (isdigit(c))
        return DIGIT;
    else if (islower(c))
        return LOWER;
    else if (c == '/')
        return SLASH;
    else if (c == '+')
        return PLUS;
    else if (c == '=')
        return EQUAL;
    else if (c == '\n')
        return NEWLINE;
    else if (c == '\r')
        return CR;
    else
        return OTHER;
}

// These characters are the "unreserved" characters from RFC 3986.
bool ck_string::_is_valid_uri_character(char c)
{
    return isalnum(c) || c == '-' || c == '.' || c == '_' || c == '~';
}

std::string ck_string::convert_utf16_string_to_multi_byte_string(const uint16_t* str)
{
    return convert_utf16_string_to_multi_byte_string(str, (size_t)-1);
}

std::string ck_string::convert_utf16_string_to_multi_byte_string(const uint16_t* str, size_t length)
{
    std::string out;
    if(str == NULL)
        return out;
    unsigned int codepoint = 0;
    for (size_t i = 0; i < length && *str != 0; ++i, ++str)
    {
        if(*str >= 0xd800 && *str <= 0xdbff)
            codepoint = ((*str - 0xd800) << 10) + 0x10000;
        else
        {
            if(*str >= 0xdc00 && *str <= 0xdfff)
                codepoint |= *str - 0xdc00;
            else
                codepoint = *str;

            if(codepoint <= 0x7f)
                out.append(1, static_cast<char>(codepoint));
            else if(codepoint <= 0x7ff)
            {
                out.append(1, static_cast<char>(0xc0 | ((codepoint >> 6) & 0x1f)));
                out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
            }
            else if(codepoint <= 0xffff)
            {
                out.append(1, static_cast<char>(0xe0 | ((codepoint >> 12) & 0x0f)));
                out.append(1, static_cast<char>(0x80 | ((codepoint >> 6) & 0x3f)));
                out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
            }
            else
            {
                out.append(1, static_cast<char>(0xf0 | ((codepoint >> 18) & 0x07)));
                out.append(1, static_cast<char>(0x80 | ((codepoint >> 12) & 0x3f)));
                out.append(1, static_cast<char>(0x80 | ((codepoint >> 6) & 0x3f)));
                out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
            }
            codepoint = 0;
        }
    }
    return out;
}

std::vector<uint16_t> ck_string::convert_multi_byte_string_to_utf16_string(const std::string& str)
{
    std::vector<uint16_t> out;
    if(str.empty())
        return out;
    char* place = const_cast<char*>(str.c_str());
    unsigned int codepoint = 0;
    int following = 0;
    for (;  *place != 0;  ++place)
    {
        unsigned char ch = *place;
        if(ch <= 0x7f)
        {
            codepoint = ch;
            following = 0;
        }
        else if(ch <= 0xbf)
        {
            if(following > 0)
            {
                codepoint = (codepoint << 6) | (ch & 0x3f);
                --following;
            }
        }
        else if(ch <= 0xdf)
        {
            codepoint = ch & 0x1f;
            following = 1;
        }
        else if(ch <= 0xef)
        {
            codepoint = ch & 0x0f;
            following = 2;
        }
        else
        {
            codepoint = ch & 0x07;
            following = 3;
        }
        if(following == 0)
        {
            if(codepoint > 0xffff)
            {
                out.push_back(static_cast<wchar_t>(0xd800 + (codepoint >> 10)));
                out.push_back(static_cast<wchar_t>(0xdc00 + (codepoint & 0x03ff)));
            }
            else
                out.push_back(static_cast<wchar_t>(codepoint));
            codepoint = 0;
        }
    }
    return out;
}

std::string ck_string::convert_utf32_string_to_multi_byte_string(const uint32_t* str)
{
    return convert_utf32_string_to_multi_byte_string(str, (size_t)-1);
}

std::string ck_string::convert_utf32_string_to_multi_byte_string(const uint32_t* str, size_t length)
{
    std::string out;
    if(str == NULL)
        return out;

    size_t i = 0;
    for (wchar_t* temp = (wchar_t*)str; i < length && *temp != 0; ++temp, ++i)
    {
        unsigned int codepoint = *temp;

        if(codepoint <= 0x7f)
            out.append(1, static_cast<char>(codepoint));
        else if(codepoint <= 0x7ff)
        {
            out.append(1, static_cast<char>(0xc0 | ((codepoint >> 6) & 0x1f)));
            out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
        }
        else if(codepoint <= 0xffff)
        {
            out.append(1, static_cast<char>(0xe0 | ((codepoint >> 12) & 0x0f)));
            out.append(1, static_cast<char>(0x80 | ((codepoint >> 6) & 0x3f)));
            out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
        }
        else
        {
            out.append(1, static_cast<char>(0xf0 | ((codepoint >> 18) & 0x07)));
            out.append(1, static_cast<char>(0x80 | ((codepoint >> 12) & 0x3f)));
            out.append(1, static_cast<char>(0x80 | ((codepoint >> 6) & 0x3f)));
            out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
        }
    }
    return out;
}

std::vector<uint32_t> ck_string::convert_multi_byte_string_to_utf32_string(const std::string& str)
{
    std::vector<uint32_t> out;

    wchar_t codepoint = 0;
    int following = 0;
    for (char* temp = const_cast<char*>(str.c_str());  *temp != 0;  ++temp)
    {
        unsigned char ch = *temp;
        if(ch <= 0x7f)
        {
            codepoint = ch;
            following = 0;
        }
        else if(ch <= 0xbf)
        {
            if(following > 0)
            {
                codepoint = (codepoint << 6) | (ch & 0x3f);
                --following;
            }
        }
        else if(ch <= 0xdf)
        {
            codepoint = ch & 0x1f;
            following = 1;
        }
        else if(ch <= 0xef)
        {
            codepoint = ch & 0x0f;
            following = 2;
        }
        else
        {
            codepoint = ch & 0x07;
            following = 3;
        }
        if(following == 0)
        {
            out.push_back(codepoint);
            codepoint = 0;
        }
    }
    return out;
}

std::string ck_string::convert_wide_string_to_multi_byte_string(const wchar_t* str)
{
    if(sizeof(wchar_t) != WIDE_CHAR_SIZE)
        CK_THROW(("wchar_t size is [%d], and should be [%d]"));
#ifdef IS_WINDOWS
    std::string result(convert_utf16_string_to_multi_byte_string((uint16_t*)str));
#else//*NIX
    std::string result(convert_utf32_string_to_multi_byte_string((uint32_t*)str));
#endif
    return result;
}

std::string ck_string::convert_wide_string_to_multi_byte_string(const wchar_t* str, size_t length)
{
    if(sizeof(wchar_t) != WIDE_CHAR_SIZE)
        CK_THROW(("wchar_t size is [%d], and should be [%d]"));
#ifdef IS_WINDOWS
    std::string result(convert_utf16_string_to_multi_byte_string((uint16_t*)str, length));
#else//*NIX
    std::string result(convert_utf32_string_to_multi_byte_string((uint32_t*)str, length));
#endif
    return result;
}

std::wstring ck_string::convert_multi_byte_string_to_wide_string(const std::string& str)
{
    if(sizeof(wchar_t) != WIDE_CHAR_SIZE)
        CK_THROW(("wchar_t size is [%d], and should be [%d]"));
#ifdef IS_WINDOWS
    std::vector<uint16_t> converted = convert_multi_byte_string_to_utf16_string(str);
    std::wstring result(converted.begin(),converted.end());
#else//*NIX
    std::vector<uint32_t> converted = convert_multi_byte_string_to_utf32_string(str);
    std::wstring result(converted.begin(),converted.end());
#endif
    return result;
}

int cppkit::compare(const ck_string& left, const ck_string& right, void* context)
{
    return strcmp(left.c_str(), right.c_str());
}

int cppkit::compare(const char* c_string, const ck_string& intString, void* context)
{
    return strcmp(c_string, intString.c_str());
}

int cppkit::compare(const ck_string& intString, const char* c_string, void* context)
{
    return strcmp(intString.c_str(), c_string);
}
