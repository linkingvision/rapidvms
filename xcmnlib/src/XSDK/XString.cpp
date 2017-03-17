
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "XSDK/XString.h"
#include "XSDK/XMemory.h"
#include "XSDK/OS.h"
#include <algorithm>

using namespace std;
using namespace XSDK;

#ifdef WIN32
const static size_t WIDE_CHAR_SIZE = 2;
#else
const static size_t WIDE_CHAR_SIZE = 4;
#endif


XString XString::Format(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    const XString retval = Format(format, args);
    va_end(args);
    return retval;
}

XString XString::Format(const char* format, va_list& args)
{
    if(format)
    {
        va_list newargs;
        va_copy(newargs, args);
        const int chars_written = x_vsnprintf(NULL, 0, format, newargs);
        const int len = chars_written + 1;

        char* str = new char[len];
        str[len - 1] = 0;

        va_copy(newargs, args);
        x_vsnprintf(str, len, format, newargs);

        try
        {
            XString formatted(str);
            delete[] str;
            return formatted;
        }
        catch(std::exception)
        {
            X_THROW(("std::basic_string assignment threw."));
        }
    }

    X_THROW(("NULL string passed to XString::Format()."));
}

XString& XString::FormatInPlace(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    *this = Format(format, args);
    va_end(args);

    return *this;
}

XString XString::FromInt(int val, int base)
{
    if(base == 10)
        return Format("%d", val);
    else if(base == 16)
        return Format("%x", val);

    X_THROW(("Unsupported numeric base requested."));
}

XString XString::FromUInt(unsigned int val, int base)
{
    if(base == 10)
        return Format("%u", val);
    else if(base == 16)
        return Format("%x", val);

    X_THROW(("Unsupported numeric base requested."));
}

XString XString::FromInt8(int8_t val, int base)
{
    int tempVal = val;

    if(base == 10)
        return Format("%d", tempVal);
    else if(base == 16)
        return Format("%x", tempVal);

    X_THROW(("Unsupported numeric base requested."));
}

XString XString::FromUInt8(uint8_t val, int base)
{
    unsigned int tempVal = val;

    if(base == 10)
        return Format("%u", tempVal);
    else if(base == 16)
        return Format("%x", tempVal);

    X_THROW(("Unsupported numeric base requested."));
}

XString XString::FromInt16(int16_t val, int base)
{
    int tempVal = val;

    if(base == 10)
        return Format("%d", tempVal);
    else if(base == 16)
        return Format("%x", tempVal);

    X_THROW(("Unsupported numeric base requested."));
}

XString XString::FromUInt16(uint16_t val, int base)
{
    unsigned tempVal = val;

    if(base == 10)
        return Format("%u", tempVal);
    else if(base == 16)
        return Format("%x", tempVal);

    X_THROW(("Unsupported numeric base requested."));
}

XString XString::FromInt32(int32_t val, int base)
{
    if(base == 10)
        return Format("%I32d", val);
    else if(base == 16)
        return Format("%I32x", val);

    X_THROW(("Unsupported numeric base requested."));
}

XString XString::FromUInt32(uint32_t val, int base)
{
    if(base == 10)
        return Format("%I32u", val);
    else if(base == 16)
        return Format("%I32x", val);

    X_THROW(("Unsupported numeric base requested."));
}

XString XString::FromInt64(int64_t val, int base)
{
    if(base == 10)
        return Format("%I64d", val);
    else if(base == 16)
        return Format("%I64x", val);

    X_THROW(("Unsupported numeric base requested."));
}

XString XString::FromUInt64(uint64_t val, int base)
{
    if(base == 10)
        return Format("%I64u", val);
    else if(base == 16)
        return Format("%I64x", val);

    X_THROW(("Unsupported numeric base requested."));
}

int XString::ToInt(bool truncate) const
{
    if(truncate ? !IsDouble() : !IsInteger())
        X_THROW(("The XString is not a valid integral value: %s", _storage.c_str()));

    int val = 0;

    if(x_sscanf(_storage.c_str(), "%d", &val) != 1)
        X_THROW(("Failed to convert the XString to an int: %s", _storage.c_str()));

    return val;
}

