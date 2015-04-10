
#include "cppkit/ck_types.h"
#include "cppkit/ck_udp_receiver.h"
#include "cppkit/os/ck_platform.h"
#include "cppkit/os/ck_time_utils.h"
#include "cppkit/os/ck_error_msg.h"

using namespace cppkit;
using namespace std;

ck_udp_receiver::ck_udp_receiver( int destinationPort,
                                  int recvBufferSize,
                                  const ck_string& destinationAddress ) :
    _sok( 0 ),
    _addr( destinationPort, destinationAddress ),
    _associatedReceivers()
{
    // Map wildcard addresses (0.0.0.0 or ::) to the addr_any type.
    if( _addr.is_wildcard_address() )
        _addr.set_address( _addr.is_ipv4() ? ip4_addr_any : ip6_addr_any );

    // First, create our datagram socket...
    _sok = socket( _addr.address_family(), SOCK_DGRAM, IPPROTO_UDP );
    if( _sok <= 0 )
        CK_THROW(( "ck_udp_receiver: Unable to create a datagram socket. %s",
                   ck_get_last_error_msg().c_str() ));

    int on = 1;

    int err = (int)::setsockopt( (SOCKET)_sok, SOL_SOCKET, SO_REUSEADDR, (const char *)&on, sizeof(int) );

    if( err < 0 )
        CK_THROW(( "ck_udp_receiver: Unable to configure socket. %s", ck_get_last_error_msg().c_str() ));

    if( recvBufferSize > 0 )
    {
        err = setsockopt( _sok, SOL_SOCKET, SO_RCVBUF, (char*)&recvBufferSize, sizeof( recvBufferSize ) );

        if( err < 0 )
            CK_THROW(( "ck_udp_receiver: Unable to configure socket rcvbuf. %s", ck_get_last_error_msg().c_str() ));
    }

    if( _addr.is_ipv6() )
    {
       // [tdistler] IPV6_V6ONLY is enabled by default on Windows and disabled on Linux.
       // This code normalizes the behavior by setting the option regardless.
       // [tdistler] Windows XP doesn't support IPV6_V6ONLY as a socket options, so test for that.
       int ipv6only;
       socklen_t optlen = sizeof( ipv6only );
       if( getsockopt( _sok, IPPROTO_IPV6, IPV6_V6ONLY, (char*)&ipv6only, &optlen ) == 0 )
       {
          ipv6only = 0;
          if( setsockopt( _sok, IPPROTO_IPV6, IPV6_V6ONLY, (char*)&ipv6only, sizeof(ipv6only) ) != 0 )
             CK_THROW(( "Failed to set IPV6_V6ONLY socket option. %s\n\n", ck_get_last_error_msg().c_str() ));
       }
       else
          CK_LOG_WARNING( "[ck_udp_receiver]: IPV6_V6ONLY socket option not supported on this platform" );
    }

    if( ck_socket_address::is_multicast( destinationAddress ) )
    {
#ifdef IS_WINDOWS
        // [tdistler] Required b/c binding to multicast addr on Windows is an error.
        ck_socket_address addr( destinationPort, _addr.address_family() == AF_INET6 ? ip6_addr_any : ip4_addr_any );

        if( ::bind( _sok, addr.get_sock_addr(), addr.sock_addr_size() ) < 0 )
            CK_THROW(( "ck_udp_receiver: Unable to bind to local interface. %s",
                       ck_get_last_error_msg().c_str() ));
#else
        if( bind( _sok, _addr.get_sock_addr(), _addr.sock_addr_size() ) < 0 )
            CK_THROW(( "ck_udp_receiver: Unable to bind to local interface. %s",
                       ck_get_last_error_msg().c_str() ));
#endif
    }
    else
    {
        if( ::bind( _sok, _addr.get_sock_addr(), _addr.sock_addr_size()) < 0 )
            CK_THROW(( "ck_udp_receiver: Unable to bind to local interface. %s",
                       ck_get_last_error_msg().c_str() ));
    }

    // If an ck_udp_receiver is created with a default destinationPort (0), then use getsockname()
    // to discover the actual local port that was used by bind() and then SET that port on _addr.
    // This allows a subsequent call to GetBoundPort() to return a discovered free port rather than
    // 0. If the destinationPort was valid and we bound to it, all this does is reset _addr to the
    // value it already had.
    sockaddr_storage sa;
    memset( &sa, 0, sizeof( sa ) );
    int size = sizeof( sa );
    getsockname( _sok, (sockaddr*)&sa, (socklen_t*)&size );
    if( sa.ss_family == AF_INET )
        _addr.set_port_num( ntohs(((sockaddr_in*)&sa)->sin_port) );
    else _addr.set_port_num( ntohs(((sockaddr_in6*)&sa)->sin6_port) );

    if( _addr.address().length() > 0 )
    {
        if( ck_socket_address::is_multicast( _addr.address() ) )
        {
            int level, optname, optlen;
            char *optval = 0;

            if( _addr.address_family() == AF_INET )
            {
                struct ip_mreq v4mreq;
                level = IPPROTO_IP;
                optname = IP_ADD_MEMBERSHIP;
                optval = (char*)&v4mreq;
                optlen = sizeof( v4mreq );
                memset( &v4mreq, 0, sizeof( v4mreq ) );
                v4mreq.imr_multiaddr.s_addr = ((struct sockaddr_in*)_addr.get_sock_addr())->sin_addr.s_addr;
                v4mreq.imr_interface.s_addr = htonl( INADDR_ANY );
            }
            else
            {
                struct ipv6_mreq v6mreq;
                level = IPPROTO_IPV6;
                optname = IPV6_JOIN_GROUP;
                optval = (char*)&v6mreq;
                optlen = sizeof( v6mreq );
                memset( &v6mreq, 0, sizeof( v6mreq ) );
                v6mreq.ipv6mr_multiaddr = ((struct sockaddr_in6*)_addr.get_sock_addr())->sin6_addr;
                v6mreq.ipv6mr_interface = 0;
            }

            setsockopt( _sok, level, optname, optval, optlen );
        }
    }
}

