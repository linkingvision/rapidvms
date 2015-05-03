
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

#ifndef cppkit_socket_h
#define cppkit_socket_h

#include "cppkit/os/ck_platform.h"
#include "cppkit/ck_stream_io.h"
#include "cppkit/ck_socket_address.h"

#include <unordered_map>
#include <vector>
#include <mutex>
#include <memory>
#include <functional>

#ifdef IS_WINDOWS
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

#define x_ntohs(x) ( (((x) >> 8) & 0xff) | (((x) << 8) & 0xff00) )
#define x_htons(x) x_ntohs(x)

#define x_ntohl(x) ( (((x) >> 24) & 0xff) | (((x) >> 8) & 0xff00) | (((x) << 8) & 0xff0000) | (((x) << 24) & 0xff000000) )
#define x_htonl(x) x_ntohl(x)

#define x_ntohll(x) ( ( (uint64_t)(ntohl( (uint32_t)((x << 32) >> 32) )) << 32) | ntohl( ((uint32_t)(x >> 32)) ) )
#define x_htonll(x) x_ntohll(x)

namespace cppkit
{

/// This typedef describes the interface to a function that will be called when
/// their is either a send or recv timeout. A return value of true means "try again".
//typedef std::function<bool()> timeout_callback;

/// This object represents a simple BSD style tcp socket connection. If you are
/// fammiliar with standard BSD or Winsock programming, you can probably discern
/// how this object works just from the function names.
///
/// The functions in ck_socket can be roughly categorized into 3 buckets:
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

class ck_socket : public ck_stream_io
{
public:
    enum ck_socket_enum
    {
        DEFAULT_RECV_BUFFER_SIZE = 1024
    };

    typedef enum _ck_socket_type
    {
        IPV4,           ///< Create an IPv4 socket only
        IPV6,           ///< Create an IPv6 socket only
        IPV6_WITH_IPV4  ///< Create an IPv6 socket that can receive traffic via IPv4
    } ck_socket_type;

    /// Created an unconnected socket object.
    CK_API ck_socket( uint32_t defaultRecvBufferSize = DEFAULT_RECV_BUFFER_SIZE );

    /// Create an unconnected socket object.
    /// @param type The type of socket to create.
    CK_API ck_socket( ck_socket_type type );

    /// Close and destroy this object.
    CK_API virtual ~ck_socket() throw();

    /// A socket is valid if it is connected and no critical system calls have returned any errors.
    CK_API virtual bool valid();

    /// Release this sockets file descriptor...
    CK_API virtual void close();

    /// If enabled, causes default close() to wait_recv() UP TO timeout millis for the client to
    /// disconnect first. Client first disconnection avoids TIME_WAIT issues on the server and guarantees
    /// the data has been read by the client.
    CK_API void enable_auto_server_close( int timeout );

    /// Return the sockets ID.
    CK_API virtual int get_sok_id() const;

    /// Causes socket object to give up ownership of underlying OS socket resources.
    CK_API SOCKET release_sok_id();

    /// Causes socket object to take over the specified underlying OS socket resource.
    CK_API void take_over_sok_id( SOCKET sok );

    /// Causes socket option SO_LINGER to be set.
    CK_API void linger( uint16_t lingerSeconds = 30 );

    /// Bind this socket to a particular port.
    /// \param port The port to bind to.
    /// \param ip The interface address to bind to
    CK_API virtual void bind( int port, const ck_string& ip = "" );

    /// Bind to any available high-number port.
    /// \param ip The interface address to bind to
    /// \return The port bound to.
    CK_API int bind_ephemeral( const ck_string& ip = "" );

    /// Begin listening on the bound port.
    CK_API virtual void listen();

    /// Accept a new connection and returns a socket for that connection.
    CK_API virtual std::shared_ptr<ck_socket> accept( uint32_t recvBufferSize = DEFAULT_RECV_BUFFER_SIZE );

    /// Connect to a host. Throws if unsuccessful.
    /// \param host The IP address of the server you'd like to connect to.
    /// \param port The tcp port you'd like to ring on.
    /// \param connectTimeoutMillis The number of milliseconds to wait beyond this it should throw.
    CK_API virtual void connect(const ck_string& host, int port, int connectTimeoutMillis = 30000);

    /// Attempts to connect to a host.
    /// \param host The IP address of the server you'd like to connect to.
    /// \param port The tcp port you'd like to ring on.
    /// \return 'true' if the connect succeeded, 'false' otherwise.
    CK_API virtual bool query_connect(const ck_string& host, int port, int connectTimeoutMillis = 30000 );

    /// Close this open socket.
    CK_API virtual void shutdown( int mode );

    /// Send data to server. May generate send timeout callbacks.
    /// \param msg A pointer to a memory buffer that is the SOURCE of data to send.
    /// \param msgLen The length of the data to send.
    CK_API virtual size_t send( const void* msg, size_t msgLen );

    /// Send data to server. May generate send timeout callbacks.
    /// \param msg A pointer to a memory buffer that is the SOURCE of data to send.
    /// \param msgLen The length of the data to send.
    /// \param sendTimeoutMillis Override send timeout for this call only.
    CK_API virtual size_t send( const void* msg, size_t msgLen, int sendTimeoutMillis );

