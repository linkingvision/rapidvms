
#include "XTimeTest.h"

#include <vector>
#include "XSDK/XDuration.h"
#include "XSDK/XTime.h"

using namespace std;
using namespace XSDK;

REGISTER_TEST_FIXTURE(XTimeTest);

#ifndef WIN32
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

void XTimeTest::TestConstructor()
{
#ifndef WIN32
    ScopedLocalTZ tz("America/Los_Angeles");
#endif

    //XTime(int64_t ticks, XIRef<XTimeZone> tz)
    UT_ASSERT(XTime(0, XUTC::Instance())._ticks == 0);
    UT_ASSERT(XTime(42, XUTC::Instance())._ticks == 42);
    UT_ASSERT(XTime(42, XUTC::Instance())._ticks == 42);
    UT_ASSERT(XTime(864000000000LL, XUTC::Instance())._ticks == 864000000000LL);

    UT_ASSERT(XTime(0, new XSimpleTimeZone(-8 * 60, 0))._ticks == 0);
    UT_ASSERT(XTime(42, new XSimpleTimeZone(-8 * 60, 60))._ticks == 42);
    UT_ASSERT(XTime(42, new XSimpleTimeZone(10 * 60, 32))._ticks == 42);
    UT_ASSERT(XTime(864000000000LL, new XSimpleTimeZone(2 * 60, 45))._ticks == 864000000000LL);

    //XTime(const XDateTuple& date, const XTimeTuple& time, XIRef<XTimeZone> tz = XUTC::Instance())
    UT_ASSERT(XTime(XDateTuple(), XTimeTuple(), XUTC::Instance())._ticks == 0);
    UT_ASSERT(XTime(XDateTuple(1, 1, 1), XTimeTuple(12, 30, 33), XUTC::Instance())._ticks == 450330000000LL);
    UT_ASSERT(XTime(XDateTuple(1, 1, 1), XTimeTuple(0, 0, 0), XUTC::Instance())._ticks == 0);
    UT_ASSERT(XTime(XDateTuple(1, 1, 1), XTimeTuple(0, 0, 1), XUTC::Instance())._ticks == 10000000LL);

    UT_ASSERT(XTime(XDateTuple(1, 1, 1), XTimeTuple(0, 0, 0), new XSimpleTimeZone(-60, 0))._ticks == 36000000000LL);
    UT_ASSERT(XTime(XDateTuple(1, 1, 1), XTimeTuple(0, 0, 0), new XSimpleTimeZone(0, 0))._ticks == 0);

    UT_ASSERT(XTime(XDateTuple(1995, 7, 2), XTimeTuple(2, 4, 59), XUTC::Instance())._ticks == 629402474990000000LL);
    UT_ASSERT(XTime(XDateTuple(2000, 2, 28), XTimeTuple(7, 7, 7), XUTC::Instance())._ticks == 630873184270000000LL);
    UT_ASSERT(XTime(XDateTuple(2000, 2, 29), XTimeTuple(23, 0, 16), XUTC::Instance())._ticks == 630874620160000000LL);
    UT_ASSERT(XTime(XDateTuple(2000, 3, 1), XTimeTuple(1, 59, 2), XUTC::Instance())._ticks == 630874727420000000LL);
    UT_ASSERT(XTime(XDateTuple(2001, 2, 28), XTimeTuple(22, 1, 22), XUTC::Instance())._ticks == 631189944820000000LL);
    UT_ASSERT(XTime(XDateTuple(2001, 3, 1), XTimeTuple(1, 1, 1), XUTC::Instance())._ticks == 631190052610000000LL);
    UT_ASSERT(XTime(XDateTuple(2011, 3, 7), XTimeTuple(17, 57, 19), XUTC::Instance())._ticks == 634351174390000000LL);
    UT_ASSERT(XTime(XDateTuple(2040, 7, 4), XTimeTuple(12, 12, 12), XUTC::Instance())._ticks == 643606135320000000LL);

    UT_ASSERT(XTime(XDateTuple(1995, 7, 2), XTimeTuple(2, 4, 59), new XSimpleTimeZone(-360, 0))._ticks == 629402690990000000LL);
    UT_ASSERT(XTime(XDateTuple(2000, 2, 28), XTimeTuple(7, 7, 7), new XSimpleTimeZone(-360, 0))._ticks == 630873400270000000LL);
    UT_ASSERT(XTime(XDateTuple(2000, 2, 29), XTimeTuple(23, 0, 16), new XSimpleTimeZone(-360, 0))._ticks == 630874836160000000LL);
    UT_ASSERT(XTime(XDateTuple(2000, 3, 1), XTimeTuple(1, 59, 2), new XSimpleTimeZone(-360, 0))._ticks == 630874943420000000LL);
    UT_ASSERT(XTime(XDateTuple(2001, 2, 28), XTimeTuple(22, 1, 22), new XSimpleTimeZone(-360, 0))._ticks == 631190160820000000LL);
    UT_ASSERT(XTime(XDateTuple(2001, 3, 1), XTimeTuple(1, 1, 1), new XSimpleTimeZone(-360, 0))._ticks == 631190268610000000LL);
    UT_ASSERT(XTime(XDateTuple(2011, 3, 7), XTimeTuple(17, 57, 19), new XSimpleTimeZone(-360, 0))._ticks == 634351390390000000LL);
    UT_ASSERT(XTime(XDateTuple(2040, 7, 4), XTimeTuple(12, 12, 12), new XSimpleTimeZone(-360, 0))._ticks == 643606351320000000LL);

    UT_ASSERT(XTime(XDateTuple(1995, 7, 2), XTimeTuple(2, 4, 59), new XSimpleTimeZone(-390, 30))._ticks == 629402690990000000LL);
    UT_ASSERT(XTime(XDateTuple(2000, 2, 28), XTimeTuple(7, 7, 7), new XSimpleTimeZone(-390, 30))._ticks == 630873400270000000LL);
    UT_ASSERT(XTime(XDateTuple(2000, 2, 29), XTimeTuple(23, 0, 16), new XSimpleTimeZone(-390, 30))._ticks == 630874836160000000LL);
    UT_ASSERT(XTime(XDateTuple(2000, 3, 1), XTimeTuple(1, 59, 2), new XSimpleTimeZone(-390, 30))._ticks == 630874943420000000LL);
    UT_ASSERT(XTime(XDateTuple(2001, 2, 28), XTimeTuple(22, 1, 22), new XSimpleTimeZone(-390, 30))._ticks == 631190160820000000LL);
    UT_ASSERT(XTime(XDateTuple(2001, 3, 1), XTimeTuple(1, 1, 1), new XSimpleTimeZone(-390, 30))._ticks == 631190268610000000LL);
    UT_ASSERT(XTime(XDateTuple(2011, 3, 7), XTimeTuple(17, 57, 19), new XSimpleTimeZone(-390, 30))._ticks == 634351390390000000LL);
    UT_ASSERT(XTime(XDateTuple(2040, 7, 4), XTimeTuple(12, 12, 12), new XSimpleTimeZone(-390, 30))._ticks == 643606351320000000LL);

    UT_ASSERT(XTime(XDateTuple(1995, 7, 2), XTimeTuple(2, 4, 59), new XSimpleTimeZone(360, 0))._ticks == 629402258990000000LL);
    UT_ASSERT(XTime(XDateTuple(2000, 2, 28), XTimeTuple(7, 7, 7), new XSimpleTimeZone(360, 0))._ticks == 630872968270000000LL);
    UT_ASSERT(XTime(XDateTuple(2000, 2, 29), XTimeTuple(23, 0, 16), new XSimpleTimeZone(360, 0))._ticks == 630874404160000000LL);
    UT_ASSERT(XTime(XDateTuple(2000, 3, 1), XTimeTuple(1, 59, 2), new XSimpleTimeZone(360, 0))._ticks == 630874511420000000LL);
    UT_ASSERT(XTime(XDateTuple(2001, 2, 28), XTimeTuple(22, 1, 22), new XSimpleTimeZone(360, 0))._ticks == 631189728820000000LL);
    UT_ASSERT(XTime(XDateTuple(2001, 3, 1), XTimeTuple(1, 1, 1), new XSimpleTimeZone(360, 0))._ticks == 631189836610000000LL);
    UT_ASSERT(XTime(XDateTuple(2011, 3, 7), XTimeTuple(17, 57, 19), new XSimpleTimeZone(360, 0))._ticks == 634350958390000000LL);
    UT_ASSERT(XTime(XDateTuple(2040, 7, 4), XTimeTuple(12, 12, 12), new XSimpleTimeZone(360, 0))._ticks == 643605919320000000LL);

    UT_ASSERT(XTime(XDateTuple(1995, 7, 2), XTimeTuple(2, 4, 59), new XSimpleTimeZone(300, 60))._ticks == 629402258990000000LL);
    UT_ASSERT(XTime(XDateTuple(2000, 2, 28), XTimeTuple(7, 7, 7), new XSimpleTimeZone(300, 60))._ticks == 630872968270000000LL);
    UT_ASSERT(XTime(XDateTuple(2000, 2, 29), XTimeTuple(23, 0, 16), new XSimpleTimeZone(300, 60))._ticks == 630874404160000000LL);
    UT_ASSERT(XTime(XDateTuple(2000, 3, 1), XTimeTuple(1, 59, 2), new XSimpleTimeZone(300, 60))._ticks == 630874511420000000LL);
    UT_ASSERT(XTime(XDateTuple(2001, 2, 28), XTimeTuple(22, 1, 22), new XSimpleTimeZone(300, 60))._ticks == 631189728820000000LL);
    UT_ASSERT(XTime(XDateTuple(2001, 3, 1), XTimeTuple(1, 1, 1), new XSimpleTimeZone(300, 60))._ticks == 631189836610000000LL);
    UT_ASSERT(XTime(XDateTuple(2011, 3, 7), XTimeTuple(17, 57, 19), new XSimpleTimeZone(300, 60))._ticks == 634350958390000000LL);
    UT_ASSERT(XTime(XDateTuple(2040, 7, 4), XTimeTuple(12, 12, 12), new XSimpleTimeZone(300, 60))._ticks == 643605919320000000LL);

#ifdef IS_LINUX
    //Unfortunately, these are specific to America/Los_Angeles, and I can't find a way to
    //temporarily set the local time zone on Windows without affecting the whole box, so
    //for now, this is Linux-specific.
    UT_ASSERT(XTime(XDateTuple(1970, 1, 1), XTimeTuple(0, 0, 0))._ticks == 621356256000000000LL);
    UT_ASSERT(XTime(XDateTuple(2011, 3, 16), XTimeTuple(14, 19, 22))._ticks == 634359071620000000LL);
    UT_ASSERT(XTime(XDateTuple(2012, 12, 21), XTimeTuple(3, 2, 1))._ticks == 634916845210000000LL);
#endif

    UT_ASSERT_THROWS(XTime(XDateTuple(0, 1, 1), XTimeTuple()), XException);
    UT_ASSERT_THROWS(XTime(XDateTuple(-1, 1, 1), XTimeTuple()), XException);

    //XTime(const XDateTuple& date, const XTimeTuple& time, const XFracSec& fracSec, XIRef<XTimeZone> tz = XUTC::Instance())
    UT_ASSERT(XTime(XDateTuple(), XTimeTuple(), XFracSec::Zero(), XUTC::Instance())._ticks == 0);
    UT_ASSERT(XTime(XDateTuple(1, 1, 1), XTimeTuple(12, 30, 33), XFracSec(HNSECS, 42), XUTC::Instance())._ticks == 450330000042LL);
    UT_ASSERT(XTime(XDateTuple(1, 1, 1), XTimeTuple(0, 0, 0), XFracSec(HNSECS, 42), XUTC::Instance())._ticks == 42);
    UT_ASSERT(XTime(XDateTuple(1, 1, 1), XTimeTuple(0, 0, 1), XFracSec(HNSECS, 42), XUTC::Instance())._ticks == 10000042LL);

    UT_ASSERT(XTime(XDateTuple(1, 1, 1), XTimeTuple(0, 0, 0), XFracSec(HNSECS, 42), new XSimpleTimeZone(-60, 0))._ticks == 36000000042LL);
    UT_ASSERT(XTime(XDateTuple(1, 1, 1), XTimeTuple(0, 0, 0), XFracSec(HNSECS, 42), new XSimpleTimeZone(0, 0))._ticks == 42);

    UT_ASSERT(XTime(XDateTuple(1995, 7, 2), XTimeTuple(2, 4, 59), XFracSec(HNSECS, 17), XUTC::Instance())._ticks == 629402474990000017LL);
    UT_ASSERT(XTime(XDateTuple(2000, 2, 28), XTimeTuple(7, 7, 7), XFracSec(HNSECS, 17), XUTC::Instance())._ticks == 630873184270000017LL);
    UT_ASSERT(XTime(XDateTuple(2000, 2, 29), XTimeTuple(23, 0, 16), XFracSec(HNSECS, 17), XUTC::Instance())._ticks == 630874620160000017LL);
    UT_ASSERT(XTime(XDateTuple(2000, 3, 1), XTimeTuple(1, 59, 2), XFracSec(HNSECS, 17), XUTC::Instance())._ticks == 630874727420000017LL);
    UT_ASSERT(XTime(XDateTuple(2001, 2, 28), XTimeTuple(22, 1, 22), XFracSec(HNSECS, 17), XUTC::Instance())._ticks == 631189944820000017LL);
    UT_ASSERT(XTime(XDateTuple(2001, 3, 1), XTimeTuple(1, 1, 1), XFracSec(HNSECS, 17), XUTC::Instance())._ticks == 631190052610000017LL);
    UT_ASSERT(XTime(XDateTuple(2011, 3, 7), XTimeTuple(17, 57, 19), XFracSec(HNSECS, 17), XUTC::Instance())._ticks == 634351174390000017LL);
    UT_ASSERT(XTime(XDateTuple(2040, 7, 4), XTimeTuple(12, 12, 12), XFracSec(HNSECS, 17), XUTC::Instance())._ticks == 643606135320000017LL);

    UT_ASSERT(XTime(XDateTuple(1995, 7, 2), XTimeTuple(2, 4, 59), XFracSec(HNSECS, 17), new XSimpleTimeZone(-390, 30))._ticks == 629402690990000017LL);
    UT_ASSERT(XTime(XDateTuple(2000, 2, 28), XTimeTuple(7, 7, 7), XFracSec(HNSECS, 17), new XSimpleTimeZone(-390, 30))._ticks == 630873400270000017LL);
    UT_ASSERT(XTime(XDateTuple(2000, 2, 29), XTimeTuple(23, 0, 16), XFracSec(HNSECS, 17), new XSimpleTimeZone(-390, 30))._ticks == 630874836160000017LL);
    UT_ASSERT(XTime(XDateTuple(2000, 3, 1), XTimeTuple(1, 59, 2), XFracSec(HNSECS, 17), new XSimpleTimeZone(-390, 30))._ticks == 630874943420000017LL);
    UT_ASSERT(XTime(XDateTuple(2001, 2, 28), XTimeTuple(22, 1, 22), XFracSec(HNSECS, 17), new XSimpleTimeZone(-390, 30))._ticks == 631190160820000017LL);
    UT_ASSERT(XTime(XDateTuple(2001, 3, 1), XTimeTuple(1, 1, 1), XFracSec(HNSECS, 17), new XSimpleTimeZone(-390, 30))._ticks == 631190268610000017LL);
    UT_ASSERT(XTime(XDateTuple(2011, 3, 7), XTimeTuple(17, 57, 19), XFracSec(HNSECS, 17), new XSimpleTimeZone(-390, 30))._ticks == 634351390390000017LL);
    UT_ASSERT(XTime(XDateTuple(2040, 7, 4), XTimeTuple(12, 12, 12), XFracSec(HNSECS, 17), new XSimpleTimeZone(-390, 30))._ticks == 643606351320000017LL);

    UT_ASSERT(XTime(XDateTuple(1995, 7, 2), XTimeTuple(2, 4, 59), XFracSec(HNSECS, 17), new XSimpleTimeZone(300, 60))._ticks == 629402258990000017LL);
    UT_ASSERT(XTime(XDateTuple(2000, 2, 28), XTimeTuple(7, 7, 7), XFracSec(HNSECS, 17), new XSimpleTimeZone(300, 60))._ticks == 630872968270000017LL);
    UT_ASSERT(XTime(XDateTuple(2000, 2, 29), XTimeTuple(23, 0, 16), XFracSec(HNSECS, 17), new XSimpleTimeZone(300, 60))._ticks == 630874404160000017LL);
    UT_ASSERT(XTime(XDateTuple(2000, 3, 1), XTimeTuple(1, 59, 2), XFracSec(HNSECS, 17), new XSimpleTimeZone(300, 60))._ticks == 630874511420000017LL);
    UT_ASSERT(XTime(XDateTuple(2001, 2, 28), XTimeTuple(22, 1, 22), XFracSec(HNSECS, 17), new XSimpleTimeZone(300, 60))._ticks == 631189728820000017LL);
    UT_ASSERT(XTime(XDateTuple(2001, 3, 1), XTimeTuple(1, 1, 1), XFracSec(HNSECS, 17), new XSimpleTimeZone(300, 60))._ticks == 631189836610000017LL);
    UT_ASSERT(XTime(XDateTuple(2011, 3, 7), XTimeTuple(17, 57, 19), XFracSec(HNSECS, 17), new XSimpleTimeZone(300, 60))._ticks == 634350958390000017LL);
    UT_ASSERT(XTime(XDateTuple(2040, 7, 4), XTimeTuple(12, 12, 12), XFracSec(HNSECS, 17), new XSimpleTimeZone(300, 60))._ticks == 643605919320000017LL);

    UT_ASSERT_THROWS(XTime(XDateTuple(), XTimeTuple(), XFracSec(HNSECS, -1)), XException);
    UT_ASSERT_THROWS(XTime(XDateTuple(), XTimeTuple(), XFracSec(HNSECS, -9999999)), XException);

#ifdef IS_LINUX
    //Unfortunately, these are specific to America/Los_Angeles, and I can't find a way to
    //temporarily set the local time zone on Windows without affecting the whole box, so
    //for now, this is Linux-specific.
    UT_ASSERT(XTime(XDateTuple(1970, 1, 1), XTimeTuple(0, 0, 0), XFracSec::Zero())._ticks == 621356256000000000LL);
    UT_ASSERT(XTime(XDateTuple(2011, 3, 16), XTimeTuple(14, 19, 22), XFracSec(USECS, 45))._ticks == 634359071620000450LL);
    UT_ASSERT(XTime(XDateTuple(2012, 12, 21), XTimeTuple(3, 2, 1), XFracSec(HNSECS, 12))._ticks == 634916845210000012LL);
#endif

    UT_ASSERT_THROWS(XTime(XDateTuple(0, 1, 1), XTimeTuple(), XFracSec(HNSECS, 17)), XException);
    UT_ASSERT_THROWS(XTime(XDateTuple(-1, 1, 1), XTimeTuple(), XFracSec(HNSECS, 17)), XException);

    //XTime(time_t unixTime, XFracSec fracSec, XIRef<XTimeZone> tz)
    UT_ASSERT(XTime(0, XFracSec::Zero(), XUTC::Instance())._ticks == 621355968000000000LL);
    UT_ASSERT(XTime(86400, XFracSec::Zero(), XUTC::Instance())._ticks == 621355968000000000LL + 864000000000LL);
    UT_ASSERT(XTime(0, XFracSec(MSECS, 17), XUTC::Instance())._ticks == 621355968000170000LL);
    UT_ASSERT(XTime(86400, XFracSec(USECS, 5002), XUTC::Instance())._ticks == 621355968000000000LL + 864000050020LL);

    UT_ASSERT(XTime(0, XFracSec::Zero(), new XSimpleTimeZone(-8 * 60, 0))._ticks == 621355968000000000LL);
    UT_ASSERT(XTime(0, XFracSec::Zero(), new XSimpleTimeZone(-8 * 60, 60))._ticks == 621355968000000000LL);
    UT_ASSERT(XTime(0, XFracSec::Zero(), new XSimpleTimeZone(5 * 60, 22))._ticks == 621355968000000000LL);
    UT_ASSERT(XTime(86400, XFracSec::Zero(), new XSimpleTimeZone(-8 * 60, 0))._ticks == 621355968000000000LL + 864000000000LL);
    UT_ASSERT(XTime(86400, XFracSec::Zero(), new XSimpleTimeZone(-8 * 60, 60))._ticks == 621355968000000000LL + 864000000000LL);
    UT_ASSERT(XTime(86400, XFracSec::Zero(), new XSimpleTimeZone(10 * 60, 22))._ticks == 621355968000000000LL + 864000000000LL);
    UT_ASSERT(XTime(86477, XFracSec::Zero(), new XSimpleTimeZone(-8 * 60, 0))._ticks == 621355968000000000LL + 864770000000LL);
    UT_ASSERT(XTime(86477, XFracSec::Zero(), new XSimpleTimeZone(-8 * 60, 60))._ticks == 621355968000000000LL + 864770000000LL);
    UT_ASSERT(XTime(86477, XFracSec::Zero(), new XSimpleTimeZone(10 * 60, 22))._ticks == 621355968000000000LL + 864770000000LL);
    UT_ASSERT(XTime(0, XFracSec(MSECS, 17), new XSimpleTimeZone(2 * 60, 60))._ticks == 621355968000170000LL);
    UT_ASSERT(XTime(0, XFracSec(MSECS, 17), new XSimpleTimeZone(-2 * 60, 12))._ticks == 621355968000170000LL);
    UT_ASSERT(XTime(0, XFracSec(MSECS, 17), new XSimpleTimeZone(-12 * 60, 60))._ticks == 621355968000170000LL);
    UT_ASSERT(XTime(86400, XFracSec(USECS, 5002), new XSimpleTimeZone(7 * 60, 0))._ticks == 621355968000000000LL + 864000050020LL);
    UT_ASSERT(XTime(86400, XFracSec(USECS, 5002), new XSimpleTimeZone(7 * 60, 60))._ticks == 621355968000000000LL + 864000050020LL);
    UT_ASSERT(XTime(86477, XFracSec(USECS, 5002), new XSimpleTimeZone(7 * 60, 0))._ticks == 621355968000000000LL + 864770050020LL);
    UT_ASSERT(XTime(86477, XFracSec(USECS, 5002), new XSimpleTimeZone(7 * 60, 60))._ticks == 621355968000000000LL + 864770050020LL);

    UT_ASSERT_THROWS(XTime(86477, XFracSec(HNSECS, -1)), XException);
    UT_ASSERT_THROWS(XTime(86477, XFracSec(HNSECS, -9999999)), XException);

    //create(int64_t unixTimeAsMSecs, XIRef<XTimeZone> tz)
    UT_ASSERT(XTime::CreateFromUnixTimeAsMSecs(0, XUTC::Instance())._ticks == 621355968000000000LL);
    UT_ASSERT(XTime::CreateFromUnixTimeAsMSecs(86400000, XUTC::Instance())._ticks == 621355968000000000LL + 864000000000LL);
    UT_ASSERT(XTime::CreateFromUnixTimeAsMSecs(86477123, XUTC::Instance())._ticks == 621355968000000000LL + 864771230000LL);
    UT_ASSERT(XTime::CreateFromUnixTimeAsMSecs(17, XUTC::Instance())._ticks == 621355968000170000LL);
    UT_ASSERT(XTime::CreateFromUnixTimeAsMSecs(86477005, XUTC::Instance())._ticks == 621355968000000000LL + 864770050000LL);

    UT_ASSERT(XTime::CreateFromUnixTimeAsMSecs(0, new XSimpleTimeZone(-8 * 60, 0))._ticks == 621355968000000000LL);
    UT_ASSERT(XTime::CreateFromUnixTimeAsMSecs(0, new XSimpleTimeZone(-8 * 60, 60))._ticks == 621355968000000000LL);
    UT_ASSERT(XTime::CreateFromUnixTimeAsMSecs(0, new XSimpleTimeZone(5 * 60, 22))._ticks == 621355968000000000LL);
    UT_ASSERT(XTime::CreateFromUnixTimeAsMSecs(86400000, new XSimpleTimeZone(-8 * 60, 0))._ticks == 621355968000000000LL + 864000000000LL);
    UT_ASSERT(XTime::CreateFromUnixTimeAsMSecs(86400000, new XSimpleTimeZone(-8 * 60, 60))._ticks == 621355968000000000LL + 864000000000LL);
    UT_ASSERT(XTime::CreateFromUnixTimeAsMSecs(86400000, new XSimpleTimeZone(10 * 60, 22))._ticks == 621355968000000000LL + 864000000000LL);
    UT_ASSERT(XTime::CreateFromUnixTimeAsMSecs(86477123, new XSimpleTimeZone(-8 * 60, 0))._ticks == 621355968000000000LL + 864771230000LL);
    UT_ASSERT(XTime::CreateFromUnixTimeAsMSecs(86477123, new XSimpleTimeZone(-8 * 60, 60))._ticks == 621355968000000000LL + 864771230000LL);
    UT_ASSERT(XTime::CreateFromUnixTimeAsMSecs(86477123, new XSimpleTimeZone(10 * 60, 22))._ticks == 621355968000000000LL + 864771230000LL);
    UT_ASSERT(XTime::CreateFromUnixTimeAsMSecs(17, new XSimpleTimeZone(2 * 60, 60))._ticks == 621355968000170000LL);
    UT_ASSERT(XTime::CreateFromUnixTimeAsMSecs(17, new XSimpleTimeZone(-2 * 60, 12))._ticks == 621355968000170000LL);
    UT_ASSERT(XTime::CreateFromUnixTimeAsMSecs(17, new XSimpleTimeZone(-12 * 60, 60))._ticks == 621355968000170000LL);
    UT_ASSERT(XTime::CreateFromUnixTimeAsMSecs(86400005, new XSimpleTimeZone(7 * 60, 0))._ticks == 621355968000000000LL + 864000050000LL);
    UT_ASSERT(XTime::CreateFromUnixTimeAsMSecs(86400005, new XSimpleTimeZone(7 * 60, 60))._ticks == 621355968000000000LL + 864000050000LL);
}


