
#include "XCommandQueueTest.h"
#include "XSDK/OS/STL.h"
#include "XSDK/TimeUtils.h"
#include "XSDK/XMutex.h"
#include "XSDK/XGuard.h"
#include "XSDK/XCondition.h"

using namespace XSDK;

REGISTER_TEST_FIXTURE(XCommandQueueTest);

static int _globalInt = 0;
static XMutex _processLock;
static XCondition _processCond( _processLock );

SimpleCommandQueue::SimpleCommandQueue()
{
}

SimpleCommandQueue::~SimpleCommandQueue() throw()
{
}

bool SimpleCommandQueue::Process( int type )
{
    XGuard guard(_processLock);
    _globalInt = type;
    _processCond.Signal();

    return true;
}

const int CMD_COUNT = 5;
static int _globalInts[CMD_COUNT] = { 0, 0, 0, 0, 0 };

PriorityCommandQueue::PriorityCommandQueue()
{
}

PriorityCommandQueue::~PriorityCommandQueue() throw()
{
}

bool PriorityCommandQueue::Process( int type )
{
    static int counter = 0;

    XGuard guard(_processLock);

    _globalInts[counter] = type;

    if( counter == (CMD_COUNT-1) )
        _processCond.Signal();

    counter++;

    return true;
}

int PriorityCommandQueue::Compare( const int& a, const int& b )
{
    if( a < b )
        return -1;
    if( a == b )
        return 0;
    return 1;
}

void XCommandQueueTest::TestCreateDestroy()
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

void XCommandQueueTest::TestRoundTripCommand()
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

void XCommandQueueTest::TestLots()
{
    try
    {
        _q.Startup();

        for( int i = 0; i < 5000; i++ )
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

void XCommandQueueTest::TestPriorityCommandQueue()
{
    try
    {
        _pq.Startup();

        {
            XGuard guard( _processLock );

            // We put them in out of order, then below test that
            // they came out it order.
            _pq.Post( 1 );
            _pq.Post( 2 );
            _pq.Post( 5 );
            _pq.Post( 4 );
            _pq.Post( 3 );

            _processCond.Wait();
        }

        UT_ASSERT( _globalInts[0] == 1 );
        UT_ASSERT( _globalInts[1] == 2 );
        UT_ASSERT( _globalInts[2] == 3 );
        UT_ASSERT( _globalInts[3] == 4 );
        UT_ASSERT( _globalInts[4] == 5 );

        _pq.Shutdown();
    }

    catch(XSDK::XException&)
    {
        UT_ASSERT(false && "Test threw!");
    }
}
