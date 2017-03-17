
#include "XTimeZoneTest.h"

#include "XSDK/XTimeZone.h"

#include <cstdlib>
#include <ctime>

#include "XSDK/XDomParser.h"
#include "XSDK/XException.h"
#include "XSDK/XString.h"
#include "XSDK/XTime.h"

using namespace std;
using namespace XSDK;

REGISTER_TEST_FIXTURE(XTimeZoneTest);

void XTimeZoneTest::TestUnixTimeToTicks()
{
    UT_ASSERT(unixTimeToTicks(0) == 621355968000000000LL);  //Midnight, January 1st, 1970
    UT_ASSERT(unixTimeToTicks(86400) == 621355968000000000LL + 864000000000LL);  //Midnight, January 2nd, 1970
    UT_ASSERT(unixTimeToTicks(-86400) == 621355968000000000LL - 864000000000LL);  //Midnight, December 31st, 1969
}


void XTimeZoneTest::TestTicksToUnixTime()
{
    UT_ASSERT(ticksToUnixTime(621355968000000000LL) == 0);  //Midnight, January 1st, 1970
    UT_ASSERT(ticksToUnixTime(621355968000000000LL + 864000000000LL) == 86400);  //Midnight, January 2nd, 1970
    UT_ASSERT(ticksToUnixTime(621355968000000000LL - 864000000000LL) == -86400);  //Midnight, December 31st, 1969

    UT_ASSERT(ticksToUnixTime(621355968000000000LL + 864771234567LL) == 86477);
    UT_ASSERT(ticksToUnixTime(621355968000000000LL - 864771234567LL) == -86477);
}

void XTimeZoneTest::TestUnixTimeEpochToTicks()
{
    UT_ASSERT(unixTimeEpochToTicks(0) == 621355968000000000LL);  //Midnight, January 1st, 1970
    UT_ASSERT(unixTimeEpochToTicks(864000000000LL) == 621355968000000000LL + 864000000000LL);  //Midnight, January 2nd, 1970
    UT_ASSERT(unixTimeEpochToTicks(-864000000000LL) == 621355968000000000LL - 864000000000LL);  //Midnight, December 31st, 1969

    UT_ASSERT(unixTimeEpochToTicks(864771234567LL) == 621355968000000000LL + 864771234567LL);
    UT_ASSERT(unixTimeEpochToTicks(-864771234567LL) == 621355968000000000LL - 864771234567LL);
}


void XTimeZoneTest::TestTicksToUnixTimeEpoch()
{
    UT_ASSERT(ticksToUnixTimeEpoch(621355968000000000LL) == 0);  //Midnight, January 1st, 1970
    UT_ASSERT(ticksToUnixTimeEpoch(621355968000000000LL + 864000000000LL) == 864000000000LL);  //Midnight, January 2nd, 1970
    UT_ASSERT(ticksToUnixTimeEpoch(621355968000000000LL - 864000000000LL) == -864000000000LL);  //Midnight, December 31st, 1969

    UT_ASSERT(ticksToUnixTimeEpoch(621355968000000000LL + 864001234567LL) == 864001234567LL);
    UT_ASSERT(ticksToUnixTimeEpoch(621355968000000000LL - 864001234567LL) == -864001234567LL);
}


void XTimeZoneTest::TestXUTC_Offsets()
{
    UT_ASSERT(XUTC::Instance()->UTCOffset() == 0);
    UT_ASSERT(XUTC::Instance()->DSTOffset() == 0);
}


void XTimeZoneTest::TestXUTC_ToXML()
{
    XDomParser parser;

    UT_ASSERT(parser.GetXmlString(XIRef<XTimeZone>(XUTC::Instance())->ToXML()) == "<XUTC></XUTC>");
}


void XTimeZoneTest::TestXSimpleTimeZone_Constructor()
{
    UT_ASSERT_NO_THROW(XIRef<XTimeZone>(new XSimpleTimeZone(780)));
    UT_ASSERT_NO_THROW(XIRef<XTimeZone>(new XSimpleTimeZone(-780)));

    UT_ASSERT_THROWS(XIRef<XTimeZone>(new XSimpleTimeZone(781)), XException);
    UT_ASSERT_THROWS(XIRef<XTimeZone>(new XSimpleTimeZone(-781)), XException);

    UT_ASSERT_NO_THROW(XIRef<XTimeZone>(new XSimpleTimeZone(720, 60)));
    UT_ASSERT_NO_THROW(XIRef<XTimeZone>(new XSimpleTimeZone(-780, 60)));

    UT_ASSERT_THROWS(XIRef<XTimeZone>(new XSimpleTimeZone(721, 60)), XException);
    UT_ASSERT_THROWS(XIRef<XTimeZone>(new XSimpleTimeZone(720, 61)), XException);
    UT_ASSERT_THROWS(XIRef<XTimeZone>(new XSimpleTimeZone(0, -1)), XException);
    UT_ASSERT_THROWS(XIRef<XTimeZone>(new XSimpleTimeZone(0, -60)), XException);
}


void XTimeZoneTest::TestXSimpleTimeZone_Offsets()
{
    UT_ASSERT(XIRef<XTimeZone>(new XSimpleTimeZone(-8 * 60))->UTCOffset() == -8 * 60);
    UT_ASSERT(XIRef<XTimeZone>(new XSimpleTimeZone(-8 * 60, 50))->UTCOffset() == -8 * 60);

    UT_ASSERT(XIRef<XTimeZone>(new XSimpleTimeZone(7 * 60))->UTCOffset() == 7 * 60);
    UT_ASSERT(XIRef<XTimeZone>(new XSimpleTimeZone(7 * 60, 50))->UTCOffset() == 7 * 60);

    UT_ASSERT(XIRef<XTimeZone>(new XSimpleTimeZone(-8 * 60))->DSTOffset() == 0);
    UT_ASSERT(XIRef<XTimeZone>(new XSimpleTimeZone(-8 * 60, 50))->DSTOffset() == 50);

    UT_ASSERT(XIRef<XTimeZone>(new XSimpleTimeZone(7 * 60))->DSTOffset() == 0);
    UT_ASSERT(XIRef<XTimeZone>(new XSimpleTimeZone(7 * 60, 50))->DSTOffset() == 50);
}


