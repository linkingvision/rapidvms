
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef XSDK_XSocket_h
#define XSDK_XSocket_h

#include "XSDK/XDuration.h"
#include "XSDK/Platform.h"
#include "XSDK/XHash.h"
#include "XSDK/XStreamIO.h"

#include <vector>

#ifdef WIN32

    #include <WinSock2.h>
    #include <ws2tcpip.h>

    #define SOCKET_SHUT_FLAGS SD_BOTH
    #define SOCKET_SHUT_SEND_FLAGS SD_SEND
    #define SOCKET_SHUT_RECV_FLAGS SD_RECEIVE

#else

    #include <sys/types.h>
    #include <sys/socket.h>
    #include <sys/time.h>
    #include <netinet/in.h>
    #include <netdb.h>
    #include <unistd.h>
    #include <arpa/inet.h>

    typedef int SOCKET;

    #define SOCKET_SHUT_FLAGS SHUT_RDWR
    #define SOCKET_SHUT_SEND_FLAGS SHUT_WR
    #define SOCKET_SHUT_RECV_FLAGS SHUT_RD

#endif

#include "XSDK/XSocketAddress.h"
#include "XSDK/XMutex.h"
#include "XSDK/XString.h"

// tdistler: Added 16 and 32 bit versions so you don't have to link against winsock on windows.

#define x_ntohs(x) ( (((x) >> 8) & 0xff) | (((x) << 8) & 0xff00) )

#define x_htons(x) x_ntohs(x)

#define x_ntohl(x) ( (((x) >> 24) & 0xff) | (((x) >> 8) & 0xff00) | (((x) << 8) & 0xff0000) | (((x) << 24) & 0xff000000) )

#define x_htonl(x) x_ntohl(x)

#define x_ntohll(x) ( ( (uint64_t)(ntohl( (uint32_t)((x << 32) >> 32) )) << 32) | ntohl( ((uint32_t)(x >> 32)) ) )

#define x_htonll(x) x_ntohll(x)

namespace XSDK
{

/// This typedef describes the interface to a function that will be called when
/// their is either a send or recv timeout. A return value of true means "try again".
typedef bool (*TimeoutHandler)( void* opaque );

/// This object represents a simple BSD style tcp socket connection. If you are
/// fammiliar with standard BSD or Winsock programming, you can probably discern
/// how this object works just from the function names.
///
/// The functions in XSocket can be roughly categorized into 3 buckets:
///     1) Setup Functions
///        Create(), Bind(), Listen(), Connect()
///     2) Live Functions
///        Accept(), Send(), Recv(), RawSend(), RawRecv(), WaitSend(),
///        WaitRecv()
///     3) Shutdown Functions
///        Shutdown(), Close()
/// In general, it is illegal to call shutdown functions from 1 thread,
/// while calling setup functions from another. It IS legal, however, to
/// call shutdown functions from 1 thread, while calling Live functions
/// from another (tho, these functions obviousley may not succeed, you
/// should not get exceptions in that case).

class XSocket : public XStreamIO
{
public:
    enum XSocketEnum
    {
        DEFAULT_RECV_BUFFER_SIZE = 1024
    };

    typedef enum _XSocketType
    {
        IPV4,           ///< Create an IPv4 socket only
        IPV6,           ///< Create an IPv6 socket only
        IPV6_WITH_IPV4  ///< Create an IPv6 socket that can receive traffic via IPv4
    } XSocketType;

    /// Created an unconnected socket object.
    X_API XSocket( uint32_t defaultRecvBufferSize = DEFAULT_RECV_BUFFER_SIZE );

    /// Create an unconnected socket object.
    /// @param type The type of socket to create.
    X_API XSocket( XSocketType type );

    /// Close and destroy this object.
    X_API virtual ~XSocket() throw();

    X_API virtual bool Valid();

    /// Release this sockets file descriptor...
    X_API virtual void Close();

    /// If enabled, causes default Close() to WaitRecv() UP TO timeout  for the client to
    /// disconnect first. Client first disconnection avoids TIME_WAIT issues on the server and guarantees
    /// the data has been read by the client.
    X_API virtual void EnableAutoServerClose( XDuration timeout );
    X_API virtual void EnableAutoServerClose( int timeoutMillis )
    { EnableAutoServerClose(XDuration(MSECS, timeoutMillis)); }

    /// Return the sockets ID.
    X_API virtual int GetSokId() const;

    /// Causes socket object to give up ownership of underlying OS socket resources.
    X_API SOCKET ReleaseSokId();

