
#include "XDurationTest.h"

#include "XSDK/XDuration.h"

#include <iostream>
using namespace std;

using namespace XSDK;

REGISTER_TEST_FIXTURE(XFracSecTest);
REGISTER_TEST_FIXTURE(XDurationTest);

const int signVals[2] = {1, -1};

void XFracSecTest::TestConvert()
{
    for(size_t i = 0; i < 2; ++i)
    {
        const int sign = signVals[i];

        UT_ASSERT(convert(WEEKS, HNSECS, 1 * sign) == 6048000000000LL * sign);
        UT_ASSERT(convert(DAYS, HNSECS, 1 * sign) == 864000000000LL * sign);
        UT_ASSERT(convert(HOURS, HNSECS, 1 * sign) == 36000000000LL * sign);
        UT_ASSERT(convert(MINUTES, HNSECS, 1 * sign) == 600000000LL * sign);
        UT_ASSERT(convert(SECONDS, HNSECS, 1 * sign) == 10000000LL * sign);
        UT_ASSERT(convert(MSECS, HNSECS, 1 * sign) == 10000 * sign);
        UT_ASSERT(convert(USECS, HNSECS, 1 * sign) == 10 * sign);

        UT_ASSERT(convert(HNSECS, WEEKS, 6048000000000LL * sign) == 1 * sign);
        UT_ASSERT(convert(HNSECS, DAYS, 864000000000LL * sign) == 1 * sign);
        UT_ASSERT(convert(HNSECS, HOURS, 36000000000LL * sign) == 1 * sign);
        UT_ASSERT(convert(HNSECS, MINUTES, 600000000LL * sign) == 1 * sign);
        UT_ASSERT(convert(HNSECS, SECONDS, 10000000LL * sign) == 1 * sign);
        UT_ASSERT(convert(HNSECS, MSECS, 10000 * sign) == 1 * sign);
        UT_ASSERT(convert(HNSECS, USECS, 10 * sign) == 1 * sign);

        UT_ASSERT(convert(WEEKS, WEEKS, 12 * sign) == 12 * sign);
        UT_ASSERT(convert(DAYS, DAYS, 12 * sign) == 12 * sign);
        UT_ASSERT(convert(HOURS, HOURS, 12 * sign) == 12 * sign);
        UT_ASSERT(convert(MINUTES, MINUTES, 12 * sign) == 12 * sign);
        UT_ASSERT(convert(SECONDS, SECONDS, 12 * sign) == 12 * sign);
        UT_ASSERT(convert(MSECS, MSECS, 12 * sign) == 12 * sign);
        UT_ASSERT(convert(USECS, USECS, 12 * sign) == 12 * sign);
        UT_ASSERT(convert(HNSECS, HNSECS, 12 * sign) == 12 * sign);

        UT_ASSERT(convert(WEEKS, DAYS, 1 * sign) == 7 * sign);
        UT_ASSERT(convert(DAYS, WEEKS, 7 * sign) == 1 * sign);

        UT_ASSERT(convert(DAYS, HOURS, 1 * sign) == 24 * sign);
        UT_ASSERT(convert(HOURS, DAYS, 24 * sign) == 1 * sign);

        UT_ASSERT(convert(HOURS, MINUTES, 1 * sign) == 60 * sign);
        UT_ASSERT(convert(MINUTES, HOURS, 60 * sign) == 1 * sign);

        UT_ASSERT(convert(MINUTES, SECONDS, 1 * sign) == 60 * sign);
        UT_ASSERT(convert(SECONDS, MINUTES, 60 * sign) == 1 * sign);

        UT_ASSERT(convert(SECONDS, MSECS, 1 * sign) == 1000 * sign);
        UT_ASSERT(convert(MSECS, SECONDS, 1000 * sign) == 1 * sign);

        UT_ASSERT(convert(MSECS, USECS, 1 * sign) == 1000 * sign);
        UT_ASSERT(convert(USECS, MSECS, 1000 * sign) == 1 * sign);

        UT_ASSERT(convert(USECS, HNSECS, 1 * sign) == 10 * sign);
        UT_ASSERT(convert(HNSECS, USECS, 10 * sign) == 1 * sign);

        UT_ASSERT(convert(HNSECS, HNSECS, 10 * sign) == 10 * sign);
        UT_ASSERT(convert(HNSECS, HNSECS, 10 * sign) == 10 * sign);

        UT_ASSERT(convert(WEEKS, NSECS, 1 * sign) == 604800000000000LL * sign);
        UT_ASSERT(convert(DAYS, NSECS, 1 * sign) == 86400000000000LL * sign);
        UT_ASSERT(convert(HOURS, NSECS, 1 * sign) == 3600000000000LL * sign);
        UT_ASSERT(convert(MINUTES, NSECS, 1 * sign) == 60000000000LL * sign);
        UT_ASSERT(convert(SECONDS, NSECS, 1 * sign) == 1000000000LL * sign);
        UT_ASSERT(convert(MSECS, NSECS, 1 * sign) == 1000000 * sign);
        UT_ASSERT(convert(USECS, NSECS, 1 * sign) == 1000 * sign);
        UT_ASSERT(convert(HNSECS, NSECS, 1 * sign) == 100 * sign);

        UT_ASSERT(convert(NSECS, WEEKS, 604800000000000LL * sign) == 1 * sign);
        UT_ASSERT(convert(NSECS, DAYS, 86400000000000LL * sign) == 1 * sign);
        UT_ASSERT(convert(NSECS, HOURS, 3600000000000LL * sign) == 1 * sign);
        UT_ASSERT(convert(NSECS, MINUTES, 60000000000LL * sign) == 1 * sign);
        UT_ASSERT(convert(NSECS, SECONDS, 1000000000LL * sign) == 1 * sign);
        UT_ASSERT(convert(NSECS, MSECS, 1000000 * sign) == 1 * sign);
        UT_ASSERT(convert(NSECS, USECS, 1000 * sign) == 1 * sign);
        UT_ASSERT(convert(NSECS, HNSECS, 100 * sign) == 1 * sign);

        UT_ASSERT(convert(NSECS, NSECS, 1 * sign) == 1 * sign);

        //Verify Examples
        UT_ASSERT(convert(WEEKS, DAYS, 1 * sign) == 7 * sign);
        UT_ASSERT(convert(HOURS, SECONDS, 1 * sign) == 3600 * sign);
        UT_ASSERT(convert(SECONDS, DAYS, 1 * sign) == 0);
        UT_ASSERT(convert(SECONDS, DAYS, 86400 * sign) == 1 * sign);

        UT_ASSERT(convert(NSECS, NSECS, 1 * sign) == 1 * sign);
        UT_ASSERT(convert(NSECS, HNSECS, 1 * sign) == 0);
        UT_ASSERT(convert(HNSECS, NSECS, 1 * sign) == 100 * sign);
        UT_ASSERT(convert(NSECS, SECONDS, 1 * sign) == 0);
        UT_ASSERT(convert(SECONDS, NSECS, 1 * sign) == 1000000000 * sign);
    }
}


