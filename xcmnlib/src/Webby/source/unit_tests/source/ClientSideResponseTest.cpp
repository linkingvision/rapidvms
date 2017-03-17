
#include "ClientSideResponseTest.h"
#include "XSDK/XHash.h"
#include "XSDK/TimeUtils.h"
#include "XSDK/XSocket.h"
#include "Webby/ClientSideRequest.h"
#include "Webby/ClientSideResponse.h"
#include "Webby/ServerSideRequest.h"
#include "Webby/ServerSideResponse.h"
#include "Webby/WebbyException.h"

using namespace std;
using namespace XSDK;
using namespace WEBBY;
using namespace XSDK;

#ifdef WIN32
#define SLEEP(a) Sleep((a*1000))
#else
#define SLEEP(a) sleep(a)
#endif

REGISTER_TEST_FIXTURE(ClientSideResponseTest);

void ClientSideResponseTest::setup()
{
}


void ClientSideResponseTest::teardown()
{
}

bool ClientSideResponseTest::MyChunkHandler(void* context, XIRef<XSDK::XMemory> chunk, const ClientSideResponse& response)
{
    ClientSideResponseTest* testObject = (ClientSideResponseTest*)context;

    const unsigned char* chunkBuffer = chunk->Map();

    testObject->_total += *chunkBuffer;

    return true;
}

bool ClientSideResponseTest::MyPartHandler(void* context, XIRef<XSDK::XMemory> chunk, const XSDK::XHash<XSDK::XString>& partHeaders, const ClientSideResponse& response )
{
    ClientSideResponseTest* testObject = (ClientSideResponseTest*)context;

    const unsigned char* chunkBuffer = chunk->Map();

    testObject->_total += *chunkBuffer;

    return true;
}

