
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "XSDK/XUDPReceiver.h"
#include "XSDK/OS.h"
#include "XSDK/TimeUtils.h"

using namespace XSDK;

XUDPReceiver::XUDPReceiver(int destinationPort,
                            int recvBufferSize,
                            const XString& destinationAddress) :
    XBaseObject(),
    _sok(0),
    _addr(destinationPort, destinationAddress),
    _associatedReceivers()
{
    // Map wildcard addresses (0.0.0.0 or ::) to the addr_any type.
    if (_addr.IsWildcardAddress())
        _addr.SetAddress(_addr.IsIPv4() ? ip4_addr_any : ip6_addr_any);

    // First, create our datagram socket...
    _sok = socket(_addr.AddressFamily(), SOCK_DGRAM, IPPROTO_UDP);
    if(_sok <= 0)
        X_THROW(("XUDPReceiver: Unable to create a datagram socket. %s",
                  GetLastErrorMsg().c_str()));

    int on = 1;

    int err = (int)::setsockopt((SOCKET)_sok, SOL_SOCKET, SO_REUSEADDR, (const char *)&on, sizeof(int));

    if(err < 0)
        X_THROW(("XUDPReceiver: Unable to configure socket. %s", GetLastErrorMsg().c_str()));

    if(recvBufferSize > 0)
    {
        err = setsockopt(_sok, SOL_SOCKET, SO_RCVBUF, (char*)&recvBufferSize, sizeof(recvBufferSize));

        if(err < 0)
            X_THROW(("XUDPReceiver: Unable to configure socket rcvbuf. %s", GetLastErrorMsg().c_str()));
    }

    if (_addr.IsIPv6())
    {
       // [tdistler] IPV6_V6ONLY is enabled by default on Windows and disabled on Linux.
       // This code normalizes the behavior by setting the option regardless.
       // [tdistler] Windows XP doesn't support IPV6_V6ONLY as a socket options, so test for that.
       int ipv6only;
       socklen_t optlen = sizeof(ipv6only);
       if (getsockopt(_sok, IPPROTO_IPV6, IPV6_V6ONLY, (char*)&ipv6only, &optlen) == 0)
       {
          ipv6only = 0;
          if (setsockopt(_sok, IPPROTO_IPV6, IPV6_V6ONLY, (char*)&ipv6only, sizeof(ipv6only)) != 0)
             X_THROW(("Failed to set IPV6_V6ONLY socket option. %s\n\n", GetLastErrorMsg().c_str()));
       }
       else
          X_LOG_WARNING("[XUDPReceiver]: IPV6_V6ONLY socket option not supported on this platform");
    }

    if(XSocketAddress::IsMulticast(destinationAddress))
    {
#ifdef WIN32
        // [tdistler] Required b/c binding to multicast addr on Windows is an error.
        XSocketAddress addr(destinationPort, _addr.AddressFamily() == AF_INET6 ? ip6_addr_any : ip4_addr_any);

        if(bind(_sok, addr.GetSockAddr(), addr.SockAddrSize()) < 0)
            X_THROW(("XUDPReceiver: Unable to bind to local interface. %s",
                      GetLastErrorMsg().c_str()));
#else
        if(bind(_sok, _addr.GetSockAddr(), _addr.SockAddrSize()) < 0)
            X_THROW(("XUDPReceiver: Unable to bind to local interface. %s",
                      GetLastErrorMsg().c_str()));
#endif
    }
    else
    {
        if(bind(_sok, _addr.GetSockAddr(), _addr.SockAddrSize()) < 0)
            X_THROW(("XUDPReceiver: Unable to bind to local interface. %s",
                      GetLastErrorMsg().c_str()));
    }

    // If an XUDPReceiver is created with a default destinationPort (0), then use getsockname()
    // to discover the actual local port that was used by bind() and then SET that port on _addr.
    // This allows a subsequent call to GetBoundPort() to return a discovered free port rather than
    // 0. If the destinationPort was valid and we bound to it, all this does is reset _addr to the
    // value it already had.
    sockaddr_storage sa;
    memset(&sa, 0, sizeof(sa));
    int size = sizeof(sa);
    getsockname(_sok, (sockaddr*)&sa, (socklen_t*)&size);
    if(sa.ss_family == AF_INET)
        _addr.SetPortNum(ntohs(((sockaddr_in*)&sa)->sin_port));
    else _addr.SetPortNum(ntohs(((sockaddr_in6*)&sa)->sin6_port));

    if(_addr.Address().length() > 0)
    {
        if(XSocketAddress::IsMulticast(_addr.Address()))
        {
            int level, optname, optlen;
            char *optval = 0;

            if(_addr.AddressFamily() == AF_INET)
            {
                struct ip_mreq v4mreq;
                level = IPPROTO_IP;
                optname = IP_ADD_MEMBERSHIP;
                optval = (char*)&v4mreq;
                optlen = sizeof(v4mreq);
                memset(&v4mreq, 0, sizeof(v4mreq));
                v4mreq.imr_multiaddr.s_addr = ((struct sockaddr_in*)_addr.GetSockAddr())->sin_addr.s_addr;
                v4mreq.imr_interface.s_addr = htonl(INADDR_ANY);
            }
            else
            {
                X_ASSERT(_addr.AddressFamily() == AF_INET6);
                struct ipv6_mreq v6mreq;
                level = IPPROTO_IPV6;
                optname = IPV6_JOIN_GROUP;
                optval = (char*)&v6mreq;
                optlen = sizeof(v6mreq);
                memset(&v6mreq, 0, sizeof(v6mreq));
                v6mreq.ipv6mr_multiaddr = ((struct sockaddr_in6*)_addr.GetSockAddr())->sin6_addr;
                v6mreq.ipv6mr_interface = 0;
            }

            setsockopt(_sok, level, optname, optval, optlen);
        }
    }
}