void XFracSecTest::TestConstructor()
{
    UT_ASSERT(XFracSec(0)._hnsecs == 0);

    for(size_t i = 0; i < 2; ++i)
    {
        const int sign = signVals[i];

        UT_ASSERT(XFracSec(1 * sign)._hnsecs == 1 * sign);
        UT_ASSERT(XFracSec(42 * sign)._hnsecs == 42 * sign);
        UT_ASSERT(XFracSec(12007 * sign)._hnsecs == 12007 * sign);
    }

    UT_ASSERT(XFracSec(MSECS, 0) == XFracSec(0));
    UT_ASSERT(XFracSec(USECS, 0) == XFracSec(0));
    UT_ASSERT(XFracSec(HNSECS, 0) == XFracSec(0));
    UT_ASSERT(XFracSec(NSECS, 0) == XFracSec(0));

    for(size_t i = 0; i < 2; ++i)
    {
        const int sign = signVals[i];

        UT_ASSERT(XFracSec(MSECS, 1 * sign) == XFracSec(10000 * sign));
        UT_ASSERT(XFracSec(MSECS, 999 * sign) == XFracSec(9990000 * sign));

        UT_ASSERT(XFracSec(USECS, 1 * sign) == XFracSec(10 * sign));
        UT_ASSERT(XFracSec(USECS, 999 * sign) == XFracSec(9990 * sign));
        UT_ASSERT(XFracSec(USECS, 999999 * sign) == XFracSec(9999990 * sign));

        UT_ASSERT(XFracSec(HNSECS, 1 * sign) == XFracSec(1 * sign));
        UT_ASSERT(XFracSec(HNSECS, 999 * sign) == XFracSec(999 * sign));
        UT_ASSERT(XFracSec(HNSECS, 999999 * sign) == XFracSec(999999 * sign));
        UT_ASSERT(XFracSec(HNSECS, 9999999 * sign) == XFracSec(9999999 * sign));

        UT_ASSERT(XFracSec(NSECS, 1 * sign) == XFracSec(0));
        UT_ASSERT(XFracSec(NSECS, 10 * sign) == XFracSec(0));
        UT_ASSERT(XFracSec(NSECS, 99 * sign) == XFracSec(0));
        UT_ASSERT(XFracSec(NSECS, 100 * sign) == XFracSec(1 * sign));
        UT_ASSERT(XFracSec(NSECS, 99999 * sign) == XFracSec(999 * sign));
        UT_ASSERT(XFracSec(NSECS, 99999999 * sign) == XFracSec(999999 * sign));
        UT_ASSERT(XFracSec(NSECS, 999999999 * sign) == XFracSec(9999999 * sign));

        UT_ASSERT_THROWS(XFracSec(HNSECS, 10000001 * sign), XException);
        UT_ASSERT_THROWS(XFracSec(USECS, 1000001 * sign), XException);
        UT_ASSERT_THROWS(XFracSec(MSECS, 1001 * sign), XException);
        UT_ASSERT_THROWS(XFracSec(SECONDS, 1 * sign), XException);
        UT_ASSERT_THROWS(XFracSec(MINUTES, 1 * sign), XException);
        UT_ASSERT_THROWS(XFracSec(HOURS, 1 * sign), XException);
        UT_ASSERT_THROWS(XFracSec(DAYS, 1 * sign), XException);
        UT_ASSERT_THROWS(XFracSec(WEEKS, 1 * sign), XException);
    }
}


void XFracSecTest::TestEquality()
{
    UT_ASSERT(XFracSec(42) == XFracSec(42));
    UT_ASSERT(!(XFracSec(42) != XFracSec(42)));
    UT_ASSERT(!(XFracSec(42) < XFracSec(42)));
    UT_ASSERT(XFracSec(42) <= XFracSec(42));
    UT_ASSERT(!(XFracSec(42) > XFracSec(42)));
    UT_ASSERT(XFracSec(42) >= XFracSec(42));

    UT_ASSERT(!(XFracSec(42) == XFracSec(100)));
    UT_ASSERT(XFracSec(42) != XFracSec(100));
    UT_ASSERT(XFracSec(42) < XFracSec(100));
    UT_ASSERT(XFracSec(42) <= XFracSec(100));
    UT_ASSERT(!(XFracSec(42) > XFracSec(100)));
    UT_ASSERT(!(XFracSec(42) >= XFracSec(100)));

    UT_ASSERT(!(XFracSec(100) == XFracSec(42)));
    UT_ASSERT(XFracSec(100) != XFracSec(42));
    UT_ASSERT(!(XFracSec(100) < XFracSec(42)));
    UT_ASSERT(!(XFracSec(100) <= XFracSec(42)));
    UT_ASSERT(XFracSec(100) > XFracSec(42));
    UT_ASSERT(XFracSec(100) >= XFracSec(42));

    UT_ASSERT(!(XFracSec(-42) == XFracSec(42)));
    UT_ASSERT(XFracSec(-42) != XFracSec(42));
    UT_ASSERT(XFracSec(-42) < XFracSec(42));
    UT_ASSERT(XFracSec(-42) <= XFracSec(42));
    UT_ASSERT(!(XFracSec(-42) > XFracSec(42)));
    UT_ASSERT(!(XFracSec(-42) >= XFracSec(42)));

    UT_ASSERT(XFracSec(-42) == XFracSec(-42));
    UT_ASSERT(!(XFracSec(-42) != XFracSec(-42)));
    UT_ASSERT(!(XFracSec(-42) < XFracSec(-42)));
    UT_ASSERT(XFracSec(-42) <= XFracSec(-42));
    UT_ASSERT(!(XFracSec(-42) > XFracSec(-42)));
    UT_ASSERT(XFracSec(-42) >= XFracSec(-42));

    UT_ASSERT(!(XFracSec(42) == XFracSec(-42)));
    UT_ASSERT(XFracSec(42) != XFracSec(-42));
    UT_ASSERT(!(XFracSec(42) < XFracSec(-42)));
    UT_ASSERT(!(XFracSec(42) <= XFracSec(-42)));
    UT_ASSERT(XFracSec(42) > XFracSec(-42));
    UT_ASSERT(XFracSec(42) >= XFracSec(-42));
}