void XTimeTest::TestEquality()
{
    XTime utc0(0, XUTC::Instance());
    XTime simple0(0, new XSimpleTimeZone(-7 * 60, 0));
    XTime utc42(42, XUTC::Instance());
    XTime simple42(42, new XSimpleTimeZone(-8 * 60, 60));

    UT_ASSERT(utc0 == simple0);
    UT_ASSERT(!(utc0 != simple0));
    UT_ASSERT(!(utc0 < simple0));
    UT_ASSERT(utc0 <= simple0);
    UT_ASSERT(!(utc0 > simple0));
    UT_ASSERT(utc0 >= simple0);

    UT_ASSERT(simple0 == utc0);
    UT_ASSERT(!(simple0 != utc0));
    UT_ASSERT(!(simple0 < utc0));
    UT_ASSERT(simple0 <= utc0);
    UT_ASSERT(!(simple0 > utc0));
    UT_ASSERT(simple0 >= utc0);

    UT_ASSERT(utc42 == simple42);
    UT_ASSERT(!(utc42 != simple42));
    UT_ASSERT(!(utc42 < simple42));
    UT_ASSERT(utc42 <= simple42);
    UT_ASSERT(!(utc42 > simple42));
    UT_ASSERT(utc42 >= simple42);

    UT_ASSERT(simple42 == utc42);
    UT_ASSERT(!(simple42 != utc42));
    UT_ASSERT(!(simple42 < utc42));
    UT_ASSERT(simple42 <= utc42);
    UT_ASSERT(!(simple42 > utc42));
    UT_ASSERT(simple42 >= utc42);

    UT_ASSERT(!(utc0 == simple42));
    UT_ASSERT(utc0 != simple42);
    UT_ASSERT(utc0 < simple42);
    UT_ASSERT(utc0 <= simple42);
    UT_ASSERT(!(utc0 > simple42));
    UT_ASSERT(!(utc0 >= simple42));

    UT_ASSERT(!(simple42 == utc0));
    UT_ASSERT(simple42 != utc0);
    UT_ASSERT(!(simple42 < utc0));
    UT_ASSERT(!(simple42 <= utc0));
    UT_ASSERT(simple42 > utc0);
    UT_ASSERT(simple42 >= utc0);
}


void XTimeTest::TestAddSubXTime()
{
    XTime utc0(0, XUTC::Instance());
    XTime simple0(0, new XSimpleTimeZone(-7 * 60, 0));
    XTime utc42(42, XUTC::Instance());
    XTime simple42(42, new XSimpleTimeZone(-8 * 60, 60));

    UT_ASSERT(utc0 - utc0 ==  XDuration::Zero());
    UT_ASSERT(utc0 - simple0 ==  XDuration::Zero());
    UT_ASSERT(simple0 - utc0 ==  XDuration::Zero());
    UT_ASSERT(simple0 - simple0 ==  XDuration::Zero());

    UT_ASSERT(utc42 - utc42 ==  XDuration::Zero());
    UT_ASSERT(utc42 - simple42 ==  XDuration::Zero());
    UT_ASSERT(simple42 - utc42 ==  XDuration::Zero());
    UT_ASSERT(simple42 - simple42 ==  XDuration::Zero());

    UT_ASSERT(utc0 - utc42 ==  XDuration(HNSECS, -42));
    UT_ASSERT(utc0 - simple42 ==  XDuration(HNSECS, -42));
    UT_ASSERT(simple0 - utc42 ==  XDuration(HNSECS, -42));
    UT_ASSERT(simple0 - simple42 ==  XDuration(HNSECS, -42));

    UT_ASSERT(utc42 - utc0 ==  XDuration(HNSECS, 42));
    UT_ASSERT(utc42 - simple0 ==  XDuration(HNSECS, 42));
    UT_ASSERT(simple42 - utc0 ==  XDuration(HNSECS, 42));
    UT_ASSERT(simple42 - simple0 ==  XDuration(HNSECS, 42));
}


void XTimeTest::TestAddSubXDuration()
{
    UT_ASSERT(XTime(5007, XUTC::Instance()) + XDuration::Zero() == XTime(5007, XUTC::Instance()));
    UT_ASSERT(XTime(5007, XUTC::Instance()) - XDuration::Zero() == XTime(5007, XUTC::Instance()));
    UT_ASSERT(XTime(5007, XUTC::Instance()) + XDuration(HNSECS, 42) == XTime(5049, XUTC::Instance()));
    UT_ASSERT(XTime(5007, XUTC::Instance()) - XDuration(HNSECS, 42) == XTime(4965, XUTC::Instance()));
    UT_ASSERT(XTime(5007, XUTC::Instance()) + XDuration::Zero() == XTime(5007, XUTC::Instance()));

    UT_ASSERT((XTime(5007, XUTC::Instance()) + XDuration::Zero()).TimeZone().Get() == XUTC::Instance().Get());
    UT_ASSERT((XTime(5007, XUTC::Instance()) - XDuration::Zero()).TimeZone().Get() == XUTC::Instance().Get());

    XIRef<XTimeZone> tz = new XSimpleTimeZone(-8 * 60, 0);
    UT_ASSERT((XTime(5007, tz) + XDuration::Zero()).TimeZone().Get() == tz.Get());
    UT_ASSERT((XTime(5007, tz) - XDuration::Zero()).TimeZone().Get() == tz.Get());
}


void XTimeTest::TestAddSubAssign()
{
    UT_ASSERT((XTime(5007, XUTC::Instance()) += XDuration::Zero()) == XTime(5007, XUTC::Instance()));
    UT_ASSERT((XTime(5007, XUTC::Instance()) -= XDuration::Zero()) == XTime(5007, XUTC::Instance()));
    UT_ASSERT((XTime(5007, XUTC::Instance()) += XDuration(HNSECS, 42)) == XTime(5049, XUTC::Instance()));
    UT_ASSERT((XTime(5007, XUTC::Instance()) -= XDuration(HNSECS, 42)) == XTime(4965, XUTC::Instance()));
    UT_ASSERT((XTime(5007, XUTC::Instance()) += XDuration::Zero()) == XTime(5007, XUTC::Instance()));

    UT_ASSERT((XTime(5007, XUTC::Instance()) += XDuration::Zero()).TimeZone().Get() == XUTC::Instance().Get());
    UT_ASSERT((XTime(5007, XUTC::Instance()) -= XDuration::Zero()).TimeZone().Get() == XUTC::Instance().Get());

    XIRef<XTimeZone> tz = new XSimpleTimeZone(-8 * 60, 0);
    UT_ASSERT((XTime(5007, tz) += XDuration::Zero()).TimeZone().Get() == tz.Get());
    UT_ASSERT((XTime(5007, tz) -= XDuration::Zero()).TimeZone().Get() == tz.Get());

    {
        XTime xt(2121212, XUTC::Instance());
        xt += XDuration(HNSECS, 57);
        UT_ASSERT(xt == XTime(2121269, XUTC::Instance()));
        xt -= XDuration(HNSECS, 77);
        UT_ASSERT(xt == XTime(2121192, XUTC::Instance()));
        UT_ASSERT(xt.TimeZone().Get() == XUTC::Instance().Get());
    }

    {
        XTime xt(2121212, tz);
        xt += XDuration(HNSECS, 57);
        UT_ASSERT(xt == XTime(2121269, tz));
        xt -= XDuration(HNSECS, 77);
        UT_ASSERT(xt == XTime(2121192, tz));
        UT_ASSERT(xt.TimeZone().Get() == tz.Get());
    }
}

void XTimeTest::TestDayOfGregorianCal()
{
    UT_ASSERT(XTime(XDateTuple(1, 1, 1), XTimeTuple(0, 0, 0), XFracSec::Zero(), XUTC::Instance()).DayOfGregorianCal() == 1);
    UT_ASSERT(XTime(XDateTuple(1, 1, 1), XTimeTuple(0, 0, 0), XFracSec(HNSECS, 1), XUTC::Instance()).DayOfGregorianCal() == 1);
    UT_ASSERT(XTime(XDateTuple(1, 1, 1), XTimeTuple(23, 59, 59), XFracSec(HNSECS, 9999999), XUTC::Instance()).DayOfGregorianCal() == 1);
    UT_ASSERT(XTime(XDateTuple(1, 1, 2), XTimeTuple(0, 0, 0), XFracSec::Zero(), XUTC::Instance()).DayOfGregorianCal() == 2);
    UT_ASSERT(XTime(XDateTuple(1, 2, 1), XTimeTuple(0, 0, 0), XFracSec::Zero(), XUTC::Instance()).DayOfGregorianCal() == 32);
    UT_ASSERT(XTime(XDateTuple(2, 1, 1), XTimeTuple(0, 0, 0), XFracSec::Zero(), XUTC::Instance()).DayOfGregorianCal() == 366);

    UT_ASSERT(XTime(XDateTuple(1945, 11, 12), XTimeTuple(12, 2, 9), XFracSec(MSECS, 212)).DayOfGregorianCal() == 710347);
    UT_ASSERT(XTime(XDateTuple(1999, 1, 1), XTimeTuple(12, 2, 9), XFracSec(MSECS, 212)).DayOfGregorianCal() == 729755);
    UT_ASSERT(XTime(XDateTuple(2000, 1, 1), XTimeTuple(12, 2, 9), XFracSec(MSECS, 212)).DayOfGregorianCal() == 730120);
    UT_ASSERT(XTime(XDateTuple(2001, 1, 1), XTimeTuple(12, 2, 9), XFracSec(MSECS, 212)).DayOfGregorianCal() == 730486);

    UT_ASSERT(XTime(XDateTuple(2010, 1, 1), XTimeTuple(23, 59, 59), XFracSec(MSECS, 999)).DayOfGregorianCal() == 733773);
    UT_ASSERT(XTime(XDateTuple(2010, 1, 31), XTimeTuple(23, 59, 59), XFracSec(MSECS, 999)).DayOfGregorianCal() == 733803);
    UT_ASSERT(XTime(XDateTuple(2010, 2, 1), XTimeTuple(23, 59, 59), XFracSec(MSECS, 999)).DayOfGregorianCal() == 733804);
    UT_ASSERT(XTime(XDateTuple(2010, 2, 28), XTimeTuple(23, 59, 59), XFracSec(MSECS, 999)).DayOfGregorianCal() == 733831);
    UT_ASSERT(XTime(XDateTuple(2010, 3, 1), XTimeTuple(23, 59, 59), XFracSec(MSECS, 999)).DayOfGregorianCal() == 733832);
    UT_ASSERT(XTime(XDateTuple(2010, 3, 31), XTimeTuple(23, 59, 59), XFracSec(MSECS, 999)).DayOfGregorianCal() == 733862);
    UT_ASSERT(XTime(XDateTuple(2010, 4, 1), XTimeTuple(23, 59, 59), XFracSec(MSECS, 999)).DayOfGregorianCal() == 733863);
    UT_ASSERT(XTime(XDateTuple(2010, 4, 30), XTimeTuple(23, 59, 59), XFracSec(MSECS, 999)).DayOfGregorianCal() == 733892);
    UT_ASSERT(XTime(XDateTuple(2010, 5, 1), XTimeTuple(23, 59, 59), XFracSec(MSECS, 999)).DayOfGregorianCal() == 733893);
    UT_ASSERT(XTime(XDateTuple(2010, 5, 31), XTimeTuple(23, 59, 59), XFracSec(MSECS, 999)).DayOfGregorianCal() == 733923);
    UT_ASSERT(XTime(XDateTuple(2010, 6, 1), XTimeTuple(23, 59, 59), XFracSec(MSECS, 999)).DayOfGregorianCal() == 733924);
    UT_ASSERT(XTime(XDateTuple(2010, 6, 30), XTimeTuple(23, 59, 59), XFracSec(MSECS, 999)).DayOfGregorianCal() == 733953);
    UT_ASSERT(XTime(XDateTuple(2010, 7, 1), XTimeTuple(23, 59, 59), XFracSec(MSECS, 999)).DayOfGregorianCal() == 733954);
    UT_ASSERT(XTime(XDateTuple(2010, 7, 31), XTimeTuple(23, 59, 59), XFracSec(MSECS, 999)).DayOfGregorianCal() == 733984);
    UT_ASSERT(XTime(XDateTuple(2010, 8, 1), XTimeTuple(23, 59, 59), XFracSec(MSECS, 999)).DayOfGregorianCal() == 733985);
    UT_ASSERT(XTime(XDateTuple(2010, 8, 31), XTimeTuple(23, 59, 59), XFracSec(MSECS, 999)).DayOfGregorianCal() == 734015);
    UT_ASSERT(XTime(XDateTuple(2010, 9, 1), XTimeTuple(23, 59, 59), XFracSec(MSECS, 999)).DayOfGregorianCal() == 734016);
    UT_ASSERT(XTime(XDateTuple(2010, 9, 30), XTimeTuple(23, 59, 59), XFracSec(MSECS, 999)).DayOfGregorianCal() == 734045);
    UT_ASSERT(XTime(XDateTuple(2010, 10, 1), XTimeTuple(23, 59, 59), XFracSec(MSECS, 999)).DayOfGregorianCal() == 734046);
    UT_ASSERT(XTime(XDateTuple(2010, 10, 31), XTimeTuple(23, 59, 59), XFracSec(MSECS, 999)).DayOfGregorianCal() == 734076);
    UT_ASSERT(XTime(XDateTuple(2010, 11, 1), XTimeTuple(23, 59, 59), XFracSec(MSECS, 999)).DayOfGregorianCal() == 734077);
    UT_ASSERT(XTime(XDateTuple(2010, 11, 30), XTimeTuple(23, 59, 59), XFracSec(MSECS, 999)).DayOfGregorianCal() == 734106);
    UT_ASSERT(XTime(XDateTuple(2010, 12, 1), XTimeTuple(23, 59, 59), XFracSec(MSECS, 999)).DayOfGregorianCal() == 734107);
    UT_ASSERT(XTime(XDateTuple(2010, 12, 31), XTimeTuple(23, 59, 59), XFracSec(MSECS, 999)).DayOfGregorianCal() == 734137);

    UT_ASSERT(XTime(XDateTuple(2012, 2, 1), XTimeTuple(0, 0, 0), XFracSec(MSECS, 0)).DayOfGregorianCal() == 734534);
    UT_ASSERT(XTime(XDateTuple(2012, 2, 28), XTimeTuple(0, 0, 0), XFracSec(MSECS, 0)).DayOfGregorianCal() == 734561);
    UT_ASSERT(XTime(XDateTuple(2012, 2, 29), XTimeTuple(0, 0, 0), XFracSec(MSECS, 0)).DayOfGregorianCal() == 734562);
    UT_ASSERT(XTime(XDateTuple(2012, 3, 1), XTimeTuple(0, 0, 0), XFracSec(MSECS, 0)).DayOfGregorianCal() == 734563);
}


