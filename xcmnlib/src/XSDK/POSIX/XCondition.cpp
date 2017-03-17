
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "XSDK/XCondition.h"
#include "XSDK/Errors.h"
#include "XSDK/TimeUtils.h"
#include "XSDK/XMonoClock.h"

#include <errno.h>

using namespace XSDK;

#define MICROS_IN_SECOND 1000000
#define NANOS_IN_SECOND 1000000000

#ifdef __MACH__
#include <mach/mach_time.h>
#define CLOCK_REALTIME 0
#define CLOCK_MONOTONIC 0
static int clock_gettime(int clk_id, struct timespec *t){
    mach_timebase_info_data_t timebase;
    mach_timebase_info(&timebase);
    uint64_t time;
    time = mach_absolute_time();
    double nseconds = ((double)time * (double)timebase.numer)/((double)timebase.denom);
    double seconds = ((double)time * (double)timebase.numer)/((double)timebase.denom * 1e9);
    t->tv_sec = seconds;
    t->tv_nsec = nseconds;
    return 0;
}
#endif

XCondition::XCondition( XMutex& lok ) :
    _cond(),
    _attr(),
    _lok( lok )
{
    int err = pthread_condattr_init( &_attr );

    if( err < 0 )
        X_THROW(( "Unable to allocate condition variable attribute object." ));
#ifndef __APPLE__
    err = pthread_condattr_setclock( &_attr, CLOCK_MONOTONIC );

    if( err < 0 )
        X_THROW(( "Unable to specify monotonic clock for condition variable." ));
#endif
    err = pthread_cond_init( &_cond, &_attr );

    if( err < 0 )
        X_THROW(( "Unable to allocate condition variable." ));
}

XCondition::~XCondition() throw()
{
    pthread_cond_destroy( &_cond );

    pthread_condattr_destroy( &_attr );
}

void XCondition::Wait()
{
    int err = pthread_cond_wait( &_cond, &_lok._lok );

    if( err < 0 )
        X_THROW(( "Unable to pthread_cond_wait() on a condition variable." ));
}

bool XCondition::WaitUntil( uint64_t then )
{
    uint64_t freq = XMonoClock::GetFrequency();

    struct timespec timeout = { (time_t)(then / freq),
                                (time_t)ConvertTimeScale( (then % freq), freq, NANOS_IN_SECOND ) };

    int err = pthread_cond_timedwait( &_cond, &_lok._lok, &timeout );

    if( err == ETIMEDOUT )
        return true;

    if( err < 0 )
        X_THROW(( "Unable to pthread_cond_timedwait() on a condition variable." ));

    return false;
}

void XCondition::Signal()
{
    int err = pthread_cond_signal( &_cond );

    if( err < 0 )
        X_THROW(( "Unable to pthread_cond_signal() on a condition variable." ));
}

void XCondition::Broadcast()
{
    int err = pthread_cond_broadcast( &_cond );

    if( err < 0 )
        X_THROW(( "Unable to pthread_cond_broadcast() on a condition variable." ));
}

uint64_t XCondition::FutureMonotonicTime( time_t secondsAbsolute, time_t microsecondsAbsolute )
{
    struct timeval future = { (int)secondsAbsolute, (int)microsecondsAbsolute };

    struct timeval now;
    x_gettimeofday( &now );

    struct timeval delta;
    timersub( &future, &now, &delta );

    const double deltaSeconds = (((double)((delta.tv_sec * MICROS_IN_SECOND) + delta.tv_usec)) / MICROS_IN_SECOND);

    return XMonoClock::GetTime() + (uint64_t)(deltaSeconds * XMonoClock::GetFrequency());
}

uint64_t XCondition::FutureMonotonicTime( XDuration timeFromNow )
{
    const double deltaSeconds = ((double)timeFromNow.Total(HNSECS)) / convert(SECONDS, HNSECS, 1);

    return deltaSeconds > 0 ? XMonoClock::GetTime() + (uint64_t)(deltaSeconds * XMonoClock::GetFrequency())
                            : XMonoClock::GetTime();
}