XUDPReceiver::~XUDPReceiver()
{
    Close();
}

/// Note:
/// Connect() can be used to filter incoming UDP packets by ip address. Note: the port passed to
/// connect is the SRC port. You probably want to pass 0 for the port here.
void XUDPReceiver::Connect(const XString& ipAddress, int port)
{
    XSocketAddress addr(port, ipAddress);

    if(::connect(_sok, addr.GetSockAddr(), addr.SockAddrSize()))
    {
        X_THROW(("XUDPReceiver::Connect: Failed to connect to %s:%d : %s",
                 ipAddress.c_str(),
                 port,
                 GetLastErrorMsg().c_str()));
    }
}

bool XUDPReceiver::Receive(int& port, XIRef<XMemory> buffer)
{
    return _Receive(port, buffer, true, 0);
}

bool XUDPReceiver::Receive(int& port, XIRef<XMemory> buffer, int waitMillis)
{
    // We could accept lower values, but if you really want them that low, you
    // probably should be doing a non-blocking call, and if a low enough value
    // is given to select, then it never times out (e.g. with some testing on Windows,
    // it seemed to work with 10ms but not 1ms, and it might vary from machine to
    // machine or OS to OS). And passing 250ms to select works well in almost all
    // cases, so we don't want to mess with that without good reason.
    X_ASSERT(waitMillis >= 250 && "Minimum wait time for XUDPReceiver::Receive is 250ms");
    return _Receive(port, buffer, true, waitMillis);
}

bool XUDPReceiver::NonBlockingReceive(int& port, XIRef<XMemory> buffer)
{
    return _Receive(port, buffer, false, 0);
}