void XTimeTest::TestYear()
{
    UT_ASSERT(XTime(XDateTuple(), XTimeTuple(0, 0, 0)).Year() == 1);
    UT_ASSERT(XTime(XDateTuple(2009, 1, 1), XTimeTuple(0, 0, 0)).Year() == 2009);
    UT_ASSERT(XTime(XDateTuple(2009, 12, 31), XTimeTuple(0, 0, 0)).Year() == 2009);
    UT_ASSERT(XTime(XDateTuple(2008, 1, 1), XTimeTuple(0, 0, 0)).Year() == 2008);
    UT_ASSERT(XTime(XDateTuple(2008, 12, 31), XTimeTuple(0, 0, 0)).Year() == 2008);
    UT_ASSERT(XTime(XDateTuple(2011, 3, 9), XTimeTuple(0, 0, 0)).Year() == 2011);

    UT_ASSERT(XTime(XDateTuple(), XTimeTuple(23, 59, 59)).Year() == 1);
    UT_ASSERT(XTime(XDateTuple(2009, 1, 1), XTimeTuple(23, 59, 59)).Year() == 2009);
    UT_ASSERT(XTime(XDateTuple(2009, 12, 31), XTimeTuple(23, 59, 59)).Year() == 2009);
    UT_ASSERT(XTime(XDateTuple(2008, 1, 1), XTimeTuple(23, 59, 59)).Year() == 2008);
    UT_ASSERT(XTime(XDateTuple(2008, 12, 31), XTimeTuple(23, 59, 59)).Year() == 2008);
    UT_ASSERT(XTime(XDateTuple(2011, 3, 9), XTimeTuple(23, 59, 59)).Year() == 2011);

    {
        XTime xt = XTime(XDateTuple(), XTimeTuple());
        xt.SetYear(2009);
        UT_ASSERT(xt == XTime(XDateTuple(2009, 1, 1), XTimeTuple()));

        xt.SetYear(2012);
        UT_ASSERT(xt == XTime(XDateTuple(2012, 1, 1), XTimeTuple()));
    }

    {
        XTime xt = XTime(XDateTuple(), XTimeTuple());
        xt.SetYear(2012);
        UT_ASSERT(xt == XTime(XDateTuple(2012, 1, 1), XTimeTuple()));

        xt.SetYear(2009);
        UT_ASSERT(xt == XTime(XDateTuple(2009, 1, 1), XTimeTuple()));
    }

    {
        XTime xt = XTime(XDateTuple(2008, 7, 4), XTimeTuple(12, 12 ,12));
        xt.SetYear(2009);
        UT_ASSERT(xt == XTime(XDateTuple(2009, 7, 4), XTimeTuple(12, 12 ,12)));
    }

    {
        XTime xt = XTime(XDateTuple(2008, 2, 29), XTimeTuple(12, 12 ,12));
        UT_ASSERT_THROWS(xt.SetYear(2009), XException);
        UT_ASSERT(xt == XTime(XDateTuple(2008, 2, 29), XTimeTuple(12, 12 ,12)));
        xt.SetYear(2012);
        UT_ASSERT(xt == XTime(XDateTuple(2012, 2, 29), XTimeTuple(12, 12 ,12)));
    }

    {
        XTime xt = XTime(XDateTuple(2008, 2, 28), XTimeTuple(12, 12 ,12));
        xt.SetYear(2009);
        UT_ASSERT(xt == XTime(XDateTuple(2009, 2, 28), XTimeTuple(12, 12 ,12)));
        xt.SetYear(2012);
        UT_ASSERT(xt == XTime(XDateTuple(2012, 2, 28), XTimeTuple(12, 12 ,12)));
    }

    {
        XTime xt = XTime(XDateTuple(), XTimeTuple());
        UT_ASSERT_THROWS(xt.SetYear(0), XException);
        UT_ASSERT_THROWS(xt.SetYear(-1), XException);
    }

    //Verify that time zone conversion does not result in stuff blowing up for
    //default constructed XTimes.
    {
        XTime xt;
        xt._ticks = 0;
        UT_ASSERT(xt.Year() == 1);
        xt._ticks = -1;
        UT_ASSERT(xt.Year() == 1);
    }
}


void XTimeTest::TestMonth()
{
    UT_ASSERT(XTime(XDateTuple(), XTimeTuple(0, 0, 0)).Month() == 1);
    UT_ASSERT(XTime(XDateTuple(2009, 1, 1), XTimeTuple(0, 0, 0)).Month() == 1);
    UT_ASSERT(XTime(XDateTuple(2009, 1, 31), XTimeTuple(0, 0, 0)).Month() == 1);
    UT_ASSERT(XTime(XDateTuple(2009, 2, 28), XTimeTuple(0, 0, 0)).Month() == 2);
    UT_ASSERT(XTime(XDateTuple(2009, 3, 1), XTimeTuple(0, 0, 0)).Month() == 3);
    UT_ASSERT(XTime(XDateTuple(2009, 7, 4), XTimeTuple(0, 0, 0)).Month() == 7);
    UT_ASSERT(XTime(XDateTuple(2009, 12, 1), XTimeTuple(0, 0, 0)).Month() == 12);
    UT_ASSERT(XTime(XDateTuple(2009, 12, 31), XTimeTuple(0, 0, 0)).Month() == 12);

    UT_ASSERT(XTime(XDateTuple(), XTimeTuple(23, 59, 59)).Month() == 1);
    UT_ASSERT(XTime(XDateTuple(2009, 1, 1), XTimeTuple(23, 59, 59)).Month() == 1);
    UT_ASSERT(XTime(XDateTuple(2009, 1, 31), XTimeTuple(23, 59, 59)).Month() == 1);
    UT_ASSERT(XTime(XDateTuple(2009, 2, 28), XTimeTuple(23, 59, 59)).Month() == 2);
    UT_ASSERT(XTime(XDateTuple(2009, 3, 1), XTimeTuple(23, 59, 59)).Month() == 3);
    UT_ASSERT(XTime(XDateTuple(2009, 7, 4), XTimeTuple(23, 59, 59)).Month() == 7);
    UT_ASSERT(XTime(XDateTuple(2009, 12, 1), XTimeTuple(23, 59, 59)).Month() == 12);
    UT_ASSERT(XTime(XDateTuple(2009, 12, 31), XTimeTuple(23, 59, 59)).Month() == 12);

    UT_ASSERT(XTime(XDateTuple(), XTimeTuple(0, 0, 0)).Month() == 1);
    UT_ASSERT(XTime(XDateTuple(2008, 1, 1), XTimeTuple(0, 0, 0)).Month() == 1);
    UT_ASSERT(XTime(XDateTuple(2008, 1, 31), XTimeTuple(0, 0, 0)).Month() == 1);
    UT_ASSERT(XTime(XDateTuple(2008, 2, 28), XTimeTuple(0, 0, 0)).Month() == 2);
    UT_ASSERT(XTime(XDateTuple(2008, 2, 29), XTimeTuple(0, 0, 0)).Month() == 2);
    UT_ASSERT(XTime(XDateTuple(2008, 3, 1), XTimeTuple(0, 0, 0)).Month() == 3);
    UT_ASSERT(XTime(XDateTuple(2008, 7, 4), XTimeTuple(0, 0, 0)).Month() == 7);
    UT_ASSERT(XTime(XDateTuple(2008, 12, 1), XTimeTuple(0, 0, 0)).Month() == 12);
    UT_ASSERT(XTime(XDateTuple(2008, 12, 31), XTimeTuple(0, 0, 0)).Month() == 12);

    UT_ASSERT(XTime(XDateTuple(), XTimeTuple(23, 59, 59)).Month() == 1);
    UT_ASSERT(XTime(XDateTuple(2008, 1, 1), XTimeTuple(23, 59, 59)).Month() == 1);
    UT_ASSERT(XTime(XDateTuple(2008, 1, 31), XTimeTuple(23, 59, 59)).Month() == 1);
    UT_ASSERT(XTime(XDateTuple(2008, 2, 28), XTimeTuple(23, 59, 59)).Month() == 2);
    UT_ASSERT(XTime(XDateTuple(2008, 2, 29), XTimeTuple(23, 59, 59)).Month() == 2);
    UT_ASSERT(XTime(XDateTuple(2008, 3, 1), XTimeTuple(23, 59, 59)).Month() == 3);
    UT_ASSERT(XTime(XDateTuple(2008, 7, 4), XTimeTuple(23, 59, 59)).Month() == 7);
    UT_ASSERT(XTime(XDateTuple(2008, 12, 1), XTimeTuple(23, 59, 59)).Month() == 12);
    UT_ASSERT(XTime(XDateTuple(2008, 12, 31), XTimeTuple(23, 59, 59)).Month() == 12);

    {
        XTime xt = XTime(XDateTuple(), XTimeTuple());
        xt.SetMonth(1);
        UT_ASSERT(xt == XTime(XDateTuple(1, 1, 1), XTimeTuple()));

        xt.SetMonth(2);
        UT_ASSERT(xt == XTime(XDateTuple(1, 2, 1), XTimeTuple()));

        xt.SetMonth(7);
        UT_ASSERT(xt == XTime(XDateTuple(1, 7, 1), XTimeTuple()));

        xt.SetMonth(12);
        UT_ASSERT(xt == XTime(XDateTuple(1, 12, 1), XTimeTuple()));
    }

    {
        XTime xt = XTime(XDateTuple(2009, 7, 31), XTimeTuple(13, 13, 13));
        xt.SetMonth(7);
        UT_ASSERT(xt == XTime(XDateTuple(2009, 7, 31), XTimeTuple(13, 13, 13)));

        xt.SetMonth(1);
        UT_ASSERT(xt == XTime(XDateTuple(2009, 1, 31), XTimeTuple(13, 13, 13)));

        xt.SetMonth(12);
        UT_ASSERT(xt == XTime(XDateTuple(2009, 12, 31), XTimeTuple(13, 13, 13)));
    }

    {
        XTime xt = XTime(XDateTuple(2008, 1, 29), XTimeTuple(13, 13, 13));
        xt.SetMonth(1);
        UT_ASSERT(xt == XTime(XDateTuple(2008, 1, 29), XTimeTuple(13, 13, 13)));

        xt.SetMonth(2);
        UT_ASSERT(xt == XTime(XDateTuple(2008, 2, 29), XTimeTuple(13, 13, 13)));

        xt.SetMonth(3);
        UT_ASSERT(xt == XTime(XDateTuple(2008, 3, 29), XTimeTuple(13, 13, 13)));
    }

    {
        XTime xt = XTime(XDateTuple(2009, 3, 29), XTimeTuple(13, 13, 13));
        UT_ASSERT_THROWS(xt.SetMonth(2), XException);
        UT_ASSERT(xt == XTime(XDateTuple(2009, 3, 29), XTimeTuple(13, 13, 13)));
        xt.SetMonth(4);
        UT_ASSERT(xt == XTime(XDateTuple(2009, 4, 29), XTimeTuple(13, 13, 13)));
    }

    {
        XTime xt = XTime(XDateTuple(2008, 3, 30), XTimeTuple(13, 13, 13));
        UT_ASSERT_THROWS(xt.SetMonth(2), XException);
        UT_ASSERT(xt == XTime(XDateTuple(2008, 3, 30), XTimeTuple(13, 13, 13)));
        xt.SetMonth(4);
        UT_ASSERT(xt == XTime(XDateTuple(2008, 4, 30), XTimeTuple(13, 13, 13)));
    }

    {
        XTime xt = XTime(XDateTuple(2008, 3, 28), XTimeTuple(13, 13, 13));
        xt.SetMonth(2);
        UT_ASSERT(xt == XTime(XDateTuple(2008, 2, 28), XTimeTuple(13, 13, 13)));
        xt.SetMonth(4);
        UT_ASSERT(xt == XTime(XDateTuple(2008, 4, 28), XTimeTuple(13, 13, 13)));
    }

    {
        XTime xt = XTime(XDateTuple(2011, 1, 31), XTimeTuple(13, 13, 13));
        UT_ASSERT_THROWS(xt.SetMonth(2), XException);
        UT_ASSERT_THROWS(xt.SetMonth(4), XException);
        UT_ASSERT_THROWS(xt.SetMonth(6), XException);
        UT_ASSERT_THROWS(xt.SetMonth(9), XException);
        UT_ASSERT_THROWS(xt.SetMonth(11), XException);
        UT_ASSERT(xt == XTime(XDateTuple(2011, 1, 31), XTimeTuple(13, 13, 13)));
    }

    //Verify that time zone conversion does not result in stuff blowing up for
    //default constructed XTimes.
    {
        XTime xt;
        xt._ticks = 0;
        UT_ASSERT(xt.Month() == 1);
        xt._ticks = -1;
        UT_ASSERT(xt.Month() == 1);
    }
}


void XTimeTest::TestDay()
{
    UT_ASSERT(XTime(XDateTuple(), XTimeTuple(0, 0, 0)).Day() == 1);
    UT_ASSERT(XTime(XDateTuple(1999, 1, 1), XTimeTuple(0, 0, 0)).Day() == 1);
    UT_ASSERT(XTime(XDateTuple(1999, 1, 2), XTimeTuple(0, 0, 0)).Day() == 2);
    UT_ASSERT(XTime(XDateTuple(1999, 1, 4), XTimeTuple(0, 0, 0)).Day() == 4);
    UT_ASSERT(XTime(XDateTuple(1999, 1, 12), XTimeTuple(0, 0, 0)).Day() == 12);
    UT_ASSERT(XTime(XDateTuple(1999, 1, 31), XTimeTuple(0, 0, 0)).Day() == 31);
    UT_ASSERT(XTime(XDateTuple(1999, 2, 1), XTimeTuple(0, 0, 0)).Day() == 1);
    UT_ASSERT(XTime(XDateTuple(1999, 2, 28), XTimeTuple(0, 0, 0)).Day() == 28);
    UT_ASSERT(XTime(XDateTuple(1999, 3, 1), XTimeTuple(0, 0, 0)).Day() == 1);
    UT_ASSERT(XTime(XDateTuple(1999, 7, 4), XTimeTuple(0, 0, 0)).Day() == 4);
    UT_ASSERT(XTime(XDateTuple(1999, 12, 30), XTimeTuple(0, 0, 0)).Day() == 30);
    UT_ASSERT(XTime(XDateTuple(1999, 12, 31), XTimeTuple(0, 0, 0)).Day() == 31);

    UT_ASSERT(XTime(XDateTuple(2008, 1, 1), XTimeTuple(0, 0, 0)).Day() == 1);
    UT_ASSERT(XTime(XDateTuple(2008, 1, 2), XTimeTuple(0, 0, 0)).Day() == 2);
    UT_ASSERT(XTime(XDateTuple(2008, 1, 4), XTimeTuple(0, 0, 0)).Day() == 4);
    UT_ASSERT(XTime(XDateTuple(2008, 1, 12), XTimeTuple(0, 0, 0)).Day() == 12);
    UT_ASSERT(XTime(XDateTuple(2008, 1, 31), XTimeTuple(0, 0, 0)).Day() == 31);
    UT_ASSERT(XTime(XDateTuple(2008, 2, 1), XTimeTuple(0, 0, 0)).Day() == 1);
    UT_ASSERT(XTime(XDateTuple(2008, 2, 28), XTimeTuple(0, 0, 0)).Day() == 28);
    UT_ASSERT(XTime(XDateTuple(2008, 2, 29), XTimeTuple(0, 0, 0)).Day() == 29);
    UT_ASSERT(XTime(XDateTuple(2008, 3, 1), XTimeTuple(0, 0, 0)).Day() == 1);
    UT_ASSERT(XTime(XDateTuple(2008, 7, 4), XTimeTuple(0, 0, 0)).Day() == 4);
    UT_ASSERT(XTime(XDateTuple(2008, 12, 30), XTimeTuple(0, 0, 0)).Day() == 30);
    UT_ASSERT(XTime(XDateTuple(2008, 12, 31), XTimeTuple(0, 0, 0)).Day() == 31);

    UT_ASSERT(XTime(XDateTuple(), XTimeTuple(23, 59, 59)).Day() == 1);
    UT_ASSERT(XTime(XDateTuple(1999, 1, 1), XTimeTuple(23, 59, 59)).Day() == 1);
    UT_ASSERT(XTime(XDateTuple(1999, 1, 2), XTimeTuple(23, 59, 59)).Day() == 2);
    UT_ASSERT(XTime(XDateTuple(1999, 1, 4), XTimeTuple(23, 59, 59)).Day() == 4);
    UT_ASSERT(XTime(XDateTuple(1999, 1, 12), XTimeTuple(23, 59, 59)).Day() == 12);
    UT_ASSERT(XTime(XDateTuple(1999, 1, 31), XTimeTuple(23, 59, 59)).Day() == 31);
    UT_ASSERT(XTime(XDateTuple(1999, 2, 1), XTimeTuple(23, 59, 59)).Day() == 1);
    UT_ASSERT(XTime(XDateTuple(1999, 2, 28), XTimeTuple(23, 59, 59)).Day() == 28);
    UT_ASSERT(XTime(XDateTuple(1999, 3, 1), XTimeTuple(23, 59, 59)).Day() == 1);
    UT_ASSERT(XTime(XDateTuple(1999, 7, 4), XTimeTuple(23, 59, 59)).Day() == 4);
    UT_ASSERT(XTime(XDateTuple(1999, 12, 30), XTimeTuple(23, 59, 59)).Day() == 30);
    UT_ASSERT(XTime(XDateTuple(1999, 12, 31), XTimeTuple(23, 59, 59)).Day() == 31);

    UT_ASSERT(XTime(XDateTuple(2008, 1, 1), XTimeTuple(23, 59, 59)).Day() == 1);
    UT_ASSERT(XTime(XDateTuple(2008, 1, 4), XTimeTuple(23, 59, 59)).Day() == 4);
    UT_ASSERT(XTime(XDateTuple(2008, 1, 12), XTimeTuple(23, 59, 59)).Day() == 12);
    UT_ASSERT(XTime(XDateTuple(2008, 1, 31), XTimeTuple(23, 59, 59)).Day() == 31);
    UT_ASSERT(XTime(XDateTuple(2008, 2, 1), XTimeTuple(23, 59, 59)).Day() == 1);
    UT_ASSERT(XTime(XDateTuple(2008, 2, 28), XTimeTuple(23, 59, 59)).Day() == 28);
    UT_ASSERT(XTime(XDateTuple(2008, 2, 29), XTimeTuple(23, 59, 59)).Day() == 29);
    UT_ASSERT(XTime(XDateTuple(2008, 3, 1), XTimeTuple(23, 59, 59)).Day() == 1);
    UT_ASSERT(XTime(XDateTuple(2008, 7, 4), XTimeTuple(23, 59, 59)).Day() == 4);
    UT_ASSERT(XTime(XDateTuple(2008, 12, 30), XTimeTuple(23, 59, 59)).Day() == 30);
    UT_ASSERT(XTime(XDateTuple(2008, 12, 31), XTimeTuple(23, 59, 59)).Day() == 31);

    {
        XTime xt = XTime(XDateTuple(), XTimeTuple());
        xt.SetDay(1);
        UT_ASSERT(xt == XTime(XDateTuple(1, 1, 1), XTimeTuple()));

        xt.SetDay(2);
        UT_ASSERT(xt == XTime(XDateTuple(1, 1, 2), XTimeTuple()));

        xt.SetDay(7);
        UT_ASSERT(xt == XTime(XDateTuple(1, 1, 7), XTimeTuple()));

        xt.SetDay(31);
        UT_ASSERT(xt == XTime(XDateTuple(1, 1, 31), XTimeTuple()));
    }

    {
        XTime xt = XTime(XDateTuple(2009, 7, 31), XTimeTuple(4, 5, 6));
        xt.SetDay(31);
        UT_ASSERT(xt == XTime(XDateTuple(2009, 7, 31), XTimeTuple(4, 5, 6)));

        xt.SetDay(1);
        UT_ASSERT(xt == XTime(XDateTuple(2009, 7, 1), XTimeTuple(4, 5, 6)));

        xt.SetDay(12);
        UT_ASSERT(xt == XTime(XDateTuple(2009, 7, 12), XTimeTuple(4, 5, 6)));
    }

    {
        XTime xt = XTime(XDateTuple(2008, 2, 29), XTimeTuple(7, 8, 9));
        xt.SetDay(7);
        UT_ASSERT(xt == XTime(XDateTuple(2008, 2, 7), XTimeTuple(7, 8, 9)));

        xt.SetDay(29);
        UT_ASSERT(xt == XTime(XDateTuple(2008, 2, 29), XTimeTuple(7, 8, 9)));

        xt.SetDay(28);
        UT_ASSERT(xt == XTime(XDateTuple(2008, 2, 28), XTimeTuple(7, 8, 9)));
    }

    {
        XTime xt = XTime(XDateTuple(2011, 1, 31), XTimeTuple(10, 11, 12));
        UT_ASSERT_THROWS(xt.SetDay(32), XException);
        UT_ASSERT(xt == XTime(XDateTuple(2011, 1, 31), XTimeTuple(10, 11, 12)));
    }

    {
        XTime xt = XTime(XDateTuple(2009, 2, 5), XTimeTuple(13, 14, 15));
        UT_ASSERT_THROWS(xt.SetDay(29), XException);
        UT_ASSERT(xt == XTime(XDateTuple(2009, 2, 5), XTimeTuple(13, 14, 15)));
        xt.SetDay(28);
        UT_ASSERT(xt == XTime(XDateTuple(2009, 2, 28), XTimeTuple(13, 14, 15)));
    }

    {
        XTime xt = XTime(XDateTuple(2008, 2, 5), XTimeTuple(16, 17, 18));
        UT_ASSERT_THROWS(xt.SetDay(30), XException);
        UT_ASSERT(xt == XTime(XDateTuple(2008, 2, 5), XTimeTuple(16, 17, 18)));
        xt.SetDay(29);
        UT_ASSERT(xt == XTime(XDateTuple(2008, 2, 29), XTimeTuple(16, 17, 18)));
    }

    {
        XTime xt = XTime(XDateTuple(2011, 3, 31), XTimeTuple(19, 20, 21));
        UT_ASSERT_THROWS(xt.SetDay(32), XException);
    }

    {
        XTime xt = XTime(XDateTuple(2011, 4, 30), XTimeTuple(22, 23, 24));
        UT_ASSERT_THROWS(xt.SetDay(31), XException);
    }

    {
        XTime xt = XTime(XDateTuple(2011, 5, 30), XTimeTuple(0, 26, 27));
        UT_ASSERT_THROWS(xt.SetDay(32), XException);
    }

    {
        XTime xt = XTime(XDateTuple(2011, 6, 7), XTimeTuple(1, 29, 30));
        UT_ASSERT_THROWS(xt.SetDay(31), XException);
    }

    {
        XTime xt = XTime(XDateTuple(2011, 7, 7), XTimeTuple(2, 32, 33));
        UT_ASSERT_THROWS(xt.SetDay(32), XException);
    }

    {
        XTime xt = XTime(XDateTuple(2011, 8, 7), XTimeTuple(3, 35, 36));
        UT_ASSERT_THROWS(xt.SetDay(32), XException);
    }

    {
        XTime xt = XTime(XDateTuple(2011, 9, 22), XTimeTuple(4, 38, 39));
        UT_ASSERT_THROWS(xt.SetDay(31), XException);
    }

    {
        XTime xt = XTime(XDateTuple(2011, 10, 22), XTimeTuple(5, 41, 42));
        UT_ASSERT_THROWS(xt.SetDay(32), XException);
    }

    {
        XTime xt = XTime(XDateTuple(2011, 11, 22), XTimeTuple(6, 44, 45));
        UT_ASSERT_THROWS(xt.SetDay(31), XException);
    }

    {
        XTime xt = XTime(XDateTuple(2011, 12, 31), XTimeTuple(7, 47, 48));
        UT_ASSERT_THROWS(xt.SetDay(32), XException);
    }

    //Verify that time zone conversion does not result in stuff blowing up for
    //default constructed XTimes.
    {
        XTime xt;
        xt._ticks = 0;
        UT_ASSERT(xt.Day() == 1);
        xt._ticks = -1;
        UT_ASSERT(xt.Day() == 1);
    }
}