int XString::ToInt(int base) const
{
    if(base == 10)
        return ToInt(false);
    else if(base == 16)
    {
        if(!IsHex())
            X_THROW(("The XString is not a valid integral value: %s", _storage.c_str()));

        int val = 0;

        if(x_sscanf(_storage.c_str(), "%x", &val) != 1)
            X_THROW(("Failed to convert the XString to an int: %s", _storage.c_str()));

        return val;
    }
    else
        X_THROW(("Unsupported numeric base requested."));
}

unsigned int XString::ToUInt(bool truncate) const
{
    if(truncate ? !IsDouble(false) : !IsInteger(false))
        X_THROW(("The XString is not a valid unsigned integral value: %s", _storage.c_str()));

    unsigned int val = 0;

    if(x_sscanf(_storage.c_str(), "%u", &val) != 1)
        X_THROW(("Unsupported to convert a string to unsigned int. XString is probably not a valid unsigned int."));

    return val;
}

unsigned int XString::ToUInt(int base) const
{
    if(base == 10)
        return ToUInt(false);
    else if(base == 16)
    {
        if(!IsHex(false))
            X_THROW(("The XString is not a valid unsigned hex value: %s", _storage.c_str()));

        unsigned int val = 0;

        if(x_sscanf(_storage.c_str(), "%x", &val) != 1)
            X_THROW(("Unsupported to convert a string to unsigned int. XString is probably not a valid unsigned int."));

        return val;
    }
    else
        X_THROW(("Unsupported numeric base requested."));
}

int8_t XString::ToInt8(bool truncate) const
{
    if(truncate ? !IsDouble() : !IsInteger())
        X_THROW(("The XString is not a valid integral value: %s", _storage.c_str()));

    int val = 0;

    if(x_sscanf(_storage.c_str(), "%d", &val) != 1)
        X_THROW(("Failed to convert the XString to int8_t: %s", _storage.c_str()));

    return (int8_t)val;
}

int8_t XString::ToInt8(int base) const
{
    if(base == 10)
        return ToInt8(false);
    else if(base == 16)
    {
        if(!IsHex())
            X_THROW(("The XString is not a valid integral value: %s", _storage.c_str()));

        int val = 0;

        if(x_sscanf(_storage.c_str(), "%x", &val) != 1)
            X_THROW(("Failed to convert the XString to int8_t: %s", _storage.c_str()));

        return (int8_t)val;
    }
    else
        X_THROW(("Unsupported numeric base requested."));
}

uint8_t XString::ToUInt8(bool truncate) const
{
    if(truncate ? !IsDouble(false) : !IsInteger(false))
        X_THROW(("The XString is not a valid unsigned integral value: %s", _storage.c_str()));

    unsigned int val = 0;

    if(x_sscanf(_storage.c_str(), "%u", &val) != 1)
        X_THROW(("Unsupported to convert a string to uint8_t. XString is probably not a valid uint8_t."));

    return (uint8_t)val;
}

uint8_t XString::ToUInt8(int base) const
{
    if(base == 10)
        return ToUInt8(false);
    else if(base == 16)
    {
        if(!IsHex(false))
            X_THROW(("The XString is not a valid unsigned hex value: %s", _storage.c_str()));

        unsigned int val = 0;

        if(x_sscanf(_storage.c_str(), "%x", &val) != 1)
            X_THROW(("Unsupported to convert a string to uint8_t. XString is probably not a valid uint8_t."));

        return (uint8_t)val;
    }
    else
        X_THROW(("Unsupported numeric base requested."));
}

int16_t XString::ToInt16(bool truncate) const
{
    if(truncate ? !IsDouble() : !IsInteger())
        X_THROW(("The XString is not a valid integral value: %s", _storage.c_str()));

    int val = 0;

    if(x_sscanf(_storage.c_str(), "%d", &val) != 1)
        X_THROW(("Failed to convert the XString to int16_t: %s", _storage.c_str()));

    return (int16_t)val;
}

int16_t XString::ToInt16(int base) const
{
    if(base == 10)
        return ToInt16(false);
    else if(base == 16)
    {
        if(!IsHex())
            X_THROW(("The XString is not a valid integral value: %s", _storage.c_str()));

        int val = 0;

        if(x_sscanf(_storage.c_str(), "%x", &val) != 1)
            X_THROW(("Failed to convert the XString to int16_t: %s", _storage.c_str()));

        return (int16_t)val;
    }
    else
        X_THROW(("Unsupported numeric base requested."));
}