void XFracSecTest::TestNegation()
{
    const int vals[5] = {-7, -5, 0, 5, 7};

    for(size_t i = 0; i < 5; ++i)
    {
        XFracSec fs = XFracSec(vals[i]);
        const XFracSec cfs = XFracSec(vals[i]);

        UT_ASSERT(-fs == XFracSec(-vals[i]));
        UT_ASSERT(-cfs == XFracSec(-vals[i]));
    }
}


void XFracSecTest::TestMSecs()
{
    UT_ASSERT(XFracSec(0).MSecs() == 0);

    for(size_t i = 0; i < 2; ++i)
    {
        const int sign = signVals[i];

        UT_ASSERT(XFracSec(1 * sign).MSecs() == 0);
        UT_ASSERT(XFracSec(999 * sign).MSecs() == 0);
        UT_ASSERT(XFracSec(999999 * sign).MSecs() == 99 * sign);

        XFracSec fs(42 * sign);

        fs.MSecs(0);
        UT_ASSERT(fs == XFracSec(0));

        fs.MSecs(1 * sign);
        UT_ASSERT(fs == XFracSec(10000 * sign));

        fs.MSecs(999 * sign);
        UT_ASSERT(fs == XFracSec(9990000 * sign));
    }
}


void XFracSecTest::TestUSecs()
{
    UT_ASSERT(XFracSec(0).USecs() == 0);

    for(size_t i = 0; i < 2; ++i)
    {
        const int sign = signVals[i];

        UT_ASSERT(XFracSec(1 * sign).USecs() == 0);
        UT_ASSERT(XFracSec(999 * sign).USecs() == 99 * sign);
        UT_ASSERT(XFracSec(999999 * sign).USecs() == 99999 * sign);
        UT_ASSERT(XFracSec(9999999 * sign).USecs() == 999999 * sign);

        XFracSec fs(42 * sign);

        fs.USecs(0);
        UT_ASSERT(fs == XFracSec(0));

        fs.USecs(1 * sign);
        UT_ASSERT(fs == XFracSec(10 * sign));

        fs.USecs(999 * sign);
        UT_ASSERT(fs == XFracSec(9990 * sign));

        fs.USecs(999999 * sign);
        UT_ASSERT(fs == XFracSec(9999990 * sign));
    }
}


void XFracSecTest::TestHNSecs()
{
    UT_ASSERT(XFracSec(0).HNSecs() == 0);

    for(size_t i = 0; i < 2; ++i)
    {
        const int sign = signVals[i];

        UT_ASSERT(XFracSec(1 * sign).HNSecs() == 1 * sign);
        UT_ASSERT(XFracSec(999 * sign).HNSecs() == 999 * sign);
        UT_ASSERT(XFracSec(999999 * sign).HNSecs() == 999999 * sign);
        UT_ASSERT(XFracSec(9999999 * sign).HNSecs() == 9999999 * sign);

        XFracSec fs(42 * sign);

        fs.HNSecs(0);
        UT_ASSERT(fs == XFracSec(0));

        fs.HNSecs(1 * sign);
        UT_ASSERT(fs == XFracSec(1 * sign));

        fs.HNSecs(999 * sign);
        UT_ASSERT(fs == XFracSec(999 * sign));

        fs.HNSecs(999999 * sign);
        UT_ASSERT(fs == XFracSec(999999 * sign));

        fs.HNSecs(9999999 * sign);
        UT_ASSERT(fs == XFracSec(9999999 * sign));
    }
}


void XFracSecTest::TestNSecs()
{
    UT_ASSERT(XFracSec(0).NSecs() == 0);

    for(size_t i = 0; i < 2; ++i)
    {
        const int sign = signVals[i];

        UT_ASSERT(XFracSec(1 * sign).NSecs() == 100 * sign);
        UT_ASSERT(XFracSec(999 * sign).NSecs() == 99900 * sign);
        UT_ASSERT(XFracSec(999999 * sign).NSecs() == 99999900LL * sign);
        UT_ASSERT(XFracSec(9999999LL * sign).NSecs() == 999999900LL * sign);

        XFracSec fs(42 * sign);

        fs.NSecs(0);
        UT_ASSERT(fs == XFracSec(0));

        fs.NSecs(1 * sign);
        UT_ASSERT(fs == XFracSec(0));

        fs.NSecs(10 * sign);
        UT_ASSERT(fs == XFracSec(0));

        fs.NSecs(100 * sign);
        UT_ASSERT(fs == XFracSec(1 * sign));

        fs.NSecs(999 * sign);
        UT_ASSERT(fs == XFracSec(9 * sign));

        fs.NSecs(999999 * sign);
        UT_ASSERT(fs == XFracSec(9999 * sign));

        fs.NSecs(9999999 * sign);
        UT_ASSERT(fs == XFracSec(99999 * sign));
    }
}


void XFracSecTest::TestToISOString()
{
    UT_ASSERT(XFracSec::ToISOString(0) == "");
    UT_ASSERT(XFracSec::ToISOString(1) == ".0000001");
    UT_ASSERT(XFracSec::ToISOString(10) == ".000001");
    UT_ASSERT(XFracSec::ToISOString(100) == ".00001");
    UT_ASSERT(XFracSec::ToISOString(1000) == ".0001");
    UT_ASSERT(XFracSec::ToISOString(10000) == ".001");
    UT_ASSERT(XFracSec::ToISOString(100000) == ".01");
    UT_ASSERT(XFracSec::ToISOString(1000000) == ".1");
    UT_ASSERT(XFracSec::ToISOString(1000001) == ".1000001");
    UT_ASSERT(XFracSec::ToISOString(1001001) == ".1001001");
    UT_ASSERT(XFracSec::ToISOString(1071601) == ".1071601");
    UT_ASSERT(XFracSec::ToISOString(1271641) == ".1271641");
    UT_ASSERT(XFracSec::ToISOString(9999999) == ".9999999");
    UT_ASSERT(XFracSec::ToISOString(9999990) == ".999999");
    UT_ASSERT(XFracSec::ToISOString(9999900) == ".99999");
    UT_ASSERT(XFracSec::ToISOString(9999000) == ".9999");
    UT_ASSERT(XFracSec::ToISOString(9990000) == ".999");
    UT_ASSERT(XFracSec::ToISOString(9900000) == ".99");
    UT_ASSERT(XFracSec::ToISOString(9000000) == ".9");
    UT_ASSERT(XFracSec::ToISOString(999) == ".0000999");
    UT_ASSERT(XFracSec::ToISOString(9990) == ".000999");
    UT_ASSERT(XFracSec::ToISOString(99900) == ".00999");
    UT_ASSERT(XFracSec::ToISOString(999000) == ".0999");
}


