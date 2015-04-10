
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef _XSDK_INTERFACE_H_
#define _XSDK_INTERFACE_H_

#include "XSDK/Types.h"
#include "XSDK/UuidUtils.h"

//=============================================================================
/// @page DEFINE_INTERFACE_ID
///
/// This macro is used to define a UUID for an interface. The primary
/// purpose of the UUID is to provide an identifier that can be used to
/// request interfaces of objects through the IXUnknown::QueryInterface()
/// method. To use the macro, add it to the top of your interface declaration.
/// The macro also declares the virtual destructor for the interface and will
/// leave the access level set to "public:" for all declarations following
/// the macro (or at least until set otherwise). The macro will define a
/// public static function named Uuid() that can be used to refer to the
/// interface's UUID value as needed.
///
/// Example:
///
/// @code
/// class IFoo
/// {
///     DEFINE_INTERFACE_ID(IFoo, 0x93,0xcb,0x99,0x93,0x67,0x4e,
///         0x47,0x36,0x8f,0x44,0x46,0x71,0x4f,0x4b,0x89,0xfd)
///     virtual void Method1() = 0;
///     virtual void Method2() = 0;
///     ...
/// };
/// @endcode
///
///
/// @param    INTERFACE_NAME This is expected to be the name of the interface
///           the macro is being used for.
///
/// @param    D1 The first byte for the UUID.
///
/// @param    D2 The second byte for the UUID.
///
/// @param    D3 The third byte for the UUID.
///
/// @param    D4 The forth byte for the UUID.
///
/// @param    D5 The fifth byte for the UUID.
///
/// @param    D6 The sixth byte for the UUID.
///
/// @param    D7 The seventh byte for the UUID.
///
/// @param    D8 The eighth byte for the UUID.
///
/// @param    D9 The ninth byte for the UUID.
///
/// @param    D10 The tenth byte for the UUID.
///
/// @param    D11 The eleventh byte for the UUID.
///
/// @param    D12 The twelfth byte for the UUID.
///
/// @param    D13 The thirteenth byte for the UUID.
///
/// @param    D14 The fourteenth byte for the UUID.
///
/// @param    D15 The fifteenth byte for the UUID.
///
/// @param    D16 The sixteenth byte for the UUID.
//=============================================================================
#define DEFINE_INTERFACE_ID(INTERFACE_NAME,D1,D2,D3,                        \
    D4,D5,D6,D7,D8,D9,D10,D11,D12,D13,D14,D15,D16)                          \
                                                                            \
public:                                                                     \
    /* Declare a virtual destructor to avoid compiler warnings */           \
    virtual ~INTERFACE_NAME() {}                                            \
                                                                            \
    /* FYI: We ran into problems if we tried to return a reference      */  \
    /* to a static UUID from the Uuid() routine. The routine would      */  \
    /* work correctly for most types, but every once in a while a       */  \
    /* type would return an empty UUID (consistently). Best we could    */  \
    /* tell, it seemed the problem was due to the fact that the method  */  \
    /* was declared inline and we were using a static X_UUID variable  */   \
    /* to hold the UUID value. In this case, the first binary that      */  \
    /* called the routine (because of inline, there would be a          */  \
    /* different version of the routine in each binary) would have      */  \
    /* the UUID set correctly. In any binary that tried to call the     */  \
    /* routine after the first, an empty UUID would be returned. This   */  \
    /* seemed to be a Linux only problem where the compiler did not     */  \
    /* realize that the static UUID value had to be initialized in      */  \
    /* each binary the inline routine appeared. After discovering       */  \
    /* this problem, we decided to always create a temporary X_UUID    */  \
    /* variable and return it by value (this also avoided issues that   */  \
    /* might have occurred with multithreaded applications).            */  \
                                                                            \
    /* Report the UUID associated with the interface - this is made     */  \
    /* available so the UUID can be discovered by simply using the      */  \
    /* name of the interface and calling this method.                   */  \
    static const X_UUID Uuid()                                              \
    {                                                                       \
        X_UUID uuid = {{D1,D2,D3,D4,D5,D6,D7,D8,                           \
            D9,D10,D11,D12,D13,D14,D15,D16}};                               \
        return (uuid);                                                      \
    }                                                                       \


#endif // _XSDK_INTERFACE_H_

