
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef _X_SHARED_LIB_H_
#define _X_SHARED_LIB_H_

//=============================================================================
/// @file
/// This header is used to set the import/export mode to apply for the
/// current executable. The primary outcome from the header is the definition
/// of the X_API macro. This macro should precede any function or class
/// method declaration for which the import/export mode needs to be applied.
/// \n\n
/// By Default, the header defines the import/export mode for X_API
/// based on whether the X_API_EXPORT or the X_API_IMPORT macro is defined
/// (normally defined within the project settings). These macros are expected
/// to be defined during the compilation of a binary in order to set the
/// default mode for X_API.
/// \n\n
/// It is sometime necessary to switch import/export modes within a source
/// code module. This often occurs when a shared library by default exports
/// its functions/methods, but then needs to make use of another shared
/// library's imports. If the required shared libraries header file is
/// included where the X_API macro is set for exporting, the compiler
/// will assume that the required shared library's functions/methods are
/// to be exported as well. The end results are linker errors indicating
/// that the needed shared library's functions/methods are undefined. To
/// fix this issue, there are a number of header files that can be used
/// to temporarily switch the import/export mode. The general idea is
/// to switch to the desired mode, include the needed shared library's
/// header files and then switch back to the default import/export mode.
/// \n\n
/// The headers used to switch between import/export modes include the
/// following: XSharedLibExp.h, XSharedLibImp.h and XSharedLibDef.h.
/// A common use for these headers often looks like the following:
/// \n\n
/// @code
/// // Normally would appear in your project settings
/// #define X_API_EXPORT
///
/// // Include my headers as would normally be done (notice
/// // that we are setup to export all X_API functions/methods)
/// #include "MyGreatCode.h"
/// #include "MyLifeChangingCode.h"
///
/// // Switch import/export modes for the shared library headers
/// // (in this case, we are temporarily switching to importing)
/// #include "XSharedLibImp.h"
///
/// // Now X_API will be set to import from the library
/// // we want to pull function/method implementations from
/// #include "YourLibHeader.h"
/// #include "YourOtherLibHeader.h"
///
/// // Put X_API back to its default import/export mode (which would
/// // be to export due to the X_API_EXPORT macro definition above)
/// #include "XSharedLibDef.h" @endcode
//=============================================================================

#include "XSDK/Types.h"


// Do we have both exporting and importing macros defined?
#if (defined(X_API_EXPORT) && defined(X_API_IMPORT))

    // Report an error - it does not make sense to define both macros
#   error Cannot define both X_API_EXPORT and X_API_IMPORT
#endif

// Make sure the macros set the thhis header are cleared
#undef X_API
#undef DLLIMPORT
#undef DLLEXPORT

#ifdef IS_WINDOWS
    #define X_API __declspec(dllexport)
    #define DLLIMPORT __declspec(dllimport)
    #define DLLEXPORT __declspec(dllexport)
#else
    // On *nix, CK_API et al. expand into nothingness...
    #if (!defined(X_API))
        #define X_API
    #endif
    #if (!defined(DLLIMPORT))
        #define DLLIMPORT
    #endif
    #if (!defined(DLLEXPORT))
        #define DLLEXPORT
    #endif
#endif

// If X_API has not be set to anything, define it to be
// nothing (i.e., disable it from applying any special meaning)
#if (!defined(X_API))
#   define X_API
#endif

// If DLLIMPORT has not be set to anything, define it to be
// nothing (i.e., disable it from applying any special meaning)
#if (!defined(DLLIMPORT))
#   define DLLIMPORT
#endif

// If DLLEXPORT has not be set to anything, define it to be
// nothing (i.e., disable it from applying any special meaning)
#if (!defined(DLLEXPORT))
#   define DLLEXPORT
#endif


#endif // _X_SHARED_LIB_H_

