
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef XSDK_XFullVariant_h
#define XSDK_XFullVariant_h

#include "XSDK/XException.h"
#include "XSDK/XString.h"

namespace XSDK
{

// Variant is meant to provide a flexible storage mechanism for many
// different types of data. The immediate impetus for this class is a means
// of returning non text results from a database (for example, frames of
// video).
class XFullVariant
{
  union NumericBox
  {
    int s32;
    unsigned int u32;
    int64_t s64;
    uint64_t u64;
    double d64;
  };

public:

  enum VariantType
  {
    EMPTY,
    SIGNED_INTEGER,
    UNSIGNED_INTEGER,
    SIGNED_64_INTEGER,
    UNSIGNED_64_INTEGER,
    FLOATING_POINT,
    BYTES,
    TEXT
  };

  X_API XFullVariant();

  X_API XFullVariant( const XFullVariant& obj );

  X_API virtual ~XFullVariant() throw();

  X_API XFullVariant& operator = ( const XFullVariant& obj );

  X_API VariantType GetType() const;

  X_API void SetSignedInteger( int val );

  X_API void SetUnsignedInteger( unsigned int val );

  X_API void SetSigned64Integer( int64_t val );

  X_API void SetUnsigned64Integer( uint64_t val );

  X_API void SetFloatingPoint( double val );

  X_API void SetBytes( const unsigned char* data, size_t size );

  X_API void SetString(const XSDK::XString& val);

  X_API int GetSignedInteger() const;

  X_API unsigned int GetUnsignedInteger() const;

  X_API int64_t GetSigned64Integer() const;

  X_API uint64_t GetUnsigned64Integer() const;

  X_API double GetFloatingPoint() const;

  X_API size_t GetBytes(unsigned char** data) const;

  X_API const XSDK::XString& GetString() const;

  X_API void Clear();

private:

  XFullVariant& _Copy( const XFullVariant& obj );

  VariantType _type;

  // Storage
  // The actual value is stored in ONE of the following containers.
  union NumericBox _numerics;
  unsigned char* _bytes;
  size_t _bytesSize;
  XSDK::XString _text;

};

}

#endif
