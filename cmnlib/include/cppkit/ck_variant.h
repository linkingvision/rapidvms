
#ifndef cppkit_variant_h
#define cppkit_variant_h

#include "cppkit/ck_types.h"
#include "cppkit/os/ck_exports.h"
#include "cppkit/ck_exception.h"
#include "cppkit/ck_string.h"
#include "cppkit/ck_memory.h"

namespace cppkit
{

class ck_variant
{
    enum NUMERIC_TYPE
    {
        NUMTYPE_EMPTY = 0,
        NUMTYPE_BOOL,
        NUMTYPE_CHAR,
        NUMTYPE_SIGNED_CHAR,
        NUMTYPE_UNSIGNED_CHAR,
        NUMTYPE_SIGNED_SHORT,
        NUMTYPE_UNSIGNED_SHORT,
        NUMTYPE_SIGNED_INT,
        NUMTYPE_UNSIGNED_INT,
        NUMTYPE_SIGNED_LONG,
        NUMTYPE_UNSIGNED_LONG,
        NUMTYPE_SIGNED_LONG_LONG,
        NUMTYPE_UNSIGNED_LONG_LONG,
        NUMTYPE_FLOAT,
        NUMTYPE_DOUBLE,
        NUMTYPE_LONG_DOUBLE,
        NUMTYPE_UUID,
        NUMTYPE_PTR
    };

    struct CK_VARIANT
    {
        NUMERIC_TYPE type;
        union
        {
            bool                 b;
            char                 c;
            signed char         sc;
            unsigned char       uc;
            signed short        ss;
            unsigned short      us;
            signed int          si;
            unsigned int        ui;
            signed long         sl;
            unsigned long       ul;
            signed long long   sll;
            unsigned long long ull;
            float                f;
            double               d;
            long double         ld;
            ck_uuid_bits      uuid;
            void*              ptr;
        } value;
    };

public:

    CK_API ck_variant();
    CK_API ck_variant( const ck_variant& other );
    CK_API ck_variant( ck_variant&& other ) noexcept;

    // The constructors are explicit to avoid ambiguity between
    // methods that take ck_variant and method overloads that take a type
    // supported by ck_variant.
    //
    // For example: foo(int) and foo(ck_variant) in the same code module is
    // allowed b/c the constructors are explicit. Otherwise, the compiler
    // doesn't know which version of foo() to call when passing an int.
    CK_API explicit ck_variant( bool value );
    CK_API explicit ck_variant( char value );
    CK_API explicit ck_variant( signed char value );
    CK_API explicit ck_variant( unsigned char value );
    CK_API explicit ck_variant( signed short value );
    CK_API explicit ck_variant( unsigned short value );
    CK_API explicit ck_variant( signed int value );
    CK_API explicit ck_variant( unsigned int value );
    CK_API explicit ck_variant( signed long value );
    CK_API explicit ck_variant( unsigned long value );
    CK_API explicit ck_variant( signed long long value );
    CK_API explicit ck_variant( unsigned long long value );
    CK_API explicit ck_variant( float value );
    CK_API explicit ck_variant( double value );
    CK_API explicit ck_variant( long double value );
    CK_API explicit ck_variant( const ck_uuid_bits & value );
    CK_API explicit ck_variant( const char * value );
    CK_API explicit ck_variant( const ck_string& value );
    CK_API explicit ck_variant( const ck_memory& value );
    CK_API explicit ck_variant( void * value );
    CK_API ~ck_variant() noexcept;

    CK_API CK_VARIANT_TYPE get_type() const;
    CK_API void clear();
    CK_API bool is_empty() const;