bool XUDPReceiver::_Receive(int& port, XIRef<XMemory> buffer, bool block, int waitMillis)
{
    fd_set readFileDescriptors;
    int selectRet = 0;
    struct timeval beforeSelect = { 0, 0 };
    x_gettimeofday(&beforeSelect);

    while((_sok > 0) && (selectRet == 0))
    {
        struct timeval tv = { 0, block ? 250000 : 0 };

        FD_ZERO(&readFileDescriptors);

        SOCKET currentLargestSOK = _sok;

        // First, add ourseleves...
        FD_SET(_sok, &readFileDescriptors);

        // Next, loop over all the associated sockets and add them to the set.
        std::list<XIRef<XUDPReceiver> >::iterator i;
        for(i = _associatedReceivers.begin(); i != _associatedReceivers.end(); i++)
        {
            FD_SET((*i)->_sok, &readFileDescriptors);

            if((*i)->_sok > currentLargestSOK)
                currentLargestSOK = (*i)->_sok;
        }

        selectRet = select((int)(currentLargestSOK + 1),
                           &readFileDescriptors,
                           NULL,
                           NULL,
                           &tv);

        // Check for timeout (if one is provided)
        if(waitMillis > 0 && selectRet == 0)
        {
            struct timeval afterSelect = { 0, 0 };
            x_gettimeofday(&afterSelect);

            struct timeval delta = { 0, 0 };
            timersub(&afterSelect, &beforeSelect, &delta);

            int deltaMillis = (delta.tv_sec * 1000) + (delta.tv_usec / 1000);

            if(((waitMillis - deltaMillis) <= 0))
                return false;
        }
        else if(!block)
            break;
    }

    if(selectRet > 0)
    {
        if(FD_ISSET(_sok, &readFileDescriptors))
        {
            unsigned char bytes[2048];

            int bytesReceived = recv(_sok,
                                      (char*)bytes,
                                      2048,
                                      0);
            if(_sok <= 0)
                return false;

            if(bytesReceived <= 0)
                return false;

            buffer->Clear();

            unsigned char* writer = &buffer->Extend(bytesReceived);

            memcpy(writer, bytes, bytesReceived);

            port = _addr.Port();

            return true;
        }
        else
        {
            std::list<XIRef<XUDPReceiver> >::iterator i;
            for(i = _associatedReceivers.begin(); i != _associatedReceivers.end(); i++)
            {
                XIRef<XUDPReceiver> receiver = *i;

                if(FD_ISSET(receiver->_sok, &readFileDescriptors))
                    return receiver->Receive(port, buffer);
            }
        }
    }

    return false;
}

bool XUDPReceiver::RawReceive(int& port, XIRef<XMemory> buffer)
{
    buffer->ResizeData(2048);

    socklen_t sockLen = _addr.SockAddrSize();
    int bytesReceived = recvfrom(_sok,
#ifdef WIN32
                                  (char*)buffer->Map(),
#else
                                  buffer->Map(),
#endif
                                  2048,
                                  0,
                                  _addr.GetSockAddr(),
                                  &sockLen);

    buffer->ResizeData((bytesReceived <= 0) ? 0 : bytesReceived);

    if(_sok <= 0)
        return false;

    if(bytesReceived <= 0)
        return false;

    return true;
}

void XUDPReceiver::Shutdown()
{
    shutdown(_sok, SOCKET_SHUT_FLAGS);
}

bool XUDPReceiver::Valid()
{
    return (_sok > 0) ? true : false;
}

void XUDPReceiver::Close()
{
    // I'm adding this because I have seen cases where Close() alone is not
    // enough to unblock a recvfrom() on another thread, but shutdown() does
    // the trick. on linux!
    Shutdown();

    SOCKET tmpSok = _sok;

    _sok = 0;

    FULL_MEM_BARRIER();

#ifndef WIN32
    if(tmpSok != 0)
        ::close(tmpSok);
#else
    if(tmpSok != 0)
        closesocket(tmpSok);
#endif
}

void XUDPReceiver::Associate(XIRef<XUDPReceiver> receiver)
{
    _associatedReceivers.push_back(receiver);
}

void XUDPReceiver::ClearAssociations()
{
    _associatedReceivers.clear();
}

int XUDPReceiver::GetBoundPort()
{
    return _addr.Port();
}

size_t XUDPReceiver::GetRecvBufferSize()
{
    size_t size = 0;
    socklen_t sizelen = sizeof(size);
    int err = getsockopt(_sok, SOL_SOCKET, SO_RCVBUF, (char*)&size, &sizelen);
    if (err)
        X_THROW(("XUDPReceiver::GetRecvBufferSize: Failed to get the buffer size. %s", GetLastErrorMsg().c_str()));
    return size;
}

void XUDPReceiver::SetRecvBufferSize(size_t size)
{
    int err = setsockopt(_sok, SOL_SOCKET, SO_RCVBUF, (char*)&size, sizeof(size));
    if (err)
        X_THROW(("XUDPReceiver::SetRecvBufferSize: Failed to set the buffer size. %s", GetLastErrorMsg().c_str()));
}
