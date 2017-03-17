
#include "XSSLSocketTest.h"
#include "XSDK/XSSLSocket.h"
#include "XSDK/XRef.h"
#include "XSDK/XGuard.h"
#include "XSDK/TimeUtils.h"

using namespace XSDK;
using namespace std;

REGISTER_TEST_FIXTURE(XSSLSocketTest);


namespace FOO
{
#include "CACert.cpp"
#include "ServerCRT_1.cpp"
#include "ServerCRT_2.cpp"
#include "PrivateKey_1.cpp"
#include "PrivateKey_2.cpp"
}

void XSSLSocketTest::setup()
{
    _serverIPAddr.clear();

    FILE* outFile = fopen( "CACert.pem", "wb" );
    fwrite( FOO::CACertPEM, 1, sizeof( FOO::CACertPEM ), outFile );
    fclose( outFile );

    outFile = fopen( "ServerCRT1.crt", "wb" );
    fwrite( FOO::ServerCRT_1, 1, sizeof( FOO::ServerCRT_1 ), outFile );
    fclose( outFile );

    outFile = fopen( "ServerCRT2.crt", "wb" );
    fwrite( FOO::ServerCRT_2, 1, sizeof( FOO::ServerCRT_2 ), outFile );
    fclose( outFile );

    outFile = fopen( "PrivateKey1.key", "wb" );
    fwrite( FOO::PrivateKey_1, 1, sizeof( FOO::PrivateKey_1 ), outFile );
    fclose( outFile );

    outFile = fopen( "PrivateKey2.key", "wb" );
    fwrite( FOO::PrivateKey_2, 1, sizeof( FOO::PrivateKey_2 ), outFile );
    fclose( outFile );
}

void XSSLSocketTest::teardown()
{
    unlink( "CACert.pem" );
    unlink( "ServerCRT1.crt" );
    unlink( "ServerCRT2.crt" );
    unlink( "PrivateKey1.key" );
    unlink( "PrivateKey2.key" );
}

void* XSSLSocketTest::EntryPoint()
{
    switch( _testNum )
    {
    case 0:
    {
        XRef<XSSLSocket> serverSocket = new XSSLSocket;

        serverSocket->UsePEMCertificateFile( "ServerCRT1.crt" );
        serverSocket->UsePEMRSAPrivateKeyFile( "PrivateKey1.key" );

        serverSocket->Bind( 8700, _serverIPAddr );
        serverSocket->Listen();

        XRef<XSocket> clientSocket = serverSocket->Accept();

        uint32_t val = 0;
        clientSocket->Recv( &val, 4 );

        val = val + 1;

        clientSocket->Send( &val, 4 );
    }
    break;
    case 1:
    {
        XRef<XSSLSocket> serverSocket = new XSSLSocket;

        serverSocket->UsePEMCertificateFile( "ServerCRT1.crt" );
        serverSocket->UsePEMRSAPrivateKeyFile( "PrivateKey1.key" );

        serverSocket->Bind( 8701, _serverIPAddr );
        serverSocket->Listen();

        XRef<XSocket> clientSocket = serverSocket->Accept();

        uint32_t val = 0;
        clientSocket->Recv( &val, 4 );

        val = val + 1;

        clientSocket->Send( &val, 4 );
    }
    break;
    case 2:
    {
        XRef<XSSLSocket> serverSocket = new XSSLSocket;

        serverSocket->UsePEMCertificateFile( "ServerCRT1.crt" );
        serverSocket->UsePEMRSAPrivateKeyFile( "PrivateKey1.key" );

        serverSocket->Bind( 8702, _serverIPAddr );
        serverSocket->Listen();

        XRef<XSocket> clientSocket = serverSocket->Accept();

        uint32_t val = 0;
        clientSocket->Recv( &val, 4 );

        val = val + 1;

        clientSocket->Send( &val, 4 );

        clientSocket = serverSocket->Accept();

        val = 0;
        clientSocket->Recv( &val, 4 );

        val = val + 1;

        clientSocket->Send( &val, 4 );
    }
    break;
    case 3:
    {
        XRef<XSSLSocket> socket = new XSSLSocket;
        x_sleep(1);
        socket->Connect( _serverIPAddr, 8704 );
    }
    break;
    case 4:
    {
        XRef<XSSLSocket> serverSocket = new XSSLSocket;

        serverSocket->UsePEMCertificateFile( "ServerCRT2.crt" );
        serverSocket->UsePEMRSAPrivateKeyFile( "PrivateKey2.key", "passphrase" );

        UT_ASSERT_NO_THROW( serverSocket->Bind( 8705, _serverIPAddr ) );
        serverSocket->Listen();

        XRef<XSocket> clientSocket = serverSocket->Accept();

        uint32_t val = 0;
        clientSocket->Recv( &val, 4 );

        val = val + 1;

        clientSocket->Send( &val, 4 );

    }
    break;
    case 5:
    {
        XRef<XSSLSocket> serverSocket = new XSSLSocket;

        serverSocket->UsePEMCertificateFile( "ServerCRT1.crt" );
        serverSocket->UsePEMRSAPrivateKeyFile( "PrivateKey1.key" );

        serverSocket->Bind( 8706, _serverIPAddr );
        serverSocket->Listen();

        XRef<XSocket> clientSocket = serverSocket->Accept();

        uint32_t val = 0;
        clientSocket->Recv( &val, 4 );

        val = val + 1;

        clientSocket->Send( &val, 4 );

        x_usleep( 500000 );
    }
    break;
    default:
        break;
    };
    return &_val;
}

