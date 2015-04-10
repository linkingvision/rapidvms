
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "XSDK/XSocketAddress.h"
#include "XSDK/XException.h"
#include "XSDK/OS.h"
#include <cctype>

using namespace std;
using namespace XSDK;

XSocketAddress::XSocketAddress(int port, const XSDK::XString& address) :
_port(port),
_addr(address)
{
    _InitSubSystem();
    SetAddress(address);
}

XSocketAddress::XSocketAddress(const struct sockaddr* addr, const socklen_t len) :
_port(0),
_addr()
{
    _InitSubSystem();

    memset(&_sockaddr, 0, sizeof(_sockaddr));
    memcpy(&_sockaddr, addr, len);

    if (_sockaddr.ss_family == AF_INET)
        _port = ntohs( ((struct sockaddr_in*)&_sockaddr)->sin_port );
    else if (_sockaddr.ss_family == AF_INET6)
        _port = ntohs( ((struct sockaddr_in6*)&_sockaddr)->sin6_port );

    _addr = XSocketAddress::AddressToString(addr, len);
}

XSocketAddress::~XSocketAddress() throw()
{
#ifdef IS_WINDOWS
    WSACleanup();
#endif
}

X_API void XSocketAddress::SetPortNum(int port)
{
    _port = port;
    if (_sockaddr.ss_family == AF_INET)
        ((struct sockaddr_in*)&_sockaddr)->sin_port = htons( _port );
    else if (_sockaddr.ss_family == AF_INET6)
        ((struct sockaddr_in6*)&_sockaddr)->sin6_port = htons( _port );
}

void XSocketAddress::SetAddress(const XString& addr, int port)
{
    if (port >= 0)
        _port = port;

    memset(&_sockaddr, 0, sizeof(_sockaddr));
    if (addr.length() == 0 || addr == XSDK::ip4_addr_any)
    {
        // IPv4 any address
        struct sockaddr_in* pa = (struct sockaddr_in*)&_sockaddr;
        pa->sin_family = AF_INET;
        pa->sin_port = htons( _port );
        pa->sin_addr.s_addr = htonl(INADDR_ANY);
    }
    else if (addr == XSDK::ip6_addr_any)
    {
        // IPv6 any address
        struct sockaddr_in6* pa = (struct sockaddr_in6*)&_sockaddr;
        pa->sin6_family = AF_INET6;
        pa->sin6_port = htons( _port );
        pa->sin6_addr = in6addr_any;
    }
    else
    {
        // Determine address type and set sockaddr_storage structure.
        GetAddressFamily(addr, (struct sockaddr*)&_sockaddr);
        if (_sockaddr.ss_family == AF_INET)
        {
            struct sockaddr_in* pa = (struct sockaddr_in*)&_sockaddr;
            pa->sin_port = htons( _port );
        }
        else if (_sockaddr.ss_family == AF_INET6)
        {
            struct sockaddr_in6* pa = (struct sockaddr_in6*)&_sockaddr;
            pa->sin6_port = htons( _port );
        }
        else
            X_THROW((XString::Format("XSocketAddress::SetAddress: Unknown address family (%d) for address \'%s\'", _sockaddr.ss_family, addr.c_str())));
    }

    _addr = addr;
}

unsigned int XSocketAddress::GetAddressFamily(const XSDK::XString& address, struct sockaddr* saddr)
{
    struct addrinfo hint, *info = 0;
    memset(&hint, 0, sizeof(hint));
    hint.ai_family = AF_UNSPEC;
    hint.ai_flags = AI_CANONNAME;
    //hint.ai_flags = AI_NUMERICHOST;  // Uncomment this to disable DNS lookup
    int ret = getaddrinfo(IsolateAddress(address).c_str(), 0, &hint, &info);
    if (ret) {
        X_THROW(("XSocketAddress::GetAddressFamily: Failed to determine address info for \'%s\'. %s", address.c_str(), GetErrorMsg(ret).c_str()));
    }
    unsigned int family = info->ai_family;
    if (saddr)
        memcpy(saddr, info->ai_addr, SockAddrSize(family));
    freeaddrinfo(info);
    return family;
}

bool XSocketAddress::operator==(const XSocketAddress& other) const
{
    return *this == other.GetSockAddr();
}

bool XSocketAddress::operator==(const struct sockaddr* other) const
{
    X_ASSERT(other);

    size_t len = 0;
    if (other->sa_family == AF_INET)
        len = sizeof(struct sockaddr_in);
    else if (other->sa_family == AF_INET6)
        len = sizeof(struct sockaddr_in6);
    else
        X_THROW(("XSocketAddress::operator==: Unknown address family (%u)", other->sa_family));

    return memcmp(GetSockAddr(), other, len) == 0;
}

