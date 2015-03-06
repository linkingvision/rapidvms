
#ifndef __ffkit_locky_h
#define __ffkit_locky_h

#include <mutex>
#include <list>
#include <memory>

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
};

namespace ffkit
{

class locky
{
public:
    static void register_ffmpeg();
    static void unregister_ffmpeg();
private:
    locky();
    locky( const locky& obj );
    locky& operator = ( const locky& obj );

    static std::recursive_mutex* _create_lock();
    static void _destroy_lock( std::recursive_mutex* lock );

    static int _locky_cb( void** mutex, enum AVLockOp op );

    static std::list<std::shared_ptr<std::recursive_mutex> > _locks;
};

}

#endif
