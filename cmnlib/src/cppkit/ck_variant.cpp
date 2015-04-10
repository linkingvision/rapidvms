
#include "cppkit/ck_variant.h"
#include "cppkit/ck_uuid_utils.h"

using namespace cppkit;

namespace cppkit
{

template<>
struct ck_variant::_conversion<bool,ck_variant::CK_VARIANT>
{
    static bool convert(const ck_variant::CK_VARIANT & from)
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
            CK_THROW(("Could not convert\n"));
        }
    }
};

template<class To>
struct ck_variant::_conversion<To,ck_variant::CK_VARIANT>
{
    static To convert(const ck_variant::CK_VARIANT & from)
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
            CK_THROW(("Could not convert\n"));
        }
    }
};

template<>
struct ck_variant::_conversion<ck_uuid_bits,ck_variant::CK_VARIANT>
{
    static ck_uuid_bits convert(const ck_variant::CK_VARIANT & from)
    {
        if(from.type == NUMTYPE_UUID)
            return from.value.uuid;
        return CK_EMPTY_UUID;
    }
};

template<>
struct ck_variant::_conversion<ck_string,ck_variant::CK_VARIANT>
{
    static ck_string convert(const ck_variant::CK_VARIANT & from)
    {
        switch(from.type)
        {
        case CK_VARTYPE_BOOL:
            return from.value.b ? "true" : "false";
        case CK_VARTYPE_CHAR:
            return ck_string::format("%c",from.value.c);
        case CK_VARTYPE_SIGNED_CHAR:
            return ck_string::format("%c",from.value.sc);
        case CK_VARTYPE_UNSIGNED_CHAR:
            return ck_string::format("%c",from.value.uc);
        case CK_VARTYPE_SIGNED_SHORT:
            return ck_string::format("%hd",from.value.ss);
        case CK_VARTYPE_UNSIGNED_SHORT:
            return ck_string::format("%hu",from.value.us);
        case CK_VARTYPE_SIGNED_INT:
            return ck_string::format("%d",from.value.si);
        case CK_VARTYPE_UNSIGNED_INT:
            return ck_string::format("%u",from.value.ui);
        case CK_VARTYPE_SIGNED_LONG:
            return ck_string::format("%ld",from.value.sl);
        case CK_VARTYPE_UNSIGNED_LONG:
            return ck_string::format("%lu",from.value.ul);
        case CK_VARTYPE_SIGNED_LONG_LONG:
            return ck_string::format("%lld",from.value.sll);
        case CK_VARTYPE_UNSIGNED_LONG_LONG:
            return ck_string::format("%llu",from.value.ull);
        case CK_VARTYPE_FLOAT:
            return ck_string::format("%f",from.value.f);
        case CK_VARTYPE_DOUBLE:
            return ck_string::format("%f",from.value.d);
        case CK_VARTYPE_LONG_DOUBLE:
            return ck_string::format("%Lf",from.value.ld);
        case CK_VARTYPE_UUID:
        {
            char to[CK_UUID_STRING_LENGTH+1];
            if(ck_uuid_to_string(&from.value.uuid,to,CK_UUID_STRING_LENGTH+1))
                return to;
            return "0-0-0-0-00";
        }
        default:
            CK_THROW(("Could not convert from a numeric %d to text\n",from.type));
        }
    }
};

