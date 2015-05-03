
/// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=--=-=-=-=-=-
/// hyperhttp - http://www.cppkit.org
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

#ifndef hyperhttp_server_request_h
#define hyperhttp_server_request_h

#include <list>
#include <map>
#include <memory>
#include "cppkit/ck_stream_io.h"
#include "cppkit/ck_string.h"
#include "cppkit/ck_memory.h"
#include "cppkit/ck_byte_ptr.h"
#include "hyperhttp/hyperuri.h"
#include "hyperhttp/methods.h"

namespace hyperhttp
{

class server_request
{
public:
    CK_API server_request();

    CK_API server_request(const server_request& rhs);

    CK_API virtual ~server_request() throw();

    CK_API server_request& operator = (const server_request& rhs);

    CK_API void read_request(std::shared_ptr<cppkit::ck_stream_io> socket);

    CK_API int get_method() const;

    CK_API uri get_uri() const;

    CK_API cppkit::ck_string get_content_type() const;

    CK_API bool get_header( const cppkit::ck_string& key, cppkit::ck_string& val ) const;
    CK_API std::map<std::string,cppkit::ck_string> get_headers() const;

    CK_API cppkit::ck_byte_ptr get_body() const;
    CK_API cppkit::ck_string get_body_as_string() const;

    CK_API std::map<std::string,cppkit::ck_string> get_post_vars() const;

    CK_API bool is_patch_request() const;
    CK_API bool is_put_request() const;
    CK_API bool is_post_request() const;
    CK_API bool is_get_request() const;
    CK_API bool is_delete_request() const;

private:
    void _set_header(const cppkit::ck_string& name, const cppkit::ck_string& value);
    bool _receive_data(std::shared_ptr<cppkit::ck_stream_io> socket, void* data, size_t dataLen);
    void _clean_socket(std::shared_ptr<cppkit::ck_stream_io> socket, char** writer);
    void _read_header_line(std::shared_ptr<cppkit::ck_stream_io> socket, char* writer, bool firstLine);
    bool _add_line(std::list<cppkit::ck_string>& lines, const cppkit::ck_string& line);
    void _process_request_lines(const std::list<cppkit::ck_string>& requestLines);
    void _process_body(std::shared_ptr<cppkit::ck_stream_io> socket);

    cppkit::ck_string _initialLine;
    std::map<std::string,cppkit::ck_string> _headerParts;
    std::map<std::string,cppkit::ck_string> _postVars;
    cppkit::ck_memory _body;
    cppkit::ck_string _contentType;
};

}

#endif
