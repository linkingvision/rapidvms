
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "XSDK/XSSLSocket.h"
#include "XSDK/TimeUtils.h"
#include "XSDK/OS.h"
#include "XSDK/XGuard.h"
#include "XSDK/XMonoClock.h"
#include "XSDK/XTaskBase.h"

#include "openssl/ssl.h"
#include "openssl/err.h"
#include "openssl/crypto.h"
#include "openssl/x509v3.h"

#include <vector>


#ifndef WIN32
#include <signal.h>
#include <poll.h>
#include <fcntl.h>
#include <errno.h>
#endif

using namespace XSDK;


static const int XSSL_ERROR_CRITICAL = -2;
static const int XSSL_ERROR_TIMEOUT = -1;
static const int XSSL_ERROR_NONE = 0;
static const int XSSL_ERROR_RETRY = 1;


XMutex* XSSLSocket::StaticInit::_mutexes;
XMutex XSSLSocket::StaticInit::_initLok;
bool XSSLSocket::StaticInit::_init(false);


void XSSLSocket::StaticInit::lock(int mode, int n, const char* file, int line)
{
    if( mode & CRYPTO_LOCK )
        _mutexes[n].Acquire();
    else
        _mutexes[n].Release();

}

unsigned long XSSLSocket::StaticInit::id()
{
    return (unsigned long) XTaskBase::CurrThreadID();
}

struct CRYPTO_dynlock_value* XSSLSocket::StaticInit::dynlockCreate(const char* file, int line)
{
    X_LOG_WARNING("!!!!!!!!!!!!!!!!!! OPEN SSL dynlock create !!!!!!!!");
    return new CRYPTO_dynlock_value;
}

void XSSLSocket::StaticInit::dynlock(int mode, struct CRYPTO_dynlock_value* lock, const char* file, int line)
{
    X_LOG_WARNING("!!!!!!!!!!!!!!!!!! OPEN SSL dynlock !!!!!!!!");
    if( lock != NULL )
    {
        if( mode & CRYPTO_LOCK )
            lock->_mutex.Acquire();
        else
            lock->_mutex.Release();
    }
}

void XSSLSocket::StaticInit::dynlockDestroy(struct CRYPTO_dynlock_value* lock, const char* file, int line)
{
    X_LOG_WARNING("!!!!!!!!!!!!!!!!!! OPEN SSL dynlock  destroy !!!!!!!!");
    delete lock;
}

XSSLSocket::StaticInit::StaticInit()
{
    XGuard guard(_initLok);
    if( ! _init )
    {
        _init = true;

        SSL_library_init();
        SSL_load_error_strings();
        ERR_load_BIO_strings();

        int mutexCount = CRYPTO_num_locks();
        _mutexes = new XMutex[mutexCount];

        CRYPTO_set_locking_callback(&XSSLSocket::StaticInit::lock);

        //TESTING BELOW BUT IS DEPRECATED IN FAVOR OF
        //CRYPTO_THREADID_set_callback(&XSSLSocket::StaticInit::THREADID);
        //I don't think we need this as OpenSSL default implementation will be used if we
        //don't set this.
        CRYPTO_set_id_callback(&XSSLSocket::StaticInit::id);

        CRYPTO_set_dynlock_create_callback(&XSSLSocket::StaticInit::dynlockCreate);
        CRYPTO_set_dynlock_lock_callback(&XSSLSocket::StaticInit::dynlock);
        CRYPTO_set_dynlock_destroy_callback(&XSSLSocket::StaticInit::dynlockDestroy);
    }
}

XSSLSocket::StaticInit::~StaticInit()
{
    XGuard guard(_initLok);
    if( _init )
    {
        _init = false;
        ERR_free_strings();
        CRYPTO_set_locking_callback(NULL);
        delete [] _mutexes;
    }
}

const XSSLSocket::StaticInit _sslStaticInit;




// TODO:
// 1. Refactor XSocket & XSSLSocket to remove duplicate code
// 2. Add IPv6 indicator if a hostname is passed to Connect

static const int POLL_NFDS = 1;