template<>
struct ck_variant::_conversion<ck_memory,ck_variant::CK_VARIANT>
{
    static ck_memory convert(const ck_variant::CK_VARIANT & from)
    {
        ck_memory to;
        switch(from.type)
        {
        case CK_VARTYPE_BOOL:
            memcpy(to.extend_data(sizeof(bool)).get_ptr(),&from.value.b,sizeof(bool));
            break;
        case CK_VARTYPE_CHAR:
            memcpy(to.extend_data(sizeof(char)).get_ptr(),&from.value.c,sizeof(char));
            break;
        case CK_VARTYPE_SIGNED_CHAR:
            memcpy(to.extend_data(sizeof(signed char)).get_ptr(),&from.value.sc,sizeof(signed char));
            break;
        case CK_VARTYPE_UNSIGNED_CHAR:
            memcpy(to.extend_data(sizeof(unsigned char)).get_ptr(),&from.value.uc,sizeof(unsigned char));
            break;
        case CK_VARTYPE_SIGNED_SHORT:
            memcpy(to.extend_data(sizeof(signed short)).get_ptr(),&from.value.ss,sizeof(signed short));
            break;
        case CK_VARTYPE_UNSIGNED_SHORT:
            memcpy(to.extend_data(sizeof(unsigned short)).get_ptr(),&from.value.us,sizeof(unsigned short));
            break;
        case CK_VARTYPE_SIGNED_INT:
            memcpy(to.extend_data(sizeof(signed int)).get_ptr(),&from.value.si,sizeof(signed int));
            break;
        case CK_VARTYPE_UNSIGNED_INT:
            memcpy(to.extend_data(sizeof(unsigned int)).get_ptr(),&from.value.si,sizeof(unsigned int));
            break;
        case CK_VARTYPE_SIGNED_LONG:
            memcpy(to.extend_data(sizeof(signed long)).get_ptr(),&from.value.sl,sizeof(signed long));
            break;
        case CK_VARTYPE_UNSIGNED_LONG:
            memcpy(to.extend_data(sizeof(unsigned long)).get_ptr(),&from.value.ul,sizeof(unsigned long));
            break;
        case CK_VARTYPE_SIGNED_LONG_LONG:
            memcpy(to.extend_data(sizeof(signed long long)).get_ptr(),&from.value.sll,sizeof(signed long long));
            break;
        case CK_VARTYPE_UNSIGNED_LONG_LONG:
            memcpy(to.extend_data(sizeof(unsigned long long)).get_ptr(),&from.value.ull,sizeof(unsigned long long));
            break;
        case CK_VARTYPE_FLOAT:
            memcpy(to.extend_data(sizeof(float)).get_ptr(),&from.value.f,sizeof(float));
            break;
        case CK_VARTYPE_DOUBLE:
            memcpy(to.extend_data(sizeof(double)).get_ptr(),&from.value.d,sizeof(double));
            break;
        case CK_VARTYPE_LONG_DOUBLE:
            memcpy(to.extend_data(sizeof(long double)).get_ptr(),&from.value.ld,sizeof(long double));
            break;
        case CK_VARTYPE_UUID:
            memcpy(to.extend_data(sizeof(ck_uuid_bits)).get_ptr(),&from.value.uuid,sizeof(ck_uuid_bits));
            break;
        default:
            CK_THROW(("Could not convert from a numeric type to %d",from.type));
        }
        return to;
    }
};

template<>
struct ck_variant::_conversion<bool,ck_string>
{
    static bool convert(const ck_string & from)
    {
        return from.to_int() != 0;
    }
};

template<>
struct ck_variant::_conversion<char,ck_string>
{
    static char convert(const ck_string & from)
    {
        return (char)from.to_int();
    }
};

template<>
struct ck_variant::_conversion<signed char,ck_string>
{
    static signed char convert(const ck_string & from)
    {
        return (signed char)from.to_int();
    }
};

template<>
struct ck_variant::_conversion<unsigned char,ck_string>
{
    static unsigned char convert(const ck_string & from)
    {
        return (unsigned char)from.to_uint();
    }
};

template<>
struct ck_variant::_conversion<signed short,ck_string>
{
    static short convert(const ck_string & from)
    {
        return (short)from.to_int();
    }
};

template<>
struct ck_variant::_conversion<unsigned short,ck_string>
{
    static short convert(const ck_string & from)
    {
        return (unsigned short)from.to_uint();
    }
};

template<>
struct ck_variant::_conversion<signed int,ck_string>
{
    static int convert(const ck_string & from)
    {
        return from.to_int();
    }
};

template<>
struct ck_variant::_conversion<unsigned int,ck_string>
{
    static unsigned int convert(const ck_string & from)
    {
        return from.to_uint();
    }
};

template<>
struct ck_variant::_conversion<signed long,ck_string>
{
    static long convert(const ck_string & from)
    {
        return (long)from.to_int64();
    }
};

template<>
struct ck_variant::_conversion<unsigned long,ck_string>
{
    static unsigned long convert(const ck_string & from)
    {
        return (unsigned long)from.to_uint64();
    }
};

template<>
struct ck_variant::_conversion<signed long long,ck_string>
{
    static long long convert(const ck_string & from)
    {
        //Truncates on 64 bit Linux, but I don't think that we'll ever care.
        return (long long)from.to_int64();
    }
};

