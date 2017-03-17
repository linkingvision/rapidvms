
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "XSDK/XSocket.h"
#include "XSDK/TimeUtils.h"
#include "XSDK/XGuard.h"
#include "XSDK/OS.h"
#include "XSDK/XMonoClock.h"

#ifdef IS_LINUX

#include <sys/errno.h>
#ifndef __APPLE__
#include <asm-generic/errno.h>
#else
  #define MSG_NOSIGNAL 0x2000 /* don't raise SIGPIPE */
#endif
#include <ifaddrs.h>
#include <poll.h>

#else

#include <Iphlpapi.h>

#endif

using namespace XSDK;
using namespace std;

static const XDuration DEFAULT_RECV_TIMEOUT = XDuration(SECONDS, 5);
static const XDuration DEFAULT_SEND_TIMEOUT = XDuration(SECONDS, 5);
static const int MAX_CONNECTIONS = 5;
static const int POLL_NFDS = 1;

//Used for default validation of the port
bool StubChecker(XString ip, int port);

int XSocket::_sokCount = 0;
XMutex XSocket::_sokLock;

XSocket::XSocket( uint32_t defaultRecvBufferSize ) : XStreamIO(),
    _sok(0),
    _addr(0),
    _recvTimeout( DEFAULT_RECV_TIMEOUT ),
    _recvTimeoutHandler( NULL ),
    _recvTimeoutHandlerOpaque( NULL ),
    _sendTimeout( DEFAULT_SEND_TIMEOUT ),
    _sendTimeoutHandler( NULL ),
    _sendTimeoutHandlerOpaque( NULL ),
    _host(),
    _hostPort(80),
    _recvBuffer( defaultRecvBufferSize ),
    _bufferedBytes( 0 ),
    _recvPos( 0 ),
    _delayClose()
{
    // Lock for socket startup and count increment
    XGuard g( _sokLock );

    if ( _sokCount == 0 )
        SocketStartup();

    _sokCount++;
}

XSocket::XSocket( XSocketType type ) : XStreamIO(),
    _sok(0),
    _addr(0),
    _recvTimeout( DEFAULT_RECV_TIMEOUT ),
    _recvTimeoutHandler( NULL ),
    _recvTimeoutHandlerOpaque( NULL ),
    _sendTimeout( DEFAULT_SEND_TIMEOUT ),
    _sendTimeoutHandler( NULL ),
    _sendTimeoutHandlerOpaque( NULL ),
    _host(),
    _hostPort(80),
    _recvBuffer( DEFAULT_RECV_BUFFER_SIZE ),
    _bufferedBytes( 0 ),
    _recvPos( 0 ),
    _delayClose()
{
    // Lock for socket startup and count increment
    XGuard g( _sokLock );

    if ( _sokCount == 0 )
        SocketStartup();

    _sokCount++;

    if (type == IPV6 || type == IPV6_WITH_IPV4)
    {
        Create(AF_INET6);

        // [tdistler] IPV6_V6ONLY is enabled by default on Windows and disabled on Linux.
        // This code normalizes the behavior by setting the option regardless.
        // [tdistler] Windows XP doesn't support IPV6_V6ONLY as a socket options, so test for that.
        int ipv6only;
        socklen_t optlen = sizeof(ipv6only);
        if (getsockopt(_sok, IPPROTO_IPV6, IPV6_V6ONLY, (char*)&ipv6only, &optlen) == 0)
        {
            ipv6only = 1;
            if (type == IPV6_WITH_IPV4)
                ipv6only = 0;
            if (setsockopt(_sok, IPPROTO_IPV6, IPV6_V6ONLY, (char*)&ipv6only, sizeof(ipv6only)) != 0)
                X_THROW(("Failed to set IPV6_V6ONLY socket option. %s\n\n", GetLastErrorMsg().c_str()));
        }
        else
            X_LOG_WARNING("XSocket: IPV6_V6ONLY socket option not supported on this platform");
    }
    else
        Create(AF_INET);
}

XSocket::~XSocket() throw()
{
    Close();
}

void XSocket::SocketStartup()
{
#ifdef WIN32
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

    wVersionRequested = MAKEWORD( 2, 2 );

    err = WSAStartup( wVersionRequested, &wsaData );
    if ( err != 0 )
        X_STHROW( XSocketException, ( "Unable to load WinSock DLL: %s" , GetErrorMsg(err).c_str() ));

    if ( LOBYTE( wsaData.wVersion ) != 2 ||
         HIBYTE( wsaData.wVersion ) != 2 )
    {
        X_STHROW( XSocketException, ( "Unable to load WinSock DLL: %s", GetErrorMsg(err).c_str() ));
    }
#endif
}

