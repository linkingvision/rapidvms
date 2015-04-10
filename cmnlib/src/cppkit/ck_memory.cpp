
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

#include "cppkit/ck_memory.h"

using namespace std;
using namespace cppkit;

ck_memory::ck_memory(size_t size) :
    _bits(size),
    _data_sentry(0)
{
}

ck_memory::ck_memory(const ck_memory& obj) :
    _bits(obj._bits),
    _data_sentry(obj._data_sentry)
{
}

ck_memory::ck_memory(ck_memory&& obj) noexcept :
    _bits(std::move(obj._bits)),
    _data_sentry(obj._data_sentry)
{
}

ck_memory::~ck_memory() noexcept
{
}

ck_memory& ck_memory::operator=(const ck_memory& obj)
{
    _bits=obj._bits;
    _data_sentry=obj._data_sentry;
    return *this;
}

ck_memory& ck_memory::operator=(ck_memory&& obj) noexcept
{
    _bits=std::move(obj._bits);
    _data_sentry=obj._data_sentry;
    return *this;
}

void ck_memory::clear()
{
    set_data_size( 0 );
}

ck_byte_ptr ck_memory::map() const
{
    return ck_byte_ptr(&_bits[0], _bits.size());
}

size_t ck_memory::size() const
{
    return _bits.size();
}

void ck_memory::resize(size_t size)
{
    _bits.resize(size);
    if(_data_sentry > size)
        _data_sentry=size;
}

ck_byte_ptr ck_memory::extend_data(size_t size)
{
    size_t current_size = _bits.size();
    size_t current_data_sentry = _data_sentry;
    size_t free_size = (current_size - current_data_sentry);

    size_t extend_size = (free_size >= size) ? 0 : (size - free_size);

    if(extend_size > 0)
        _bits.resize(current_size + extend_size);

    _data_sentry+=size;

    return ck_byte_ptr(&_bits[current_data_sentry], size);
}

void ck_memory::set_data_size(size_t size)
{
    if(size > _bits.size())
        resize(size);

    _data_sentry=size;
}

ck_byte_ptr ck_memory::map_data()
{
    return ck_byte_ptr(&_bits[0], _data_sentry);
}

size_t ck_memory::size_data() const
{
    return _data_sentry;
}

shared_ptr<ck_memory> ck_memory::clone( shared_ptr<ck_memory> src )
{
    shared_ptr<ck_memory> dst = make_shared<ck_memory>();
    size_t srcSize = src->size_data();
    memcpy( dst->extend_data( srcSize ).get_ptr(), src->map().get_ptr(), srcSize );
    return dst;
}