template<>
struct ck_variant::_conversion<unsigned long long,ck_string>
{
    static unsigned long long convert(const ck_string & from)
    {
        //Truncates on 64 bit Linux, but I don't think that we'll ever care.
        return (unsigned long long)from.to_uint64();
    }
};

template<>
struct ck_variant::_conversion<float,ck_string>
{
    static float convert(const ck_string & from)
    {
        return (float)(from.to_double());
    }
};

template<>
struct ck_variant::_conversion<double,ck_string>
{
    static double convert(const ck_string & from)
    {
        return from.to_double();
    }
};

template<>
struct ck_variant::_conversion<long double,ck_string>
{
    static long double convert(const ck_string & from)
    {
        return (long double)from.to_double();
    }
};

template<>
struct ck_variant::_conversion<ck_uuid_bits,ck_string>
{
    static ck_uuid_bits convert(const ck_string & from)
    {
        ck_uuid_bits to;
        ck_uuid_from_string(from.c_str(),&to);
        return to;
    }
};

template<>
struct ck_variant::_conversion<ck_string,ck_string>
{
    static ck_string convert(const ck_string & from)
    {
        return from;
    }
};

template<>
struct ck_variant::_conversion<ck_memory,ck_string>
{
    static ck_memory convert(const ck_string & from)
    {
        ck_memory to;
        memcpy(to.extend_data(from.size()).get_ptr(),from.data(),from.size());
        return to;
    }
};

template<class To>
struct ck_variant::_conversion<To,ck_memory>
{
    static To convert(const ck_memory & from)
    {
        To to;
        memcpy(&to,from.map().get_ptr(),sizeof(to));
        return to;
    }
};

template<>
struct ck_variant::_conversion<ck_string,ck_memory>
{
    static ck_string convert(const ck_memory & from)
    {
        ck_string to((const char *)from.map().get_ptr(),from.size_data());
        return to;
    }
};

template<>
struct ck_variant::_conversion<ck_memory,ck_memory>
{
    static ck_memory convert(const ck_memory & from)
    {
        return from;
    }
};

}

ck_variant::ck_variant() : _type(CK_VARTYPE_EMPTY),_variant(),_text(),_bytes()
{
    clear();
}

ck_variant::ck_variant( bool value ) : _type(CK_VARTYPE_EMPTY),_variant(),_text(),_bytes()
{
    *this = value;
}

ck_variant::ck_variant( char value ) : _type(CK_VARTYPE_EMPTY),_variant(),_text(),_bytes()
{
    *this = value;
}

ck_variant::ck_variant( signed char value ) : _type(CK_VARTYPE_EMPTY),_variant(),_text(),_bytes()
{
    *this = value;
}

ck_variant::ck_variant( unsigned char value ) : _type(CK_VARTYPE_EMPTY),_variant(),_text(),_bytes()
{
    *this = value;
}

ck_variant::ck_variant( signed short value ) : _type(CK_VARTYPE_EMPTY),_variant(),_text(),_bytes()
{
    *this = value;
}

ck_variant::ck_variant( unsigned short value ) : _type(CK_VARTYPE_EMPTY),_variant(),_text(),_bytes()
{
    *this = value;
}

ck_variant::ck_variant( signed int value ) : _type(CK_VARTYPE_EMPTY),_variant(),_text(),_bytes()
{
    *this = value;
}

ck_variant::ck_variant( unsigned int value ) : _type(CK_VARTYPE_EMPTY),_variant(),_text(),_bytes()
{
    *this = value;
}

ck_variant::ck_variant( signed long value ) : _type(CK_VARTYPE_EMPTY),_variant(),_text(),_bytes()
{
    *this = value;
}

ck_variant::ck_variant( unsigned long value ) : _type(CK_VARTYPE_EMPTY),_variant(),_text(),_bytes()
{
    *this = value;
}

ck_variant::ck_variant( signed long long value ) : _type(CK_VARTYPE_EMPTY),_variant(),_text(),_bytes()
{
    *this = value;
}

ck_variant::ck_variant( unsigned long long value ) : _type(CK_VARTYPE_EMPTY),_variant(),_text(),_bytes()
{
    *this = value;
}

ck_variant::ck_variant( float value ) : _type(CK_VARTYPE_EMPTY),_variant(),_text(),_bytes()
{
    *this = value;
}

