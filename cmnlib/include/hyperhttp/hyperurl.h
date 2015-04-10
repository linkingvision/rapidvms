
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

#ifndef hyperhttp_url_h
#define hyperhttp_url_h

#include "cppkit/ck_nullable.h"
#include "cppkit/ck_string.h"
#include "cppkit/ck_string_slice.h"
#include "hyperhttp/hyperuri.h"

namespace hyperhttp
{

/// \brief Similar to URI except that it also has a protocol, IP, and port.
///
/// The IP can be a hostname rather than an IP literal.
///
/// The protocol is required, and if a port is provided, the IP is required.
/// Otherwise, the IP is optional, and the URI portion defaults to "/" if none
/// is provided.
///
/// "http://www.google.com/"
/// "http://127.0.0.1/hello/world?foo=bar"
/// "rtsp://[fc00:1:2:10:8a51:fbff:fec6:d992]:57342/"
/// "file:///etc/fstab"
class url
{
public:

    CK_API url() : _protocol("SetMe"), _ip("0.0.0.0"), _port(), _uri() {}

    CK_API url(const cppkit::ck_string& protocol, const cppkit::ck_string ip, cppkit::ck_nullable<int> port, const uri& uri);

    CK_API url(const cppkit::ck_string& protocol, const cppkit::ck_string ip, const uri& uri);

    CK_API url(const cppkit::ck_string& url);

    /// So string literals work when constructing with =
    CK_API url(const char* cstr) : _protocol(), _ip(), _port(), _uri() { *this = url(cppkit::ck_string(cstr)); }

    CK_API virtual ~url() throw() {}

    CK_API bool operator==(const url& rhs)
    { return _protocol == rhs._protocol && _ip == rhs._ip && _port == rhs._port && _uri == rhs._uri ; }

    CK_API bool operator!=(const url& rhs) { return !(*this == rhs); }

    CK_API cppkit::ck_string get_protocol() const { return _protocol; }
    CK_API void set_protocol(const cppkit::ck_string protocol);

    CK_API cppkit::ck_string get_ip() const { return _ip; }
    CK_API void set_ip(const cppkit::ck_string ip);

    CK_API cppkit::ck_nullable<int> get_port_num() const { return _port; }
    CK_API void set_port_num(const cppkit::ck_nullable<int> port);

    CK_API const uri& get_uri() const { return _uri; }
    CK_API void set_uri(const uri& a) { _uri = a; }

    CK_API cppkit::ck_string get_full_raw_url() const;

    CK_API const std::map<std::string, cppkit::ck_string>& get_get_args() const { return _uri.get_get_args(); }

    CK_API void add_get_arg(const cppkit::ck_string& name, const cppkit::ck_string& value) { _uri.add_get_arg(name, value); }

    CK_API void clear_get_args() { _uri.clear_get_args(); }

    /// \brief Whether protocol is a legal protocol in a url per RFC 3986.
    CK_API static bool is_valid_protocol(const cppkit::ck_string& str);

    /// \brief Creates a URL from a string which may represent an "absolute" url
    ///        or a relative one.
    ///
    /// If the given url string has a protocol on it, then the url is created
    /// directly from it, but if it doesn't have one, then the string is
    /// treated as the uri portion of the url, and the given protocol, IP, and
    /// port are used.
    CK_API static url make_absolute(const cppkit::ck_string& protocol, const cppkit::ck_string& ip, int port, const cppkit::ck_string& url)
    { return _make_absolute(protocol, ip, port, url); }

    CK_API static url make_absolute(const cppkit::ck_string& protocol, const cppkit::ck_string& ip, const cppkit::ck_string& url)
    { return _make_absolute(protocol, ip, cppkit::ck_nullable<int>(), url); }

private:

    CK_API void _set_ip_and_port(cppkit::ck_string_slice ipAndPort, const cppkit::ck_string& fullURL);

    CK_API static url _make_absolute(const cppkit::ck_string& protocol, const cppkit::ck_string& ip,
                                     cppkit::ck_nullable<int> port, const cppkit::ck_string& url);

    cppkit::ck_string _protocol;
    cppkit::ck_string _ip;
    cppkit::ck_nullable<int> _port;
    uri _uri;
};

}

#endif
