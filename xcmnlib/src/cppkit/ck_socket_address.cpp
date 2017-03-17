
/// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=--=-=-=-=-=-
/// cppkit - http://www.cppkit.org
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

#include "cppkit/ck_socket_address.h"
#include "cppkit/ck_exception.h"
#include "cppkit/os/ck_error_msg.h"

#include <cctype>

using namespace std;
using namespace cppkit;

ck_socket_address::ck_socket_address(int port, const ck_string& address) :
    _port(port),
    _addr(address)
{
    _init_sub_system();
    set_address(address);
}

ck_socket_address::ck_socket_address(const struct sockaddr* addr, const socklen_t len) :
    _port(0),
    _addr()
{
    _init_sub_system();

    memset(&_sockaddr, 0, sizeof(_sockaddr));
    memcpy(&_sockaddr, addr, len);

    if (_sockaddr.ss_family == AF_INET)
        _port = ntohs( ((struct sockaddr_in*)&_sockaddr)->sin_port );
    else if (_sockaddr.ss_family == AF_INET6)
        _port = ntohs( ((struct sockaddr_in6*)&_sockaddr)->sin6_port );

    _addr = ck_socket_address::address_to_string(addr, len);
}

ck_socket_address::~ck_socket_address() throw()
{
#ifdef IS_WINDOWS
    WSACleanup();
#endif
}

void ck_socket_address::set_port_num(int port)
{
    _port = port;
    if (_sockaddr.ss_family == AF_INET)
        ((struct sockaddr_in*)&_sockaddr)->sin_port = htons( _port );
    else if (_sockaddr.ss_family == AF_INET6)
        ((struct sockaddr_in6*)&_sockaddr)->sin6_port = htons( _port );
}

void ck_socket_address::set_address(const ck_string& addr, int port)
{
    if (port >= 0)
        _port = port;

    memset(&_sockaddr, 0, sizeof(_sockaddr));
    if (addr.length() == 0 || addr == ip4_addr_any)
    {
        // IPv4 any address
        struct sockaddr_in* pa = (struct sockaddr_in*)&_sockaddr;
        pa->sin_family = AF_INET;
        pa->sin_port = htons( _port );
        pa->sin_addr.s_addr = htonl(INADDR_ANY);
    }
    else if (addr == ip6_addr_any)
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
        get_address_family(addr, (struct sockaddr*)&_sockaddr);
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
            CK_THROW((ck_string::format("ck_socket_address::set_address: Unknown address family (%d) for address \'%s\'", _sockaddr.ss_family, addr.c_str())));
    }

    _addr = addr;
}

unsigned int ck_socket_address::get_address_family(const ck_string& address, struct sockaddr* saddr)
{
    struct addrinfo hint, *info = 0;
    memset(&hint, 0, sizeof(hint));
    hint.ai_family = AF_UNSPEC;
    hint.ai_flags = AI_CANONNAME;
    //hint.ai_flags = AI_NUMERICHOST;  // Uncomment this to disable DNS lookup
    int ret = getaddrinfo(isolate_address(address).c_str(), 0, &hint, &info);
    if (ret) {
        CK_THROW(("ck_socket_address::get_address_family: Failed to determine address info for \'%s\'. %s", address.c_str(), ck_get_error_msg(ret).c_str()));
    }
    unsigned int family = info->ai_family;
    if (saddr)
        memcpy(saddr, info->ai_addr, sock_addr_size(family));
    freeaddrinfo(info);
    return family;
}

bool ck_socket_address::operator==(const ck_socket_address& other) const
{
    return *this == other.get_sock_addr();
}

bool ck_socket_address::operator==(const struct sockaddr* other) const
{
    CK_ASSERT(other);

    size_t len = 0;
    if (other->sa_family == AF_INET)
        len = sizeof(struct sockaddr_in);
    else if (other->sa_family == AF_INET6)
        len = sizeof(struct sockaddr_in6);
    else
        CK_THROW(("ck_socket_address::operator==: Unknown address family (%u)", other->sa_family));

    return memcmp(get_sock_addr(), other, len) == 0;
}

bool ck_socket_address::operator==(const struct sockaddr_storage& other) const
{
    return *this == (struct sockaddr*)&other;
}

bool ck_socket_address::operator!=(const ck_socket_address& other) const
{
    return !(*this == other.get_sock_addr());
}

bool ck_socket_address::operator!=(const struct sockaddr* other) const
{
    return !(*this == other);
}

bool ck_socket_address::operator!=(const struct sockaddr_storage& other) const
{
    return !(*this ==(struct sockaddr*)&other);
}

socklen_t ck_socket_address::sock_addr_size(unsigned int addrFamily)
{
    if (addrFamily == AF_INET)
        return sizeof(struct sockaddr_in);
    else if (addrFamily == AF_INET6)
        return sizeof(struct sockaddr_in6);
    CK_THROW(("ck_socket_address::sock_addr_size(): Unknown socket address family (%d)", addrFamily));
}

