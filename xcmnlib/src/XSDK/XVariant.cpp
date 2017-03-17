
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "XSDK/UuidUtils.h"
#include "XSDK/MemDebugBegin.h"
#include "XSDK/Errors.h"
#include "XSDK/Module.h"
#include "XSDK/XVariant.h"
#include "XSDK/MemDebugEnd.h"
#include "XSDK/UuidUtils.h"

using namespace XSDK;

//==============================================================================
// Certain MS compiler warnings can be disabled because they warn against
// things that are not a problem.
//==============================================================================
#ifdef IS_WINDOWS
    // Save the current state regarding which warnings are enables. This state
    // is restored at the end of this file.
    #pragma warning(push)

    // Disables "C++ exception specification ignored except to indicate a
    // function is not __declspec(nothrow)", related to "throw(XException)"
    // being declared on IXUnknown::QueryInterface.
    #pragma warning(disable:4290)
#endif

namespace XSDK
{

template<>
struct XVariant::_Conversion<bool,XSDK::XVariant::X_VARIANT>
{
    static bool Convert(const XSDK::XVariant::X_VARIANT & from)
    {
        switch(from.type)
        {
        case NUMTYPE_BOOL:
            return from.value.b;
        case NUMTYPE_CHAR:
            return from.value.c != 0;
        case NUMTYPE_SIGNED_CHAR:
            return from.value.sc != 0;
        case NUMTYPE_UNSIGNED_CHAR:
            return from.value.uc != 0;
        case NUMTYPE_SIGNED_SHORT:
            return from.value.ss != 0;
        case NUMTYPE_UNSIGNED_SHORT:
            return from.value.us != 0;
        case NUMTYPE_SIGNED_INT:
            return from.value.si != 0;
        case NUMTYPE_UNSIGNED_INT:
            return from.value.ui != 0;
        case NUMTYPE_SIGNED_LONG:
            return from.value.sl != 0;
        case NUMTYPE_UNSIGNED_LONG:
            return from.value.ul != 0;
        case NUMTYPE_SIGNED_LONG_LONG:
            return from.value.sll != 0;
        case NUMTYPE_UNSIGNED_LONG_LONG:
            return from.value.ull != 0;
        case NUMTYPE_FLOAT:
            return from.value.f != 0.0f;
        case NUMTYPE_DOUBLE:
            return from.value.d != 0.0;
        case NUMTYPE_LONG_DOUBLE:
            return from.value.ld != 0.0;
        case NUMTYPE_UUID:
            return false;
        default:
            X_THROW(("Could not convert\n"));
        }
    }
};

template<class To>
struct XVariant::_Conversion<To,XSDK::XVariant::X_VARIANT>
{
    static To Convert(const XSDK::XVariant::X_VARIANT & from)
    {
        switch(from.type)
        {
        case NUMTYPE_BOOL:
            return (To)from.value.b;
        case NUMTYPE_CHAR:
            return (To)from.value.c;
        case NUMTYPE_SIGNED_CHAR:
            return (To)from.value.sc;
        case NUMTYPE_UNSIGNED_CHAR:
            return (To)from.value.uc;
        case NUMTYPE_SIGNED_SHORT:
            return (To)from.value.ss;
        case NUMTYPE_UNSIGNED_SHORT:
            return (To)from.value.us;
        case NUMTYPE_SIGNED_INT:
            return (To)from.value.si;
        case NUMTYPE_UNSIGNED_INT:
            return (To)from.value.ui;
        case NUMTYPE_SIGNED_LONG:
            return (To)from.value.sl;
        case NUMTYPE_UNSIGNED_LONG:
            return (To)from.value.ul;
        case NUMTYPE_SIGNED_LONG_LONG:
            return (To)from.value.sll;
        case NUMTYPE_UNSIGNED_LONG_LONG:
            return (To)from.value.ull;
        case NUMTYPE_FLOAT:
            return (To)from.value.f;
        case NUMTYPE_DOUBLE:
            return (To)from.value.d;
        case NUMTYPE_LONG_DOUBLE:
            return (To)from.value.ld;
        case NUMTYPE_UUID:
            return (To)0;
        default:
            X_THROW(("Could not convert\n"));
        }
    }
};

template<>
struct XVariant::_Conversion<X_UUID,XSDK::XVariant::X_VARIANT>
{
    static X_UUID Convert(const XSDK::XVariant::X_VARIANT & from)
    {
        if(from.type == NUMTYPE_UUID)
            return from.value.uuid;
        return X_EMPTY_UUID;
    }
};

template<>
struct XVariant::_Conversion<XSDK::XString,XSDK::XVariant::X_VARIANT>
{
    static XSDK::XString Convert(const XSDK::XVariant::X_VARIANT & from)
    {
        switch(from.type)
        {
        case XVARTYPE_BOOL:
            return from.value.b ? "true" : "false";
        case XVARTYPE_CHAR:
            return XSDK::XString::Format("%c",from.value.c);
        case XVARTYPE_SIGNED_CHAR:
            return XSDK::XString::Format("%c",from.value.sc);
        case XVARTYPE_UNSIGNED_CHAR:
            return XSDK::XString::Format("%c",from.value.uc);
        case XVARTYPE_SIGNED_SHORT:
            return XSDK::XString::Format("%hd",from.value.ss);
        case XVARTYPE_UNSIGNED_SHORT:
            return XSDK::XString::Format("%hu",from.value.us);
        case XVARTYPE_SIGNED_INT:
            return XSDK::XString::Format("%d",from.value.si);
        case XVARTYPE_UNSIGNED_INT:
            return XSDK::XString::Format("%u",from.value.ui);
        case XVARTYPE_SIGNED_LONG:
            return XSDK::XString::Format("%ld",from.value.sl);
        case XVARTYPE_UNSIGNED_LONG:
            return XSDK::XString::Format("%lu",from.value.ul);
        case XVARTYPE_SIGNED_LONG_LONG:
            return XSDK::XString::Format("%lld",from.value.sll);
        case XVARTYPE_UNSIGNED_LONG_LONG:
            return XSDK::XString::Format("%llu",from.value.ull);
        case XVARTYPE_FLOAT:
            return XSDK::XString::Format("%f",from.value.f);
        case XVARTYPE_DOUBLE:
            return XSDK::XString::Format("%f",from.value.d);
        case XVARTYPE_LONG_DOUBLE:
            return XSDK::XString::Format("%Lf",from.value.ld);
        case XVARTYPE_UUID:
        {
            char to[X_UUID_STRING_LENGTH+1];
            if(XUuidToString(&from.value.uuid,to,X_UUID_STRING_LENGTH+1))
                return to;
            return "0-0-0-0-00";
        }
        default:
            X_THROW(("Could not convert from a numeric %d to text\n",from.type));
        }
    }
};

template<>
struct XVariant::_Conversion<XSDK::XMemory,XSDK::XVariant::X_VARIANT>
{
    static XSDK::XMemory Convert(const XSDK::XVariant::X_VARIANT & from)
    {
        XSDK::XMemory to;
        switch(from.type)
        {
        case XVARTYPE_BOOL:
            memcpy(&to.Extend(sizeof(bool)),&from.value.b,sizeof(bool));
            break;
        case XVARTYPE_CHAR:
            memcpy(&to.Extend(sizeof(char)),&from.value.c,sizeof(char));
            break;
        case XVARTYPE_SIGNED_CHAR:
            memcpy(&to.Extend(sizeof(signed char)),&from.value.sc,sizeof(signed char));
            break;
        case XVARTYPE_UNSIGNED_CHAR:
            memcpy(&to.Extend(sizeof(unsigned char)),&from.value.uc,sizeof(unsigned char));
            break;
        case XVARTYPE_SIGNED_SHORT:
            memcpy(&to.Extend(sizeof(signed short)),&from.value.ss,sizeof(signed short));
            break;
        case XVARTYPE_UNSIGNED_SHORT:
            memcpy(&to.Extend(sizeof(unsigned short)),&from.value.us,sizeof(unsigned short));
            break;
        case XVARTYPE_SIGNED_INT:
            memcpy(&to.Extend(sizeof(signed int)),&from.value.si,sizeof(signed int));
            break;
        case XVARTYPE_UNSIGNED_INT:
            memcpy(&to.Extend(sizeof(unsigned int)),&from.value.si,sizeof(unsigned int));
            break;
        case XVARTYPE_SIGNED_LONG:
            memcpy(&to.Extend(sizeof(signed long)),&from.value.sl,sizeof(signed long));
            break;
        case XVARTYPE_UNSIGNED_LONG:
            memcpy(&to.Extend(sizeof(unsigned long)),&from.value.ul,sizeof(unsigned long));
            break;
        case XVARTYPE_SIGNED_LONG_LONG:
            memcpy(&to.Extend(sizeof(signed long long)),&from.value.sll,sizeof(signed long long));
            break;
        case XVARTYPE_UNSIGNED_LONG_LONG:
            memcpy(&to.Extend(sizeof(unsigned long long)),&from.value.ull,sizeof(unsigned long long));
            break;
        case XVARTYPE_FLOAT:
            memcpy(&to.Extend(sizeof(float)),&from.value.f,sizeof(float));
            break;
        case XVARTYPE_DOUBLE:
            memcpy(&to.Extend(sizeof(double)),&from.value.d,sizeof(double));
            break;
        case XVARTYPE_LONG_DOUBLE:
            memcpy(&to.Extend(sizeof(long double)),&from.value.ld,sizeof(long double));
            break;
        case XVARTYPE_UUID:
            memcpy(&to.Extend(sizeof(X_UUID)),&from.value.uuid,sizeof(X_UUID));
            break;
        default:
            X_THROW(("Could not convert from a numeric type to %d",from.type));
        }
        return to;
    }
};

template<>
struct XVariant::_Conversion<bool,XSDK::XString>
{
    static bool Convert(const XSDK::XString & from)
    {
        return from.ToInt() != 0;
    }
};

template<>
struct XVariant::_Conversion<char,XSDK::XString>
{
    static char Convert(const XSDK::XString & from)
    {
        return (char)from.ToInt();
    }
};

template<>
struct XVariant::_Conversion<signed char,XSDK::XString>
{
    static signed char Convert(const XSDK::XString & from)
    {
        return (signed char)from.ToInt();
    }
};

template<>
struct XVariant::_Conversion<unsigned char,XSDK::XString>
{
    static unsigned char Convert(const XSDK::XString & from)
    {
        return (unsigned char)from.ToUInt();
    }
};

template<>
struct XVariant::_Conversion<signed short,XSDK::XString>
{
    static short Convert(const XSDK::XString & from)
    {
        return (short)from.ToInt();
    }
};

template<>
struct XVariant::_Conversion<unsigned short,XSDK::XString>
{
    static short Convert(const XSDK::XString & from)
    {
        return (unsigned short)from.ToUInt();
    }
};

template<>
struct XVariant::_Conversion<signed int,XSDK::XString>
{
    static int Convert(const XSDK::XString & from)
    {
        return from.ToInt();
    }
};

template<>
struct XVariant::_Conversion<unsigned int,XSDK::XString>
{
    static unsigned int Convert(const XSDK::XString & from)
    {
        return from.ToUInt();
    }
};

template<>
struct XVariant::_Conversion<signed long,XSDK::XString>
{
    static long Convert(const XSDK::XString & from)
    {
        return (long)from.ToInt64();
    }
};

template<>
struct XVariant::_Conversion<unsigned long,XSDK::XString>
{
    static unsigned long Convert(const XSDK::XString & from)
    {
        return (unsigned long)from.ToUInt64();
    }
};

template<>
struct XVariant::_Conversion<signed long long,XSDK::XString>
{
    static long long Convert(const XSDK::XString & from)
    {
        //Truncates on 64 bit Linux, but I don't think that we'll ever care.
        return (long long)from.ToInt64();
    }
};

template<>
struct XVariant::_Conversion<unsigned long long,XSDK::XString>
{
    static unsigned long long Convert(const XSDK::XString & from)
    {
        //Truncates on 64 bit Linux, but I don't think that we'll ever care.
        return (unsigned long long)from.ToUInt64();
    }
};

template<>
struct XVariant::_Conversion<float,XSDK::XString>
{
    static float Convert(const XSDK::XString & from)
    {
        return (float)(from.ToDouble());
    }
};

template<>
struct XVariant::_Conversion<double,XSDK::XString>
{
    static double Convert(const XSDK::XString & from)
    {
        return from.ToDouble();
    }
};

template<>
struct XVariant::_Conversion<long double,XSDK::XString>
{
    static long double Convert(const XSDK::XString & from)
    {
        return (long double)from.ToDouble();
    }
};

template<>
struct XVariant::_Conversion<X_UUID,XSDK::XString>
{
    static X_UUID Convert(const XSDK::XString & from)
    {
        X_UUID to;
        XUuidFromString(from.c_str(),&to);
        return to;
    }
};

template<>
struct XVariant::_Conversion<XSDK::XString,XSDK::XString>
{
    static XSDK::XString Convert(const XSDK::XString & from)
    {
        return from;
    }
};

template<>
struct XVariant::_Conversion<XSDK::XMemory,XSDK::XString>
{
    static XSDK::XMemory Convert(const XSDK::XString & from)
    {
        XSDK::XMemory to;
        memcpy(&to.Extend(from.size()),from.data(),from.size());
        return to;
    }
};

template<class To>
struct XVariant::_Conversion<To,XSDK::XMemory>
{
    static To Convert(const XSDK::XMemory & from)
    {
        To to;
        memcpy(&to,from.Map(),sizeof(to));
        return to;
    }
};

template<>
struct XVariant::_Conversion<XSDK::XString,XSDK::XMemory>
{
    static XSDK::XString Convert(const XSDK::XMemory & from)
    {
        XSDK::XString to((const char *)from.Map(),from.GetDataSize());
        return to;
    }
};

template<>
struct XVariant::_Conversion<XSDK::XMemory,XSDK::XMemory>
{
    static XSDK::XMemory Convert(const XSDK::XMemory & from)
    {
        return from;
    }
};

};

