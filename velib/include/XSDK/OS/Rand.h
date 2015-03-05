
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "XSDK/XSharedLib.h"

namespace XSDK
{

/// Wrapper around rand which makes it thread-safe.
X_API int x_rand();

/// Wrapper around srand which makes it thread-safe.
X_API void x_srand(unsigned int seed);

/// Calls x_srand with the seed but only if x_srand has not previously been called.
X_API void x_srand_if(unsigned int seed);

};