ck_variant::ck_variant( double value ) : _type(CK_VARTYPE_EMPTY),_variant(),_text(),_bytes()
{
    *this = value;
}

ck_variant::ck_variant( long double value ) : _type(CK_VARTYPE_EMPTY),_variant(),_text(),_bytes()
{
    *this = value;
}

ck_variant::ck_variant( const ck_uuid_bits & value ) : _type(CK_VARTYPE_EMPTY),_variant(),_text(),_bytes()
{
    *this = value;
}

ck_variant::ck_variant( const char * value ) : _type(CK_VARTYPE_EMPTY),_variant(),_text(),_bytes()
{
    *this = ck_string(value);
}

ck_variant::ck_variant( const ck_string & value ) : _type(CK_VARTYPE_EMPTY),_variant(),_text(),_bytes()
{
    *this = value;
}

ck_variant::ck_variant( const ck_memory & value ) : _type(CK_VARTYPE_EMPTY),_variant(),_text(),_bytes()
{
    *this = value;
}

ck_variant::ck_variant( void * value ) : _type(CK_VARTYPE_EMPTY),_variant(),_text(),_bytes()
{
    *this = value;
}

ck_variant::ck_variant( const ck_variant& other ) : _type(CK_VARTYPE_EMPTY),_variant(),_text(),_bytes()
{
    *this = other;
}

ck_variant::ck_variant( ck_variant&& other ) noexcept :
    _type(std::move(other._type)),
    _variant(std::move(other._variant)),
    _text(std::move(other._text)),
    _bytes(std::move(other._bytes))
{
}

ck_variant::~ck_variant() noexcept
{
    clear();
}

CK_VARIANT_TYPE ck_variant::get_type() const
{
    return _type;
}

void ck_variant::clear()
{
    _type = CK_VARTYPE_EMPTY;
    memset(&_variant, 0, sizeof(_variant));
    _text.clear();
    _bytes.clear();
}

bool ck_variant::is_empty() const
{
    return (_type == CK_VARTYPE_EMPTY);
}

ck_variant& ck_variant::operator=( const ck_variant& other )
{
    _type = other._type;
    _variant = other._variant;
    _text = other._text;
    _bytes = other._bytes;

    return *this;
}

ck_variant& ck_variant::operator=( ck_variant&& other ) noexcept
{
    _type = std::move(other._type);
    _variant = std::move(other._variant);
    _text = std::move(other._text);
    _bytes = std::move(other._bytes);

    return *this;
}

ck_variant& ck_variant::operator=( bool value )
{
    clear();
    _type = CK_VARTYPE_BOOL;
    _variant.type = NUMTYPE_BOOL;
    _variant.value.b = value;
    return *this;
}

ck_variant& ck_variant::operator=( char value )
{
    clear();
    _type = CK_VARTYPE_CHAR;
    _variant.type = NUMTYPE_CHAR;
    _variant.value.c = value;
    return *this;
}

ck_variant& ck_variant::operator=( signed char value )
{
    clear();
    _type = CK_VARTYPE_SIGNED_CHAR;
    _variant.type = NUMTYPE_SIGNED_CHAR;
    _variant.value.sc = value;
    return *this;
}

ck_variant& ck_variant::operator=( unsigned char value )
{
    clear();
    _type = CK_VARTYPE_UNSIGNED_CHAR;
    _variant.type = NUMTYPE_UNSIGNED_CHAR;
    _variant.value.uc = value;
    return *this;
}

ck_variant& ck_variant::operator=( signed short value )
{
    clear();
    _type = CK_VARTYPE_SIGNED_SHORT;
    _variant.type = NUMTYPE_SIGNED_SHORT;
    _variant.value.ss = value;
    return *this;
}

ck_variant& ck_variant::operator=( unsigned short value )
{
    clear();
    _type = CK_VARTYPE_UNSIGNED_SHORT;
    _variant.type = NUMTYPE_UNSIGNED_SHORT;
    _variant.value.us = value;
    return *this;
}

ck_variant& ck_variant::operator=( signed int value )
{
    clear();
    _type = CK_VARTYPE_SIGNED_INT;
    _variant.type = NUMTYPE_SIGNED_INT;
    _variant.value.si = value;
    return *this;
}

