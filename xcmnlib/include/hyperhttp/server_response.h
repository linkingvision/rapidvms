
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

#ifndef hyperhttp_server_response_h
#define hyperhttp_server_response_h

#include "cppkit/ck_stream_io.h"
#include "cppkit/ck_string.h"
#include "cppkit/ck_memory.h"
#include <list>
#include <map>

namespace hyperhttp
{

class server_response
{
public:

    enum status_code
    {
        SC_100_Continue = 100,
        SC_101_Switching_Protocols = 101,
        SC_200_OK = 200,
        SC_201_Created = 201,
        SC_202_Accepted = 202,
        SC_204_No_Content = 204,
        SC_400_Bad_Request = 400,
        SC_401_Unauthorized = 401,
        SC_403_Forbidden = 403,
        SC_404_Not_Found = 404,
        SC_406_Not_Acceptable = 406,
        SC_412_Precondition_Failed = 412,
        SC_415_Unsupported_Media_Type = 415,
        SC_418_Im_A_Teapot = 418,
        SC_453_Not_Enough_Bandwidth = 453,
        SC_454_Session_Not_Found = 454,
        SC_500_Internal_Server_Error = 500,
        SC_501_Not_Implemented = 501
    };

    CK_API server_response(status_code status = SC_200_OK,
                          const cppkit::ck_string& contentType = "text/plain");

    CK_API server_response(const server_response& obj);

    CK_API virtual ~server_response() throw();

    CK_API server_response& operator=(const server_response& obj);

    CK_API void set_status_code(status_code status);
    CK_API status_code get_status_code() const;

//
// Possible Content-Type: values...
//     "text/plain"
//     "image/jpeg"
//     "application/xhtml+xml"
//     "application/x-rtsp-tunnelled"
//     "multipart/mixed; boundary="foo"

    CK_API void set_content_type(const cppkit::ck_string& contentType);
    CK_API cppkit::ck_string get_content_type() const;

    CK_API void set_body(const cppkit::ck_string& body);
    CK_API void set_body(const cppkit::ck_memory& body);

    CK_API cppkit::ck_string get_body_as_string() const;
    CK_API cppkit::ck_memory get_body() const;

    CK_API void clear_additional_headers();
    CK_API void add_additional_header(const cppkit::ck_string& headerName,
                                      const cppkit::ck_string& headerValue);
    CK_API cppkit::ck_string get_additional_header(const cppkit::ck_string& headerName);

    CK_API bool write_response(std::shared_ptr<cppkit::ck_stream_io> socket);

    // Chunked transfer encoding support...
    CK_API bool write_chunk(std::shared_ptr<cppkit::ck_stream_io> socket, std::shared_ptr<cppkit::ck_memory> chunk);
    CK_API bool write_chunk_finalizer(std::shared_ptr<cppkit::ck_stream_io> socket);

    // Multipart mimetype support
    // WritePart() will automaticaly add a Content-Length header per
    // part.
    CK_API bool write_part( std::shared_ptr<cppkit::ck_stream_io> socket,
                            const cppkit::ck_string& boundary,
                            const std::map<std::string,cppkit::ck_string>& partHeaders,
                            std::shared_ptr<cppkit::ck_memory> chunk );

    CK_API bool write_part( std::shared_ptr<cppkit::ck_stream_io> socket,
                            const cppkit::ck_string& boundary,
                            const std::map<std::string,cppkit::ck_string>& partHeaders,
                            void* chunk,
                            uint32_t size );

    CK_API bool write_part_finalizer(std::shared_ptr<cppkit::ck_stream_io> socket, const cppkit::ck_string& boundary);

private:
    cppkit::ck_string _get_status_message(status_code sc);
    bool _write_header(std::shared_ptr<cppkit::ck_stream_io> socket);
    bool _send_string(std::shared_ptr<cppkit::ck_stream_io> socket, const cppkit::ck_string& line);
    bool _send_data(std::shared_ptr<cppkit::ck_stream_io> socket, const void* data, size_t dataLen);

    status_code _status;
    cppkit::ck_string _contentType;
    cppkit::ck_memory _body;
    bool _headerWritten;
    std::map<std::string,cppkit::ck_string> _additionalHeaders;
};

}

#endif