void XFracSecTest::TestToMPEG7String()
{
    UT_ASSERT(XFracSec::ToMPEG7String(0) == "");
    UT_ASSERT(XFracSec::ToMPEG7String(1) == ":1F10000000");
    UT_ASSERT(XFracSec::ToMPEG7String(10) == ":1F1000000");
    UT_ASSERT(XFracSec::ToMPEG7String(100) == ":1F100000");
    UT_ASSERT(XFracSec::ToMPEG7String(1000) == ":1F10000");
    UT_ASSERT(XFracSec::ToMPEG7String(10000) == ":1F1000");
    UT_ASSERT(XFracSec::ToMPEG7String(100000) == ":1F100");
    UT_ASSERT(XFracSec::ToMPEG7String(1000000) == ":1F10");
    UT_ASSERT(XFracSec::ToMPEG7String(1000001) == ":1000001F10000000");
    UT_ASSERT(XFracSec::ToMPEG7String(1001001) == ":1001001F10000000");
    UT_ASSERT(XFracSec::ToMPEG7String(1071601) == ":1071601F10000000");
    UT_ASSERT(XFracSec::ToMPEG7String(1271641) == ":1271641F10000000");
    UT_ASSERT(XFracSec::ToMPEG7String(9999999) == ":9999999F10000000");
    UT_ASSERT(XFracSec::ToMPEG7String(9999990) == ":999999F1000000");
    UT_ASSERT(XFracSec::ToMPEG7String(9999900) == ":99999F100000");
    UT_ASSERT(XFracSec::ToMPEG7String(9999000) == ":9999F10000");
    UT_ASSERT(XFracSec::ToMPEG7String(9990000) == ":999F1000");
    UT_ASSERT(XFracSec::ToMPEG7String(9900000) == ":99F100");
    UT_ASSERT(XFracSec::ToMPEG7String(9000000) == ":9F10");
    UT_ASSERT(XFracSec::ToMPEG7String(999) == ":999F10000000");
    UT_ASSERT(XFracSec::ToMPEG7String(9990) == ":999F1000000");
    UT_ASSERT(XFracSec::ToMPEG7String(99900) == ":999F100000");
    UT_ASSERT(XFracSec::ToMPEG7String(999000) == ":999F10000");
}


void XFracSecTest::TestFromISOString()
{
    UT_ASSERT_THROWS(XFracSec::FromISOString("."), XException);
    UT_ASSERT_THROWS(XFracSec::FromISOString(".00000001"), XException);

    UT_ASSERT(XFracSec::FromISOString("") == XFracSec::Zero());
    UT_ASSERT(XFracSec::FromISOString(".0000001") == XFracSec(HNSECS, 1));
    UT_ASSERT(XFracSec::FromISOString(".000001") == XFracSec(HNSECS, 10));
    UT_ASSERT(XFracSec::FromISOString(".00001") == XFracSec(HNSECS, 100));
    UT_ASSERT(XFracSec::FromISOString(".0001") == XFracSec(HNSECS, 1000));
    UT_ASSERT(XFracSec::FromISOString(".001") == XFracSec(HNSECS, 10000));
    UT_ASSERT(XFracSec::FromISOString(".01") == XFracSec(HNSECS, 100000));
    UT_ASSERT(XFracSec::FromISOString(".1") == XFracSec(HNSECS, 1000000));
    UT_ASSERT(XFracSec::FromISOString(".1000001") == XFracSec(HNSECS, 1000001));
    UT_ASSERT(XFracSec::FromISOString(".1001001") == XFracSec(HNSECS, 1001001));
    UT_ASSERT(XFracSec::FromISOString(".1071601") == XFracSec(HNSECS, 1071601));
    UT_ASSERT(XFracSec::FromISOString(".1271641") == XFracSec(HNSECS, 1271641));
    UT_ASSERT(XFracSec::FromISOString(".9999999") == XFracSec(HNSECS, 9999999));
    UT_ASSERT(XFracSec::FromISOString(".999999") == XFracSec(HNSECS, 9999990));
    UT_ASSERT(XFracSec::FromISOString(".99999") == XFracSec(HNSECS, 9999900));
    UT_ASSERT(XFracSec::FromISOString(".9999") == XFracSec(HNSECS, 9999000));
    UT_ASSERT(XFracSec::FromISOString(".999") == XFracSec(HNSECS, 9990000));
    UT_ASSERT(XFracSec::FromISOString(".99") == XFracSec(HNSECS, 9900000));
    UT_ASSERT(XFracSec::FromISOString(".9") == XFracSec(HNSECS, 9000000));
    UT_ASSERT(XFracSec::FromISOString(".0000999") == XFracSec(HNSECS, 999));
    UT_ASSERT(XFracSec::FromISOString(".000999") == XFracSec(HNSECS, 9990));
    UT_ASSERT(XFracSec::FromISOString(".00999") == XFracSec(HNSECS, 99900));
    UT_ASSERT(XFracSec::FromISOString(".0999") == XFracSec(HNSECS, 999000));
}


