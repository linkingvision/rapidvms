
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef _XSDK_OBJECT_H_
#define _XSDK_OBJECT_H_

#include "XSDK/Types.h"
#include "XSDK/UuidUtils.h"


//=============================================================================
/// @page DEFINE_CLASS_ID
///
/// This macro is used to define a UUID for a class. This UUID may be used
/// to identify the class in a type library, for instance. To use the macro,
/// add it to the public section of your class definition. The macro defines
/// a static method named ClassId() that can be used to obtain the class'
/// UUID value when only the class name is used. For example:
///
/// @code
/// //=========================================================================
/// // Declaration of a class called MyClass(Abs).
/// //=========================================================================
/// class MyClassAbs :
///     public IFoo
/// {
///     DEFINE_CLASS_ID(0x71,0xcc,0xa6,0x47,0xdd,0x50,0x48,
///         0x2f,0x91,0xa0,0x81,0x6a,0x4e,0xa0,0x3e,0x89);
///     ...
/// };
///
/// //=========================================================================
/// // Define the concrete version of MyClass
/// //=========================================================================
/// typedef IXUnkownImpl<MyClassAbs> MyClass;
///
/// //=========================================================================
/// // Method that uses the class ID of MyClass.
/// //=========================================================================
/// void Method()
/// {
///     // Create a MyClass object.
///     MyClass* pC = IFactory->CreateObject(MyClass::ClassId());
///     ...
/// }
/// @endcode
///
/// It is important to note that this method cannot be used if the class
/// name is not known. To dynamically determine the type of an unknown class
/// instance, call the IXUnknown::GetClassId() method of the object. It
/// should also be noted that this macro is written with the intent of
/// storing the given UUID data in network byte order.
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
#define DEFINE_CLASS_ID(D1,D2,D3,D4,D5,D6,D7,                                 \
    D8,D9,D10,D11,D12,D13,D14,D15,D16)                                        \
                                                                              \
    static const X_UUID ClassId()                                             \
    {                                                                         \
        X_UUID uuid = {{D1,D2,D3,D4,D5,D6,D7,                             \
               D8,D9,D10,D11,D12,D13,D14,D15,D16}};                           \
        return (uuid);                                                        \
    }                                                                         \


//=============================================================================
/// @page SET_CLASS_ID
///
/// This macro is used to set a UUID for a class based on a pre-existing
/// X_UUID definition. This UUID may be used to identify the class in a
/// type library, for instance. To use the macro, add it to the public
/// section of your class definition. The macro defines a static method
/// named ClassId() that can be used to obtain the class' UUID value when
/// only the class name is used. For example:
///
/// @code
/// //=========================================================================
/// // Define my class identifier value.
/// //=========================================================================
/// const X_UUID kMyClassUuid = {0x71,0xcc,0xa6,0x47,0xdd,0x50,
///     x48, 0x2f,0x91,0xa0,0x81,0x6a,0x4e,0xa0,0x3e,0x89};
///
/// //=========================================================================
/// // Declaration of a class called MyClass(Abs).
/// //=========================================================================
/// class MyClassAbs :
///     public IFoo
/// {
///     SET_CLASS_ID(kMyClassUuid);
///     ...
/// };
///
/// //=========================================================================
/// // Define the concrete version of MyClass
/// //=========================================================================
/// typedef IXUnkownImpl<MyClassAbs> MyClass;
///
/// //=========================================================================
/// // Method that uses the class ID of MyClass.
/// //=========================================================================
/// void Method()
/// {
///     // Create a MyClass object.
///     MyClass* pC = IFactory->CreateObject(MyClass::ClassId());
///     ...
/// }
/// @endcode
///
/// It is important to note that this method cannot be used if the class
/// name is not known. To dynamically determine the type of an unknown class
/// instance, call the IXUnknown::GetClassId() method on the object.
///
/// @param    UUID This is expected to be a pre-existing X_UUID value. This
///         value is expected to remain constant throughout the execution of
///         your program.
//=============================================================================
#define SET_CLASS_ID(UUID)                                                    \
                                                                              \
    static const X_UUID ClassId()                                             \
    {                                                                         \
        return (UUID);                                                        \
    }                                                                         \


#endif // _XSDK_OBJECT_H_

