
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "Webby/URL.h"
#include "Webby/WebbyException.h"
#include "Webby/WebbyUtils.h"

using namespace std;
using namespace XSDK;
using namespace WEBBY;

URL::URL(const XString& protocol, const XString ip, XNullable<int> port, const URI& uri)
: _protocol(protocol), _ip(), _port(port), _uri(uri)
{
    if(!IsValidProtocol(protocol))
        X_STHROW(WebbyException, ("Invalid protocol"));
    SetIP(ip);
    if(_ip.empty() && !_port.IsNull())
        X_STHROW(WebbyException, ("Cannot construct URL with port but no IP"));
}

URL::URL(const XString& protocol, const XString ip, const URI& uri)
: _protocol(protocol), _ip(ip), _port(), _uri(uri)
{
    if(!IsValidProtocol(protocol))
        X_STHROW(WebbyException, ("Invalid protocol"));
    SetIP(ip);
}

URL::URL(const XString& url) : _protocol(), _ip(), _port(), _uri()
{
    XStringSlice urlSlice = &url;
    XStringSlice protocol = urlSlice.FindUntil(':');

    if(urlSlice.front() == '[' || protocol.length() == urlSlice.length())
        X_STHROW(WebbyException, ("URL must have a protocol: %s", url.c_str()));

    SetProtocol(protocol.ToString());
    urlSlice.popFrontN(protocol.length() + 1);
    if(!urlSlice.StartsWith("//"))
        X_STHROW(WebbyException, ("Invalid URL: %s", url.c_str()));
    urlSlice.popFrontN(2);

    if(!urlSlice.empty())
    {
        XStringSlice ipAndPort = urlSlice.FindUntil('/');
        _SetIPAndPort(ipAndPort, url);
        urlSlice.popFrontN(ipAndPort.length());
    }

    SetURI(urlSlice.ToString());
}


void URL::SetProtocol(const XString protocol)
{
    if(!protocol.empty() && !IsValidProtocol(protocol))
        X_STHROW(WebbyException, ("Invalid URL protocol: %s", protocol.c_str()));

    _protocol = protocol;
}


void URL::SetIP(const XString ip)
{
    //FIXME Make this do full validation for valid hostname or IP address.
    if(ip.StartsWith('[') || ip.EndsWith(']'))
        X_STHROW(WebbyException, ("It's not legal for IP addresses to have [ or ] outside of raw URLs"));
    _ip = ip;
}


void URL::SetPortNum(const XNullable<int> port)
{
    if(_ip.empty() && !port.IsNull())
        X_STHROW(WebbyException, ("Cannot have port if IP is empty."));
    _port = port;
}

//FIXME XSocketAddress needs a function for testing IP literals for IPv6 without doing any lookups.
bool IsIPv6(const XString& ip)
{
    return ip.find(':') != string::npos;
}

XString URL::GetFullRawURL() const
{
    const XString ipStr = IsIPv6(_ip) ? XString::Format("[%s]", _ip.c_str()) : _ip;
    const XString portStr = _port.IsNull() ? "" : XString::Format(":%d", _port.Value());
    X_ASSERT(!_ip.empty() || _port.IsNull());

    if(!_protocol.empty())
        return XString::Format("%s://%s%s%s", _protocol.c_str(), ipStr.c_str(), portStr.c_str(), _uri.GetFullRawURI().c_str());

    return XString::Format("%s%s%s", ipStr.c_str(), portStr.c_str(), _uri.GetFullRawURI().c_str());
}


void URL::_SetIPAndPort(XStringSlice ipAndPort, const XString& fullURL)
{
    if(ipAndPort.StartsWith('['))
    {
        XStringSlice ip = ipAndPort.FindUntil(']');
        if(ip.length() == ipAndPort.length())
            X_STHROW(WebbyException, ("Unterminated IPv6 address in URL: %s", fullURL.c_str()));
        ipAndPort.popFrontN(ip.length() + 1);
        SetIP(ip.ToString(1));
    }
    else
    {
        XString ip = ipAndPort.FindUntil(':').ToString();
        SetIP(ip);
        ipAndPort.popFrontN(ip.size());
    }

    if(!ipAndPort.empty())
    {
        X_ASSERT(ipAndPort.front() == ':');

        XString portStr = ipAndPort.ToString(1);
        if(!portStr.IsInteger())
            X_STHROW(WebbyException, ("URL has invalid port: %s", fullURL.c_str()));
        SetPortNum(portStr.ToInt());
    }
}


bool URL::IsValidProtocol(const XString& protocol)
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


URL URL::_MakeAbsolute(const XString& protocol, const XString& ip, XNullable<int> port, const XString& url)
{
    X_ASSERT(!protocol.empty());
    X_ASSERT(!ip.empty());

    XStringSlice possibleProtocol = XStringSlice(&url).FindUntil("://");
    if(possibleProtocol.length() != url.size())
        return URL(url);

    return URL(protocol, ip, port, URI(url));
}