XVariant::XVariant() : _type(XVARTYPE_EMPTY),_variant(),_text(),_bytes()
{
    Clear();
}

XVariant::XVariant( bool value ) : _type(XVARTYPE_EMPTY),_variant(),_text(),_bytes()
{
    *this = value;
}

XVariant::XVariant( char value ) : _type(XVARTYPE_EMPTY),_variant(),_text(),_bytes()
{
    *this = value;
}

XVariant::XVariant( signed char value ) : _type(XVARTYPE_EMPTY),_variant(),_text(),_bytes()
{
    *this = value;
}

XVariant::XVariant( unsigned char value ) : _type(XVARTYPE_EMPTY),_variant(),_text(),_bytes()
{
    *this = value;
}

XVariant::XVariant( signed short value ) : _type(XVARTYPE_EMPTY),_variant(),_text(),_bytes()
{
    *this = value;
}

XVariant::XVariant( unsigned short value ) : _type(XVARTYPE_EMPTY),_variant(),_text(),_bytes()
{
    *this = value;
}

XVariant::XVariant( signed int value ) : _type(XVARTYPE_EMPTY),_variant(),_text(),_bytes()
{
    *this = value;
}

XVariant::XVariant( unsigned int value ) : _type(XVARTYPE_EMPTY),_variant(),_text(),_bytes()
{
    *this = value;
}