XSSLSocket::XSSLSocket( uint32_t defaultRecvBufferSize ) :
    XSocket(),
    _ctx( NULL ),
    _ssl( NULL ),
    _sok( new XSocket ),
    _host(),
    _privateKeyPassword( "" ),
    _bindAddress( "" ),
    _bindPort( "" ),
    _caCertificateFilePath( "" ),
    _pemCertificateChainFilePath( "" ),
    _pemCertificateFilePath( "" ),
    _pemRSAPrivateKeyFilePath( "" ),
    _recvTimeout( XDuration(SECONDS, 5) ),
    _recvTimeoutHandler( NULL ),
    _recvTimeoutHandlerOpaque( NULL ),
    _sendTimeout( XDuration(SECONDS, 5) ),
    _sendTimeoutHandler( NULL ),
    _sendTimeoutHandlerOpaque( NULL ),
    _recvBuffer( defaultRecvBufferSize ),
    _bufferedBytes( 0 ),
    _recvPos( 0 ),
    _verifyConnection( false ),
    _sslLok()
{
    _ctx = SSL_CTX_new( SSLv23_method() );
    SSL_CTX_set_options( _ctx, SSL_OP_NO_SSLv2 );
    SSL_CTX_set_options( _ctx, SSL_OP_NO_SSLv3 );
    SSL_CTX_set_default_passwd_cb( _ctx, _PrivateKeyPasswordCB );
    SSL_CTX_set_default_passwd_cb_userdata( _ctx, this );
    SSL_CTX_set_cipher_list(_ctx, "HIGH:MEDIUM");
    SSL_CTX_set_session_cache_mode(_ctx, SSL_SESS_CACHE_OFF);
    SSL_CTX_set_mode(_ctx, SSL_MODE_AUTO_RETRY);
    SSL_CTX_set_default_verify_paths( _ctx );

}

XSSLSocket::~XSSLSocket() throw()
{
    XGuard guard(_sslLok);

    Close();
    if( _ctx )
        SSL_CTX_free( _ctx );
}

bool XSSLSocket::Valid()
{
    return _sok->Valid();
}

void XSSLSocket::Close() throw()
{
    XGuard guard(_sslLok);

    if( _ssl )
    {
        SSL_shutdown(_ssl);
        // A bug in SSL_free() doesn't release the string allocated by BIO_set_conn_hostname(), the work
        // around is to set it to NULL right before we free the SSL*.
        SSL_set_tlsext_host_name( _ssl, NULL );

        SSL_free(_ssl);
        _ssl = NULL;
    }
    if( _sok->Valid() )
    {
        //Shutdown to wake up socket blocked in accept..??
        _sok->Shutdown(SOCKET_SHUT_FLAGS);
        _sok->Close();
    }
}

int XSSLSocket::GetSokId() const
{
    return _sok->GetSokId();
}

void XSSLSocket::Linger( uint16_t lingerSeconds )
{
    _sok->Linger( lingerSeconds );
}


void XSSLSocket::Bind( int port, const XString& ip )
{
    XGuard guard(_sslLok);

    _ssl = SSL_new(_ctx);

    // First, cache the information passed.
    _bindPort = XString::FromInt( port );
    _bindAddress = ip;

    _sok->Bind(port, ip);

    SSL_set_fd(_ssl, _sok->GetSokId());
}

void XSSLSocket::Listen()
{
    _sok->Listen();
}

XRef<XSocket> XSSLSocket::Accept( uint32_t defaultRecvBufferSize )
{
    XGuard guard(_sslLok);

    XRef<XSocket> sok = _sok->Accept( defaultRecvBufferSize );

    XRef<XSSLSocket> clientSocket = new XSSLSocket( defaultRecvBufferSize );

    clientSocket->_sok = sok;
    clientSocket->_ssl = SSL_new(_ctx);

#ifdef WIN32
    ULONG mode = 1;
    ioctlsocket( sok->GetSokId(), FIONBIO, &mode);
#else
    int flags = fcntl( sok->GetSokId(), F_GETFL, 0);
    fcntl( sok->GetSokId(), F_SETFL, flags | O_NONBLOCK );
#endif

    SSL_set_fd( clientSocket->_ssl, sok->GetSokId() );
    SSL_set_accept_state( clientSocket->_ssl );

    return clientSocket;
}