void XFracSecTest::TestFromMPEG7String()
{
    UT_ASSERT_THROWS(XFracSec::FromISOString(":"), XException);
    UT_ASSERT_THROWS(XFracSec::FromISOString(":F"), XException);
    UT_ASSERT_THROWS(XFracSec::FromISOString(":10"), XException);
    UT_ASSERT_THROWS(XFracSec::FromISOString(":10F1"), XException);
    UT_ASSERT_THROWS(XFracSec::FromISOString(":1F1"), XException);
    UT_ASSERT_THROWS(XFracSec::FromISOString(":10F10"), XException);
    UT_ASSERT_THROWS(XFracSec::FromISOString(":25F5"), XException);

    UT_ASSERT(XFracSec::FromMPEG7String("") == XFracSec::Zero());
    UT_ASSERT(XFracSec::FromMPEG7String(":1F10000000") == XFracSec(HNSECS, 1));
    UT_ASSERT(XFracSec::FromMPEG7String(":1F1000000") == XFracSec(HNSECS, 10));
    UT_ASSERT(XFracSec::FromMPEG7String(":1F100000") == XFracSec(HNSECS, 100));
    UT_ASSERT(XFracSec::FromMPEG7String(":1F10000") == XFracSec(HNSECS, 1000));
    UT_ASSERT(XFracSec::FromMPEG7String(":1F1000") == XFracSec(HNSECS, 10000));
    UT_ASSERT(XFracSec::FromMPEG7String(":1F100") == XFracSec(HNSECS, 100000));
    UT_ASSERT(XFracSec::FromMPEG7String(":1F10") == XFracSec(HNSECS, 1000000));
    UT_ASSERT(XFracSec::FromMPEG7String(":1000001F10000000") == XFracSec(HNSECS, 1000001));
    UT_ASSERT(XFracSec::FromMPEG7String(":1001001F10000000") == XFracSec(HNSECS, 1001001));
    UT_ASSERT(XFracSec::FromMPEG7String(":1071601F10000000") == XFracSec(HNSECS, 1071601));
    UT_ASSERT(XFracSec::FromMPEG7String(":1271641F10000000") == XFracSec(HNSECS, 1271641));
    UT_ASSERT(XFracSec::FromMPEG7String(":9999999F10000000") == XFracSec(HNSECS, 9999999));
    UT_ASSERT(XFracSec::FromMPEG7String(":999999F1000000") == XFracSec(HNSECS, 9999990));
    UT_ASSERT(XFracSec::FromMPEG7String(":99999F100000") == XFracSec(HNSECS, 9999900));
    UT_ASSERT(XFracSec::FromMPEG7String(":9999F10000") == XFracSec(HNSECS, 9999000));
    UT_ASSERT(XFracSec::FromMPEG7String(":999F1000") == XFracSec(HNSECS, 9990000));
    UT_ASSERT(XFracSec::FromMPEG7String(":99F100") == XFracSec(HNSECS, 9900000));
    UT_ASSERT(XFracSec::FromMPEG7String(":9F10") == XFracSec(HNSECS, 9000000));
    UT_ASSERT(XFracSec::FromMPEG7String(":999F10000000") == XFracSec(HNSECS, 999));
    UT_ASSERT(XFracSec::FromMPEG7String(":999F1000000") == XFracSec(HNSECS, 9990));
    UT_ASSERT(XFracSec::FromMPEG7String(":999F100000") == XFracSec(HNSECS, 99900));
    UT_ASSERT(XFracSec::FromMPEG7String(":999F10000") == XFracSec(HNSECS, 999000));

    UT_ASSERT(XFracSec::FromMPEG7String(":2F4") == XFracSec(MSECS, 500));
}


void XFracSecTest::TestIsNegative()
{
    UT_ASSERT(!XFracSec(100).IsNegative());
    UT_ASSERT(!XFracSec(1).IsNegative());
    UT_ASSERT(!XFracSec(0).IsNegative());
    UT_ASSERT(XFracSec(-1).IsNegative());
    UT_ASSERT(XFracSec(-100).IsNegative());
}


void XFracSecTest::TestIsZero()
{
    UT_ASSERT(!XFracSec(100).IsZero());
    UT_ASSERT(!XFracSec(1).IsZero());
    UT_ASSERT( XFracSec(0).IsZero());
    UT_ASSERT(!XFracSec(-1).IsZero());
    UT_ASSERT(!XFracSec(-100).IsZero());
}


//XDurationTest:

void XDurationTest::TestConstructor()
{
    UT_ASSERT(XDuration(0)._hnsecs == 0);

    for(size_t i = 0; i < 2; ++i)
    {
        const int sign = signVals[i];

        UT_ASSERT(XDuration(1 * sign)._hnsecs == 1 * sign);
        UT_ASSERT(XDuration(42 * sign)._hnsecs == 42 * sign);
        UT_ASSERT(XDuration(12007 * sign)._hnsecs == 12007 * sign);

        UT_ASSERT(XDuration(DAYS, 12 * sign) == XDuration(10368000000000LL * sign));
        UT_ASSERT(XDuration(HNSECS, 27 * sign) == XDuration(27 * sign));

        UT_ASSERT(XDuration(WEEKS, 7 * sign).Total(WEEKS) == 7 * sign);
        UT_ASSERT(XDuration(DAYS, 7 * sign).Total(DAYS) == 7 * sign);
        UT_ASSERT(XDuration(HOURS, 7 * sign).Total(HOURS) == 7 * sign);
        UT_ASSERT(XDuration(MINUTES, 7 * sign).Total(MINUTES) == 7 * sign);
        UT_ASSERT(XDuration(SECONDS, 7 * sign).Total(SECONDS) == 7 * sign);
        UT_ASSERT(XDuration(MSECS, 7 * sign).Total(MSECS) == 7 * sign);
        UT_ASSERT(XDuration(USECS, 7 * sign).Total(USECS) == 7 * sign);
        UT_ASSERT(XDuration(HNSECS, 7 * sign).Total(HNSECS) == 7 * sign);
        UT_ASSERT(XDuration(NSECS, 7 * sign).Total(NSECS) == 0);
        UT_ASSERT(XDuration(NSECS, 767 * sign).Total(NSECS) == 700 * sign);
    }

}


