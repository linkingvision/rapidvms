
/// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=--=-=-=-=-=-
/// cppkit - http://www.cppkit.org
/// Copyright (c) 2013, Tony Di Croce
/// All rights reserved.
///
/// Redistribution and use in source and binary forms, with or without modification, are permitted
/// provided that the following conditions are met:
///
/// 1. Redistributions of source code must retain the above copyright notice, this list of conditions and
///    the following disclaimer.
/// 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions
///    and the following disclaimer in the documentation and/or other materials provided with the
///    distribution.
///
/// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
/// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
/// PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
/// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
/// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
/// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
/// TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
/// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
///
/// The views and conclusions contained in the software and documentation are those of the authors and
/// should not be interpreted as representing official policies, either expressed or implied, of the cppkit
/// Project.
/// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=--=-=-=-=-=-

#ifndef cppkit_command_queue_h
#define cppkit_command_queue_h

//#include <condition_variable>
#include <mutex>
#include <list>

using namespace std;

namespace cppkit
{

template<class CMD>
class command_queue
{
    typedef std::unique_lock<std::mutex> guard;

public:
    command_queue() = default;
    command_queue( const command_queue& ) = delete;

    virtual ~command_queue() throw()
    {
        if( started() )
            stop();
    }

    command_queue& operator = ( const command_queue& ) = delete;

    void start()
    {
        guard g( _lock );
        _started = true;
    }

    inline bool started() const
    {
        return _started;
    }

    void stop()
    {
        guard g( _lock );
        _started = false;
        _queue.clear();

        //_cond.notify_one();
    }

    void post( const CMD& cmd )
    {
        guard g( _lock );
        _queue.push_front( cmd );
        //_cond.notify_one();
    }
	
    int size()
    {
        guard g( _lock );
        return _queue.size();
    }

    void wait()
    {
#if 0
        guard g( _lock );
        _cond.wait( g, [this] () { return !this->_queue.empty() || !this->_started; } );
#endif
    }

    CMD pop()
    {
        guard g( _lock );
        auto val = _queue.back();
        _queue.pop_back();
        return val;
    }

private:
    std::mutex _lock;
    //std::condition_variable _cond;
    std::list<CMD> _queue;
    bool _started = false;
};

}

#endif
