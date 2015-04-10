
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "XSDK/XFullVariant.h"
#include "XSDK/Errors.h"

#include <cstdlib>

using namespace XSDK;

XFullVariant::XFullVariant() :
    _type( EMPTY ),
    _numerics(),
    _bytes( NULL ),
    _bytesSize( 0 ),
    _text()
{
    memset( &_numerics, 0, sizeof( union NumericBox ) );
}

XFullVariant::XFullVariant( const XFullVariant& obj ) :
    _type( EMPTY ),
    _numerics(),
    _bytes( NULL ),
    _bytesSize( 0 ),
    _text()
{
    memset( &_numerics, 0, sizeof( union NumericBox ) );
    _Copy( obj );
}

XFullVariant::~XFullVariant() throw()
{
    if( _bytes )
        free( _bytes );
}

XFullVariant& XFullVariant::operator = ( const XFullVariant& obj )
{
    return _Copy( obj );
}

XFullVariant::VariantType XFullVariant::GetType() const
{
    return _type;
}

void XFullVariant::SetSignedInteger( int val )
{
    Clear();
    _type = SIGNED_INTEGER;
    _numerics.s32 = val;
}

void XFullVariant::SetUnsignedInteger( unsigned int val )
{
    Clear();
    _type = UNSIGNED_INTEGER;
    _numerics.u32 = val;
}

void XFullVariant::SetSigned64Integer( int64_t val )
{
    Clear();
    _type = SIGNED_64_INTEGER;
    _numerics.s64 = val;
}

void XFullVariant::SetUnsigned64Integer( uint64_t val )
{
    Clear();
    _type = UNSIGNED_64_INTEGER;
    _numerics.u64 = val;
}

void XFullVariant::SetFloatingPoint( double val )
{
    Clear();
    _type = FLOATING_POINT;
    _numerics.d64 = val;
}

void XFullVariant::SetBytes( const unsigned char* data, size_t size )
{
    Clear();

    _bytes = (unsigned char*)malloc( size );

    if( !_bytes )
        X_THROW(( "malloc() failed." ));

    _type = BYTES;
    memcpy( _bytes, data, size );
    _bytesSize = size;
}

void XFullVariant::SetString( const XString& val )
{
    Clear();

    _type = TEXT;
    _text = val;
}

int XFullVariant::GetSignedInteger() const
{
    if( _type == SIGNED_INTEGER )
        return _numerics.s32;
    else X_THROW(( "Invalid type." ));
}

unsigned int XFullVariant::GetUnsignedInteger() const
{
    if( _type == UNSIGNED_INTEGER )
        return _numerics.u32;
    else X_THROW(( "Invalid type." ));
}

int64_t XFullVariant::GetSigned64Integer() const
{
    if( _type == SIGNED_64_INTEGER )
        return _numerics.s64;
    else X_THROW(( "Invalid type." ));
}

uint64_t XFullVariant::GetUnsigned64Integer() const
{
    if( _type == UNSIGNED_64_INTEGER )
        return _numerics.u64;
    else X_THROW(( "Invalid type." ));
}

double XFullVariant::GetFloatingPoint() const
{
    if( _type == FLOATING_POINT )
        return _numerics.d64;
    else X_THROW(( "Invalid type." ));
}

size_t XFullVariant::GetBytes( unsigned char** data) const
{
    if( _type == BYTES )
    {
        *data = _bytes;
        return _bytesSize;
    }
    else X_THROW(( "Invalid type." ));
}

const XString& XFullVariant::GetString() const
{
    if( _type == TEXT )
        return _text;
    else X_THROW(( "Invalid type." ));
}

void XFullVariant::Clear()
{
    _type = EMPTY;
    memset( &_numerics, 0, sizeof( union NumericBox ) );
    if( _bytes )
        free( _bytes );
    _bytes = NULL;
    _bytesSize = 0;
    _text = "";
}

XFullVariant& XFullVariant::_Copy( const XFullVariant& obj )
{
    Clear();

    _type = obj._type;

    memcpy( &_numerics, &obj._numerics, sizeof( union NumericBox ) );

    if( obj._bytes )
    {
        _bytes = (unsigned char*)malloc( obj._bytesSize );

        if( !_bytes )
            X_THROW(( "malloc() failed." ));

        _bytesSize = obj._bytesSize;
        memcpy( _bytes, obj._bytes, _bytesSize );
    }

    _text = obj._text;

    return *this;
}
