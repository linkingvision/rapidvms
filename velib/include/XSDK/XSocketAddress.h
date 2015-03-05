
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef _XSDK_XSocketAddress_H_
#define _XSDK_XSocketAddress_H_

#include "XSDK/XSharedLib.h"
#include "XSDK/XString.h"

#ifdef IS_WINDOWS

    #include <WinSock2.h>
    #include <ws2tcpip.h>
    #include <BaseTsd.h>

#else

    #include <sys/types.h>
    #include <sys/socket.h>
    #include <sys/time.h>
    #include <netinet/in.h>
    #include <netdb.h>
    #include <unistd.h>
    #include <arpa/inet.h>

#endif

namespace XSDK
{

    /// String constant that can be used with XSocketAddress to represent the IPv4 INADDR_ANY enumeration.
    const XString ip4_addr_any("INADDR_ANY");

    /// String constant that can be used with XSocketAddress to represent the IPv6 IN6ADDR_ANY enumeration.
    const XString ip6_addr_any("IN6ADDR_ANY");

    /// This class provides a common way to deal with IPv4 and IPv6 addresses.
    class XSocketAddress
    {
    public:
        /// Initializes the class.
        /// @param port The port number to associate with the address.
        /// @param address The IP address or hostname to use.
        X_API XSocketAddress(int port, const XString& address = ip4_addr_any);

        /// @param addr The address to initialize wit.
        /// @param len The length of 'addr'
        X_API XSocketAddress(const struct sockaddr* addr, const socklen_t len);

        X_API virtual ~XSocketAddress() throw();

        X_API int Port() const { return _port; }
        X_API void SetPortNum(int port); // Can't be SetPort b/c of stupid Window macro of same name

        /// Returns the address that was set on the class.
        /// @note This method does NOT convert hostnames to numeric addresses.
        X_API const XString& Address() const { return _addr; }

        /// Sets the IP address (or hostname) on the class. This method causes
        /// the sockaddr structure to become valid.
        /// @param addr The address or hostname to set. If "", then INADDR_ANY is set.
        /// @param port The port to set.
        X_API void SetAddress(const XString& addr, int port = -1);

        /// Returns the address family of the IP address (if one has been set).
        /// @return AF_UNSPEC, AF_INET (IPv4), or AF_INET6 (IPv6)
        X_API unsigned int AddressFamily() const { return _sockaddr.ss_family; }

        // Comparison operators
        X_API bool operator==(const XSocketAddress&) const;
        X_API bool operator==(const struct sockaddr*) const;
        X_API bool operator==(const struct sockaddr_storage&) const;
        X_API bool operator!=(const XSocketAddress&) const;
        X_API bool operator!=(const struct sockaddr*) const;
        X_API bool operator!=(const struct sockaddr_storage&) const;

        /// Returns a sockaddr structure that represents the address.
        /// @note Call SockAddrSize() to get the true size of the structure returned.
        X_API struct sockaddr* GetSockAddr() const { return (struct sockaddr*)&_sockaddr; }

        /// Returns the size of the sockaddr structure.
        X_API socklen_t SockAddrSize() const { return SockAddrSize(_sockaddr.ss_family); }

        X_API bool IsIPv4() const;
        X_API bool IsIPv6() const;
        X_API bool IsMulticast() const;
        /// @param unmapped Optional. The string that will receive the unmapped IPv4 address if
        /// the object indeed holds an IPv4 mapped to IPv6 address.
        X_API bool IsIPv4MappedToIPv6(XString* unmapped=0) const;
        /// Tests if the address is 0.0.0.0 or ::
        X_API bool IsWildcardAddress() const;

        /// Returns the address family type for the given address.
        /// @param address The address to determine the family of.
        /// @param saddr Optional. If not 0, it will be filled with the result of
        /// the address lookup.
        /// @return AF_INET (IPv4) or AF_INET6 (IPv6)
        X_API static unsigned int GetAddressFamily(const XString& address, struct sockaddr* saddr=0);

        /// Returns the sockaddr structure size for the given address family.
        /// @param addrFamily AF_INET (IPv4) or AF_INET6 (IPv6)
        X_API static socklen_t SockAddrSize(unsigned int addrFamily);

        /// Converts an IP address structure to a string.
        /// @note Functions like inet_ntop() but support Windows XP.
        X_API static XString AddressToString(const struct sockaddr* addr, const socklen_t len);

        /// Converts an IP address string to an address structure.
        /// @note Functions like inet_pton() but supports Windows XP.
        /// @param saddr The IP address to convert to a string.
        /// @param addr A pointer to the sockaddr_stroage structure to receive the address.
        /// @param len The length of 'addr'.
        X_API static void StringToAddress(const XString& saddr, struct sockaddr* addr, const socklen_t len);

        /// Returns the address portion if an IPv6 address string.
        /// Used to extract the address from strings of the form "[<address>]:<port>
        /// @param addr The address string to extract the IP address from.
        /// @return The IPv6 address portion of the string.
        X_API static XString IsolateAddress(const XString& addr);

        /// Returns whether the string represents a possible hostname per the grammar for a hostname.
        /// It does not do DNS lookup to verify whether the host in question actually exists.
        static X_API bool IsHostname(const XString& addr);

        X_API static bool IsIPv4(const XString& addr);
        X_API static bool IsIPv4(const struct sockaddr* addr, const socklen_t len);
        X_API static bool IsIPv6(const XString& addr);
        X_API static bool IsIPv6(const struct sockaddr* addr, const socklen_t len);
        X_API static bool IsMulticast(const XString& addr);
        X_API static bool IsMulticast(const struct sockaddr* addr, const socklen_t len);
        X_API static bool IsWildcardAddress(const XString& addr);
        X_API static bool IsWildcardAddress(const struct sockaddr* addr, const socklen_t len);

        /// @param addr The address to test.
        /// @param unmapped Optional. The string that will receive the unmapped IPv4 address if
        /// the object indeed holds an IPv4 mapped to IPv6 address.
        X_API static bool IsIPv4MappedToIPv6(const XString& addr, XString* unmapped=0);
        /// @param addr The address structure that contains the address to test.
        /// @param len The length of the data pointed to by 'addr'
        /// @param unmapped Optional. The string that will receive the unmapped IPv4 address if
        /// the object indeed holds an IPv4 mapped to IPv6 address.
        X_API static bool IsIPv4MappedToIPv6(const struct sockaddr* addr, const socklen_t len, XString* unmapped=0);

    private:
        void _InitSubSystem() const;

        int _port;        ///< The port associated with the address.
        XString _addr;    ///< The original address (or hostname) passed to this class.
        struct sockaddr_storage _sockaddr;  ///< The sockaddr structure used for the sockets API.
    };
}

#endif