bool XSocketAddress::operator==(const struct sockaddr_storage& other) const
{
    return *this == (struct sockaddr*)&other;
}

bool XSocketAddress::operator!=(const XSocketAddress& other) const
{
    return !(*this == other.GetSockAddr());
}

bool XSocketAddress::operator!=(const struct sockaddr* other) const
{
    return !(*this == other);
}

bool XSocketAddress::operator!=(const struct sockaddr_storage& other) const
{
    return !(*this ==(struct sockaddr*)&other);
}

socklen_t XSocketAddress::SockAddrSize(unsigned int addrFamily)
{
    if (addrFamily == AF_INET)
        return sizeof(struct sockaddr_in);
    else if (addrFamily == AF_INET6)
        return sizeof(struct sockaddr_in6);
    X_THROW(("XSocketAddress::SockAddrSize(): Unknown socket address family (%d)", addrFamily));
}

XString XSocketAddress::AddressToString(const struct sockaddr* addr, const socklen_t len)
{
    XString result;

#ifdef IS_WINDOWS
    DWORD blen = 64;
    wchar_t buf[64];

    // Must clear the port number (requires copying) so WSAAddressToString()
    // doesn't append the port number to the string.
    sockaddr_storage sa;
    memset(&sa, 0, sizeof(sa));
    memcpy(&sa, addr, len);
    if (addr->sa_family == AF_INET)
        ((sockaddr_in*)&sa)->sin_port = 0;
    else if (addr->sa_family == AF_INET6)
        ((sockaddr_in6*)&sa)->sin6_port = 0;
    else
       X_THROW(("XSocketAddress::AddressToString(): Unknown address family (%d)", addr->sa_family));

    if (WSAAddressToStringW((struct sockaddr*)&sa, len, 0, buf, &blen))
        X_THROW(("XSocketAddress::AddressToString(): %s", GetLastErrorMsg().c_str()));
    result = buf;
#else
    char tmp[INET6_ADDRSTRLEN];
    void* pa = 0;
    if (addr->sa_family == AF_INET)
        pa = &(((struct sockaddr_in*)addr)->sin_addr);
    else if (addr->sa_family == AF_INET6)
        pa = &(((struct sockaddr_in6*)addr)->sin6_addr);
    else
        X_THROW(("XSocketAddress::AddressToString(): Unknown address family (%d)", addr->sa_family));
    const char* s = inet_ntop(addr->sa_family, pa, tmp, INET6_ADDRSTRLEN);
    if (!s)
        X_THROW(("XSocketAddress::AddressToString(): %s", GetLastErrorMsg().c_str()));
    result = s;
#endif

    return result;
}

void XSocketAddress::StringToAddress(const XString& saddr, struct sockaddr* addr, const socklen_t len)
{
    XSocketAddress a(0, saddr);
    if (a.SockAddrSize() > len)
        X_THROW(("XSocketAddress:StringToAddress: Address structure not large enough to hold result. len (%d)", len));
    memcpy(addr, a.GetSockAddr(), a.SockAddrSize());
}

XString XSocketAddress::IsolateAddress(const XString& addr)
{
    // If "[]" is present, return the value between the brackets.
    size_t spos = addr.find_first_of('[');
    if (spos == std::string::npos)
        return addr;
    size_t epos = addr.find_last_of(']');
    if (epos == std::string::npos)
        return addr;
    ++spos;
    return addr.substr(spos, epos-spos);
}

bool XSocketAddress::IsIPv4() const
{
    return _sockaddr.ss_family == AF_INET;
}

bool XSocketAddress::IsIPv6() const
{
    return _sockaddr.ss_family == AF_INET6;
}

bool XSocketAddress::IsMulticast() const
{
    if (IsIPv6())
    {
        struct sockaddr_in6* a6 = (struct sockaddr_in6*)&_sockaddr;
        return IN6_IS_ADDR_MULTICAST(&a6->sin6_addr);
    }
    else
    {
        struct sockaddr_in* a4 = (struct sockaddr_in*)&_sockaddr;
        uint8_t firstPart = ntohl(a4->sin_addr.s_addr) >> 24;
        if( firstPart >= 224 && firstPart <= 239 )
            return true;
        return false;
    }
}

bool XSocketAddress::IsIPv4MappedToIPv6(XString* unmapped) const
{
    if (IsIPv6())
    {
        struct sockaddr_in6* a6 = (struct sockaddr_in6*)&_sockaddr;
        if (IN6_IS_ADDR_V4MAPPED(&a6->sin6_addr))
        {
            if (unmapped)
                unmapped->FormatInPlace("%u.%u.%u.%u",
                    a6->sin6_addr.s6_addr[12],
                    a6->sin6_addr.s6_addr[13],
                    a6->sin6_addr.s6_addr[14],
                    a6->sin6_addr.s6_addr[15]);
            return true;
        }
    }
    return false;
}