XVariant::XVariant( signed long value ) : _type(XVARTYPE_EMPTY),_variant(),_text(),_bytes()
{
    *this = value;
}

XVariant::XVariant( unsigned long value ) : _type(XVARTYPE_EMPTY),_variant(),_text(),_bytes()
{
    *this = value;
}

XVariant::XVariant( signed long long value ) : _type(XVARTYPE_EMPTY),_variant(),_text(),_bytes()
{
    *this = value;
}

XVariant::XVariant( unsigned long long value ) : _type(XVARTYPE_EMPTY),_variant(),_text(),_bytes()
{
    *this = value;
}

XVariant::XVariant( float value ) : _type(XVARTYPE_EMPTY),_variant(),_text(),_bytes()
{
    *this = value;
}

XVariant::XVariant( double value ) : _type(XVARTYPE_EMPTY),_variant(),_text(),_bytes()
{
    *this = value;
}

XVariant::XVariant( long double value ) : _type(XVARTYPE_EMPTY),_variant(),_text(),_bytes()
{
    *this = value;
}

XVariant::XVariant( const X_UUID & value ) : _type(XVARTYPE_EMPTY),_variant(),_text(),_bytes()
{
    *this = value;
}

XVariant::XVariant( const char * value ) : _type(XVARTYPE_EMPTY),_variant(),_text(),_bytes()
{
    *this = XSDK::XString(value);
}

