
#ifndef _XTimeTest_H_
#define _XTimeTest_H_

#include "framework.h"

class XTimeTest : public test_fixture
{
public:
    TEST_SUITE(XTimeTest);
        TEST(XTimeTest::TestConstructor);
        TEST(XTimeTest::TestEquality);
        TEST(XTimeTest::TestAddSubXTime);
        TEST(XTimeTest::TestAddSubXDuration);
        TEST(XTimeTest::TestAddSubAssign);
        TEST(XTimeTest::TestDayOfGregorianCal);
        TEST(XTimeTest::TestYear);
        TEST(XTimeTest::TestMonth);
        TEST(XTimeTest::TestDay);
        TEST(XTimeTest::TestHour);
        TEST(XTimeTest::TestMinute);
        TEST(XTimeTest::TestSecond);
        TEST(XTimeTest::TestFracSec);
        TEST(XTimeTest::TestTimeZone);
        TEST(XTimeTest::TestToLocalTime);
        TEST(XTimeTest::TestToUTC);
        TEST(XTimeTest::TestToOtherTZ);
        TEST(XTimeTest::TestToUnixTime);
        TEST(XTimeTest::TestToUnixTimeAsMSecs);
        TEST(XTimeTest::TestToTimeVal);
        TEST(XTimeTest::TestToXDateTuple);
        TEST(XTimeTest::TestToXTimeTuple);
        TEST(XTimeTest::TestToISOExtString);
        TEST(XTimeTest::TestToMPEG7String);
        TEST(XTimeTest::TestToCustomString);
        TEST(XTimeTest::TestToXML);
        TEST(XTimeTest::TestFromISOExtString);
        TEST(XTimeTest::TestFromMPEG7String);
        TEST(XTimeTest::TestFromXML);
    TEST_SUITE_END();

    virtual ~XTimeTest() throw()
    {}

    void setup() {}
    void teardown() {}

protected:

    void TestConstructor();
    void TestEquality();
    void TestAddSubXTime();
    void TestAddSubXDuration();
    void TestAddSubAssign();
    void TestDayOfGregorianCal();
    void TestYear();
    void TestMonth();
    void TestDay();
    void TestHour();
    void TestMinute();
    void TestSecond();
    void TestFracSec();
    void TestTimeZone();
    void TestToLocalTime();
    void TestToUTC();
    void TestToOtherTZ();
    void TestToUnixTime();
    void TestToUnixTimeAsMSecs();
    void TestToTimeVal();
    void TestToXDateTuple();
    void TestToXTimeTuple();
    void TestToISOExtString();
    void TestToMPEG7String();
    void TestToCustomString();
    void TestToXML();
    void TestFromISOExtString();
    void TestFromMPEG7String();
    void TestFromXML();
};

#endif