bool XSocketAddress::IsWildcardAddress() const
{
    if (IsIPv6())
    {
        static uint8_t wildcard[16] = {0};
        struct sockaddr_in6* a6 = (struct sockaddr_in6*)&_sockaddr;
        return memcmp(a6->sin6_addr.s6_addr, wildcard, sizeof(wildcard)) == 0;
    }
    else
    {
        static uint8_t wildcard[4] = {0};
        struct sockaddr_in* a4 = (struct sockaddr_in*)&_sockaddr;
        return memcmp(&a4->sin_addr.s_addr, wildcard, sizeof(wildcard)) == 0;
    }
}


bool XSocketAddress::IsHostname(const XString& addr)
{
    // ABNF grammar for a hostname (RFC 2396, section 3.2.2):
    // hostname      = *( domainlabel "." ) toplabel [ "." ]
    // domainlabel   = alphanum | alphanum *( alphanum | "-" ) alphanum
    // toplabel      = alpha | alpha *( alphanum | "-" ) alphanum
    if(addr.empty())
        return false;

    // check toplabel
    const size_t lastDot = addr.find_last_of('.', addr.size() - 2);
    const size_t topLabelStart = lastDot == string::npos ? 0 : lastDot + 1;
    const size_t topLabelLast = addr.size() - (addr[addr.size() - 1] == '.' ? 2 : 1);

    if(!isalpha(addr[topLabelStart]))
        return false;

    for(size_t i = topLabelStart + 1; i < topLabelLast; ++i)
    {
        if(!isalnum(addr[i]) && addr[i] != '-')
            return false;
    }

    if(!isalnum(addr[topLabelLast]))
        return false;

    //check domainlabel
    for(size_t i = 0; i < topLabelStart; )
    {
        if(!isalnum(addr[i++]))
            return false;

        const size_t nextDot = addr.find('.', i);
        if(nextDot == string::npos)
            return false;

        if(nextDot == i)
        {
            ++i;
            continue;
        }

        for(const size_t lastAlphaNum = nextDot - 1; i < lastAlphaNum; ++i)
        {
            if(!isalnum(addr[i]) && addr[i] != '-')
                return false;
        }

        if(!isalnum(addr[i]))
            return false;

        i += 2;
    }

    return true;
}


// STATIC versions
bool XSocketAddress::IsIPv4(const XString& addr)
{
    return XSocketAddress(0, addr).IsIPv4();
}

bool XSocketAddress::IsIPv4(const struct sockaddr* addr, const socklen_t len)
{
    return addr->sa_family == AF_INET;
}

bool XSocketAddress::IsIPv6(const XString& addr)
{
    return XSocketAddress(0, addr).IsIPv6();
}

bool XSocketAddress::IsIPv6(const struct sockaddr* addr, const socklen_t len)
{
    return addr->sa_family == AF_INET6;
}

bool XSocketAddress::IsMulticast(const XString& addr)
{
    return XSocketAddress(0, addr).IsMulticast();
}

bool XSocketAddress::IsMulticast(const struct sockaddr* addr, const socklen_t len)
{
    return XSocketAddress(addr, len).IsMulticast();
}

bool XSocketAddress::IsIPv4MappedToIPv6(const XString& addr, XString* unmapped)
{
    return XSocketAddress(0, addr).IsIPv4MappedToIPv6(unmapped);
}

bool XSocketAddress::IsIPv4MappedToIPv6(const struct sockaddr* addr, const socklen_t len, XString* unmapped)
{
    return XSocketAddress(addr, len).IsIPv4MappedToIPv6(unmapped);
}

bool XSocketAddress::IsWildcardAddress(const XString& addr)
{
    return XSocketAddress(0, addr).IsWildcardAddress();
}

bool XSocketAddress::IsWildcardAddress(const struct sockaddr* addr, const socklen_t len)
{
    return XSocketAddress(addr, len).IsWildcardAddress();
}

void XSocketAddress::_InitSubSystem() const
{
#ifdef IS_WINDOWS
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

    wVersionRequested = MAKEWORD( 2, 2 );

    err = WSAStartup( wVersionRequested, &wsaData );
    if ( err != 0 )
        X_THROW( ( "Unable to load WinSock DLL." ));

    if ( LOBYTE( wsaData.wVersion ) != 2 ||
         HIBYTE( wsaData.wVersion ) != 2 )
    {
        X_THROW( ( "Unable to load WinSock DLL." ));
    }
#endif
}