void XSSLSocketTest::TestConnect()
{
    const char addrs[][32] = { "127.0.0.1", "::1" };
    _testNum = 0;

    for (int ii=0; ii<2; ++ii)
    {
        _serverIPAddr = addrs[ii];
        Start();

        x_usleep( 250000 );
        XRef<XSSLSocket> socket = new XSSLSocket;

        // Self signed cert will not verify...
        UT_ASSERT_NO_THROW( socket->Connect( _serverIPAddr, 8700 ) );

        uint32_t val = 41;

        socket->Send( &val, 4 );

        socket->Recv( &val, 4 );

        UT_ASSERT( val == 42 );

        Join();
    }
}

void XSSLSocketTest::TestNonDefaultRecvBufferSize()
{
    XRef<XSSLSocket> sok;
    UT_ASSERT_NO_THROW( sok = new XSSLSocket( 2048 ) );
}

void XSSLSocketTest::TestDestructor()
{
    XRef<XSSLSocket> sok;
    UT_ASSERT_NO_THROW( sok = new XSSLSocket( 2048 ) );

    UT_ASSERT_NO_THROW( sok.Clear() );
}

void XSSLSocketTest::TestCloseWarmSocket()
{
    // First, we use the socket to get it "warm"....

    const char addrs[][32] = { "127.0.0.1", "::1" };
    _testNum = 1;

    for (int ii=0; ii<2; ++ii)
    {
        _serverIPAddr = addrs[ii];
        Start();

        x_usleep( 250000 );
        XRef<XSSLSocket> socket = new XSSLSocket;

        // Self signed cert will not verify...
        UT_ASSERT_NO_THROW( socket->Connect( _serverIPAddr, 8701 ) );

        uint32_t val = 41;

        socket->Send( &val, 4 );

        socket->Recv( &val, 4 );

        UT_ASSERT( val == 42 );

        socket->Close();

        //This is different than before.  After a close a new _ssl
        //context is created so the socket can be reuse.
        //_ssl is only NULL when destroyed...
        UT_ASSERT( socket->_ssl == NULL );


        Join();
    }
}

void XSSLSocketTest::TestTwoConnects()
{
    _testNum = 2;
    const char addrs[][32] = { "127.0.0.1", "::1" };

    for (int ii=0; ii<2; ++ii)
    {
        _serverIPAddr = addrs[ii];
        Start();

        x_usleep( 250000 );
        XRef<XSSLSocket> socket = new XSSLSocket;

        // Self signed cert will not verify...
        UT_ASSERT_NO_THROW( socket->Connect( _serverIPAddr, 8702 ) );

        uint32_t val = 99;

        socket->Send( &val, 4 );

        socket->Recv( &val, 4 );

        UT_ASSERT( val == 100 );

        socket->Close();

        // Self signed cert will not verify...
        UT_ASSERT_NO_THROW( socket->Connect( _serverIPAddr, 8702 ) );

        uint32_t val2 = 99;

        socket->Send( &val2, 4 );

        socket->Recv( &val2, 4 );

        UT_ASSERT( (val + val2) == 200 );

        Join();
    }
}