void XTimeTest::TestHour()
{
    UT_ASSERT(XTime(XDateTuple(), XTimeTuple()).Hour() == 0);
    UT_ASSERT(XTime(XDateTuple(1999, 3, 1), XTimeTuple(7, 0, 0)).Hour() == 7);
    UT_ASSERT(XTime(XDateTuple(1999, 3, 18), XTimeTuple(7, 0, 0)).Hour() == 7);
    UT_ASSERT(XTime(XDateTuple(1999, 4, 1), XTimeTuple(7, 0, 0)).Hour() == 7);
    UT_ASSERT(XTime(XDateTuple(1999, 4, 18), XTimeTuple(7, 0, 0)).Hour() == 7);
    UT_ASSERT(XTime(XDateTuple(1999, 10, 1), XTimeTuple(7, 0, 0)).Hour() == 7);
    UT_ASSERT(XTime(XDateTuple(1999, 10, 18), XTimeTuple(7, 0, 0)).Hour() == 7);
    UT_ASSERT(XTime(XDateTuple(1999, 10, 30), XTimeTuple(7, 0, 0)).Hour() == 7);
    UT_ASSERT(XTime(XDateTuple(1999, 11, 2), XTimeTuple(7, 0, 0)).Hour() == 7);
    UT_ASSERT(XTime(XDateTuple(1999, 11, 18), XTimeTuple(7, 0, 0)).Hour() == 7);

    UT_ASSERT(XTime(XDateTuple(2000, 3, 1), XTimeTuple(7, 0, 0)).Hour() == 7);
    UT_ASSERT(XTime(XDateTuple(2000, 3, 18), XTimeTuple(7, 0, 0)).Hour() == 7);
    UT_ASSERT(XTime(XDateTuple(2000, 4, 1), XTimeTuple(7, 0, 0)).Hour() == 7);
    UT_ASSERT(XTime(XDateTuple(2000, 4, 18), XTimeTuple(7, 0, 0)).Hour() == 7);
    UT_ASSERT(XTime(XDateTuple(2000, 10, 1), XTimeTuple(7, 0, 0)).Hour() == 7);
    UT_ASSERT(XTime(XDateTuple(2000, 10, 18), XTimeTuple(7, 0, 0)).Hour() == 7);
    UT_ASSERT(XTime(XDateTuple(2000, 10, 30), XTimeTuple(7, 0, 0)).Hour() == 7);
    UT_ASSERT(XTime(XDateTuple(2000, 11, 1), XTimeTuple(7, 0, 0)).Hour() == 7);
    UT_ASSERT(XTime(XDateTuple(2000, 11, 18), XTimeTuple(7, 0, 0)).Hour() == 7);

    UT_ASSERT(XTime(XDateTuple(2009, 3, 1), XTimeTuple(7, 0, 0)).Hour() == 7);
    UT_ASSERT(XTime(XDateTuple(2009, 3, 18), XTimeTuple(7, 0, 0)).Hour() == 7);
    UT_ASSERT(XTime(XDateTuple(2009, 4, 1), XTimeTuple(7, 0, 0)).Hour() == 7);
    UT_ASSERT(XTime(XDateTuple(2009, 4, 18), XTimeTuple(7, 0, 0)).Hour() == 7);
    UT_ASSERT(XTime(XDateTuple(2009, 10, 1), XTimeTuple(7, 0, 0)).Hour() == 7);
    UT_ASSERT(XTime(XDateTuple(2009, 10, 18), XTimeTuple(7, 0, 0)).Hour() == 7);
    UT_ASSERT(XTime(XDateTuple(2009, 10, 30), XTimeTuple(7, 0, 0)).Hour() == 7);
    UT_ASSERT(XTime(XDateTuple(2009, 11, 2), XTimeTuple(7, 0, 0)).Hour() == 7);
    UT_ASSERT(XTime(XDateTuple(2009, 11, 18), XTimeTuple(7, 0, 0)).Hour() == 7);

    UT_ASSERT(XTime(XDateTuple(2012, 3, 1), XTimeTuple(7, 0, 0)).Hour() == 7);
    UT_ASSERT(XTime(XDateTuple(2012, 3, 18), XTimeTuple(7, 0, 0)).Hour() == 7);
    UT_ASSERT(XTime(XDateTuple(2012, 4, 1), XTimeTuple(7, 0, 0)).Hour() == 7);
    UT_ASSERT(XTime(XDateTuple(2012, 4, 18), XTimeTuple(7, 0, 0)).Hour() == 7);
    UT_ASSERT(XTime(XDateTuple(2012, 10, 1), XTimeTuple(7, 0, 0)).Hour() == 7);
    UT_ASSERT(XTime(XDateTuple(2012, 10, 18), XTimeTuple(7, 0, 0)).Hour() == 7);
    UT_ASSERT(XTime(XDateTuple(2012, 10, 30), XTimeTuple(7, 0, 0)).Hour() == 7);
    UT_ASSERT(XTime(XDateTuple(2012, 11, 1), XTimeTuple(7, 0, 0)).Hour() == 7);
    UT_ASSERT(XTime(XDateTuple(2012, 11, 18), XTimeTuple(7, 0, 0)).Hour() == 7);

    UT_ASSERT(XTime(XDateTuple(2000, 7, 5), XTimeTuple(5, 0, 0)).Hour() == 5);
    UT_ASSERT(XTime(XDateTuple(2007, 2, 20), XTimeTuple(5, 59, 59)).Hour() == 5);
    UT_ASSERT(XTime(XDateTuple(2010, 4, 30), XTimeTuple(23, 0, 0)).Hour() == 23);
    UT_ASSERT(XTime(XDateTuple(2012, 12, 21), XTimeTuple(23, 59, 59)).Hour() == 23);

    {
        XTime xt = XTime(XDateTuple(), XTimeTuple());
        xt.SetHour(0);
        UT_ASSERT(xt == XTime(XDateTuple(), XTimeTuple(0, 0, 0)));
    }

    {
        XTime xt = XTime(XDateTuple(), XTimeTuple());
        xt.SetHour(9);
        UT_ASSERT(xt == XTime(XDateTuple(), XTimeTuple(9, 0, 0)));
    }

    {
        XTime xt = XTime(XDateTuple(), XTimeTuple());
        xt.SetHour(23);
        UT_ASSERT(xt == XTime(XDateTuple(), XTimeTuple(23, 0, 0)));
    }

    {
        XTime xt = XTime(XDateTuple(2012, 12, 21), XTimeTuple(23, 59, 59));
        xt.SetHour(0);
        UT_ASSERT(xt == XTime(XDateTuple(2012, 12, 21), XTimeTuple(0, 59, 59)));
    }

    {
        XTime xt = XTime(XDateTuple(2012, 12, 21), XTimeTuple(23, 59, 59));
        xt.SetHour(9);
        UT_ASSERT(xt == XTime(XDateTuple(2012, 12, 21), XTimeTuple(9, 59, 59)));
    }

    {
        XTime xt = XTime(XDateTuple(2012, 12, 21), XTimeTuple(23, 59, 59));
        xt.SetHour(23);
        UT_ASSERT(xt == XTime(XDateTuple(2012, 12, 21), XTimeTuple(23, 59, 59)));
    }

    {
        XTime xt = XTime(XDateTuple(), XTimeTuple());
        UT_ASSERT_THROWS(xt.SetHour(-1), XException);
        UT_ASSERT_THROWS(xt.SetHour(24), XException);
        UT_ASSERT(xt == XTime(XDateTuple(), XTimeTuple()));
    }

    //Verify that time zone conversion does not result in stuff blowing up for
    //default constructed XTimes.
    {
        XTime xt;
        xt._ticks = 0;
        UT_ASSERT(xt.Hour() == 0);
        xt._ticks = -1;
        UT_ASSERT(xt.Hour() == 0);
    }
}


void XTimeTest::TestMinute()
{
    UT_ASSERT(XTime(XDateTuple(), XTimeTuple()).Minute() == 0);
    UT_ASSERT(XTime(XDateTuple(2000, 7, 5), XTimeTuple(0, 33, 0)).Minute() == 33);
    UT_ASSERT(XTime(XDateTuple(2007, 2, 20), XTimeTuple(23, 33, 59)).Minute() == 33);
    UT_ASSERT(XTime(XDateTuple(2010, 4, 30), XTimeTuple(0, 59, 0)).Minute() == 59);
    UT_ASSERT(XTime(XDateTuple(2012, 12, 21), XTimeTuple(23, 59, 59)).Minute() == 59);

    {
        XTime xt = XTime(XDateTuple(), XTimeTuple());
        xt.SetMinute(0);
        UT_ASSERT(xt == XTime(XDateTuple(), XTimeTuple(0, 0, 0)));
    }

    {
        XTime xt = XTime(XDateTuple(), XTimeTuple());
        xt.SetMinute(17);
        UT_ASSERT(xt == XTime(XDateTuple(), XTimeTuple(0, 17, 0)));
    }

    {
        XTime xt = XTime(XDateTuple(), XTimeTuple());
        xt.SetMinute(59);
        UT_ASSERT(xt == XTime(XDateTuple(), XTimeTuple(0, 59, 0)));
    }

    {
        XTime xt = XTime(XDateTuple(2012, 12, 21), XTimeTuple(23, 59, 59));
        xt.SetMinute(0);
        UT_ASSERT(xt == XTime(XDateTuple(2012, 12, 21), XTimeTuple(23, 0, 59)));
    }

    {
        XTime xt = XTime(XDateTuple(2012, 12, 21), XTimeTuple(23, 59, 59));
        xt.SetMinute(17);
        UT_ASSERT(xt == XTime(XDateTuple(2012, 12, 21), XTimeTuple(23, 17, 59)));
    }

    {
        XTime xt = XTime(XDateTuple(2012, 12, 21), XTimeTuple(23, 59, 59));
        xt.SetMinute(59);
        UT_ASSERT(xt == XTime(XDateTuple(2012, 12, 21), XTimeTuple(23, 59, 59)));
    }

    {
        XTime xt = XTime(XDateTuple(), XTimeTuple());
        UT_ASSERT_THROWS(xt.SetMinute(-1), XException);
        UT_ASSERT_THROWS(xt.SetMinute(60), XException);
        UT_ASSERT(xt == XTime(XDateTuple(), XTimeTuple()));
    }

    //Verify that time zone conversion does not result in stuff blowing up for
    //default constructed XTimes.
    {
        XTime xt;
        xt._ticks = 0;
        UT_ASSERT(xt.Minute() == 0);
        xt._ticks = -1;
        UT_ASSERT(xt.Minute() == 0);
    }
}


void XTimeTest::TestSecond()
{
    UT_ASSERT(XTime(XDateTuple(), XTimeTuple()).Second() == 0);
    UT_ASSERT(XTime(XDateTuple(2000, 7, 5), XTimeTuple(0, 0, 33)).Second() == 33);
    UT_ASSERT(XTime(XDateTuple(2007, 2, 20), XTimeTuple(23, 59, 33)).Second() == 33);
    UT_ASSERT(XTime(XDateTuple(2010, 4, 30), XTimeTuple(0, 0, 59)).Second() == 59);
    UT_ASSERT(XTime(XDateTuple(2012, 12, 21), XTimeTuple(23, 59, 59)).Second() == 59);

    {
        XTime xt = XTime(XDateTuple(), XTimeTuple());
        xt.SetSecond(0);
        UT_ASSERT(xt == XTime(XDateTuple(), XTimeTuple(0, 0, 0)));
    }

    {
        XTime xt = XTime(XDateTuple(), XTimeTuple());
        xt.SetSecond(17);
        UT_ASSERT(xt == XTime(XDateTuple(), XTimeTuple(0, 0, 17)));
    }

    {
        XTime xt = XTime(XDateTuple(), XTimeTuple());
        xt.SetSecond(59);
        UT_ASSERT(xt == XTime(XDateTuple(), XTimeTuple(0, 0, 59)));
    }

    {
        XTime xt = XTime(XDateTuple(2012, 12, 21), XTimeTuple(23, 59, 59));
        xt.SetSecond(0);
        UT_ASSERT(xt == XTime(XDateTuple(2012, 12, 21), XTimeTuple(23, 59, 0)));
    }

    {
        XTime xt = XTime(XDateTuple(2012, 12, 21), XTimeTuple(23, 59, 59));
        xt.SetSecond(17);
        UT_ASSERT(xt == XTime(XDateTuple(2012, 12, 21), XTimeTuple(23, 59, 17)));
    }

    {
        XTime xt = XTime(XDateTuple(2012, 12, 21), XTimeTuple(23, 59, 59));
        xt.SetSecond(59);
        UT_ASSERT(xt == XTime(XDateTuple(2012, 12, 21), XTimeTuple(23, 59, 59)));
    }

    {
        XTime xt = XTime(XDateTuple(), XTimeTuple());
        UT_ASSERT_THROWS(xt.SetSecond(-1), XException);
        UT_ASSERT_THROWS(xt.SetSecond(60), XException);
        UT_ASSERT(xt == XTime(XDateTuple(), XTimeTuple()));
    }

    //Verify that time zone conversion does not result in stuff blowing up for
    //default constructed XTimes.
    {
        XTime xt;
        xt._ticks = 0;
        UT_ASSERT(xt.Second() == 0);
        xt._ticks = -1;
        UT_ASSERT(xt.Second() == 0);
    }
}


