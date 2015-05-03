
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef _XSDK_MODULE_ID_H_
#define _XSDK_MODULE_ID_H_

#include "XSDK/Types.h"


//=============================================================================
/// @mainpage XSDK
///
/// XSDK is a package that contains utility classes and templates that
/// provide object-oriented abstractions of many common types. For example,
/// the XUuid class wraps the X_UUID datatype and provides many useful
/// methods for manipulating UUIDs.
///
/// All classes and datatypes defined by this package are contained within
/// the XSDK namespace to prevent collisions. Each header file will be
/// installed under the "XSDK" directory to provide namespacing for the file
/// names. For example, to include the "XUuid.h" header file in your code, add
/// the following:
///
/// @code
/// #include "XSDK/XUuid.h"
/// @endcode
///
/// When using the XSDK package, make sure to link your project against
/// libXSDK.so.
//=============================================================================

//=============================================================================
/// Encapsulates all of the types provided by the XSDK package.
//=============================================================================
namespace XSDK
{
    //=========================================================================
    /// This is the module identifier. The module identifier is primarily
    /// used to indicate what module has thrown an exception.
    //=========================================================================

        //const char ModuleId[] = "www.pelco.com/XSDK";

} // XSDK

#endif // _XSDK_MODULE_ID_H_

