
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#ifndef _Webby_URL_h_
#define _Webby_URL_h_

#include "XSDK/XNullable.h"
#include "XSDK/XString.h"
#include "XSDK/XStringSlice.h"
#include "Webby/URI.h"

namespace WEBBY
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
class URL
{
public:

    X_API URL() : _protocol("SetMe"), _ip("0.0.0.0"), _port(), _uri() {}

    X_API URL(const XSDK::XString& protocol, const XSDK::XString ip, XSDK::XNullable<int> port, const URI& uri);

    X_API URL(const XSDK::XString& protocol, const XSDK::XString ip, const URI& uri);

    X_API URL(const XSDK::XString& url);

    /// So string literals work when constructing with =
    X_API URL(const char* cstr) : _protocol(), _ip(), _port(), _uri() { *this = URL(XSDK::XString(cstr)); }

    X_API virtual ~URL() throw() {}

    X_API bool operator==(const URL& rhs)
    { return _protocol == rhs._protocol && _ip == rhs._ip && _port == rhs._port && _uri == rhs._uri ; }

    X_API bool operator!=(const URL& rhs) { return !(*this == rhs); }

    X_API XSDK::XString GetProtocol() const { return _protocol; }
    X_API void SetProtocol(const XSDK::XString protocol);

    X_API XSDK::XString GetIP() const { return _ip; }
    X_API void SetIP(const XSDK::XString ip);

    X_API XSDK::XNullable<int> GetPortNum() const { return _port; }
    X_API void SetPortNum(const XSDK::XNullable<int> port);

    X_API const URI& GetURI() const { return _uri; }
    X_API void SetURI(const URI& uri) { _uri = uri; }

    X_API XSDK::XString GetFullRawURL() const;

    X_API const XSDK::XHash<XSDK::XString>& GetGetArgs() const { return _uri.GetGetArgs(); }

    X_API void AddGetArg(const XSDK::XString& name, const XSDK::XString& value) { _uri.AddGetArg(name, value); }

    X_API void ClearGetArgs() { _uri.ClearGetArgs(); }

    /// \brief Whether protocol is a legal protocol in a URL per RFC 3986.
    X_API static bool IsValidProtocol(const XSDK::XString& str);

    /// \brief Creates a URL from a string which may represent an "absolute" URL
    ///        or a relative one.
    ///
    /// If the given url string has a protocol on it, then the URL is created
    /// directly from it, but if it doesn't have one, then the string is
    /// treated as the URI portion of the URL, and the given protocol, IP, and
    /// port are used.
    ///
    /// The primary purpose of this function is dealing with hrefs from Serenity.
    /// They're usually just the URI portion of the URL (in which case the protocol,
    /// IP, and port are assumed from previous communications), but they _can_
    /// be the full URL. So, by passing the assumed protocol, IP, and port along
    /// with the href string, both types of hrefs are handled the same way.
    X_API static URL MakeAbsolute(const XSDK::XString& protocol, const XSDK::XString& ip, int port, const XSDK::XString& url)
    { return _MakeAbsolute(protocol, ip, port, url); }

    X_API static URL MakeAbsolute(const XSDK::XString& protocol, const XSDK::XString& ip, const XSDK::XString& url)
    { return _MakeAbsolute(protocol, ip, XSDK::XNullable<int>(), url); }

private:

    X_API void _SetIPAndPort(XSDK::XStringSlice ipAndPort, const XSDK::XString& fullURL);

    X_API static URL _MakeAbsolute(const XSDK::XString& protocol, const XSDK::XString& ip,
                                   XSDK::XNullable<int> port, const XSDK::XString& url);

    XSDK::XString _protocol;
    XSDK::XString _ip;
    XSDK::XNullable<int> _port;
    URI _uri;
};

}

#endif