//SSL_accept also does a handshake, so this is an "active" accept
XRef<XSocket> XSSLSocket::AcceptSSL( uint32_t defaultRecvBufferSize )
{
    XGuard guard(_sslLok);

    XRef<XSSLSocket> sok = Accept( defaultRecvBufferSize );
    SSL_accept( sok->_ssl);
    return sok;
}

void XSSLSocket::Connect( const XSDK::XString& host, int port, XDuration connectTimeout )
{
    if (!QueryConnect(host, port, connectTimeout))
        X_STHROW( XSocketException, ( host, port, "Unable to connect to host." ));

}

bool XSSLSocket::QueryConnect( const XSDK::XString& host, int port, XDuration connectTimeout )
{
    XGuard guard(_sslLok);

    if( _sok->Valid() )
        _sok->Close();

    if( _ssl )
        Close();

    _ssl = SSL_new(_ctx);

    // Only actual hostsnames (not IP literals) are valid with SNI, so we
    // can't set the TLS hostname to an IP literal.
    XString ip = host;
    if( XSocketAddress::IsHostname(host) )
    {
        std::vector<XString> addresses = XSocket::GetAddressesByHostname( host );
        if( addresses.size() == 0 )
            X_STHROW( XSocketException, ( host, port, "Unable to resolve hostname." ));

        ip = addresses[0];
        // If someone called SetHost(), use that value instead
        if( ! _host.empty() )
            SSL_set_tlsext_host_name( _ssl, _host.c_str() );
        else
            SSL_set_tlsext_host_name( _ssl, host.c_str() );
    }


    if( ! _sok->QueryConnect( ip, port, connectTimeout ) )
        return false;

    bool connected = true;

    //Only set _host if it has not been set by SetHost()
    if( _host.empty() )
        _host = host;

    _hostPort = port;

#ifdef WIN32
    ULONG mode = 1;
    ioctlsocket( _sok->GetSokId(), FIONBIO, &mode);
#else
    int flags = fcntl( _sok->GetSokId(), F_GETFL, 0);
    fcntl( _sok->GetSokId(), F_SETFL, flags | O_NONBLOCK );
#endif

    SSL_set_fd(_ssl, _sok->GetSokId());

#ifndef WIN32
    signal( SIGPIPE, SIG_IGN );
#endif

    bool verified = false;

    //If someone has asked to verify, then we must SSL_connect in order
    //to retreive the X509 cert.  Otherwise we do a passive connect
    if( _verifyConnection )
    {
        //Do handshake here to get X509
        int error = -1;
        ssize_t result = 0;
        XDuration waitTime = connectTimeout;
        do
        {
            result = SSL_connect( _ssl );
            error = _HandleError( result, waitTime );

        }
        while( waitTime > XDuration::Zero() && error > XSSL_ERROR_NONE );

        if( result == 1 && error == XSSL_ERROR_NONE )
        {
            X509 *cert = SSL_get_peer_certificate( _ssl );
            if( cert != NULL )
            {
                if( SSL_get_verify_result( _ssl ) == X509_V_OK )
                {
                    int check = _X509CheckHost(cert, _host);
                    //If check is 1, the certificate matched the hostname
                    verified = (check == 1);

                    if( ! verified )
                        X_LOG_ERROR("WARNING: Possible man-in-the-middle attack detected.  The hostname specified %s does not match the hostname in the server's X509 certificate.", _host.c_str() );
                }
                else
                {
                    X_LOG_ERROR("Unable to verify the ssl certificate presented by: %s", _host.c_str() );
                }

                X509_free( cert );
            }
            else
            {
                X_LOG_ERROR("Verification was requested but the server didn't return an X509 Certificate.");
            }

            return verified;
        }

        return false;
    }
    //passive connect
    SSL_set_connect_state( _ssl );
    return connected;
}

void XSSLSocket::VerifyConnection( bool verify )
{
    _verifyConnection = verify;
}

void XSSLSocket::UseCACertificateFile( const XSDK::XString& path )
{
    _caCertificateFilePath = path;
    if( _caCertificateFilePath.length() > 0 )
        SSL_CTX_load_verify_locations( _ctx, _caCertificateFilePath.c_str(), NULL );
}

void XSSLSocket::UsePEMCertificateFile( const XSDK::XString& path )
{
    _pemCertificateFilePath = path;
    if( SSL_CTX_use_certificate_file( _ctx, _pemCertificateFilePath.c_str(), SSL_FILETYPE_PEM ) != 1 )
        X_THROW(( "Invalid certificate file. PEM format?" ));
}