void XTimeTest::TestFracSec()
{
    UT_ASSERT(XTime(0, XUTC::Instance()).FracSec() == XFracSec(HNSECS, 0));
    UT_ASSERT(XTime(10000000, XUTC::Instance()).FracSec() == XFracSec(HNSECS, 0));
    UT_ASSERT(XTime(10000001, XUTC::Instance()).FracSec() == XFracSec(HNSECS, 1));
    UT_ASSERT(XTime(10000002, XUTC::Instance()).FracSec() == XFracSec(HNSECS, 2));
    UT_ASSERT(XTime(11000000, XUTC::Instance()).FracSec() == XFracSec(MSECS, 100));
    UT_ASSERT(XTime(19999999, XUTC::Instance()).FracSec() == XFracSec(HNSECS, 9999999LL));
    UT_ASSERT(XTime(621355968000000000LL, XUTC::Instance()).FracSec() == XFracSec(HNSECS, 0));
    UT_ASSERT(XTime(621355968000005012LL, XUTC::Instance()).FracSec() == XFracSec(HNSECS, 5012));

    {
        XTime xt = XTime(XDateTuple(), XTimeTuple());
        xt.SetFracSec(XFracSec::Zero());
        UT_ASSERT(xt == XTime(XDateTuple(), XTimeTuple(0, 0, 0)));
    }

    {
        XTime xt = XTime(XDateTuple(), XTimeTuple());
        xt.SetFracSec(XFracSec(HNSECS, 1007));
        UT_ASSERT(xt == XTime(XDateTuple(), XTimeTuple(0, 0, 0), XFracSec(HNSECS, 1007)));
    }

    {
        XTime xt = XTime(XDateTuple(), XTimeTuple());
        xt.SetFracSec(XFracSec(HNSECS, 9999999));
        UT_ASSERT(xt == XTime(XDateTuple(), XTimeTuple(0, 0, 0), XFracSec(HNSECS, 9999999)));
    }

    {
        XTime xt = XTime(XDateTuple(2012, 12, 21), XTimeTuple(23, 59, 59), XFracSec(HNSECS, 9999999));
        xt.SetFracSec(XFracSec::Zero());
        UT_ASSERT(xt == XTime(XDateTuple(2012, 12, 21), XTimeTuple(23, 59, 59)));
    }

    {
        XTime xt = XTime(XDateTuple(2012, 12, 21), XTimeTuple(23, 59, 59));
        xt.SetFracSec(XFracSec(HNSECS, 1007));
        UT_ASSERT(xt == XTime(XDateTuple(2012, 12, 21), XTimeTuple(23, 59, 59), XFracSec(HNSECS, 1007)));
    }

    {
        XTime xt = XTime(XDateTuple(2012, 12, 21), XTimeTuple(23, 59, 59), XFracSec(HNSECS, 9999999));
        xt.SetFracSec(XFracSec(HNSECS, 9999999));
        UT_ASSERT(xt == XTime(XDateTuple(2012, 12, 21), XTimeTuple(23, 59, 59), XFracSec(HNSECS, 9999999)));
    }

    {
        XTime xt = XTime(XDateTuple(), XTimeTuple());
        UT_ASSERT_THROWS(xt.SetFracSec(XFracSec(HNSECS, -1)), XException);
        UT_ASSERT_THROWS(xt.SetFracSec(XFracSec(HNSECS, -9999999)), XException);
        UT_ASSERT(xt == XTime(XDateTuple(), XTimeTuple()));
    }

    //Verify that time zone conversion does not result in stuff blowing up for
    //default constructed XTimes.
    {
        XTime xt;
        xt._ticks = 0;
        UT_ASSERT(xt.FracSec() == XFracSec::Zero());
        xt._ticks = -1;
        UT_ASSERT(xt.FracSec() == XFracSec::Zero());
    }
}


void XTimeTest::TestTimeZone()
{
    XIRef<XTimeZone> simple = new XSimpleTimeZone(-8 * 60, 0);

    UT_ASSERT(XTime(0, XUTC::Instance()).TimeZone().Get() == XUTC::Instance().Get());
    UT_ASSERT(XTime(0, XUTC::Instance()).TimeZone().Get() != simple.Get());
    UT_ASSERT(XTime(0, simple).TimeZone().Get() == simple.Get());
    UT_ASSERT(XTime(0, simple).TimeZone().Get() != XUTC::Instance().Get());

    UT_ASSERT(XTime(1234567890, XUTC::Instance()).TimeZone().Get() == XUTC::Instance().Get());
    UT_ASSERT(XTime(1234567890, XUTC::Instance()).TimeZone().Get() != simple.Get());
    UT_ASSERT(XTime(1234567890, simple).TimeZone().Get() == simple.Get());
    UT_ASSERT(XTime(1234567890, simple).TimeZone().Get() != XUTC::Instance().Get());
}


void XTimeTest::TestToLocalTime()
{

    {
        XTime xt = XTime(0, XLocalTime::Instance());
        XTime localTime = xt.ToLocalTime();

        UT_ASSERT(xt == localTime);
        UT_ASSERT(xt.TimeZone().Get() == localTime.TimeZone().Get());
        UT_ASSERT(xt.TimeZone().Get() == XLocalTime::Instance().Get());
        UT_ASSERT(localTime.TimeZone().Get() == XLocalTime::Instance().Get());
    }

    {
        XTime xt = XTime(0, XUTC::Instance());
        XTime localTime = xt.ToLocalTime();

        UT_ASSERT(xt == localTime);
        UT_ASSERT(xt.TimeZone().Get() != localTime.TimeZone().Get());
        UT_ASSERT(xt.TimeZone().Get() == XUTC::Instance().Get());
        UT_ASSERT(localTime.TimeZone().Get() == XLocalTime::Instance().Get());
    }

    {
        XIRef<XTimeZone> simple = new XSimpleTimeZone(-2 * 60, 0);
        XTime xt = XTime(0, simple);
        XTime localTime = xt.ToLocalTime();

        UT_ASSERT(xt == localTime);
        UT_ASSERT(xt.TimeZone().Get() != localTime.TimeZone().Get());
        UT_ASSERT(xt.TimeZone().Get() == simple.Get());
        UT_ASSERT(localTime.TimeZone().Get() == XLocalTime::Instance().Get());
    }
}


void XTimeTest::TestToUTC()
{
    {
        const XTime xt = XTime(0, XLocalTime::Instance());
        const XTime utc = xt.ToUTC();

        UT_ASSERT(xt == utc);
        UT_ASSERT(xt.TimeZone().Get() != utc.TimeZone().Get());
        UT_ASSERT(xt.TimeZone().Get() == XLocalTime::Instance().Get());
        UT_ASSERT(utc.TimeZone().Get() == XUTC::Instance().Get());
    }

    {
        const XTime xt = XTime(0, XUTC::Instance());
        const XTime utc = xt.ToUTC();

        UT_ASSERT(xt == utc);
        UT_ASSERT(xt.TimeZone().Get() == utc.TimeZone().Get());
        UT_ASSERT(xt.TimeZone().Get() == XUTC::Instance().Get());
        UT_ASSERT(utc.TimeZone().Get() == XUTC::Instance().Get());
    }

    {
        XIRef<XTimeZone> simple = new XSimpleTimeZone(-8 * 60, 0);
        const XTime xt = XTime(0, simple);
        const XTime utc = xt.ToUTC();

        UT_ASSERT(xt == utc);
        UT_ASSERT(xt.TimeZone().Get() != utc.TimeZone().Get());
        UT_ASSERT(xt.TimeZone().Get() == simple.Get());
        UT_ASSERT(utc.TimeZone().Get() == XUTC::Instance().Get());
    }
}


void XTimeTest::TestToOtherTZ()
{
    XIRef<XTimeZone> simple = new XSimpleTimeZone(-8 * 60, 0);

    {
        XTime xt = XTime(0, XLocalTime::Instance());
        XTime other = xt.ToOtherTZ(simple);

        UT_ASSERT(xt == other);
        UT_ASSERT(xt.TimeZone().Get() != other.TimeZone().Get());
        UT_ASSERT(xt.TimeZone().Get() == XLocalTime::Instance().Get());
        UT_ASSERT(other.TimeZone().Get() == simple.Get());
    }

    {
        XTime xt = XTime(0, XUTC::Instance());
        XTime other = xt.ToOtherTZ(simple);

        UT_ASSERT(xt == other);
        UT_ASSERT(xt.TimeZone().Get() != other.TimeZone().Get());
        UT_ASSERT(xt.TimeZone().Get() == XUTC::Instance().Get());
        UT_ASSERT(other.TimeZone().Get() == simple.Get());
    }

    {
        XTime xt = XTime(0, simple);
        XTime other = xt.ToOtherTZ(simple);

        UT_ASSERT(xt == other);
        UT_ASSERT(xt.TimeZone().Get() == other.TimeZone().Get());
        UT_ASSERT(xt.TimeZone().Get() == simple.Get());
        UT_ASSERT(other.TimeZone().Get() == simple.Get());
    }
}


void XTimeTest::TestToUnixTime()
{
    UT_ASSERT(XTime(621355968000000000LL, XUTC::Instance()).ToUnixTime() == 0);  //Midnight, January 1st, 1970
    UT_ASSERT(XTime(621355968000000000LL + 864000000000LL, XUTC::Instance()).ToUnixTime() == 86400);  //Midnight, January 2nd, 1970
    UT_ASSERT(XTime(621355968000000000LL - 864000000000LL, XUTC::Instance()).ToUnixTime() == -86400);  //Midnight, December 31st, 1969

    UT_ASSERT(XTime(621355968000000000LL, new XSimpleTimeZone(2 * 60, 42)).ToUnixTime() == 0);  //Midnight, January 1st, 1970
    UT_ASSERT(XTime(621355968000000000LL + 864000000000LL, new XSimpleTimeZone(-11 * 60, 0)).ToUnixTime() == 86400);  //Midnight, January 2nd, 1970
    UT_ASSERT(XTime(621355968000000000LL - 864000000000LL, new XSimpleTimeZone(-8 * 60, 0)).ToUnixTime() == -86400);  //Midnight, December 31st, 1969

    UT_ASSERT(XTime(621355968000000000LL, XUTC::Instance()).ToUnixTime() == 0);
    UT_ASSERT(XTime(621355968000000000LL + 864771234567LL, XUTC::Instance()).ToUnixTime() == 86477);
    UT_ASSERT(XTime(621355968000000000LL - 864771234567LL, XUTC::Instance()).ToUnixTime() == -86477);

    UT_ASSERT(XTime(621355968000000000LL, new XSimpleTimeZone(2 * 60, 42)).ToUnixTime() == 0);
    UT_ASSERT(XTime(621355968000000000LL + 864771234567LL, new XSimpleTimeZone(-11 * 60, 0)).ToUnixTime() == 86477);
    UT_ASSERT(XTime(621355968000000000LL - 864771234567LL, new XSimpleTimeZone(-8 * 60, 0)).ToUnixTime() == -86477);
}


void XTimeTest::TestToUnixTimeAsMSecs()
{
    UT_ASSERT(XTime(621355968000000000LL, XUTC::Instance()).ToUnixTimeAsMSecs() == 0);  //Midnight, January 1st, 1970
    UT_ASSERT(XTime(621355968000000000LL + 864000000000LL, XUTC::Instance()).ToUnixTimeAsMSecs() == 86400000);  //Midnight, January 2nd, 1970
    UT_ASSERT(XTime(621355968000000000LL - 864000000000LL, XUTC::Instance()).ToUnixTimeAsMSecs() == -86400000);  //Midnight, December 31st, 1969

    UT_ASSERT(XTime(621355968000000000LL, new XSimpleTimeZone(2 * 60, 42)).ToUnixTimeAsMSecs() == 0);  //Midnight, January 1st, 1970
    UT_ASSERT(XTime(621355968000000000LL + 864000000000LL, new XSimpleTimeZone(-11 * 60, 0)).ToUnixTimeAsMSecs() == 86400000);  //Midnight, January 2nd, 1970
    UT_ASSERT(XTime(621355968000000000LL - 864000000000LL, new XSimpleTimeZone(-8 * 60, 0)).ToUnixTimeAsMSecs() == -86400000);  //Midnight, December 31st, 1969

    UT_ASSERT(XTime(621355968000000000LL + 864771234567LL, XUTC::Instance()).ToUnixTimeAsMSecs() == 86477123);
    UT_ASSERT(XTime(621355968000000000LL - 864771234567LL, XUTC::Instance()).ToUnixTimeAsMSecs() == -86477123);

    UT_ASSERT(XTime(621355968000000000LL, new XSimpleTimeZone(2 * 60, 42)).ToUnixTimeAsMSecs() == 0);
    UT_ASSERT(XTime(621355968000000000LL + 864771234567LL, new XSimpleTimeZone(-11 * 60, 0)).ToUnixTimeAsMSecs() == 86477123);
    UT_ASSERT(XTime(621355968000000000LL - 864771234567LL, new XSimpleTimeZone(-8 * 60, 0)).ToUnixTimeAsMSecs() == -86477123);
}


void XTimeTest::TestToTimeVal()
{
    {
        timeval tv = XTime(621355968000000000LL, XUTC::Instance()).ToTimeVal();
        UT_ASSERT(tv.tv_sec == 0);
        UT_ASSERT(tv.tv_usec == 0);
    }

    {
        timeval tv = XTime(621355968000001234LL, XUTC::Instance()).ToTimeVal();
        UT_ASSERT(tv.tv_sec == 0);
        UT_ASSERT(tv.tv_usec == 123);
    }

    {
        timeval tv = XTime(621355968123456789LL, XUTC::Instance()).ToTimeVal();
        UT_ASSERT(tv.tv_sec == 12);
        UT_ASSERT(tv.tv_usec == 345678);
    }

    {
        timeval tv = XTime(621355968000000000LL, new XSimpleTimeZone(2 * 60, 42)).ToTimeVal();
        UT_ASSERT(tv.tv_sec == 0);
        UT_ASSERT(tv.tv_usec == 0);
    }

    {
        timeval tv = XTime(621355968000001234LL, new XSimpleTimeZone(11 * 60, 0)).ToTimeVal();
        UT_ASSERT(tv.tv_sec == 0);
        UT_ASSERT(tv.tv_usec == 123);
    }

    {
        timeval tv = XTime(621355968123456789LL, new XSimpleTimeZone(-8 * 60, 60)).ToTimeVal();
        UT_ASSERT(tv.tv_sec == 12);
        UT_ASSERT(tv.tv_usec == 345678);
    }

    {
        timeval tv = XTime(621355968000000000LL + 864070000000LL + 12009LL, new XSimpleTimeZone(-8 * 60, 60)).ToTimeVal();
        UT_ASSERT(tv.tv_sec == 86407);
        UT_ASSERT(tv.tv_usec == 1200);
    }
}


void XTimeTest::TestToXDateTuple()
{
    vector<XIRef<XTimeZone> > tzs;
    tzs.push_back(XUTC::Instance());
    //tzs.push_back(XLocalTime::Instance());
    tzs.push_back(new XSimpleTimeZone(-300, 0));
    tzs.push_back(new XSimpleTimeZone(-300, 60));
    tzs.push_back(new XSimpleTimeZone(300, 0));
    tzs.push_back(new XSimpleTimeZone(300, 60));

    for(vector<XIRef<XTimeZone> >::iterator iter = tzs.begin(),
                                               end  = tzs.end();
        iter != end;
        ++iter)
    {
        UT_ASSERT(XTime(XDateTuple(1999, 7, 6), XTimeTuple(0, 0, 0), *iter).ToXDateTuple() == XDateTuple(1999, 7, 6));
        UT_ASSERT(XTime(XDateTuple(2000, 12, 31), XTimeTuple(0, 0, 0), *iter).ToXDateTuple() == XDateTuple(2000, 12, 31));
        UT_ASSERT(XTime(XDateTuple(2001, 1, 1), XTimeTuple(0, 0, 0), *iter).ToXDateTuple() == XDateTuple(2001, 1, 1));

        UT_ASSERT(XTime(XDateTuple(1999, 7, 6), XTimeTuple(9, 10, 11), *iter).ToXDateTuple() == XDateTuple(1999, 7, 6));
        UT_ASSERT(XTime(XDateTuple(2000, 12, 31), XTimeTuple(12, 13, 14), *iter).ToXDateTuple() == XDateTuple(2000, 12, 31));
        UT_ASSERT(XTime(XDateTuple(2001, 1, 1), XTimeTuple(15, 16, 17), *iter).ToXDateTuple() == XDateTuple(2001, 1, 1));

        UT_ASSERT(XTime(XDateTuple(1999, 7, 6), XTimeTuple(23, 59, 59), *iter).ToXDateTuple() == XDateTuple(1999, 7, 6));
        UT_ASSERT(XTime(XDateTuple(2000, 12, 31), XTimeTuple(23, 59, 59), *iter).ToXDateTuple() == XDateTuple(2000, 12, 31));
        UT_ASSERT(XTime(XDateTuple(2001, 1, 1), XTimeTuple(23, 59, 59), *iter).ToXDateTuple() == XDateTuple(2001, 1, 1));

        UT_ASSERT(XTime(XDateTuple(1999, 7, 6), XTimeTuple(0, 0, 0), XFracSec(HNSECS, 1), *iter).ToXDateTuple() == XDateTuple(1999, 7, 6));
        UT_ASSERT(XTime(XDateTuple(1999, 7, 6), XTimeTuple(23, 59, 59), XFracSec(HNSECS, 1), *iter).ToXDateTuple() == XDateTuple(1999, 7, 6));
        UT_ASSERT(XTime(XDateTuple(1999, 7, 6), XTimeTuple(23, 59, 59), XFracSec(HNSECS, 9999999), *iter).ToXDateTuple() == XDateTuple(1999, 7, 6));
    }

    //Verify that time zone conversion does not result in stuff blowing up for
    //default constructed XTimes.
    {
        XTime xt;
        xt._ticks = 0;
        UT_ASSERT(xt.ToXDateTuple() == XDateTuple());
        xt._ticks = -1;
        UT_ASSERT(xt.ToXDateTuple() == XDateTuple());
    }
}


void XTimeTest::TestToXTimeTuple()
{
    vector<XIRef<XTimeZone> > tzs;
    tzs.push_back(XUTC::Instance());
    //tzs.push_back(XLocalTime::Instance());
    tzs.push_back(new XSimpleTimeZone(-300, 0));
    tzs.push_back(new XSimpleTimeZone(-300, 60));
    tzs.push_back(new XSimpleTimeZone(300, 0));
    tzs.push_back(new XSimpleTimeZone(300, 60));

    for(vector<XIRef<XTimeZone> >::iterator iter = tzs.begin(),
                                               end  = tzs.end();
        iter != end;
        ++iter)
    {
        UT_ASSERT(XTime(XDateTuple(1999, 7, 6), XTimeTuple(0, 0, 0), *iter).ToXTimeTuple() == XTimeTuple(0, 0, 0));
        UT_ASSERT(XTime(XDateTuple(2000, 12, 31), XTimeTuple(0, 0, 0), *iter).ToXTimeTuple() == XTimeTuple(0, 0, 0));
        UT_ASSERT(XTime(XDateTuple(2001, 1, 1), XTimeTuple(0, 0, 0), *iter).ToXTimeTuple() == XTimeTuple(0, 0, 0));

        UT_ASSERT(XTime(XDateTuple(1999, 7, 6), XTimeTuple(9, 10, 11), *iter).ToXTimeTuple() == XTimeTuple(9, 10, 11));
        UT_ASSERT(XTime(XDateTuple(2000, 12, 31), XTimeTuple(12, 13, 14), *iter).ToXTimeTuple() == XTimeTuple(12, 13, 14));
        UT_ASSERT(XTime(XDateTuple(2001, 1, 1), XTimeTuple(15, 16, 17), *iter).ToXTimeTuple() == XTimeTuple(15, 16, 17));

        UT_ASSERT(XTime(XDateTuple(1999, 7, 6), XTimeTuple(23, 59, 59), *iter).ToXTimeTuple() == XTimeTuple(23, 59, 59));
        UT_ASSERT(XTime(XDateTuple(2000, 12, 31), XTimeTuple(23, 59, 59), *iter).ToXTimeTuple() == XTimeTuple(23, 59, 59));
        UT_ASSERT(XTime(XDateTuple(2001, 1, 1), XTimeTuple(23, 59, 59), *iter).ToXTimeTuple() == XTimeTuple(23, 59, 59));

        UT_ASSERT(XTime(XDateTuple(1999, 7, 6), XTimeTuple(0, 0, 0), XFracSec(HNSECS, 1), *iter).ToXTimeTuple() == XTimeTuple(0, 0, 0));
        UT_ASSERT(XTime(XDateTuple(1999, 7, 6), XTimeTuple(23, 59, 59), XFracSec(HNSECS, 1), *iter).ToXTimeTuple() == XTimeTuple(23, 59, 59));
        UT_ASSERT(XTime(XDateTuple(1999, 7, 6), XTimeTuple(23, 59, 59), XFracSec(HNSECS, 9999999), *iter).ToXTimeTuple() == XTimeTuple(23, 59, 59));
    }

    //Verify that time zone conversion does not result in stuff blowing up for
    //default constructed XTimes.
    {
        XTime xt;
        xt._ticks = 0;
        UT_ASSERT(xt.ToXTimeTuple() == XTimeTuple());
        xt._ticks = -1;
        UT_ASSERT(xt.ToXTimeTuple() == XTimeTuple());
    }
}


