
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef XSDK_XSSLSocket_h
#define XSDK_XSSLSocket_h

#include "XSDK/XBaseObject.h"
#include "XSDK/XSocket.h"
#include "XSDK/XRef.h"
#include "XSDK/XStreamIO.h"
#include "XSDK/XMutex.h"
#include "openssl/ssl.h"

extern "C"
{
    struct CRYPTO_dynlock_value
    {
        XSDK::XMutex _mutex;
    };
}


class XSSLSocketTest;

namespace XSDK
{

/// XSSLSocket is a general purpose SSL socket. It is based upon OpenSSL and can be used portably to create both
/// SSL TCP clients and servers. It works very similarly to XSocket.
class XSSLSocket : public XSocket
{
    friend class ::XSSLSocketTest;

public:
    enum XSSLSocketEnum
    {
        DEFAULT_RECV_BUFFER_SIZE = 1024,
    };

    /// Create and destroy SSL sockets...
    X_API XSSLSocket( uint32_t defaultRecvBufferSize = DEFAULT_RECV_BUFFER_SIZE );
    X_API virtual ~XSSLSocket() throw();

    X_API virtual bool Valid();

    /// Release this sockets file descriptor...
    X_API virtual void Close() throw();

    /// Return the sockets ID.
    X_API virtual int GetSokId() const;

    /// Causes socket option SO_LINGER to be set.
    X_API virtual void Linger( uint16_t lingerSeconds = 30 );

    /// Bind this socket to a particular port.
    /// \param The port to bind to.
    X_API virtual void Bind( int port, const XSDK::XString& ip = "" );

    /// Listen - this is just a NO-OP for SSL
    X_API virtual void Listen();

    /// Accept a new connection and returns a socket for that connection.
    X_API virtual XRef<XSocket> Accept( uint32_t defaultRecvBufferSize = DEFAULT_RECV_BUFFER_SIZE );

    X_API virtual XRef<XSocket> AcceptSSL( uint32_t defaultRecvBufferSize = DEFAULT_RECV_BUFFER_SIZE );

    /// Connect to a host. Throws if unsuccessful.
    /// \param host The IP address of the server you'd like to connect to.
    /// \param port The tcp port you'd like to ring on.
    /// \param pemPath A string containg a path to a PEM file (local certificate store).
    /// Throws an XSocketException if unable to connect or ValidateConnection(true) was set and
    /// connection did not verify.
    X_API virtual void Connect( const XSDK::XString& host, int port, XDuration connectTimeout = XDuration(SECONDS, 30) );
    X_API virtual void Connect( const XSDK::XString& host, int port, int connectTimeoutMillis )
    { XSocket::Connect(host, port, connectTimeoutMillis); }

    /// Attempts to connect to a host.  If ValidateConnection( true ) has been set.  This will return false
    X_API virtual bool QueryConnect( const XSDK::XString& host, int port, XDuration connectTimeout = XDuration(SECONDS, 30) );
    X_API virtual bool QueryConnect( const XSDK::XString& host, int port, int connectTimeoutMillis )
    { return XSocket::QueryConnect(host, port, connectTimeoutMillis); }

    /// Determine wheather or not to verify an SSL connection
    /// Default behavior is not to verify
    X_API virtual void VerifyConnection( bool verify );

    /// Use this path to a CACertificate file to verify the SSL socket
    /// \param path The path to a PEM encoded certificate file.
    X_API virtual void UseCACertificateFile( const XSDK::XString& path );

    /// Associate this SSL socket with a particular certificate.
    /// \param path The path to a PEM encoded certificate file.
    X_API virtual void UsePEMCertificateFile( const XSDK::XString& path );

    /// Associate this SSL socket with a particular certificate chain file.
    /// Calling this method will override any calls to UsePEMCertificateFile.
    /// The certificates must be in PEM format and must be sorted starting with the subject's
    /// certificate (actual client or server certificate), followed by intermediate CA
    /// certificates if applicable, and ending at the highest level (root) CA.
    /// \param path The path to a PEM encoded certificate file.
    X_API virtual void UsePEMCertificateChainFile( const XSDK::XString& path );

    /// Associate this SSL socket with a particular private key.
    /// \param path The path to a PEM encoded certificate file.
    /// \param privateKeyPassword If the private key is encrypted, decrypt it with this password.
    X_API virtual void UsePEMRSAPrivateKeyFile( const XSDK::XString& path,
                                                const XSDK::XString& privateKeyPassword = "" );

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

    /// Recieve data from server. May generate recv timeout callbacks.
    /// \param buf A pointer to a memory buffer that is filled in by the incoming data.
    /// \param msgLen The length of the data to recieve.
    X_API virtual size_t Recv( void* buf, size_t msgLen );

    /// Recieve data from server. May generate recv timeout callbacks.
    /// \param buf A pointer to a memory buffer that is filled in by the incoming data.
    /// \param msgLen The length of the data to recieve.
    /// \param recvTimeout Override recv timeout for this call only.
    X_API virtual size_t Recv( void* buf, size_t msgLen, XDuration recvTimeout );
    X_API virtual size_t Recv( void* buf, size_t msgLen, int recvTimeoutMillis )
    { return XStreamIO::Recv(buf, msgLen, recvTimeoutMillis); }

    /// Attempts to pre buffer some data to be received.
    /// Returns true if some data was buffered.
    X_API bool BufferedRecv();

