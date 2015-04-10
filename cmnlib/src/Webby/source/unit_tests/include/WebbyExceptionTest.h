#ifndef _WebbyExceptionTest_H_
#define _WebbyExceptionTest_H_

//=============================================================================
//Copyright (c) 2012 Pelco. All rights reserved.
//
// This file contains trade secrets of Pelco.  No part may be reproduced or
// transmitted in any form by any means or for any purpose without the express
// written permission of Pelco.
//
// $File: WebbyExceptionTest.h $
// $Revision:$
// $Date:$
// $Author: Jonathan M Davis $
//=============================================================================

#include "framework.h"

class WebbyExceptionTest : public test_fixture
{
public:
    TEST_SUITE(WebbyExceptionTest);
        TEST(WebbyExceptionTest::TestConstructors);
        TEST(WebbyExceptionTest::TestGetStatus);
        TEST(WebbyExceptionTest::TestThrowHTTP);
    TEST_SUITE_END();

    ~WebbyExceptionTest() throw() {}
    void setup() {}
    void teardown() {}

protected:

    void TestConstructors();
    void TestGetStatus();
    void TestThrowHTTP();
};

#endif