void* ClientSideResponseTest::EntryPoint()
{
    if(_testNum == 0)
    {
        XRef<XSocket> socket = new XSocket;
        socket->Bind(2290);
        socket->Listen();

        XRef<XSocket> clientSocket = socket->Accept();

        ServerSideRequest ssRequest;
        ssRequest.ReadRequest(clientSocket);

        XString body = ssRequest.GetBodyAsString();

        ServerSideResponse ssResponse;
        ssResponse.SetBody(body);
        UT_ASSERT(ssResponse.WriteResponse(clientSocket));

        clientSocket->Shutdown(SOCKET_SHUT_FLAGS);
        clientSocket->Close();

        socket->Shutdown(SOCKET_SHUT_FLAGS);
        socket->Close();
    }
    else if(_testNum == 1)
    {
        XRef<XSocket> socket = new XSocket;
        socket->Bind(2292);
        socket->Listen();

        XRef<XSocket> clientSocket = socket->Accept();

        ServerSideRequest ssRequest;
        ssRequest.ReadRequest(clientSocket);

        ServerSideResponse ssResponse;
        ssResponse.SetContentType("application/x-rtsp-tunnelled");

        XIRef<XMemory> chunk = new XMemory;
        unsigned char* buf = &chunk->Extend(1);

        for(int i = 0; i < 10; i++)
        {
            *buf = i;
            ssResponse.WriteChunk(clientSocket, chunk);
        }
        UT_ASSERT(ssResponse.WriteChunkFinalizer(clientSocket));

        clientSocket->Shutdown(SOCKET_SHUT_FLAGS);
        clientSocket->Close();

        socket->Shutdown(SOCKET_SHUT_FLAGS);
        socket->Close();
    }
    else if(_testNum == 2)
    {
        XRef<XSocket> socket = new XSocket;
        socket->Bind(2294);
        socket->Listen();

        XRef<XSocket> clientSocket = socket->Accept();

        ServerSideRequest ssRequest;
        ssRequest.ReadRequest(clientSocket);

        XString body = ssRequest.GetBodyAsString();

        XString cont = "HTTP/1.1 100 Continue\r\n\r\n";
        clientSocket->Send(cont.c_str(), cont.length());

        ServerSideResponse ssResponse;
        ssResponse.SetBody(body);
        UT_ASSERT(ssResponse.WriteResponse(clientSocket));

        clientSocket->Shutdown(SOCKET_SHUT_FLAGS);
        clientSocket->Close();

        socket->Shutdown(SOCKET_SHUT_FLAGS);
        socket->Close();
    }
    else if(_testNum == 3)
    {
        XRef<XSocket> socket = new XSocket;
        socket->Bind(2296);
        socket->Listen();

        XRef<XSocket> clientSocket = socket->Accept();

        ServerSideRequest ssRequest;
        ssRequest.ReadRequest(clientSocket);

        ServerSideResponse ssResponse;
        ssResponse.SetContentType("application/x-rtsp-tunnelled");

        XIRef<XMemory> chunk = new XMemory;
        unsigned char* buf = &chunk->Extend(1);

        for(int i = 0; i < 10; i++)
        {
            *buf = i;
            ssResponse.WriteChunk(clientSocket, chunk);
            x_sleep(1);
        }
        ssResponse.WriteChunkFinalizer(clientSocket);

//      clientSocket->Shutdown(SOCKET_SHUT_FLAGS);
//      clientSocket->Close();

//      socket->Shutdown(SOCKET_SHUT_FLAGS);
//      socket->Close();
    }
    else if(_testNum == 4)
    {
        XRef<XSocket> socket = new XSocket;
        socket->Bind(2298);
        socket->Listen();

        XRef<XSocket> clientSocket = socket->Accept();

        ServerSideRequest ssRequest;
        ssRequest.ReadRequest(clientSocket);

        ServerSideResponse ssResponse;
        ssResponse.SetContentType("multipart/x-mixed-replace; boundary=\"foo\"");

        UT_ASSERT(ssResponse.WriteResponse(clientSocket));

        XIRef<XMemory> chunk = new XMemory;
        unsigned char* buf = &chunk->Extend(1);

        for(int i = 0; i < 10; i++)
        {
            *buf = i;
            XHash<XString> headers;
            headers.Add("Content-Type", "image/jpeg");
            UT_ASSERT(ssResponse.WritePart(clientSocket,
                          "foo",
                          headers,
                          chunk));
            x_sleep(1);
        }
        UT_ASSERT(ssResponse.WritePartFinalizer("foo", clientSocket));
    }
    else if(_testNum == 5)
    {
        XRef<XSocket> socket = new XSocket;
        socket->Bind(2300);
        socket->Listen();

        XRef<XSocket> clientSocket = socket->Accept();

        ServerSideRequest ssRequest;
        ssRequest.ReadRequest(clientSocket);

        ServerSideResponse ssResponse;
        ssResponse.AddAdditionalHeader("HeaderWithColon", "This header has a : in it.");

        UT_ASSERT(ssResponse.WriteResponse(clientSocket));
    }
    else if(_testNum == 6)
    {
        XRef<XSocket> socket = new XSocket;
        socket->Bind(2302);
        socket->Listen();

        XRef<XSocket> clientSocket = socket->Accept();

        ServerSideRequest ssRequest;
        ssRequest.ReadRequest(clientSocket);

        //Note: We don't use ServerSideResponse here because ServerSideResponse does not support
        //multiple header values associated with the same key. If this is needed, we could add it, but
        //for now we just return a manually constructed response.

        XString response = "HTTP/1.1 200 OK\r\nDate: Mon Nov 14 09:58:23 2011\r\nContent-Type: text/plain\r\nkey: val 1\r\nkey: val 2\r\n\r\n";
        clientSocket->Send(response.c_str(), response.length());
    }
    else if(_testNum == 7)
    {
        XRef<XSocket> socket = new XSocket;
        socket->Bind(2304);
        socket->Listen();

        XRef<XSocket> clientSocket = socket->Accept();

        ServerSideRequest ssRequest;
        ssRequest.ReadRequest(clientSocket);

        XString response = "HTTP/1.1 200 OK\r\n Date: Mon Nov 14 09:58:23 2011\r\nContent-Type: text/plain\r\nkey: val 1\r\nhole: val 2\r\nvalue:key\r\n\r\n";
        clientSocket->Send(response.c_str(), response.length());
    }
    else if(_testNum == 8)
    {
        XRef<XSocket> socket = new XSocket;
        socket->Bind(2306);
        socket->Listen();

        XRef<XSocket> clientSocket = socket->Accept();

        ServerSideRequest ssRequest;
        ssRequest.ReadRequest(clientSocket);

        XString response = "HTTP/1.1 200 OK\r\n\tDate: Mon Nov 14 09:58:23 2011\r\nContent-Type: text/plain\r\nkey: val 1\r\nhole: val 2\r\nvalue:key\r\n\r\n";
        clientSocket->Send(response.c_str(), response.length());
    }
    else if(_testNum == 9)
    {
        XRef<XSocket> socket = new XSocket;
        socket->Bind(2308);
        socket->Listen();

        XRef<XSocket> clientSocket = socket->Accept();

        ServerSideRequest ssRequest;
        ssRequest.ReadRequest(clientSocket);

        XString response = "HTTP/1.1 200 OK\r\nDate: Mon Nov 14 09:58:23 2011\r\nContent-Type: text/plain\r\nkey: val 1\r\nhole: val 2\r\n value:key\r\n\r\n";
        clientSocket->Send(response.c_str(), response.length());
    }
    else if(_testNum == 10)
    {
        XRef<XSocket> socket = new XSocket;
        socket->Bind(2310);
        socket->Listen();

        XRef<XSocket> clientSocket = socket->Accept();

        ServerSideRequest ssRequest;
        ssRequest.ReadRequest(clientSocket);

        XString response = "HTTP/1.1 200 OK\r\nDate: Mon Nov 14 09:58:23 2011\r\nContent-Type: text/plain\r\nkey: val 1\r\nhole: val 2\r\n\tvalue:key\r\n\r\n";
        clientSocket->Send(response.c_str(), response.length());
    }
    else if(_testNum == 11)
    {
        XRef<XSocket> socket = new XSocket;
        socket->Bind(2312);
        socket->Listen();

        XRef<XSocket> clientSocket = socket->Accept();

        ServerSideRequest ssRequest;
        ssRequest.ReadRequest(clientSocket);

        XString response = "HTTP/1.1 200 OK\r\nDate: Mon Nov 14 09:58:23 2011\r\nContent-Type: text/plain\r\nkey: val 1\r\nhole: val 2\r\nvalue:key\r\n\r\n";
        clientSocket->Send(response.c_str(), response.length());
    }

    return NULL;
}

