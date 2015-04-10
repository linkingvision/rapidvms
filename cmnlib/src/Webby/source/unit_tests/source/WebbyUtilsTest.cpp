//=============================================================================
//Copyright (c) 2012 Pelco. All rights reserved.
//
// This file contains trade secrets of Pelco.  No part may be reproduced or
// transmitted in any form by any means or for any purpose without the express
// written permission of Pelco.
//
// $File: WebbyUtilsTest.cpp $
// $Revision:$
// $Date:$
// $Author: Jonathan M Davis $
//=============================================================================

#include "WebbyUtilsTest.h"
#include "Webby/WebbyUtils.h"

using namespace std;
using namespace WEBBY;
using namespace XSDK;

REGISTER_TEST_FIXTURE(WebbyUtilsTest);

void WebbyUtilsTest::TestAdjustHeaderName()
{
    UT_ASSERT(AdjustHeaderName("") == "");
    UT_ASSERT(AdjustHeaderName("hello") == "hello");
    UT_ASSERT(AdjustHeaderName("hELlo") == "hello");

    UT_ASSERT_THROWS(AdjustHeaderName(" hello"), XException);
    UT_ASSERT_THROWS(AdjustHeaderName("hello "), XException);
    UT_ASSERT_THROWS(AdjustHeaderName("hello\r\n"), XException);
    UT_ASSERT_THROWS(AdjustHeaderName("hello\n"), XException);
    UT_ASSERT_THROWS(AdjustHeaderName("hello:world"), XException);
}


void WebbyUtilsTest::TestAdjustHeaderValue()
{
    UT_ASSERT(AdjustHeaderValue("") == "");
    UT_ASSERT(AdjustHeaderValue("hello") == "hello");
    UT_ASSERT(AdjustHeaderValue("hELlo") == "hELlo");
    UT_ASSERT(AdjustHeaderValue(" hello") == "hello");
    UT_ASSERT(AdjustHeaderValue("hello ") == ("hello"));
    UT_ASSERT(AdjustHeaderValue("  \t  hello \t ") == ("hello"));
    UT_ASSERT(AdjustHeaderValue("hello:world") == ("hello:world"));
}