void XSSLSocket::UsePEMCertificateChainFile( const XSDK::XString& path )
{
    _pemCertificateChainFilePath = path;
    if( ! _pemCertificateChainFilePath.empty() )
    {
        if( SSL_CTX_use_certificate_chain_file( _ctx, _pemCertificateChainFilePath.c_str() ) != 1 )
            X_THROW(( "Invalid certificate chain file. PEM format?" ));
    }
}

void XSSLSocket::UsePEMRSAPrivateKeyFile( const XSDK::XString& path, const XSDK::XString& privateKeyPassword )
{
    _pemRSAPrivateKeyFilePath = path;
    _privateKeyPassword = privateKeyPassword;

    if( SSL_CTX_use_RSAPrivateKey_file( _ctx, _pemRSAPrivateKeyFilePath.c_str(), SSL_FILETYPE_PEM ) != 1 )
        X_THROW(( "Invalid private key file. RSA? PEM?" ));

}

void XSSLSocket::Shutdown( int mode )
{
    if( !_ssl )
        X_THROW(( "No active connection to shutdown." ));

    SSL_shutdown( _ssl );

    if( _sok->Valid() )
        _sok->Shutdown(mode);
}

size_t XSSLSocket::Send( const void* msg, size_t msgLen )
{
    return _Send( msg, msgLen, _sendTimeout );
}

size_t XSSLSocket::Send( const void* msg, size_t msgLen, XDuration sendTimeout )
{
    return _Send( msg, msgLen, sendTimeout );
}

size_t XSSLSocket::Recv( void* buf, size_t msgLen )
{
    return _Recv( buf, msgLen, _recvTimeout);
}

size_t XSSLSocket::Recv( void* buf, size_t msgLen, XDuration recvTimeout )
{
    return _Recv( buf, msgLen, recvTimeout );
}

bool XSSLSocket::BufferedRecv()
{
    if( _SomeBuffered() )
        return true;


    int error = -1;
    ssize_t bytesJustReceived = 0;
    size_t recvBufferSize = _recvBuffer.size();
    XDuration waitTime = _recvTimeout;

    XGuard guard(_sslLok);
    do
    {
        bytesJustReceived = RawRecv( &_recvBuffer[0], recvBufferSize );
        error = _HandleError( bytesJustReceived, waitTime );
    }
    while( waitTime > XDuration::Zero() && error > XSSL_ERROR_NONE );

    if( error == XSSL_ERROR_NONE && bytesJustReceived > 0 )
    {
        _recvPos = 0;
        _bufferedBytes = bytesJustReceived;
        return true;
    }
    return false;
}

ssize_t XSSLSocket::RawSend( const void* msg, size_t msgLen )
{
    //If we call RawSend and _ssl is NULL, we could segfault if SIGPIPE isn't suppressed. I haven't seen
    //this behavior, but I am copying from RawRecv as that is 100% observable and repeatable
    //This could happen if another thread called close on the socket

    if( _ssl == NULL )
        return -1;

    return SSL_write( _ssl, msg, msgLen );
}

ssize_t XSSLSocket::RawRecv( void* buf, size_t msgLen )
{
    if( _ssl == NULL )
        return -1;

    return SSL_read( _ssl, buf, msgLen );
}

void XSSLSocket::AttachRecvTimeoutHandler( TimeoutHandler rtcb, void* opaque )
{
    _recvTimeoutHandler = rtcb;
    _recvTimeoutHandlerOpaque = opaque;
}

void XSSLSocket::SetRecvTimeout( XDuration timeout )
{
    _recvTimeout = timeout;
}

void XSSLSocket::AttachSendTimeoutHandler( TimeoutHandler stcb, void* opaque )
{
    _sendTimeoutHandler = stcb;
    _sendTimeoutHandlerOpaque = opaque;
}

void XSSLSocket::SetSendTimeout( XDuration timeout )
{
    _sendTimeout = timeout;
}

bool XSSLSocket::ReadyToRecv()
{
    int fd = SSL_get_fd(_ssl);
    ssize_t retVal = _CanRecvData(XDuration::Zero(), fd);

    if( retVal )
        return true;

    return false;
}

