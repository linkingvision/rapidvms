
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "XSDK/MemDebugBegin.h"
#include "XSDK/Errors.h"
#include "XSDK/Module.h"
#include "XSDK/XUuid.h"
#include "XSDK/MemDebugEnd.h"

#ifdef WIN32
#include "Objbase.h"
#else
#ifndef ARM_LINUX
#include <uuid/uuid.h>
#endif
#endif


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

//=============================================================================
// Initializes the object with an empty UUID.
//=============================================================================
XUuid::XUuid()
{
    Clear();
}

//=============================================================================
// Initializes the object with a UUID value.
//=============================================================================
XUuid::XUuid( IN const X_UUID& uuid )
{
    *this = uuid;
}

//=============================================================================
// Initializes the object with a UUID represented in string format.
//=============================================================================
XUuid::XUuid( IN const XString& str )
{
    // FromString() can throw
    FromString(str);
}

//=============================================================================
// Copy constructor
//=============================================================================
XUuid::XUuid( IN const XUuid& other )
{
    *this = other;
}

//=============================================================================
// Destructor
//=============================================================================
XUuid::~XUuid() throw()
{
    Clear();
}

//=============================================================================
// Gets the UUID value represented by this class.
//=============================================================================
const X_UUID& XUuid::GetUuid() const
{
    return _uuid;
}

//=============================================================================
// Sets the UUID to an empty value.
//=============================================================================
void XUuid::Clear()
{
    XUuidClear(&_uuid);
}

//=============================================================================
// Returns a boolean indicating if the UUID value is empty.
//=============================================================================
bool XUuid::IsEmpty() const
{
    return (true == XUuidIsEmpty(&_uuid));
}

//=============================================================================
// Tests a string to determine if it represents a valid UUID.
//=============================================================================
bool XUuid::IsStringValid( IN const XSDK::XString& str )
{
    return (true == XUuidIsStringValid(str.c_str())); // c_str() does not throw
}

//=============================================================================
// Gets a string representation of the UUID.
//=============================================================================
XSDK::XString XUuid::ToString() const
{
    char uuidStr[X_UUID_STRING_LENGTH + 1];

    // Shouldn't need to check return value since this method only returns
    // false if the buffer is not large enough to hold the string.
    X_VERIFY(true == XUuidToString(&_uuid, uuidStr, -1));

    // Verify that we have a NULL terminated UUID string
    X_ASSERT(uuidStr[X_UUID_STRING_LENGTH] == 0);
    return (XSDK::XString(uuidStr));
}

//=============================================================================
// Set the UUID value from a string.
//=============================================================================
void XUuid::FromString( IN const XSDK::XString& str )
{
    // This method only returns false if the string format is not valid.
    if (XUuidFromString(str.c_str(), &_uuid) == false)
        X_THROW_NO_LOG(("Unable to convert from string."));
}

//=============================================================================
// Sets the UUID of this object equal to the specified value.
//=============================================================================
XUuid& XUuid::operator = ( IN const XUuid& other )
{
    return (*this = other._uuid);
}

//=============================================================================
// Sets the UUID of this object equal to the specified value.
//=============================================================================
XUuid& XUuid::operator=( IN const X_UUID& other )
{
    XUuidCopy(&other, &_uuid);
    return (*this);
}

//=============================================================================
// Sets the UUID of this object equal to the specified value.
//=============================================================================
XUuid& XUuid::operator=( IN const XString& other )
{
    return *this = XUuid(other);
}

//=============================================================================
// Tests to see if this object is equal to the specified UUID.
//=============================================================================
bool XUuid::operator==( IN const XUuid& other ) const
{
    return (*this == other._uuid);
}

//=============================================================================
// Tests to see if this object is equal to the specified UUID.
//=============================================================================
bool XUuid::operator==( IN const X_UUID& other ) const
{
    return (true == XUuidIsEqual(&this->_uuid, &other));
}

//=============================================================================
// Tests to see if this object is <b>NOT</b> equal to the specified UUID.
//=============================================================================
bool XUuid::operator!=( IN const XUuid& other ) const
{
    return !(*this == other._uuid);
}

//=============================================================================
// Tests to see if this object is <b>NOT</b> equal to the specified UUID.
//=============================================================================
bool XUuid::operator!=( IN const X_UUID& other ) const
{
    return !(*this == other);
}

//=============================================================================
// Casts the object back to a X_UUID.
//=============================================================================
XUuid::operator const X_UUID&() const
{
    return _uuid;
}

XSDK::XString XUuid::Generate()
{
#ifdef WIN32
  RPC_WSTR str = NULL;
  GUID pguid;
  if ( CoCreateGuid(&pguid) != S_OK )
          return "";
  if ( UuidToStringW(&pguid, &str) != RPC_S_OK )
          return "";
  XSDK::XString res((wchar_t*)str);
  RpcStringFreeW(&str);
  return res;
#else
  char str[37];
#ifndef ARM_LINUX
  uuid_t id;
  uuid_generate_random(id);
  uuid_unparse(id,str);
#endif
  return str;
#endif
}


//==============================================================================
// Restore the compiler warning settings back the way they were.
//==============================================================================
#ifdef IS_WINDOWS
    #pragma warning(pop)
#endif