void ClientSideResponseTest::TestDefaultConstructor()
{
    ClientSideResponse response;
}

void ClientSideResponseTest::TestCopyConstructor()
{
    ClientSideResponse ra;
    ClientSideResponse rb(ra);
}


void ClientSideResponseTest::TestAssignmentOperator()
{
    ClientSideResponse ra;
    ClientSideResponse rb = ra;
}

void ClientSideResponseTest::TestReceiveResponse()
{
    _testNum = 0;

    Start();

    SLEEP(1);

    XRef<XSocket> socket = new XSocket;
    socket->Connect("127.0.0.1", 2290);

    XString message = "Hello, Webby!";

    ClientSideRequest request;
    request.SetRequestType(kWebbyPOSTRequest);
    request.SetBody((unsigned char*)message.c_str(), message.length());

    UT_ASSERT(request.WriteRequest(socket));

    ClientSideResponse response;
    response.ReadResponse(socket);

    XString responseBody = response.GetBodyAsString();

    UT_ASSERT(responseBody == "Hello, Webby!");

    socket->Shutdown(SOCKET_SHUT_FLAGS);
    socket->Close();
}

void ClientSideResponseTest::TestStreaming()
{
    _testNum = 1;
    _total = 0;

    Start();

    SLEEP(1);

    XRef<XSocket> socket = new XSocket;
    socket->Connect("127.0.0.1", 2292);

    XString message = "Hello, Webby!";

    ClientSideRequest request;
    request.SetRequestType(kWebbyPOSTRequest);
    request.SetBody((unsigned char*)message.c_str(), message.length());

    UT_ASSERT(request.WriteRequest(socket));

    ClientSideResponse response;
    response.RegisterChunkHandler(MyChunkHandler, this, true);
    response.ReadResponse(socket);

    // For this test the server sends back 10 chunks, each 1 byte long... Containing the values
    // 9 down to 0. The sum of 0 .. 9 == 45. Our callback just sums them, so at this point our
    // _total should == 45.
    UT_ASSERT(_total == 45);

    socket->Shutdown(SOCKET_SHUT_FLAGS);
    socket->Close();
}