void XSocket::SocketCleanup()
{
#ifdef WIN32
    WSACleanup();
#endif
}

XString XSocket::GetHostname()
{
    char hostname[1024];

    if( gethostname( hostname, 1024 ) != 0 )
        X_STHROW( XSocketException, ( "Unable to get hostname: %s", GetLastErrorMsg().c_str() ));

    return hostname;
}

vector<XString> XSocket::GetAddressesByHostname( const XString& hostname )
{
    vector<XString> addresses;

    struct addrinfo hints, *addrInfo;
    memset(&hints, 0, sizeof (hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_flags = AI_CANONNAME;
    int err = getaddrinfo(hostname.c_str(), 0, &hints, &addrInfo);
    if( err )
        X_STHROW( XSocketException, (hostname, -1, "Failed to get address by hostname: %s", GetErrorMsg(err).c_str()) );

    for( struct addrinfo* cur = addrInfo; cur != 0; cur = cur->ai_next )
    {
        addresses.push_back( XSocketAddress::AddressToString(cur->ai_addr, (socklen_t)cur->ai_addrlen) );
    }

    freeaddrinfo(addrInfo);

    return addresses;
}

std::vector<XString> XSocket::Resolve( int type, const XString& name )
{
    vector<XString> addresses;

    struct addrinfo hints, *addrInfo = NULL;
    memset( &hints, 0, sizeof( hints ) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_flags = AI_CANONNAME;// | AI_NUMERICHOST;

    int err = getaddrinfo( name.c_str(), 0, &hints, &addrInfo );
    if (err)
        X_STHROW( XSocketException, (name, -1, "Failed to resolve address by hostname: %s", GetErrorMsg(err).c_str()) );

    for( struct addrinfo* cur = addrInfo; cur != 0; cur = cur->ai_next )
    {
        // We're only interested in IPv4 and IPv6
        if( (cur->ai_family != AF_INET) && (cur->ai_family != AF_INET6) )
            continue;

        if( cur->ai_addr->sa_family == type )
            addresses.push_back( XSocketAddress::AddressToString(cur->ai_addr, (socklen_t)cur->ai_addrlen) );
    }

    freeaddrinfo( addrInfo );

    return addresses;
}

XHash<std::vector<XString> > XSocket::GetInterfaceAddresses( int addressFamily )
{
    XHash<std::vector<XString> > interfaceAddresses;

#ifdef WIN32
    ULONG adapterInfoBufferSize = (sizeof( IP_ADAPTER_ADDRESSES ) * 32);
    unsigned char adapterInfoBuffer[(sizeof( IP_ADAPTER_ADDRESSES ) * 32)];
    PIP_ADAPTER_ADDRESSES adapterAddress = (PIP_ADAPTER_ADDRESSES)&adapterInfoBuffer[0];

    int err = GetAdaptersAddresses( addressFamily,
                                    0,
                                    0,
                                    adapterAddress,
                                    &adapterInfoBufferSize );

    if( err != ERROR_SUCCESS )
        X_STHROW( XSocketException, ("Unable to query available network interfaces. %s", GetErrorMsg(err).c_str() ));

    while( adapterAddress )
    {
        wchar_t ipstringbuffer[46];
        DWORD ipbufferlength = 46;

        if( WSAAddressToStringW( (LPSOCKADDR)adapterAddress->FirstUnicastAddress->Address.lpSockaddr,
                                 adapterAddress->FirstUnicastAddress->Address.iSockaddrLength,
                                 NULL,
                                 ipstringbuffer,
                                 &ipbufferlength ) == 0 )
        {
            XString key = (PWCHAR)adapterAddress->FriendlyName;
            XString val = ipstringbuffer;

            if( interfaceAddresses.Find(key) == NULL )
            {
                std::vector<XString> addresses;
                interfaceAddresses.Add( key, addresses );

            }
            interfaceAddresses.Find(key)->push_back( val );
        }

        adapterAddress = adapterAddress->Next;
    }
#else

    struct ifaddrs* ifaddrs = NULL, *ifa = NULL;
    int family = 0, s = 0;
    char host[NI_MAXHOST];

    if( getifaddrs( &ifaddrs ) == -1 )
        X_STHROW( XSocketException, ( "Unable to query network interfaces: %s", GetLastErrorMsg().c_str() ));

    for( ifa = ifaddrs; ifa != NULL; ifa = ifa->ifa_next )
    {
        if( ifa->ifa_addr == NULL )
        continue;

        family = ifa->ifa_addr->sa_family;

        if( family != addressFamily )
            continue;

        XString key = ifa->ifa_name;
        s = getnameinfo( ifa->ifa_addr,
                         (family==AF_INET) ?
                             sizeof( struct sockaddr_in ) :
                             sizeof( struct sockaddr_in6 ),
                         host,
                         NI_MAXHOST,
                         NULL,
                         0,
                         NI_NUMERICHOST );

        // s will be 0 if getnameinfo was successful
        if( !s )
        {

            if( interfaceAddresses.Find(key) == NULL )
            {
                std::vector<XString> addresses;
                interfaceAddresses.Add( key, addresses );

            }
            XString val = host;
            interfaceAddresses.Find(key)->push_back( val );
        }
        else
            X_LOG_WARNING("Failed on call to getnameinfo(). %s", GetErrorMsg(s).c_str());
    }

    freeifaddrs( ifaddrs );
#endif

    return interfaceAddresses;
}

void XSocket::Create(unsigned int addrFamily)
{
    if( _sok != 0 )
        return;

    _sok = (SOCKET)::socket(addrFamily, SOCK_STREAM, 0);

    if( _sok <= 0 )
        X_LOG_WARNING("Unable to create socket resource: %s", GetLastErrorMsg().c_str());

    int on = 1;

    int err = (int)::setsockopt( (SOCKET)_sok, SOL_SOCKET, SO_REUSEADDR, (const char *)&on, sizeof(int) );

    if( err < 0 )
        X_STHROW( XSocketException, ( "Unable to configure socket: %s", GetLastErrorMsg().c_str()));
}

bool XSocket::Valid()
{
    return (_sok > 0) ? true : false;
}

void XSocket::Close()
{
    // Note: This function has a subtle interaction with other functions in this class.
    // It is legal to interrupt an Accept(), Send(), or Recv() (and a handfull of other
    // functions) with a call to Close() on another thread. In general, those functions
    // will only throw on a API failure if they still have a valid file descriptor after
    // an api failure. If we set _sok to 0 AFTER a call to close(), there would be a race
    // condition because a system call could return and decide to throw simply because we
    // haven't got to setting it to 0 yet.
    // The solution here is this: First, cache our current FD on the stack. Second, set our
    // FD member to 0. Third, call close on the cached FD.

    if( Valid() && (_delayClose > XDuration::Zero()) )
    {
        WaitRecv( _delayClose );
    }

    int sokTemp = (int)_sok;
    int err = 0;

    _sok = 0;
    FULL_MEM_BARRIER();

#ifdef WIN32
    if( sokTemp != 0 )
        err = closesocket( sokTemp );
#else
    if( sokTemp != 0 )
        err = ::close( sokTemp );
#endif

    if( err < 0)
        X_LOG_WARNING("Failed to close socket: %s", GetLastErrorMsg().c_str());
}

/// Let me now spin for you now a tale of woe so dark it may make you question your sanity. I will begin with
/// a simple question: Do you know how to close a socket? Of course you say... what kind of programmer doesn't
/// understand how to properly close a socket! Well, me apparently. You see, I was under the mistaken
/// impression that all you really needed to do was call close() to close a socket. Oh how wrong I was.
/// If a server calls close() immediately after writing its response 2 bad things may happen: 1) It's likely
/// the tcp stack was not actually done sending all of the data you wrote to the client. 2) The server will
/// almost certainly be the initiator of the close (fin,fin/ack,ack), and the initator of a close will see
/// his sockets accumulate in the TIME_WAIT state. If said server is being hammered by requests, then it is
/// very likely that the server will run out of sockets (especially if connected to by lots of slow links, as a
/// socket may sit in TIME_WAIT state for up to 2 times the MSL!). How can you detect this?
///   netstat -anp | grep PORT_NUMBER. (Note: don't grep by process name as sockets in time_wait wont show up).
/// What the server really needs to do is delay its close until the client has closed it's end (perhaps with
/// some sort of max wait). Luckily, their is a way to do this: If the server calls recv() (or in XSocket's
/// case RawRecv()) after writing its response repeatedly in a loop, a return value of 0 will indicate the
/// client has closed his end of the socket. Additionally, select() and poll() will return upon receipt of a
/// close() from the client so an ever better approach is to wait in a select() (which btw, also gets you a way
/// to have a clean max wait time). So, what about the case of crazy clients that never call close? Well, if
/// you set Linger(0) on your server client socket, a close will cause a RST (instead of the normal fin/ack
/// sequence) which will bypass the TIME_WAIT state.
/// So, is this the ultimate server close? Nope. Even better one would work like this: Linux and Windows both
/// have a means of querying how much application data is waiting in the tcp buffer to be sent. A even better
/// method would call WaitRecv() in a loop as long as that value was getting smaller (so if you had a really
/// HUGE send buffer, and were connected to by really slow clients, you wouldn't cut off their reads).

void XSocket::EnableAutoServerClose( XDuration timeout )
{
    _delayClose = timeout;
}

int XSocket::GetSokId() const
{
    return (int)_sok;
}

/// Causes socket object to give up ownership of underlying OS socket resources.
SOCKET XSocket::ReleaseSokId()
{
    SOCKET tmp = _sok;
    _sok = 0;
    return tmp;
}

/// Causes socket object to take over the specified underlying OS socket resource.
void XSocket::TakeOverSokId( SOCKET sok )
{
    if( sok <= 0 )
        X_STHROW( XSocketException, ( "Invalid socket id passed to TakeOverSokId()."));

    Close();

    _sok = sok;
}

/// Causes socket option SO_LINGER to be set. For reasons why you might want to do this google
/// "the ultimate SO_LINGER page".
void XSocket::Linger( uint16_t lingerSeconds )
{
    struct linger l;
    l.l_onoff = 1;
    l.l_linger = lingerSeconds;
    int err = (int)::setsockopt( (SOCKET)_sok, SOL_SOCKET, SO_LINGER, (const char *)&l, sizeof(struct linger) );
    if( err != 0 )
        X_STHROW( XSocketException, ("Unable to set SO_LINGER socket option.") );
}

int XSocket::ScanPortRange(CheckPortValid checker, XSDK::XString ip, int startRange, int endRange)
{
    CheckPortValid check = StubChecker;
    if ( checker == NULL )
        X_LOG_NOTICE("No port validator given defaulting to Stub, which will always return true");
    X_LOG_INFO("Attempting to scan Host(%s), from(%d), to(%d) ports.",ip.c_str(),startRange,endRange);
    for ( int i = startRange; i < endRange; ++i )
    {
        try
        {//not, do or do not there is no try....
            XSocket socket;
            socket.Create(XSocketAddress::GetAddressFamily(ip));
            X_LOG_INFO("Attempting to connect to: %s:%d",ip.c_str(),i);
            if (!socket.QueryConnect(ip,i,1000))
                continue;
            if ( check(ip,i) )
            {
                X_LOG_INFO("Established Connection on: %d, and check indicates it is valid",i);
                return i;
            }
            else X_LOG_INFO("Established Connection on: %d, check indicates this is not valid",i);
        }catch( std::exception& e )
        {
            X_LOG_NOTICE("While attempting to connect to: [%s:%d], exception occured: %s",ip.c_str(),i,e.what());
        }
    }
    return -1;
}

int XSocket::BindEphemeral( const XString& ip )
{
    int err = 0;

    _addr.SetAddress(ip, 0);
    Create(_addr.AddressFamily());

    err = ::bind( _sok, _addr.GetSockAddr(), _addr.SockAddrSize() );
    if( err < 0 )
        X_STHROW( XSocketException, ( XString("INADDR_ANY"), _addr.Port(), "Unable to bind: %s", GetLastErrorMsg().c_str()));

    sockaddr_in addr;
    int size = sizeof(addr);
    getsockname(_sok,(sockaddr*)&addr,(socklen_t*)&size);
    X_LOG_INFO("Binding to: %d",ntohs(addr.sin_port));
    return ntohs(addr.sin_port);
}

void XSocket::Bind( int port, const XString& ip )
{
    _addr.SetAddress(ip, port);

    Create(_addr.AddressFamily());

    int err = 0;
    err = ::bind( _sok, _addr.GetSockAddr(), _addr.SockAddrSize() );

    if( err < 0 )
        X_STHROW( XSocketException, ( ip, port, "Unable to bind to given port and IP: %s", GetLastErrorMsg().c_str()));
}

void XSocket::Listen()
{
    if( _sok <= 0 )
        X_STHROW( XSocketException, ( "Unable to Listen() on uninitialized socket." ));

    int err = listen( _sok, MAX_CONNECTIONS );

    if( err < 0 )
        X_STHROW( XSocketException, ( _host, _addr.Port(), "Unable to listen on bound port: %s", GetLastErrorMsg().c_str()));
}

XRef<XSocket> XSocket::Accept( uint32_t defaultRecvBufferSize )
{
    XRef<XSocket> clientSocket = new XSocket( defaultRecvBufferSize );

    if( _sok <= 0 )
        X_STHROW( XSocketException, ( "Unable to Accept() on uninitialized socket." ));

    int clientSok = 0;
    socklen_t addrLength = _addr.SockAddrSize();

#ifdef WIN32
    clientSok = (int)::accept( (SOCKET)_sok,
                               _addr.GetSockAddr(),
                               (int *) &addrLength );
#else
    clientSok = ::accept( _sok,
                          _addr.GetSockAddr(),
                          &addrLength );
#endif

    // Since the socket can be closed by another thread while we were waiting in accept(),
    // we only throw here if _sok is still a valid fd.
    if( _sok > 0 && clientSok <= 0 )
        X_STHROW( XSocketException, ( _host, _addr.Port(), "Unable to accept inbound connection: %s", GetLastErrorMsg().c_str()));

    clientSocket->_sok = clientSok;

    return clientSocket;
}

void XSocket::Connect(const XSDK::XString& host, int port, XDuration connectTimeout )
{
    Create( XSocketAddress::GetAddressFamily(host) );

    if (!QueryConnect(host, port, connectTimeout))
        X_STHROW( XSocketException, ( host, port, "Unable to connect to host." ));
}

bool XSocket::QueryConnect(const XSDK::XString& host, int port, XDuration connectTimeout)
{
    if (_sok <= 0)
        Create( XSocketAddress::GetAddressFamily(host) );

    timeval timeout = connectTimeout.ToTimeVal();

    if (setsockopt (_sok, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout,sizeof(timeout)) < 0)
        X_STHROW( XSocketException, ( host, port, "Failed to set receive timeout on socket: %s", GetLastErrorMsg().c_str()));

    if (setsockopt (_sok, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout)) < 0)
        X_STHROW( XSocketException, ( host, port, "Failed to set send timeout on socket: %s", GetLastErrorMsg().c_str()));

    _host = host;
    _hostPort = port;
    _addr.SetAddress( host, port );

    int err = 0;

    err = ::connect( _sok, _addr.GetSockAddr(), _addr.SockAddrSize() );

    if( err < 0 )
    {
       X_LOG_WARNING("Failed to connect on socket in QueryConnect(%s, %d): %s",host.c_str(),port,GetLastErrorMsg().c_str());
        return false;
    }

    return true;
}

uint32_t XSocket::GetHostPort() const
{
   return _hostPort;
}

void XSocket::Shutdown( int mode )
{
    if( _sok <= 0 )
        X_STHROW( XSocketException, ( _host, _addr.Port(), "Unable to Shutdown() on uninitialized socket." ));

    int ret = ::shutdown( _sok, mode );

#ifdef WIN32
    if( ret < 0 )
    {
        int lastWSAError = WSAGetLastError();

        // Proper shutdown of a socket on windows is interestingly complicated. For now,
        // we're taking the easy way out and ignoring not connected errors....
        if( _sok > 0 && lastWSAError != WSAENOTCONN )
            X_STHROW( XSocketException, ( _host, _addr.Port(), "Unable to shutdown socket. %s", GetErrorMsg(lastWSAError).c_str() ));
    }
#else
    // On Linux, errno will be set to ENOTCONN when the client has already
    // closed the socket.
    if( _sok > 0 && errno != ENOTCONN && ret < 0 )
        X_STHROW( XSocketException, ( _host, _addr.Port(), "Unable to shutdown socket: %s", strerror(errno) ));
#endif

}

size_t XSocket::Send( const void* msg, size_t msgLen )
{
    return _Send( msg, msgLen, _sendTimeout );
}

size_t XSocket::Send( const void* msg, size_t msgLen, XDuration sendTimeout )
{
    return _Send( msg, msgLen, sendTimeout );
}

size_t XSocket::Recv( void* buf, size_t msgLen )
{
    return _Recv( buf, msgLen, _recvTimeout );
}

size_t XSocket::Recv( void* buf, size_t msgLen, XDuration recvTimeout )
{
    return _Recv( buf, msgLen, recvTimeout );
}

bool XSocket::BufferedRecv()
{
    size_t recvBufferSize = _recvBuffer.size();
    ssize_t bytesJustReceived = RawRecv( &_recvBuffer[0], recvBufferSize );

    if( bytesJustReceived < 0 )
        return false;

    _recvPos = 0;
    _bufferedBytes = (uint32_t)bytesJustReceived;

    if( bytesJustReceived > 0 )
        return true;

    return false;
}

ssize_t XSocket::RawSend( const void* msg, size_t msgLen )
{
    int ret = 0;

#ifdef WIN32
    ret = ::send(_sok, (char*)msg, (int)msgLen, 0);
#else
    ret = ::send(_sok, msg, msgLen, MSG_NOSIGNAL);

#endif

    return ret;
}

ssize_t XSocket::RawRecv( void* buf, size_t msgLen )
{
    int ret = 0;

#ifdef WIN32
    ret = ::recv(_sok, (char*)buf, (int)msgLen, 0);
#else
    ret = ::recv(_sok, buf, msgLen, 0);

#endif

    return ret;
}

void XSocket::AttachRecvTimeoutHandler( TimeoutHandler rtcb, void* opaque )
{
    _recvTimeoutHandler = rtcb;
    _recvTimeoutHandlerOpaque = opaque;
}

void XSocket::SetRecvTimeout( XDuration timeout )
{
    _recvTimeout = timeout;
}

void XSocket::AttachSendTimeoutHandler( TimeoutHandler stcb, void* opaque )
{
    _sendTimeoutHandler = stcb;
    _sendTimeoutHandlerOpaque = opaque;
}

void XSocket::SetSendTimeout( XDuration timeout )
{
    _sendTimeout = timeout;
}

bool XSocket::WaitRecv( XDuration& waitTime )
{
    if( _SomeBuffered() )
        return false;

    const uint64_t before = XMonoClock::GetTime();
    ssize_t retVal = _CanRecvData(waitTime, (int)_sok);

    if( _sok > 0 && retVal < 0 )
    {
        Close();
        return true;
    }

    const uint64_t after = XMonoClock::GetTime();
    const XDuration delta = XDuration(HNSECS, int64_t(XMonoClock::GetElapsedTime(before, after) * 10000000));

    if( waitTime <= delta || retVal == 0 )
    {
        waitTime = XDuration::Zero();
        return true;
    }

    waitTime -= delta;

    return false;
}

// Returns true on timeout.
bool XSocket::WaitSend( XDuration& waitTime )
{
    const uint64_t before = XMonoClock::GetTime();
    ssize_t retVal = _CanSendData(waitTime, (int)_sok);

    if( _sok > 0 && retVal < 0 )
    {
        Close();
        return true;
    }
    const uint64_t after = XMonoClock::GetTime();
    const XDuration delta = XDuration(HNSECS, int64_t(XMonoClock::GetElapsedTime(before, after) * 10000000));

    if( waitTime <= delta || retVal == 0 )
    {
        waitTime = XDuration::Zero();
        return true;
    }

    waitTime -= delta;

    return false;
}

bool XSocket::ReadyToRecv()
{
    ssize_t retVal = _CanRecvData(XDuration::Zero(), (int)_sok);

    if( _sok > 0 && retVal < 0 )
        return false;

    if( retVal )
        return true;

    return false;
}

bool XSocket::ReadyToSend()
{
    ssize_t retVal = _CanSendData(XDuration::Zero(), (int)_sok);

    if( _sok > 0 && retVal < 0 )
        return false;

    if( retVal )
        return true;

    return false;
}

XString XSocket::GetHost() const
{
    return _host;
}

ssize_t XSocket::_CanRecvData( XDuration waitTime, int fd )
{
#ifdef WIN32
    ssize_t retVal = _DoSelectRecv(waitTime, fd);
#else
    ssize_t retVal = _DoPollRecv(waitTime, fd);
#endif
    return retVal;
}

ssize_t XSocket::_CanSendData( XDuration waitTime, int fd )
{
#ifdef WIN32
    ssize_t retVal = _DoSelectSend(waitTime, fd);
#else
    ssize_t retVal = _DoPollSend(waitTime, fd);
#endif
    return retVal;
}

#ifdef IS_LINUX
ssize_t XSocket::_DoPollRecv( XDuration waitTime, int fd )
{
    struct pollfd fds[POLL_NFDS];
    int nfds = POLL_NFDS;

    fds[0].fd = fd;
#ifndef __APPLE__
    fds[0].events = POLLIN | POLLHUP | POLLERR | POLLNVAL | POLLRDHUP;
#else
    fds[0].events = POLLIN | POLLHUP | POLLERR | POLLNVAL;    
#endif

    int retVal = poll(fds, nfds, waitTime.Total(MSECS) );

    // The socket may have been closed, but there might be data in
    // the buffer to read first
    if( fds[0].revents & POLLIN )
        return retVal;

    //Check for error conditions
    //if the remote side shut down their socket, POLLRDHUP will be active.
#ifndef __APPLE__
    if( fds[0].revents & ( POLLERR | POLLRDHUP | POLLHUP | POLLNVAL) )
#else
    if( fds[0].revents & ( POLLERR | POLLHUP | POLLNVAL) )    
#endif
        retVal = -1;

    return retVal;
}

ssize_t XSocket::_DoPollSend( XDuration waitTime, int fd )
{
    struct pollfd fds[POLL_NFDS];
    int nfds = POLL_NFDS;

    fds[0].fd = fd;
    fds[0].events = POLLOUT | POLLHUP;

    int retVal = poll(fds, nfds, waitTime.Total(MSECS) );

    if( fds[0].revents & POLLHUP )
        retVal = -1;

    return retVal;
}
#endif

ssize_t XSocket::_DoSelectRecv( XDuration waitTime, int fd )
{
    fd_set recvFileDescriptors;
    FD_ZERO( &recvFileDescriptors );
    FD_SET( fd, &recvFileDescriptors );

    timeval recvTimeout = waitTime.ToTimeVal();
    return select( fd+1, &recvFileDescriptors, NULL, NULL, &recvTimeout );
}

ssize_t XSocket::_DoSelectSend( XDuration waitTime, int fd )
{
    fd_set sendFileDescriptors;

    FD_ZERO( &sendFileDescriptors );

    FD_SET( fd, &sendFileDescriptors );

    timeval sendTimeout = waitTime.ToTimeVal();
    return select( fd+1, NULL, &sendFileDescriptors, NULL, &sendTimeout );
}

size_t XSocket::_Send( const void* msg, size_t msgLen, XDuration sendTimeout )
{
    if( !msg || (msgLen <= 0) )
        X_STHROW( XSocketException, ( _host, _addr.Port(), "Invalid argument passed to XSocket::Send()."));

    bool timedOut = false;
    XDuration waitTime = sendTimeout;

    size_t bytesToWrite = msgLen;
    unsigned char* writer = (unsigned char*)msg;

    while( _sok > 0 && !timedOut && (bytesToWrite > 0) )
    {
        ssize_t bytesJustWritten = RawSend( writer, bytesToWrite );

        // If we get here, and we're unable to send any bytes, something
        // funny is going on (perhaps the other hand has closed their
        // socket), so just bail.

        if( bytesJustWritten < 0 )
        {
#ifdef IS_LINUX
            if( (errno != EAGAIN) && (errno != EWOULDBLOCK) )
#endif
            {
                Close();

                return (msgLen - bytesToWrite);
            }
        }

        if( bytesJustWritten > 0 )
        {
            bytesToWrite -= bytesJustWritten;
            writer += bytesJustWritten;
        }

        if( bytesToWrite > 0 )
        {
            // WaitSend() will decrement waitTime by the amount of time waited each time...
		/* so platform will change while waittime, such macos */
		waitTime = sendTimeout;
		timedOut = WaitSend( waitTime );

            if( timedOut )
            {
                // callback returns true if we should try again...
                timedOut = (_sendTimeoutHandler) ? !_sendTimeoutHandler( _sendTimeoutHandlerOpaque ) : false;

                if( !timedOut )
                    waitTime = sendTimeout;
            }
        }
    }

    return (msgLen - bytesToWrite);
}

size_t XSocket::_Recv( void* buf, size_t msgLen, XDuration recvTimeout )
{
    if( !buf )
        X_STHROW( XSocketException, ( _host, _addr.Port(), "Invalid buffer passed to XSocket::Recv()."));

    if( msgLen <= 0 )
        X_STHROW( XSocketException, ( _host, _addr.Port(), "Invalid msgLen passed to XSocket::Recv(). [%u]", msgLen ));

    bool timedOut = false;
    XDuration waitTime = recvTimeout;

    size_t bytesToRecv = msgLen;
    unsigned char* writer = (unsigned char*)buf;

    while( _sok > 0 && !timedOut && (bytesToRecv > 0) )
    {
        ssize_t bytesJustReceived = 0;

        if( _SomeBuffered() )
            bytesJustReceived = _ReadRecvBuffer( writer, bytesToRecv );
        else
        {
            /* so platform will change while waittime, such macos */
            waitTime = recvTimeout;
	
            timedOut = WaitRecv( waitTime );

            if( timedOut )
            {
                X_LOG_INFO("Receive on socket timed out.");

                // callback returns true if we should try again...
                bool tryAgain = (_recvTimeoutHandler) ? _recvTimeoutHandler( _recvTimeoutHandlerOpaque ) : false;

                if( tryAgain )
                {
                    timedOut = false;
                    waitTime = recvTimeout; // If we're here, we need to retry... so reset our timeout.
                }

                continue;
            }

            bytesJustReceived = RawRecv( &_recvBuffer[0], _recvBuffer.size() );

            if( bytesJustReceived <= 0 )
            {
#ifdef IS_LINUX
                if( (errno != EAGAIN) && (errno != EWOULDBLOCK) )
#endif
                {
                    Close();

                    continue;  // In this case, _sok will be set to 0, so continuing here will result in the loop
                               // terminating... as one of the conditions of the loop is _sok > 0.
                }
            }
            else
            {
                // We got some data...

                _recvPos = 0;

                _bufferedBytes = (uint32_t)bytesJustReceived;

                continue; // If we read some bytes, we loop here so that we go into the SomeBuffered() case above.
            }
        }

        if( bytesJustReceived > 0 )
        {
            bytesToRecv -= bytesJustReceived;
            writer += bytesJustReceived;
        }
    }
    return (msgLen - bytesToRecv);
}

size_t XSocket::_ReadRecvBuffer( void* buf, size_t msgLen )
{
    if( _SomeBuffered() )
    {
        if( _bufferedBytes > (uint32_t)msgLen )
        {
            const unsigned char* pos = &_recvBuffer[_recvPos];

            memcpy( buf, pos, msgLen );

            _recvPos += (uint32_t)msgLen;

            _bufferedBytes -= (uint32_t)msgLen;

            return msgLen;
        }
        else
        {
            const unsigned char* pos = &_recvBuffer[_recvPos];

            uint32_t bytesToWrite = _bufferedBytes;

            memcpy( buf, pos, bytesToWrite );

            _bufferedBytes = 0;
            _recvPos = 0;

            return bytesToWrite;
        }
    }

    return 0;
}

bool XSocket::_SomeBuffered()
{
    return (_bufferedBytes > 0) ? true : false;
}

bool StubChecker(XString ip, int port)
{
    return true;
}

XString XSocket::GetPeerIp() const
{
    struct sockaddr_storage peer;
    int peerLength = sizeof(peer);

#ifdef WIN32
    if ( getpeername(_sok,(sockaddr*)&peer,&peerLength) < 0 )
    {
        X_LOG_WARNING("Unable to get peer ip. %s", GetLastErrorMsg().c_str());
        return "";
    }
#else
    if ( getpeername(_sok,(sockaddr*)&peer,(socklen_t*)&peerLength) < 0 )
    {
        X_LOG_WARNING("Unable to get peer ip: %s", GetLastErrorMsg().c_str());
        return "";
    }
#endif

    return XSocketAddress::AddressToString((sockaddr*)&peer, (socklen_t)peerLength);
}

XString XSocket::GetLocalIp() const
{
    struct sockaddr_storage local;
    int addrLength = sizeof(local);

#ifdef WIN32
    if ( getsockname(_sok, (sockaddr*)&local, &addrLength) < 0 )
    {
        X_LOG_WARNING("Unable to get local ip. %s", GetLastErrorMsg().c_str());
        return "";
    }
#else
    if ( getsockname(_sok, (sockaddr*)&local, (socklen_t*)&addrLength) < 0 )
    {
        X_LOG_WARNING("Unable to get local ip: %s", GetLastErrorMsg().c_str());
        return "";
    }
#endif

    return XSocketAddress::AddressToString((sockaddr*)&local, (socklen_t)addrLength);
}


XSocketException::XSocketException()
    : XException(),
      ip(""),
      port(-1)
{}

XSocketException::~XSocketException() throw()
{}

XSocketException::XSocketException(const char* msg, ...)
    : XException(),
      ip(),
      port(-1)
{
    va_list args;
    va_start(args, msg);
    SetMsg(XString::Format(msg, args));
    va_end(args);
}

XSocketException::XSocketException(const XSDK::XString& ip, int port, const char* msg, ...)
    : XException(),
      ip(ip),
      port(port)
{
    va_list args;
    va_start(args, msg);
    SetMsg(XString::Format("ip [%s], port [%d]: ", ip.c_str(), port) + XString::Format(msg, args));
    va_end(args);
}

XSocketException::XSocketException(const XString& msg)
    : XException(msg),
      ip(),
      port(-1)
{}

XSocketException::XSocketException(const XSDK::XString& ip, int port, const XSDK::XString& msg)
    : XException(XString::Format("ip [%s], port [%d]: ", ip.c_str(), port) + msg),
      ip(ip),
      port(port)
{}
