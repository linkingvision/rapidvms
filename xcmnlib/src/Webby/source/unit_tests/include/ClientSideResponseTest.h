//=============================================================================
//Copyright (c) 2009 Pelco. All rights reserved.
//
// This file contains trade secrets of Pelco.  No part may be reproduced or
// transmitted in any form by any means or for any purpose without the express
// written permission of Pelco.
//
// $File: ClientSideRequestTest.h $
// $Revision:$
// $Date:$
// $Author: Matt Spaulding $
//=============================================================================


#ifndef _WEBBY_CLIENTSIDE_REQUEST_TEST_H_
#define _WEBBY_CLIENTSIDE_REQUEST_TEST_H_

#include "framework.h"
#include "XSDK/XTaskBase.h"
#include "XSDK/XMemory.h"
#include "XSDK/XRef.h"
#include "XSDK/XSocket.h"
#include "XSDK/TimeUtils.h"

namespace WEBBY
{
class ClientSideResponse;
}

class SocketCloser : public XSDK::XTaskBase
{
public:
    SocketCloser(XRef<XSDK::XSocket> socket, int delay) :
        _socket(socket),
        _delay(delay)
    {}

    ~SocketCloser() throw() {}

    virtual void* EntryPoint()
    {
        XSDK::x_sleep(_delay);

        _socket->Close();

        return NULL;
    }

private:
    XRef<XSDK::XSocket> _socket;
    int _delay;
};

class ClientSideResponseTest : public test_fixture, public XSDK::XTaskBase
{
public:
    TEST_SUITE_INIT(ClientSideResponseTest)
        _testNum(0),
        _total(0)
    TEST_SUITE_BEGIN()
        TEST(ClientSideResponseTest::TestDefaultConstructor);
        TEST(ClientSideResponseTest::TestCopyConstructor);
        TEST(ClientSideResponseTest::TestAssignmentOperator);
        TEST(ClientSideResponseTest::TestReceiveResponse);
        TEST(ClientSideResponseTest::TestStreaming);
        TEST(ClientSideResponseTest::Test100Continue);
        TEST(ClientSideResponseTest::TestInterruptedStreaming);
        TEST(ClientSideResponseTest::TestMultiPart);
        TEST(ClientSideResponseTest::TestColonsInHeaders);
        TEST(ClientSideResponseTest::TestMultipleHeadersWithSameKey);
        TEST(ClientSideResponseTest::TestSpaceAtHeaderStart);
        TEST(ClientSideResponseTest::TestTabAtHeaderStart);
        TEST(ClientSideResponseTest::TestSpaceAtHeaderLine);
        TEST(ClientSideResponseTest::TestTabAtHeaderLine);
        TEST(ClientSideResponseTest::TestMultipleKeys);
    TEST_SUITE_END()

public:

    virtual ~ClientSideResponseTest() throw() {}
    void setup();
    void teardown();

    static bool MyChunkHandler(void* context, XIRef<XSDK::XMemory> chunk, const WEBBY::ClientSideResponse& response);
    static bool MyPartHandler(void* context, XIRef<XSDK::XMemory> chunk, const XSDK::XHash<XSDK::XString>& partHeaders, const WEBBY::ClientSideResponse& response);

protected:

    virtual void* EntryPoint();

    void TestDefaultConstructor();
    void TestCopyConstructor();
    void TestAssignmentOperator();
    void TestReceiveResponse();
    void TestStreaming();
    void Test100Continue();
    void TestInterruptedStreaming();
    void TestMultiPart();
    void TestColonsInHeaders();
    void TestMultipleHeadersWithSameKey();
    void TestSpaceAtHeaderStart();
    void TestTabAtHeaderStart();
    void TestSpaceAtHeaderLine();
    void TestTabAtHeaderLine();
    void TestMultipleKeys();

 private:

    int _testNum;
    int _total;
};

#endif
