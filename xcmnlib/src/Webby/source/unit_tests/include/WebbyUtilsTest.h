//=============================================================================
//Copyright (c) 2012 Pelco. All rights reserved.
//
// This file contains trade secrets of Pelco.  No part may be reproduced or
// transmitted in any form by any means or for any purpose without the express
// written permission of Pelco.
//
// $File: WebbyUtilsTest.h $
// $Revision:$
// $Date:$
// $Author: Jonathan M Davis $
//=============================================================================

#ifndef WEBBY_WebbyUtilsTest_h
#define WEBBY_WebbyUtilsTest_h


#include "framework.h"

class WebbyUtilsTest : public test_fixture
{
public:
    TEST_SUITE(WebbyUtilsTest);
        TEST(WebbyUtilsTest::TestAdjustHeaderName);
        TEST(WebbyUtilsTest::TestAdjustHeaderValue);
    TEST_SUITE_END();

    ~WebbyUtilsTest() throw() {}

    void setup() {}
    void teardown() {}

private:

    void TestAdjustHeaderName();
    void TestAdjustHeaderValue();
};

#endif
