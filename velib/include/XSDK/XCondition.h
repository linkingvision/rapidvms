
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef XSDK_XCondition_h
#define XSDK_XCondition_h

#include "XSDK/XDuration.h"
#include "XSDK/XException.h"
#include "XSDK/XMutex.h"
#include "XSDK/Platform.h"

#ifndef WIN32
#include <pthread.h>
#endif

namespace XSDK
{

class XCondition
{
public:
    X_API XCondition( XSDK::XMutex& lok );
    X_API virtual ~XCondition() throw();

    /// Wait() until the condition is signaled, or not.. Depends really
    /// on our mood at the time... (see 'man pthread_cond_wait')
    X_API void Wait();

    /// Wait() until the condition is signaled, or the timeout has expired
    /// or not.. Depends really on our mood at the time... (see 'man pthread_cond_wait')
    /// Returns true if the call times out.
    X_API bool WaitUntil( uint64_t then );

    // Signal our condition, unblock the first waiting thread...
    X_API void Signal();

    // Unblock ALL waiting (one at a time)...
    X_API void Broadcast();

    /// Get the absolute monotonic time from the given absolute wall clock time.
    X_API static uint64_t FutureMonotonicTime( time_t secondsAbsolute, time_t microsecondsAbsolute );

    /// Get the absolute monotonic time which is the given duration from now.
    X_API static uint64_t FutureMonotonicTime( XDuration timeFromNow );

private:
    XCondition( const XCondition& obj );
    XCondition& operator = ( const XCondition& obj );

#ifdef WIN32
    enum { SIGNAL=0, BROADCAST=1, MAX_EVENTS=2 } ;
    HANDLE _events[XCondition::MAX_EVENTS];
    volatile unsigned int _numWaitingThreads;
    CRITICAL_SECTION _threadCountLock;
#else
    pthread_cond_t _cond;
    pthread_condattr_t _attr;
#endif // WIN32

    XSDK::XMutex& _lok;
};

}

#endif