uint16_t XString::ToUInt16(bool truncate) const
{
    if(truncate ? !IsDouble(false) : !IsInteger(false))
        X_THROW(("The XString is not a valid unsigned integral value: %s", _storage.c_str()));

    unsigned int val = 0;

    if(x_sscanf(_storage.c_str(), "%u", &val) != 1)
        X_THROW(("Unsupported to convert a string to uint16_t. XString is probably not a valid unsigned int."));

    return (uint16_t)val;
}

uint16_t XString::ToUInt16(int base) const
{
    if(base == 10)
        return ToUInt16(false);
    else if(base == 16)
    {
        if(!IsHex(false))
            X_THROW(("The XString is not a valid unsigned hex value: %s", _storage.c_str()));

        unsigned int val = 0;

        if(x_sscanf(_storage.c_str(), "%x", &val) != 1)
            X_THROW(("Unsupported to convert a string to uint16_t. XString is probably not a valid uint16_t."));

        return (uint16_t)val;
    }
    else
        X_THROW(("Unsupported numeric base requested."));
}

int32_t XString::ToInt32(bool truncate) const
{
    if(truncate ? !IsDouble() : !IsInteger())
        X_THROW(("The XString is not a valid integral value: %s", _storage.c_str()));

    int32_t val = 0;

    if(x_sscanf(_storage.c_str(), "%I32d", &val) != 1)
        X_THROW(("Unsupported to convert a string to int32_t. XString is probably not a valid int32_t."));

    return val;
}

int32_t XString::ToInt32(int base) const
{
    if(base == 10)
        return ToInt32(false);
    else if(base == 16)
    {
        if(!IsHex())
            X_THROW(("The XString is not a valid hex value: %s", _storage.c_str()));

        int32_t val = 0;

        if(x_sscanf(_storage.c_str(), "%I32x", &val) != 1)
            X_THROW(("Unsupported to convert a string to int32_t. XString is probably not a valid int32_t."));

        return val;
    }
    else
        X_THROW(("Unsupported numeric base requested."));
}

uint32_t XString::ToUInt32(bool truncate) const
{
    if(truncate ? !IsDouble(false) : !IsInteger(false))
        X_THROW(("The XString is not a valid unsigned integral value: %s", _storage.c_str()));

    uint32_t val = 0;

    if(x_sscanf(_storage.c_str(), "%I32u", &val) != 1)
        X_THROW(("Unsupported to convert a string to uint32_t. XString is probably not a valid uint32_t."));

    return val;
}

uint32_t XString::ToUInt32(int base) const
{
    if(base == 10)
        return ToUInt(false);
    else if(base == 16)
    {
        if(!IsHex(false))
            X_THROW(("The XString is not a valid unsigned hex value: %s", _storage.c_str()));

        uint32_t val = 0;

        if(x_sscanf(_storage.c_str(), "%I32x", &val) != 1)
            X_THROW(("Unsupported to convert a string to uint32_t. XString is probably not a valid uint32_t."));

        return val;
    }
    else
        X_THROW(("Unsupported numeric base requested."));
}

int64_t XString::ToInt64(bool truncate) const
{
    if(truncate ? !IsDouble() : !IsInteger())
        X_THROW(("The XString is not a valid integral value: %s", _storage.c_str()));

    int64_t val = 0;

    if(x_sscanf(_storage.c_str(), "%I64d", &val) != 1)
        X_THROW(("Unsupported to convert a string to int64_t. XString is probably not a valid int64_t."));

    return val;
}

int64_t XString::ToInt64(int base) const
{
    if(base == 10)
        return ToInt64(false);
    else if(base == 16)
    {
        if(!IsHex())
            X_THROW(("The XString is not a valid hex value: %s", _storage.c_str()));

        int64_t val = 0;

        if(x_sscanf(_storage.c_str(), "%I64x", &val) != 1)
            X_THROW(("Unsupported to convert a string to int64_t. XString is probably not a valid int64_t."));

        return val;
    }
    else
        X_THROW(("Unsupported numeric base requested."));
}

uint64_t XString::ToUInt64(bool truncate) const
{
    if(truncate ? !IsDouble(false) : !IsInteger(false))
        X_THROW(("The XString is not a valid unsigned integral value: %s", _storage.c_str()));

    uint64_t val = 0;

    if(x_sscanf(_storage.c_str(), "%I64u", &val) != 1)
        X_THROW(("Unsupported to convert a string to uint64_t. XString is probably not a valid uint64_t."));

    return val;
}