void XTimeTest::TestToISOExtString()
{
    UT_ASSERT(XTime(XDateTuple(), XTimeTuple(), XLocalTime::Instance()).ToISOExtString() == "0001-01-01T00:00:00");
    UT_ASSERT(XTime(XDateTuple(), XTimeTuple(), XUTC::Instance()).ToISOExtString() == "0001-01-01T00:00:00Z");
    UT_ASSERT(XTime(XDateTuple(1, 1, 1), XTimeTuple(0, 0, 0), XFracSec(HNSECS, 1), XLocalTime::Instance()).ToISOExtString() == "0001-01-01T00:00:00.0000001");
    UT_ASSERT(XTime(XDateTuple(1, 1, 1), XTimeTuple(0, 0, 0), XFracSec(HNSECS, 1), XUTC::Instance()).ToISOExtString() == "0001-01-01T00:00:00.0000001Z");

    UT_ASSERT(XTime(XDateTuple(9, 12, 4), XTimeTuple(0, 0, 0)).ToISOExtString() == "0009-12-04T00:00:00");
    UT_ASSERT(XTime(XDateTuple(99, 12, 4), XTimeTuple(5, 6, 12)).ToISOExtString() == "0099-12-04T05:06:12");
    UT_ASSERT(XTime(XDateTuple(999, 12, 4), XTimeTuple(13, 44, 59)).ToISOExtString() == "0999-12-04T13:44:59");
    UT_ASSERT(XTime(XDateTuple(9999, 7, 4), XTimeTuple(23, 59, 59)).ToISOExtString() == "9999-07-04T23:59:59");
    UT_ASSERT(XTime(XDateTuple(10000, 10, 20), XTimeTuple(1, 1, 1)).ToISOExtString() == "+10000-10-20T01:01:01");

    UT_ASSERT(XTime(XDateTuple(9, 12, 4), XTimeTuple(0, 0, 0), XFracSec(MSECS, 42)).ToISOExtString() == "0009-12-04T00:00:00.042");
    UT_ASSERT(XTime(XDateTuple(99, 12, 4), XTimeTuple(5, 6, 12), XFracSec(MSECS, 100)).ToISOExtString() == "0099-12-04T05:06:12.1");
    UT_ASSERT(XTime(XDateTuple(999, 12, 4), XTimeTuple(13, 44, 59), XFracSec(USECS, 45020)).ToISOExtString() == "0999-12-04T13:44:59.04502");
    UT_ASSERT(XTime(XDateTuple(9999, 7, 4), XTimeTuple(23, 59, 59), XFracSec(HNSECS, 12)).ToISOExtString() == "9999-07-04T23:59:59.0000012");
    UT_ASSERT(XTime(XDateTuple(10000, 10, 20), XTimeTuple(1, 1, 1), XFracSec(HNSECS, 507890)).ToISOExtString() == "+10000-10-20T01:01:01.050789");

    UT_ASSERT(XTime(XDateTuple(2012, 12, 21), XTimeTuple(13, 13, 13), new XSimpleTimeZone(-5 * 60, 0)).ToISOExtString() == "2012-12-21T13:13:13-05:00");
    UT_ASSERT(XTime(XDateTuple(2012, 12, 21), XTimeTuple(13, 13, 13), new XSimpleTimeZone(-5 * 60, 30)).ToISOExtString() == "2012-12-21T13:13:13-04:30");

    UT_ASSERT(XTime(XDateTuple(2012, 12, 21), XTimeTuple(13, 13, 13), new XSimpleTimeZone(5 * 60, 0)).ToISOExtString() == "2012-12-21T13:13:13+05:00");
    UT_ASSERT(XTime(XDateTuple(2012, 12, 21), XTimeTuple(13, 13, 13), new XSimpleTimeZone(5 * 60, 30)).ToISOExtString() == "2012-12-21T13:13:13+05:30");

    //Verify that time zone conversion does not result in stuff blowing up for
    //default constructed XTimes.
    {
        XTime xt;
        xt._ticks = 0;
        UT_ASSERT(xt.ToISOExtString() == "0001-01-01T00:00:00");
        xt._ticks = -1;
        UT_ASSERT(xt.ToISOExtString() == "0001-01-01T00:00:00");
    }
}


void XTimeTest::TestToMPEG7String()
{
    UT_ASSERT(XTime(XDateTuple(), XTimeTuple(), XLocalTime::Instance()).ToMPEG7String() == "0001-01-01T00:00:00");
    UT_ASSERT(XTime(XDateTuple(), XTimeTuple(), XUTC::Instance()).ToMPEG7String() == "0001-01-01T00:00:00");
    UT_ASSERT(XTime(XDateTuple(1, 1, 1), XTimeTuple(0, 0, 0), XFracSec(HNSECS, 1), XLocalTime::Instance()).ToMPEG7String() == "0001-01-01T00:00:00:1F10000000");
    UT_ASSERT(XTime(XDateTuple(1, 1, 1), XTimeTuple(0, 0, 0), XFracSec(HNSECS, 1), XUTC::Instance()).ToMPEG7String() == "0001-01-01T00:00:00:1F10000000");

    UT_ASSERT(XTime(XDateTuple(9, 12, 4), XTimeTuple(0, 0, 0)).ToMPEG7String() == "0009-12-04T00:00:00");
    UT_ASSERT(XTime(XDateTuple(99, 12, 4), XTimeTuple(5, 6, 12)).ToMPEG7String() == "0099-12-04T05:06:12");
    UT_ASSERT(XTime(XDateTuple(999, 12, 4), XTimeTuple(13, 44, 59)).ToMPEG7String() == "0999-12-04T13:44:59");
    UT_ASSERT(XTime(XDateTuple(9999, 7, 4), XTimeTuple(23, 59, 59)).ToMPEG7String() == "9999-07-04T23:59:59");
    UT_ASSERT(XTime(XDateTuple(10000, 10, 20), XTimeTuple(1, 1, 1)).ToMPEG7String() == "+10000-10-20T01:01:01");

    UT_ASSERT(XTime(XDateTuple(9, 12, 4), XTimeTuple(0, 0, 0), XFracSec(MSECS, 42)).ToMPEG7String() == "0009-12-04T00:00:00:42F1000");
    UT_ASSERT(XTime(XDateTuple(99, 12, 4), XTimeTuple(5, 6, 12), XFracSec(MSECS, 100)).ToMPEG7String() == "0099-12-04T05:06:12:1F10");
    UT_ASSERT(XTime(XDateTuple(999, 12, 4), XTimeTuple(13, 44, 59), XFracSec(USECS, 45020)).ToMPEG7String() == "0999-12-04T13:44:59:4502F100000");
    UT_ASSERT(XTime(XDateTuple(9999, 7, 4), XTimeTuple(23, 59, 59), XFracSec(HNSECS, 12)).ToMPEG7String() == "9999-07-04T23:59:59:12F10000000");
    UT_ASSERT(XTime(XDateTuple(10000, 10, 20), XTimeTuple(1, 1, 1), XFracSec(HNSECS, 507890)).ToMPEG7String() == "+10000-10-20T01:01:01:50789F1000000");

    UT_ASSERT(XTime(XDateTuple(2012, 12, 21), XTimeTuple(13, 13, 13), new XSimpleTimeZone(-5 * 60, 0)).ToMPEG7String() == "2012-12-21T13:13:13");
    UT_ASSERT(XTime(XDateTuple(2012, 12, 21), XTimeTuple(13, 13, 13), new XSimpleTimeZone(-5 * 60, 30)).ToMPEG7String() == "2012-12-21T13:13:13");

    UT_ASSERT(XTime(XDateTuple(2012, 12, 21), XTimeTuple(13, 13, 13), new XSimpleTimeZone(5 * 60, 0)).ToMPEG7String() == "2012-12-21T13:13:13");
    UT_ASSERT(XTime(XDateTuple(2012, 12, 21), XTimeTuple(13, 13, 13), new XSimpleTimeZone(5 * 60, 30)).ToMPEG7String() == "2012-12-21T13:13:13");

    //Verify that time zone conversion does not result in stuff blowing up for
    //default constructed XTimes.
    {
        XTime xt;
        xt._ticks = 0;
        UT_ASSERT(xt.ToMPEG7String() == "0001-01-01T00:00:00");
        xt._ticks = -1;
        UT_ASSERT(xt.ToMPEG7String() == "0001-01-01T00:00:00");
    }
}


