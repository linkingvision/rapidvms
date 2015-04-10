
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//============================================================================
// This file needs to be used in conjunction with "XMemDebugBegin.h". These
// files are used to activate memory debugging on the Windows platform.
//
// Usage: These files should be included in any .cpp file where memory
// debugging is desired. The "XMemDebugBegin.h" include file needs to be
// the first include file in the .cpp's include list. The "XMemDebugEnd.h"
// needs to be the last include file in the .cpp's include list.
//
// @see "Memory Leak Detection Enabling" in the MSN help documentation.
//      Also see http://msdn.microsoft.com/msdnmag/issues/1200/bugslayer.
//============================================================================
#include "XSDK/Module.h"

#ifdef IS_WINDOWS

#include <stdlib.h>
#include <crtdbg.h>

#ifdef IS_DEBUG
#ifndef NEW_INLINE_WORKAROUND

#define NEW_INLINE_WORKAROUND new (_NORMAL_BLOCK ,__FILE__, __LINE__)
#define new NEW_INLINE_WORKAROUND

#endif // NEW_INLINE_WORKAROUND
#endif // IS_DEBUG

#endif // IS_WINDOWS