XVariant::XVariant( const XSDK::XString & value ) : _type(XVARTYPE_EMPTY),_variant(),_text(),_bytes()
{
    *this = value;
}

XVariant::XVariant( const XSDK::XMemory & value ) : _type(XVARTYPE_EMPTY),_variant(),_text(),_bytes()
{
    *this = value;
}

XVariant::XVariant( void * value ) : _type(XVARTYPE_EMPTY),_variant(),_text(),_bytes()
{
    *this = value;
}

XVariant::XVariant( const XVariant& other ) : _type(XVARTYPE_EMPTY),_variant(),_text(),_bytes()
{
    *this = other;
}

XVariant::~XVariant() throw()
{
    Clear();
}

XSDK::X_VARIANT_TYPE XVariant::GetType() const
{
    return _type;
}

void XVariant::Clear()
{
    _type = XVARTYPE_EMPTY;
    memset(&_variant, 0, sizeof(_variant));
    _text.clear();
    _bytes.Clear();
}

bool XVariant::IsEmpty() const
{
    return (_type == XVARTYPE_EMPTY);
}

XVariant& XVariant::operator=( const XVariant& other )
{
    _type = other._type;
    _variant = other._variant;
    _text = other._text;
    _bytes = other._bytes;

    return *this;
}

XVariant& XVariant::operator=( bool value )
{
    Clear();
    _type = XVARTYPE_BOOL;
    _variant.type = NUMTYPE_BOOL;
    _variant.value.b = value;
    return *this;
}