void XTimeTest::TestToCustomString()
{
    UT_ASSERT(XTime(XDateTuple(), XTimeTuple(), XLocalTime::Instance()).ToCustomString("YYYY-MM-DD", "HH:MM:SS") == "0001-01-01 00:00:00");
    UT_ASSERT(XTime(XDateTuple(), XTimeTuple(), XUTC::Instance()).ToCustomString("YYYY-MM-DD", "HH:MM:SS") == "0001-01-01 00:00:00");
    UT_ASSERT(XTime(XDateTuple(1, 1, 1), XTimeTuple(0, 0, 0), XFracSec(HNSECS, 1), XLocalTime::Instance()).ToCustomString("YYYY-MM-DD", "HH:MM:SS") == "0001-01-01 00:00:00");
    UT_ASSERT(XTime(XDateTuple(1, 1, 1), XTimeTuple(0, 0, 0), XFracSec(HNSECS, 1), XUTC::Instance()).ToCustomString("YYYY-MM-DD", "HH:MM:SS") == "0001-01-01 00:00:00");

    UT_ASSERT(XTime(XDateTuple(9, 12, 4), XTimeTuple(0, 0, 0)).ToCustomString("YYYY-MM-DD", "HH:MM:SS") == "0009-12-04 00:00:00");
    UT_ASSERT(XTime(XDateTuple(99, 12, 4), XTimeTuple(5, 6, 12)).ToCustomString("YYYY-MM-DD", "HH:MM:SS") == "0099-12-04 05:06:12");
    UT_ASSERT(XTime(XDateTuple(999, 12, 4), XTimeTuple(13, 44, 59)).ToCustomString("YYYY-MM-DD", "HH:MM:SS") == "0999-12-04 13:44:59");
    UT_ASSERT(XTime(XDateTuple(9999, 7, 4), XTimeTuple(23, 59, 59)).ToCustomString("YYYY-MM-DD", "HH:MM:SS") == "9999-07-04 23:59:59");

    UT_ASSERT(XTime(XDateTuple(9, 12, 4), XTimeTuple(0, 0, 0), XFracSec(MSECS, 42)).ToCustomString("YYYY-MM-DD", "HH:MM:SS") == "0009-12-04 00:00:00");
    UT_ASSERT(XTime(XDateTuple(99, 12, 4), XTimeTuple(5, 6, 12), XFracSec(MSECS, 100)).ToCustomString("YYYY-MM-DD", "HH:MM:SS") == "0099-12-04 05:06:12");
    UT_ASSERT(XTime(XDateTuple(999, 12, 4), XTimeTuple(13, 44, 59), XFracSec(USECS, 45020)).ToCustomString("YYYY-MM-DD", "HH:MM:SS") == "0999-12-04 13:44:59");
    UT_ASSERT(XTime(XDateTuple(9999, 7, 4), XTimeTuple(23, 59, 59), XFracSec(HNSECS, 12)).ToCustomString("YYYY-MM-DD", "HH:MM:SS") == "9999-07-04 23:59:59");

    UT_ASSERT(XTime(XDateTuple(2012, 12, 21), XTimeTuple(13, 13, 13), new XSimpleTimeZone(-5 * 60, 0)).ToCustomString("YYYY-MM-DD", "HH:MM:SS") == "2012-12-21 13:13:13");
    UT_ASSERT(XTime(XDateTuple(2012, 12, 21), XTimeTuple(13, 13, 13), new XSimpleTimeZone(-5 * 60, 30)).ToCustomString("YYYY-MM-DD", "HH:MM:SS") == "2012-12-21 13:13:13");

    UT_ASSERT(XTime(XDateTuple(2012, 12, 21), XTimeTuple(13, 13, 13), new XSimpleTimeZone(5 * 60, 0)).ToCustomString("YYYY-MM-DD", "HH:MM:SS") == "2012-12-21 13:13:13");
    UT_ASSERT(XTime(XDateTuple(2012, 12, 21), XTimeTuple(13, 13, 13), new XSimpleTimeZone(5 * 60, 30)).ToCustomString("YYYY-MM-DD", "HH:MM:SS") == "2012-12-21 13:13:13");


    UT_ASSERT(XTime(XDateTuple(), XTimeTuple(), XLocalTime::Instance()).ToCustomString("YYYY-MM-DD", "HH:MM:SS", "T", true, 7) == "0001-01-01T00:00:00");
    UT_ASSERT(XTime(XDateTuple(), XTimeTuple(), XUTC::Instance()).ToCustomString("YYYY-MM-DD", "HH:MM:SS", "T", true, 7) == "0001-01-01T00:00:00");
    UT_ASSERT(XTime(XDateTuple(1, 1, 1), XTimeTuple(0, 0, 0), XFracSec(HNSECS, 1), XLocalTime::Instance()).ToCustomString("YYYY-MM-DD", "HH:MM:SS", "T", true, 7) == "0001-01-01T00:00:00.0000001");
    UT_ASSERT(XTime(XDateTuple(1, 1, 1), XTimeTuple(0, 0, 0), XFracSec(HNSECS, 1), XUTC::Instance()).ToCustomString("YYYY-MM-DD", "HH:MM:SS", "T", true, 7) == "0001-01-01T00:00:00.0000001");

    UT_ASSERT(XTime(XDateTuple(9, 12, 4), XTimeTuple(0, 0, 0)).ToCustomString("YYYY-MM-DD", "HH:MM:SS", "T", true, 7) == "0009-12-04T00:00:00");
    UT_ASSERT(XTime(XDateTuple(99, 12, 4), XTimeTuple(5, 6, 12)).ToCustomString("YYYY-MM-DD", "HH:MM:SS", "T", true, 7) == "0099-12-04T05:06:12");
    UT_ASSERT(XTime(XDateTuple(999, 12, 4), XTimeTuple(13, 44, 59)).ToCustomString("YYYY-MM-DD", "HH:MM:SS", "T", true, 7) == "0999-12-04T13:44:59");
    UT_ASSERT(XTime(XDateTuple(9999, 7, 4), XTimeTuple(23, 59, 59)).ToCustomString("YYYY-MM-DD", "HH:MM:SS", "T", true, 7) == "9999-07-04T23:59:59");

    UT_ASSERT(XTime(XDateTuple(9, 12, 4), XTimeTuple(0, 0, 0), XFracSec(MSECS, 42)).ToCustomString("YYYY-MM-DD", "HH:MM:SS", "T", true, 7) == "0009-12-04T00:00:00.042");
    UT_ASSERT(XTime(XDateTuple(99, 12, 4), XTimeTuple(5, 6, 12), XFracSec(MSECS, 100)).ToCustomString("YYYY-MM-DD", "HH:MM:SS", "T", true, 7) == "0099-12-04T05:06:12.1");
    UT_ASSERT(XTime(XDateTuple(999, 12, 4), XTimeTuple(13, 44, 59), XFracSec(USECS, 45020)).ToCustomString("YYYY-MM-DD", "HH:MM:SS", "T", true, 7) == "0999-12-04T13:44:59.04502");
    UT_ASSERT(XTime(XDateTuple(9999, 7, 4), XTimeTuple(23, 59, 59), XFracSec(HNSECS, 12)).ToCustomString("YYYY-MM-DD", "HH:MM:SS", "T", true, 7) == "9999-07-04T23:59:59.0000012");

    UT_ASSERT(XTime(XDateTuple(2012, 12, 21), XTimeTuple(13, 13, 13), new XSimpleTimeZone(-5 * 60, 0)).ToCustomString("YYYY-MM-DD", "HH:MM:SS", "T", true, 7) == "2012-12-21T13:13:13");
    UT_ASSERT(XTime(XDateTuple(2012, 12, 21), XTimeTuple(13, 13, 13), new XSimpleTimeZone(-5 * 60, 30)).ToCustomString("YYYY-MM-DD", "HH:MM:SS", "T", true, 7) == "2012-12-21T13:13:13");

    UT_ASSERT(XTime(XDateTuple(2012, 12, 21), XTimeTuple(13, 13, 13), new XSimpleTimeZone(5 * 60, 0)).ToCustomString("YYYY-MM-DD", "HH:MM:SS", "T", true, 7) == "2012-12-21T13:13:13");
    UT_ASSERT(XTime(XDateTuple(2012, 12, 21), XTimeTuple(13, 13, 13), new XSimpleTimeZone(5 * 60, 30)).ToCustomString("YYYY-MM-DD", "HH:MM:SS", "T", true, 7) == "2012-12-21T13:13:13");


    UT_ASSERT(XTime(XDateTuple(), XTimeTuple(), XLocalTime::Instance()).ToCustomString("YYYY-MM-DD", "HH:MM:SS", "T", true, 2) == "0001-01-01T00:00:00");
    UT_ASSERT(XTime(XDateTuple(), XTimeTuple(), XUTC::Instance()).ToCustomString("YYYY-MM-DD", "HH:MM:SS", "T", true, 2) == "0001-01-01T00:00:00");
    UT_ASSERT(XTime(XDateTuple(1, 1, 1), XTimeTuple(0, 0, 0), XFracSec(HNSECS, 1), XLocalTime::Instance()).ToCustomString("YYYY-MM-DD", "HH:MM:SS", "T", true, 3) == "0001-01-01T00:00:00");
    UT_ASSERT(XTime(XDateTuple(1, 1, 1), XTimeTuple(0, 0, 0), XFracSec(HNSECS, 1), XUTC::Instance()).ToCustomString("YYYY-MM-DD", "HH:MM:SS", "T", true, 3) == "0001-01-01T00:00:00");

    UT_ASSERT(XTime(XDateTuple(9, 12, 4), XTimeTuple(0, 0, 0)).ToCustomString("YYYY-MM-DD", "HH:MM:SS", "T", true, 3) == "0009-12-04T00:00:00");
    UT_ASSERT(XTime(XDateTuple(99, 12, 4), XTimeTuple(5, 6, 12)).ToCustomString("YYYY-MM-DD", "HH:MM:SS", "T", true, 3) == "0099-12-04T05:06:12");
    UT_ASSERT(XTime(XDateTuple(999, 12, 4), XTimeTuple(13, 44, 59)).ToCustomString("YYYY-MM-DD", "HH:MM:SS", "T", true, 3) == "0999-12-04T13:44:59");
    UT_ASSERT(XTime(XDateTuple(9999, 7, 4), XTimeTuple(23, 59, 59)).ToCustomString("YYYY-MM-DD", "HH:MM:SS", "T", true, 3) == "9999-07-04T23:59:59");

    UT_ASSERT(XTime(XDateTuple(9, 12, 4), XTimeTuple(0, 0, 0), XFracSec(MSECS, 42)).ToCustomString("YYYY-MM-DD", "HH:MM:SS", "T", true, 3) == "0009-12-04T00:00:00.042");
    UT_ASSERT(XTime(XDateTuple(99, 12, 4), XTimeTuple(5, 6, 12), XFracSec(MSECS, 100)).ToCustomString("YYYY-MM-DD", "HH:MM:SS", "T", true, 3) == "0099-12-04T05:06:12.1");
    UT_ASSERT(XTime(XDateTuple(999, 12, 4), XTimeTuple(13, 44, 59), XFracSec(USECS, 40520)).ToCustomString("YYYY-MM-DD", "HH:MM:SS", "T", true, 4) == "0999-12-04T13:44:59.04");
    UT_ASSERT(XTime(XDateTuple(9999, 7, 4), XTimeTuple(23, 59, 59), XFracSec(HNSECS, 12)).ToCustomString("YYYY-MM-DD", "HH:MM:SS", "T", true, 2) == "9999-07-04T23:59:59");

    UT_ASSERT(XTime(XDateTuple(2012, 12, 21), XTimeTuple(13, 13, 13), new XSimpleTimeZone(-5 * 60, 0)).ToCustomString("YYYY-MM-DD", "HH:MM:SS", "T", true, 3) == "2012-12-21T13:13:13");
    UT_ASSERT(XTime(XDateTuple(2012, 12, 21), XTimeTuple(13, 13, 13), new XSimpleTimeZone(-5 * 60, 30)).ToCustomString("YYYY-MM-DD", "HH:MM:SS", "T", true, 3) == "2012-12-21T13:13:13");

    UT_ASSERT(XTime(XDateTuple(2012, 12, 21), XTimeTuple(13, 13, 13), new XSimpleTimeZone(5 * 60, 0)).ToCustomString("YYYY-MM-DD", "HH:MM:SS", "T", true, 3) == "2012-12-21T13:13:13");
    UT_ASSERT(XTime(XDateTuple(2012, 12, 21), XTimeTuple(13, 13, 13), new XSimpleTimeZone(5 * 60, 30)).ToCustomString("YYYY-MM-DD", "HH:MM:SS", "T", true, 3) == "2012-12-21T13:13:13");


    UT_ASSERT(XTime(XDateTuple(), XTimeTuple(), XLocalTime::Instance()).ToCustomString("YYYY-MM-DD", "HH:MM:SS", "hello world", false, 7) == "00:00:00hello world0001-01-01");
    UT_ASSERT(XTime(XDateTuple(), XTimeTuple(), XUTC::Instance()).ToCustomString("YYYY-MM-DD", "HH:MM:SS", "hello world", false, 7) == "00:00:00hello world0001-01-01");
    UT_ASSERT(XTime(XDateTuple(1, 1, 1), XTimeTuple(0, 0, 0), XFracSec(HNSECS, 1), XLocalTime::Instance()).ToCustomString("YYYY-MM-DD", "HH:MM:SS", "hello world", false, 7) == "00:00:00.0000001hello world0001-01-01");
    UT_ASSERT(XTime(XDateTuple(1, 1, 1), XTimeTuple(0, 0, 0), XFracSec(HNSECS, 1), XUTC::Instance()).ToCustomString("YYYY-MM-DD", "HH:MM:SS", "hello world", false, 7) == "00:00:00.0000001hello world0001-01-01");

    UT_ASSERT(XTime(XDateTuple(9, 12, 4), XTimeTuple(0, 0, 0)).ToCustomString("YYYY-MM-DD", "HH:MM:SS", "hello world", false, 7) == "00:00:00hello world0009-12-04");
    UT_ASSERT(XTime(XDateTuple(99, 12, 4), XTimeTuple(5, 6, 12)).ToCustomString("YYYY-MM-DD", "HH:MM:SS", "hello world", false, 7) == "05:06:12hello world0099-12-04");
    UT_ASSERT(XTime(XDateTuple(999, 12, 4), XTimeTuple(13, 44, 59)).ToCustomString("YYYY-MM-DD", "HH:MM:SS", "hello world", false, 7) == "13:44:59hello world0999-12-04");
    UT_ASSERT(XTime(XDateTuple(9999, 7, 4), XTimeTuple(23, 59, 59)).ToCustomString("YYYY-MM-DD", "HH:MM:SS", "hello world", false, 7) == "23:59:59hello world9999-07-04");

    UT_ASSERT(XTime(XDateTuple(9, 12, 4), XTimeTuple(0, 0, 0), XFracSec(MSECS, 42)).ToCustomString("YYYY-MM-DD", "HH:MM:SS", "hello world", false, 7) == "00:00:00.042hello world0009-12-04");
    UT_ASSERT(XTime(XDateTuple(99, 12, 4), XTimeTuple(5, 6, 12), XFracSec(MSECS, 100)).ToCustomString("YYYY-MM-DD", "HH:MM:SS", "hello world", false, 7) == "05:06:12.1hello world0099-12-04");
    UT_ASSERT(XTime(XDateTuple(999, 12, 4), XTimeTuple(13, 44, 59), XFracSec(USECS, 45020)).ToCustomString("YYYY-MM-DD", "HH:MM:SS", "hello world", false, 7) == "13:44:59.04502hello world0999-12-04");
    UT_ASSERT(XTime(XDateTuple(9999, 7, 4), XTimeTuple(23, 59, 59), XFracSec(HNSECS, 12)).ToCustomString("YYYY-MM-DD", "HH:MM:SS", "hello world", false, 7) == "23:59:59.0000012hello world9999-07-04");

    UT_ASSERT(XTime(XDateTuple(2012, 12, 21), XTimeTuple(13, 13, 13), new XSimpleTimeZone(-5 * 60, 0)).ToCustomString("YYYY-MM-DD", "HH:MM:SS", "hello world", false, 7) == "13:13:13hello world2012-12-21");
    UT_ASSERT(XTime(XDateTuple(2012, 12, 21), XTimeTuple(13, 13, 13), new XSimpleTimeZone(-5 * 60, 30)).ToCustomString("YYYY-MM-DD", "HH:MM:SS", "hello world", false, 7) == "13:13:13hello world2012-12-21");

    UT_ASSERT(XTime(XDateTuple(2012, 12, 21), XTimeTuple(13, 13, 13), new XSimpleTimeZone(5 * 60, 0)).ToCustomString("YYYY-MM-DD", "HH:MM:SS", "hello world", false, 7) == "13:13:13hello world2012-12-21");
    UT_ASSERT(XTime(XDateTuple(2012, 12, 21), XTimeTuple(13, 13, 13), new XSimpleTimeZone(5 * 60, 30)).ToCustomString("YYYY-MM-DD", "HH:MM:SS", "hello world", false, 7) == "13:13:13hello world2012-12-21");


    UT_ASSERT(XTime(XDateTuple(), XTimeTuple(), XLocalTime::Instance()).ToCustomString("MM-YY/DD", "MM_SS_HH", "???", false) == "00_00_00???01-01/01");
    UT_ASSERT(XTime(XDateTuple(), XTimeTuple(), XUTC::Instance()).ToCustomString("MM-YY/DD", "MM_SS_HH", "???", false) == "00_00_00???01-01/01");
    UT_ASSERT(XTime(XDateTuple(1, 1, 1), XTimeTuple(0, 0, 0), XFracSec(HNSECS, 1), XLocalTime::Instance()).ToCustomString("MM-YY/DD", "MM_SS_HH", "???", false) == "00_00_00???01-01/01");
    UT_ASSERT(XTime(XDateTuple(1, 1, 1), XTimeTuple(0, 0, 0), XFracSec(HNSECS, 1), XUTC::Instance()).ToCustomString("MM-YY/DD", "MM_SS_HH", "???", false) == "00_00_00???01-01/01");

    UT_ASSERT(XTime(XDateTuple(9, 12, 4), XTimeTuple(0, 0, 0)).ToCustomString("MM-YY/DD", "MM_SS_HH", "???", false) == "00_00_00???12-09/04");
    UT_ASSERT(XTime(XDateTuple(99, 12, 4), XTimeTuple(5, 6, 12)).ToCustomString("MM-YY/DD", "MM_SS_HH", "???", false) == "06_12_05???12-99/04");
    UT_ASSERT(XTime(XDateTuple(999, 12, 4), XTimeTuple(13, 44, 59)).ToCustomString("MM-YY/DD", "MM_SS_HH", "???", false) == "44_59_13???12-99/04");
    UT_ASSERT(XTime(XDateTuple(9999, 7, 4), XTimeTuple(23, 59, 59)).ToCustomString("MM-YY/DD", "MM_SS_HH", "???", false) == "59_59_23???07-99/04");

    UT_ASSERT(XTime(XDateTuple(9, 12, 4), XTimeTuple(0, 0, 0), XFracSec(MSECS, 42)).ToCustomString("MM-YY/DD", "MM_SS_HH", "???", false) == "00_00_00???12-09/04");
    UT_ASSERT(XTime(XDateTuple(99, 12, 4), XTimeTuple(5, 6, 12), XFracSec(MSECS, 100)).ToCustomString("MM-YY/DD", "MM_SS_HH", "???", false) == "06_12_05???12-99/04");
    UT_ASSERT(XTime(XDateTuple(999, 12, 4), XTimeTuple(13, 44, 59), XFracSec(USECS, 45020)).ToCustomString("MM-YY/DD", "MM_SS_HH", "???", false) == "44_59_13???12-99/04");
    UT_ASSERT(XTime(XDateTuple(9999, 7, 4), XTimeTuple(23, 59, 59), XFracSec(HNSECS, 12)).ToCustomString("MM-YY/DD", "MM_SS_HH", "???", false) == "59_59_23???07-99/04");

    UT_ASSERT(XTime(XDateTuple(2012, 12, 21), XTimeTuple(13, 13, 13), new XSimpleTimeZone(-5 * 60, 0)).ToCustomString("MM-YY/DD", "MM_SS_HH", "???", false) == "13_13_13???12-12/21");
    UT_ASSERT(XTime(XDateTuple(2012, 12, 21), XTimeTuple(13, 13, 13), new XSimpleTimeZone(-5 * 60, 30)).ToCustomString("MM-YY/DD", "MM_SS_HH", "???", false) == "13_13_13???12-12/21");

    UT_ASSERT(XTime(XDateTuple(2012, 12, 21), XTimeTuple(13, 13, 13), new XSimpleTimeZone(5 * 60, 0)).ToCustomString("MM-YY/DD", "MM_SS_HH", "???", false) == "13_13_13???12-12/21");
    UT_ASSERT(XTime(XDateTuple(2012, 12, 21), XTimeTuple(13, 13, 13), new XSimpleTimeZone(5 * 60, 30)).ToCustomString("MM-YY/DD", "MM_SS_HH", "???", false) == "13_13_13???12-12/21");


    UT_ASSERT_THROWS(XTime(XDateTuple(10000, 10, 20), XTimeTuple(1, 1, 1)).ToCustomString("YYYY-MM-DD", "HH:MM:SS", "T", true, 7), XException);
    UT_ASSERT_THROWS(XTime(XDateTuple(10000, 10, 20), XTimeTuple(1, 1, 1), XFracSec(HNSECS, 507890)).ToCustomString("YYYY-MM-DD", "HH:MM:SS", "T", true, 7), XException);

    //Verify that time zone conversion does not result in stuff blowing up for
    //default constructed XTimes.
    {
        XTime xt;
        xt._ticks = 0;
        UT_ASSERT(xt.ToCustomString("YYYY-MM-DD", "HH:MM:SS", " ", true, 7) == "0001-01-01 00:00:00");
        xt._ticks = -1;
        UT_ASSERT(xt.ToCustomString("YYYY-MM-DD", "HH:MM:SS", " ", true, 7) == "0001-01-01 00:00:00");
    }
}


void XTimeTest::TestToXML()
{
    UT_ASSERT(XTime(0, XLocalTime::Instance()).ToXML() == "<XTime ticks=\"0\"><XLocalTime></XLocalTime></XTime>");
    UT_ASSERT(XTime(123567890, XLocalTime::Instance()).ToXML() == "<XTime ticks=\"123567890\"><XLocalTime></XLocalTime></XTime>");

    UT_ASSERT(XTime(0, XUTC::Instance()).ToXML() == "<XTime ticks=\"0\"><XUTC></XUTC></XTime>");
    UT_ASSERT(XTime(123567890, XUTC::Instance()).ToXML() == "<XTime ticks=\"123567890\"><XUTC></XUTC></XTime>");

    UT_ASSERT(XTime(0, new XSimpleTimeZone(0, 0)).ToXML() == "<XTime ticks=\"0\"><XSimpleTimeZone utcOffset=\"0\" dstOffset=\"0\"></XSimpleTimeZone></XTime>");
    UT_ASSERT(XTime(0, new XSimpleTimeZone(480, 0)).ToXML() == "<XTime ticks=\"0\"><XSimpleTimeZone utcOffset=\"480\" dstOffset=\"0\"></XSimpleTimeZone></XTime>");
    UT_ASSERT(XTime(0, new XSimpleTimeZone(-480, 0)).ToXML() == "<XTime ticks=\"0\"><XSimpleTimeZone utcOffset=\"-480\" dstOffset=\"0\"></XSimpleTimeZone></XTime>");
    UT_ASSERT(XTime(0, new XSimpleTimeZone(0, 60)).ToXML() == "<XTime ticks=\"0\"><XSimpleTimeZone utcOffset=\"0\" dstOffset=\"60\"></XSimpleTimeZone></XTime>");
    UT_ASSERT(XTime(0, new XSimpleTimeZone(480, 60)).ToXML() == "<XTime ticks=\"0\"><XSimpleTimeZone utcOffset=\"480\" dstOffset=\"60\"></XSimpleTimeZone></XTime>");
    UT_ASSERT(XTime(0, new XSimpleTimeZone(-480, 60)).ToXML() == "<XTime ticks=\"0\"><XSimpleTimeZone utcOffset=\"-480\" dstOffset=\"60\"></XSimpleTimeZone></XTime>");

    UT_ASSERT(XTime(1234567890, new XSimpleTimeZone(0, 0)).ToXML() == "<XTime ticks=\"1234567890\"><XSimpleTimeZone utcOffset=\"0\" dstOffset=\"0\"></XSimpleTimeZone></XTime>");
    UT_ASSERT(XTime(1234567890, new XSimpleTimeZone(480, 0)).ToXML() == "<XTime ticks=\"1234567890\"><XSimpleTimeZone utcOffset=\"480\" dstOffset=\"0\"></XSimpleTimeZone></XTime>");
    UT_ASSERT(XTime(1234567890, new XSimpleTimeZone(-480, 0)).ToXML() == "<XTime ticks=\"1234567890\"><XSimpleTimeZone utcOffset=\"-480\" dstOffset=\"0\"></XSimpleTimeZone></XTime>");
    UT_ASSERT(XTime(1234567890, new XSimpleTimeZone(0, 60)).ToXML() == "<XTime ticks=\"1234567890\"><XSimpleTimeZone utcOffset=\"0\" dstOffset=\"60\"></XSimpleTimeZone></XTime>");
    UT_ASSERT(XTime(1234567890, new XSimpleTimeZone(480, 60)).ToXML() == "<XTime ticks=\"1234567890\"><XSimpleTimeZone utcOffset=\"480\" dstOffset=\"60\"></XSimpleTimeZone></XTime>");
    UT_ASSERT(XTime(1234567890, new XSimpleTimeZone(-480, 60)).ToXML() == "<XTime ticks=\"1234567890\"><XSimpleTimeZone utcOffset=\"-480\" dstOffset=\"60\"></XSimpleTimeZone></XTime>");
}


