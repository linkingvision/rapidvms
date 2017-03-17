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

class ClientSideRequestTest : public test_fixture, public XSDK::XTaskBase
{
public:
    TEST_SUITE(ClientSideRequestTest);
        TEST(ClientSideRequestTest::TestDefaultConstructor);
        TEST(ClientSideRequestTest::TestCopyConstructor);
        TEST(ClientSideRequestTest::TestAssignmentOperator);
        TEST(ClientSideRequestTest::TestWriteRequest);
        TEST(ClientSideRequestTest::TestWritePost);
        TEST(ClientSideRequestTest::TestWritePostWithBody);
        TEST(ClientSideRequestTest::TestSoapConstruction);
        TEST(ClientSideRequestTest::TestSoapConstruction2);
        TEST(ClientSideRequestTest::TestMPF1430);
        TEST(ClientSideRequestTest::TestMPF1422);
    TEST_SUITE_END();

    ~ClientSideRequestTest() throw() {}
    void setup();
    void teardown();

protected:

    virtual void* EntryPoint();

    void TestDefaultConstructor();
    void TestCopyConstructor();
    void TestAssignmentOperator();
    void TestWriteRequest();
    void TestWritePost();
    void TestWritePostWithBody();
    void TestSoapConstruction();
    void TestSoapConstruction2();
    void TestMPF1430();
    void TestMPF1422();

 private:

    int _testNum;
};

#endif