XVariant& XVariant::operator=( char value )
{
    Clear();
    _type = XVARTYPE_CHAR;
    _variant.type = NUMTYPE_CHAR;
    _variant.value.c = value;
    return *this;
}

XVariant& XVariant::operator=( signed char value )
{
    Clear();
    _type = XVARTYPE_SIGNED_CHAR;
    _variant.type = NUMTYPE_SIGNED_CHAR;
    _variant.value.sc = value;
    return *this;
}

XVariant& XVariant::operator=( unsigned char value )
{
    Clear();
    _type = XVARTYPE_UNSIGNED_CHAR;
    _variant.type = NUMTYPE_UNSIGNED_CHAR;
    _variant.value.uc = value;
    return *this;
}

XVariant& XVariant::operator=( signed short value )
{
    Clear();
    _type = XVARTYPE_SIGNED_SHORT;
    _variant.type = NUMTYPE_SIGNED_SHORT;
    _variant.value.ss = value;
    return *this;
}

XVariant& XVariant::operator=( unsigned short value )
{
    Clear();
    _type = XVARTYPE_UNSIGNED_SHORT;
    _variant.type = NUMTYPE_UNSIGNED_SHORT;
    _variant.value.us = value;
    return *this;
}

XVariant& XVariant::operator=( signed int value )
{
    Clear();
    _type = XVARTYPE_SIGNED_INT;
    _variant.type = NUMTYPE_SIGNED_INT;
    _variant.value.si = value;
    return *this;
}

XVariant& XVariant::operator=( unsigned int value )
{
    Clear();
    _type = XVARTYPE_UNSIGNED_INT;
    _variant.type = NUMTYPE_UNSIGNED_INT;
    _variant.value.ui = value;
    return *this;
}

XVariant& XVariant::operator=( signed long value )
{
    Clear();
    _type = XVARTYPE_SIGNED_LONG;
    _variant.type = NUMTYPE_SIGNED_LONG;
    _variant.value.sl = value;
    return *this;
}