uint64_t XString::ToUInt64(int base) const
{
    if(base == 10)
        return ToUInt64(false);
    else if(base == 16)
    {
       if(!IsHex(false))
            X_THROW(("The XString is not a valid unsigned hex value: %s", _storage.c_str()));

        uint64_t val = 0;

        if(x_sscanf(_storage.c_str(), "%I64x", &val) != 1)
            X_THROW(("Unsupported to convert a string to uint64_t. XString is probably not a valid uint64_t."));

        return val;
    }
    else
        X_THROW(("Unsupported numeric base requested."));
}

double XString::ToDouble() const
{
    if(!IsDouble())
        X_THROW(("The XString is not a valid floating-point value: %s", _storage.c_str()));

    double val = 0;

    if(x_sscanf(_storage.c_str(), "%lf", &val) != 1)
        X_THROW(("Failed to convert a string to double. XString is probably not a valid double."));

    return val;
}

XString XString::ToBase64(XIRef<XMemory> data)
{
    // Calculate output buffer size
    size_t srcLen = data->GetDataSize();
    size_t bufferSize = (4 * ((srcLen + 2 - ((srcLen + 2) % 3)) / 3));

    XIRef<XSDK::XMemory> destBuffer = new XSDK::XMemory(bufferSize);
    destBuffer->Extend(bufferSize);

    size_t encodeLen = 0;
    uint8_t* pData = (uint8_t*)destBuffer->Map();
    uint8_t* src = (uint8_t*)data->Map();

    // Encode
    if( (src != NULL) && (srcLen > 0))
    {
        for(size_t i = 0;  i < (srcLen / 3); i++, src += 3)
        {
            pData[encodeLen++] = encodingTable[src[0]>>2];
            pData[encodeLen++] = encodingTable[((src[0]& 0x3)<<4) | (src[1]>>4)];
            pData[encodeLen++] = encodingTable[((src[1]& 0xF)<<2) | (src[2]>>6)];
            pData[encodeLen++] = encodingTable[(src[2]& 0x3F)];
        }

        // Add padding '=' if necessary
        switch (srcLen % 3)
        {
        case 1:
            pData[encodeLen++] = encodingTable[src[0]>>2];
            pData[encodeLen++] = encodingTable[(src[0]& 0x3)<<4];
            pData[encodeLen++] = '=';
            pData[encodeLen++] = '=';
            break;

        case 2:
            pData[encodeLen++] = encodingTable[src[0]>>2];
            pData[encodeLen++] = encodingTable[((src[0]& 0x3)<<4) | (src[1]>>4)];
            pData[encodeLen++] = encodingTable[((src[1]& 0xF)<<2)];
            pData[encodeLen++] = '=';
            break;

#ifdef OLD_ENDURA_COMPATIBLE             // This is not compatible with standard BASE64
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
        return XString();
    }

    // Copy encoded data into an XString object
    XString encoded((char*)destBuffer->Map(), destBuffer->GetDataSize());
    return encoded;
}

XString XString::Base64Encode( const void* source, size_t length )
{
    // Calculate output buffer size
    size_t srcLen = length;
    size_t bufferSize = (4 * ((srcLen + 2 - ((srcLen + 2) % 3)) / 3));

    XIRef<XSDK::XMemory> destBuffer = new XSDK::XMemory(bufferSize);
    destBuffer->Extend(bufferSize);

    size_t encodeLen = 0;
    uint8_t* pData = (uint8_t*)destBuffer->Map();
    uint8_t* src = (uint8_t*)source;

    // Encode
    if( (src != NULL) && (srcLen > 0))
    {
        for(size_t i = 0;  i < (srcLen / 3); i++, src += 3)
        {
            pData[encodeLen++] = encodingTable[src[0]>>2];
            pData[encodeLen++] = encodingTable[((src[0]& 0x3)<<4) | (src[1]>>4)];
            pData[encodeLen++] = encodingTable[((src[1]& 0xF)<<2) | (src[2]>>6)];
            pData[encodeLen++] = encodingTable[(src[2]& 0x3F)];
        }

        // Add padding '=' if necessary
        switch (srcLen % 3)
        {
        case 1:
            pData[encodeLen++] = encodingTable[src[0]>>2];
            pData[encodeLen++] = encodingTable[(src[0]& 0x3)<<4];
            pData[encodeLen++] = '=';
            pData[encodeLen++] = '=';
            break;

        case 2:
            pData[encodeLen++] = encodingTable[src[0]>>2];
            pData[encodeLen++] = encodingTable[((src[0]& 0x3)<<4) | (src[1]>>4)];
            pData[encodeLen++] = encodingTable[((src[1]& 0xF)<<2)];
            pData[encodeLen++] = '=';
            break;

#ifdef OLD_ENDURA_COMPATIBLE             // This is not compatible with standard BASE64
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
        return XString();
    }

    // Copy encoded data into an XString object
    XString encoded((char*)destBuffer->Map(), destBuffer->GetDataSize());

    return encoded;
}

XIRef<XMemory> XString::FromBase64() const
{
    //this might be able to be bumped up to 4 since it looks like encoding
    //a single byte should result in 4 encoded bytes.  For now just do
    //enough checking to ensure that DecodeSize doesn't return a negative
    //number
    if (_storage.size() < 2)
        return XIRef<XSDK::XMemory>();

    // This buffer size is an upper bound.
    // This value can be: N, N+1 or N+2,
    // where N is the length of the raw data.
    size_t bufferSize = ((3 * _storage.size()) / 4);

    // Allocate some memory
    XIRef<XSDK::XMemory> destBuffer = new XSDK::XMemory(bufferSize);
    destBuffer->Extend(bufferSize);

    uint8_t* pData = (uint8_t*)destBuffer->Map();
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

        switch (_GetCharClass(c))
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
            return XIRef<XSDK::XMemory>();
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
    if ( decodeLen < destBuffer->GetDataSize() )
    {
        XIRef<XSDK::XMemory> tempBuffer = new XSDK::XMemory(decodeLen);
        memcpy( &tempBuffer->Extend(decodeLen), destBuffer->Map(), decodeLen );
        destBuffer = tempBuffer;
    }

    return destBuffer;
}

XString XString::URIEncode() const
{
    XString retval;

    if(!_storage.empty())
    {
        char c;

        for(size_t i = 0, size = _storage.size(); i < size; ++i)
        {
            c = _storage[i];

            if(_IsValidURICharacter(c))
                retval += c;
            // unsafe characters
            else
                retval += Format("%%%02X", c);
        }
    }

    return retval;
}

XString XString::URIDecode() const
{
    XString retval;

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
                    XString hexStr = "XX";
                    for(size_t j = 0; j < 2; ++j)
                    {
                        if(isxdigit(_storage[i+j+1]))
                           hexStr[j] = _storage[i+j+1];
                        else
                          X_THROW(("malformed url"));
                    }

                    retval += (char)XString(hexStr).ToUInt(16);
                    i += 2;
                }
                else
                  X_THROW(("malformed url"));
            }
            else
               retval += c;
       }
    }

   return retval;
}