void XSSLSocketTest::TestBind()
{
    const char addrs[][32] = { "127.0.0.1", "::1" };
    for (int ii=0; ii<2; ++ii)
    {
        XRef<XSSLSocket> serverSocket = new XSSLSocket;

        serverSocket->UsePEMCertificateFile( "ServerCRT1.crt" );
        serverSocket->UsePEMRSAPrivateKeyFile( "PrivateKey1.key" );

        serverSocket->Bind( 8703, addrs[ii] );

        UT_ASSERT( serverSocket->Valid() == true );
    }
}

void XSSLSocketTest::TestAccept()
{
    _testNum = 3;
    const char addrs[][32] = { "127.0.0.1", "::1" };
    for (int ii=0; ii<2; ++ii)
    {
        XRef<XSSLSocket> serverSocket = new XSSLSocket;

        serverSocket->UsePEMCertificateFile( "ServerCRT1.crt" );
        serverSocket->UsePEMRSAPrivateKeyFile( "PrivateKey1.key" );

        _serverIPAddr = addrs[ii];

        serverSocket->Bind( 8704, _serverIPAddr );
        serverSocket->Listen();

        Start();

        //XRef<XSSLSocket> clientSocket = (XRef<XSSLSocket>)serverSocket->AcceptSSL();
        XRef<XSSLSocket> clientSocket = (XRef<XSSLSocket>)serverSocket->Accept();

        UT_ASSERT( clientSocket->_ssl != NULL );

        Join();
    }
}

void XSSLSocketTest::TestEncryptedPrivateKey()
{
    _testNum = 4;
    const char addrs[][32] = { "127.0.0.1", "::1" };
    for (int ii=0; ii<2; ++ii)
    {
        _serverIPAddr = addrs[ii];
        Start();

        x_usleep( 250000 );
        XRef<XSSLSocket> socket = new XSSLSocket;

        // Self signed cert will not verify...
        UT_ASSERT_NO_THROW( socket->Connect( _serverIPAddr, 8705 ) );

        uint32_t val = 41;

        socket->Send( &val, 4 );

        socket->Recv( &val, 4 );

        UT_ASSERT( val == 42 );

        Join();
    }
}

void XSSLSocketTest::TestShutdown()
{
    _testNum = 5;
    const char addrs[][32] = { "127.0.0.1", "::1" };
    for (int ii=0; ii<2; ++ii)
    {
        _serverIPAddr = addrs[ii];
        Start();

        x_usleep( 250000 );
        XRef<XSSLSocket> socket = new XSSLSocket;


        // Note: Not call to VerifyConnection( true )
        // Self signed cert will not verify...
        UT_ASSERT_NO_THROW( socket->Connect( _serverIPAddr, 8706 ) );

        uint32_t val = 41;

        socket->Send( &val, 4 );

        socket->Recv( &val, 4 );

        UT_ASSERT( val == 42 );

        UT_ASSERT_NO_THROW( socket->Shutdown(SOCKET_SHUT_FLAGS) );

        Join();
    }
}

void XSSLSocketTest::TestVerify()
{
    XRef<XSSLSocket> socket = new XSSLSocket;

    socket->UseCACertificateFile( "CACert.pem" );
    socket->VerifyConnection( true );

    UT_ASSERT_NO_THROW( socket->Connect( "www.google.com", 443 ) );

}

void XSSLSocketTest::TestRecvTimeout()
{
    XRef<XSSLSocket> socket = new XSSLSocket;

    socket->AttachRecvTimeoutHandler( _RecvTimeoutCB, this );

    socket->SetRecvTimeout( 2000 );

    socket->Connect( "www.google.com", 443 );

    _val = 0;
    _recvTimeoutAnswer = false;

    char buffer[1024];

    socket->Recv( buffer, 1024 );

    UT_ASSERT( _val == 1 );
}

void XSSLSocketTest::TestXSocketAssign()
{
    XRef<XSSLSocket> sslSocket = new XSSLSocket;

    sslSocket->UseCACertificateFile( "CACert.pem" );
    sslSocket->VerifyConnection( true );

    XRef<XSocket> socket = sslSocket;

    UT_ASSERT_NO_THROW( socket->Connect( "www.google.com", 443 ) );
}