void XTimeZoneTest::TestXSimpleTimeZone_UTCToTZ()
{
    UT_ASSERT(XIRef<XTimeZone>(new XSimpleTimeZone(-8 * 60))->UTCToTZ(0) == -288000000000LL);
    UT_ASSERT(XIRef<XTimeZone>(new XSimpleTimeZone(8 * 60))->UTCToTZ(0) == 288000000000LL);
    UT_ASSERT(XIRef<XTimeZone>(new XSimpleTimeZone(-8 * 60))->UTCToTZ(54321234567890LL) == 54033234567890LL);

    UT_ASSERT(XIRef<XTimeZone>(new XSimpleTimeZone(-8 * 60, 60))->UTCToTZ(0) == -252000000000LL);
    UT_ASSERT(XIRef<XTimeZone>(new XSimpleTimeZone(8 * 60, 60))->UTCToTZ(0) == 324000000000LL);
    UT_ASSERT(XIRef<XTimeZone>(new XSimpleTimeZone(-8 * 60, 60))->UTCToTZ(54321234567890LL) == 54069234567890LL);
}


void XTimeZoneTest::TestXSimpleTimeZone_TZToUTC()
{
    UT_ASSERT(XIRef<XTimeZone>(new XSimpleTimeZone(-8 * 60))->TZToUTC(-288000000000LL) == 0);
    UT_ASSERT(XIRef<XTimeZone>(new XSimpleTimeZone(8 * 60))->TZToUTC(288000000000LL) == 0);
    UT_ASSERT(XIRef<XTimeZone>(new XSimpleTimeZone(-8 * 60))->TZToUTC(54033234567890LL) == 54321234567890LL);

    UT_ASSERT(XIRef<XTimeZone>(new XSimpleTimeZone(-8 * 60, 60))->TZToUTC(-252000000000LL) == 0);
    UT_ASSERT(XIRef<XTimeZone>(new XSimpleTimeZone(8 * 60, 60))->TZToUTC(324000000000LL) == 0);
    UT_ASSERT(XIRef<XTimeZone>(new XSimpleTimeZone(-8 * 60, 60))->TZToUTC(54069234567890LL) == 54321234567890LL);
}


void XTimeZoneTest::TestXSimpleTimeZone_ToISOString()
{
    UT_ASSERT(XSimpleTimeZone::ToISOString(0) == "+00:00");
    UT_ASSERT(XSimpleTimeZone::ToISOString(1) == "+00:01");
    UT_ASSERT(XSimpleTimeZone::ToISOString(10) == "+00:10");
    UT_ASSERT(XSimpleTimeZone::ToISOString(59) == "+00:59");
    UT_ASSERT(XSimpleTimeZone::ToISOString(60) == "+01:00");
    UT_ASSERT(XSimpleTimeZone::ToISOString(90) == "+01:30");
    UT_ASSERT(XSimpleTimeZone::ToISOString(120) == "+02:00");
    UT_ASSERT(XSimpleTimeZone::ToISOString(480) == "+08:00");
    UT_ASSERT(XSimpleTimeZone::ToISOString(779) == "+12:59");

    UT_ASSERT(XSimpleTimeZone::ToISOString(-1) == "-00:01");
    UT_ASSERT(XSimpleTimeZone::ToISOString(-10) == "-00:10");
    UT_ASSERT(XSimpleTimeZone::ToISOString(-59) == "-00:59");
    UT_ASSERT(XSimpleTimeZone::ToISOString(-60) == "-01:00");
    UT_ASSERT(XSimpleTimeZone::ToISOString(-90) == "-01:30");
    UT_ASSERT(XSimpleTimeZone::ToISOString(-120) == "-02:00");
    UT_ASSERT(XSimpleTimeZone::ToISOString(-480) == "-08:00");
    UT_ASSERT(XSimpleTimeZone::ToISOString(-779) == "-12:59");
}