ck_udp_receiver::~ck_udp_receiver() throw()
{
    close();
}

/// Note:
/// connect() can be used to filter incoming UDP packets by ip address. Note: the port passed to
/// connect is the SRC port. You probably want to pass 0 for the port here.
void ck_udp_receiver::connect( const ck_string& ipAddress, int port )
{
    ck_socket_address addr( port, ipAddress );

    if( ::connect( _sok, addr.get_sock_addr(), addr.sock_addr_size() ) )
    {
        CK_THROW(( "ck_udp_receiver::connect: Failed to connect to %s:%d : %s",
                   ipAddress.c_str(),
                   port,
                   ck_get_last_error_msg().c_str() ));
    }
}

bool ck_udp_receiver::receive( int& port, shared_ptr<ck_memory> buffer )
{
    return _receive( port, buffer, true, 0 );
}

bool ck_udp_receiver::receive( int& port, shared_ptr<ck_memory> buffer, int waitMillis )
{
    // We could accept lower values, but if you really want them that low, you
    // probably should be doing a non-blocking call, and if a low enough value
    // is given to select, then it never times out (e.g. with some testing on Windows,
    // it seemed to work with 10ms but not 1ms, and it might vary from machine to
    // machine or OS to OS). And passing 250ms to select works well in almost all
    // cases, so we don't want to mess with that without good reason.

    return _receive( port, buffer, true, waitMillis );
}

bool ck_udp_receiver::non_blocking_receive( int& port, shared_ptr<ck_memory> buffer )
{
    return _receive( port, buffer, false, 0 );
}

