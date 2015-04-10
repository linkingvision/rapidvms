
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef XSDK_XTimer_h
#define XSDK_XTimer_h

#include "XSDK/TimeUtils.h"
#include "XSDK/XTaskBase.h"
#include "XSDK/XCondition.h"
#include "XSDK/XTime.h"
#include "XSDK/XMonoClock.h"

namespace XSDK
{

/**
    \brief A timer class that you Poll to know if it has gone off.

    XSimpleTimer does not start any threads or have any callback functions or
    anything like that. Rather, you tell it at what interval you want the
    timer to go off and then you Poll it to find out whether it has gone off.
    The idea is that if you're supposed to do something in a function every
    X seconds (or minutes or whatever), you use an XSimpleTimer to keep track
    of whether you're supposed to do that something yet.
 */
class XSimpleTimer
{
public:

    /**
        Constructs timer which isn't running.
     */
    XSimpleTimer()
        : _startOfInterval(XMonoClock::GetTime()),
          _intervalLen(XDuration::Zero()),
          _repeat(false),
          _running(false)
    {
    }

    /**
        \brief Constructor which starts the timer immediately.

        \param How often the timer should go off.
        \param Whether the time should go off more than once.
     */
    XSimpleTimer(XDuration intervalLen, bool repeat = true)
        : _startOfInterval(XMonoClock::GetTime()),
          _intervalLen(intervalLen),
          _repeat(repeat),
          _running(true)
    {
        X_ASSERT(!intervalLen.IsNegative());
    }


    /**
        \brief Constructor which starts the timer at the time given.

        \param The time to start the timer.
        \param How often the timer should go off.
        \param Whether the time should go off more than once.
     */
    XSimpleTimer(XTime startTime, XDuration intervalLen, bool repeat = true)
        : _startOfInterval(XCondition::FutureMonotonicTime( startTime - Clock::currTime() )),
          _intervalLen(intervalLen),
          _repeat(repeat),
          _running(true)
    {
        X_ASSERT(!intervalLen.IsNegative());
    }


    /**
        \brief Returns whether the timer has gone off.

        When Poll is called, if the timer has gone off and the timer is
        set to repeat, then it will be set to go off again at intervalLen
        from the last time that the timer went off, not intervalLen from
        when you Polled last.
     */
    bool Poll()
    {
        if(!_running)
            return false;

        uint64_t currTime = XMonoClock::GetTime();

        if( currTime <= _startOfInterval )
            return false;

        uint64_t timePassedTicks = currTime - _startOfInterval;

        XDuration timePassedInterval( USECS, (int64_t)(((double)timePassedTicks / XMonoClock::GetFrequency())*1000000) );

        const bool retval = timePassedInterval >= _intervalLen;

        if(retval)
        {
            const int64_t multiplier = timePassedInterval / _intervalLen;

            XDuration delta = _intervalLen * multiplier;

            _startOfInterval += (uint64_t)((((double)((delta.Total(SECONDS) * 1000000) + delta.FracSec().USecs())) / 1000000) * XMonoClock::GetFrequency());

            if(!_repeat)
                _running = false;
        }

        return retval;
    }


    /**
        \brief Starts the timer using the current time as the starting point
               and the same intervalLen value that it was constructed with.

        If the timer is already running, then this resets the timer.
      */
    void Start(bool repeat = true)
    {
        *this = XSimpleTimer(_intervalLen, repeat);
    }


    /**
        \brief Starts the timer using the current time as the starting point
               and the given intervalLen.

        If the timer is already running, then this resets the timer.
      */
    void Start(XDuration intervalLen, bool repeat = true)
    {
        *this = XSimpleTimer(intervalLen, repeat);
    }


    /**
        \brief Starts the timer at the given time using the given intervalLen.

        If the timer is already running, then this resets the timer.
      */
    void Start(XTime startTime, XDuration intervalLen, bool repeat = true)
    {
        *this = XSimpleTimer(startTime, intervalLen, repeat);
    }


    /**
        \brief Stops the timer.

        Poll() will return false after this until Start() is called, even if the
        timer has gone off since the last time Poll() was called.
      */
    void Stop() { _running = false; }


    /**
        \brief Returns the time interval between consecutive times that
               the timer goes off.
     */
    XDuration IntervalLen() const { return _intervalLen; }


    /**
        \brief Whether the timer is currently running.
     */
    bool IsRunning() const { return _running; }


    /**
        \brief Whether the timer is set to repeat.
     */
    bool Repeating() const { return _repeat; }


private:

    uint64_t  _startOfInterval;
    XDuration _intervalLen;
    bool      _repeat;
    bool      _running;
};


class XTimer : public XSDK::XTaskBase
{
public:
    typedef void (*TimerCB)( void* data );

public:
    X_API XTimer();
    X_API virtual ~XTimer() throw();

    /// Creates a timer thread and starts the timer.
    /// @param interval The interval of the timer in milliseconds.
    /// @param oneShot If true, the timer will only fire once. Otherwise, the
    ///                timer will continue to fire until at each interval
    ///                until Stop() is called.
    /// @param callback A pointer to the method that gets called when the timer
    ///                 fires.
    /// @param data User-defined data that gets passed to the callback function.
    X_API void StartTimer(unsigned int interval, bool oneShot, XTimer::TimerCB callback, void* data);

    /// Stops/Cancels the timer.
    /// @note This method will cause DoTimerLoop() to return.
    X_API void StopTimer();

    /// Implements the main timer logic. Call this method instead of Start() if
    /// you want to implement the timer using your own thread. This method
    /// doesn't return until the timer has fired (if 'oneShot' is true), or
    /// Stop() is called.
    /// See the documentation for Start() for an explanation of the parameters.
    X_API void DoTimerLoop(unsigned int interval, bool oneShot, XTimer::TimerCB callback, void* data);

private:
    //Disallow copying.
    X_API XTimer(const XTimer&);
    X_API XTimer& operator =(const XTimer&);

    // Hide these XTaskBase methods.
    void Start() { XTaskBase::Start(); }
    void* Join() { return XTaskBase::Join(); }
    X_API virtual void* EntryPoint();

    // Timer arguments
    unsigned int _interval;
    bool    _oneShot;
    TimerCB _callback;
    void*   _data;

    XSDK::XMutex     _lok;
    XSDK::XCondition _cond; ///< Condition for signaling DoTimerLoop to return.
    volatile bool    _stop; ///< Indicates the timer loop should stop.
};

};

#endif