void XDurationTest::TestEquality()
{
    UT_ASSERT(XDuration(42) == XDuration(42));
    UT_ASSERT(!(XDuration(42) != XDuration(42)));
    UT_ASSERT(!(XDuration(42) < XDuration(42)));
    UT_ASSERT(XDuration(42) <= XDuration(42));
    UT_ASSERT(!(XDuration(42) > XDuration(42)));
    UT_ASSERT(XDuration(42) >= XDuration(42));

    UT_ASSERT(!(XDuration(42) == XDuration(100)));
    UT_ASSERT(XDuration(42) != XDuration(100));
    UT_ASSERT(XDuration(42) < XDuration(100));
    UT_ASSERT(XDuration(42) <= XDuration(100));
    UT_ASSERT(!(XDuration(42) > XDuration(100)));
    UT_ASSERT(!(XDuration(42) >= XDuration(100)));

    UT_ASSERT(!(XDuration(100) == XDuration(42)));
    UT_ASSERT(XDuration(100) != XDuration(42));
    UT_ASSERT(!(XDuration(100) < XDuration(42)));
    UT_ASSERT(!(XDuration(100) <= XDuration(42)));
    UT_ASSERT(XDuration(100) > XDuration(42));
    UT_ASSERT(XDuration(100) >= XDuration(42));

    UT_ASSERT(!(XDuration(-42) == XDuration(42)));
    UT_ASSERT(XDuration(-42) != XDuration(42));
    UT_ASSERT(XDuration(-42) < XDuration(42));
    UT_ASSERT(XDuration(-42) <= XDuration(42));
    UT_ASSERT(!(XDuration(-42) > XDuration(42)));
    UT_ASSERT(!(XDuration(-42) >= XDuration(42)));

    UT_ASSERT(XDuration(-42) == XDuration(-42));
    UT_ASSERT(!(XDuration(-42) != XDuration(-42)));
    UT_ASSERT(!(XDuration(-42) < XDuration(-42)));
    UT_ASSERT(XDuration(-42) <= XDuration(-42));
    UT_ASSERT(!(XDuration(-42) > XDuration(-42)));
    UT_ASSERT(XDuration(-42) >= XDuration(-42));

    UT_ASSERT(!(XDuration(42) == XDuration(-42)));
    UT_ASSERT(XDuration(42) != XDuration(-42));
    UT_ASSERT(!(XDuration(42) < XDuration(-42)));
    UT_ASSERT(!(XDuration(42) <= XDuration(-42)));
    UT_ASSERT(XDuration(42) > XDuration(-42));
    UT_ASSERT(XDuration(42) >= XDuration(-42));
}


void XDurationTest::TestAddSub()
{
    UT_ASSERT(XDuration(5) + XDuration(7) == XDuration(12));
    UT_ASSERT(XDuration(5) - XDuration(7) == XDuration(-2));
    UT_ASSERT(XDuration(7) + XDuration(5) == XDuration(12));
    UT_ASSERT(XDuration(7) - XDuration(5) == XDuration(2));

    UT_ASSERT(XDuration(5) + XDuration(-7) == XDuration(-2));
    UT_ASSERT(XDuration(5) - XDuration(-7) == XDuration(12));
    UT_ASSERT(XDuration(7) + XDuration(-5) == XDuration(2));
    UT_ASSERT(XDuration(7) - XDuration(-5) == XDuration(12));

    UT_ASSERT(XDuration(-5) + XDuration(7) == XDuration(2));
    UT_ASSERT(XDuration(-5) - XDuration(7) == XDuration(-12));
    UT_ASSERT(XDuration(-7) + XDuration(5) == XDuration(-2));
    UT_ASSERT(XDuration(-7) - XDuration(5) == XDuration(-12));

    UT_ASSERT(XDuration(-5) + XDuration(-7) == XDuration(-12));
    UT_ASSERT(XDuration(-5) - XDuration(-7) == XDuration(2));
    UT_ASSERT(XDuration(-7) + XDuration(-5) == XDuration(-12));
    UT_ASSERT(XDuration(-7) - XDuration(-5) == XDuration(-2));
}


void XDurationTest::TestAddSubAssign()
{
    {
        XDuration duration(5);
        UT_ASSERT((duration += XDuration(7)) == XDuration(12));
        UT_ASSERT(duration == XDuration(12));
    }

    {
        XDuration duration(5);
        UT_ASSERT((duration -= XDuration(7)) == XDuration(-2));
        UT_ASSERT(duration == XDuration(-2));
    }

    {
        XDuration duration(7);
        UT_ASSERT((duration += XDuration(5)) == XDuration(12));
        UT_ASSERT(duration == XDuration(12));
    }

    {
        XDuration duration(7);
        UT_ASSERT((duration -= XDuration(5)) == XDuration(2));
        UT_ASSERT(duration == XDuration(2));
    }


    {
        XDuration duration(5);
        UT_ASSERT((duration += XDuration(-7)) == XDuration(-2));
        UT_ASSERT(duration == XDuration(-2));
    }

    {
        XDuration duration(5);
        UT_ASSERT((duration -= XDuration(-7)) == XDuration(12));
        UT_ASSERT(duration == XDuration(12));
    }

    {
        XDuration duration(7);
        UT_ASSERT((duration += XDuration(-5)) == XDuration(2));
        UT_ASSERT(duration == XDuration(2));
    }

    {
        XDuration duration(7);
        UT_ASSERT((duration -= XDuration(-5)) == XDuration(12));
        UT_ASSERT(duration == XDuration(12));
    }


    {
        XDuration duration(-5);
        UT_ASSERT((duration += XDuration(7)) == XDuration(2));
        UT_ASSERT(duration == XDuration(2));
    }

    {
        XDuration duration(-5);
        UT_ASSERT((duration -= XDuration(7)) == XDuration(-12));
        UT_ASSERT(duration == XDuration(-12));
    }

    {
        XDuration duration(-7);
        UT_ASSERT((duration += XDuration(5)) == XDuration(-2));
        UT_ASSERT(duration == XDuration(-2));
    }

    {
        XDuration duration(-7);
        UT_ASSERT((duration -= XDuration(5)) == XDuration(-12));
        UT_ASSERT(duration == XDuration(-12));
    }


    {
        XDuration duration(-5);
        UT_ASSERT((duration += XDuration(-7)) == XDuration(-12));
        UT_ASSERT(duration == XDuration(-12));
    }

    {
        XDuration duration(-5);
        UT_ASSERT((duration -= XDuration(-7)) == XDuration(2));
        UT_ASSERT(duration == XDuration(2));
    }

    {
        XDuration duration(-7);
        UT_ASSERT((duration += XDuration(-5)) == XDuration(-12));
        UT_ASSERT(duration == XDuration(-12));
    }

    {
        XDuration duration(-7);
        UT_ASSERT((duration -= XDuration(-5)) == XDuration(-2));
        UT_ASSERT(duration == XDuration(-2));
    }
}


void XDurationTest::TestMul()
{
    UT_ASSERT(XDuration(5) * 7 == XDuration(35));
    UT_ASSERT(XDuration(7) * 5 == XDuration(35));

    UT_ASSERT(XDuration(5) * -7 == XDuration(-35));
    UT_ASSERT(XDuration(7) * -5 == XDuration(-35));

    UT_ASSERT(XDuration(-5) * 7 == XDuration(-35));
    UT_ASSERT(XDuration(-7) * 5 == XDuration(-35));

    UT_ASSERT(XDuration(-5) * -7 == XDuration(35));
    UT_ASSERT(XDuration(-7) * -5 == XDuration(35));

    UT_ASSERT(XDuration(5) * 0 == XDuration(0));
    UT_ASSERT(XDuration(-5) * 0 == XDuration(0));
}