void XTimeZoneTest::TestXSimpleTimeZone_FromISOString()
{
    UT_ASSERT_THROWS(XSimpleTimeZone::FromISOString(""), XException);
    UT_ASSERT_THROWS(XSimpleTimeZone::FromISOString("Z"), XException);
    UT_ASSERT_THROWS(XSimpleTimeZone::FromISOString("-"), XException);
    UT_ASSERT_THROWS(XSimpleTimeZone::FromISOString("+"), XException);
    UT_ASSERT_THROWS(XSimpleTimeZone::FromISOString("-:"), XException);
    UT_ASSERT_THROWS(XSimpleTimeZone::FromISOString("+:"), XException);
    UT_ASSERT_THROWS(XSimpleTimeZone::FromISOString("-1:"), XException);
    UT_ASSERT_THROWS(XSimpleTimeZone::FromISOString("+1:"), XException);
    UT_ASSERT_THROWS(XSimpleTimeZone::FromISOString("1"), XException);
    UT_ASSERT_THROWS(XSimpleTimeZone::FromISOString("+1:0"), XException);

    UT_ASSERT(XSimpleTimeZone::FromISOString("+00:00")->UTCOffset() == XIRef<XTimeZone>(new XSimpleTimeZone(0))->UTCOffset());
    UT_ASSERT(XSimpleTimeZone::FromISOString("+00:01")->UTCOffset() == XIRef<XTimeZone>(new XSimpleTimeZone(1))->UTCOffset());
    UT_ASSERT(XSimpleTimeZone::FromISOString("+00:10")->UTCOffset() == XIRef<XTimeZone>(new XSimpleTimeZone(10))->UTCOffset());
    UT_ASSERT(XSimpleTimeZone::FromISOString("+00:59")->UTCOffset() == XIRef<XTimeZone>(new XSimpleTimeZone(59))->UTCOffset());
    UT_ASSERT(XSimpleTimeZone::FromISOString("+01:00")->UTCOffset() == XIRef<XTimeZone>(new XSimpleTimeZone(60))->UTCOffset());
    UT_ASSERT(XSimpleTimeZone::FromISOString("+01:30")->UTCOffset() == XIRef<XTimeZone>(new XSimpleTimeZone(90))->UTCOffset());
    UT_ASSERT(XSimpleTimeZone::FromISOString("+02:00")->UTCOffset() == XIRef<XTimeZone>(new XSimpleTimeZone(120))->UTCOffset());
    UT_ASSERT(XSimpleTimeZone::FromISOString("+08:00")->UTCOffset() == XIRef<XTimeZone>(new XSimpleTimeZone(480))->UTCOffset());

    UT_ASSERT(XSimpleTimeZone::FromISOString("-00:01")->UTCOffset() == XIRef<XTimeZone>(new XSimpleTimeZone(-1))->UTCOffset());
    UT_ASSERT(XSimpleTimeZone::FromISOString("-00:10")->UTCOffset() == XIRef<XTimeZone>(new XSimpleTimeZone(-10))->UTCOffset());
    UT_ASSERT(XSimpleTimeZone::FromISOString("-00:59")->UTCOffset() == XIRef<XTimeZone>(new XSimpleTimeZone(-59))->UTCOffset());
    UT_ASSERT(XSimpleTimeZone::FromISOString("-01:00")->UTCOffset() == XIRef<XTimeZone>(new XSimpleTimeZone(-60))->UTCOffset());
    UT_ASSERT(XSimpleTimeZone::FromISOString("-01:30")->UTCOffset() == XIRef<XTimeZone>(new XSimpleTimeZone(-90))->UTCOffset());
    UT_ASSERT(XSimpleTimeZone::FromISOString("-02:00")->UTCOffset() == XIRef<XTimeZone>(new XSimpleTimeZone(-120))->UTCOffset());
    UT_ASSERT(XSimpleTimeZone::FromISOString("-08:00")->UTCOffset() == XIRef<XTimeZone>(new XSimpleTimeZone(-480))->UTCOffset());

    UT_ASSERT(XSimpleTimeZone::FromISOString("+0")->UTCOffset() == XIRef<XTimeZone>(new XSimpleTimeZone(0))->UTCOffset());
    UT_ASSERT(XSimpleTimeZone::FromISOString("+1")->UTCOffset() == XIRef<XTimeZone>(new XSimpleTimeZone(60))->UTCOffset());
    UT_ASSERT(XSimpleTimeZone::FromISOString("+2")->UTCOffset() == XIRef<XTimeZone>(new XSimpleTimeZone(120))->UTCOffset());
    UT_ASSERT(XSimpleTimeZone::FromISOString("+1:00")->UTCOffset() == XIRef<XTimeZone>(new XSimpleTimeZone(60))->UTCOffset());
    UT_ASSERT(XSimpleTimeZone::FromISOString("+1:01")->UTCOffset() == XIRef<XTimeZone>(new XSimpleTimeZone(61))->UTCOffset());

    UT_ASSERT(XSimpleTimeZone::FromISOString("-0")->UTCOffset() == XIRef<XTimeZone>(new XSimpleTimeZone(0))->UTCOffset());
    UT_ASSERT(XSimpleTimeZone::FromISOString("-1")->UTCOffset() == XIRef<XTimeZone>(new XSimpleTimeZone(-60))->UTCOffset());
    UT_ASSERT(XSimpleTimeZone::FromISOString("-2")->UTCOffset() == XIRef<XTimeZone>(new XSimpleTimeZone(-120))->UTCOffset());
    UT_ASSERT(XSimpleTimeZone::FromISOString("-1:00")->UTCOffset() == XIRef<XTimeZone>(new XSimpleTimeZone(-60))->UTCOffset());
    UT_ASSERT(XSimpleTimeZone::FromISOString("-1:01")->UTCOffset() == XIRef<XTimeZone>(new XSimpleTimeZone(-61))->UTCOffset());
}


