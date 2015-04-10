
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "XSDK/XUDPSender.h"
#include "XSDK/OS.h"

using namespace XSDK;

XUDPSender::XUDPSender( const XString& targetIP,
                        int targetPort,
                        const XString& localInterfaceIP,
                        int localPort ) :
  XBaseObject(),
  _sok( 0 ),
  _addr( targetPort, targetIP ),
  _localInterfaceIP( localInterfaceIP ),
  _localPort( localPort )
{
    _Configure();
}

XUDPSender::~XUDPSender() throw()
{
    _Close();
}

void XUDPSender::Aim( const XString& targetIP,
              int targetPort )
{
    _addr.SetPortNum( targetPort );
    _addr.SetAddress( targetIP );
    _Configure();
}

int32_t XUDPSender::Send( void* buffer, size_t length )
{
    int32_t bytesSent = sendto( _sok,
                                (char*)buffer,
                                (int)length,
                                0,
                                _addr.GetSockAddr(),
                                _addr.SockAddrSize() );

    if( bytesSent == -1 )
        X_THROW(( "XUDPSender: sendto() returned an error. %s",
                  GetLastErrorMsg().c_str()));

    return bytesSent;
}

bool XUDPSender::SendWouldBlock()
{
    fd_set sendFileDescriptors;
    FD_ZERO( &sendFileDescriptors );

    FD_SET( _sok, &sendFileDescriptors );

    timeval sendTimeout;
    sendTimeout.tv_sec = 0;
    sendTimeout.tv_usec = 0;

    int numSet = select( (int)(_sok + 1), NULL, &sendFileDescriptors, NULL, &sendTimeout );

    if( numSet > 0 )
        return false;

    return true;
}

void XUDPSender::_Configure()
{
    _Close();

    // First, create our datagram socket...
    _sok = socket( _addr.AddressFamily(), SOCK_DGRAM, IPPROTO_UDP );
    if( _sok < 0 )
        X_THROW(( "XUDPSender: Unable to create a datagram socket. %s",
                  GetLastErrorMsg().c_str()));

    // Now, we'd like this socket to optionally use the local interface
    // identified by "localInterfaceIP" if one was passed, or INADDR_ANY
    // if a local interface was not specified.
    XSocketAddress addr( _localPort );
    if (_localInterfaceIP.length() > 0)
        addr.SetAddress( _localInterfaceIP );
    else
        addr.SetAddress( _addr.AddressFamily() == AF_INET ? ip4_addr_any : ip6_addr_any );

    // This call binds our UDP socket to a particular local interface,
    // OR any local interface if localInterfaceIP == "INADDR_ANY" (the
    // default).
    if( bind( _sok, addr.GetSockAddr(), addr.SockAddrSize() ) < 0 )
        X_THROW(( "XUDPSender: Unable to bind to local interface. %s",
                  GetLastErrorMsg().c_str()));
}

void XUDPSender::_Close() throw()
{
#ifndef WIN32
    if( _sok != 0 )
        ::close( _sok );
#else
    if( _sok != 0 )
        closesocket( _sok );
#endif
    _sok = 0;
}

size_t XUDPSender::GetSendBufferSize()
{
    size_t size = 0;
    socklen_t sizelen = sizeof(size);
    const int err = getsockopt(_sok, SOL_SOCKET, SO_SNDBUF, (char*)&size, &sizelen);
    if (err)
        X_THROW(("XUDPSender::GetRecvBufferSize: Failed to get the buffer size. %s", GetLastErrorMsg().c_str()));
    return size;
}

void XUDPSender::SetSendBufferSize(size_t size)
{
    const int err = setsockopt(_sok, SOL_SOCKET, SO_SNDBUF, (char*)&size, sizeof(size));
    if (err)
        X_THROW(("XUDPSender::SetRecvBufferSize: Failed to set the buffer size. %s", GetLastErrorMsg().c_str()));
}