void XDurationTest::TestMulAssign()
{
    {
        XDuration duration(5);
        UT_ASSERT((duration *= 7) == XDuration(35));
        UT_ASSERT(duration == XDuration(35));
    }

    {
        XDuration duration(7);
        UT_ASSERT((duration *= 5) == XDuration(35));
        UT_ASSERT(duration == XDuration(35));
    }


    {
        XDuration duration(5);
        UT_ASSERT((duration *= -7) == XDuration(-35));
        UT_ASSERT(duration == XDuration(-35));
    }

    {
        XDuration duration(7);
        UT_ASSERT((duration *= -5) == XDuration(-35));
        UT_ASSERT(duration == XDuration(-35));
    }


    {
        XDuration duration(-5);
        UT_ASSERT((duration *= 7) == XDuration(-35));
        UT_ASSERT(duration == XDuration(-35));
    }

    {
        XDuration duration(-7);
        UT_ASSERT((duration *= 5) == XDuration(-35));
        UT_ASSERT(duration == XDuration(-35));
    }


    {
        XDuration duration(-5);
        UT_ASSERT((duration *= -7) == XDuration(35));
        UT_ASSERT(duration == XDuration(35));
    }

    {
        XDuration duration(-7);
        UT_ASSERT((duration *= -5) == XDuration(35));
        UT_ASSERT(duration == XDuration(35));
    }


    {
        XDuration duration(5);
        UT_ASSERT((duration *= 0) == XDuration(0));
        UT_ASSERT(duration == XDuration(0));
    }

    {
        XDuration duration(-5);
        UT_ASSERT((duration *= 0) == XDuration(0));
        UT_ASSERT(duration == XDuration(0));
    }
}


void XDurationTest::TestDiv()
{
    UT_ASSERT(XDuration(5) / 7 == XDuration(0));
    UT_ASSERT(XDuration(7) / 5 == XDuration(1));

    UT_ASSERT(XDuration(5) / -7 == XDuration(0));
    UT_ASSERT(XDuration(7) / -5 == XDuration(-1));

    UT_ASSERT(XDuration(-5) / 7 == XDuration(0));
    UT_ASSERT(XDuration(-7) / 5 == XDuration(-1));

    UT_ASSERT(XDuration(-5) / -7 == XDuration(0));
    UT_ASSERT(XDuration(-7) / -5 == XDuration(1));
}


void XDurationTest::TestDiv2()
{
    UT_ASSERT(XDuration(5) / XDuration(16) == 0);
    UT_ASSERT(XDuration(16) / XDuration(5) == 3);

    UT_ASSERT(XDuration(5) / XDuration(-16) == 0);
    UT_ASSERT(XDuration(16) / XDuration(-5) == -3);

    UT_ASSERT(XDuration(-5) / XDuration(16) == 0);
    UT_ASSERT(XDuration(-16) / XDuration(5) == -3);

    UT_ASSERT(XDuration(-5) / XDuration(-16) == 0);
    UT_ASSERT(XDuration(-16) / XDuration(-5) == 3);
}


void XDurationTest::TestDivAssign()
{
    {
        XDuration duration(5);
        UT_ASSERT((duration /= 7) == XDuration(0));
        UT_ASSERT(duration == XDuration(0));
    }

    {
        XDuration duration(7);
        UT_ASSERT((duration /= 5) == XDuration(1));
        UT_ASSERT(duration == XDuration(1));
    }


    {
        XDuration duration(5);
        UT_ASSERT((duration /= -7) == XDuration(0));
        UT_ASSERT(duration == XDuration(0));
    }

    {
        XDuration duration(7);
        UT_ASSERT((duration /= -5) == XDuration(-1));
        UT_ASSERT(duration == XDuration(-1));
    }


    {
        XDuration duration(-5);
        UT_ASSERT((duration /= 7) == XDuration(0));
        UT_ASSERT(duration == XDuration(0));
    }

    {
        XDuration duration(-7);
        UT_ASSERT((duration /= 5) == XDuration(-1));
        UT_ASSERT(duration == XDuration(-1));
    }


    {
        XDuration duration(-5);
        UT_ASSERT((duration /= -7) == XDuration(0));
        UT_ASSERT(duration == XDuration(0));
    }

    {
        XDuration duration(-7);
        UT_ASSERT((duration /= -5) == XDuration(1));
        UT_ASSERT(duration == XDuration(1));
    }
}


void XDurationTest::TestNegation()
{
    const int vals[5] = {-7, -5, 0, 5, 7};

    for(size_t i = 0; i < 5; ++i)
    {
        XDuration fs = XDuration(vals[i]);
        const XDuration cfs = XDuration(vals[i]);

        UT_ASSERT(-fs == XDuration(-vals[i]));
        UT_ASSERT(-cfs == XDuration(-vals[i]));
    }
}


void XDurationTest::TestGetOnly()
{
    //Verify Examples.
    UT_ASSERT(XDuration(WEEKS, 12).GetOnly(WEEKS) == 12);
    UT_ASSERT(XDuration(WEEKS, 12).GetOnly(DAYS) == 0);

    for(size_t i = 0; i < 2; ++i)
    {
        const int sign = signVals[i];

        UT_ASSERT(XDuration(DAYS, 13 * sign).GetOnly(WEEKS) == 1 * sign);
        UT_ASSERT(XDuration(DAYS, 13 * sign).GetOnly(DAYS) == 6 * sign);

        UT_ASSERT(XDuration(HOURS, 49 * sign).GetOnly(DAYS) == 2 * sign);
        UT_ASSERT(XDuration(HOURS, 49 * sign).GetOnly(HOURS) == 1 * sign);

        UT_ASSERT(XDuration(HOURS, 25 * sign).GetOnly(HOURS) == 1 * sign);
        UT_ASSERT(XDuration(HOURS, 25 * sign).GetOnly(SECONDS) == 0);

        UT_ASSERT(XDuration(MSECS, 1002 * sign).GetOnly(SECONDS) == 1 * sign);
    }
}


