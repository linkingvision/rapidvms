
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef XSDK_XUDPReceiver_h
#define XSDK_XUDPReceiver_h

#include "XSDK/Platform.h"
#include "XSDK/XSocket.h"
#include "XSDK/XString.h"
#include "XSDK/XMemory.h"
#include "XSDK/Types.h"
#include "XSDK/XBaseObject.h"
#include "XSDK/XSocketAddress.h"

namespace XSDK
{

// 0 here means do not set.
const int DEFAULT_UDP_RECV_BUF_SIZE = 0;

class XUDPReceiver : public XBaseObject
{
public:
    /// @note If destinationAddress is empty, then it defaults to IPv4
    X_API XUDPReceiver(int destinationPort = 0,
                        int recvBufferSize = DEFAULT_UDP_RECV_BUF_SIZE,
                        const XString& destinationAddress = ip4_addr_any);

    X_API virtual ~XUDPReceiver();

    X_API void Connect(const XString& ipAddress, int port);

    X_API virtual bool Receive(int& port, XIRef<XMemory> buffer);

    //Minumum timeout is 250ms.
    X_API virtual bool Receive(int& port, XIRef<XMemory> buffer, int waitMillis);

    X_API virtual bool NonBlockingReceive(int& port, XIRef<XMemory> buffer);

    X_API virtual bool RawReceive(int& port, XIRef<XMemory> buffer);

    X_API virtual void Shutdown();

    X_API virtual bool Valid();

    X_API virtual void Close();

    X_API virtual void Associate(XIRef<XUDPReceiver> receiver);

    X_API virtual void ClearAssociations();

    X_API virtual int GetBoundPort();

    X_API virtual size_t GetRecvBufferSize();

    X_API virtual void SetRecvBufferSize(size_t size);

private:
    XUDPReceiver(const XUDPReceiver& obj);
    XUDPReceiver& operator = (const XUDPReceiver& obj);

    bool _Receive(int& port, XIRef<XMemory> buffer, bool block, int waitMillis);

    SOCKET _sok;
    XSocketAddress _addr;
    std::list<XIRef<XUDPReceiver> > _associatedReceivers;
};

}


#endif
