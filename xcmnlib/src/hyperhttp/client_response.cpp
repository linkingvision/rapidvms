
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

#include "hyperhttp/client_response.h"
#include "hyperhttp/status_codes.h"
#include "hyperhttp/hyperhttp_exception.h"
#include "hyperhttp/hyperhttp_utils.h"
#include "cppkit/ck_socket.h"
#include "cppkit/ck_string.h"

#include <iostream>

using namespace hyperhttp;
using namespace cppkit;
using namespace std;

static const unsigned int MAX_HEADER_LINE = 2048;

client_response::client_response() :
    _initialLine(),
    _headerParts(),
    _bodyContents(new ck_memory),
    _success(false),
    _statusCode(-1),
    _chunkCallback(),
    _partCallback(),
    _chunk(new ck_memory),
    _streaming(false)
{
}

client_response::client_response(const client_response& rhs)
    : _initialLine(rhs._initialLine),
      _headerParts(rhs._headerParts),
      _bodyContents(rhs._bodyContents),
      _success(rhs._success),
      _statusCode(-1),
      _chunkCallback(rhs._chunkCallback),
      _partCallback(rhs._partCallback),
      _chunk(rhs._chunk),
      _streaming(rhs._streaming)
{
}

client_response::~client_response() throw()
{
}

client_response& client_response::operator = (const client_response& rhs)
{
    if(&rhs != this)
    {
        _initialLine = rhs._initialLine;
        _headerParts = rhs._headerParts;
        _bodyContents = rhs._bodyContents;
        _success = rhs._success;
        _statusCode = -1;
        _chunkCallback = rhs._chunkCallback;
        _partCallback = rhs._partCallback;
        _chunk = rhs._chunk;
        _streaming = rhs._streaming;
    }
    return *this;
}

void client_response::read_response(shared_ptr<ck_stream_io> socket)
{
READ_BEGIN:

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

    if(initialLineParts.size() <= 2)
        CK_STHROW(hyperhttp_exception, ("HTTP request initial line doesn't have enough parts."));

    _add_header(ck_string("http_version"), initialLineParts[0]);
    _add_header(ck_string("response_code"), initialLineParts[1]);

    // After response code, we have a message, usually either "OK" or "Not Found", this code appends all the initial line
    // pieces after the first two parts so that we end up with a complete "message".

    ck_string msg = initialLineParts[2];

    for(int i = 3, e = initialLineParts.size(); i < e; ++i)
    {
        msg += " ";
        msg += initialLineParts[i];
    }

    _add_header(ck_string("message"), msg);

    auto i = _headerParts.find( "response_code" );
    if( i != _headerParts.end() )
    {
        _statusCode = (*i).second.front().to_int();
        if(response_ok <= _statusCode && _statusCode < response_multiple_choices)
            _success = true;
    }

    // Handling a "100 continue" initial line, as per http 1.1 spec; we basically
    // just restart... A 100 continue means another complete header and body follows...
    if(msg.to_lower().contains("continue"))
        goto READ_BEGIN;

    _process_request_lines(requestLines);
    _process_body(socket);
}

bool client_response::_receive_data(shared_ptr<ck_stream_io> socket, void* data, size_t dataLen)
{
    const ssize_t bytesSent = socket->recv(data, dataLen);

    return socket->valid() && bytesSent == (ssize_t)dataLen;
}

void client_response::_clean_socket(shared_ptr<ck_stream_io> socket, char** writer)
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

void client_response::_read_header_line(shared_ptr<ck_stream_io> socket, char* writer, bool firstLine)
{
    bool lineDone = false;
    size_t bytesReadThisLine = 0;

    // Get initial header line
    while(!lineDone && (bytesReadThisLine + 1) < MAX_HEADER_LINE)
    {
        if(!_receive_data(socket, writer, 1))
            CK_STHROW(hyperhttp_exception, ("Failed to read data from socket->"));

        ++bytesReadThisLine;

        if(*writer == '\n')
            lineDone = true;

        ++writer;
    }

    if(!lineDone)
    {
        ck_string msg = firstLine ? "The HTTP initial request line exceeded our max."
                                : "The HTTP initial request line exceeded our max.";

        CK_STHROW(hyperhttp_exception, (msg));
    }
}

bool client_response::_add_line(std::list<ck_string>& lines, const ck_string& line)
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