void XDurationTest::TestTotal()
{
    //Verify Examples.
    UT_ASSERT(XDuration(WEEKS, 12).Total(WEEKS) == 12);
    UT_ASSERT(XDuration(WEEKS, 12).Total(DAYS) == 84);

    for(size_t i = 0; i < 2; ++i)
    {
        const int sign = signVals[i];

        UT_ASSERT(XDuration(DAYS, 13 * sign).Total(WEEKS) == 1 * sign);
        UT_ASSERT(XDuration(DAYS, 13 * sign).Total(DAYS) == 13 * sign);

        UT_ASSERT(XDuration(HOURS, 49 * sign).Total(DAYS) == 2 * sign);
        UT_ASSERT(XDuration(HOURS, 49 * sign).Total(HOURS) == 49 * sign);

        UT_ASSERT(XDuration(HOURS, 25 * sign).Total(HOURS) == 25 * sign);
        UT_ASSERT(XDuration(HOURS, 25 * sign).Total(SECONDS) == 90000 * sign);

        UT_ASSERT(XDuration(MSECS, 1002 * sign).Total(SECONDS) == 1 * sign);
        UT_ASSERT(XDuration(MSECS, 1002 * sign).Total(MSECS) == 1002 * sign);

        UT_ASSERT(XDuration(NSECS, 2007 * sign).Total(HNSECS) == 20 * sign);
        UT_ASSERT(XDuration(NSECS, 2007 * sign).Total(NSECS) == 2000 * sign);
    }
}


void XDurationTest::TestIsNegative()
{
    //Verify Examples.
    UT_ASSERT(!XDuration(100).IsNegative());
    UT_ASSERT(!XDuration(1).IsNegative());
    UT_ASSERT(!XDuration(0).IsNegative());
    UT_ASSERT(XDuration(-1).IsNegative());
    UT_ASSERT(XDuration(-100).IsNegative());
}


void XDurationTest::TestIsZero()
{
    //Verify Examples.
    UT_ASSERT(!XDuration(100).IsZero());
    UT_ASSERT(!XDuration(1).IsZero());
    UT_ASSERT( XDuration(0).IsZero());
    UT_ASSERT(!XDuration(-1).IsZero());
    UT_ASSERT(!XDuration(-100).IsZero());
}


void XDurationTest::TestFracSec()
{
    //Verify Examples.
    UT_ASSERT(XDuration(MSECS, 1000).FracSec() == XFracSec(MSECS, 0));
    UT_ASSERT(XDuration(MSECS, 1217).FracSec() == XFracSec(MSECS, 217));
    UT_ASSERT(XDuration(USECS, 43).FracSec() == XFracSec(USECS, 43));
    UT_ASSERT(XDuration(HNSECS, 50007).FracSec() == XFracSec(HNSECS, 50007));

    UT_ASSERT(XDuration(MSECS, -1000).FracSec() == XFracSec(MSECS, 0));
    UT_ASSERT(XDuration(MSECS, -1217).FracSec() == XFracSec(MSECS, -217));
    UT_ASSERT(XDuration(USECS, -43).FracSec() == XFracSec(USECS, -43));
    UT_ASSERT(XDuration(HNSECS, -50007).FracSec() == XFracSec(HNSECS, -50007));
}



void XDurationTest::TestToString()
{
    UT_ASSERT(XDuration(0).ToString() == "0 hnsecs");
    UT_ASSERT(XDuration(1).ToString() == "1 hnsec");
    UT_ASSERT(XDuration(7).ToString() == "7 hnsecs");
    UT_ASSERT(XDuration(10).ToString() == "1 \u03BCs");
    UT_ASSERT(XDuration(20).ToString() == "2 \u03BCs");
    UT_ASSERT(XDuration(10000).ToString() == "1 ms");
    UT_ASSERT(XDuration(20000).ToString() == "2 ms");
    UT_ASSERT(XDuration(10000000LL).ToString() == "1 sec");
    UT_ASSERT(XDuration(20000000LL).ToString() == "2 secs");
    UT_ASSERT(XDuration(600000000LL).ToString() == "1 minute");
    UT_ASSERT(XDuration(1200000000LL).ToString() == "2 minutes");
    UT_ASSERT(XDuration(36000000000LL).ToString() == "1 hour");
    UT_ASSERT(XDuration(72000000000LL).ToString() == "2 hours");
    UT_ASSERT(XDuration(864000000000LL).ToString() == "1 day");
    UT_ASSERT(XDuration(1728000000000LL).ToString() == "2 days");
    UT_ASSERT(XDuration(6048000000000LL).ToString() == "1 week");
    UT_ASSERT(XDuration(12096000000000LL).ToString() == "2 weeks");

    UT_ASSERT(XDuration(12).ToString() == "1 \u03BCs and 2 hnsecs");
    UT_ASSERT(XDuration(120795).ToString() == "12 ms, 79 \u03BCs, and 5 hnsecs");
    UT_ASSERT(XDuration(12096020900003LL).ToString() == "2 weeks, 2 secs, 90 ms, and 3 hnsecs");

    UT_ASSERT(XDuration(-1).ToString() == "-1 hnsecs");
    UT_ASSERT(XDuration(-7).ToString() == "-7 hnsecs");
    UT_ASSERT(XDuration(-10).ToString() == "-1 \u03BCs");
    UT_ASSERT(XDuration(-20).ToString() == "-2 \u03BCs");
    UT_ASSERT(XDuration(-10000).ToString() == "-1 ms");
    UT_ASSERT(XDuration(-20000).ToString() == "-2 ms");
    UT_ASSERT(XDuration(-10000000LL).ToString() == "-1 secs");
    UT_ASSERT(XDuration(-20000000LL).ToString() == "-2 secs");
    UT_ASSERT(XDuration(-600000000LL).ToString() == "-1 minutes");
    UT_ASSERT(XDuration(-1200000000LL).ToString() == "-2 minutes");
    UT_ASSERT(XDuration(-36000000000LL).ToString() == "-1 hours");
    UT_ASSERT(XDuration(-72000000000LL).ToString() == "-2 hours");
    UT_ASSERT(XDuration(-864000000000LL).ToString() == "-1 days");
    UT_ASSERT(XDuration(-1728000000000LL).ToString() == "-2 days");
    UT_ASSERT(XDuration(-6048000000000LL).ToString() == "-1 weeks");
    UT_ASSERT(XDuration(-12096000000000LL).ToString() == "-2 weeks");

    UT_ASSERT(XDuration(-12).ToString() == "-1 \u03BCs and -2 hnsecs");
    UT_ASSERT(XDuration(-120795).ToString() == "-12 ms, -79 \u03BCs, and -5 hnsecs");
    UT_ASSERT(XDuration(-12096020900003LL).ToString() == "-2 weeks, -2 secs, -90 ms, and -3 hnsecs");
}
