
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

#ifndef cppkit_socket_address_h_
#define cppkit_socket_address_h_

#include "cppkit/os/ck_exports.h"
#include "cppkit/ck_string.h"

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

namespace cppkit
{

/// String constant that can be used with XSocketAddress to represent the IPv4 INADDR_ANY enumeration.
const ck_string ip4_addr_any("INADDR_ANY");

/// String constant that can be used with XSocketAddress to represent the IPv6 IN6ADDR_ANY enumeration.
const ck_string ip6_addr_any("IN6ADDR_ANY");

/// This class provides a common way to deal with IPv4 and IPv6 addresses.
class ck_socket_address
{
public:
    /// Initializes the class.
    /// @param port The port number to associate with the address.
    /// @param address The IP address or hostname to use.
    CK_API ck_socket_address(int port, const ck_string& address = ip4_addr_any);

    /// @param addr The address to initialize wit.
    /// @param len The length of 'addr'
    CK_API ck_socket_address(const struct sockaddr* addr, const socklen_t len);

    CK_API virtual ~ck_socket_address() throw();

    CK_API int port() const { return _port; }
    CK_API void set_port_num(int port); // Can't be SetPort b/c of stupid Window macro of same name

    /// Returns the address that was set on the class.
    /// @note This method does NOT convert hostnames to numeric addresses.
    CK_API const ck_string& address() const { return _addr; }

    /// Sets the IP address (or hostname) on the class. This method causes the sockaddr structure to
    /// become valid.
    /// @param addr The address or hostname to set. If "", then INADDR_ANY is set.
    /// @param port The port to set.
    CK_API void set_address(const ck_string& addr, int port = -1);

    /// Returns the address family of the IP address (if one has been set).
    /// @return AF_UNSPEC, AF_INET (IPv4), or AF_INET6 (IPv6)
    CK_API unsigned int address_family() const { return _sockaddr.ss_family; }

    // Comparison operators
    CK_API bool operator==(const ck_socket_address&) const;
    CK_API bool operator==(const struct sockaddr*) const;
    CK_API bool operator==(const struct sockaddr_storage&) const;
    CK_API bool operator!=(const ck_socket_address&) const;
    CK_API bool operator!=(const struct sockaddr*) const;
    CK_API bool operator!=(const struct sockaddr_storage&) const;

    /// Returns a sockaddr structure that represents the address.
    /// @note Call sock_addr_size() to get the true size of the structure returned.
    CK_API struct sockaddr* get_sock_addr() const { return (struct sockaddr*)&_sockaddr; }

    /// Returns the size of the sockaddr structure.
    CK_API socklen_t sock_addr_size() const { return sock_addr_size(_sockaddr.ss_family); }

    CK_API bool is_ipv4() const;
    CK_API bool is_ipv6() const;
    CK_API bool is_multicast() const;
    /// @param unmapped Optional. The string that will receive the unmapped IPv4 address if the object
    /// indeed holds an IPv4 mapped to IPv6 address.
    CK_API bool is_ipv4_mapped_to_ipv6(ck_string* unmapped=0) const;
    /// Tests if the address is 0.0.0.0 or ::
    CK_API bool is_wildcard_address() const;

    /// Returns the address family type for the given address.
    /// @param address The address to determine the family of.
    /// @param saddr Optional. If not 0, it will be filled with the result of the address lookup.
    /// @return AF_INET (IPv4) or AF_INET6 (IPv6)
    CK_API static unsigned int get_address_family(const ck_string& address, struct sockaddr* saddr=0);

    /// Returns the sockaddr structure size for the given address family.
    /// @param addrFamily AF_INET (IPv4) or AF_INET6 (IPv6)
    CK_API static socklen_t sock_addr_size(unsigned int addrFamily);

    /// Converts an IP address structure to a string.
    /// @note Functions like inet_ntop() but support Windows XP.
    CK_API static ck_string address_to_string(const struct sockaddr* addr, const socklen_t len);

    /// Converts an IP address string to an address structure.
    /// @note Functions like inet_pton() but supports Windows XP.
    /// @param saddr The IP address to convert to a string.
    /// @param addr A pointer to the sockaddr_stroage structure to receive the address.
    /// @param len The length of 'addr'.
    CK_API static void string_to_address(const ck_string& saddr,
                                         struct sockaddr* addr,
                                         const socklen_t len);

    /// Returns the address portion if an IPv6 address string. Used to extract the address from strings
    /// of the form "[<address>]:<port>
    /// @param addr The address string to extract the IP address from.
    /// @return The IPv6 address portion of the string.
    CK_API static ck_string isolate_address(const ck_string& addr);

    /// Returns whether the string represents a possible hostname per the grammar for a hostname. It does
    /// not do DNS lookup to verify whether the host in question actually exists.
    static CK_API bool is_hostname(const ck_string& addr);

    CK_API static bool is_ipv4(const ck_string& addr);
    CK_API static bool is_ipv4(const struct sockaddr* addr, const socklen_t len);
    CK_API static bool is_ipv6(const ck_string& addr);
    CK_API static bool is_ipv6(const struct sockaddr* addr, const socklen_t len);
    CK_API static bool is_multicast(const ck_string& addr);
    CK_API static bool is_multicast(const struct sockaddr* addr, const socklen_t len);
    CK_API static bool is_wildcard_address(const ck_string& addr);
    CK_API static bool is_wildcard_address(const struct sockaddr* addr, const socklen_t len);

    /// @param addr The address to test.
    /// @param unmapped Optional. The string that will receive the unmapped IPv4 address if the object
    /// indeed holds an IPv4 mapped to IPv6 address.
    CK_API static bool is_ipv4_mapped_to_ipv6(const ck_string& addr, ck_string* unmapped=0);

    /// @param addr The address structure that contains the address to test.
    /// @param len The length of the data pointed to by 'addr'
    /// @param unmapped Optional. The string that will receive the unmapped IPv4 address if the object
    /// indeed holds an IPv4 mapped to IPv6 address.
    CK_API static bool is_ipv4_mapped_to_ipv6(const struct sockaddr* addr,
                                              const socklen_t len,
                                              ck_string* unmapped=0);

private:
    void _init_sub_system() const;

    int _port;    ///< The port associated with the address.
    ck_string _addr;    ///< The original address (or hostname) passed to this class.
    struct sockaddr_storage _sockaddr;  ///< The sockaddr structure used for the sockets API.
};

}

#endif