XString XString::ToLower() const
{
    XString retval = *this;
    retval.ToLowerInPlace();
    return retval;
}

XString XString::ToUpper() const
{
    XString retval = *this;
    retval.ToUpperInPlace();
    return retval;
}

XString& XString::ToLowerInPlace()
{
    try
    {
        transform(_storage.begin(), _storage.end(), _storage.begin(), ::tolower);
        return *this;
    }
    catch(std::exception)
    {
        X_THROW(("std::transform threw."));
    }
}

XString& XString::ToUpperInPlace()
{
    try
    {
        transform(_storage.begin(), _storage.end(), _storage.begin(), ::toupper);
        return *this;
    }

    catch(std::exception)
    {
        X_THROW(("std::transform threw."));
    }
}

bool XString::StartsWith(const XString& other) const
{
    const size_t otherSize = other._storage.size();
    return otherSize <= _storage.size() && _storage.compare(0, otherSize, other._storage) == 0;
}

bool XString::StartsWith(const std::string& stdstr) const
{
    const size_t otherSize = stdstr.size();
    return otherSize <= _storage.size() && _storage.compare(0, otherSize, stdstr) == 0;
}


bool XString::StartsWith(const wchar_t* wcstr) const
{
    if(!wcstr)
        X_THROW(("NULL string passed to XString::StartsWith()."));
    return StartsWith(ConvertWideStringToMultiByteString(wcstr));
}


