
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//=============================================================================
/// @file
/// This header is used to temporarily switch from the function/method
/// default import/export mode so that all functions/methods proceeded
/// by the X_API macro are being imported. Normally, the inclusion of
/// this header is followed by some number of headers (for which the
/// import/export mode is being modified) followed by the inclusion of
/// the XSharedLibDef.h header file. For a more complete description
/// of how this header file (and related header files) are to be used
/// see the documentation for the XSharedLib.h file.
//=============================================================================


// Clear some (possibly) already defined
// values before attempting the switch
#undef X_API
#undef _X_API_SWITCH_TO_EXPORT_

// Define a macro to indicate that the default import/export
// mode is to be ignored (i.e., X_API_EXPORT or X_API_IMPORT)
#define _X_API_IGNORE_DEFAULT_

// Define a macro to indicate that we are to switch the
// mode such that we are importing functions/methods
#define _X_API_SWITCH_TO_IMPORT_

// Re-include the header used to define the import/export mode (we clear the
// header sentinal definition to make sure the header content is not skipped)
#undef _X_SHARED_LIB_H_
#include "XSDK/XSharedLib.h"

// Undefine the macro definitions used to indicate
// we are switching to importing functions/methods
#undef _X_API_SWITCH_TO_IMPORT_
#undef _X_API_IGNORE_DEFAULT_