void XTimeZoneTest::TestXSimpleTimeZone_ToXML()
{
    XDomParser parser;

    UT_ASSERT(parser.GetXmlString(XIRef<XTimeZone>(new XSimpleTimeZone(0, 0))->ToXML()) == "<XSimpleTimeZone utcOffset=\"0\" dstOffset=\"0\"></XSimpleTimeZone>");
    UT_ASSERT(parser.GetXmlString(XIRef<XTimeZone>(new XSimpleTimeZone(1, 0))->ToXML()) == "<XSimpleTimeZone utcOffset=\"1\" dstOffset=\"0\"></XSimpleTimeZone>");
    UT_ASSERT(parser.GetXmlString(XIRef<XTimeZone>(new XSimpleTimeZone(10, 0))->ToXML()) == "<XSimpleTimeZone utcOffset=\"10\" dstOffset=\"0\"></XSimpleTimeZone>");
    UT_ASSERT(parser.GetXmlString(XIRef<XTimeZone>(new XSimpleTimeZone(59, 0))->ToXML()) == "<XSimpleTimeZone utcOffset=\"59\" dstOffset=\"0\"></XSimpleTimeZone>");
    UT_ASSERT(parser.GetXmlString(XIRef<XTimeZone>(new XSimpleTimeZone(60, 0))->ToXML()) == "<XSimpleTimeZone utcOffset=\"60\" dstOffset=\"0\"></XSimpleTimeZone>");
    UT_ASSERT(parser.GetXmlString(XIRef<XTimeZone>(new XSimpleTimeZone(90, 0))->ToXML()) == "<XSimpleTimeZone utcOffset=\"90\" dstOffset=\"0\"></XSimpleTimeZone>");
    UT_ASSERT(parser.GetXmlString(XIRef<XTimeZone>(new XSimpleTimeZone(120, 0))->ToXML()) == "<XSimpleTimeZone utcOffset=\"120\" dstOffset=\"0\"></XSimpleTimeZone>");
    UT_ASSERT(parser.GetXmlString(XIRef<XTimeZone>(new XSimpleTimeZone(480, 0))->ToXML()) == "<XSimpleTimeZone utcOffset=\"480\" dstOffset=\"0\"></XSimpleTimeZone>");

    UT_ASSERT(parser.GetXmlString(XIRef<XTimeZone>(new XSimpleTimeZone(0, 60))->ToXML()) == "<XSimpleTimeZone utcOffset=\"0\" dstOffset=\"60\"></XSimpleTimeZone>");
    UT_ASSERT(parser.GetXmlString(XIRef<XTimeZone>(new XSimpleTimeZone(1, 60))->ToXML()) == "<XSimpleTimeZone utcOffset=\"1\" dstOffset=\"60\"></XSimpleTimeZone>");
    UT_ASSERT(parser.GetXmlString(XIRef<XTimeZone>(new XSimpleTimeZone(10, 60))->ToXML()) == "<XSimpleTimeZone utcOffset=\"10\" dstOffset=\"60\"></XSimpleTimeZone>");
    UT_ASSERT(parser.GetXmlString(XIRef<XTimeZone>(new XSimpleTimeZone(59, 60))->ToXML()) == "<XSimpleTimeZone utcOffset=\"59\" dstOffset=\"60\"></XSimpleTimeZone>");
    UT_ASSERT(parser.GetXmlString(XIRef<XTimeZone>(new XSimpleTimeZone(60, 60))->ToXML()) == "<XSimpleTimeZone utcOffset=\"60\" dstOffset=\"60\"></XSimpleTimeZone>");
    UT_ASSERT(parser.GetXmlString(XIRef<XTimeZone>(new XSimpleTimeZone(90, 60))->ToXML()) == "<XSimpleTimeZone utcOffset=\"90\" dstOffset=\"60\"></XSimpleTimeZone>");
    UT_ASSERT(parser.GetXmlString(XIRef<XTimeZone>(new XSimpleTimeZone(120, 60))->ToXML()) == "<XSimpleTimeZone utcOffset=\"120\" dstOffset=\"60\"></XSimpleTimeZone>");
    UT_ASSERT(parser.GetXmlString(XIRef<XTimeZone>(new XSimpleTimeZone(480, 60))->ToXML()) == "<XSimpleTimeZone utcOffset=\"480\" dstOffset=\"60\"></XSimpleTimeZone>");

    UT_ASSERT(parser.GetXmlString(XIRef<XTimeZone>(new XSimpleTimeZone(-1, 0))->ToXML()) == "<XSimpleTimeZone utcOffset=\"-1\" dstOffset=\"0\"></XSimpleTimeZone>");
    UT_ASSERT(parser.GetXmlString(XIRef<XTimeZone>(new XSimpleTimeZone(-10, 0))->ToXML()) == "<XSimpleTimeZone utcOffset=\"-10\" dstOffset=\"0\"></XSimpleTimeZone>");
    UT_ASSERT(parser.GetXmlString(XIRef<XTimeZone>(new XSimpleTimeZone(-59, 0))->ToXML()) == "<XSimpleTimeZone utcOffset=\"-59\" dstOffset=\"0\"></XSimpleTimeZone>");
    UT_ASSERT(parser.GetXmlString(XIRef<XTimeZone>(new XSimpleTimeZone(-60, 0))->ToXML()) == "<XSimpleTimeZone utcOffset=\"-60\" dstOffset=\"0\"></XSimpleTimeZone>");
    UT_ASSERT(parser.GetXmlString(XIRef<XTimeZone>(new XSimpleTimeZone(-90, 0))->ToXML()) == "<XSimpleTimeZone utcOffset=\"-90\" dstOffset=\"0\"></XSimpleTimeZone>");
    UT_ASSERT(parser.GetXmlString(XIRef<XTimeZone>(new XSimpleTimeZone(-120, 0))->ToXML()) == "<XSimpleTimeZone utcOffset=\"-120\" dstOffset=\"0\"></XSimpleTimeZone>");
    UT_ASSERT(parser.GetXmlString(XIRef<XTimeZone>(new XSimpleTimeZone(-480, 0))->ToXML()) == "<XSimpleTimeZone utcOffset=\"-480\" dstOffset=\"0\"></XSimpleTimeZone>");

    UT_ASSERT(parser.GetXmlString(XIRef<XTimeZone>(new XSimpleTimeZone(-1, 60))->ToXML()) == "<XSimpleTimeZone utcOffset=\"-1\" dstOffset=\"60\"></XSimpleTimeZone>");
    UT_ASSERT(parser.GetXmlString(XIRef<XTimeZone>(new XSimpleTimeZone(-10, 60))->ToXML()) == "<XSimpleTimeZone utcOffset=\"-10\" dstOffset=\"60\"></XSimpleTimeZone>");
    UT_ASSERT(parser.GetXmlString(XIRef<XTimeZone>(new XSimpleTimeZone(-59, 60))->ToXML()) == "<XSimpleTimeZone utcOffset=\"-59\" dstOffset=\"60\"></XSimpleTimeZone>");
    UT_ASSERT(parser.GetXmlString(XIRef<XTimeZone>(new XSimpleTimeZone(-60, 60))->ToXML()) == "<XSimpleTimeZone utcOffset=\"-60\" dstOffset=\"60\"></XSimpleTimeZone>");
    UT_ASSERT(parser.GetXmlString(XIRef<XTimeZone>(new XSimpleTimeZone(-90, 60))->ToXML()) == "<XSimpleTimeZone utcOffset=\"-90\" dstOffset=\"60\"></XSimpleTimeZone>");
    UT_ASSERT(parser.GetXmlString(XIRef<XTimeZone>(new XSimpleTimeZone(-120, 60))->ToXML()) == "<XSimpleTimeZone utcOffset=\"-120\" dstOffset=\"60\"></XSimpleTimeZone>");
    UT_ASSERT(parser.GetXmlString(XIRef<XTimeZone>(new XSimpleTimeZone(-480, 60))->ToXML()) == "<XSimpleTimeZone utcOffset=\"-480\" dstOffset=\"60\"></XSimpleTimeZone>");
}