ck_variant& ck_variant::operator=( unsigned int value )
{
    clear();
    _type = CK_VARTYPE_UNSIGNED_INT;
    _variant.type = NUMTYPE_UNSIGNED_INT;
    _variant.value.ui = value;
    return *this;
}

ck_variant& ck_variant::operator=( signed long value )
{
    clear();
    _type = CK_VARTYPE_SIGNED_LONG;
    _variant.type = NUMTYPE_SIGNED_LONG;
    _variant.value.sl = value;
    return *this;
}

ck_variant& ck_variant::operator=( unsigned long value )
{
    clear();
    _type = CK_VARTYPE_UNSIGNED_LONG;
    _variant.type = NUMTYPE_UNSIGNED_LONG;
    _variant.value.ul = value;
    return *this;
}

ck_variant& ck_variant::operator=( signed long long value )
{
    clear();
    _type = CK_VARTYPE_SIGNED_LONG_LONG;
    _variant.type = NUMTYPE_SIGNED_LONG_LONG;
    _variant.value.sll = value;
    return *this;
}

ck_variant& ck_variant::operator=( unsigned long long value )
{
    clear();
    _type = CK_VARTYPE_UNSIGNED_LONG_LONG;
    _variant.type = NUMTYPE_UNSIGNED_LONG_LONG;
    _variant.value.ull = value;
    return *this;
}

ck_variant& ck_variant::operator=( float value )
{
    clear();
    _type = CK_VARTYPE_FLOAT;
    _variant.type = NUMTYPE_FLOAT;
    _variant.value.f = value;
    return *this;
}

ck_variant& ck_variant::operator=( double value )
{
    clear();
    _type = CK_VARTYPE_DOUBLE;
    _variant.type = NUMTYPE_DOUBLE;
    _variant.value.d = value;
    return *this;
}

ck_variant& ck_variant::operator=( long double value )
{
    clear();
    _type = CK_VARTYPE_LONG_DOUBLE;
    _variant.type = NUMTYPE_LONG_DOUBLE;
    _variant.value.ld = value;
    return *this;
}

ck_variant& ck_variant::operator=( const ck_uuid_bits & value )
{
    clear();
    _type = CK_VARTYPE_UUID;
    _variant.type = NUMTYPE_UUID;
    ck_uuid_copy(&value, &_variant.value.uuid);
    return *this;
}

ck_variant& ck_variant::operator=( const ck_string & value )
{
    clear();
    _type = CK_VARTYPE_TEXT;
    _text = value;
    return *this;
}

ck_variant& ck_variant::operator=( const ck_memory & val )
{
    clear();
    _type = CK_VARTYPE_BYTES;
    _bytes = val;
    return *this;
}

ck_variant& ck_variant::operator=( void * value )
{
    clear();
    _type = CK_VARTYPE_PTR;
    _variant.type = NUMTYPE_PTR;
    _variant.value.ptr = value;
    return *this;
}

bool ck_variant::_get_as_bool() const
{
    return _convert<bool>();
}

char ck_variant::_get_as_char() const
{
    return _convert<char>();
}

signed char ck_variant::_get_as_signed_char() const
{
    return _convert<signed char>();
}

unsigned char ck_variant::_get_as_unsigned_char() const
{
    return _convert<unsigned char>();
}

signed short ck_variant::_get_as_signed_short() const
{
    return _convert<signed short>();
}

unsigned short ck_variant::_get_as_unsigned_short() const
{
    return _convert<unsigned short>();
}

signed int ck_variant::_get_as_signed_int() const
{
    return _convert<signed int>();
}

unsigned int ck_variant::_get_as_unsigned_int() const
{
    return _convert<unsigned int>();
}

signed long ck_variant::_get_as_signed_long() const
{
    return _convert<signed long>();
}

unsigned long ck_variant::_get_as_unsigned_long() const
{
    return _convert<unsigned long>();
}

signed long long ck_variant::_get_as_signed_long_long() const
{
    return _convert<signed long long>();
}

unsigned long long ck_variant::_get_as_unsigned_long_long() const
{
    return _convert<unsigned long long>();
}

float ck_variant::_get_as_float() const
{
    return _convert<float>();
}

double ck_variant::_get_as_double() const
{
    return _convert<double>();
}

long double ck_variant::_get_as_long_double() const
{
    return _convert<long double>();
}

ck_uuid_bits ck_variant::_get_as_uuid() const
{
    return _convert<ck_uuid_bits>();
}