bool XString::StartsWith(const char* cstr) const
{
    if(!cstr)
        X_THROW(("NULL string passed to XString::StartsWith()."));

    const size_t otherSize = strlen(cstr);
    return otherSize <= _storage.size() && _storage.compare(0, otherSize, cstr, 0, otherSize) == 0;
}

bool XString::EndsWith(const XString& other) const
{
    const size_t otherSize = other._storage.size();
    const size_t thisSize = _storage.size();

    return otherSize <= thisSize && _storage.compare(thisSize - otherSize, otherSize, other._storage) == 0;
}

bool XString::EndsWith(const std::string& stdstr) const
{
    const size_t otherSize = stdstr.size();
    const size_t thisSize = _storage.size();

    return otherSize <= thisSize && _storage.compare(thisSize - otherSize, otherSize, stdstr) == 0;
}

bool XString::EndsWith(const wchar_t* wcstr) const
{
    if(!wcstr)
        X_THROW(("NULL string passed to XString::EndsWith()."));
    return EndsWith(ConvertWideStringToMultiByteString(wcstr));
}


bool XString::EndsWith(const char* cstr) const
{
    if(!cstr)
        X_THROW(("NULL string passed to XString::EndsWith()."));

    const size_t otherSize = strlen(cstr);
    const size_t thisSize = _storage.size();

    return otherSize <= thisSize && _storage.compare(thisSize - otherSize, otherSize, cstr, 0, otherSize) == 0;
}

//For some reason, I can't get isdigit to work directly (something about templates).
static bool verifyDigit(char c)
{
    //I should be able to just return the result, but Windows is stupid.
    return isdigit(c) != 0;
}

bool XString::IsInteger(bool canHaveSign) const
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

    return count_if(_storage.begin() + strippedFront, _storage.end() - strippedBack, verifyDigit) == numSize;
}

//For some reason, I can't get isxdigit to work directly (something about templates).
static bool verifyHexDigit(char c)
{
    //I should be able to just return the result, but Windows is stupid.
    return isxdigit(c) != 0;
}

bool XString::IsHex(bool canHaveSign) const
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

    return count_if(_storage.begin() + strippedFront, _storage.end() - strippedBack, verifyHexDigit) == numSize;
}

bool XString::IsDouble(bool canHaveSign) const
{
    const size_t decimal = _storage.find('.');

    if(decimal == string::npos)
        return IsInteger();

    const size_t first = _storage.find_first_not_of(' ');

    if(first == string::npos || (!canHaveSign && _storage[first] == '-'))
        return false;

    const size_t strippedFront = _storage[first] == '-' ? first + 1 : first;
    const size_t last = _storage.find_last_not_of(' ');
    const size_t strippedBack = last == string::npos ? 0 : (size() - 1) - last;
    const size_t numSize = (int)(size() - (strippedFront + strippedBack) - 1);

    if(numSize == 0)
        return false;

    const size_t numDigits = count_if(_storage.begin() + strippedFront, _storage.begin() + decimal, verifyDigit) +
                             count_if(_storage.begin() + decimal + 1, _storage.end() - (int)strippedBack, verifyDigit);

    return numDigits == numSize;
}

XString XString::LStrip() const
{
    XString retval = *this;
    retval.LStripInPlace();
    return retval;
}

XString XString::RStrip() const
{
    XString retval = *this;
    retval.RStripInPlace();
    return retval;
}

XString XString::Strip() const
{
    XString retval = *this;
    retval.StripInPlace();
    return retval;
}

XString& XString::LStripInPlace()
{
    size_t pos = 0;
    while(pos < _storage.size() && XString::IsSpace(_storage[pos])) pos++;
    _storage.erase(0, pos);
    return *this;
}

XString& XString::RStripInPlace()
{
    size_t pos = _storage.size();
    while(pos > 0 && XString::IsSpace(_storage[pos - 1])) pos--;
    _storage.erase(pos);
    return *this;
}

XString& XString::StripInPlace()
{
    LStripInPlace();
    RStripInPlace();
    return *this;
}