bool XTimeZoneTest::CmpSTZ(XIRef<XTimeZone> lhs, XIRef<XTimeZone> rhs)
{
    if(!lhs.IsValid() || !rhs.IsValid())
    {
        printf("-------> It's null, Captain!\n");
        fflush(stdout);

        return false;
    }

    const XSimpleTimeZone* r = dynamic_cast<const XSimpleTimeZone*>(lhs.Get());
    const XSimpleTimeZone* l = dynamic_cast<const XSimpleTimeZone*>(rhs.Get());

    return l->_utcOffset == r->_utcOffset && l->_dstOffset == r->_dstOffset;
}

void XTimeZoneTest::TestXSimpleTimeZone_FromXML()
{

    UT_ASSERT(CmpSTZ(XSimpleTimeZone::FromXML(new XDomParserNode("<XSimpleTimeZone utcOffset=\"0\" dstOffset=\"0\"></XSimpleTimeZone>")), new XSimpleTimeZone(0, 0)));
    UT_ASSERT(CmpSTZ(XSimpleTimeZone::FromXML(new XDomParserNode("<XSimpleTimeZone utcOffset=\"1\" dstOffset=\"0\"></XSimpleTimeZone>")), new XSimpleTimeZone(1, 0)));
    UT_ASSERT(CmpSTZ(XSimpleTimeZone::FromXML(new XDomParserNode("<XSimpleTimeZone utcOffset=\"10\" dstOffset=\"0\"></XSimpleTimeZone>")), new XSimpleTimeZone(10, 0)));
    UT_ASSERT(CmpSTZ(XSimpleTimeZone::FromXML(new XDomParserNode("<XSimpleTimeZone utcOffset=\"59\" dstOffset=\"0\"></XSimpleTimeZone>")), new XSimpleTimeZone(59, 0)));
    UT_ASSERT(CmpSTZ(XSimpleTimeZone::FromXML(new XDomParserNode("<XSimpleTimeZone utcOffset=\"60\" dstOffset=\"0\"></XSimpleTimeZone>")), new XSimpleTimeZone(60, 0)));
    UT_ASSERT(CmpSTZ(XSimpleTimeZone::FromXML(new XDomParserNode("<XSimpleTimeZone utcOffset=\"90\" dstOffset=\"0\"></XSimpleTimeZone>")), new XSimpleTimeZone(90, 0)));
    UT_ASSERT(CmpSTZ(XSimpleTimeZone::FromXML(new XDomParserNode("<XSimpleTimeZone utcOffset=\"120\" dstOffset=\"0\"></XSimpleTimeZone>")), new XSimpleTimeZone(120, 0)));
    UT_ASSERT(CmpSTZ(XSimpleTimeZone::FromXML(new XDomParserNode("<XSimpleTimeZone utcOffset=\"480\" dstOffset=\"0\"></XSimpleTimeZone>")), new XSimpleTimeZone(480, 0)));

    UT_ASSERT(CmpSTZ(XSimpleTimeZone::FromXML(new XDomParserNode("<XSimpleTimeZone utcOffset=\"0\" dstOffset=\"60\"></XSimpleTimeZone>")), new XSimpleTimeZone(0, 60)));
    UT_ASSERT(CmpSTZ(XSimpleTimeZone::FromXML(new XDomParserNode("<XSimpleTimeZone utcOffset=\"1\" dstOffset=\"60\"></XSimpleTimeZone>")), new XSimpleTimeZone(1, 60)));
    UT_ASSERT(CmpSTZ(XSimpleTimeZone::FromXML(new XDomParserNode("<XSimpleTimeZone utcOffset=\"10\" dstOffset=\"60\"></XSimpleTimeZone>")), new XSimpleTimeZone(10, 60)));
    UT_ASSERT(CmpSTZ(XSimpleTimeZone::FromXML(new XDomParserNode("<XSimpleTimeZone utcOffset=\"59\" dstOffset=\"60\"></XSimpleTimeZone>")), new XSimpleTimeZone(59, 60)));
    UT_ASSERT(CmpSTZ(XSimpleTimeZone::FromXML(new XDomParserNode("<XSimpleTimeZone utcOffset=\"60\" dstOffset=\"60\"></XSimpleTimeZone>")), new XSimpleTimeZone(60, 60)));
    UT_ASSERT(CmpSTZ(XSimpleTimeZone::FromXML(new XDomParserNode("<XSimpleTimeZone utcOffset=\"90\" dstOffset=\"60\"></XSimpleTimeZone>")), new XSimpleTimeZone(90, 60)));
    UT_ASSERT(CmpSTZ(XSimpleTimeZone::FromXML(new XDomParserNode("<XSimpleTimeZone utcOffset=\"120\" dstOffset=\"60\"></XSimpleTimeZone>")), new XSimpleTimeZone(120, 60)));
    UT_ASSERT(CmpSTZ(XSimpleTimeZone::FromXML(new XDomParserNode("<XSimpleTimeZone utcOffset=\"480\" dstOffset=\"60\"></XSimpleTimeZone>")), new XSimpleTimeZone(480, 60)));

    UT_ASSERT(CmpSTZ(XSimpleTimeZone::FromXML(new XDomParserNode("<XSimpleTimeZone utcOffset=\"-1\" dstOffset=\"0\"></XSimpleTimeZone>")), new XSimpleTimeZone(-1, 0)));
    UT_ASSERT(CmpSTZ(XSimpleTimeZone::FromXML(new XDomParserNode("<XSimpleTimeZone utcOffset=\"-10\" dstOffset=\"0\"></XSimpleTimeZone>")), new XSimpleTimeZone(-10, 0)));
    UT_ASSERT(CmpSTZ(XSimpleTimeZone::FromXML(new XDomParserNode("<XSimpleTimeZone utcOffset=\"-59\" dstOffset=\"0\"></XSimpleTimeZone>")), new XSimpleTimeZone(-59, 0)));
    UT_ASSERT(CmpSTZ(XSimpleTimeZone::FromXML(new XDomParserNode("<XSimpleTimeZone utcOffset=\"-60\" dstOffset=\"0\"></XSimpleTimeZone>")), new XSimpleTimeZone(-60, 0)));
    UT_ASSERT(CmpSTZ(XSimpleTimeZone::FromXML(new XDomParserNode("<XSimpleTimeZone utcOffset=\"-90\" dstOffset=\"0\"></XSimpleTimeZone>")), new XSimpleTimeZone(-90, 0)));
    UT_ASSERT(CmpSTZ(XSimpleTimeZone::FromXML(new XDomParserNode("<XSimpleTimeZone utcOffset=\"-120\" dstOffset=\"0\"></XSimpleTimeZone>")), new XSimpleTimeZone(-120, 0)));
    UT_ASSERT(CmpSTZ(XSimpleTimeZone::FromXML(new XDomParserNode("<XSimpleTimeZone utcOffset=\"-480\" dstOffset=\"0\"></XSimpleTimeZone>")), new XSimpleTimeZone(-480, 0)));

    UT_ASSERT(CmpSTZ(XSimpleTimeZone::FromXML(new XDomParserNode("<XSimpleTimeZone utcOffset=\"-1\" dstOffset=\"60\"></XSimpleTimeZone>")), new XSimpleTimeZone(-1, 60)));
    UT_ASSERT(CmpSTZ(XSimpleTimeZone::FromXML(new XDomParserNode("<XSimpleTimeZone utcOffset=\"-10\" dstOffset=\"60\"></XSimpleTimeZone>")), new XSimpleTimeZone(-10, 60)));
    UT_ASSERT(CmpSTZ(XSimpleTimeZone::FromXML(new XDomParserNode("<XSimpleTimeZone utcOffset=\"-59\" dstOffset=\"60\"></XSimpleTimeZone>")), new XSimpleTimeZone(-59, 60)));
    UT_ASSERT(CmpSTZ(XSimpleTimeZone::FromXML(new XDomParserNode("<XSimpleTimeZone utcOffset=\"-60\" dstOffset=\"60\"></XSimpleTimeZone>")), new XSimpleTimeZone(-60, 60)));
    UT_ASSERT(CmpSTZ(XSimpleTimeZone::FromXML(new XDomParserNode("<XSimpleTimeZone utcOffset=\"-90\" dstOffset=\"60\"></XSimpleTimeZone>")), new XSimpleTimeZone(-90, 60)));
    UT_ASSERT(CmpSTZ(XSimpleTimeZone::FromXML(new XDomParserNode("<XSimpleTimeZone utcOffset=\"-120\" dstOffset=\"60\"></XSimpleTimeZone>")), new XSimpleTimeZone(-120, 60)));
    UT_ASSERT(CmpSTZ(XSimpleTimeZone::FromXML(new XDomParserNode("<XSimpleTimeZone utcOffset=\"-480\" dstOffset=\"60\"></XSimpleTimeZone>")), new XSimpleTimeZone(-480, 60)));
}


