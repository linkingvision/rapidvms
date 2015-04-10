
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef XSDK_XTimedCommandQueue_h
#define XSDK_XTimedCommandQueue_h

#include "XSDK/XCommandQueue.h"
#include "XSDK/XInternalList.h"
#include "XSDK/TimeUtils.h"
#include "XSDK/XMonoClock.h"

namespace XSDK
{

template< class T >
class XTimedCommandQueue : public XCommandQueue<T>
{
 public:
    typedef void (*TimerCB)( void* context, int id );

 private:
    class TimedEvent
    {
        friend class XTimedCommandQueue<T>;
    public:
        TimedEvent() :
            _absFireTime( 0 ),
            _relFireTimeSeconds( 0 ),
            _relFireTimeMicro( 0 ),
            _repeating( false ),
            _callback( NULL ),
            _context( NULL ),
            _id( 0 )
        {
        }

        TimedEvent( const TimedEvent& obj ) :
            _absFireTime( obj._absFireTime ),
            _relFireTimeSeconds( obj._relFireTimeSeconds ),
            _relFireTimeMicro( obj._relFireTimeMicro ),
            _repeating( obj._repeating ),
            _callback( obj._callback ),
            _context( obj._context ),
            _id( obj._id )
        {
        }

        virtual ~TimedEvent() throw()
        {
        }

        TimedEvent& operator = ( const TimedEvent& obj )
        {
            _absFireTime = obj._absFireTime;
            _relFireTimeSeconds = obj._relFireTimeSeconds;
            _relFireTimeMicro = obj._relFireTimeMicro;
            _repeating = obj._repeating;
            _callback = obj._callback;
            _context = obj._context;
            _id = obj._id;

            return *this;
        }

    private:
        uint64_t _absFireTime;
        time_t _relFireTimeSeconds;
        time_t _relFireTimeMicro;
        bool _repeating;
        TimerCB _callback;
        void* _context;
        int _id;
    };

public:

    XTimedCommandQueue( const XString& threadName = DEFAULT_THREAD_NAME ) :
        XCommandQueue<T>( true, threadName ),
        _timedEvents( _EventCompare ),
        _nextTimerId( 0 )
    {
    }

    virtual ~XTimedCommandQueue() throw()
    {
    }

    int CreateTimedEvent( time_t relFireTimeSeconds, time_t relFireTimeMicro,
                          TimerCB callback,
                          void* context,
                          bool repeating )
    {
        XGuard guard( this->_queueLock );

        return _CreateTimedEventInternal( relFireTimeSeconds, relFireTimeMicro,
                                          callback, context, repeating, _GetNextTimerId() );
    }

    void CancelTimedEvent( int id )
    {
        XGuard guard( this->_queueLock );

        for( _timedEvents.IterStart(); _timedEvents.IterValid(); _timedEvents.Iterate() )
        {
            if( _timedEvents.IterData()->_id == id )
            {
                _timedEvents.RemoveAtIter();
                break;
            }
        }
    }

    virtual void* EntryPoint()
    {

        while( this->_running )
        {
            try
            {
                T type = T();
                TimedEvent event;
                bool eventFireTime = false;

                XGuard guard( this->_queueLock );

                while( this->_queueS.empty() && this->_running && !eventFireTime )
                {
                    if( _timedEvents.Empty() )
                        this->_queueCond.Wait();
                    else
                    {
                        _timedEvents.IterStart();
                        if( _timedEvents.IterValid() )
                        {
                            int waitingId = _timedEvents.IterData()->_id;
                            if( this->_queueCond.WaitUntil( _timedEvents.IterData()->_absFireTime ) == true )
                            {
                                _timedEvents.IterStart();
                                if( _timedEvents.IterValid() && _timedEvents.IterData()->_id == waitingId )
                                {
                                    eventFireTime = true;

                                    if( _timedEvents.IterData()->_repeating )
                                        _CreateTimedEventInternal( _timedEvents.IterData()->_relFireTimeSeconds,
                                                                   _timedEvents.IterData()->_relFireTimeMicro,
                                                                   _timedEvents.IterData()->_callback,
                                                                   _timedEvents.IterData()->_context,
                                                                   _timedEvents.IterData()->_repeating,
                                                                   _timedEvents.IterData()->_id );
                                }
                            }
                        }
                    }
                }

                if( !this->_running )
                    continue;

                //The order in which we check these two coniditions MATTERS!
                //It is possible for BOTH conditions to be true.
                //In the case that both conditions are true if we were
                //to check the queue first inside of an if else if block
                //we would remove the command queue command and not process it
                //and not call the event timer callback.
                if( eventFireTime )
                {
                    _timedEvents.IterStart();
                    if( _timedEvents.IterValid() )
                    {
                        event = *_timedEvents.IterData();
                        _timedEvents.RemoveAtIter();

                        if( event._callback )
                        {
                            guard.Release();
                            (*event._callback)( event._context, event._id );
                        }
                    }
                }
                else if( !this->_queueS.empty() )
                {
                    type = this->_queueS.front();
                    this->_queueS.pop_front();

                    //Release the queue lock to allow other threads
                    //to post commands while we Process the current one.
                    guard.Release();
                    bool ret = this->Process( type );
                    guard.Acquire();
                    if( !ret )
                        this->_queueS.push_front( type );
                }
            }
            catch( std::exception& ex )
            {
                X_LOG_ERROR( ex.what() );
            }
            catch( ... )
            {
                X_LOG_ERROR( "[unknown] - An unknown exception occured." );
            }
        }


        return NULL;
    }


    void* AccessContext(int id)
    {
        x_foreach(_timedEvents)
        {
            if( _timedEvents.IterData() == NULL )
                X_THROW(("List had NULL value but should not!"));
            if( _timedEvents.IterData()->_id == id )
                return _timedEvents.IterData()->_context;
        }

        return NULL;
    }


 private:

    inline int _GetNextTimerId()
    {
        return ++_nextTimerId;
    }

    static int _EventCompare( const TimedEvent& a, const TimedEvent& b, void* compareContext = NULL )
    {
#ifdef WIN32
        compareContext;
#endif
        if( a._absFireTime < b._absFireTime )
            return -1;
        if( a._absFireTime == b._absFireTime )
            return 0;
        return 1;
    }

    int _CreateTimedEventInternal( time_t relFireTimeSeconds, time_t relFireTimeMicro,
                                   TimerCB callback,
                                   void* context,
                                   bool repeating,
                                   int id )
    {
        double deltaSeconds = (double)relFireTimeSeconds + ((double)relFireTimeMicro / 1000000);

        uint64_t now = XMonoClock::GetTime();

        uint64_t absFireTime = now + (uint64_t)(deltaSeconds * (double)XMonoClock::GetFrequency());

        TimedEvent event;

        event._absFireTime = absFireTime;
        event._relFireTimeSeconds = relFireTimeSeconds;
        event._relFireTimeMicro = relFireTimeMicro;
        event._repeating = repeating;
        event._callback = callback;
        event._context = context;
        event._id = id;

        _timedEvents.InsertSorted( event );

        this->_queueCond.Signal();

        return id;
    }

    XInternalList<TimedEvent> _timedEvents;
    int _nextTimerId;

};

}

#endif
