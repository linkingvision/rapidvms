
#include "XTimedCommandQueueTest.h"
#include "XSDK/OS/STL.h"
#include "XSDK/TimeUtils.h"
#include "XSDK/XString.h"
#include "XSDK/XMutex.h"
#include "XSDK/XGuard.h"
#include "XSDK/XCondition.h"
//
REGISTER_TEST_FIXTURE(XTimedCommandQueueTest);

using namespace XSDK;

int _globalInt = 0;
XMutex _processLock;
XCondition _processCond( _processLock );

SimpleTimedCommandQueue::SimpleTimedCommandQueue() :
    XTimedCommandQueue<int>()
{
}

SimpleTimedCommandQueue::~SimpleTimedCommandQueue() throw()
{
}

bool SimpleTimedCommandQueue::Process( int type )
{
    XSDK::XGuard guard(_processLock);
    _globalInt = type;
    _processCond.Signal();

    return true;
}

void SimpleTimedCommandQueue::TimerCallback( void* context, int id )
{
    XSDK::XGuard guard(_processLock);
    _globalInt = id;
    _processCond.Signal();
}

void XTimedCommandQueueTest::TestCreateDestroy()
{
    try
    {
        _q.Startup();
        _q.Shutdown();
    }

    catch(XSDK::XException&)
    {
        UT_ASSERT(false && "Test threw!");
    }
}

void XTimedCommandQueueTest::TestRoundTripCommand()
{
    try
    {
        _q.Startup();

        UT_ASSERT( _globalInt == 0 );

        {
            XGuard guard( _processLock );

            _q.Post( 42 );

            _processCond.Wait();
        }

        UT_ASSERT( _globalInt == 42 );

        _q.Shutdown();
    }

    catch(XSDK::XException&)
    {
        UT_ASSERT(false && "Test threw!");
    }
}

void XTimedCommandQueueTest::TestLots()
{
    try
    {
        _q.Startup();

        for( int i = 0; i < 30000; i++ )
        {
            XGuard guard( _processLock );

            _q.Post( i );

            _processCond.Wait();

            UT_ASSERT( _globalInt == i );
        }

        _q.Shutdown();
    }

    catch(XSDK::XException&)
    {
        UT_ASSERT(false && "Test threw!");
    }
}

void XTimedCommandQueueTest::TestOneSecondTimer()
{
   try
    {
        _q.Startup();

        {
            XGuard guard( _processLock );

            int timerId = _q.CreateTimedEvent( (time_t)1, (time_t)0, SimpleTimedCommandQueue::TimerCallback, &_q, false );

            _processCond.Wait();

            UT_ASSERT( timerId == _globalInt );
        }

        _q.Shutdown();
    }

    catch(XSDK::XException&)
    {
        UT_ASSERT(false && "Test threw!");
    }

}


void XTimedCommandQueueTest::TestMultipleTimers()
{
   try
    {
        _q.Startup();

        {
            XGuard guard( _processLock );

            int timerId[10];
            timerId[3] = _q.CreateTimedEvent( (time_t)1, (time_t)750000, SimpleTimedCommandQueue::TimerCallback, &_q, false );
            timerId[0] = _q.CreateTimedEvent( (time_t)1, (time_t)0, SimpleTimedCommandQueue::TimerCallback, &_q, false );
            timerId[5] = _q.CreateTimedEvent( (time_t)3, (time_t)100000, SimpleTimedCommandQueue::TimerCallback, &_q, false );
            timerId[6] = _q.CreateTimedEvent( (time_t)3, (time_t)200000, SimpleTimedCommandQueue::TimerCallback, &_q, false );
            timerId[1] = _q.CreateTimedEvent( (time_t)1, (time_t)100000, SimpleTimedCommandQueue::TimerCallback, &_q, false );
            timerId[9] = _q.CreateTimedEvent( (time_t)6, (time_t)0, SimpleTimedCommandQueue::TimerCallback, &_q, false );
            timerId[2] = _q.CreateTimedEvent( (time_t)1, (time_t)500000, SimpleTimedCommandQueue::TimerCallback, &_q, false );
            timerId[8] = _q.CreateTimedEvent( (time_t)5, (time_t)0, SimpleTimedCommandQueue::TimerCallback, &_q, false );
            timerId[4] = _q.CreateTimedEvent( (time_t)3, (time_t)0, SimpleTimedCommandQueue::TimerCallback, &_q, false );
            timerId[7] = _q.CreateTimedEvent( (time_t)3, (time_t)400000, SimpleTimedCommandQueue::TimerCallback, &_q, false );

            for(int i=0; i < 10; i++)
            {
                _processCond.Wait();
                UT_ASSERT( timerId[i] == _globalInt );
            }
        }

        _q.Shutdown();
    }

    catch(XSDK::XException&)
    {
        UT_ASSERT(false && "Test threw!");
    }

}