void client_response::_process_request_lines(const list<ck_string>& requestLines)
{
    // Now, iterate on the header lines...

    for(list<ck_string>::const_iterator iter = requestLines.begin(), end = requestLines.end();
        iter != end;
        ++iter)
    {
        const size_t firstColon = iter->find(':');

        if(firstColon != string::npos)
        {
            const ck_string key = iter->substr(0, firstColon);
            const ck_string val = firstColon + 1 < iter->size() ? iter->substr(firstColon + 1) : "";

            _add_header(key.to_lower(), val.strip_eol());
        }
    }
}

void client_response::_process_body(shared_ptr<ck_stream_io> socket)
{
    /// Get the body if we were given a Content Length
    auto found = _headerParts.find( "content-length" );
    if( found != _headerParts.end() )
    {
        const uint32_t contentLength = (*found).second.front().to_uint32();

        if(contentLength > 0)
        {
            if(!_receive_data(socket, _bodyContents->extend_data(contentLength).get_ptr(), contentLength))
                CK_STHROW(hyperhttp_exception, ("Failed to read data from socket->"));
        }
    }
    else if( (found = _headerParts.find( "transfer-encoding" )) != _headerParts.end() )
    {
        if( (*found).second.front().to_lower().contains( "chunked" ) )
        {
            _read_chunked_body(socket);
            return;
        }
    }

    if( (found = _headerParts.find("content-type")) != _headerParts.end() )
    {
        if((*found).second.front().to_lower().contains("multipart"))
        {
            _read_multi_part(socket);
            return;
        }
    }

    if(get_header("content-type").to_lower().contains("multipart"))
        _read_multi_part(socket);
}

const void* client_response::get_body() const
{
    return _bodyContents->map().get_ptr();
}

size_t client_response::get_body_size() const
{
    return _bodyContents->size_data();
}

ck_string client_response::get_body_as_string() const
{
    return ck_string((char*)_bodyContents->map().get_ptr(), _bodyContents->size_data());
}

ck_string client_response::get_header(const ck_string& name) const
{
    auto values = _headerParts.find( name.to_lower() );

    return (values != _headerParts.end()) ? (*values).second.front() : "";
}

vector<ck_string> client_response::get_all_matching_headers(const ck_string& header) const
{
    vector<ck_string> matchingHeaders;

    auto matches = _headerParts.find(header.to_lower());
    if( matches != _headerParts.end() )
    {
        for( auto i = (*matches).second.begin(); i != (*matches).second.end(); ++i )
            matchingHeaders.push_back( *i );
    }

    return matchingHeaders;
}

ck_string client_response::get_message()
{
    return get_header("message");
}

bool client_response::is_success() const
{
    return _success;
}

bool client_response::is_failure() const
{
    return !_success;
}

void client_response::register_chunk_callback( chunk_callback cb, bool streaming )
{
    _chunkCallback = cb;
    _streaming = streaming;
}

void client_response::register_part_callback( part_callback pb )
{
    _partCallback = pb;
}

void client_response::_read_chunked_body(shared_ptr<ck_stream_io> socket)
{
    char lineBuf[MAX_HEADER_LINE+1];
    bool moreChunks = true;

    while(moreChunks)
    {
        memset(lineBuf, 0, MAX_HEADER_LINE+1);

        _read_header_line(socket, lineBuf, false);

        if(lineBuf[0] == '0')
        {
            _consume_footer(socket);
            return;
        }

        if(_embed_null(lineBuf))
        {
            const ck_string chunkLenS = lineBuf;
            const uint32_t chunkLen = chunkLenS.to_uint(16);

            // We read our chunk into a temporary "chunk" ck_memory object, we then optionally
            // call our "chunk callback" function... Finally, we copy the new chunk into the
            // main body contents object.

            _chunk->clear();
            if(!_receive_data(socket, _chunk->extend_data(chunkLen).get_ptr(), chunkLen))
                CK_STHROW(hyperhttp_exception, ("Failed to read data from socket->"));

            // call callback here...
            if( _chunkCallback )
                _chunkCallback( _chunk, *this );

            // We only append a chunk to our "_bodyContents" if we are not streaming (
            // because "streams" potentially have no end, so an ck_memory that contains the
            // complete body contents would just grow forever).
            if(!_streaming)
                memcpy(_bodyContents->extend_data(chunkLen).get_ptr(), _chunk->map().get_ptr(), chunkLen);

            _read_end_of_line(socket);
        }
    }
}