    /// Causes socket object to take over the specified underlying OS socket resource.
    X_API void TakeOverSokId( SOCKET sok );

    /// Causes socket option SO_LINGER to be set.
    X_API virtual void Linger( uint16_t lingerSeconds = 30 );

    /// Bind this socket to a particular port.
    /// \param port The port to bind to.
    /// \param ip The interface address to bind to
    X_API virtual void Bind( int port, const XString& ip = "" );

    /// Bind to any available high-number port.
    /// \param ip The interface address to bind to
    /// \return The port bound to.
    X_API int BindEphemeral( const XString& ip = "" );

    /// Begin listening on the bound port.
    X_API virtual void Listen();

    /// Accept a new connection and returns a socket for that connection.
    X_API virtual XRef<XSocket> Accept( uint32_t defaultRecvBufferSize = DEFAULT_RECV_BUFFER_SIZE );

    /// Connect to a host. Throws if unsuccessful.
    /// \param host The IP address of the server you'd like to connect to.
    /// \param port The tcp port you'd like to ring on.
    /// \param connectTimeout The length of time to wait beyond this it should throw.
    X_API virtual void Connect(const XSDK::XString& host, int port, XDuration connectTimeout = XDuration(SECONDS, 30));
    X_API virtual void Connect(const XSDK::XString& host, int port, int connectTimeoutMillis)
    { Connect(host, port, XDuration(MSECS, connectTimeoutMillis)); }

    /// Attempts to connect to a host.
    /// \param host The IP address of the server you'd like to connect to.
    /// \param port The tcp port you'd like to ring on.
    /// \return 'true' if the connect succeeded, 'false' otherwise.
    X_API virtual bool QueryConnect(const XSDK::XString& host, int port, XDuration connectTimeout = XDuration(SECONDS, 30));
    X_API virtual bool QueryConnect(const XSDK::XString& host, int port, int connectTimeoutMillis)
    { return QueryConnect(host, port, XDuration(MSECS, connectTimeoutMillis)); }

    /// Close this open socket.
    X_API virtual void Shutdown( int mode );

    /// Send data to server. May generate send timeout callbacks.
    /// \param msg A pointer to a memory buffer that is the SOURCE of data to send.
    /// \param msgLen The length of the data to send.
    X_API virtual size_t Send( const void* msg, size_t msgLen );

    /// Send data to server. May generate send timeout callbacks.
    /// \param msg A pointer to a memory buffer that is the SOURCE of data to send.
    /// \param msgLen The length of the data to send.
    /// \param sendTimeout Override send timeout for this call only.
    X_API virtual size_t Send( const void* msg, size_t msgLen, XDuration sendTimeout );
    X_API virtual size_t Send( const void* msg, size_t msgLen, int sendTimeoutMillis )
    { return XStreamIO::Send(msg, msgLen, sendTimeoutMillis); }

    /// Receive data from server. May generate recv timeout callbacks.
    /// \param buf A pointer to a memory buffer that is filled in by the incoming data.
    /// \param msgLen The length of the data to receive.
    X_API virtual size_t Recv( void* buf, size_t msgLen );

    /// Receive data from server. May generate recv timeout callbacks.
    /// \param buf A pointer to a memory buffer that is filled in by the incoming data.
    /// \param msgLen The length of the data to receive.
    /// \param recvTimeout Override recv timeout for this call only.
    X_API virtual size_t Recv( void* buf, size_t msgLen, XDuration recvTimeout );
    X_API virtual size_t Recv( void* buf, size_t msgLen, int recvTimeoutMillis )
    { return XStreamIO::Recv(buf, msgLen, recvTimeoutMillis); }

    /// Attempts to pre buffer some data to be received.
    /// Returns true if some data was buffered.
    X_API virtual bool BufferedRecv();

    /// Send data to server. May write less bytes than asked for.
    /// \param msg A pointer to a memory buffer that is the SOURCE of data to send.
    /// \param msgLen The length of the data to send.
    X_API virtual ssize_t RawSend( const void* msg, size_t msgLen );

    /// Receive data from server. May read less bytes than asked for.
    /// \param buf A pointer to a memory buffer that is filled in by the incoming data.
    /// \param msgLen The length of the data to receive.
    X_API virtual ssize_t RawRecv( void* buf, size_t msgLen );

    /// Attach a handler function pointer to send notifications about recv timeouts.
    /// \param rtcb A pointer to a function to call when recv times out.
    /// \param opaque User data to be passed to rtcb.
    X_API virtual void AttachRecvTimeoutHandler( TimeoutHandler rtcb, void* opaque );