XVariant& XVariant::operator=( unsigned long value )
{
    Clear();
    _type = XVARTYPE_UNSIGNED_LONG;
    _variant.type = NUMTYPE_UNSIGNED_LONG;
    _variant.value.ul = value;
    return *this;
}

XVariant& XVariant::operator=( signed long long value )
{
    Clear();
    _type = XVARTYPE_SIGNED_LONG_LONG;
    _variant.type = NUMTYPE_SIGNED_LONG_LONG;
    _variant.value.sll = value;
    return *this;
}

XVariant& XVariant::operator=( unsigned long long value )
{
    Clear();
    _type = XVARTYPE_UNSIGNED_LONG_LONG;
    _variant.type = NUMTYPE_UNSIGNED_LONG_LONG;
    _variant.value.ull = value;
    return *this;
}

XVariant& XVariant::operator=( float value )
{
    Clear();
    _type = XVARTYPE_FLOAT;
    _variant.type = NUMTYPE_FLOAT;
    _variant.value.f = value;
    return *this;
}

XVariant& XVariant::operator=( double value )
{
    Clear();
    _type = XVARTYPE_DOUBLE;
    _variant.type = NUMTYPE_DOUBLE;
    _variant.value.d = value;
    return *this;
}

XVariant& XVariant::operator=( long double value )
{
    Clear();
    _type = XVARTYPE_LONG_DOUBLE;
    _variant.type = NUMTYPE_LONG_DOUBLE;
    _variant.value.ld = value;
    return *this;
}

XVariant& XVariant::operator=( const X_UUID & value )
{
    Clear();
    _type = XVARTYPE_UUID;
    _variant.type = NUMTYPE_UUID;
    XUuidCopy(&value, &_variant.value.uuid);
    return *this;
}

XVariant& XVariant::operator=( const XSDK::XString & value )
{
    Clear();
    _type = XVARTYPE_TEXT;
    _text = value;
    return *this;
}

XVariant& XVariant::operator=( const XSDK::XMemory & val )
{
    Clear();
    _type = XVARTYPE_BYTES;
    _bytes = val;
    return *this;
}

XVariant& XVariant::operator=( void * value )
{
    Clear();
    _type = XVARTYPE_PTR;
    _variant.type = NUMTYPE_PTR;
    _variant.value.ptr = value;
    return *this;
}

bool XVariant::_GetAsBool() const
{
    return _Convert<bool>();
}

char XVariant::_GetAsChar() const
{
    return _Convert<char>();
}

signed char XVariant::_GetAsSignedChar() const
{
    return _Convert<signed char>();
}

unsigned char XVariant::_GetAsUnsignedChar() const
{
    return _Convert<unsigned char>();
}

signed short XVariant::_GetAsSignedShort() const
{
    return _Convert<signed short>();
}

unsigned short XVariant::_GetAsUnsignedShort() const
{
    return _Convert<unsigned short>();
}

signed int XVariant::_GetAsSignedInt() const
{
    return _Convert<signed int>();
}

unsigned int XVariant::_GetAsUnsignedInt() const
{
    return _Convert<unsigned int>();
}

signed long XVariant::_GetAsSignedLong() const
{
    return _Convert<signed long>();
}

unsigned long XVariant::_GetAsUnsignedLong() const
{
    return _Convert<unsigned long>();
}

signed long long XVariant::_GetAsSignedLongLong() const
{
    return _Convert<signed long long>();
}

unsigned long long XVariant::_GetAsUnsignedLongLong() const
{
    return _Convert<unsigned long long>();
}

float XVariant::_GetAsFloat() const
{
    return _Convert<float>();
}

double XVariant::_GetAsDouble() const
{
    return _Convert<double>();
}

long double XVariant::_GetAsLongDouble() const
{
    return _Convert<long double>();
}

X_UUID XVariant::_GetAsUuid() const
{
    return _Convert<X_UUID>();
}

XSDK::XString XVariant::_GetAsString() const
{
    return _Convert<XSDK::XString>();
}