bool XSSLSocket::WaitRecv( XDuration& waitTime )
{
    if( _SomeBuffered() )
        return false;

    return _WaitToRecv( waitTime );
}

bool XSSLSocket::WaitSend( XDuration& waitTime )
{
    return _WaitToSend( waitTime );
}

bool XSSLSocket::_WaitToRecv( XDuration& waitTime )
{
    if( _ssl == NULL )
        return true;

    const uint64_t before = XMonoClock::GetTime();
    int fd = SSL_get_fd(_ssl);
    ssize_t retVal = _CanRecvData(waitTime, fd);

    if( _sok->Valid() && retVal < 0 )
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

bool XSSLSocket::ReadyToSend()
{
    int fd = SSL_get_fd(_ssl);
    ssize_t retVal = _CanSendData(XDuration::Zero(), fd);

    if( retVal )
        return true;

    return false;
}

// Returns true on timeout.
bool XSSLSocket::_WaitToSend( XDuration& waitTime )
{
    const uint64_t before = XMonoClock::GetTime();
    int fd = SSL_get_fd(_ssl);
    ssize_t retVal = _CanSendData(waitTime, fd);

    if( _sok->Valid() && retVal < 0 )
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

XString XSSLSocket::GetHost() const
{
    return _host;
}

void XSSLSocket::SetHost( XString host )
{
    _host = host;
}

XString XSSLSocket::GetPeerIp() const
{
    struct sockaddr_storage peer;
    int peerLength = sizeof(peer);

    int fd = SSL_get_fd(_ssl);

#ifdef WIN32
    if ( getpeername( fd,(sockaddr*)&peer,&peerLength) < 0 )
    {
        X_LOG_WARNING("Unable to get peer ip. %s", GetLastErrorMsg().c_str());
        return "";
    }
#else
    if ( getpeername( fd,(sockaddr*)&peer,(socklen_t*)&peerLength) < 0 )
    {
        X_LOG_WARNING("Unable to get peer ip: %s", GetLastErrorMsg().c_str());
        return "";
    }
#endif

    return XSocketAddress::AddressToString((sockaddr*)&peer, (socklen_t)peerLength);
}

ssize_t XSSLSocket::_CanRecvData( XDuration waitTime, int fd )
{
#ifdef WIN32
    ssize_t retVal = _DoSelectRecv(waitTime, fd);
#else
    ssize_t retVal = _DoPollRecv(waitTime, fd);
#endif
    return retVal;
}

ssize_t XSSLSocket::_CanSendData( XDuration waitTime, int fd )
{
#ifdef WIN32
    ssize_t retVal = _DoSelectSend(waitTime, fd);
#else
    ssize_t retVal = _DoPollSend(waitTime, fd);
#endif
    return retVal;
}



#ifndef WIN32
ssize_t XSSLSocket::_DoPollRecv( XDuration waitTime, int fd )
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

    ///Check for error conditions
#ifndef __APPLE__
    if( fds[0].revents & ( POLLERR | POLLRDHUP | POLLHUP | POLLNVAL) )
#else
    if( fds[0].revents & ( POLLERR | POLLHUP | POLLNVAL) )    
#endif
        retVal = -1;

    return retVal;
}

ssize_t XSSLSocket::_DoPollSend( XDuration waitTime, int fd )
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

ssize_t XSSLSocket::_DoSelectRecv( XDuration waitTime, int fd )
{
    fd_set recvFileDescriptors;
    FD_ZERO( &recvFileDescriptors );
    FD_SET( fd, &recvFileDescriptors );

    timeval recvTimeout = waitTime.ToTimeVal();
    return select( fd+1, &recvFileDescriptors, NULL, NULL, &recvTimeout );
}

ssize_t XSSLSocket::_DoSelectSend( XDuration waitTime, int fd )
{
    fd_set sendFileDescriptors;
    FD_ZERO( &sendFileDescriptors );
    FD_SET( fd, &sendFileDescriptors );

    timeval sendTimeout = waitTime.ToTimeVal();
    return select( fd+1, NULL, &sendFileDescriptors, NULL, &sendTimeout );
}

size_t XSSLSocket::_Send( const void* msg, size_t msgLen, XDuration sendTimeout )
{
    if( !msg || (msgLen <= 0) )
        X_THROW(( "Invalid argument passed to XSSLSocket::Send()."));

    bool timedOut = false;
    XDuration waitTime = sendTimeout;

    size_t bytesToWrite = msgLen;
    char* writer = (char*)msg;

    while( _sok->Valid() && !timedOut && (bytesToWrite > 0) )
    {
        ssize_t bytesJustWritten = 0;
        int error = -1;

        XGuard guard(_sslLok);
        do
        {
            bytesJustWritten = RawSend( writer, bytesToWrite );
            error = _HandleError( bytesJustWritten, waitTime );
        }
        while( waitTime > XDuration::Zero() && error > XSSL_ERROR_NONE );

        if( error == XSSL_ERROR_NONE && bytesJustWritten > 0 )
        {
            bytesToWrite -= bytesJustWritten;
            writer += bytesJustWritten;
            continue;
        }
        else if( error == XSSL_ERROR_TIMEOUT )
        {
            timedOut = true;
            // callback returns true if we should try again...
            bool retry = (_sendTimeoutHandler) ? !_sendTimeoutHandler( _sendTimeoutHandlerOpaque ) : false;

            if( retry )
            {
                timedOut = false;
                waitTime = sendTimeout;
            }
            else
            {
                X_LOG_INFO("Socket send timed out.");
                XLog::LogBacktrace();
                break;
            }
        }
        else if( error == XSSL_ERROR_CRITICAL )
        {
            X_LOG_WARNING("Critical SSL Error in _Send()");
            XLog::LogBacktrace();
            break;
        }
    }

    if( bytesToWrite > 0 )
    {
        X_LOG_WARNING("Failed socket send in _Send()");
        XLog::LogBacktrace();
        Close();
    }
    return (msgLen - bytesToWrite);
}

size_t XSSLSocket::_Recv( void* buf, size_t msgLen, XDuration recvTimeout )
{
    if( !buf || (msgLen <= 0) )
        X_THROW(( "Invalid argument passed to XSSLSocket::Recv()."));

    bool timedOut = false;
    XDuration waitTime = recvTimeout;

    ssize_t bytesToRecv = msgLen;
    char* writer = (char*)buf;

    while( _sok->Valid() && !timedOut && (bytesToRecv > 0) )
    {
        ssize_t bytesJustReceived = 0;

        if( _SomeBuffered() )
        {
            bytesJustReceived = _ReadRecvBuffer( writer, bytesToRecv );
        }
        else
        {

            int error = -1;
            size_t recvBufferSize = _recvBuffer.size();
            XGuard guard(_sslLok);
            do
            {
                bytesJustReceived = RawRecv( &_recvBuffer[0], recvBufferSize );
                error = _HandleError( bytesJustReceived, waitTime );
            }
            while( waitTime > XDuration::Zero() && error > XSSL_ERROR_NONE );
            if( error == XSSL_ERROR_NONE && bytesJustReceived > 0 )
            {
                _recvPos = 0;
                _bufferedBytes = bytesJustReceived;
                continue;
            }
            else if( error == XSSL_ERROR_TIMEOUT )
            {
                timedOut = true;
                // callback returns true if we should try again...
                bool retry = (_recvTimeoutHandler) ? _recvTimeoutHandler( _recvTimeoutHandlerOpaque ) : false;

                if( retry )
                {
                    timedOut = false;
                    waitTime = recvTimeout;
                }
                else
                {
                    X_LOG_INFO("Receive on socket timed out.");
                    break;
                }
            }
            else if( error == XSSL_ERROR_CRITICAL )
            {
                X_LOG_WARNING("Failed socket recv in _Recv()");
                Close();
                break;
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


int XSSLSocket::_HandleError( int error, XDuration& waitTime )
{
    //Check if Close() was called by another thread before a Send/Recv/BufferedRecv was called.
    if( _ssl == NULL )
        return XSSL_ERROR_CRITICAL;

    bool timedOut;
    //This line converts the error we get from doing IO into
    //a value we can pass to the switch statement
    int sslError = SSL_get_error(_ssl, error );

    //This loop pops errors off of the openssl error queue
    //so we don't end up with an ever growing queue
    unsigned long errVal = ERR_get_error();
    while( errVal != 0 )
    {
        X_LOG_ERROR("SSL Error: %s", ERR_error_string(errVal, NULL) );
        errVal = ERR_get_error();
    }
    switch ( sslError )
    {
        case SSL_ERROR_NONE:
            {
                return XSSL_ERROR_NONE;
            }
        case SSL_ERROR_WANT_READ:
            {
                timedOut = _WaitToRecv( waitTime );
                break;
            }
        case SSL_ERROR_WANT_WRITE:
            {

                timedOut = _WaitToSend( waitTime );
                break;
            }
        default:
            {
                return XSSL_ERROR_CRITICAL;
            }
    }

    //Check if Close() was called while  in _WaitToRecv/Send
    if( _ssl == NULL )
        return XSSL_ERROR_CRITICAL;

    if( timedOut )
        return XSSL_ERROR_TIMEOUT;

    return XSSL_ERROR_RETRY;
}

size_t XSSLSocket::_ReadRecvBuffer( void* buf, size_t msgLen )
{
    if( _SomeBuffered() )
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

bool XSSLSocket::_SomeBuffered()
{
    return (_bufferedBytes > 0) ? true : false;
}

int XSSLSocket::_PrivateKeyPasswordCB( char *buf, int size, int rwflag, void *userdata )
{
    if( rwflag != 0 )
        X_THROW(( "Right now, we only support decrypting private keys."));

    if( !userdata )
        X_THROW(( "No userdata passed to password callback."));

    XSSLSocket* sok = (XSSLSocket*)userdata;

    if( size < (int)sok->_privateKeyPassword.length() )
        X_THROW(( "Not enough space for the password."));

    memset( buf, 0, size );
    memcpy( buf, sok->_privateKeyPassword.c_str(), sok->_privateKeyPassword.length() );

    return sok->_privateKeyPassword.length();
}

int XSSLSocket::_X509CheckHost(X509 *xcert, const XString& hostname)
{
    GENERAL_NAMES *gens = NULL;
    X509_NAME *name = NULL;
    int i;

    gens = (GENERAL_NAMES*)X509_get_ext_d2i(xcert, NID_subject_alt_name, NULL, NULL);
    if (gens)
    {
        int rv = 0;
        for (i = 0; i < sk_GENERAL_NAME_num(gens); i++)
        {
            GENERAL_NAME *gen = NULL;
            ASN1_STRING *cstr = NULL;
            gen = sk_GENERAL_NAME_value(gens, i);

            if(gen->type != GEN_DNS)
                continue;

            cstr = gen->d.dNSName;

            if ( _CompareStrings(cstr, V_ASN1_IA5STRING, hostname) )
            {
                rv = 1;
                break;
            }
        }
        GENERAL_NAMES_free(gens);
        if (rv)
            return 1;

    }

    i = -1;
    name = X509_get_subject_name(xcert);
    while((i = X509_NAME_get_index_by_NID(name, NID_commonName, i)) >= 0)
    {
        X509_NAME_ENTRY *ne;
        ASN1_STRING *str;
        ne = X509_NAME_get_entry(name, i);
        str = X509_NAME_ENTRY_get_data(ne);
        if ( _CompareStrings( str, -1, hostname ) )
            return 1;
    }
    return 0;
}

/* Compare while ASCII ignoring case. */
int XSSLSocket::_EqualNocase(const unsigned char *pattern, size_t pattern_len,
        const unsigned char *subject, size_t subject_len)
{
    if (pattern_len != subject_len)
        return 0;
    while (pattern_len)
    {
        unsigned char l = *pattern;
        unsigned char r = *subject;
        /* The pattern must not contain NUL characters. */
        if (l == 0)
            return 0;
        if (l != r)
        {
            if ('A' <= l && l <= 'Z')
                l = (l - 'A') + 'a';
            if ('A' <= r && r <= 'Z')
                r = (r - 'A') + 'a';
            if (l != r)
                return 0;
        }
        ++pattern;
        ++subject;
        --pattern_len;
    }
    return 1;
}

/* Compare the prefix and suffix with the subject, and check that the
   characters in-between are valid. */
int XSSLSocket::_WildcardMatch(const unsigned char *prefix, size_t prefix_len,
        const unsigned char *suffix, size_t suffix_len,
        const unsigned char *subject, size_t subject_len)
{
    const unsigned char *wildcard_start;
    const unsigned char *wildcard_end;
    const unsigned char *p;
    if (subject_len < prefix_len + suffix_len)
        return 0;
    if (!_EqualNocase(prefix, prefix_len, subject, prefix_len))
        return 0;
    wildcard_start = subject + prefix_len;
    wildcard_end = subject + (subject_len - suffix_len);
    if (!_EqualNocase(wildcard_end, suffix_len, suffix, suffix_len))
        return 0;
    /* The wildcard must match at least one character. */
    if (wildcard_start == wildcard_end)
        return 0;
    /* Check that the part matched by the wildcard contains only
       permitted characters and only matches a single label. */
    for (p = wildcard_start; p != wildcard_end; ++p)
        if (!(('0' <= *p && *p <= '9') ||
                    ('A' <= *p && *p <= 'Z') ||
                    ('a' <= *p && *p <= 'z') ||
                    *p == '-'))
            return 0;
    return 1;
}

/* Checks if the memory region consistens of [0-9A-Za-z.-]. */
int XSSLSocket::_ValidDomainCharacters(const unsigned char *p, size_t len)
{
    while (len)
    {
        if (!(('0' <= *p && *p <= '9') ||
                    ('A' <= *p && *p <= 'Z') ||
                    ('a' <= *p && *p <= 'z') ||
                    *p == '-' || *p == '.'))
            return 0;
        ++p;
        --len;
    }
    return 1;
}

/* Find the '*' in a wildcard pattern.  If no such character is found
   or the pattern is otherwise invalid, returns NULL. */
const unsigned char *XSSLSocket::_WildcardFindStar(const unsigned char *pattern,
        size_t pattern_len)
{
    const unsigned char *star = (unsigned char*)memchr(pattern, '*', pattern_len);
    size_t dot_count = 0;
    const unsigned char *suffix_start;
    size_t suffix_length;
    if (star == NULL)
        return NULL;
    suffix_start = star + 1;
    suffix_length = (pattern + pattern_len) - (star + 1);
    if (!(_ValidDomainCharacters(pattern, star - pattern) &&
                _ValidDomainCharacters(suffix_start, suffix_length)))
        return NULL;
    /* Check that the suffix matches at least two labels. */
    while (suffix_length)
    {
        if (*suffix_start == '.')
            ++dot_count;
        ++suffix_start;
        --suffix_length;
    }
    if (dot_count < 2)
        return NULL;
    return star;
}

/* Compare using wildcards. */
int XSSLSocket::_EqualWildcard(const unsigned char *pattern, size_t pattern_len,
        const unsigned char *subject, size_t subject_len)
{
    const unsigned char *star = _WildcardFindStar(pattern, pattern_len);
    if (star == NULL)
        return _EqualNocase(pattern, pattern_len,
                subject, subject_len);
    return _WildcardMatch(pattern, star - pattern,
            star + 1, (pattern + pattern_len) - star - 1,
            subject, subject_len);
}

/* Compare an ASN1_STRING to a supplied string. If they match
 * return 1. If cmp_type > 0 only compare if string matches the
 * type, otherwise convert it to UTF8.
 */

int XSSLSocket::_CompareStrings(ASN1_STRING *a, int cmp_type, XString hostname )
{
    if (!a->data || !a->length)
        return 0;

    if (cmp_type > 0)
    {
        if (cmp_type != a->type)
            return 0;
        if (cmp_type == V_ASN1_IA5STRING)
            return _EqualWildcard(a->data, a->length, (unsigned char*)hostname.c_str(), hostname.length() );
        if (a->length == (int)hostname.length() && !memcmp(a->data, (unsigned char*)hostname.c_str(), hostname.length() ))
            return 1;
        else
            return 0;
    }
    else
    {
        int astrlen = 0, rv = 0;
        unsigned char *astr = NULL;
        try
        {
            astrlen = ASN1_STRING_to_UTF8(&astr, a);
            if (astrlen < 0)
                return -1;
            rv = _EqualWildcard(astr, astrlen, (unsigned char*)hostname.c_str(), hostname.length() );
            OPENSSL_free(astr);
        }
        catch(...)
        {
            if( astr )
                OPENSSL_free(astr);
            throw;
        }
        return rv;
    }
}
