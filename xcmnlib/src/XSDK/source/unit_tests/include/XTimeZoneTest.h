
#ifndef _XTimeZoneTest_H_
#define _XTimeZoneTest_H_

#include "framework.h"

#include "XSDK/XDuration.h"
#include "XSDK/XIRef.h"
#include "XSDK/XString.h"
#include "XSDK/XTimeZone.h"

class XTimeZoneTest : public test_fixture
{
public:
    TEST_SUITE(XTimeZoneTest);
        TEST(XTimeZoneTest::TestUnixTimeToTicks);
        TEST(XTimeZoneTest::TestTicksToUnixTime);
        TEST(XTimeZoneTest::TestUnixTimeEpochToTicks);
        TEST(XTimeZoneTest::TestTicksToUnixTimeEpoch);
        TEST(XTimeZoneTest::TestXUTC_Offsets);
        TEST(XTimeZoneTest::TestXUTC_ToXML);
        TEST(XTimeZoneTest::TestXSimpleTimeZone_Constructor);
        TEST(XTimeZoneTest::TestXSimpleTimeZone_Offsets);
        TEST(XTimeZoneTest::TestXSimpleTimeZone_UTCToTZ);
        TEST(XTimeZoneTest::TestXSimpleTimeZone_TZToUTC);
        TEST(XTimeZoneTest::TestXSimpleTimeZone_ToISOString);
        TEST(XTimeZoneTest::TestXSimpleTimeZone_FromISOString);
        TEST(XTimeZoneTest::TestXSimpleTimeZone_ToXML);
        TEST(XTimeZoneTest::TestXSimpleTimeZone_FromXML);
        TEST(XTimeZoneTest::TestXLocalTime_Offsets);
        TEST(XTimeZoneTest::TestXLocalTime_UTCToTZ);
        TEST(XTimeZoneTest::TestXLocalTime_TZToUTC);
        TEST(XTimeZoneTest::TestXLocalTime_ToXML);
        TEST(XTimeZoneTest::TestDSTSwitches);
    TEST_SUITE_END();

    virtual ~XTimeZoneTest() throw()
    {}

    void setup() {}
    void teardown() {}

protected:

    void TestUnixTimeToTicks();
    void TestTicksToUnixTime();
    void TestUnixTimeEpochToTicks();
    void TestTicksToUnixTimeEpoch();
    void TestXUTC_Offsets();
    void TestXUTC_ToXML();
    void TestXSimpleTimeZone_Constructor();
    void TestXSimpleTimeZone_Offsets();
    void TestXSimpleTimeZone_UTCToTZ();
    void TestXSimpleTimeZone_TZToUTC();
    void TestXSimpleTimeZone_ToISOString();
    void TestXSimpleTimeZone_FromISOString();
    void TestXSimpleTimeZone_ToXML();
    void TestXSimpleTimeZone_FromXML();
    void TestXLocalTime_Offsets();
    void TestXLocalTime_UTCToTZ();
    void TestXLocalTime_TZToUTC();
    void TestXLocalTime_ToXML();
    void TestDSTSwitches();

private:
    static bool CmpSTZ(XIRef<XSDK::XTimeZone> lhs, XIRef<XSDK::XTimeZone> rhs);
};

#endif

