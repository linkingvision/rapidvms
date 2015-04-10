
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

#include "hyperhttp/server_request.h"
#include "hyperhttp/client_request.h"
#include "hyperhttp/hyperhttp_exception.h"
#include "hyperhttp/hyperhttp_utils.h"
#include "hyperhttp/methods.h"
#include "cppkit/ck_socket.h"
#include "cppkit/ck_string.h"

using namespace hyperhttp;
using namespace cppkit;
using namespace std;

static const unsigned int MAX_HEADER_LINE = 16384;

server_request::server_request() :
    _initialLine(),
    _headerParts(),
    _postVars(),
    _body(),
    _contentType()
{
}

server_request::server_request(const server_request& obj) :
    _initialLine(obj._initialLine),
    _headerParts(obj._headerParts),
    _postVars(obj._postVars),
    _body(obj._body),
    _contentType()
{
}

server_request::~server_request() throw()
{
}

server_request& server_request::operator = (const server_request& obj)
{
    _initialLine = obj._initialLine;
    _headerParts = obj._headerParts;
    _postVars = obj._postVars;
    _body = obj._body;
    _contentType = obj._contentType;

    return *this;
}

void server_request::read_request(shared_ptr<cppkit::ck_stream_io> socket)
{
    list<ck_string> requestLines;

    {
        char lineBuf[MAX_HEADER_LINE+1];
        memset(lineBuf, 0, MAX_HEADER_LINE+1);

        {
            char* writer = &lineBuf[0];
            _clean_socket(socket, &writer);

            // Get initial header line
            _read_header_line(socket, writer, true);
        }

        _initialLine = ck_string(lineBuf).strip_eol();

        /// Now, read the rest of the header lines...
        do
        {
            memset(lineBuf, 0, MAX_HEADER_LINE);
            _read_header_line(socket, lineBuf, false);

        } while(!_add_line(requestLines, lineBuf));
    }

    /// Now, populate our header hash...

    _headerParts.clear();

    const vector<ck_string> initialLineParts = _initialLine.split(' ');

    if(initialLineParts.size() != 3)
        CK_STHROW(hyperhttp_exception, ("HTTP request initial line exceeded 3 parts"));

    _set_header("method", initialLineParts[0]);
    _set_header("uri", initialLineParts[1]);
    _set_header("http_version", initialLineParts[2].strip_eol());

    _process_request_lines(requestLines);

    const ck_string method = initialLineParts[0].to_lower();
    if( method == method_text( METHOD_POST ).to_lower() || method == method_text( METHOD_PUT ).to_lower() || method == method_text( METHOD_PATCH ).to_lower() )
        _process_body(socket);
}

bool server_request::is_patch_request() const
{
    return get_method() == METHOD_PATCH;
}

bool server_request::is_post_request() const
{
    return get_method() == METHOD_POST;
}

bool server_request::is_get_request() const
{
    return get_method() == METHOD_GET;
}

bool server_request::is_put_request() const
{
    return get_method() == METHOD_PUT;
}

bool server_request::is_delete_request() const
{
    return get_method() == METHOD_DELETE;
}

int server_request::get_method() const
{
    ck_string method;
    if( get_header( "method", method ) )
        return method_type( method );

    CK_STHROW(hyperhttp_exception, ("Request has no method."));
}

uri server_request::get_uri() const
{
    ck_string u;
    if( get_header( "uri", u ) )
        return uri( u );

    CK_STHROW(hyperhttp_exception, ("Request has no uri."));
}

ck_string server_request::get_content_type() const
{
    return _contentType;
}

void server_request::_set_header(const ck_string& name, const ck_string& value)
{
    const ck_string adjName = adjust_header_name(name);
    const ck_string adjValue = adjust_header_value(value);

    ck_string val;
    if( get_header( adjName, val ) )
        _headerParts.erase( adjName );

    _headerParts.insert( make_pair( (string)adjName, adjValue ) );
}

bool server_request::get_header( const ck_string& key, ck_string& val ) const
{
    auto found = _headerParts.find( key.to_lower() );
    if( found != _headerParts.end() )
    {
        val = (*found).second;
        return true;
    }

    return false;
}