    /// Explicitly set a recv timeout value.
    /// \param timeout The length of time to attempt a recv before failing.
    X_API virtual void SetRecvTimeout( XDuration timeout );
    X_API virtual void SetRecvTimeout( int timeoutMillis ) { XStreamIO::SetRecvTimeout(timeoutMillis); }

    /// Attach a handler function pointer to send notifications about send timeouts.
    /// \param stcb A pointer to a function to call when send times out.
    /// \param opaque User data to be passed to stcb.
    X_API virtual void AttachSendTimeoutHandler( TimeoutHandler stcb, void* opaque );

    /// Explicitly set a send timeout value.
    /// \param time The length of time to attempt a send before failing.
    X_API virtual void SetSendTimeout( XDuration timeout );
    X_API virtual void SetSendTimeout( int timeoutMillis ) { XStreamIO::SetSendTimeout(timeoutMillis); }

    /// Wait for data to be available to be received on the socket.
    X_API virtual bool WaitRecv( XDuration& waitTime );
    X_API virtual bool WaitRecv( int& waitTimeMillis ) { return XStreamIO::WaitRecv(waitTimeMillis); }

    /// Wait for socket clear to send.
    X_API virtual bool WaitSend( XDuration& waitTime );
    X_API virtual bool WaitSend( int& waitTimeMillis ) { return XStreamIO::WaitSend(waitTimeMillis); }

    /// Returns true if a RawRecv() would not block.
    X_API virtual bool ReadyToRecv();

    /// Returns true if a RawSend() would not block.
    X_API virtual bool ReadyToSend();

    /// Get Host used for Connect
    X_API virtual XSDK::XString GetHost() const;
    X_API virtual uint32_t GetHostPort() const;

    X_API virtual XSDK::XString GetPeerIp() const;

    X_API virtual XSDK::XString GetLocalIp() const;

    /// Do Socket Startup for Application
    X_API static void SocketStartup();

    /// Do Socket Cleanup for Application
    X_API static void SocketCleanup();

    X_API static XString GetHostname();
    X_API static std::vector<XString> GetAddressesByHostname( const XString& hostname );

    X_API static std::vector<XString> Resolve( int type, const XString& name );

    X_API static XHash<std::vector<XString> > GetInterfaceAddresses( int addressFamily = AF_INET );

    typedef bool (*CheckPortValid)( XSDK::XString ip, int port );

    X_API static int ScanPortRange( CheckPortValid checker,
                                    XSDK::XString ip,
                                    int startRange,
                                    int endRange );

private:

    /// Basic socket initialization.
    void Create(unsigned int addrFamily);

    XSocket( const XSocket& obj );
    XSocket& operator = ( const XSocket& obj );

    X_API size_t _Send( const void* msg, size_t msgLen, XDuration sendTimeout );
    X_API size_t _Recv( void* buf, size_t msgLen, XDuration recvTimeout );

    X_API size_t _ReadRecvBuffer( void* buf, size_t msgLen );
    X_API bool _SomeBuffered();

    X_API ssize_t _CanRecvData( XDuration waitTime, int fd );
    X_API ssize_t _CanSendData( XDuration waitTime, int fd );

    X_API ssize_t _DoSelectRecv( XDuration waitTime, int fd );
    X_API ssize_t _DoSelectSend( XDuration waitTime, int fd );

#ifndef WIN32
    X_API ssize_t _DoPollRecv( XDuration waitTime, int fd );
    X_API ssize_t _DoPollSend( XDuration waitTime, int fd );
#endif

    static int _sokCount;
    static XMutex _sokLock;
    SOCKET _sok;
    XSocketAddress _addr;
    XDuration _recvTimeout;
    XSDK::TimeoutHandler _recvTimeoutHandler;
    void* _recvTimeoutHandlerOpaque;
    XDuration _sendTimeout;
    XSDK::TimeoutHandler _sendTimeoutHandler;
    void* _sendTimeoutHandlerOpaque;
    XSDK::XString _host;
    uint32_t _hostPort;

    std::vector<uint8_t> _recvBuffer;
    uint32_t _bufferedBytes;
    uint32_t _recvPos;

    XDuration _delayClose;
};


class XSocketException : public XException
{
public:

    X_API XSocketException();

    X_API virtual ~XSocketException() throw();

    X_API XSocketException(const char* msg, ...);

    X_API XSocketException(const XSDK::XString& ip, int port, const char* msg, ...);

    X_API XSocketException(const XSDK::XString& msg);

    X_API XSocketException(const XSDK::XString& ip, int port, const XSDK::XString& msg);


    XString ip;
    int     port;
};

}

#endif