void ClientSideResponseTest::Test100Continue()
{
    _testNum = 2;

    Start();

    SLEEP(1);

    XRef<XSocket> socket = new XSocket;
    socket->Connect("127.0.0.1", 2294);

    XString message = "Hello, Webby!";

    ClientSideRequest request;
    request.SetRequestType(kWebbyPOSTRequest);
    request.SetBody((unsigned char*)message.c_str(), message.length());

    UT_ASSERT(request.WriteRequest(socket));

    ClientSideResponse response;
    response.ReadResponse(socket);

    XString responseBody = response.GetBodyAsString();

    UT_ASSERT(responseBody == "Hello, Webby!");

    socket->Shutdown(SOCKET_SHUT_FLAGS);
    socket->Close();
}

void ClientSideResponseTest::TestInterruptedStreaming()
{
    _testNum = 3;
    _total = 0;

    Start();

    SLEEP(1);

    XRef<XSocket> socket = new XSocket;
    socket->Connect("127.0.0.1", 2296);

    XString message = "Hello, Webby!";

    ClientSideRequest request;
    request.SetRequestType(kWebbyPOSTRequest);
    request.SetBody((unsigned char*)message.c_str(), message.length());

    request.WriteRequest(socket);

    SocketCloser socketCloser(socket, 5);
    socketCloser.Start();

    ClientSideResponse response;
    response.RegisterChunkHandler(MyChunkHandler, this, true);

    UT_ASSERT_THROWS(response.ReadResponse(socket), WebbyException);

    socketCloser.Join();

    // For this test the server sends back 10 chunks, each 1 byte long... Containing the values
    // 9 down to 0. But, we interrupt it 5 seconds in... So, the total here is probably 10 (0+1+2+3+4), but
    // the safer test (with less race conditions) is to just test that the _total is less than the 45 it would
    // be if it had run to completion.
    UT_ASSERT(_total < 45);
}

void ClientSideResponseTest::TestMultiPart()
{
    _testNum = 4;
    _total = 0;

    Start();

    SLEEP(1);

    XRef<XSocket> socket = new XSocket;
    socket->Connect("127.0.0.1", 2298);

    XString message = "Hello, Webby!";

    ClientSideRequest request;
    request.SetRequestType(kWebbyPOSTRequest);
    request.SetBody((unsigned char*)message.c_str(), message.length());

    UT_ASSERT(request.WriteRequest(socket));

    ClientSideResponse response;
    response.RegisterPartHandler(MyPartHandler, this);
    response.ReadResponse(socket);

    UT_ASSERT(_total == 45);
}

void ClientSideResponseTest::TestColonsInHeaders()
{
    _testNum = 5;
    _total = 0;

    Start();

    SLEEP(1);

    XRef<XSocket> socket = new XSocket;
    socket->Connect("127.0.0.1", 2300);

    ClientSideRequest request;
    UT_ASSERT(request.WriteRequest(socket));

    ClientSideResponse response;
    response.ReadResponse(socket);

    XString headerValue = response.GetHeader("HeaderWithColon");

    UT_ASSERT(headerValue.Contains("This header has a : in it."));
}

void ClientSideResponseTest::TestMultipleHeadersWithSameKey()
{
    _testNum = 6;
    _total = 0;

    Start();

    SLEEP(1);

    XRef<XSocket> socket = new XSocket;
    socket->Connect("127.0.0.1", 2302);

    ClientSideRequest request;
    UT_ASSERT(request.WriteRequest(socket));

    ClientSideResponse response;
    response.ReadResponse(socket);

    vector<XString> matchingHeaders = response.GetAllMatchingHeaders("key");

    int numHeaders = matchingHeaders.size();

    UT_ASSERT(numHeaders == 2);

    UT_ASSERT(matchingHeaders[0] == "val 1");
    UT_ASSERT(matchingHeaders[1] == "val 2");
}

