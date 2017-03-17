
#ifndef _XBitsyTest_H_
#define _XBitsyTest_H_

#include "framework.h"

class XBitsyTest : public test_fixture
{
public:
    TEST_SUITE(XBitsyTest);
        TEST(XBitsyTest::TestConstructor);
        TEST(XBitsyTest::TestSetBuffer);
        TEST(XBitsyTest::TestReset);
        TEST(XBitsyTest::TestGetBits);
        TEST(XBitsyTest::TestGetExactBits);
    TEST_SUITE_END();

    virtual ~XBitsyTest() throw()
    {}

    void setup() {}
    void teardown() {}

protected:

    void TestConstructor();
    void TestSetBuffer();
    void TestReset();
    void TestGetBits();
    void TestGetExactBits();
};

#endif

