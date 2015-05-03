
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef XSDK_XUDPSender_h
#define XSDK_XUDPSender_h

#include "XSDK/Platform.h"
#include "XSDK/XString.h"
#include "XSDK/XMemory.h"
#include "XSDK/Types.h"
#include "XSDK/XBaseObject.h"
#include "XSDK/XSocketAddress.h"

#ifdef WIN32
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

    typedef int SOCKET;
#endif

namespace XSDK
{

class XUDPSender : public XBaseObject
{
public:
    X_API XUDPSender( const XString& targetIP,
              int targetPort,
              const XString& localInterfaceIP = "",
              int localPort = 0 );

    X_API virtual ~XUDPSender() throw();

    X_API void Aim( const XString& targetIP,
            int targetPort );

    X_API int32_t Send( void* buffer, size_t length );

    X_API bool SendWouldBlock();

    X_API virtual size_t GetSendBufferSize();

    X_API virtual void SetSendBufferSize(size_t size);

private:
    XUDPSender( const XUDPSender& obj );
    XUDPSender& operator = ( const XUDPSender& obj );

    void _Configure();
    void _Close() throw();

    SOCKET _sok;
    XSocketAddress _addr;
    XString _localInterfaceIP;
    int _localPort;
};

}


#endif
