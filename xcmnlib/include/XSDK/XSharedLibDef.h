
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
/// This header is used to set a source code module back to its default
/// import/export mode. The macro X_API_IMPORT or the macro X_API_EXPORT
/// is intended to be used to indicate the default import/export mode to use
/// within a source module. In some cases, however, it may be necessary to
/// temporarily change the default import/export within the source. To change
/// the mode, the headers XSharedLibImp.h and XSharedLibExp.h have been
/// provided. This header is to be included after including XSharedLibExp.h
/// or XSharedLibImp.h to put the mode back to what it should be set to
/// based on the X_API_IMPORT/X_API_EXPORT macros. For a more complete
/// description of how this header file and related header files are to be
/// used, see the documentation in the XSharedLib.h header file.
//=============================================================================


// Make sure all macros used to temporarily switch to a different mode
// are undefined so we can move back to the default configured mode
#undef _X_API_SWITCH_TO_EXPORT_
#undef _X_API_SWITCH_TO_IMPORT_
#undef _X_API_IGNORE_DEFAULT_

// Clear whatever mode was previously defined
#undef X_API

// Re-include the main include file used to set the mode (we undefine
// the sentinel for the header to make sure its contents are not skipped)
#undef _X_SHARED_LIB_H_
#include "XSDK/XSharedLib.h"

