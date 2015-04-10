
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

#include "cppkit/ck_socket.h"
#include "cppkit/os/ck_time_utils.h"
#include "cppkit/os/ck_error_msg.h"

#ifdef IS_LINUX
  #include <sys/errno.h>
  #include <asm-generic/errno.h>
  #include <ifaddrs.h>
  #include <poll.h>
#endif

#ifdef IS_WINDOWS
  #include <Iphlpapi.h>
#endif

#include <mutex>

using namespace cppkit;
using namespace std;

static const int DEFAULT_RECV_TIMEOUT = 5000;
static const int DEFAULT_SEND_TIMEOUT = 5000;
static const int MAX_CONNECTIONS = 5;
static const int POLL_NFDS = 1;

//Used for default validation of the port
bool stub_checker(ck_string ip, int port);

int ck_socket::_sokCount = 0;
recursive_mutex ck_socket::_sokLock;

ck_socket::ck_socket( uint32_t defaultRecvBufferSize )
    : ck_stream_io(),
      _sok(0),
      _addr(0),
      _recvTimeoutMilliseconds( DEFAULT_RECV_TIMEOUT ),
      _recvTimeoutHandler(),
      _sendTimeoutMilliseconds( DEFAULT_SEND_TIMEOUT ),
      _sendTimeoutHandler(),
      _host(),
      _hostPort(80),
      _recvBuffer( defaultRecvBufferSize ),
      _bufferedBytes( 0 ),
      _recvPos( 0 ),
      _delayClose( 0 )
{
    // Lock for socket startup and count increment
    lock_guard<recursive_mutex> g( _sokLock );

    if ( _sokCount == 0 )
        socket_startup();

    _sokCount++;
}

ck_socket::ck_socket( ck_socket_type type )
    : ck_stream_io(),
      _sok(0),
      _addr(0),
      _recvTimeoutMilliseconds( DEFAULT_RECV_TIMEOUT ),
      _recvTimeoutHandler(),
      _sendTimeoutMilliseconds( DEFAULT_SEND_TIMEOUT ),
      _sendTimeoutHandler(),
      _host(),
      _hostPort(80),
      _recvBuffer( DEFAULT_RECV_BUFFER_SIZE ),
      _bufferedBytes( 0 ),
      _recvPos( 0 ),
      _delayClose( 0 )
{
    // Lock for socket startup and count increment
    lock_guard<recursive_mutex> g( _sokLock );

    if ( _sokCount == 0 )
        socket_startup();

    _sokCount++;

    if (type == IPV6 || type == IPV6_WITH_IPV4)
    {
        create(AF_INET6);

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
                CK_THROW(("Failed to set IPV6_V6ONLY socket option. %s\n\n", ck_get_last_error_msg().c_str()));
        }
        else
            CK_LOG_WARNING("ck_socket: IPV6_V6ONLY socket option not supported on this platform");
    }
    else
        create(AF_INET);
}

ck_socket::~ck_socket() throw()
{
    this->close();

    // Lock to decrement counter
    lock_guard<recursive_mutex> g( _sokLock );
    _sokCount--;
}

void ck_socket::socket_startup()
{
#ifdef IS_WINDOWS
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

    wVersionRequested = MAKEWORD( 2, 2 );

    err = WSAStartup( wVersionRequested, &wsaData );
    if ( err != 0 )
        CK_STHROW( ck_socket_exception, ( "Unable to load WinSock DLL." ));

    if ( LOBYTE( wsaData.wVersion ) != 2 ||
         HIBYTE( wsaData.wVersion ) != 2 )
    {
        CK_STHROW( ck_socket_exception, ( "Unable to load WinSock DLL." ));
    }
#endif
}

void ck_socket::socket_cleanup()
{
#ifdef IS_WINDOWS
    WSACleanup();
#endif
}

ck_string ck_socket::get_hostname()
{
    char hostname[1024];

    if( gethostname( hostname, 1024 ) != 0 )
        CK_STHROW( ck_socket_exception, ( "Unable to get hostname." ));

    return hostname;
}

