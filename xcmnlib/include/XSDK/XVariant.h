
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef _XSDK_X_VARIANT_H_
#define _XSDK_X_VARIANT_H_

#include "XSDK/XSharedLibImp.h"
#include "XSDK/Types.h"
#include "XSDK/XSharedLib.h"
#include "XSDK/XException.h"
#include "XSDK/XSharedLibDef.h"
#include "XSDK/XString.h"
#include "XSDK/XMemory.h"

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
    class XVariant
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

        struct X_VARIANT
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
                X_UUID            uuid;
                void*              ptr;
            } value;
        };

    public:

        X_API XVariant();
        X_API XVariant( const XVariant& other );

        // tdistler: The constructors are explicit to avoid ambiguity between
        // methods that take XVariant and method overloads that take a type
        // supported by XVariant.
        //
        // For example: foo(int) and foo(XVariant) in the same code module is
        // allowed b/c the constructors are explicit. Otherwise, the compiler
        // doesn't know which version of foo() to call when passing an int.
        X_API explicit XVariant( bool value );
        X_API explicit XVariant( char value );
        X_API explicit XVariant( signed char value );
        X_API explicit XVariant( unsigned char value );
        X_API explicit XVariant( signed short value );
        X_API explicit XVariant( unsigned short value );
        X_API explicit XVariant( signed int value );
        X_API explicit XVariant( unsigned int value );
        X_API explicit XVariant( signed long value );
        X_API explicit XVariant( unsigned long value );
        X_API explicit XVariant( signed long long value );
        X_API explicit XVariant( unsigned long long value );
        X_API explicit XVariant( float value );
        X_API explicit XVariant( double value );
        X_API explicit XVariant( long double value );
        X_API explicit XVariant( const X_UUID & value );
        X_API explicit XVariant( const char * value );
        X_API explicit XVariant( const XSDK::XString& value );
        X_API explicit XVariant( const XSDK::XMemory& value );
        X_API explicit XVariant( void * value );
        X_API ~XVariant() throw();

        X_API XSDK::X_VARIANT_TYPE GetType() const;
        X_API void Clear();
        X_API bool IsEmpty() const;

        X_API XVariant& operator=( const XVariant& other );
        X_API XVariant& operator=( bool value );
        X_API XVariant& operator=( char value );
        X_API XVariant& operator=( signed char value );
        X_API XVariant& operator=( unsigned char value );
        X_API XVariant& operator=( signed short value );
        X_API XVariant& operator=( unsigned short value );
        X_API XVariant& operator=( signed int value );
        X_API XVariant& operator=( unsigned int value );
        X_API XVariant& operator=( signed long value );
        X_API XVariant& operator=( unsigned long value );
        X_API XVariant& operator=( signed long long value );
        X_API XVariant& operator=( unsigned long long value );
        X_API XVariant& operator=( float value );
        X_API XVariant& operator=( double value );
        X_API XVariant& operator=( long double value );
        X_API XVariant& operator=( const X_UUID & value );
        X_API XVariant& operator=( const XSDK::XString& value );
        X_API XVariant& operator=( const XSDK::XMemory& value );
        X_API XVariant& operator=( void * value );

        template<class T>
        X_API T Get() const;

        template<class To>
        X_API operator To() const
        {
            return Get<To>();
        }

        X_API bool operator==( const XVariant& other ) const;
        X_API bool operator!=( const XVariant& other ) const;

    private:

        X_API bool _GetAsBool() const;
        X_API char _GetAsChar() const;
        X_API signed char _GetAsSignedChar() const;
        X_API unsigned char _GetAsUnsignedChar() const;
        X_API signed short _GetAsSignedShort() const;
        X_API unsigned short _GetAsUnsignedShort() const;
        X_API signed int _GetAsSignedInt() const;
        X_API unsigned int _GetAsUnsignedInt() const;
        X_API signed long _GetAsSignedLong() const;
        X_API unsigned long _GetAsUnsignedLong() const;
        X_API signed long long _GetAsSignedLongLong() const;
        X_API unsigned long long _GetAsUnsignedLongLong() const;
        X_API float _GetAsFloat() const;
        X_API double _GetAsDouble() const;
        X_API long double _GetAsLongDouble() const;
        X_API X_UUID _GetAsUuid() const;
        X_API XSDK::XString _GetAsString() const;
        X_API XSDK::XMemory _GetAsBytes() const;
        X_API void * _GetAsPtr() const;

        template<class To,class From>
        struct _Conversion;

        template<class To,class From>
        friend struct _Conversion;

        template<class ToType>
        ToType _Convert() const;

    private:

        X_VARIANT_TYPE _type;
        X_VARIANT _variant;
        XSDK::XString _text;
        XSDK::XMemory _bytes;

    };

    template<>
    X_API inline bool XVariant::Get() const
    {
        return _GetAsBool();
    }

    template<>
    X_API inline char XVariant::Get() const
    {
        return _GetAsChar();
    }

    template<>
    X_API inline signed char XVariant::Get() const
    {
        return _GetAsSignedChar();
    }

    template<>
    X_API inline unsigned char XVariant::Get() const
    {
        return _GetAsUnsignedChar();
    }

    template<>
    X_API inline signed short XVariant::Get() const
    {
        return _GetAsSignedShort();
    }

    template<>
    X_API inline unsigned short XVariant::Get() const
    {
        return _GetAsUnsignedShort();
    }

    template<>
    X_API inline signed int XVariant::Get() const
    {
        return _GetAsSignedInt();
    }

    template<>
    X_API inline unsigned int XVariant::Get() const
    {
        return _GetAsUnsignedInt();
    }

    template<>
    X_API inline signed long XVariant::Get() const
    {
        return _GetAsSignedLong();
    }

    template<>
    X_API inline unsigned long XVariant::Get() const
    {
        return _GetAsUnsignedLong();
    }

    template<>
    X_API inline signed long long XVariant::Get() const
    {
        return _GetAsSignedLongLong();
    }

    template<>
    X_API inline unsigned long long XVariant::Get() const
    {
        return _GetAsUnsignedLongLong();
    }

    template<>
    X_API inline float XVariant::Get() const
    {
        return _GetAsFloat();
    }

    template<>
    X_API inline double XVariant::Get() const
    {
        return _GetAsDouble();
    }

    template<>
    X_API inline long double XVariant::Get() const
    {
        return _GetAsLongDouble();
    }

    template<>
    X_API inline X_UUID XVariant::Get() const
    {
        return _GetAsUuid();
    }

    template<>
    X_API inline XSDK::XString XVariant::Get() const
    {
        return _GetAsString();
    }

    template<>
    X_API inline XSDK::XMemory XVariant::Get() const
    {
        return _GetAsBytes();
    }

    template<>
    X_API inline void * XVariant::Get() const
    {
        return _GetAsPtr();
    }
}

//==============================================================================
// Restore the compiler warning settings back the way they were.
//==============================================================================
#ifdef IS_WINDOWS
    #pragma warning(pop)
#endif

#endif // _XSDK_X_VARIANT_H_

