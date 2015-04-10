
#ifndef _XDurationTest_H_
#define _XDurationTest_H_

#include "framework.h"

class XFracSecTest : public test_fixture
{
public:
    TEST_SUITE(XFracSecTest);
        TEST(XFracSecTest::TestConvert);
        TEST(XFracSecTest::TestConstructor);
        TEST(XFracSecTest::TestEquality);
        TEST(XFracSecTest::TestNegation);
        TEST(XFracSecTest::TestMSecs);
        TEST(XFracSecTest::TestUSecs);
        TEST(XFracSecTest::TestHNSecs);
        TEST(XFracSecTest::TestNSecs);
        TEST(XFracSecTest::TestToISOString);
        TEST(XFracSecTest::TestToMPEG7String);
        TEST(XFracSecTest::TestFromISOString);
        TEST(XFracSecTest::TestFromMPEG7String);
        TEST(XFracSecTest::TestIsNegative);
        TEST(XFracSecTest::TestIsZero);
    TEST_SUITE_END();

    virtual ~XFracSecTest() throw()
    {}

    void setup() {}
    void teardown() {}

protected:

    void TestConstructor();
    void TestConvert();
    void TestEquality();
    void TestNegation();
    void TestMSecs();
    void TestUSecs();
    void TestHNSecs();
    void TestNSecs();
    void TestToISOString();
    void TestToMPEG7String();
    void TestFromISOString();
    void TestFromMPEG7String();
    void TestIsNegative();
    void TestIsZero();
};


class XDurationTest : public test_fixture
{
public:
    TEST_SUITE(XDurationTest);
        TEST(XDurationTest::TestConstructor);
        TEST(XDurationTest::TestEquality);
        TEST(XDurationTest::TestAddSub);
        TEST(XDurationTest::TestAddSubAssign);
        TEST(XDurationTest::TestMul);
        TEST(XDurationTest::TestMulAssign);
        TEST(XDurationTest::TestDiv);
        TEST(XDurationTest::TestDiv2);
        TEST(XDurationTest::TestDivAssign);
        TEST(XDurationTest::TestNegation);
        TEST(XDurationTest::TestGetOnly);
        TEST(XDurationTest::TestTotal);
        TEST(XDurationTest::TestIsNegative);
        TEST(XDurationTest::TestIsZero);
        TEST(XDurationTest::TestFracSec);
        TEST(XDurationTest::TestToString);
    TEST_SUITE_END();

    virtual ~XDurationTest() throw()
    {}

    void setup() {}
    void teardown() {}

protected:

    void TestConstructor();
    void TestEquality();
    void TestAddSub();
    void TestAddSubAssign();
    void TestMul();
    void TestMulAssign();
    void TestDiv();
    void TestDiv2();
    void TestDivAssign();
    void TestNegation();
    void TestGetOnly();
    void TestTotal();
    void TestIsNegative();
    void TestIsZero();
    void TestFracSec();
    void TestToString();
};

#endif

