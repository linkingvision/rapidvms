
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

#ifndef hyperhttp_client_response_h
#define hyperhttp_client_response_h

#include <list>
#include <vector>
#include <map>
#include <functional>
#include "cppkit/ck_stream_io.h"
#include "cppkit/ck_string.h"
#include "cppkit/ck_memory.h"

namespace hyperhttp
{

class client_response;

typedef std::function<bool(std::shared_ptr<cppkit::ck_memory>, const client_response&)> chunk_callback;
typedef std::function<bool(std::shared_ptr<cppkit::ck_memory>, const std::map<std::string, cppkit::ck_string>&, const client_response&)> part_callback;

class client_response
{
public:
    CK_API client_response();

    CK_API client_response(const client_response& obj);

    CK_API virtual ~client_response() throw();

    CK_API client_response& operator = (const client_response& obj);

    CK_API void read_response(std::shared_ptr<cppkit::ck_stream_io> socket);

    CK_API cppkit::ck_string get_message();

    CK_API void debug_print_request();

    CK_API const void* get_body() const;

    CK_API size_t get_body_size() const;

    CK_API cppkit::ck_string get_body_as_string() const;

    CK_API std::shared_ptr<const cppkit::ck_memory> get_body_buffer() const { return _bodyContents; }

    CK_API cppkit::ck_string get_header(const cppkit::ck_string& header) const;

    CK_API std::vector<cppkit::ck_string> get_all_matching_headers(const cppkit::ck_string& header) const;

    CK_API bool is_success() const;

    CK_API bool is_failure() const;

    CK_API int get_status() const { return _statusCode; }

    CK_API void register_chunk_callback( chunk_callback cb, bool streaming = false );
    CK_API void register_part_callback( part_callback pb );

private:
    void _read_chunked_body(std::shared_ptr<cppkit::ck_stream_io> socket);
    void _read_multi_part(std::shared_ptr<cppkit::ck_stream_io> socket);
    std::map<std::string, cppkit::ck_string> _read_multi_header_lines(std::shared_ptr<cppkit::ck_stream_io> socket, char* lineBuf);
    void _read_end_of_line(std::shared_ptr<cppkit::ck_stream_io> socket);

    bool _is_legal_chunk_size_char(char ch) { return isxdigit(ch) ? true : false; } // VS warning: forcing int to bool.
    bool _embed_null(char* lineBuf);
    void _consume_footer(std::shared_ptr<cppkit::ck_stream_io> socket);

    void _add_header(const cppkit::ck_string& name, const cppkit::ck_string& value);

    bool _receive_data(std::shared_ptr<cppkit::ck_stream_io> socket, void* data, size_t dataLen);
    void _clean_socket(std::shared_ptr<cppkit::ck_stream_io> socket, char** writer);
    void _read_header_line(std::shared_ptr<cppkit::ck_stream_io> socket, char* writer, bool firstLine);
    bool _add_line(std::list<cppkit::ck_string>& lines, const cppkit::ck_string& line);
    void _process_request_lines(const std::list<cppkit::ck_string>& requestLines);
    void _process_body(std::shared_ptr<cppkit::ck_stream_io> socket);

    bool _is_end_of_line(char* buffer)
    {
        return (buffer[0] == '\r' && buffer[1] == '\n') || buffer[0] == '\n';
    }

    cppkit::ck_string _initialLine;
    std::map<std::string, std::list<cppkit::ck_string> > _headerParts;
    std::shared_ptr<cppkit::ck_memory> _bodyContents;
    bool _success;
    int  _statusCode;
    chunk_callback _chunkCallback;
    part_callback _partCallback;
    std::shared_ptr<cppkit::ck_memory> _chunk;
    bool _streaming;
};

}

#endif
