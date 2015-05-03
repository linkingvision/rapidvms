
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

#ifndef hyperhttp_client_request_h
#define hyperhttp_client_request_h

#include "cppkit/ck_stream_io.h"
#include "cppkit/ck_string.h"
#include "cppkit/ck_memory.h"
#include "cppkit/ck_byte_ptr.h"

#include <unordered_map>
#include <memory>

#include "hyperhttp/hyperuri.h"
#include "hyperhttp/methods.h"

namespace hyperhttp
{

const cppkit::ck_string get_request = "GET";
const cppkit::ck_string post_request = "POST";
const cppkit::ck_string put_request = "PUT";
const cppkit::ck_string delete_request = "DELETE";
const cppkit::ck_string patch_request = "PATCH";

class client_request
{
public:
    CK_API client_request( const cppkit::ck_string& host, int hostPort );

    CK_API client_request( const client_request& rhs );

    CK_API virtual ~client_request() throw();

    CK_API client_request& operator = ( const client_request& rhs );

    CK_API void set_method( int method );

    CK_API bool write_request( std::shared_ptr<cppkit::ck_stream_io> socket ) const;

    CK_API void set_accept_type( const cppkit::ck_string& acceptType );

    CK_API void set_content_type( const cppkit::ck_string& contentType );

    CK_API void set_uri( const uri& res );

    CK_API void set_basic_authentication( const cppkit::ck_string& user, const cppkit::ck_string& pass );

    CK_API void add_post_var( const cppkit::ck_string& name, const cppkit::ck_string& value );

    CK_API void add_header( const cppkit::ck_string& name, const cppkit::ck_string& value );

    CK_API void set_body( const cppkit::ck_byte_ptr& body );
    CK_API void set_body( const cppkit::ck_string& body );

private:
    cppkit::ck_string _get_headers_as_string( const std::shared_ptr<cppkit::ck_stream_io> socket ) const;

    uri _uri;
    cppkit::ck_string _acceptType;
    cppkit::ck_string _contentType;
    int _method;
    cppkit::ck_string _authData;
    std::unordered_map<std::string, cppkit::ck_string> _postVars;
    std::unordered_map<std::string, cppkit::ck_string> _headerParts;
    cppkit::ck_memory _body;
    cppkit::ck_string _host;
    int _hostPort;
};

}

#endif
