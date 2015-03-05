
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef XSDK_XMonoClock_h
#define XSDK_XMonoClock_h

#include "XSDK/XSharedLib.h"

namespace XSDK
{
    // The XMonoClock class provides access to a high-performance, monotonically
    // increasing counter that can be used for interval timing. The counter value
    // returned from GetTime() increments at the rate returned by GetFrequency().
    class XMonoClock
    {
    public:

        // @return The current value of the clock.
        X_API static uint64_t GetTime();

        // @return The number of ticks per second that the clock increments.
        X_API static uint64_t GetFrequency();

        // @param start The clock value at the start of the interval we are measuring.
        // @param end The clock value at the end of the interval we are measuring.
        // @return The elapsed time (in seconds) from 'start' until 'end'.
        X_API static double GetElapsedTime(uint64_t start, uint64_t end);
    };
}

#endif
