
#ifndef _XDateTupleTest_H_
#define _XDateTupleTest_H_

#include "framework.h"

#include "XSDK/XDateTuple.h"

class XDateTupleTest : public test_fixture
{
public:
    TEST_SUITE(XDateTupleTest);
        TEST(XDateTupleTest::TestConstructor);
        TEST(XDateTupleTest::TestEquality);
        TEST(XDateTupleTest::TestDayOfGregorianCal);
        TEST(XDateTupleTest::TestDayOfYear);
        TEST(XDateTupleTest::TestDayOfWeek);
        TEST(XDateTupleTest::TestSub);
        TEST(XDateTupleTest::TestYear);
        TEST(XDateTupleTest::TestMonth);
        TEST(XDateTupleTest::TestDay);
        TEST(XDateTupleTest::TestEndOfMonth);
        TEST(XDateTupleTest::TestToISOExtString);
        TEST(XDateTupleTest::TestToCustomString);
        TEST(XDateTupleTest::TestFromISOExtString);
    TEST_SUITE_END();

    virtual ~XDateTupleTest() throw()
    {}

    void setup() {}
    void teardown() {}

protected:

    void TestConstructor();
    void TestEquality();
    void TestDayOfGregorianCal();
    void TestDayOfYear();
    void TestDayOfWeek();
    void TestSub();
    void TestYear();
    void TestMonth();
    void TestDay();
    void TestEndOfMonth();
    void TestToISOExtString();
    void TestToCustomString();
    void TestFromISOExtString();

    void TestDayOfWeek_test(XSDK::XDateTuple date, int day, int expectedMonth, int expectedDay);
};


class XTimeTupleTest : public test_fixture
{
public:
    TEST_SUITE(XTimeTupleTest);
        TEST(XTimeTupleTest::TestConstructor);
        TEST(XTimeTupleTest::TestEquality);
        TEST(XTimeTupleTest::TestSub);
        TEST(XTimeTupleTest::TestHour);
        TEST(XTimeTupleTest::TestMinute);
        TEST(XTimeTupleTest::TestSecond);
        TEST(XTimeTupleTest::TestToISOExtString);
        TEST(XTimeTupleTest::TestToCustomString);
        TEST(XTimeTupleTest::TestFromISOExtString);
        TEST(XTimeTupleTest::TestCmpDateTime);
        TEST(XTimeTupleTest::TestAddDuration);
    TEST_SUITE_END();

    virtual ~XTimeTupleTest() throw()
    {}

    void setup() {}
    void teardown() {}

protected:

    void TestConstructor();
    void TestEquality();
    void TestSub();
    void TestHour();
    void TestMinute();
    void TestSecond();
    void TestToISOExtString();
    void TestToCustomString();
    void TestFromISOExtString();
    void TestCmpDateTime();
    void TestAddDuration();
};

#endif

