
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

#include "hyperhttp/client_request.h"
#include "hyperhttp/hyperhttp_exception.h"
#include "hyperhttp/hyperhttp_utils.h"
#include "cppkit/ck_socket.h"
#include "cppkit/ck_string.h"

#include <iostream>

using namespace hyperhttp;
using namespace cppkit;
using namespace std;

static const unsigned int MAX_HEADER_LINE = 2048;

client_request::client_request( const ck_string& host, int hostPort ) :
    _uri(),
    _acceptType(),
    _contentType(),
    _method( METHOD_GET ),
    _authData(),
    _postVars(),
    _headerParts(),
    _body(),
    _host( host ),
    _hostPort( hostPort )
{
}

client_request::client_request(const client_request& rhs) :
    _uri(rhs._uri),
    _acceptType(rhs._acceptType),
    _contentType(rhs._contentType),
    _method(rhs._method),
    _authData(rhs._authData),
    _postVars(rhs._postVars),
    _headerParts(rhs._headerParts),
    _body(rhs._body),
    _host(rhs._host),
    _hostPort(rhs._hostPort)
{
}

client_request::~client_request() throw()
{
}

client_request& client_request::operator = (const client_request& rhs)
{
    if (this != &rhs)
    {
        _uri = rhs._uri;
        _acceptType = rhs._acceptType;
        _contentType = rhs._contentType;
        _method = rhs._method;
        _authData = rhs._authData;
        _postVars = rhs._postVars;
        _headerParts = rhs._headerParts;
        _body = rhs._body;
        _host = rhs._host;
        _hostPort = rhs._hostPort;
    }

    return *this;
}

void client_request::set_method( int method )
{
    _method = method;
}

bool client_request::write_request( shared_ptr<ck_stream_io> socket ) const
{
    ck_string msgHeader = _get_headers_as_string( socket );

    ssize_t bytesWritten = socket->send( msgHeader.c_str(), msgHeader.length() );
    if( !socket->valid() || (bytesWritten != (int32_t)msgHeader.length()) )
        return false;

    if( (_method == METHOD_POST || _method == METHOD_PATCH || _method == METHOD_PUT) &&
        !_contentType.contains("x-www-form-urlencoded") &&
        _body.size_data() )
    {
        bytesWritten = socket->send(_body.map(), _body.size_data());
        if(!socket->valid() || (bytesWritten != (int32_t)_body.size_data()))
            return false;
    }

    return true;
}

void client_request::set_accept_type( const ck_string& acceptType )
{
    _acceptType = acceptType;
}

void client_request::set_content_type(const ck_string& contentType )
{
    _contentType = contentType;
}

void client_request::set_uri( const uri& res )
{
    _uri = res;
}

void client_request::set_basic_authentication( const ck_string& user, const ck_string& pass )
{
    const ck_string auth = ck_string::format( "%s:%s", user.c_str(), pass.c_str() );

    _authData = ck_string::to_base64( auth.c_str(), auth.length() );
}

void client_request::add_post_var( const ck_string& name, const ck_string& value )
{
    _contentType = "x-www-form-urlencoded";
    _postVars[name] = value;
}

void client_request::add_header( const ck_string& name, const ck_string& value )
{
    _headerParts[adjust_header_name( name )] = adjust_header_value( value );
}

void client_request::set_body( const ck_byte_ptr& body )
{
    _body.clear();

    memcpy( _body.extend_data( body.length() ).get_ptr(), body.get_ptr(), body.length() );
}

void client_request::set_body( const cppkit::ck_string& body )
{
    ck_byte_ptr src( body.c_str(), body.length() );

    set_body( src );
}

ck_string client_request::_get_headers_as_string( const shared_ptr<ck_stream_io> socket ) const
{
    ck_string msgHeader;
    msgHeader = method_text( _method ) + " " + _uri.get_full_raw_uri() + " HTTP/1.1\r\nHost: " + _host + ":" + ck_string::from_int( _hostPort ) + "\r\n";

    if (!_acceptType.empty())
        msgHeader += "Accept: " + _acceptType + "\r\n";

    if (!_contentType.empty())
        msgHeader += "Content-Type: " + _contentType + "\r\n";

    if (!_authData.empty())
        msgHeader += "Authorization: Basic " + _authData + "\r\n";

    for( auto& kv : _headerParts )
    {
        msgHeader += ck_string::format( "%s: %s\r\n", kv.first.c_str(), kv.second.c_str() );
    }

    if( _method == METHOD_POST || _method == METHOD_PATCH || _method == METHOD_PUT )
    {
        if(_contentType.contains("x-www-form-urlencoded"))
        {
            // Post, with url-encoded nv pairs...

            ck_string body;

            for( auto& kv : _postVars )
            {
                body += ck_string::format( "%s=%s&", kv.first.c_str(), kv.second.c_str() );
            }

            body = body.substr( 0, body.length() - 1 );

            msgHeader += ck_string::format( "Content-Length: %d\r\n\r\n", body.length() );
            msgHeader += body;
        }
        else
        {
            // Post, with an unknown body (this is the SOAP case)...

            if( _body.size_data() )
            {
                // If we have a body, add a Content-Length and write it...
                msgHeader += ck_string::format("Content-Length: %d\r\n\r\n", _body.size_data());
            }
            else
            {
                // No body in this request, so just write this...
                msgHeader += "\r\n";
            }
        }
    }
    else
    {
        // This is the GET case...

        msgHeader += "\r\n";
    }

    return msgHeader;
}