vector<ck_string> ck_socket::get_addresses_by_hostname( const ck_string& hostname )
{
    vector<ck_string> addresses;

    struct addrinfo hints, *addrInfo;
    memset(&hints, 0, sizeof (hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_flags = AI_CANONNAME;
    int err = getaddrinfo(hostname.c_str(), 0, &hints, &addrInfo);
    if( err )
        CK_STHROW( ck_socket_exception, (hostname, -1, "Failed to get address by hostname: %s", ck_get_error_msg(err).c_str()) );

    for( struct addrinfo* cur = addrInfo; cur != 0; cur = cur->ai_next )
    {
        addresses.push_back( ck_socket_address::address_to_string(cur->ai_addr, cur->ai_addrlen) );
    }

    freeaddrinfo(addrInfo);

    return addresses;
}

std::vector<ck_string> ck_socket::resolve( int type, const ck_string& name )
{
    vector<ck_string> addresses;

    struct addrinfo hints, *addrInfo = nullptr;
    memset( &hints, 0, sizeof( hints ) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_flags = AI_CANONNAME;// | AI_NUMERICHOST;

    int err = getaddrinfo( name.c_str(), 0, &hints, &addrInfo );
    if (err)
        CK_STHROW( ck_socket_exception, (name, -1, "Failed to resolve address by hostname: %s", ck_get_error_msg(err).c_str()) );

    for( struct addrinfo* cur = addrInfo; cur != 0; cur = cur->ai_next )
    {
        // We're only interested in IPv4 and IPv6
        if( (cur->ai_family != AF_INET) && (cur->ai_family != AF_INET6) )
            continue;

        if( cur->ai_addr->sa_family == type )
            addresses.push_back( ck_socket_address::address_to_string(cur->ai_addr, cur->ai_addrlen) );
    }

    freeaddrinfo( addrInfo );

    return addresses;
}

unordered_map<string,vector<ck_string> > ck_socket::get_interface_addresses( int addressFamily )
{
    unordered_map<string,vector<ck_string> > interfaceAddresses;

#ifdef IS_WINDOWS
    ULONG adapterInfoBufferSize = (sizeof( IP_ADAPTER_ADDRESSES ) * 32);
    unsigned char adapterInfoBuffer[(sizeof( IP_ADAPTER_ADDRESSES ) * 32)];
    PIP_ADAPTER_ADDRESSES adapterAddress = (PIP_ADAPTER_ADDRESSES)&adapterInfoBuffer[0];

    int err = GetAdaptersAddresses( addressFamily,
                                    0,
                                    0,
                                    adapterAddress,
                                    &adapterInfoBufferSize );

    if( err != ERROR_SUCCESS )
        CK_STHROW( ck_socket_exception, ("Unable to query available network interfaces. %s", ck_get_error_msg(err).c_str() ));

    while( adapterAddress )
    {
        wchar_t ipstringbuffer[46];
        DWORD ipbufferlength = 46;

        if( WSAAddressToStringW( (LPSOCKADDR)adapterAddress->FirstUnicastAddress->Address.lpSockaddr,
                                 adapterAddress->FirstUnicastAddress->Address.iSockaddrLength,
                                 nullptr,
                                 ipstringbuffer,
                                 &ipbufferlength ) == 0 )
        {
            ck_string key = (PWCHAR)adapterAddress->FriendlyName;
            ck_string val = ipstringbuffer;

            if( interfaceAddresses.find(key) == interfaceAddresses.end() )
            {
                std::vector<ck_string> addresses;
                interfaceAddresses.insert( make_pair(key, addresses) );
            }
            interfaceAddresses.find(key)->second.push_back( val );
        }

        adapterAddress = adapterAddress->Next;
    }
#endif
#ifdef IS_LINUX
    struct ifaddrs* ifaddrs = nullptr, *ifa = nullptr;
    int family = 0, s = 0;
    char host[NI_MAXHOST];

    if( getifaddrs( &ifaddrs ) == -1 )
        CK_STHROW( ck_socket_exception, ( "Unable to query network interfaces: %s", ck_get_last_error_msg().c_str() ));

    for( ifa = ifaddrs; ifa != nullptr; ifa = ifa->ifa_next )
    {
        if( ifa->ifa_addr == nullptr )
        continue;

        family = ifa->ifa_addr->sa_family;

        if( family != addressFamily )
            continue;

        ck_string key = ifa->ifa_name;
        s = getnameinfo( ifa->ifa_addr,
                         (family==AF_INET) ?
                             sizeof( struct sockaddr_in ) :
                             sizeof( struct sockaddr_in6 ),
                         host,
                         NI_MAXHOST,
                         nullptr,
                         0,
                         NI_NUMERICHOST );

        // s will be 0 if getnameinfo was successful
        if( !s )
        {

            if( interfaceAddresses.find(key) == interfaceAddresses.end() )
            {
                std::vector<ck_string> addresses;
                interfaceAddresses.insert( make_pair(key, addresses) );
            }
            ck_string val = host;
            interfaceAddresses.find(key)->second.push_back( val );
        }
        else
            CK_LOG_WARNING("Failed on call to getnameinfo(). %s", ck_get_error_msg(s).c_str());
    }

    freeifaddrs( ifaddrs );
#endif

    return interfaceAddresses;
}

void ck_socket::create(unsigned int addrFamily)
{
    if( _sok != 0 )
        return;

    _sok = (SOCKET)::socket(addrFamily, SOCK_STREAM, 0);

    if( _sok <= 0 )
        CK_LOG_WARNING("Unable to create socket resource: %s", ck_get_last_error_msg().c_str());

    int on = 1;

    int err = (int)::setsockopt( (SOCKET)_sok, SOL_SOCKET, SO_REUSEADDR, (const char *)&on, sizeof(int) );

    if( err < 0 )
        CK_STHROW( ck_socket_exception, ( "Unable to configure socket: %s", ck_get_last_error_msg().c_str()));
}

bool ck_socket::valid()
{
    return (_sok > 0) ? true : false;
}

void ck_socket::close()
{
    // Note: This function has a subtle interaction with other functions in this class.
    // It is legal to interrupt an accept(), send(), or Recv() (and a handfull of other
    // functions) with a call to close() on another thread. In general, those functions
    // will only throw on a API failure if they still have a valid file descriptor after
    // an api failure. If we set _sok to 0 AFTER a call to close(), there would be a race
    // condition because a system call could return and decide to throw simply because we
    // haven't got to setting it to 0 yet.
    // The solution here is this: First, cache our current FD on the stack. Second, set our
    // FD member to 0. Third, call close on the cached FD.

    if( _delayClose > 0 )
    {
        wait_recv( _delayClose );
    }

    int sokTemp = _sok;
    int err = 0;

    _sok = 0;

#ifdef IS_WINDOWS
    if( sokTemp != 0 )
        err = closesocket( sokTemp );
#endif
#ifdef IS_LINUX
    if( sokTemp != 0 )
        err = ::close( sokTemp );
#endif

    if( err < 0)
        CK_LOG_WARNING("Failed to close socket: %s", ck_get_last_error_msg().c_str());
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

void ck_socket::enable_auto_server_close( int timeout )
{
    _delayClose = timeout;
}

int ck_socket::get_sok_id() const
{
    return _sok;
}

/// Causes socket object to give up ownership of underlying OS socket resources.
SOCKET ck_socket::release_sok_id()
{
    SOCKET tmp = _sok;
    _sok = 0;
    return tmp;
}

/// Causes socket object to take over the specified underlying OS socket resource.
void ck_socket::take_over_sok_id( SOCKET sok )
{
    if( sok <= 0 )
        CK_STHROW( ck_socket_exception, ( "Invalid socket id passed to take_over_sok_id()."));

    this->close();

    _sok = sok;
}

void ck_socket::linger( uint16_t lingerSeconds )
{
    struct linger l;
    l.l_onoff = 1;
    l.l_linger = lingerSeconds;
    int err = (int)::setsockopt( (SOCKET)_sok, SOL_SOCKET, SO_LINGER, (const char *)&l, sizeof(struct linger) );
    if( err != 0 )
        CK_STHROW( ck_socket_exception, ("Unable to set SO_LINGER socket option.") );
}

int ck_socket::bind_ephemeral( const ck_string& ip )
{
    int err = 0;

    _addr.set_address(ip, 0);
    create(_addr.address_family());

    err = ::bind( _sok, _addr.get_sock_addr(), _addr.sock_addr_size() );
    if( err < 0 )
        CK_STHROW( ck_socket_exception, ( ck_string("INADDR_ANY"), _addr.port(), "Unable to bind: %s", ck_get_last_error_msg().c_str()));

    sockaddr_in addr;
    int size = sizeof(addr);
    getsockname(_sok,(sockaddr*)&addr,(socklen_t*)&size);
    CK_LOG_INFO("Binding to: %d",ntohs(addr.sin_port));
    return ntohs(addr.sin_port);
}

void ck_socket::bind( int port, const ck_string& ip )
{
    _addr.set_address(ip, port);

    create(_addr.address_family());

    int err = 0;
    err = ::bind( _sok, _addr.get_sock_addr(), _addr.sock_addr_size() );

    if( err < 0 )
        CK_STHROW( ck_socket_exception, ( ip, port, "Unable to bind to given port and IP: %s", ck_get_last_error_msg().c_str()));
}

void ck_socket::listen()
{
    if( _sok <= 0 )
        CK_STHROW( ck_socket_exception, ( "Unable to listen() on uninitialized socket." ));

    int err = ::listen( _sok, MAX_CONNECTIONS );

    if( err < 0 )
        CK_STHROW( ck_socket_exception, ( _host, _addr.port(), "Unable to listen on bound port: %s", ck_get_last_error_msg().c_str()));
}

shared_ptr<ck_socket> ck_socket::accept( uint32_t defaultRecvBufferSize )
{
    shared_ptr<ck_socket> clientSocket = make_shared<ck_socket>( defaultRecvBufferSize );

    if( _sok <= 0 )
        CK_STHROW( ck_socket_exception, ( "Unable to accept() on uninitialized socket." ));

    int clientSok = 0;
    socklen_t addrLength = _addr.sock_addr_size();

#ifdef IS_WINDOWS
    clientSok = (int)::accept( (SOCKET)_sok,
                               _addr.get_sock_addr(),
                               (int *) &addrLength );
#endif
#ifdef IS_LINUX
    clientSok = ::accept( _sok,
                          _addr.get_sock_addr(),
                          &addrLength );
#endif

    // Since the socket can be closed by another thread while we were waiting in accept(),
    // we only throw here if _sok is still a valid fd.
    if( _sok > 0 && clientSok <= 0 )
        CK_STHROW( ck_socket_exception, ( _host, _addr.port(), "Unable to accept inbound connection: %s", ck_get_last_error_msg().c_str()));

    clientSocket->_sok = clientSok;

    return clientSocket;
}

void ck_socket::connect(const ck_string& host, int port, int connectTimeoutMillis )
{
    create( ck_socket_address::get_address_family(host) );

    if (!query_connect(host, port, connectTimeoutMillis))
        CK_STHROW( ck_socket_exception, ( host, port, "Unable to connect to host." ));
}

bool ck_socket::query_connect(const ck_string& host, int port, int connectTimeoutMillis)
{
    if (_sok <= 0)
        create( ck_socket_address::get_address_family(host) );

    struct timeval timeout;
    timeout.tv_sec = (connectTimeoutMillis / 1000);
    timeout.tv_usec = ((connectTimeoutMillis % 1000) * 1000);


    if (setsockopt (_sok, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout,sizeof(timeout)) < 0)
        CK_STHROW( ck_socket_exception, ( host, port, "Failed to set receive timeout on socket: %s", ck_get_last_error_msg().c_str()));

    if (setsockopt (_sok, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout)) < 0)
        CK_STHROW( ck_socket_exception, ( host, port, "Failed to set send timeout on socket: %s", ck_get_last_error_msg().c_str()));

    _host = host;
    _hostPort = port;
    _addr.set_address( host, port );

    int err = 0;

    err = ::connect( _sok, _addr.get_sock_addr(), _addr.sock_addr_size() );

    if( err < 0 )
    {
       CK_LOG_WARNING("Failed to connect on socket in query_connect(%s, %d): %s",host.c_str(),port,ck_get_last_error_msg().c_str());
        return false;
    }

    return true;
}

uint32_t ck_socket::get_host_port() const
{
   return _hostPort;
}

void ck_socket::shutdown( int mode )
{
    if( _sok <= 0 )
        CK_STHROW( ck_socket_exception, ( _host, _addr.port(), "Unable to shutdown() on uninitialized socket." ));

    int ret = ::shutdown( _sok, mode );

#ifdef IS_WINDOWS
    if( ret < 0 )
    {
        int lastWSAError = WSAGetLastError();

        // Proper shutdown of a socket on windows is interestingly complicated. For now,
        // we're taking the easy way out and ignoring not connected errors....
        if( _sok > 0 && lastWSAError != WSAENOTCONN )
            CK_STHROW( ck_socket_exception, ( _host, _addr.port(), "Unable to shutdown socket. %s", ck_get_error_msg(lastWSAError).c_str() ));
    }
#endif
#ifdef IS_LINUX
    // On Linux, errno will be set to ENOTCONN when the client has already
    // closed the socket.
    if( _sok > 0 && errno != ENOTCONN && ret < 0 )
        CK_STHROW( ck_socket_exception, ( _host, _addr.port(), "Unable to shutdown socket: %s", strerror(errno) ));
#endif

}

size_t ck_socket::send( const void* msg, size_t msgLen )
{
    return _send( msg, msgLen, _sendTimeoutMilliseconds );
}

size_t ck_socket::send( const void* msg, size_t msgLen, int sendTimeoutMillis )
{
    return _send( msg, msgLen, sendTimeoutMillis );
}

size_t ck_socket::recv( void* buf, size_t msgLen )
{
    return _recv( buf, msgLen, _recvTimeoutMilliseconds );
}

size_t ck_socket::recv( void* buf, size_t msgLen, int recvTimeoutMillis )
{
    return _recv( buf, msgLen, recvTimeoutMillis );
}

bool ck_socket::buffered_recv()
{
    size_t recvBufferSize = _recvBuffer.size();
    ssize_t bytesJustReceived = raw_recv( &_recvBuffer[0], recvBufferSize );

    if( bytesJustReceived < 0 )
        return false;

    _recvPos = 0;
    _bufferedBytes = (uint32_t)bytesJustReceived;

    if( bytesJustReceived > 0 )
        return true;

    return false;
}

ssize_t ck_socket::raw_send( const void* msg, size_t msgLen )
{
    int ret = 0;

#ifdef IS_WINDOWS
    ret = ::send(_sok, (char*)msg, (int)msgLen, 0);
#endif
#ifdef IS_LINUX
    ret = ::send(_sok, msg, msgLen, MSG_NOSIGNAL);
#endif

    return ret;
}

ssize_t ck_socket::raw_recv( void* buf, size_t msgLen )
{
    int ret = 0;

#ifdef IS_WINDOWS
    ret = ::recv(_sok, (char*)buf, (int)msgLen, 0);
#endif
#ifdef IS_LINUX
    ret = ::recv(_sok, buf, msgLen, 0);
#endif

    return ret;
}

void ck_socket::register_recv_timeout_callback( timeout_callback cb )
{
    _recvTimeoutHandler = cb;
}

void ck_socket::set_recv_timeout( int milliseconds )
{
    _recvTimeoutMilliseconds = milliseconds;
}

void ck_socket::register_send_timeout_callback( timeout_callback cb )
{
    _sendTimeoutHandler = cb;
}

void ck_socket::set_send_timeout( int milliseconds )
{
    _sendTimeoutMilliseconds = milliseconds;
}

bool ck_socket::wait_recv( int& waitMillis )
{
    if( _some_buffered() )
        return false;

    struct timeval beforeSelect = { 0, 0 };
    ck_gettimeofday(&beforeSelect);

    ssize_t retVal = _can_recv_data(waitMillis, _sok);

    if( _sok > 0 && retVal < 0 )
    {
        this->close();
        return true;
    }

    struct timeval afterSelect = { 0, 0 };
    ck_gettimeofday(&afterSelect);

    struct timeval delta = { 0, 0 };
    timersub( &afterSelect, &beforeSelect, &delta );

    int deltaMillis = (delta.tv_sec * 1000) + (delta.tv_usec / 1000);

    if( ((waitMillis - deltaMillis) <= 0) || retVal == 0 )
    {
        waitMillis = 0;
        return true;
    }

    waitMillis -= deltaMillis;

    return false;
}

// Returns true on timeout.
bool ck_socket::wait_send( int& waitMillis )
{
    struct timeval beforeSelect = { 0, 0 };
    ck_gettimeofday(&beforeSelect);

    ssize_t retVal = _can_send_data(waitMillis, _sok);

    if( _sok > 0 && retVal < 0 )
    {
        this->close();
        return true;
    }

    struct timeval afterSelect = { 0, 0 };
    ck_gettimeofday(&afterSelect);

    struct timeval delta = { 0, 0 };
    timersub( &afterSelect, &beforeSelect, &delta );

    int deltaMillis = (delta.tv_sec * 1000) + (delta.tv_usec / 1000);

    if( ((waitMillis - deltaMillis) <= 0) || retVal == 0 )
    {
        waitMillis = 0;
        return true;
    }

    waitMillis -= deltaMillis;

    return false;
}

bool ck_socket::ready_to_recv()
{
    ssize_t retVal = _can_recv_data(0, _sok);

    if( _sok > 0 && retVal < 0 )
        return false;

    if( retVal )
        return true;

    return false;
}

bool ck_socket::ready_to_send()
{
    ssize_t retVal = _can_send_data(0, _sok);

    if( _sok > 0 && retVal < 0 )
        return false;

    if( retVal )
        return true;

    return false;
}

ck_string ck_socket::get_host() const
{
    return _host;
}

ssize_t ck_socket::_can_recv_data( int waitMillis, int fd )
{
#ifdef IS_WINDOWS
    ssize_t retVal = _do_select_recv(waitMillis, fd);
#endif
#ifdef IS_LINUX
    ssize_t retVal = _do_poll_recv(waitMillis, fd);
#endif
    return retVal;
}

ssize_t ck_socket::_can_send_data( int waitMillis, int fd )
{
#ifdef IS_WINDOWS
    ssize_t retVal = _do_select_send(waitMillis, fd);
#endif
#ifdef IS_LINUX
    ssize_t retVal = _do_poll_send(waitMillis, fd);
#endif
    return retVal;
}

#ifdef IS_LINUX
ssize_t ck_socket::_do_poll_recv( int waitMillis, int fd )
{
    struct pollfd fds[POLL_NFDS];
    int nfds = POLL_NFDS;

    fds[0].fd = fd;
    fds[0].events = POLLIN | POLLHUP | POLLERR | POLLNVAL | POLLRDHUP;

    int retVal = poll(fds, nfds, waitMillis );

    // The socket may have been closed, but there might be data in
    // the buffer to read first
    if( fds[0].revents & POLLIN )
        return retVal;

    //Check for error conditions
    //if the remote side shut down their socket, POLLRDHUP will be active.
    if( fds[0].revents & ( POLLERR | POLLRDHUP | POLLHUP | POLLNVAL) )
        retVal = -1;

    return retVal;
}

ssize_t ck_socket::_do_poll_send( int waitMillis, int fd )
{
    struct pollfd fds[POLL_NFDS];
    int nfds = POLL_NFDS;

    fds[0].fd = fd;
    fds[0].events = POLLOUT | POLLHUP;

    int retVal = poll(fds, nfds, waitMillis );

    if( fds[0].revents & POLLHUP )
        retVal = -1;

    return retVal;
}
#endif

ssize_t ck_socket::_do_select_recv( int waitMillis, int fd )
{
    fd_set recvFileDescriptors;

    FD_ZERO( &recvFileDescriptors );

    FD_SET( fd, &recvFileDescriptors );

    struct timeval recvTimeout;

    recvTimeout.tv_sec = (waitMillis / 1000);
    recvTimeout.tv_usec = ((waitMillis % 1000) * 1000);

    int selectRet = select( fd+1, &recvFileDescriptors, nullptr, nullptr, &recvTimeout );

    return selectRet;
}

ssize_t ck_socket::_do_select_send( int waitMillis, int fd )
{
    fd_set sendFileDescriptors;

    FD_ZERO( &sendFileDescriptors );

    FD_SET( fd, &sendFileDescriptors );

    struct timeval sendTimeout;
    sendTimeout.tv_sec = (waitMillis / 1000);
    sendTimeout.tv_usec = ((waitMillis % 1000) * 1000);

    int selectRet = select( fd+1, nullptr, &sendFileDescriptors, nullptr, &sendTimeout );

    return selectRet;
}

size_t ck_socket::_send( const void* msg, size_t msgLen, int sendTimeoutMillis )
{
    if( !msg || (msgLen <= 0) )
        CK_STHROW( ck_socket_exception, ( _host, _addr.port(), "Invalid argument passed to ck_socket::send()."));

    bool timedOut = false;
    int waitMillis = sendTimeoutMillis;

    size_t bytesToWrite = msgLen;
    unsigned char* writer = (unsigned char*)msg;

    while( _sok > 0 && !timedOut && (bytesToWrite > 0) )
    {
        ssize_t bytesJustWritten = raw_send( writer, bytesToWrite );

        // If we get here, and we're unable to send any bytes, something
        // funny is going on (perhaps the other hand has closed their
        // socket), so just bail.

        if( bytesJustWritten < 0 )
        {
#ifdef IS_LINUX
            if( (errno != EAGAIN) && (errno != EWOULDBLOCK) )
#endif
            {
                this->close();

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
            // wait_send() will decrement waitMillis by the amount of time waited each time...
            timedOut = wait_send( waitMillis );

            if( timedOut )
            {
                // callback returns true if we should try again...
                timedOut = (_sendTimeoutHandler) ? !_sendTimeoutHandler() : false;

                if( !timedOut )
                    waitMillis = sendTimeoutMillis;
            }
        }
    }

    return (msgLen - bytesToWrite);
}

size_t ck_socket::_recv( void* buf, size_t msgLen, int recvTimeoutMillis )
{
    if( !buf )
        CK_STHROW( ck_socket_exception, ( _host, _addr.port(), "Invalid buffer passed to ck_socket::recv()."));

    if( msgLen <= 0 )
        CK_STHROW( ck_socket_exception, ( _host, _addr.port(), "Invalid msgLen passed to ck_socket::recv(). [%u]", msgLen ));

    bool timedOut = false;
    int waitMillis = recvTimeoutMillis;

    size_t bytesToRecv = msgLen;
    unsigned char* writer = (unsigned char*)buf;

    while( _sok > 0 && !timedOut && (bytesToRecv > 0) )
    {
        ssize_t bytesJustReceived = 0;

        if( _some_buffered() )
            bytesJustReceived = _read_recv_buffer( writer, bytesToRecv );
        else
        {
            timedOut = wait_recv( waitMillis );

            if( timedOut )
            {
                CK_LOG_INFO("Receive on socket timed out.");

                // callback returns true if we should try again...
                bool tryAgain = (_recvTimeoutHandler) ? _recvTimeoutHandler() : false;

                if( tryAgain )
                {
                    timedOut = false;
                    waitMillis = recvTimeoutMillis; // If we're here, we need to retry... so reset our timeout.
                }

                continue;
            }

            bytesJustReceived = raw_recv( &_recvBuffer[0], _recvBuffer.size() );

            if( bytesJustReceived <= 0 )
            {
#ifdef IS_LINUX
                if( (errno != EAGAIN) && (errno != EWOULDBLOCK) )
#endif
                {
                    this->close();

                    continue;  // In this case, _sok will be set to 0, so continuing here will result in the loop
                               // terminating... as one of the conditions of the loop is _sok > 0.
                }
            }
            else
            {
                // We got some data...

                _recvPos = 0;

                _bufferedBytes = bytesJustReceived;

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

size_t ck_socket::_read_recv_buffer( void* buf, size_t msgLen )
{
    if( _some_buffered() )
    {
        if( _bufferedBytes > (uint32_t)msgLen )
        {
            const unsigned char* pos = &_recvBuffer[_recvPos];

            memcpy( buf, pos, msgLen );

            _recvPos += msgLen;

            _bufferedBytes -= msgLen;

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

bool ck_socket::_some_buffered()
{
    return (_bufferedBytes > 0) ? true : false;
}

bool stub_checker(ck_string ip, int port)
{
    return true;
}

ck_string ck_socket::get_peer_ip() const
{
    struct sockaddr_storage peer;
    int peerLength = sizeof(peer);

#ifdef IS_WINDOWS
    if ( getpeername(_sok,(sockaddr*)&peer,&peerLength) < 0 )
    {
        CK_LOG_WARNING("Unable to get peer ip. %s", ck_get_last_error_msg().c_str());
        return "";
    }
#endif
#ifdef IS_LINUX
    if ( getpeername(_sok,(sockaddr*)&peer,(socklen_t*)&peerLength) < 0 )
    {
        CK_LOG_WARNING("Unable to get peer ip: %s", ck_get_last_error_msg().c_str());
        return "";
    }
#endif

    return ck_socket_address::address_to_string((sockaddr*)&peer, (socklen_t)peerLength);
}

ck_string ck_socket::get_local_ip() const
{
    struct sockaddr_storage local;
    int addrLength = sizeof(local);

#ifdef IS_WINDOWS
    if ( getsockname(_sok, (sockaddr*)&local, &addrLength) < 0 )
    {
        CK_LOG_WARNING("Unable to get local ip. %s", ck_get_last_error_msg().c_str());
        return "";
    }
#endif
#ifdef IS_LINUX
    if ( getsockname(_sok, (sockaddr*)&local, (socklen_t*)&addrLength) < 0 )
    {
        CK_LOG_WARNING("Unable to get local ip: %s", ck_get_last_error_msg().c_str());
        return "";
    }
#endif

    return ck_socket_address::address_to_string((sockaddr*)&local, (socklen_t)addrLength);
}

ck_socket_exception::ck_socket_exception()
    : ck_exception(),
      ip(""),
      port(-1)
{
}

ck_socket_exception::~ck_socket_exception() throw()
{
}

ck_socket_exception::ck_socket_exception(const char* msg, ...)
    : ck_exception(),
      ip(),
      port(-1)
{
    va_list args;
    va_start(args, msg);
    set_msg(ck_string::format(msg, args));
    va_end(args);
}

ck_socket_exception::ck_socket_exception(const ck_string& ip, int port, const char* msg, ...)
    : ck_exception(),
      ip(ip),
      port(port)
{
    va_list args;
    va_start(args, msg);
    set_msg(ck_string::format("ip [%s], port [%d]: ", ip.c_str(), port) + ck_string::format(msg, args));
    va_end(args);
}

ck_socket_exception::ck_socket_exception(const ck_string& msg)
    : ck_exception(msg),
      ip(),
      port(-1)
{
}

ck_socket_exception::ck_socket_exception(const ck_string& ip, int port, const ck_string& msg)
    : ck_exception(ck_string::format("ip [%s], port [%d]: ", ip.c_str(), port) + msg),
      ip(ip),
      port(port)
{
}
