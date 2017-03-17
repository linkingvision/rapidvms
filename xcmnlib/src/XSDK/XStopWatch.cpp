
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "XSDK/Errors.h"
#include "XSDK/XStopWatch.h"

using namespace XSDK;

XStopWatch::XStopWatch(bool start) :
    _running(false),
    _totalTime(),
    _intervalStart(0)
{
    if(start)
        Start();
}

void XStopWatch::Start()
{
    if(!_running)
        _intervalStart = XMonoClock::GetTime();
    _running = true;
}

void XStopWatch::Stop()
{
    if(_running)
    {
        _totalTime += XDuration(HNSECS, XMonoClock::GetElapsedTime(_intervalStart, XMonoClock::GetTime()) * convert(SECONDS, HNSECS, 1));
        _running = false;
    }
}

void XStopWatch::Restart()
{
    Stop();
    Reset();
    Start();
}

void XStopWatch::Reset()
{
    _totalTime = XDuration::Zero();
    _intervalStart = XMonoClock::GetTime();
}

double XStopWatch::GetAsSeconds() const
{
    return double(TimeElapsed().Total(HNSECS)) / convert(SECONDS, HNSECS, 1);
}

double XStopWatch::GetAsUSeconds() const
{
    return double(TimeElapsed().Total(HNSECS)) / convert(USECS, HNSECS, 1);
}

XDuration XStopWatch::TimeElapsed() const
{
    if(_running)
    {
        const double elapsed = XMonoClock::GetElapsedTime(_intervalStart, XMonoClock::GetTime());
        return _totalTime + XDuration(HNSECS, elapsed * convert(SECONDS, HNSECS, 1));
    }

    return _totalTime;
}

XStopWatch::XStopWatch(const XStopWatch& obj)
{
    _running = obj._running;
    _totalTime = obj._totalTime;
    _intervalStart = obj._intervalStart;
}

XStopWatch& XStopWatch::operator=(const XStopWatch& obj)
{
    _running = obj._running;
    _totalTime = obj._totalTime;
    _intervalStart = obj._intervalStart;
    return *this;
}
