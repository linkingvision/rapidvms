
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef XSDK_XCommandQueue_h
#define XSDK_XCommandQueue_h

#include "XSDK/XTaskBase.h"
#include "XSDK/XMutex.h"
#include "XSDK/XGuard.h"
#include "XSDK/XCondition.h"
#include "XSDK/XListBox.h"
#include "XSDK/XException.h"
#include <list>


namespace XSDK
{

// XCommandQueue
// This class implements a "work queue" and a "worker" thread. The essential idea is that if you have a
// thread that needs to run in response to the arrival of some command, then that thread can be waiting
// on a condition variable all the rest of the time.
//
// Note: The template argument here is the actual command type. Since this class uses an stl container
// internally the command type needs to be "copyable" (I.E., implement assignment op and copy ctor). A
// very typical use is probably to fill the queue with XIRef<>'s to "Command" objects.

template< class T >
class XCommandQueue : public XTaskBase
{
public:

    XCommandQueue( bool threaded = true, const XString& threadName = DEFAULT_THREAD_NAME ) :
        XTaskBase( threadName ),
        _queueLock(),
        _queueCond( _queueLock ),
        _queueS(),
        _queue( _queueS, Cmp, this ),
        _running( false ),
        _threaded( threaded )
    {
    }

    virtual ~XCommandQueue() throw()
    {
        if( _running )
            Shutdown();
    }

    static int Cmp( const T& a, const T& b, void* compareContext )
    {
        if( compareContext == NULL )
            X_THROW(( "XCommandQueue comparison function NEEDS valid context." ));

        return ((XCommandQueue<T>*)compareContext)->Compare( a, b );
    }

    virtual void Startup()
    {
        XGuard guard( _queueLock );

        if( _running )
            X_THROW(( "Startup() called on already running XCommandQueue." ));

        _running = true;

        if( _threaded )
        {
            Start();
        }
    }

    virtual void Shutdown()
    {
        if( _threaded )
        {
            // Note: guard must go out of scope (and hence, release the _queueLock) before we call Join() or else
            // the queue thread will never be awoken from wait.
            {
                XGuard guard( _queueLock );

                if( !_running )
                    X_THROW(( "Shutdown() called on non running XCommandQueue." ));

                _running = false;

                _queueCond.Signal();
            }

            // Now that the guard above is destroyed, it's safe to join.
            Join();
        }
        else
        {
            XGuard guard( _queueLock );

            _running = false;
        }
    }

    void Post( T type )
    {
        XGuard guard( _queueLock );

        if( !_running )
            X_THROW(( "A command was posted to a non running XCommandQueue." ));

        if( _threaded )
        {
            if( !Peek( type ) )
            {
                _queue.InsertSorted( type );
                //_queueS.push_back( type );

                _queueCond.Signal();
            }
        }
        else
        {
            if( !Peek( type ) )
                X_THROW(( "A non threaded XCommandQueue::Peek() returned false." ));
        }
    }

    virtual bool Peek( T type )
    {
#ifdef WIN32
    type;
#endif
        return false;
    }

    virtual bool Process( T type )
    {
#ifdef WIN32
        type;
#endif
        return true;
    }

    // If a subclass wishes to
    virtual int Compare( const T& a, const T& b )
    {
#ifdef WIN32
    a;
    b;
#endif
        return -1;
    }

    virtual void* EntryPoint()
    {

        while( _running )
        {
            try
            {
                T type;

                // Arbitrary scope here so that the guard goes away and releases the _queueLock WHILE
                // we're calling "Process( type )". This allows the queue to accept new T's WHILE we're
                // processing a T.
                {
                    XGuard guard( _queueLock );

                    while( _queueS.empty() && _running )
                        _queueCond.Wait();

                    if( !_running )
                        continue;

                    type = _queueS.front();
                    _queueS.pop_front();
                }

                if( !Process( type ) )
                {
                    XGuard guard( _queueLock );

                    _queueS.push_front( type );
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

    // Warning:
    // This method provides unguarded (I.E. no mutex is acquired) access to the underlying
    // queue inside XCommandQueue. It should probably only be used in very RARE circumstances.
    std::list<T>& UnguardedQueue()
    {
        return _queueS;
    }

 protected:

    XMutex _queueLock;
    XCondition _queueCond;
    std::list<T> _queueS;
    XSDK::XListBox<T> _queue;
    bool _running;
    bool _threaded;

 private:
//    static int _CompareS( const T& a, const T% b )
};

}

#endif
