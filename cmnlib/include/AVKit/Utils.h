
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef __AVKit_Utils_h
#define __AVKit_Utils_h

#include "XSDK/Types.h"
#include "XSDK/XSharedLib.h"
#include <climits>

namespace AVKit
{

// Adjusts requestedWidth X requestedHeight to maintain aspect ratio of streamWidth X streamHeight.

X_API void AspectCorrectDimensions( uint16_t streamWidth, uint16_t streamHeight,
                                    uint16_t requestedWidth, uint16_t requestedHeight,
                                    uint16_t& destWidth, uint16_t& destHeight );

X_API double QToD( int num, int den );

X_API void DToQ( double val, int& num, int& den, int max = INT_MAX );

}

#endif