void XTimedCommandQueueTest::TestCancelTimer()
{
   try
    {
        _q.Startup();

        {
            XGuard guard( _processLock );

            int timerId[2];
            timerId[1] = _q.CreateTimedEvent( (time_t)2, (time_t)0, SimpleTimedCommandQueue::TimerCallback, &_q, false );
            timerId[0] = _q.CreateTimedEvent( (time_t)1, (time_t)0, SimpleTimedCommandQueue::TimerCallback, &_q, false );

            _q.CancelTimedEvent( timerId[0] );

            {
                _processCond.Wait();
                UT_ASSERT( timerId[1] == _globalInt );
            }
        }

        _q.Shutdown();
    }

    catch(XSDK::XException&)
    {
        UT_ASSERT(false && "Test threw!");
    }

}

void XTimedCommandQueueTest::TestRepeatingTimer()
{
   try
    {
        _q.Startup();

        {
            XGuard guard( _processLock );

            int timerId = _q.CreateTimedEvent( (time_t)0, (time_t)100000, SimpleTimedCommandQueue::TimerCallback, &_q, true );

            for(int i = 0; i < 8; i++)
            {
                _processCond.Wait();
                UT_ASSERT( timerId == _globalInt );
            }

            _q.CancelTimedEvent( timerId );
        }

        _q.Shutdown();
    }

    catch(XSDK::XException&)
    {
        UT_ASSERT(false && "Test threw!");
    }

}

void XTimedCommandQueueTest::TestUltimate()
{
   try
    {
        _q.Startup();

        {
            XGuard guard( _processLock );

            int repeatingTimerId = _q.CreateTimedEvent( (time_t)0, (time_t)50000, SimpleTimedCommandQueue::TimerCallback, &_q, true );

            int oneShotTimerId = _q.CreateTimedEvent( (time_t)3, (time_t)0, SimpleTimedCommandQueue::TimerCallback, &_q, false );

            _q.Post( 42 );

            _processCond.Wait();

            UT_ASSERT( _globalInt == 42 );

            for(int i = 0; i < 4; i++)
            {
                _processCond.Wait();
                UT_ASSERT( repeatingTimerId == _globalInt );
            }

            _q.CancelTimedEvent( repeatingTimerId );

            _q.Post( 42 );

            _processCond.Wait();

            UT_ASSERT( _globalInt == 42 );

            _processCond.Wait();

            UT_ASSERT( oneShotTimerId == _globalInt );

            _q.Post( 42 );

            _processCond.Wait();

            UT_ASSERT( _globalInt == 42 );
        }

        _q.Shutdown();
    }

    catch(XSDK::XException&)
    {
        UT_ASSERT(false && "Test threw!");
    }

}

void XTimedCommandQueueTest::TestAccessContext()
{
   try
    {
        XString foo = "Bar";
        _q.Startup();

        {
            XGuard guard( _processLock );

            int qId = _q.CreateTimedEvent( (time_t)0, (time_t)50000, SimpleTimedCommandQueue::TimerCallback, &_q, true );

            int fooId = _q.CreateTimedEvent( (time_t)3, (time_t)0, SimpleTimedCommandQueue::TimerCallback, &foo, false );

            UT_ASSERT( _q.AccessContext(qId) == &_q );


            UT_ASSERT( _q.AccessContext(fooId) == &foo  );

        }

        _q.Shutdown();
    }

    catch(XSDK::XException&)
    {
        UT_ASSERT(false && "Test threw!");
    }

}














