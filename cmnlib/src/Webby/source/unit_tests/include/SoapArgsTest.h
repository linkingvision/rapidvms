//=============================================================================
//Copyright (c) 2009 Pelco. All rights reserved.
//
// This file contains trade secrets of Pelco.  No part may be reproduced or
// transmitted in any form by any means or for any purpose without the express
// written permission of Pelco.
//
// $File: SoapArgsTest.h $
// $Revision:$
// $Date:$
// $Author: Matt Spaulding $
//=============================================================================


#ifndef WEBBY_SOAP_ARGS_TEST_H
#define WEBBY_SOAP_ARGS_TEST_H

#include "framework.h"

class SoapArgsTest : public test_fixture
{
public:
    TEST_SUITE(SoapArgsTest);
        TEST(SoapArgsTest::TestConstructor);
        TEST(SoapArgsTest::TestArgSimple);
        TEST(SoapArgsTest::TestArgComplex);
        TEST(SoapArgsTest::TestMultipleTops);
        TEST(SoapArgsTest::TestAttributes);
    TEST_SUITE_END();

    ~SoapArgsTest() throw() {}
    void setup();
    void teardown();

protected:

    void TestConstructor();
    void TestArgSimple();
    void TestArgComplex();
    void TestMultipleTops();
    void TestAttributes();
};

#endif