bool ck_udp_receiver::_receive( int& port, shared_ptr<ck_memory> buffer, bool block, int waitMillis )
{
    fd_set readFileDescriptors;
    int selectRet = 0;
    struct timeval beforeSelect = { 0, 0 };
    ck_gettimeofday( &beforeSelect );

    while( (_sok > 0) && (selectRet == 0) )
    {
        struct timeval tv = { 0, block ? 250000 : 0 };

        FD_ZERO( &readFileDescriptors );

        SOCKET currentLargestSOK = _sok;

        // First, add ourseleves...
        FD_SET( _sok, &readFileDescriptors );

        // Next, loop over all the associated sockets and add them to the set.
        std::list<shared_ptr<ck_udp_receiver> >::iterator i;
        for( i = _associatedReceivers.begin(); i != _associatedReceivers.end(); i++ )
        {
            FD_SET( (*i)->_sok, &readFileDescriptors );

            if( (*i)->_sok > currentLargestSOK )
                currentLargestSOK = (*i)->_sok;
        }

        selectRet = select( currentLargestSOK + 1,
                            &readFileDescriptors,
                            NULL,
                            NULL,
                            &tv );

        // Check for timeout (if one is provided)
        if( (waitMillis > 0) && (selectRet == 0) )
        {
            struct timeval afterSelect = { 0, 0 };
            ck_gettimeofday( &afterSelect );

            struct timeval delta = { 0, 0 };
            timersub( &afterSelect, &beforeSelect, &delta );

            int deltaMillis = (delta.tv_sec * 1000) + (delta.tv_usec / 1000);

            if( ((waitMillis - deltaMillis) <= 0) )
                return false;
        }
        else if( !block )
            break;
    }

    if( selectRet > 0 )
    {
        if( FD_ISSET( _sok, &readFileDescriptors ) )
        {
            unsigned char bytes[2048];

            int bytesReceived = recv( _sok,
                                      (char*)bytes,
                                      2048,
                                      0 );
            if( _sok <= 0 )
                return false;

            if( bytesReceived <= 0 )
                return false;

            buffer->clear();

            ck_byte_ptr writer = buffer->extend_data( bytesReceived );

            memcpy( writer.get_ptr(), bytes, bytesReceived );

            port = _addr.port();

            return true;
        }
        else
        {
            for( auto i = _associatedReceivers.begin(); i != _associatedReceivers.end(); i++)
            {
                shared_ptr<ck_udp_receiver> receiver = *i;

                if(FD_ISSET(receiver->_sok, &readFileDescriptors))
                    return receiver->receive(port, buffer);
            }
        }
    }

    return false;
}

bool ck_udp_receiver::raw_receive(int& port, shared_ptr<ck_memory> buffer)
{
    buffer->resize( 2048 );

    socklen_t sockLen = _addr.sock_addr_size();
    int bytesReceived = recvfrom( _sok,
#ifdef IS_WINDOWS
                                  (char*)buffer->map().get_ptr(),
#else
                                  buffer->map().get_ptr(),
#endif
                                  2048,
                                  0,
                                  _addr.get_sock_addr(),
                                  &sockLen );

    buffer->set_data_size( (bytesReceived <= 0) ? 0 : bytesReceived );

    if( _sok <= 0 )
        return false;

    if( bytesReceived <= 0 )
        return false;

    return true;
}

void ck_udp_receiver::shutdown()
{
    ::shutdown( _sok, SOCKET_SHUT_FLAGS );
}

void ck_udp_receiver::close()
{
    // I'm adding this because I have seen cases where close() alone is not
    // enough to unblock a recvfrom() on another thread, but shutdown() does
    // the trick. on linux!
    shutdown();

    SOCKET tmpSok = _sok;

    _sok = 0;

    FULL_MEM_BARRIER();

#ifndef IS_WINDOWS
    if( tmpSok != 0 )
        ::close( tmpSok );
#else
    if( tmpSok != 0 )
        closesocket( tmpSok );
#endif
}

void ck_udp_receiver::associate( shared_ptr<ck_udp_receiver> receiver )
{
    _associatedReceivers.push_back( receiver );
}

void ck_udp_receiver::clear_associations()
{
    _associatedReceivers.clear();
}

int ck_udp_receiver::get_bound_port()
{
    return _addr.port();
}

size_t ck_udp_receiver::get_recv_buffer_size()
{
    size_t size = 0;
    socklen_t sizelen = sizeof( size );

    if( getsockopt( _sok, SOL_SOCKET, SO_RCVBUF, (char*)&size, &sizelen ) != 0 )
        CK_THROW(( "ck_udp_receiver::GetRecvBufferSize: Failed to get the buffer size. %s",
                   ck_get_last_error_msg().c_str() ));
    return size;
}

void ck_udp_receiver::set_recv_buffer_size(size_t size)
{
    if( setsockopt( _sok, SOL_SOCKET, SO_RCVBUF, (char*)&size, sizeof(size) ) != 0 )
        CK_THROW(( "ck_udp_receiver::SetRecvBufferSize: Failed to set the buffer size. %s",
                   ck_get_last_error_msg().c_str() ));
}