bool client_response::_embed_null(char* lineBuf)
{
    // Scan forward in the chunk header line and embed a NULL on the first non legal chunk size char.
    for(size_t i = 0; i < MAX_HEADER_LINE; ++i)
    {
        if(!_is_legal_chunk_size_char(lineBuf[i]))
        {
            lineBuf[i] = 0;
            return true;
        }
    }

    return false;
}

void client_response::_read_multi_part(shared_ptr<ck_stream_io> socket)
{
    char lineBuf[MAX_HEADER_LINE+1];
    bool moreParts = true;

    while(moreParts)
    {
        // First, read the boundary line...
        memset(lineBuf, 0, MAX_HEADER_LINE);

        _read_header_line(socket, lineBuf, false);

        if(ck_string(lineBuf).strip_eol().ends_with("--"))
            break;

        map<string,ck_string> partHeaders = _read_multi_header_lines(socket, lineBuf);

        auto i = partHeaders.find( "content-length" );
        if( i != partHeaders.end() )
        {
            const int partContentLength = (*i).second.to_int();

            _chunk->clear();

            if(!_receive_data(socket, _chunk->extend_data(partContentLength).get_ptr(), partContentLength))
                CK_STHROW(hyperhttp_exception, ("Failed to read data from socket->"));

            // call callback here...
            if( _partCallback )
                moreParts = _partCallback( _chunk, partHeaders, *this );

            _read_end_of_line(socket);
        }
        else
            CK_STHROW(hyperhttp_exception, ("Oops. Mime multipart without a Content-Length!"));
    }
}


void client_response::_read_end_of_line(shared_ptr<ck_stream_io> socket)
{
    char lineEnd[2] = {0, 0};

    {
        if(!_receive_data(socket, &lineEnd[0], 1))
            CK_STHROW(hyperhttp_exception, ("Failed to read data from socket->"));
    }

    if(lineEnd[0] == '\r')
    {
        if(!_receive_data(socket, &lineEnd[1], 1))
            CK_STHROW(hyperhttp_exception, ("Failed to read data from socket->"));
    }

    if(!_is_end_of_line(lineEnd))
        CK_STHROW(hyperhttp_exception, ("A chunk line didn't end with appropriate terminator."));
}


map<string,ck_string> client_response::_read_multi_header_lines(shared_ptr<ck_stream_io> socket, char* lineBuf)
{
    std::list<ck_string> partLines;

    /// Now, read the rest of the header lines...
    while(true)
    {
        memset(lineBuf, 0, MAX_HEADER_LINE);

        _read_header_line(socket, lineBuf, false);

        if(_add_line(partLines, lineBuf))
            break;
    }

    map<string,ck_string> partHeaders;

    for(auto iter = partLines.begin(), end = partLines.end(); iter != end; ++iter)
    {
        auto lineParts = iter->split(':');
        partHeaders.insert( make_pair( adjust_header_name( lineParts[0] ), adjust_header_value( lineParts[1].strip_eol() ) ) );
    }

    return partHeaders;
}

void client_response::debug_print_request()
{
    ck_string method = get_header(ck_string("method"));
    ck_string response_code = get_header(ck_string("response_code"));
    ck_string http_version = get_header(ck_string("http_version"));
    printf("method = %s\n", method.c_str());
    printf("response_code = %s\n", response_code.c_str());
    printf("http_version = %s\n", http_version.c_str());
    fflush(stdout);
}

void client_response::_consume_footer(shared_ptr<ck_stream_io> socket)
{
    char lineBuf[MAX_HEADER_LINE+1];

    /// Now, read the rest of the header lines...
    while(true)
    {
        memset(lineBuf, 0, MAX_HEADER_LINE);

        _read_header_line(socket, lineBuf, false);

        if(_is_end_of_line(lineBuf))
            break;
    }
}

void client_response::_add_header(const ck_string& name, const ck_string& value)
{
    const ck_string adjName = adjust_header_name(name);
    const ck_string adjValue = adjust_header_value(value);

    auto found = _headerParts.find( adjName );
    if( found != _headerParts.end() )
        (*found).second.push_back( adjValue );
    else
    {
        list<ck_string> values;
        values.push_back(adjValue);
        _headerParts.insert( make_pair( adjName, values ) );
    }
}