XSDK::XMemory XVariant::_GetAsBytes() const
{
    return _Convert<XSDK::XMemory>();
}

void * XVariant::_GetAsPtr() const
{
    if(_type != XVARTYPE_PTR)
        return NULL;
    return _variant.value.ptr;
}

template<class To>
To XVariant::_Convert() const
{
    switch(_type)
    {
    case XVARTYPE_BOOL:
    case XVARTYPE_CHAR:
    case XVARTYPE_SIGNED_CHAR:
    case XVARTYPE_UNSIGNED_CHAR:
    case XVARTYPE_SIGNED_SHORT:
    case XVARTYPE_UNSIGNED_SHORT:
    case XVARTYPE_SIGNED_INT:
    case XVARTYPE_UNSIGNED_INT:
    case XVARTYPE_SIGNED_LONG:
    case XVARTYPE_UNSIGNED_LONG:
    case XVARTYPE_SIGNED_LONG_LONG:
    case XVARTYPE_UNSIGNED_LONG_LONG:
    case XVARTYPE_FLOAT:
    case XVARTYPE_DOUBLE:
    case XVARTYPE_LONG_DOUBLE:
    case XVARTYPE_UUID:
        return _Conversion<To,XSDK::XVariant::X_VARIANT>::Convert(_variant);
    case XVARTYPE_BYTES:
        return _Conversion<To,XSDK::XMemory>::Convert(_bytes);
    case XVARTYPE_TEXT:
        return _Conversion<To,XSDK::XString>::Convert(_text);
    default:
        X_THROW(("Could not convert %d\n",_type));
    }
}

bool XVariant::operator==( const XVariant& other ) const
{
    if(_type == other._type)
    {
        switch(_type)
        {
        case XVARTYPE_BOOL:
            return _variant.value.b == other._variant.value.b;
        case XVARTYPE_CHAR:
            return _variant.value.c == other._variant.value.c;
        case XVARTYPE_SIGNED_CHAR:
            return _variant.value.sc == other._variant.value.sc;
        case XVARTYPE_UNSIGNED_CHAR:
            return _variant.value.uc == other._variant.value.uc;
        case XVARTYPE_SIGNED_SHORT:
            return _variant.value.ss == other._variant.value.ss;
        case XVARTYPE_UNSIGNED_SHORT:
            return _variant.value.us == other._variant.value.us;
        case XVARTYPE_SIGNED_INT:
            return _variant.value.si == other._variant.value.si;
        case XVARTYPE_UNSIGNED_INT:
            return _variant.value.ui == other._variant.value.ui;
        case XVARTYPE_SIGNED_LONG:
            return _variant.value.sl == other._variant.value.sl;
        case XVARTYPE_UNSIGNED_LONG:
            return _variant.value.ul == other._variant.value.ul;
        case XVARTYPE_SIGNED_LONG_LONG:
            return _variant.value.sll == other._variant.value.sll;
        case XVARTYPE_UNSIGNED_LONG_LONG:
            return _variant.value.ull == other._variant.value.ull;
        case XVARTYPE_FLOAT:
            return _variant.value.f == other._variant.value.f;
        case XVARTYPE_DOUBLE:
            return _variant.value.d == other._variant.value.d;
        case XVARTYPE_LONG_DOUBLE:
            return _variant.value.ld == other._variant.value.ld;
        case XVARTYPE_UUID:
            return memcmp(&_variant.value.uuid,&other._variant.value.uuid,sizeof(X_UUID)) == 0;
        case XVARTYPE_BYTES:
        {
            if(_bytes.GetDataSize() != other._bytes.GetDataSize())
                return false;
            return memcmp(_bytes.Map(),other._bytes.Map(),_bytes.GetDataSize()) == 0;
        }
        case XVARTYPE_TEXT:
            return _text == other._text;
        default:
            X_THROW(("Cannot compare these two XVariant objects\n"));
        }
    }
    else
        return false;
}

bool XVariant::operator!=( const XVariant& other ) const
{
    return !(operator==(other));
}

#ifdef IS_WINDOWS
    #pragma warning(pop)
#endif
