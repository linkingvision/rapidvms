
#include "ClientSideRequestTest.h"
#include "XSDK/XHash.h"
#include "XSDK/XSocket.h"
#include "Webby/ClientSideRequest.h"
#include "Webby/ServerSideRequest.h"
#include "Webby/SoapArgs.h"

using namespace std;
using namespace XSDK;
using namespace WEBBY;
using namespace XSDK;

#ifdef WIN32
#define SLEEP(a) Sleep((a*1000))
#else
#define SLEEP(a) sleep(a)
#endif


REGISTER_TEST_FIXTURE(ClientSideRequestTest);

void ClientSideRequestTest::setup()
{
}


void ClientSideRequestTest::teardown()
{
}

void* ClientSideRequestTest::EntryPoint()
{

    if(_testNum == 0)
    {
        XSocket socket;
        socket.Bind(4224);
        socket.Listen();

        XRef<XSocket> clientSocket = socket.Accept();

        ServerSideRequest request;

        request.ReadRequest(clientSocket);

        UT_ASSERT(request.GetMethod() == "get");

        URI uri = request.GetURI();

        UT_ASSERT(uri.GetFullRawURI() == "/vmas/device/status");

        clientSocket->Shutdown(SOCKET_SHUT_SEND_FLAGS);
        socket.Shutdown(SOCKET_SHUT_RECV_FLAGS);

        clientSocket->Close();
        socket.Close();
    }
    else if(_testNum == 1)
    {
        XSocket socket;
        socket.Bind(4334);
        socket.Listen();

        XRef<XSocket> clientSocket = socket.Accept();

        ServerSideRequest request;

        request.ReadRequest(clientSocket);

        UT_ASSERT(request.GetMethod() == "post");

        UT_ASSERT(request.GetContentType().Contains("x-www-form-urlencoded"));

        XHash<XString>& postVars = request.GetPostVars();

        UT_ASSERT(postVars.Find("name1"));
        UT_ASSERT(*postVars.Find("name1") == "val1");
        UT_ASSERT(postVars.Find("name2"));
        UT_ASSERT(*postVars.Find("name2") == "val2");

        URI uri = request.GetURI();

        UT_ASSERT(uri.GetFullRawURI() == "/vmas/device/status");

        XString bodyAsString = request.GetBodyAsString();

        clientSocket->Shutdown(SOCKET_SHUT_FLAGS);
        socket.Shutdown(SOCKET_SHUT_FLAGS);
        clientSocket->Close();
        socket.Close();
    }
    else if(_testNum == 2)
    {
        XSocket socket;
        socket.Bind(4444);
        socket.Listen();

        XRef<XSocket> clientSocket = socket.Accept();

        ServerSideRequest request;

        request.ReadRequest(clientSocket);

        UT_ASSERT(request.GetMethod() == "post");

        UT_ASSERT(request.GetBodySize() == 10);

        unsigned char body[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

        UT_ASSERT(memcmp(body, request.GetBody(), 10) == 0);

        URI uri = request.GetURI();

        UT_ASSERT(uri.GetFullRawURI() == "/vmas/device/status");

        XString bodyAsString = request.GetBodyAsString();

        clientSocket->Shutdown(SOCKET_SHUT_FLAGS);
        socket.Shutdown(SOCKET_SHUT_FLAGS);
        clientSocket->Close();
        socket.Close();
    }
    else if(_testNum == 3)
    {
        XSocket socket;
        socket.Bind(4554);
        socket.Listen();

        XRef<XSocket> clientSocket = socket.Accept();

        ServerSideRequest request;

        request.ReadRequest(clientSocket);

        UT_ASSERT(request.GetMethod() == "post");

        XString bodyAsString = request.GetBodyAsString();

        UT_ASSERT(bodyAsString == "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\"><s:Header><evHeader><userID>302</userID><UCN>uuid:c7824e90-d16f-434e-9af2-75dcb8444348</UCN></evHeader></s:Header><s:Body><u:Seek xmlns:u=\"urn:schemas-pelco-com:service:StreamControl:1\"><sessionId>uuid:0770218a-1dd2-11b2-a641-8ed9042c8688</sessionId><startTime>2010-09-07T18:02:05.147</startTime></u:Seek></s:Body></s:Envelope>");

        clientSocket->Shutdown(SOCKET_SHUT_FLAGS);
        socket.Shutdown(SOCKET_SHUT_FLAGS);
        clientSocket->Close();
        socket.Close();
    }
    else if(_testNum == 4)
    {
        XSocket socket;
        socket.Bind(4354);
        socket.Listen();

        XRef<XSocket> clientSocket = socket.Accept();

        ServerSideRequest request;
        request.ReadRequest(clientSocket);

        UT_ASSERT(request.GetMethod() == "get");

        UT_ASSERT(*request.GetHeader("hELLO") == "world");
        UT_ASSERT(*request.GetHeader("GOODbye") == "Cruel World");
        UT_ASSERT(*request.GetHeader("badbye") == "shouldn't have bought it");
        UT_ASSERT(*request.GetHeader("silly_duck") == "yes, he's silly");
        UT_ASSERT(request.GetHeader("silly duck") == 0);
        UT_ASSERT(request.GetHeader("george") == 0);

        clientSocket->Shutdown(SOCKET_SHUT_FLAGS);
        socket.Shutdown(SOCKET_SHUT_FLAGS);
        clientSocket->Close();
        socket.Close();
    }
    else if ( _testNum == 5 )
    {
        XRef<XSocket> clientSocket = new XSocket();
        SLEEP(1);
        clientSocket->Connect("127.0.0.1",4354);
        ClientSideRequest req;
        req.WriteRequest(clientSocket);
    }
    return NULL;
}

void ClientSideRequestTest::TestDefaultConstructor()
{
    ClientSideRequest request;
}

void ClientSideRequestTest::TestCopyConstructor()
{
    ClientSideRequest ra;
    ra.SetURI("/vmas/device/status/?a=1");
    ClientSideRequest rb(ra);
    UT_ASSERT(rb.GetURI().GetFullRawURI() == "/vmas/device/status?a=1");
}
void ClientSideRequestTest::TestMPF1422()
{
    _testNum = 5;

    Start();

    XSocket socket;
    socket.Bind(4354);
    socket.Listen();

    XRef<XSocket> clientSocket = socket.Accept();
    ServerSideRequest request;
    request.ReadRequest(clientSocket);

    UT_ASSERT(*request.GetHeader("Host") == "127.0.0.1:4354");
}

void ClientSideRequestTest::TestAssignmentOperator()
{
    ClientSideRequest ra;
    ra.SetURI("/vmas/device/status/");
    ClientSideRequest rb = ra;
    UT_ASSERT(rb.GetURI().GetFullRawURI() == "/vmas/device/status");
}

void ClientSideRequestTest::TestWriteRequest()
{

    _testNum = 0;

    Start();

    SLEEP(1);

    XRef<XSocket> socket = new XSocket;
    socket->Connect((char*)"127.0.0.1", 4224);

    ClientSideRequest request;

    request.SetURI("/vmas/device/status");

    request.WriteRequest(socket);

    socket->Shutdown(SOCKET_SHUT_FLAGS);
    socket->Close();

    Join();
}

void ClientSideRequestTest::TestWritePost()
{
    _testNum = 1;

    Start();

    SLEEP(1);

    XRef<XSocket> socket = new XSocket;
    socket->Connect((char*)"127.0.0.1", 4334);

    ClientSideRequest request;

    request.SetRequestType("POST");
    request.AddPostVar("name1", "val1");
    request.AddPostVar("name2", "val2");

    request.SetURI("/vmas/device/status");

    request.WriteRequest(socket);

    socket->Shutdown(SOCKET_SHUT_FLAGS);
    socket->Close();

    Join();
}

void ClientSideRequestTest::TestWritePostWithBody()
{
    _testNum = 2;

    Start();

    SLEEP(1);

    XRef<XSocket> socket = new XSocket;
    socket->Connect((char*)"127.0.0.1", 4444);

    ClientSideRequest request;

    request.SetURI("/vmas/device/status");

    request.SetRequestType("POST");

    unsigned char body[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    request.SetBody(body, 10);

    request.WriteRequest(socket);

    socket->Shutdown(SOCKET_SHUT_FLAGS);
    socket->Close();

    Join();
}

void ClientSideRequestTest::TestSoapConstruction()
{
    SoapArgs header;

    header.AddArg("evHeader.userID","302");
    header.AddArg("evHeader.UCN","uuid:c7824e90-d16f-434e-9af2-75dcb8444348");

    SoapArgs args;

    args.AddArg("sessionId","uuid:0770218a-1dd2-11b2-a641-8ed9042c8688");
    args.AddArg("startTime","2010-09-07T18:02:05.147");

    ClientSideRequest request = ClientSideRequest::CreateSoapRequest(header, "Seek", "urn:schemas-pelco-com:service:StreamControl:1", args);

    _testNum = 3;

    Start();

    SLEEP(1);

    XRef<XSocket> socket = new XSocket;
    socket->Connect((char*)"127.0.0.1", 4554);

    request.SetURI("/control/StreamControl-1");

    request.WriteRequest(socket);

    socket->Shutdown(SOCKET_SHUT_FLAGS);
    socket->Close();

    Join();
}

void ClientSideRequestTest::TestSoapConstruction2()
{
    SoapArgs header;

    header.AddArg("evHeader.userID","302");
    header.AddArg("evHeader.UCN","uuid:c7824e90-d16f-434e-9af2-75dcb8444348");

    XString args = "<sessionId>uuid:0770218a-1dd2-11b2-a641-8ed9042c8688</sessionId><startTime>2010-09-07T18:02:05.147</startTime>";

    ClientSideRequest request = ClientSideRequest::CreateSoapRequest(header, "Seek", "urn:schemas-pelco-com:service:StreamControl:1", args);

    _testNum = 3;

    Start();

    SLEEP(1);

    XRef<XSocket> socket = new XSocket;
    socket->Connect((char*)"127.0.0.1", 4554);

    request.SetURI("/control/StreamControl-1");

    request.WriteRequest(socket);

    socket->Shutdown(SOCKET_SHUT_FLAGS);
    socket->Close();

    Join();
}

// The return values of ServerSideRequest::GetHeader are supposed to have been stripped.
void ClientSideRequestTest::TestMPF1430()
{
    _testNum = 4;

    Start();

    SLEEP(1);

    XRef<XSocket> socket = new XSocket;
    socket->Connect((char*)"127.0.0.1", 4354);

    ClientSideRequest request;

    request.SetRequestType("GET");
    request.AddHeader("hello", "world");
    request.AddHeader("GOODBYE", " Cruel World");
    request.AddHeader("badbye", "shouldn't have bought it ");
    request.AddHeader("silly_duck", " yes, he's silly ");

    UT_ASSERT(request.GetHeader("hELLO") == "world");
    UT_ASSERT(request.GetHeader("GOODbye") == "Cruel World");
    UT_ASSERT(request.GetHeader("badbye") == "shouldn't have bought it");
    UT_ASSERT(request.GetHeader("silly_duck") == "yes, he's silly");
    UT_ASSERT(request.GetHeader("silly duck") == "");
    UT_ASSERT(request.GetHeader("george") == "");

    request.WriteRequest(socket);

    socket->Shutdown(SOCKET_SHUT_FLAGS);
    socket->Close();

    Join();
}