#ifdef IS_LINUX
class ScopedLocalTZ
{
public:
    ScopedLocalTZ(XString tzname)
        : _env()
    {
        unsetTZ();
        setTZ(tzname);
    }

    ~ScopedLocalTZ()
    {
        unsetTZ();
    }

private:

    void setTZ(XString tzname)
    {
        _env = XString("TZ=:/usr/share/zoneinfo/") + tzname;
        putenv((char*)_env.c_str());
        tzset();
    }

    void unsetTZ()
    {
        unsetenv("TZ");
        tzset();
    }

    XString _env;
};
#endif


void XTimeZoneTest::TestXLocalTime_Offsets()
{
#ifdef WIN32
    //We need to find a way to set the time zone in Windows.
    //const int utcOffset = XLocalTime::Instance()->UTCOffset();
    //const int dstOffset = XLocalTime::Instance()->DSTOffset();
    //UT_ASSERT(utcOffset == -8 * 60);
    //UT_ASSERT(dstOffset == 60);
#else
    {
        ScopedLocalTZ tz("America/Los_Angeles");

        UT_ASSERT(XLocalTime::Instance()->UTCOffset() == -8 * 60);
        UT_ASSERT(XLocalTime::Instance()->DSTOffset() == 60);
    }

    {
        ScopedLocalTZ tz("America/New_York");
        UT_ASSERT(XLocalTime::Instance()->UTCOffset() == -5 * 60);
        UT_ASSERT(XLocalTime::Instance()->DSTOffset() == 60);
    }
    {
        ScopedLocalTZ tz("UTC");
        UT_ASSERT(XLocalTime::Instance()->UTCOffset() == 0);
        UT_ASSERT(XLocalTime::Instance()->DSTOffset() == 0);
    }

    {
        ScopedLocalTZ tz("Europe/Paris");
        UT_ASSERT(XLocalTime::Instance()->UTCOffset() == 60);

    // For some reason, this assert fails on Sarix... Don't know why! The sarix has
    // a /usr/share/zoneinfo/Europe/Paris file... Jonathan should take a look at this
    // at some point...
        //UT_ASSERT(XLocalTime::Instance()->DSTOffset() == 60);
    }
#endif
}