XString XString::StripEOL() const
{
    if(EndsWith("\r\n"))
        return substr(0, _storage.size() - 2);
    if(EndsWith("\n"))
        return substr(0, _storage.size() - 1);
    return *this;
}

XString XString::ReplaceAll(wchar_t toBeReplaced, wchar_t toReplaceWith) const
{
    wchar_t replace[] = {toBeReplaced, (wchar_t)NULL};
    vector<XString> splitList = Split(toBeReplaced);
    bool endedWith = false;
    XString replaced = "";

    if(StartsWith(replace))
        replaced += toReplaceWith;

    if(EndsWith(replace))
        endedWith = true;

    bool firstPass = true;

    for(vector<XString>::iterator iter = splitList.begin(), end  = splitList.end(); iter != end; ++iter)
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


XString XString::ReplaceAll(const XString& toBeReplaced, const XString& toReplaceWith) const
{
    size_t found = _storage.find(toBeReplaced);

    if(found == string::npos)
        return *this;

    XString retval = _storage.substr(0, found);
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


XString& XString::ReplaceAllInPlace(char toBeReplaced, char toReplaceWith)
{
    for(size_t i = _storage.find(toBeReplaced);
        i != string::npos;
        i = _storage.find(toBeReplaced, i))
    {
        _storage[i] = toReplaceWith;
    }

    return *this;
}

char XString::_EncodeByte(unsigned char u)
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

XString::CHAR_CLASS XString::_GetCharClass(char c)
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
bool XString::_IsValidURICharacter(char c)
{
    return isalnum(c) || c == '-' || c == '.' || c == '_' || c == '~';
}


std::string XString::ConvertUtf16StringToMultiByteString(const uint16_t* str)
{
    return ConvertUtf16StringToMultiByteString(str, (size_t)-1);
}

std::string XString::ConvertUtf16StringToMultiByteString(const uint16_t* str, size_t length)
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
std::vector<uint16_t> XString::ConvertMultiByteStringToUtf16String(const std::string& str)
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


std::string XString::ConvertUtf32StringToMultiByteString(const uint32_t* str)
{
    return ConvertUtf32StringToMultiByteString(str, (size_t)-1);
}


std::string XString::ConvertUtf32StringToMultiByteString(const uint32_t* str, size_t length)
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

std::vector<uint32_t> XString::ConvertMultiByteStringToUtf32String(const std::string& str)
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



std::string XString::ConvertWideStringToMultiByteString(const wchar_t* str)
{
  if(sizeof(wchar_t) != WIDE_CHAR_SIZE)
    X_THROW(("wchar_t size is [%d], and should be [%d]"));
#ifdef WIN32
  std::string result(ConvertUtf16StringToMultiByteString((uint16_t*)str));
#else//*NIX
  std::string result(ConvertUtf32StringToMultiByteString((uint32_t*)str));
#endif
  return result;
}


std::string XString::ConvertWideStringToMultiByteString(const wchar_t* str, size_t length)
{
  if(sizeof(wchar_t) != WIDE_CHAR_SIZE)
    X_THROW(("wchar_t size is [%d], and should be [%d]"));
#ifdef WIN32
  std::string result(ConvertUtf16StringToMultiByteString((uint16_t*)str, length));
#else//*NIX
  std::string result(ConvertUtf32StringToMultiByteString((uint32_t*)str, length));
#endif
  return result;
}


std::wstring XString::ConvertMultiByteStringToWideString(const std::string& str)
{
  if(sizeof(wchar_t) != WIDE_CHAR_SIZE)
    X_THROW(("wchar_t size is [%d], and should be [%d]"));
#ifdef WIN32
  std::vector<uint16_t> converted = ConvertMultiByteStringToUtf16String(str);
  std::wstring result(converted.begin(),converted.end());
#else//*NIX
  std::vector<uint32_t> converted = ConvertMultiByteStringToUtf32String(str);
  std::wstring result(converted.begin(),converted.end());
#endif
  return result;
}

int XSDK::Compare(const XString& left, const XString& right, void* context)
{
    return strcmp(left.c_str(), right.c_str());
}

int XSDK::Compare(const char* c_string, const XString& intString, void* context)
{
    return strcmp(c_string, intString.c_str());
}

int XSDK::Compare(const XString& intString, const char* c_string, void* context)
{
    return strcmp(intString.c_str(), c_string);
}
















/* This is a stupid sonar work around */
