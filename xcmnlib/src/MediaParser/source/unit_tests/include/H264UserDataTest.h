
#ifndef _H264UserDataTest_H_
#define _H264UserDataTest_H_

#include <cppunit/extensions/HelperMacros.h>

class H264UserDataTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(H264UserDataTest);
        CPPUNIT_TEST(TestEmptySEI);
        CPPUNIT_TEST(TestUUID);
        CPPUNIT_TEST(TestRawSEI);
        CPPUNIT_TEST(TestZeroRawSEI);
        CPPUNIT_TEST(TestMultiplePayloads);
        CPPUNIT_TEST(TestNalify);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp() {}
    void tearDown() {}

protected:

    void TestEmptySEI();
    void TestUUID();
    void TestRawSEI();
    void TestZeroRawSEI();
    void TestMultiplePayloads();
    void TestNalify();
};

#endif
