
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "XSDK/XTimer.h"
#include "XSDK/XGuard.h"

using namespace XSDK;

XTimer::XTimer() :
    XTaskBase(),
    _interval( 0 ),
    _oneShot( false ),
    _callback( 0 ),
    _data( 0 ),
    _cond( _lok ),
    _stop( false )
{
}

XTimer::~XTimer() throw()
{
}

// Thread function.
void* XTimer::EntryPoint()
{
    this->DoTimerLoop( _interval, _oneShot, _callback, _data );
    return 0;
}

void XTimer::StartTimer( unsigned int interval, bool oneShot, TimerCB callback, void* data )
{
    if( !IsRunning() )
    {
        _interval = interval;
        _oneShot = oneShot;
        _callback = callback;
        _data = data;

        _stop = false;

        this->Start(); // Thread calls EntryPoint()
    }
}

void XTimer::StopTimer()
{
    {
        XGuard lok( _lok );
        _stop = true;
        _cond.Signal();
    }

    if( IsRunning() )
        this->Join();
}

void XTimer::DoTimerLoop( unsigned int interval, bool oneShot, TimerCB callback, void* data )
{
    do
    {
        // See if it's time to exit.
        {
            XGuard lok( _lok );
            if( _stop )
                return;
        }

        // Calculate when to wakeup.
        double intervalSeconds = (((double)interval) / 1000);
        uint64_t deltaTicks = uint64_t(intervalSeconds * XMonoClock::GetFrequency());
        uint64_t wakeTime = XMonoClock::GetTime() + deltaTicks;

        // Sleep
        {
            XGuard lok( _lok );
            while( !_cond.WaitUntil( wakeTime ) )
            {
                if( _stop )
                    return;
            }

            if( _stop )
                return;
        }

        // Execute timer callback.
        callback( data );

    }while( !_oneShot );
}
