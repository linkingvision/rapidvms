
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

#define MICROS_IN_SECOND 1000000

using namespace XSDK;

XCondition::XCondition( XMutex& lok ) :
    _lok( lok ),
    _numWaitingThreads( 0 )
{
    InitializeCriticalSection( &_threadCountLock );

    // We have to use both types of Win32 events to get the exact behavior we
    // expect from a POSIX condition variable. We use an auto-reset event for
    // the signal() call so only a single thread is woken up. We rely on the
    // "stickyness" of a manual-reset event to properly implement broadcast().
    _events[SIGNAL]    = CreateEvent( NULL, false, false, NULL ); // auto-reset event
    _events[BROADCAST] = CreateEvent( NULL, true, false, NULL );  // manual-reset event
    if( _events[SIGNAL] == NULL || _events[BROADCAST] == NULL )
        X_THROW(( "Unable to allocate Win32 event objects." ));
}

XCondition::~XCondition() throw()
{
    if( _events[SIGNAL] )
        CloseHandle( _events[SIGNAL] );
    if( _events[BROADCAST] )
        CloseHandle( _events[BROADCAST] );

    DeleteCriticalSection( &_threadCountLock );
}

void XCondition::Wait()
{
    // We keep track of the number of waiting threads so we can guarantee that
    // all waiting threads are woken up by a broadcast() call.
    EnterCriticalSection( &_threadCountLock );
    _numWaitingThreads++;
    LeaveCriticalSection( &_threadCountLock );

    // Release the lock before waiting... this is standard POSIX condition
    // semantics.
    _lok.Release();

    // NOTE: We avoid the "lost wakeup" bug common in many Win32 condition
    // implementations because we are using a manual-reset event. If we were
    // only using an auto-reset event, it would be possible to lose a signal
    // right here due to a race condition.

    // Wait for either event to be signaled.
    DWORD result = WaitForMultipleObjects( MAX_EVENTS, _events, false, INFINITE );
    if( ( result < WAIT_OBJECT_0 ) || ( result > WAIT_OBJECT_0 + MAX_EVENTS ) )
    {
        X_THROW(( "An error occurred what waiting for the Win32 event objects. Errorcode = 0x%08X.",
            GetLastError() ));
    }

    // The following code is only required for the broadcast(). Basically, if
    // broadcast was called AND we are the last waiting thread, then reset the
    // manual-reset event.
    EnterCriticalSection( &_threadCountLock );
    _numWaitingThreads--;
    bool lastThreadWaiting = ( result == WAIT_OBJECT_0 + BROADCAST ) && ( _numWaitingThreads == 0 );
    LeaveCriticalSection( &_threadCountLock );

    if( lastThreadWaiting )
        ResetEvent( _events[BROADCAST] );

    // Reacquire the lock before returning... again, this is standard POSIX
    // condition semantics.
    _lok.Acquire();
}

//bool XCondition::WaitUntil( time_t secondsAbsolute, time_t microsecondsAbsolute )
bool XCondition::WaitUntil( uint64_t then )
{
    //
    // NOTE: See the Wait() method for more details about how this works
    // on Windows.
    //

    EnterCriticalSection( &_threadCountLock );
    _numWaitingThreads++;
    LeaveCriticalSection( &_threadCountLock );

    // Convert the absolute wait timeout into a relative time. Windows wait
    // functions only operate on relative time (in milliseconds).

    int timeoutMS = (int)(ConvertTimeScale( then - XMonoClock::GetTime(), XMonoClock::GetFrequency(), 1000 ));
    if( timeoutMS < 0 )
        timeoutMS = 0;

    bool timedOut = false;

    _lok.Release();

    DWORD result = WaitForMultipleObjects( MAX_EVENTS, _events, false, (DWORD)timeoutMS );
    if( result == WAIT_TIMEOUT )
        timedOut = true;
    if( !timedOut && ( ( result < WAIT_OBJECT_0 ) || ( result > WAIT_OBJECT_0 + MAX_EVENTS ) ) )
    {
        X_THROW(( "An error occurred what waiting for the Win32 event objects. Errorcode = 0x%08X.",
            GetLastError() ));
    }

    EnterCriticalSection( &_threadCountLock );
    _numWaitingThreads--;
    bool lastThreadWaiting = ( result == WAIT_OBJECT_0 + BROADCAST ) && ( _numWaitingThreads == 0 );
    LeaveCriticalSection( &_threadCountLock );

    if( lastThreadWaiting )
        ResetEvent( _events[BROADCAST] );

    _lok.Acquire();

    return timedOut;
}

void XCondition::Signal()
{
    EnterCriticalSection( &_threadCountLock );
    bool threadsWaiting = _numWaitingThreads > 0;
    LeaveCriticalSection( &_threadCountLock );

    if( threadsWaiting )
        SetEvent( _events[SIGNAL] );
}

void XCondition::Broadcast()
{
    EnterCriticalSection( &_threadCountLock );
    bool threadsWaiting = _numWaitingThreads > 0;
    LeaveCriticalSection( &_threadCountLock );

    if( threadsWaiting )
        SetEvent( _events[BROADCAST] );
}

uint64_t XCondition::FutureMonotonicTime( time_t secondsAbsolute, time_t microsecondsAbsolute )
{
    struct timeval future = { secondsAbsolute, microsecondsAbsolute };

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