void XTimeTest::TestFromISOExtString()
{
    UT_ASSERT_THROWS(XTime::FromISOExtString(""), XException);
    UT_ASSERT_THROWS(XTime::FromISOExtString("20100704000000"), XException);
    UT_ASSERT_THROWS(XTime::FromISOExtString("20100704 000000"), XException);
    UT_ASSERT_THROWS(XTime::FromISOExtString("20100704t000000"), XException);
    UT_ASSERT_THROWS(XTime::FromISOExtString("20100704T000000."), XException);
    UT_ASSERT_THROWS(XTime::FromISOExtString("20100704T000000.0"), XException);

    UT_ASSERT_THROWS(XTime::FromISOExtString("2010-07:0400:00:00"), XException);
    UT_ASSERT_THROWS(XTime::FromISOExtString("2010-07-04 00:00:00"), XException);
    UT_ASSERT_THROWS(XTime::FromISOExtString("2010-07-04 00:00:00"), XException);
    UT_ASSERT_THROWS(XTime::FromISOExtString("2010-07-04t00:00:00"), XException);
    UT_ASSERT_THROWS(XTime::FromISOExtString("2010-07-04T00:00:00."), XException);
    UT_ASSERT_THROWS(XTime::FromISOExtString("2010-07-04T00:00:00.A"), XException);
    UT_ASSERT_THROWS(XTime::FromISOExtString("2010-07-04T00:00:00.Z"), XException);
    UT_ASSERT_THROWS(XTime::FromISOExtString("2010-07-04T00:00:00.00000000"), XException);
    UT_ASSERT_THROWS(XTime::FromISOExtString("2010-07-04T00:00:00.00000000"), XException);
    UT_ASSERT_THROWS(XTime::FromISOExtString("2010-07-04T00:00:00+"), XException);
    UT_ASSERT_THROWS(XTime::FromISOExtString("2010-07-04T00:00:00-"), XException);
    UT_ASSERT_THROWS(XTime::FromISOExtString("2010-07-04T00:00:00:"), XException);
    UT_ASSERT_THROWS(XTime::FromISOExtString("2010-07-04T00:00:00-:"), XException);
    UT_ASSERT_THROWS(XTime::FromISOExtString("2010-07-04T00:00:00+:"), XException);
    UT_ASSERT_THROWS(XTime::FromISOExtString("2010-07-04T00:00:00-1:"), XException);
    UT_ASSERT_THROWS(XTime::FromISOExtString("2010-07-04T00:00:00+1:"), XException);
    UT_ASSERT_THROWS(XTime::FromISOExtString("2010-07-04T00:00:00+1:0"), XException);
    UT_ASSERT_THROWS(XTime::FromISOExtString("2010-07-04T00:00:00-24.00"), XException);
    UT_ASSERT_THROWS(XTime::FromISOExtString("2010-07-04T00:00:00+24.00"), XException);
    UT_ASSERT_THROWS(XTime::FromISOExtString("2010-07-04T00:00:00:"), XException);
    UT_ASSERT_THROWS(XTime::FromISOExtString("2010-07-04T00:00:00:1000000F10000000"), XException);
    UT_ASSERT_THROWS(XTime::FromISOExtString("2010-07-04T00:00:00:1F10000000"), XException);
    UT_ASSERT_THROWS(XTime::FromISOExtString("2010-07-04T00:00:00:1F10"), XException);

    UT_ASSERT_THROWS(XTime::FromISOExtString("2010-Jul-0400:00:00"), XException);
    UT_ASSERT_THROWS(XTime::FromISOExtString("2010-Jul-04t00:00:00"), XException);
    UT_ASSERT_THROWS(XTime::FromISOExtString("2010-Jul-04 00:00:00."), XException);
    UT_ASSERT_THROWS(XTime::FromISOExtString("2010-Jul-04 00:00:00.0"), XException);

    UT_ASSERT_THROWS(XTime::FromISOExtString("20101222T172201"), XException);
    UT_ASSERT_THROWS(XTime::FromISOExtString("2010-Dec-22 17:22:01"), XException);

    UT_ASSERT(XTime::FromISOExtString("2010-12-22T17:22:01") == XTime(XDateTuple(2010, 12, 22), XTimeTuple(17, 22, 01)));
    UT_ASSERT(XTime::FromISOExtString("1999-07-06T12:30:33") == XTime(XDateTuple(1999, 7, 6), XTimeTuple(12, 30, 33)));
    UT_ASSERT(XTime::FromISOExtString("+01999-07-06T12:30:33") == XTime(XDateTuple(1999, 7, 6), XTimeTuple(12, 30, 33)));
    UT_ASSERT(XTime::FromISOExtString("1999-07-06T12:30:33 ") == XTime(XDateTuple(1999, 7, 6), XTimeTuple(12, 30, 33)));
    UT_ASSERT(XTime::FromISOExtString(" 1999-07-06T12:30:33") == XTime(XDateTuple(1999, 7, 6), XTimeTuple(12, 30, 33)));
    UT_ASSERT(XTime::FromISOExtString(" 1999-07-06T12:30:33 ") == XTime(XDateTuple(1999, 7, 6), XTimeTuple(12, 30, 33)));

    UT_ASSERT(XTime::FromISOExtString("1907-07-07T12:12:12.0") == XTime(XDateTuple(1907, 07, 07), XTimeTuple(12, 12, 12)));
    UT_ASSERT(XTime::FromISOExtString("1907-07-07T12:12:12.0000000") == XTime(XDateTuple(1907, 07, 07), XTimeTuple(12, 12, 12)));
    UT_ASSERT(XTime::FromISOExtString("1907-07-07T12:12:12.0000001") == XTime(XDateTuple(1907, 07, 07), XTimeTuple(12, 12, 12), XFracSec(HNSECS, 1)));
    UT_ASSERT(XTime::FromISOExtString("1907-07-07T12:12:12.000001") == XTime(XDateTuple(1907, 07, 07), XTimeTuple(12, 12, 12), XFracSec(USECS, 1)));
    UT_ASSERT(XTime::FromISOExtString("1907-07-07T12:12:12.0000010") == XTime(XDateTuple(1907, 07, 07), XTimeTuple(12, 12, 12), XFracSec(USECS, 1)));
    UT_ASSERT(XTime::FromISOExtString("1907-07-07T12:12:12.001") == XTime(XDateTuple(1907, 07, 07), XTimeTuple(12, 12, 12), XFracSec(MSECS, 1)));
    UT_ASSERT(XTime::FromISOExtString("1907-07-07T12:12:12.0010000") == XTime(XDateTuple(1907, 07, 07), XTimeTuple(12, 12, 12), XFracSec(MSECS, 1)));

    UT_ASSERT(XTime::FromISOExtString("2010-12-22T17:22:01Z") == XTime(XDateTuple(2010, 12, 22), XTimeTuple(17, 22, 01), XUTC::Instance()));
    UT_ASSERT(XTime::FromISOExtString("2010-12-22T17:22:01-1:00") == XTime(XDateTuple(2010, 12, 22), XTimeTuple(17, 22, 01), new XSimpleTimeZone(-60, 0)));
    UT_ASSERT(XTime::FromISOExtString("2010-12-22T17:22:01-1") == XTime(XDateTuple(2010, 12, 22), XTimeTuple(17, 22, 01), new XSimpleTimeZone(-60, 0)));
    UT_ASSERT(XTime::FromISOExtString("2010-12-22T17:22:01-1:30") == XTime(XDateTuple(2010, 12, 22), XTimeTuple(17, 22, 01), new XSimpleTimeZone(-90, 0)));
    UT_ASSERT(XTime::FromISOExtString("2010-12-22T17:22:01-8:00") == XTime(XDateTuple(2010, 12, 22), XTimeTuple(17, 22, 01), new XSimpleTimeZone(-480, 0)));
    UT_ASSERT(XTime::FromISOExtString("2010-12-22T17:22:01+1:00") == XTime(XDateTuple(2010, 12, 22), XTimeTuple(17, 22, 01), new XSimpleTimeZone(60, 0)));
    UT_ASSERT(XTime::FromISOExtString("2010-12-22T17:22:01+1") == XTime(XDateTuple(2010, 12, 22), XTimeTuple(17, 22, 01), new XSimpleTimeZone(60, 0)));
    UT_ASSERT(XTime::FromISOExtString("2010-12-22T17:22:01+1:30") == XTime(XDateTuple(2010, 12, 22), XTimeTuple(17, 22, 01), new XSimpleTimeZone(90, 0)));
    UT_ASSERT(XTime::FromISOExtString("2010-12-22T17:22:01+8:00") == XTime(XDateTuple(2010, 12, 22), XTimeTuple(17, 22, 01), new XSimpleTimeZone(480, 0)));

    UT_ASSERT(XTime::FromISOExtString("2010-11-03T06:51:06.57159Z") == XTime(XDateTuple(2010, 11, 3), XTimeTuple(6, 51, 6), XFracSec(HNSECS, 5715900), XUTC::Instance()));

    UT_ASSERT(XTime::FromISOExtString("2010-12-22T17:22:01.23412Z") == XTime(XDateTuple(2010, 12, 22), XTimeTuple(17, 22, 01), XFracSec(HNSECS, 2341200), XUTC::Instance()));
    UT_ASSERT(XTime::FromISOExtString("2010-12-22T17:22:01.23112-1:00") == XTime(XDateTuple(2010, 12, 22), XTimeTuple(17, 22, 01), XFracSec(HNSECS, 2311200), new XSimpleTimeZone(-60, 0)));
    UT_ASSERT(XTime::FromISOExtString("2010-12-22T17:22:01.45-1") == XTime(XDateTuple(2010, 12, 22), XTimeTuple(17, 22, 01), XFracSec(HNSECS, 4500000), new XSimpleTimeZone(-60, 0)));
    UT_ASSERT(XTime::FromISOExtString("2010-12-22T17:22:01.1-1:30") == XTime(XDateTuple(2010, 12, 22), XTimeTuple(17, 22, 01), XFracSec(HNSECS, 1000000), new XSimpleTimeZone(-90, 0)));
    UT_ASSERT(XTime::FromISOExtString("2010-12-22T17:22:01.55-8:00") == XTime(XDateTuple(2010, 12, 22), XTimeTuple(17, 22, 01), XFracSec(HNSECS, 5500000), new XSimpleTimeZone(-480, 0)));
    UT_ASSERT(XTime::FromISOExtString("2010-12-22T17:22:01.1234567+1:00") == XTime(XDateTuple(2010, 12, 22), XTimeTuple(17, 22, 01), XFracSec(HNSECS, 1234567), new XSimpleTimeZone(60, 0)));
    UT_ASSERT(XTime::FromISOExtString("2010-12-22T17:22:01.0+1") == XTime(XDateTuple(2010, 12, 22), XTimeTuple(17, 22, 01), XFracSec(HNSECS, 0), new XSimpleTimeZone(60, 0)));
    UT_ASSERT(XTime::FromISOExtString("2010-12-22T17:22:01.0000000+1:30") == XTime(XDateTuple(2010, 12, 22), XTimeTuple(17, 22, 01), XFracSec(HNSECS, 0), new XSimpleTimeZone(90, 0)));
    UT_ASSERT(XTime::FromISOExtString("2010-12-22T17:22:01.45+8:00") == XTime(XDateTuple(2010, 12, 22), XTimeTuple(17, 22, 01), XFracSec(HNSECS, 4500000), new XSimpleTimeZone(480, 0)));
}


void XTimeTest::TestFromMPEG7String()
{
    UT_ASSERT_THROWS(XTime::FromMPEG7String(""), XException);
    UT_ASSERT_THROWS(XTime::FromMPEG7String("20100704000000"), XException);
    UT_ASSERT_THROWS(XTime::FromMPEG7String("20100704 000000"), XException);
    UT_ASSERT_THROWS(XTime::FromMPEG7String("20100704t000000"), XException);
    UT_ASSERT_THROWS(XTime::FromMPEG7String("20100704T000000."), XException);
    UT_ASSERT_THROWS(XTime::FromMPEG7String("20100704T000000.0"), XException);

    UT_ASSERT_THROWS(XTime::FromMPEG7String("2010-07:0400:00:00"), XException);
    UT_ASSERT_THROWS(XTime::FromMPEG7String("2010-07-04 00:00:00"), XException);
    UT_ASSERT_THROWS(XTime::FromMPEG7String("2010-07-04 00:00:00"), XException);
    UT_ASSERT_THROWS(XTime::FromMPEG7String("2010-07-04t00:00:00"), XException);
    UT_ASSERT_THROWS(XTime::FromMPEG7String("2010-07-04T00:00:00."), XException);
    UT_ASSERT_THROWS(XTime::FromMPEG7String("2010-07-04T00:00:00.A"), XException);
    UT_ASSERT_THROWS(XTime::FromMPEG7String("2010-07-04T00:00:00.Z"), XException);
    UT_ASSERT_THROWS(XTime::FromMPEG7String("2010-07-04T00:00:00.00000000"), XException);
    UT_ASSERT_THROWS(XTime::FromMPEG7String("2010-07-04T00:00:00.00000000"), XException);
    UT_ASSERT_THROWS(XTime::FromMPEG7String("2010-07-04T00:00:00+"), XException);
    UT_ASSERT_THROWS(XTime::FromMPEG7String("2010-07-04T00:00:00-"), XException);
    UT_ASSERT_THROWS(XTime::FromMPEG7String("2010-07-04T00:00:00:"), XException);
    UT_ASSERT_THROWS(XTime::FromMPEG7String("2010-07-04T00:00:00-:"), XException);
    UT_ASSERT_THROWS(XTime::FromMPEG7String("2010-07-04T00:00:00+:"), XException);
    UT_ASSERT_THROWS(XTime::FromMPEG7String("2010-07-04T00:00:00-1:"), XException);
    UT_ASSERT_THROWS(XTime::FromMPEG7String("2010-07-04T00:00:00+1:"), XException);
    UT_ASSERT_THROWS(XTime::FromMPEG7String("2010-07-04T00:00:00+1:0"), XException);
    UT_ASSERT_THROWS(XTime::FromMPEG7String("2010-07-04T00:00:00-24.00"), XException);
    UT_ASSERT_THROWS(XTime::FromMPEG7String("2010-07-04T00:00:00+24.00"), XException);
    UT_ASSERT_THROWS(XTime::FromMPEG7String("2010-07-04T00:00:00:"), XException);
    UT_ASSERT_THROWS(XTime::FromMPEG7String("2010-07-04T00:00:00:F"), XException);
    UT_ASSERT_THROWS(XTime::FromMPEG7String("2010-07-04T00:00:00:F10"), XException);
    UT_ASSERT_THROWS(XTime::FromMPEG7String("2010-07-04T00:00:00:1F1"), XException);
    UT_ASSERT_THROWS(XTime::FromMPEG7String("2010-07-04T00:00:00:10000000F10000000"), XException);
    UT_ASSERT_THROWS(XTime::FromMPEG7String("2010-07-04T00:00:00:100F10"), XException);

    UT_ASSERT_THROWS(XTime::FromMPEG7String("2010-Jul-0400:00:00"), XException);
    UT_ASSERT_THROWS(XTime::FromMPEG7String("2010-Jul-04t00:00:00"), XException);
    UT_ASSERT_THROWS(XTime::FromMPEG7String("2010-Jul-04 00:00:00."), XException);
    UT_ASSERT_THROWS(XTime::FromMPEG7String("2010-Jul-04 00:00:00.0"), XException);

    UT_ASSERT_THROWS(XTime::FromMPEG7String("20101222T172201"), XException);
    UT_ASSERT_THROWS(XTime::FromMPEG7String("2010-Dec-22 17:22:01"), XException);

    UT_ASSERT_THROWS(XTime::FromMPEG7String("1907-07-07T12:12:12.0"), XException);
    UT_ASSERT_THROWS(XTime::FromMPEG7String("1907-07-07T12:12:12.0000000"), XException);
    UT_ASSERT_THROWS(XTime::FromMPEG7String("1907-07-07T12:12:12.0000001"), XException);
    UT_ASSERT_THROWS(XTime::FromMPEG7String("1907-07-07T12:12:12.000001"), XException);
    UT_ASSERT_THROWS(XTime::FromMPEG7String("1907-07-07T12:12:12.0000010"), XException);
    UT_ASSERT_THROWS(XTime::FromMPEG7String("1907-07-07T12:12:12.001"), XException);
    UT_ASSERT_THROWS(XTime::FromMPEG7String("1907-07-07T12:12:12.0010000"), XException);

    UT_ASSERT_THROWS(XTime::FromMPEG7String("2010-12-22T17:22:01Z"), XException);
    UT_ASSERT_THROWS(XTime::FromMPEG7String("2010-12-22T17:22:01-1:00"), XException);
    UT_ASSERT_THROWS(XTime::FromMPEG7String("2010-12-22T17:22:01-1"), XException);
    UT_ASSERT_THROWS(XTime::FromMPEG7String("2010-12-22T17:22:01-1:30"), XException);
    UT_ASSERT_THROWS(XTime::FromMPEG7String("2010-12-22T17:22:01-8:00"), XException);
    UT_ASSERT_THROWS(XTime::FromMPEG7String("2010-12-22T17:22:01+1:00"), XException);
    UT_ASSERT_THROWS(XTime::FromMPEG7String("2010-12-22T17:22:01+1"), XException);
    UT_ASSERT_THROWS(XTime::FromMPEG7String("2010-12-22T17:22:01+1:30"), XException);
    UT_ASSERT_THROWS(XTime::FromMPEG7String("2010-12-22T17:22:01+8:00"), XException);

    UT_ASSERT_THROWS(XTime::FromMPEG7String("2010-11-03T06:51:06.57159Z"), XException);

    UT_ASSERT(XTime::FromMPEG7String("2010-12-22T17:22:01") == XTime(XDateTuple(2010, 12, 22), XTimeTuple(17, 22, 01)));
    UT_ASSERT(XTime::FromMPEG7String("1999-07-06T12:30:33") == XTime(XDateTuple(1999, 7, 6), XTimeTuple(12, 30, 33)));
    UT_ASSERT(XTime::FromMPEG7String("+01999-07-06T12:30:33") == XTime(XDateTuple(1999, 7, 6), XTimeTuple(12, 30, 33)));
    UT_ASSERT(XTime::FromMPEG7String("1999-07-06T12:30:33 ") == XTime(XDateTuple(1999, 7, 6), XTimeTuple(12, 30, 33)));
    UT_ASSERT(XTime::FromMPEG7String(" 1999-07-06T12:30:33") == XTime(XDateTuple(1999, 7, 6), XTimeTuple(12, 30, 33)));
    UT_ASSERT(XTime::FromMPEG7String(" 1999-07-06T12:30:33 ") == XTime(XDateTuple(1999, 7, 6), XTimeTuple(12, 30, 33)));

    UT_ASSERT(XTime::FromMPEG7String("1907-07-07T12:12:12:0F10") == XTime(XDateTuple(1907, 07, 07), XTimeTuple(12, 12, 12)));
    UT_ASSERT(XTime::FromMPEG7String("1907-07-07T12:12:12:0000000F10000") == XTime(XDateTuple(1907, 07, 07), XTimeTuple(12, 12, 12)));
    UT_ASSERT(XTime::FromMPEG7String("1907-07-07T12:12:12:1F10000000") == XTime(XDateTuple(1907, 07, 07), XTimeTuple(12, 12, 12), XFracSec(HNSECS, 1)));
    UT_ASSERT(XTime::FromMPEG7String("1907-07-07T12:12:12:1F1000000") == XTime(XDateTuple(1907, 07, 07), XTimeTuple(12, 12, 12), XFracSec(USECS, 1)));
    UT_ASSERT(XTime::FromMPEG7String("1907-07-07T12:12:12:001F1000000") == XTime(XDateTuple(1907, 07, 07), XTimeTuple(12, 12, 12), XFracSec(USECS, 1)));
    UT_ASSERT(XTime::FromMPEG7String("1907-07-07T12:12:12:1F1000") == XTime(XDateTuple(1907, 07, 07), XTimeTuple(12, 12, 12), XFracSec(MSECS, 1)));
    UT_ASSERT(XTime::FromMPEG7String("1907-07-07T12:12:12:0010F10000") == XTime(XDateTuple(1907, 07, 07), XTimeTuple(12, 12, 12), XFracSec(MSECS, 1)));

    UT_ASSERT(XTime::FromMPEG7String("1907-07-07T12:12:12:5F25") == XTime(XDateTuple(1907, 07, 07), XTimeTuple(12, 12, 12), XFracSec(MSECS, 200)));
}


void XTimeTest::TestFromXML()
{
    UT_ASSERT(XTime::FromXML("<XTime ticks=\"0\"><XLocalTime></XLocalTime></XTime>") == XTime(0, XLocalTime::Instance()));
    UT_ASSERT(XTime::FromXML("<XTime ticks=\"1234567890\"><XLocalTime></XLocalTime></XTime>") == XTime(1234567890, XLocalTime::Instance()));

    UT_ASSERT(XTime::FromXML("<XTime ticks=\"0\"><XUTC></XUTC></XTime>") == XTime(0, XUTC::Instance()));
    UT_ASSERT(XTime::FromXML("<XTime ticks=\"1234567890\"><XUTC></XUTC></XTime>") == XTime(1234567890, XUTC::Instance()));

    UT_ASSERT(XTime::FromXML("<XTime ticks=\"0\"><XSimpleTimeZone utcOffset=\"0\" dstOffset=\"0\"></XSimpleTimeZone></XTime>") == XTime(0, new XSimpleTimeZone(0, 0)));
    UT_ASSERT(XTime::FromXML("<XTime ticks=\"0\"><XSimpleTimeZone utcOffset=\"480\" dstOffset=\"0\"></XSimpleTimeZone></XTime>") == XTime(0, new XSimpleTimeZone(480, 0)));
    UT_ASSERT(XTime::FromXML("<XTime ticks=\"0\"><XSimpleTimeZone utcOffset=\"-480\" dstOffset=\"0\"></XSimpleTimeZone></XTime>") == XTime(0, new XSimpleTimeZone(-480, 0)));
    UT_ASSERT(XTime::FromXML("<XTime ticks=\"0\"><XSimpleTimeZone utcOffset=\"0\" dstOffset=\"60\"></XSimpleTimeZone></XTime>") == XTime(0, new XSimpleTimeZone(0, 60)));
    UT_ASSERT(XTime::FromXML("<XTime ticks=\"0\"><XSimpleTimeZone utcOffset=\"480\" dstOffset=\"60\"></XSimpleTimeZone></XTime>") == XTime(0, new XSimpleTimeZone(480, 60)));
    UT_ASSERT(XTime::FromXML("<XTime ticks=\"0\"><XSimpleTimeZone utcOffset=\"-480\" dstOffset=\"60\"></XSimpleTimeZone></XTime>") == XTime(0, new XSimpleTimeZone(-480, 60)));

    UT_ASSERT(XTime::FromXML("<XTime ticks=\"1234567890\"><XSimpleTimeZone utcOffset=\"0\" dstOffset=\"0\"></XSimpleTimeZone></XTime>") == XTime(1234567890, new XSimpleTimeZone(0, 0)));
    UT_ASSERT(XTime::FromXML("<XTime ticks=\"1234567890\"><XSimpleTimeZone utcOffset=\"480\" dstOffset=\"0\"></XSimpleTimeZone></XTime>") == XTime(1234567890, new XSimpleTimeZone(480, 0)));
    UT_ASSERT(XTime::FromXML("<XTime ticks=\"1234567890\"><XSimpleTimeZone utcOffset=\"-480\" dstOffset=\"0\"></XSimpleTimeZone></XTime>") == XTime(1234567890, new XSimpleTimeZone(-480, 0)));
    UT_ASSERT(XTime::FromXML("<XTime ticks=\"1234567890\"><XSimpleTimeZone utcOffset=\"0\" dstOffset=\"60\"></XSimpleTimeZone></XTime>") == XTime(1234567890, new XSimpleTimeZone(0, 60)));
    UT_ASSERT(XTime::FromXML("<XTime ticks=\"1234567890\"><XSimpleTimeZone utcOffset=\"480\" dstOffset=\"60\"></XSimpleTimeZone></XTime>") == XTime(1234567890, new XSimpleTimeZone(480, 60)));
    UT_ASSERT(XTime::FromXML("<XTime ticks=\"1234567890\"><XSimpleTimeZone utcOffset=\"-480\" dstOffset=\"60\"></XSimpleTimeZone></XTime>") == XTime(1234567890, new XSimpleTimeZone(-480, 60)));
}

