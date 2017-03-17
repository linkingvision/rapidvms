
#include "ffkit/fflocky.h"
#include "cppkit/ck_logger.h"

using namespace ffkit;
using namespace std;
using namespace cppkit;

list<shared_ptr<recursive_mutex> > locky::_locks;

void locky::register_ffmpeg()
{
    av_register_all();
    avfilter_register_all();
    av_log_set_level(AV_LOG_PANIC);
    //av_log_set_level(AV_LOG_INFO);
    //av_lockmgr_register( locky::_locky_cb );
}

void locky::unregister_ffmpeg()
{
    //av_lockmgr_register( NULL );

    //locky::_locks.clear();
}

recursive_mutex* locky::_create_lock()
{
    shared_ptr<recursive_mutex> newLock = make_shared<recursive_mutex>();
    recursive_mutex* newLockPtr = newLock.get();

    locky::_locks.push_back( newLock );

    return newLockPtr;
}

void locky::_destroy_lock( recursive_mutex* lock )
{
    for( auto i = _locks.begin(), end = _locks.end(); i != end; i++ )
    {
        auto lockRef = *i;
        if( lockRef.get() == lock )
        {
            _locks.erase( i );
            return;
        }
    }

    CK_LOG_NOTICE( "Locky asked to clean up a lock that was not managed!" );
}

int locky::_locky_cb( void** mutex, enum AVLockOp op )
{
    switch( op )
    {
    case AV_LOCK_CREATE:
        *mutex = locky::_create_lock();
        break;
    case AV_LOCK_DESTROY:
        locky::_destroy_lock( (recursive_mutex*)*mutex );
        break;
    case AV_LOCK_OBTAIN:
        ((recursive_mutex*)(*mutex))->lock();
        break;
    case AV_LOCK_RELEASE:
        ((recursive_mutex*)(*mutex))->unlock();
        break;
    default:
        break;
    };

    return 0;
}