    /// Receive data from server. May generate recv timeout callbacks.
    /// \param buf A pointer to a memory buffer that is filled in by the incoming data.
    /// \param msgLen The length of the data to receive.
    CK_API virtual size_t recv( void* buf, size_t msgLen );

    /// Receive data from server. May generate recv timeout callbacks.
    /// \param buf A pointer to a memory buffer that is filled in by the incoming data.
    /// \param msgLen The length of the data to receive.
    /// \param recvTimeoutMillis Override recv timeout for this call only.
    CK_API virtual size_t recv( void* buf, size_t msgLen, int recvTimeoutMillis );

    /// Attempts to pre buffer some data to be received.
    /// Returns true if some data was buffered.
    CK_API virtual bool buffered_recv();

    /// Send data to server. May write less bytes than asked for.
    /// \param msg A pointer to a memory buffer that is the SOURCE of data to send.
    /// \param msgLen The length of the data to send.
    CK_API virtual ssize_t raw_send( const void* msg, size_t msgLen );

    /// Receive data from server. May read less bytes than asked for.
    /// \param buf A pointer to a memory buffer that is filled in by the incoming data.
    /// \param msgLen The length of the data to receive.
    CK_API virtual ssize_t raw_recv( void* buf, size_t msgLen );

    /// Attach a handler function pointer to send notifications about recv timeouts.
    /// \param rtcb A pointer to a function to call when recv times out.
    /// \param opaque User data to be passed to rtcb.
    CK_API virtual void register_recv_timeout_callback( timeout_callback cb );

    /// Explicitly set a recv timeout value.
    /// \param milliseconds The milliseconds to attempt a recv before failing.
    CK_API virtual void set_recv_timeout( int milliseconds );

    /// Attach a handler function pointer to send notifications about send timeouts.
    /// \param stcb A pointer to a function to call when send times out.
    /// \param opaque User data to be passed to stcb.
    CK_API virtual void register_send_timeout_callback( timeout_callback cb );

    /// Explicitly set a send timeout value.
    /// \param milliseconds The milliseconds to attempt a send before failing.
    CK_API virtual void set_send_timeout( int milliseconds );

    /// Wait for data to be available to be received on the socket.
    CK_API virtual bool wait_recv( int& waitMillis );

    /// Wait for socket clear to send.
    CK_API virtual bool wait_send( int& waitMillis );

    /// Returns true if a raw_recv() would not block.
    CK_API virtual bool ready_to_recv();

    /// Returns true if a raw_send() would not block.
    CK_API virtual bool ready_to_send();

    /// Get Host used for connect
    CK_API ck_string get_host() const;
    CK_API uint32_t get_host_port() const;

    CK_API ck_string get_peer_ip() const;

    CK_API ck_string get_local_ip() const;

    /// Do Socket Startup for Application
    CK_API static void socket_startup();

    /// Do Socket Cleanup for Application
    CK_API static void socket_cleanup();

    CK_API static ck_string get_hostname();
    CK_API static std::vector<ck_string> get_addresses_by_hostname( const ck_string& hostname );

    CK_API static std::vector<ck_string> resolve( int type, const ck_string& name );

    CK_API static std::unordered_map<std::string,std::vector<ck_string> > get_interface_addresses( int addressFamily = AF_INET );

private:

    /// Basic socket initialization.
    void create(unsigned int addrFamily);

    ck_socket( const ck_socket& obj );
    ck_socket& operator = ( const ck_socket& obj );

    CK_API size_t _send( const void* msg, size_t msgLen, int sendTimeoutMillis );
    CK_API size_t _recv( void* buf, size_t msgLen, int recvTimeoutMillis );

    CK_API size_t _read_recv_buffer( void* buf, size_t msgLen );
    CK_API bool _some_buffered();

    CK_API ssize_t _can_recv_data( int waitMillis, int fd );
    CK_API ssize_t _can_send_data( int waitMillis, int fd );

    CK_API ssize_t _do_select_recv( int waitMillis, int fd );
    CK_API ssize_t _do_select_send( int waitMillis, int fd );

#ifdef IS_LINUX
    CK_API ssize_t _do_poll_recv( int waitMillis, int fd );
    CK_API ssize_t _do_poll_send( int waitMillis, int fd );
#endif

    static int _sokCount;
    static std::recursive_mutex _sokLock;
    SOCKET _sok;
    ck_socket_address _addr;
    int _recvTimeoutMilliseconds;
    timeout_callback _recvTimeoutHandler;
    int _sendTimeoutMilliseconds;
    timeout_callback _sendTimeoutHandler;
    ck_string _host;
    uint32_t _hostPort;
    std::vector<uint8_t> _recvBuffer;
    uint32_t _bufferedBytes;
    uint32_t _recvPos;
    int _delayClose;
};

class ck_socket_exception : public ck_exception
{
public:
    CK_API ck_socket_exception();

    CK_API virtual ~ck_socket_exception() throw();

    CK_API ck_socket_exception(const char* msg, ...);

    CK_API ck_socket_exception(const ck_string& ip, int port, const char* msg, ...);

    CK_API ck_socket_exception(const ck_string& msg);

    CK_API ck_socket_exception(const ck_string& ip, int port, const ck_string& msg);

    ck_string ip;
    int port;
};

}

#endif