map<string,ck_string> server_request::get_headers() const
{
    return _headerParts;
}

ck_byte_ptr server_request::get_body() const
{
    return _body.map();
}

ck_string server_request::get_body_as_string() const
{
    return ck_string((char*)_body.map().get_ptr(), _body.size_data());
}

map<string,ck_string> server_request::get_post_vars() const
{
    return _postVars;
}

bool server_request::_receive_data(shared_ptr<ck_stream_io> socket, void* data, size_t dataLen)
{
    const ssize_t bytesSent = socket->recv(data, dataLen);

    return socket->valid() && bytesSent == (ssize_t)dataLen;
}

void server_request::_clean_socket(shared_ptr<ck_stream_io> socket, char** writer)
{
    if(!socket->valid())
        CK_STHROW(hyperhttp_exception, ("Invalid Socket"));

    char tempBuffer[1];

    // Clear junk off the socket
    while(true)
    {
        if(!_receive_data(socket, tempBuffer, 1))
            CK_STHROW(hyperhttp_exception, ("Failed to read data from socket->"));

        if(!ck_string::is_space(tempBuffer[0]))
        {
            **writer = tempBuffer[0];
            ++*writer;
            break;
        }
    }
}

void server_request::_read_header_line(shared_ptr<ck_stream_io> socket, char* writer, bool firstLine)
{
    bool lineDone = false;
    size_t bytesReadThisLine = 0;

    // Get initial header line
    while(!lineDone && bytesReadThisLine + 1 < MAX_HEADER_LINE)
    {
        if(!_receive_data(socket, writer, 1))
            CK_STHROW(hyperhttp_exception, ("Failed to read data from socket->"));

        ++bytesReadThisLine;

        if(*writer == '\n')
            lineDone = true;

        ++writer;
    }

    if(!lineDone)
        CK_STHROW(hyperhttp_exception, ("The HTTP initial request line exceeded our max."));
}


bool server_request::_add_line(std::list<ck_string>& lines, const ck_string& line)
{
    if(line.starts_with("\r\n") || line.starts_with("\n"))
        return true;

    if(line.starts_with(" ") || line.starts_with("\t"))
    {
        if(!lines.empty())
            lines.back() += line;
        else
            CK_STHROW(hyperhttp_exception, ("First line of header missing needed seperator."));
    }
    else
        lines.push_back(line);

    return false;
}

void server_request::_process_request_lines(const list<ck_string>& requestLines)
{
    // Now, iterate on the header lines...

    for(auto iter = requestLines.begin(), end  = requestLines.end(); iter != end; ++iter)
    {
        const size_t firstColon = iter->find(':');

        if(firstColon != string::npos)
        {
            const ck_string key = iter->substr(0, firstColon);
            const ck_string val = firstColon + 1 < iter->size() ? iter->substr(firstColon + 1) : "";

            _set_header(key, val.strip_eol());
        }
    }
}

void server_request::_process_body(shared_ptr<ck_stream_io> socket)
{
    ck_string contentLengthString;
    bool foundContentLength = get_header("Content-Length", contentLengthString);

    if(foundContentLength)
    {
        contentLengthString = contentLengthString.strip();
        uint32_t contentLength = contentLengthString.to_uint32();

        if(!contentLength)
            return;

        _body.clear();

        unsigned char* buffer = _body.extend_data(contentLength).get_ptr();

        if(!_receive_data(socket, buffer, contentLength))
            CK_STHROW(hyperhttp_exception, ("Failed to read data from socket->"));

        ck_string contentType;
        bool foundContentType = get_header("Content-Type", contentType);

        if(foundContentType)
        {
            _contentType = contentType.lstrip();

            if(contentType.contains("x-www-form-urlencoded"))
            {
                ck_string rawBody((char*)_body.map().get_ptr(), (int)_body.size_data());

                vector<ck_string> parts = rawBody.split("&");

                for(size_t i = 0; i < parts.size(); ++i)
                {
                    ck_string nvPair = parts[i];

                    vector<ck_string> nameAndValue = nvPair.split("=");

                    if(nameAndValue.size() == 2)
                        _postVars.insert( make_pair( (string)nameAndValue[0], nameAndValue[1] ) );
                }
            }
        }
    }
}