void XTimeZoneTest::TestXLocalTime_UTCToTZ()
{
#ifdef WIN32
    //I don't really like hard-coding a Pacific Time zone test like this,
    //but I _would_ like to be able to test this stuff on Windows, and thus
    //far, I have completely failed at finding a way to actually set the local
    //time zone on Windows. So, until someone outside of the Pacific Time zone
    //is actually developing with this code, I'm leaving this test in.
    //UT_ASSERT(XLocalTime::Instance()->UTCToTZ(621355968000000000LL) == 621355680000000000LL);
    //UT_ASSERT(XLocalTime::Instance()->UTCToTZ(631100635210000000LL) == 631100347210000000LL);
    //UT_ASSERT(XLocalTime::Instance()->UTCToTZ(634917311990000000LL) == 634917023990000000LL);
#else
    {
        ScopedLocalTZ tz("America/Los_Angeles");
        UT_ASSERT(XLocalTime::Instance()->UTCToTZ(621355968000000000LL) == 621355680000000000LL);
        UT_ASSERT(XLocalTime::Instance()->UTCToTZ(631100635210000000LL) == 631100347210000000LL);
        UT_ASSERT(XLocalTime::Instance()->UTCToTZ(634917311990000000LL) == 634917023990000000LL);
    }

    {
        ScopedLocalTZ tz("America/New_York");
        UT_ASSERT(XLocalTime::Instance()->UTCToTZ(621355968000000000LL) == 621355788000000000LL);
        UT_ASSERT(XLocalTime::Instance()->UTCToTZ(631100635210000000LL) == 631100455210000000LL);
        UT_ASSERT(XLocalTime::Instance()->UTCToTZ(634917311990000000LL) == 634917131990000000LL);
    }

    {
        ScopedLocalTZ tz("UTC");
        UT_ASSERT(XLocalTime::Instance()->UTCToTZ(621355968000000000LL) == 621355968000000000LL);
        UT_ASSERT(XLocalTime::Instance()->UTCToTZ(631100635210000000LL) == 631100635210000000LL);
        UT_ASSERT(XLocalTime::Instance()->UTCToTZ(634917311990000000LL) == 634917311990000000LL);
    }

    {
        ScopedLocalTZ tz("Europe/Paris");
        UT_ASSERT(XLocalTime::Instance()->UTCToTZ(621355968000000000LL) == 621356004000000000LL);
        UT_ASSERT(XLocalTime::Instance()->UTCToTZ(631100635210000000LL) == 631100671210000000LL);
        UT_ASSERT(XLocalTime::Instance()->UTCToTZ(634917311990000000LL) == 634917347990000000LL);
    }
#endif
}


void XTimeZoneTest::TestXLocalTime_TZToUTC()
{
#ifdef WIN32
    //We need to find a way to set the time zone in Windows.
    //UT_ASSERT(XLocalTime::Instance()->TZToUTC(621355680000000000LL) == 621355968000000000LL);
    //UT_ASSERT(XLocalTime::Instance()->TZToUTC(631100347210000000LL) == 631100635210000000LL);
    //UT_ASSERT(XLocalTime::Instance()->TZToUTC(634917023990000000LL) == 634917311990000000LL);
#else
    {
        ScopedLocalTZ tz("America/Los_Angeles");
        UT_ASSERT(XLocalTime::Instance()->TZToUTC(621355680000000000LL) == 621355968000000000LL);
        UT_ASSERT(XLocalTime::Instance()->TZToUTC(631100347210000000LL) == 631100635210000000LL);
        UT_ASSERT(XLocalTime::Instance()->TZToUTC(634917023990000000LL) == 634917311990000000LL);
    }

    {
        ScopedLocalTZ tz("America/New_York");
        UT_ASSERT(XLocalTime::Instance()->TZToUTC(621355788000000000LL) == 621355968000000000LL);
        UT_ASSERT(XLocalTime::Instance()->TZToUTC(631100455210000000LL) == 631100635210000000LL);
        UT_ASSERT(XLocalTime::Instance()->TZToUTC(634917131990000000LL) == 634917311990000000LL);
    }

    {
        ScopedLocalTZ tz("UTC");
        UT_ASSERT(XLocalTime::Instance()->TZToUTC(621355968000000000LL) == 621355968000000000LL);
        UT_ASSERT(XLocalTime::Instance()->TZToUTC(631100635210000000LL) == 631100635210000000LL);
        UT_ASSERT(XLocalTime::Instance()->TZToUTC(634917311990000000LL) == 634917311990000000LL);
    }

    {
        ScopedLocalTZ tz("Europe/Paris");
        UT_ASSERT(XLocalTime::Instance()->TZToUTC(621356004000000000LL) == 621355968000000000LL);
        UT_ASSERT(XLocalTime::Instance()->TZToUTC(631100671210000000LL) == 631100635210000000LL);
        UT_ASSERT(XLocalTime::Instance()->TZToUTC(634917347990000000LL) == 634917311990000000LL);
    }
#endif
}


void XTimeZoneTest::TestXLocalTime_ToXML()
{
    XDomParser parser;

    UT_ASSERT(parser.GetXmlString(XIRef<XTimeZone>(XLocalTime::Instance())->ToXML()) == "<XLocalTime></XLocalTime>");
}


static void testHour(const XTime& xt, int hour, const XString& tzName, int line)
{
    UT_ASSERT_EQUAL_MESSAGE(XString::Format("Line# %d: [%s] [%s]",
                                                 line,
                                                 xt.ToISOExtString().c_str(),
                                                 tzName.c_str()),
                                 hour, xt.Hour());
}


static void testOffset1(const XTime& xt, const XDuration& utcOffset, bool dstInEffect, const XString& tzName, int line)
{
    UT_ASSERT_EQUAL_MESSAGE(XString::Format("Line# %d: [%s] [%s]",
                                                 line,
                                                 xt.ToISOExtString().c_str(),
                                                 tzName.c_str()),
                                 dstInEffect, xt.DSTInEffect());

    UT_ASSERT_MESSAGE(XString::Format("Line# %d: [%s] [%s]",
                                           line,
                                           xt.ToISOExtString().c_str(),
                                           tzName.c_str()),
                           xt.UTCOffset() == utcOffset);

    UT_ASSERT_MESSAGE(XString::Format("Line# %d: [%s] [%s]",
                                           line,
                                           xt.ToISOExtString().c_str(),
                                           tzName.c_str()),
                           (xt + XDuration(MINUTES, 1)).UTCOffset() == utcOffset);
}


static void testOffset2(const XTime& utc, const XTime& local, const XDuration& utcOffset, const XString& tzName, int line)
{
    UT_ASSERT_EQUAL_MESSAGE(XString::Format("Line# %d: [%s] [%s] [%s]",
                                                 line,
                                                 utc.ToISOExtString().c_str(),
                                                 local.ToISOExtString().c_str(),
                                                 tzName.c_str()),
                                 local.Hour(), (utc + utcOffset).Hour());

    UT_ASSERT_EQUAL_MESSAGE(XString::Format("Line# %d: [%s] [%s] [%s]",
                                                 line,
                                                 utc.ToISOExtString().c_str(),
                                                 local.ToISOExtString().c_str(),
                                                 tzName.c_str()),
                                 local.Hour(), (utc + utcOffset + XDuration(MINUTES, 1)).Hour());
}