    CK_API ck_variant& operator = ( const ck_variant& other );
    CK_API ck_variant& operator = ( ck_variant&& other ) noexcept;
    CK_API ck_variant& operator = ( bool value );
    CK_API ck_variant& operator = ( char value );
    CK_API ck_variant& operator = ( signed char value );
    CK_API ck_variant& operator = ( unsigned char value );
    CK_API ck_variant& operator = ( signed short value );
    CK_API ck_variant& operator = ( unsigned short value );
    CK_API ck_variant& operator = ( signed int value );
    CK_API ck_variant& operator = ( unsigned int value );
    CK_API ck_variant& operator = ( signed long value );
    CK_API ck_variant& operator = ( unsigned long value );
    CK_API ck_variant& operator = ( signed long long value );
    CK_API ck_variant& operator = ( unsigned long long value );
    CK_API ck_variant& operator = ( float value );
    CK_API ck_variant& operator = ( double value );
    CK_API ck_variant& operator = ( long double value );
    CK_API ck_variant& operator = ( const ck_uuid_bits & value );
    CK_API ck_variant& operator = ( const ck_string& value );
    CK_API ck_variant& operator = ( const ck_memory& value );
    CK_API ck_variant& operator = ( void * value );

    template<class T>
    CK_API T get() const;

    template<class To>
    CK_API operator To() const
    {
        return get<To>();
    }

    CK_API bool operator==( const ck_variant& other ) const;
    CK_API bool operator!=( const ck_variant& other ) const;

private:

    CK_API bool _get_as_bool() const;
    CK_API char _get_as_char() const;
    CK_API signed char _get_as_signed_char() const;
    CK_API unsigned char _get_as_unsigned_char() const;
    CK_API signed short _get_as_signed_short() const;
    CK_API unsigned short _get_as_unsigned_short() const;
    CK_API signed int _get_as_signed_int() const;
    CK_API unsigned int _get_as_unsigned_int() const;
    CK_API signed long _get_as_signed_long() const;
    CK_API unsigned long _get_as_unsigned_long() const;
    CK_API signed long long _get_as_signed_long_long() const;
    CK_API unsigned long long _get_as_unsigned_long_long() const;
    CK_API float _get_as_float() const;
    CK_API double _get_as_double() const;
    CK_API long double _get_as_long_double() const;
    CK_API ck_uuid_bits _get_as_uuid() const;
    CK_API ck_string _get_as_string() const;
    CK_API ck_memory _get_as_bytes() const;
    CK_API void * _get_as_ptr() const;

    template<class To,class From>
    struct _conversion;

    template<class To,class From>
    friend struct _conversion;

    template<class ToType>
    ToType _convert() const;

private:

    CK_VARIANT_TYPE _type;
    CK_VARIANT _variant;
    ck_string _text;
    ck_memory _bytes;
};

template<>
CK_API inline bool ck_variant::get() const
{
    return _get_as_bool();
}

template<>
CK_API inline char ck_variant::get() const
{
    return _get_as_char();
}

template<>
CK_API inline signed char ck_variant::get() const
{
    return _get_as_signed_char();
}

template<>
CK_API inline unsigned char ck_variant::get() const
{
    return _get_as_unsigned_char();
}

template<>
CK_API inline signed short ck_variant::get() const
{
    return _get_as_signed_short();
}

template<>
CK_API inline unsigned short ck_variant::get() const
{
    return _get_as_unsigned_short();
}

template<>
CK_API inline signed int ck_variant::get() const
{
    return _get_as_signed_int();
}

template<>
CK_API inline unsigned int ck_variant::get() const
{
    return _get_as_unsigned_int();
}

template<>
CK_API inline signed long ck_variant::get() const
{
    return _get_as_signed_long();
}

template<>
CK_API inline unsigned long ck_variant::get() const
{
    return _get_as_unsigned_long();
}

template<>
CK_API inline signed long long ck_variant::get() const
{
    return _get_as_signed_long_long();
}

template<>
CK_API inline unsigned long long ck_variant::get() const
{
    return _get_as_unsigned_long_long();
}

template<>
CK_API inline float ck_variant::get() const
{
    return _get_as_float();
}

template<>
CK_API inline double ck_variant::get() const
{
    return _get_as_double();
}

template<>
CK_API inline long double ck_variant::get() const
{
    return _get_as_long_double();
}

template<>
CK_API inline ck_uuid_bits ck_variant::get() const
{
    return _get_as_uuid();
}

template<>
CK_API inline ck_string ck_variant::get() const
{
    return _get_as_string();
}

template<>
CK_API inline ck_memory ck_variant::get() const
{
    return _get_as_bytes();
}

template<>
CK_API inline void * ck_variant::get() const
{
    return _get_as_ptr();
}

}

#endif

