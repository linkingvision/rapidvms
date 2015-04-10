
#include "cppkit/ck_timer.h"
#include <ctime>

using namespace cppkit;
using namespace std;
using namespace std::chrono;

ck_timer::ck_timer( size_t intervalMillis, ck_timer_cb cb ) :
    _thread(),
    _intervalMillis( intervalMillis ),
    _cb( cb ),
    _lok(),
    _cond(),
    _started( false )
{
}

ck_timer::~ck_timer() throw()
{
    if( _started )
        stop();
}

void ck_timer::start()
{
    if( !_started )
    {
        _started = true;
        _thread = std::thread( &ck_timer::_timer_loop, this );
    }
}

void ck_timer::stop()
{
    if( _started )
    {
        {
            unique_lock<recursive_mutex> g( _lok );
            _started = false;
            _cond.notify_one();
        }

        _thread.join();
    }
}

void ck_timer::_timer_loop()
{
    size_t millisToWait = _intervalMillis;

    while( _started )
    {
        unique_lock<recursive_mutex> g( _lok );

        steady_clock::time_point before = steady_clock::now();

        auto result = (millisToWait>0)?_cond.wait_for( g, chrono::milliseconds( millisToWait ) ) : cv_status::timeout;

        steady_clock::time_point after = steady_clock::now();

        if( !_started )
            continue;

        size_t waitedMillis = chrono::duration_cast<chrono::milliseconds>(after - before).count();

        if( result == cv_status::timeout )
        {
            millisToWait = _intervalMillis;
            _cb();
        }
        else
        {
            if( millisToWait > waitedMillis )
                millisToWait -= waitedMillis;
            else millisToWait = 0;
        }
    }
}
