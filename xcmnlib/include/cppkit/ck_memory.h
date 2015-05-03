
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

#ifndef cppkit_memory_h
#define cppkit_memory_h

#include "cppkit/os/ck_exports.h"
#include "cppkit/ck_byte_ptr.h"

#include <vector>

namespace cppkit
{

class ck_memory
{
public:
    CK_API ck_memory(size_t size=0);
    CK_API ck_memory(const ck_memory& obj);
    CK_API ck_memory(ck_memory&& obj) noexcept;

    CK_API virtual ~ck_memory() noexcept;

    CK_API ck_memory& operator=(const ck_memory& obj);
    CK_API ck_memory& operator=(ck_memory&& obj) noexcept;

    CK_API void clear();

    CK_API ck_byte_ptr map() const;
    CK_API size_t size() const;
    CK_API void resize(size_t size);

    CK_API ck_byte_ptr extend_data(size_t size);
    CK_API void set_data_size(size_t size);
    CK_API ck_byte_ptr map_data();
    CK_API size_t size_data() const;

    CK_API static std::shared_ptr<ck_memory> clone( std::shared_ptr<ck_memory> src );

private:
    std::vector<uint8_t> _bits;
    size_t _data_sentry;
};

}

#endif