ck_string ck_variant::_get_as_string() const
{
    return _convert<ck_string>();
}

ck_memory ck_variant::_get_as_bytes() const
{
    return _convert<ck_memory>();
}

void * ck_variant::_get_as_ptr() const
{
    if(_type != CK_VARTYPE_PTR)
        return NULL;
    return _variant.value.ptr;
}

template<class To>
To ck_variant::_convert() const
{
    switch(_type)
    {
    case CK_VARTYPE_BOOL:
    case CK_VARTYPE_CHAR:
    case CK_VARTYPE_SIGNED_CHAR:
    case CK_VARTYPE_UNSIGNED_CHAR:
    case CK_VARTYPE_SIGNED_SHORT:
    case CK_VARTYPE_UNSIGNED_SHORT:
    case CK_VARTYPE_SIGNED_INT:
    case CK_VARTYPE_UNSIGNED_INT:
    case CK_VARTYPE_SIGNED_LONG:
    case CK_VARTYPE_UNSIGNED_LONG:
    case CK_VARTYPE_SIGNED_LONG_LONG:
    case CK_VARTYPE_UNSIGNED_LONG_LONG:
    case CK_VARTYPE_FLOAT:
    case CK_VARTYPE_DOUBLE:
    case CK_VARTYPE_LONG_DOUBLE:
    case CK_VARTYPE_UUID:
        return _conversion<To,ck_variant::CK_VARIANT>::convert(_variant);
    case CK_VARTYPE_BYTES:
        return _conversion<To,ck_memory>::convert(_bytes);
    case CK_VARTYPE_TEXT:
        return _conversion<To,ck_string>::convert(_text);
    default:
        CK_THROW(("Could not convert %d\n",_type));
    }
}

bool ck_variant::operator==( const ck_variant& other ) const
{
    if(_type == other._type)
    {
        switch(_type)
        {
        case CK_VARTYPE_BOOL:
            return _variant.value.b == other._variant.value.b;
        case CK_VARTYPE_CHAR:
            return _variant.value.c == other._variant.value.c;
        case CK_VARTYPE_SIGNED_CHAR:
            return _variant.value.sc == other._variant.value.sc;
        case CK_VARTYPE_UNSIGNED_CHAR:
            return _variant.value.uc == other._variant.value.uc;
        case CK_VARTYPE_SIGNED_SHORT:
            return _variant.value.ss == other._variant.value.ss;
        case CK_VARTYPE_UNSIGNED_SHORT:
            return _variant.value.us == other._variant.value.us;
        case CK_VARTYPE_SIGNED_INT:
            return _variant.value.si == other._variant.value.si;
        case CK_VARTYPE_UNSIGNED_INT:
            return _variant.value.ui == other._variant.value.ui;
        case CK_VARTYPE_SIGNED_LONG:
            return _variant.value.sl == other._variant.value.sl;
        case CK_VARTYPE_UNSIGNED_LONG:
            return _variant.value.ul == other._variant.value.ul;
        case CK_VARTYPE_SIGNED_LONG_LONG:
            return _variant.value.sll == other._variant.value.sll;
        case CK_VARTYPE_UNSIGNED_LONG_LONG:
            return _variant.value.ull == other._variant.value.ull;
        case CK_VARTYPE_FLOAT:
            return _variant.value.f == other._variant.value.f;
        case CK_VARTYPE_DOUBLE:
            return _variant.value.d == other._variant.value.d;
        case CK_VARTYPE_LONG_DOUBLE:
            return _variant.value.ld == other._variant.value.ld;
        case CK_VARTYPE_UUID:
            return memcmp(&_variant.value.uuid,&other._variant.value.uuid,sizeof(ck_uuid_bits)) == 0;
        case CK_VARTYPE_BYTES:
        {
            if(_bytes.size_data() != other._bytes.size_data())
                return false;
            return memcmp(_bytes.map().get_ptr(),other._bytes.map().get_ptr(),_bytes.size_data()) == 0;
        }
        case CK_VARTYPE_TEXT:
            return _text == other._text;
        default:
            CK_THROW(("Cannot compare these two ck_variant objects\n"));
        }
    }
    else
        return false;
}

bool ck_variant::operator!=( const ck_variant& other ) const
{
    return !(operator==(other));
}
