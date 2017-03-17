
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

#include "hyperhttp/hyperurl.h"
#include "hyperhttp/hyperhttp_exception.h"
#include "hyperhttp/hyperhttp_utils.h"

using namespace std;
using namespace cppkit;
using namespace hyperhttp;

url::url( const ck_string& protocol, const ck_string ip, ck_nullable<int> port, const uri& uri ) :
    _protocol(protocol),
    _ip(),
    _port(port),
    _uri(uri)
{
    if(!is_valid_protocol(protocol))
        CK_STHROW(hyperhttp_exception, ("Invalid protocol"));
    set_ip(ip);
    if(_ip.empty() && !_port.is_null())
        CK_STHROW(hyperhttp_exception, ("Cannot construct url with port but no IP"));
}

url::url(const ck_string& protocol, const ck_string ip, const uri& uri) :
    _protocol(protocol),
    _ip(ip),
    _port(),
    _uri(uri)
{
    if(!is_valid_protocol(protocol))
        CK_STHROW(hyperhttp_exception, ("Invalid protocol"));
    set_ip(ip);
}

url::url(const ck_string& url) : _protocol(), _ip(), _port(), _uri()
{
    ck_string_slice urlSlice = &url;
    ck_string_slice protocol = urlSlice.find_until(':');

    if(urlSlice.front() == '[' || protocol.length() == urlSlice.length())
        CK_STHROW(hyperhttp_exception, ("url must have a protocol: %s", url.c_str()));

    set_protocol(protocol.to_string());
    urlSlice.pop_front_n(protocol.length() + 1);
    if(!urlSlice.starts_with("//"))
        CK_STHROW(hyperhttp_exception, ("Invalid url: %s", url.c_str()));
    urlSlice.pop_front_n(2);

    if(!urlSlice.empty())
    {
        ck_string_slice ipAndPort = urlSlice.find_until('/');
        _set_ip_and_port(ipAndPort, url);
        urlSlice.pop_front_n(ipAndPort.length());
    }

    set_uri(urlSlice.to_string());
}

void url::set_protocol(const ck_string protocol)
{
    if(!protocol.empty() && !is_valid_protocol(protocol))
        CK_STHROW(hyperhttp_exception, ("Invalid url protocol: %s", protocol.c_str()));

    _protocol = protocol;
}

void url::set_ip(const ck_string ip)
{
    //FIXME Make this do full validation for valid hostname or IP address.
    if(ip.starts_with('[') || ip.ends_with(']'))
        CK_STHROW(hyperhttp_exception, ("It's not legal for IP addresses to have [ or ] outside of raw urls"));
    _ip = ip;
}

void url::set_port_num(const ck_nullable<int> port)
{
    if(_ip.empty() && !port.is_null())
        CK_STHROW(hyperhttp_exception, ("Cannot have port if IP is empty."));
    _port = port;
}

//FIXME XSocketAddress needs a function for testing IP literals for IPv6 without doing any lookups.
bool is_ipv6(const ck_string& ip)
{
    return ip.find(':') != string::npos;
}

ck_string url::get_full_raw_url() const
{
    const ck_string ipStr = is_ipv6(_ip) ? ck_string::format("[%s]", _ip.c_str()) : _ip;
    const ck_string portStr = _port.is_null() ? "" : ck_string::format(":%d", _port.value());

    if(!_protocol.empty())
        return ck_string::format("%s://%s%s%s", _protocol.c_str(), ipStr.c_str(), portStr.c_str(), _uri.get_full_raw_uri().c_str());

    return ck_string::format("%s%s%s", ipStr.c_str(), portStr.c_str(), _uri.get_full_raw_uri().c_str());
}

void url::_set_ip_and_port(ck_string_slice ipAndPort, const ck_string& fullURL)
{
    if(ipAndPort.starts_with('['))
    {
        ck_string_slice ip = ipAndPort.find_until(']');
        if(ip.length() == ipAndPort.length())
            CK_STHROW(hyperhttp_exception, ("Unterminated IPv6 address in url: %s", fullURL.c_str()));
        ipAndPort.pop_front_n(ip.length() + 1);
        set_ip(ip.to_string(1));
    }
    else
    {
        ck_string ip = ipAndPort.find_until(':').to_string();
        set_ip(ip);
        ipAndPort.pop_front_n(ip.size());
    }

    if(!ipAndPort.empty())
    {
        ck_string portStr = ipAndPort.to_string(1);
        if(!portStr.is_integer())
            CK_STHROW(hyperhttp_exception, ("url has invalid port: %s", fullURL.c_str()));
        set_port_num(portStr.to_int());
    }
}

bool url::is_valid_protocol(const ck_string& protocol)
{
    if(protocol.empty() || !isalpha(protocol[0]))
        return false;

    for(size_t i = 1, end = protocol.size(); i != end; ++i)
    {
        const char c = protocol[i];

        if(!isalnum(c) && c != '.' && c != '-' && c != '+')
            return false;
    }

    return true;
}

url url::_make_absolute(const ck_string& protocol, const ck_string& ip, ck_nullable<int> port, const ck_string& u)
{
    ck_string_slice possibleProtocol = ck_string_slice(&u).find_until("://");
    if(possibleProtocol.length() != u.size())
        return url(u);

    return url(protocol, ip, port, uri(u));
}