void ClientSideResponseTest::TestSpaceAtHeaderStart()
{
    _testNum = 7;
    _total = 0;
    Start();
    SLEEP(1);

    XRef<XSocket> socket = new XSocket;
    socket->Connect("127.0.0.1", 2304);

    ClientSideRequest request;
    UT_ASSERT(request.WriteRequest(socket));

    ClientSideResponse response;
    UT_ASSERT_THROWS(response.ReadResponse(socket), WebbyException);
}

void ClientSideResponseTest::TestTabAtHeaderStart()
{
    _testNum = 8;
    _total = 0;
    Start();
    SLEEP(1);

    XRef<XSocket> socket = new XSocket;
    socket->Connect("127.0.0.1", 2306);

    ClientSideRequest request;
    UT_ASSERT(request.WriteRequest(socket));

    ClientSideResponse response;
    UT_ASSERT_THROWS(response.ReadResponse(socket), WebbyException);
}

void ClientSideResponseTest::TestSpaceAtHeaderLine()
{
    _testNum = 9;
    _total = 0;
    Start();
    SLEEP(1);

    XRef<XSocket> socket = new XSocket;
    socket->Connect("127.0.0.1", 2308);

    ClientSideRequest request;
    UT_ASSERT(request.WriteRequest(socket));

    ClientSideResponse response;
    response.ReadResponse(socket);

    vector<XString> matchingHeaders = response.GetAllMatchingHeaders("key");
    UT_ASSERT(matchingHeaders.size() == 1);
    UT_ASSERT(matchingHeaders[0] == "val 1");

    matchingHeaders = response.GetAllMatchingHeaders("hole");
    UT_ASSERT(matchingHeaders.size() == 1);
    UT_ASSERT(matchingHeaders[0] == "val 2\r\n value:key");

    UT_ASSERT(response.GetAllMatchingHeaders("value").empty());
}

void ClientSideResponseTest::TestTabAtHeaderLine()
{
    _testNum = 10;
    _total = 0;
    Start();
    SLEEP(1);

    XRef<XSocket> socket = new XSocket;
    socket->Connect("127.0.0.1", 2310);

    ClientSideRequest request;
    UT_ASSERT(request.WriteRequest(socket));

    ClientSideResponse response;
    response.ReadResponse(socket);

    vector<XString> matchingHeaders = response.GetAllMatchingHeaders("key");
    UT_ASSERT(matchingHeaders.size() == 1);
    UT_ASSERT(matchingHeaders[0] == "val 1");

    matchingHeaders = response.GetAllMatchingHeaders("hole");
    UT_ASSERT(matchingHeaders.size() == 1);
    UT_ASSERT(matchingHeaders[0] == "val 2\r\n\tvalue:key");

    UT_ASSERT(response.GetAllMatchingHeaders("value").empty());
}

void ClientSideResponseTest::TestMultipleKeys()
{
    _testNum = 11;
    _total = 0;
    Start();
    SLEEP(1);

    XRef<XSocket> socket = new XSocket;
    socket->Connect("127.0.0.1", 2312);

    ClientSideRequest request;
    UT_ASSERT(request.WriteRequest(socket));

    ClientSideResponse response;
    response.ReadResponse(socket);

    vector<XString> matchingHeaders = response.GetAllMatchingHeaders("key");
    UT_ASSERT(matchingHeaders.size() == 1);
    UT_ASSERT(matchingHeaders[0] == "val 1");

    matchingHeaders = response.GetAllMatchingHeaders("hole");
    UT_ASSERT(matchingHeaders.size() == 1);
    UT_ASSERT(matchingHeaders[0] == "val 2");

    matchingHeaders = response.GetAllMatchingHeaders("value");
    UT_ASSERT(matchingHeaders.size() == 1);
    UT_ASSERT(matchingHeaders[0] == "key");
}