static void TestDST(const XString& tzName,
                    const XDateTuple& springDate,
                    int springHour,
                    const XDateTuple& fallDate,
                    int fallHour)
{
    pair<XDateTuple, XTimeTuple> dateTime = addDuration(make_pair(springDate, XTimeTuple(springHour, 0, 0)),
                                                        XDuration(DAYS, -1));
    const XDuration stdOffset = XTime(dateTime.first, dateTime.second, XLocalTime::Instance()).UTCOffset();
    const XDuration dstOffset = stdOffset + XDuration(HOURS, 1);

    //Verify that creating an XTime in the given time zone results
    //in an XTime with the correct std time during and surrounding
    //a DST switch.
    for(int hour = -12; hour < 13; ++hour)
    {
        dateTime = addDuration(make_pair(springDate, XTimeTuple()), XDuration(HOURS, hour));
        XTime xt = XTime(dateTime.first, dateTime.second, XLocalTime::Instance());
        const int targetHour = hour < 0 ? hour + 24 : hour;

        if(hour == springHour)
        {
            testHour(xt, springHour + 1, tzName, __LINE__);
            testHour(xt + XDuration(MINUTES, 1), springHour + 1, tzName, __LINE__);
        }
        else
        {
            testHour(xt, targetHour, tzName, __LINE__);
            testHour(xt + XDuration(MINUTES, 1), targetHour, tzName, __LINE__);
        }

        if(hour < springHour)
            testOffset1(xt, stdOffset, false, tzName, __LINE__);
        else
            testOffset1(xt, dstOffset, true, tzName, __LINE__);

        dateTime = addDuration(make_pair(fallDate, XTimeTuple()), XDuration(HOURS, hour));
        xt = XTime(dateTime.first, dateTime.second, XLocalTime::Instance());
        testHour(xt, targetHour, tzName, __LINE__);

        //Verify that 01:00 is the first 01:00 (or whatever hour before the switch is).
        if(hour == fallHour - 1)
            testHour(xt + XDuration(HOURS, 1), targetHour, tzName, __LINE__);

        if(hour < fallHour)
            testOffset1(xt, dstOffset, true, tzName, __LINE__);
        else
            testOffset1(xt, stdOffset, false, tzName, __LINE__);
    }

    //Verify that converting a time in UTC to a time in another
    //time zone results in the correct time during and surrounding
    //a DST switch.
    bool first = true;
    const XTime springSwitch = XTime(springDate, XTimeTuple(springHour, 0 , 0), XUTC::Instance()) - stdOffset;
    const XTime fallSwitch = XTime(fallDate, XTimeTuple(fallHour, 0 , 0), XUTC::Instance()) - dstOffset;

    for(int hour = -24; hour < 25; ++hour)
    {
        dateTime = addDuration(make_pair(springDate, XTimeTuple(springHour, 0, 0)), XDuration(HOURS, hour));
        XTime utc = XTime(dateTime.first, dateTime.second, XUTC::Instance());
        XTime local = utc.ToLocalTime();

        if(utc < springSwitch)
            testOffset2(utc, local, stdOffset, tzName, __LINE__);
        else
            testOffset2(utc, local, dstOffset, tzName, __LINE__);

        dateTime = addDuration(make_pair(fallDate, XTimeTuple(fallHour, 0, 0)), XDuration(HOURS, hour));
        utc = XTime(dateTime.first, dateTime.second, XUTC::Instance());
        local = utc.ToLocalTime();

        if(utc == fallSwitch || utc == fallSwitch - XDuration(HOURS, 1))
        {
            if(first)
            {
                testOffset2(utc, local, dstOffset, tzName, __LINE__);
                first = false;
            }
            else
                testOffset2(utc, local, stdOffset, tzName, __LINE__);
        }
        else if(utc > fallSwitch)
            testOffset2(utc, local, stdOffset, tzName, __LINE__);
        else
            testOffset2(utc, local, dstOffset, tzName, __LINE__);
    }
}


void XTimeZoneTest::TestDSTSwitches()
{
#ifdef IS_LINUX
    {
        ScopedLocalTZ tz("America/Los_Angeles");
        TestDST("America/Los_Angeles",
                XDateTuple(2012, 3, 11),
                2,
                XDateTuple(2012, 11, 4),
                2);
    }

    {
        ScopedLocalTZ tz("America/New_York");
        TestDST("America/New_York",
                XDateTuple(2012, 3, 11),
                2,
                XDateTuple(2012, 11, 4),
                2);
    }

    {
        ScopedLocalTZ tz("Europe/London");
        TestDST("Europe/London",
                XDateTuple(2012, 3, 25),
                1,
                XDateTuple(2012, 10, 28),
                2);
    }

    {
        ScopedLocalTZ tz("Europe/Paris");
        TestDST("Europe/Paris",
                XDateTuple(2012, 3, 25),
                2,
                XDateTuple(2012, 10, 28),
                3);
    }

    {
        ScopedLocalTZ tz("Australia/Adelaide");
        TestDST("Australia/Adelaide",
                XDateTuple(2012, 10, 7),
                2,
                XDateTuple(2012, 4, 1),
                3);
    }

#elif defined(IS_WINDOWS)
    const XDuration utcOffset = XLocalTime::Instance()->UTCOffsetAt(0);
    XString tzName;

    //Because I haven't yet figured out how to set the time zone in
    //Windows for testing.
    if(utcOffset == XDuration(HOURS, -8))
        tzName = "Pacific Standard Time";
    else if(utcOffset == XDuration(HOURS, -7))
        tzName = "Mountain Standard Time";
    else if(utcOffset == XDuration(HOURS, -6))
        tzName = "Central Standard Time";
    else if(utcOffset == XDuration(HOURS, -5))
        tzName = "Mountain Standard Time";
    else
        UT_FAIL("Unknown Timezone");

    TestDST(tzName,
            XDateTuple(2012, 3, 11),
            2,
            XDateTuple(2012, 11, 4),
            2);
#else
    #error ">> Unknown OS!"
#endif
}
