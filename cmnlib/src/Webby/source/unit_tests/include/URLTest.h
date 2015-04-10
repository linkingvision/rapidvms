//=============================================================================
//Copyright (c) 2013 Pelco. All rights reserved.
//
// This file contains trade secrets of Pelco.  No part may be reproduced or
// transmitted in any form by any means or for any purpose without the express
// written permission of Pelco.
//
// $File: URLTest.h $
// $Revision:$
// $Date:$
// $Author: Jonathan M Davis $
//=============================================================================
#ifndef URLTest_h
#define URLTest_h

#include "framework.h"

class URLTest : public test_fixture
{
public:
    TEST_SUITE(URLTest);
        TEST(URLTest::TestDefaultConstructor);
        TEST(URLTest::Test4ArgConstructor);
        TEST(URLTest::Test3ArgConstructor);
        TEST(URLTest::TestStringConstructor);
        TEST(URLTest::TestEquality);
        TEST(URLTest::TestAddGetArgs);
        TEST(URLTest::TestClearGetArgs);
        TEST(URLTest::TestIsValidProtocol);
        TEST(URLTest::TestMakeAbsolute);
    TEST_SUITE_END();

    void setup() {}
    void teardown() {}

protected:

    void TestDefaultConstructor();
    void Test4ArgConstructor();
    void Test3ArgConstructor();
    void TestStringConstructor();
    void TestEquality();
    void TestAddGetArgs();
    void TestClearGetArgs();
    void TestIsValidProtocol();
    void TestMakeAbsolute();
};

#endif
