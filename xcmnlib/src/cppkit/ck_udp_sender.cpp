
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

#include "cppkit/ck_udp_sender.h"
#include "cppkit/os/ck_platform.h"
#include "cppkit/os/ck_error_msg.h"

using namespace cppkit;

ck_udp_sender::ck_udp_sender( const ck_string& targetIP,
                              int targetPort,
                              const ck_string& localInterfaceIP,
                              int localPort ) :
  _sok( 0 ),
  _addr( targetPort, targetIP ),
  _localInterfaceIP( localInterfaceIP ),
  _localPort( localPort )
{
    _configure();
}

ck_udp_sender::~ck_udp_sender() throw()
{
    _close();
}

void ck_udp_sender::aim( const ck_string& targetIP, int targetPort )
{
    _addr.set_port_num( targetPort );
    _addr.set_address( targetIP );

    _configure();
}

int32_t ck_udp_sender::send( void* buffer, size_t length )
{
    int32_t bytesSent = sendto( _sok,
                                (char*)buffer,
                                length,
                                0,
                                _addr.get_sock_addr(),
                                _addr.sock_addr_size() );

    if( bytesSent == -1 )
        CK_THROW(( "ck_udp_sender: sendto() returned an error. %s",
                   ck_get_last_error_msg().c_str() ));

    return bytesSent;
}

void ck_udp_sender::_configure()
{
    _close();

    // First, create our datagram socket...
    _sok = socket( _addr.address_family(), SOCK_DGRAM, IPPROTO_UDP );
    if( _sok < 0 )
        CK_THROW(( "ck_udp_sender: Unable to create a datagram socket. %s",
                   ck_get_last_error_msg().c_str() ));

    // Now, we'd like this socket to optionally use the local interface
    // identified by "localInterfaceIP" if one was passed, or INADDR_ANY
    // if a local interface was not specified.
    ck_socket_address addr( _localPort );
    if (_localInterfaceIP.length() > 0)
        addr.set_address( _localInterfaceIP );
    else
        addr.set_address( _addr.address_family() == AF_INET ? ip4_addr_any : ip6_addr_any );

    // This call binds our UDP socket to a particular local interface,
    // OR any local interface if localInterfaceIP == "INADDR_ANY" (the
    // default).
    if( bind( _sok, addr.get_sock_addr(), addr.sock_addr_size() ) < 0 )
        CK_THROW(( "ck_udp_sender: Unable to bind to local interface. %s",
                  ck_get_last_error_msg().c_str() ));
}

void ck_udp_sender::_close() throw()
{
#ifdef IS_WINDOWS
    if( _sok != 0 )
        closesocket( _sok );
#else
    if( _sok != 0 )
        ::close( _sok );
#endif

    _sok = 0;
}

size_t ck_udp_sender::get_send_buffer_size()
{
    size_t size = 0;
    socklen_t sizelen = sizeof( size );
    const int err = getsockopt( _sok, SOL_SOCKET, SO_SNDBUF, (char*)&size, &sizelen );
    if (err)
        CK_THROW(( "ck_udp_sender::GetRecvBufferSize: Failed to get the buffer size. %s",
                   ck_get_last_error_msg().c_str() ));

    return size;
}

void ck_udp_sender::set_send_buffer_size( size_t size )
{
    const int err = setsockopt( _sok, SOL_SOCKET, SO_SNDBUF, (char*)&size, sizeof(size) );
    if (err)
        CK_THROW(( "ck_udp_sender::SetRecvBufferSize: Failed to set the buffer size. %s",
                   ck_get_last_error_msg().c_str() ));
}
