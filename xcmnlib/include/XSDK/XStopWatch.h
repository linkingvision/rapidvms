
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef XSDK_XStopWatch_h
#define XSDK_XStopWatch_h

#include "XSDK/XSharedLib.h"
#include "XSDK/XDuration.h"
#include "XSDK/XMonoClock.h"

namespace XSDK
{

/// The XStopWatch class uses a high-performance clock to measure short intervals
/// with high accurracy. Multiple calls to Start and Stop can be made to measure
/// the total accumulated time of each interval.
class XStopWatch
{
public:
    explicit X_API XStopWatch(bool start = false);
    X_API virtual ~XStopWatch() throw() {}

    /// \brief Starts the stopwatch if it wasn't already started; otherwise, does nothing.
    ///        It does not reset the counter.
    X_API void Start();

    /// Stops the stopwatch. Calling Start will then cause it to continue.
    X_API void Stop();

    /// Stops, Resets, and Starts the stopwatch.
    X_API void Restart();

    /// Resets the counter but does not change the stopwatch's running state.
    X_API void Reset();

    /// \return Returns the total time in seconds.
    X_API double GetAsSeconds() const;

    /// \return Returns the total time in microseconds.
    X_API double GetAsUSeconds() const;

    /// The total time that the stopwatch has been running.
    ///
    /// If the stopwatch was stopped and then started again (not restarted),
    /// then the all of the time that it was running is part of TimeElapsed
    /// but not the time during which it was stopped.
    X_API XDuration TimeElapsed() const;

    X_API XStopWatch(const XStopWatch& obj);
    X_API XStopWatch& operator = (const XStopWatch& obj);

private:

    bool _running;
    XDuration _totalTime;
    uint64_t _intervalStart;
};

};

#endif
