
/// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=--=-=-=-=-=-
/// cppkit - http://www.cppkit.org
/// Copyright (c) 2014, Tony Di Croce
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

#ifndef cppkit_pool_h
#define cppkit_pool_h

#include <list>
#include <mutex>
#include <memory>
#include "cppkit/ck_exception.h"

class ck_pool_test;

namespace cppkit
{

// ck_pool is a simple, thread safe object pool. ck_pool is templated on the object type, and
// constructs all of the instances it owns in it's constructor.
//
// ck_pool::get() returns a special shared_ptr that will automagically put the object back in the
// pool when it (and all copies of it) go away.
//
// Because shared_ptr<>'s tend to be passed around, this object is also thread safe (the methods
// (other than the ctor & dtor) that modify the pool grab a lock).

template<class T>
class ck_pool
{
    friend class ::ck_pool_test;
public:
    template<typename... Args>
    ck_pool(size_t num, Args&&... args)
    {
        // create num number of T's with whatever constructor arguments are provided via args.
        for( size_t i = 0; i < num; ++i )
        {
            std::shared_ptr<T> t = std::make_shared<T>( std::forward<Args>(args)... );
            _all.push_back( t );
            _free.push_back( t.get() );
        }
    }

    std::shared_ptr<T> get()
    {
        std::unique_lock<std::recursive_mutex> g( _lok );

        if( _free.empty() )
            CK_THROW(("out of buffers."));

        // Construct a shared_ptr with a custom deleter lambda that puts the pointer back in _free.
        std::shared_ptr<T> f( _free.front(), [this](T* f) {
            std::unique_lock<std::recursive_mutex> g( _lok );
            _free.push_back( f );
        } );

        _free.pop_front();

        return f;
    }

    bool empty() const { return _free.empty(); }

    size_t get_num_free() const { return _free.size(); }

private:
    ck_pool( const ck_pool& );
    ck_pool& operator = ( const ck_pool& );

    std::list<T*> _free;
    std::list<std::shared_ptr<T> > _all;
    std::recursive_mutex _lok;
};

}

#endif