ck_string ck_socket_address::address_to_string(const struct sockaddr* addr, const socklen_t len)
{
    ck_string result;

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
       CK_THROW(("ck_socket_address::address_to_string(): Unknown address family (%d)", addr->sa_family));

    if (WSAAddressToStringW((struct sockaddr*)&sa, len, 0, buf, &blen))
        CK_THROW(("ck_socket_address::address_to_string(): %s", ck_get_last_error_msg().c_str()));
    result = buf;
#else
    char tmp[INET6_ADDRSTRLEN];
    void* pa = 0;
    if (addr->sa_family == AF_INET)
        pa = &(((struct sockaddr_in*)addr)->sin_addr);
    else if (addr->sa_family == AF_INET6)
        pa = &(((struct sockaddr_in6*)addr)->sin6_addr);
    else
        CK_THROW(("ck_socket_address::address_to_string(): Unknown address family (%d)", addr->sa_family));
    const char* s = inet_ntop(addr->sa_family, pa, tmp, INET6_ADDRSTRLEN);
    if (!s)
        CK_THROW(("ck_socket_address::address_to_string(): %s", ck_get_last_error_msg().c_str()));
    result = s;
#endif

    return result;
}

void ck_socket_address::string_to_address(const ck_string& saddr, struct sockaddr* addr, const socklen_t len)
{
    ck_socket_address a(0, saddr);
    if (a.sock_addr_size() > len)
        CK_THROW(("ck_socket_address:string_to_address: Address structure not large enough to hold result. len (%d)", len));
    memcpy(addr, a.get_sock_addr(), a.sock_addr_size());
}

ck_string ck_socket_address::isolate_address(const ck_string& addr)
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

bool ck_socket_address::is_ipv4() const
{
    return _sockaddr.ss_family == AF_INET;
}

bool ck_socket_address::is_ipv6() const
{
    return _sockaddr.ss_family == AF_INET6;
}

bool ck_socket_address::is_multicast() const
{
    if (is_ipv6())
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

bool ck_socket_address::is_ipv4_mapped_to_ipv6(ck_string* unmapped) const
{
    if (is_ipv6())
    {
        struct sockaddr_in6* a6 = (struct sockaddr_in6*)&_sockaddr;
        if (IN6_IS_ADDR_V4MAPPED(&a6->sin6_addr))
        {
            if (unmapped)
                *unmapped = ck_string::format("%u.%u.%u.%u",
                                              a6->sin6_addr.s6_addr[12],
                                              a6->sin6_addr.s6_addr[13],
                                              a6->sin6_addr.s6_addr[14],
                                              a6->sin6_addr.s6_addr[15]);
            return true;
        }
    }
    return false;
}

bool ck_socket_address::is_wildcard_address() const
{
    if (is_ipv6())
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

bool ck_socket_address::is_hostname(const ck_string& addr)
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
bool ck_socket_address::is_ipv4(const ck_string& addr)
{
    return ck_socket_address(0, addr).is_ipv4();
}

bool ck_socket_address::is_ipv4(const struct sockaddr* addr, const socklen_t len)
{
    return addr->sa_family == AF_INET;
}

bool ck_socket_address::is_ipv6(const ck_string& addr)
{
    return ck_socket_address(0, addr).is_ipv6();
}

bool ck_socket_address::is_ipv6(const struct sockaddr* addr, const socklen_t len)
{
    return addr->sa_family == AF_INET6;
}

bool ck_socket_address::is_multicast(const ck_string& addr)
{
    return ck_socket_address(0, addr).is_multicast();
}

bool ck_socket_address::is_multicast(const struct sockaddr* addr, const socklen_t len)
{
    return ck_socket_address(addr, len).is_multicast();
}

bool ck_socket_address::is_ipv4_mapped_to_ipv6(const ck_string& addr, ck_string* unmapped)
{
    return ck_socket_address(0, addr).is_ipv4_mapped_to_ipv6(unmapped);
}

bool ck_socket_address::is_ipv4_mapped_to_ipv6(const struct sockaddr* addr, const socklen_t len, ck_string* unmapped)
{
    return ck_socket_address(addr, len).is_ipv4_mapped_to_ipv6(unmapped);
}

bool ck_socket_address::is_wildcard_address(const ck_string& addr)
{
    return ck_socket_address(0, addr).is_wildcard_address();
}

bool ck_socket_address::is_wildcard_address(const struct sockaddr* addr, const socklen_t len)
{
    return ck_socket_address(addr, len).is_wildcard_address();
}

void ck_socket_address::_init_sub_system() const
{
#ifdef IS_WINDOWS
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

    wVersionRequested = MAKEWORD( 2, 2 );

    err = WSAStartup( wVersionRequested, &wsaData );
    if ( err != 0 )
        CK_THROW( ( "Unable to load WinSock DLL." ));

    if ( LOBYTE( wsaData.wVersion ) != 2 ||
         HIBYTE( wsaData.wVersion ) != 2 )
    {
        CK_THROW( ( "Unable to load WinSock DLL." ));
    }
#endif
}