    /// Send data to server. May write less bytes than asked for.
    /// \param msg A pointer to a memory buffer that is the SOURCE of data to send.
    /// \param msgLen The length of the data to send.
    X_API virtual ssize_t RawSend( const void* msg, size_t msgLen );

    /// Recieve data from server. May read less bytes than asked for.
    /// \param buf A pointer to a memory buffer that is filled in by the incoming data.
    /// \param msgLen The length of the data to recieve.
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
    /// \param timeout The length of time to attempt a send before failing.
    X_API virtual void SetSendTimeout( XDuration timeout );
    X_API virtual void SetSendTimeout( int timeoutMillis ) { XStreamIO::SetSendTimeout(timeoutMillis); }

    /// Wait for data to be available to be recieved on the socket.
    /// \param waitTime The maximum amount of time to wait for data.
    X_API virtual bool WaitRecv( XDuration& waitTime );
    X_API virtual bool WaitRecv( int& waitTimeMillis ) { return XStreamIO::WaitRecv(waitTimeMillis); }

    /// Wait for socket clear to send.
    /// \param waitTime The maximum amount of time to wait for data.
    X_API virtual bool WaitSend( XDuration& waitTime );
    X_API virtual bool WaitSend( int& waitTimeMillis ) { return XStreamIO::WaitSend(waitTimeMillis); }

    /// Returns true if a RawRecv() would not block.
    X_API virtual bool ReadyToRecv();

    /// Returns true if a RawSend() would not block.
    X_API virtual bool ReadyToSend();

    /// Get Host used for Connect
    X_API virtual XSDK::XString GetHost() const;
    X_API virtual uint32_t GetHostPort() const {return _hostPort;}

    // Set Host used for verification purposes.  Allows caller to use IP in Connect(), but
    // still verify hostname
    X_API void SetHost( XSDK::XString host );

    X_API virtual XSDK::XString GetPeerIp() const;

private:
    size_t _Send( const void* msg, size_t msgLen, XDuration sendTimeout );
    size_t _Recv( void* buf, size_t msgLen, XDuration recvTimeout );

    bool _WaitToRecv( XDuration& waitTime );
    bool _WaitToSend( XDuration& waitTime );

    int _HandleError( int error, XDuration& waitTime );

    X_API ssize_t _CanRecvData( XDuration waitTime, int fd );
    X_API ssize_t _CanSendData( XDuration waitTime, int fd );

    X_API ssize_t _DoSelectRecv( XDuration waitTime, int fd );
    X_API ssize_t _DoSelectSend( XDuration waitTime, int fd );

#ifndef WIN32
    X_API ssize_t _DoPollRecv( XDuration waitTime, int fd );
    X_API ssize_t _DoPollSend( XDuration waitTime, int fd );
#endif


    size_t _ReadRecvBuffer( void* buf, size_t msgLen );
    bool _SomeBuffered();

    static int _PrivateKeyPasswordCB( char *buf, int size, int rwflag, void *userdata );

    static int _X509CheckHost(X509 *xcert, const XString& hostname);
    static int _EqualNocase(const unsigned char *pattern, size_t pattern_len, const unsigned char *subject, size_t subject_len);
    static int _WildcardMatch(const unsigned char *prefix, size_t prefix_len, const unsigned char *suffix, size_t suffix_len,
            const unsigned char *subject, size_t subject_len);
    static int _ValidDomainCharacters(const unsigned char *p, size_t len);
    static const unsigned char *_WildcardFindStar(const unsigned char *pattern, size_t pattern_len);
    static int _EqualWildcard(const unsigned char *pattern, size_t pattern_len, const unsigned char *subject, size_t subject_len);
    static int _CompareStrings(ASN1_STRING *a, int cmp_type, XString hostname );


    /// OpenSSL specific members...
    SSL_CTX* _ctx;
    SSL* _ssl;
    XRef<XSocket> _sok;
    XString _host;
    XString _privateKeyPassword;
    XString _bindAddress;
    XString _bindPort;
    XString _caCertificateFilePath;
    XString _pemCertificateChainFilePath;
    XString _pemCertificateFilePath;
    XString _pemRSAPrivateKeyFilePath;

    /// Our timeouts and timeout handlers...
    XDuration _recvTimeout;
    TimeoutHandler _recvTimeoutHandler;
    void* _recvTimeoutHandlerOpaque;
    XDuration _sendTimeout;
    TimeoutHandler _sendTimeoutHandler;
    void* _sendTimeoutHandlerOpaque;

    /// Received data is read in chunks and cached here...
    std::vector<uint8_t> _recvBuffer;
    uint32_t _bufferedBytes;
    uint32_t _recvPos;
    uint32_t _hostPort;
    bool _verifyConnection;
    XMutex _sslLok;
    bool _listener;



public:
    /// We have some global SSL state initializatoin that is done with the
    /// static initialization trick.
    class StaticInit
    {
    public:
        StaticInit();
        ~StaticInit();


        static void lock(int mode, int n, const char* file, int line);
        static unsigned long id();
        static struct CRYPTO_dynlock_value* dynlockCreate(const char* file, int line);
        static void dynlock(int mode, struct CRYPTO_dynlock_value* lock, const char* file, int line);
        static void dynlockDestroy(struct CRYPTO_dynlock_value* lock, const char* file, int line);


    private:

        static XSDK::XMutex* _mutexes;
        static XSDK::XMutex _initLok;
        static bool _init;
    };

    friend class StaticInit;

    const static StaticInit _sslStaticInit;

};

}

#endif
