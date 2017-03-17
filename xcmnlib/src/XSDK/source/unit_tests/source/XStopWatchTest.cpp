
#include "XStopWatchTest.h"
#include "XSDK/XStopWatch.h"
#include "XSDK/TimeUtils.h"

using namespace XSDK;

REGISTER_TEST_FIXTURE(XStopWatchTest);

void XStopWatchTest::TestTheBasics()
{
    XStopWatch watch;

    // XStopWatch should start out zero'd out.
    double time = -1.0;
    UT_ASSERT(watch.GetAsSeconds() == 0.0);
    UT_ASSERT(watch.GetAsUSeconds() == 0.0);
    UT_ASSERT(watch.TimeElapsed() == XDuration::Zero());

    watch.Start();
    x_sleep(XDuration(USECS, 1001000)); // Sleep longer than a second
    watch.Stop();
    UT_ASSERT(watch.GetAsSeconds() > 1.0);
    UT_ASSERT(watch.GetAsUSeconds() > 1000000.0);
    UT_ASSERT(watch.TimeElapsed() > XDuration(SECONDS, 1));

    watch.Reset();
    UT_ASSERT(watch.GetAsSeconds() == 0.0);
    UT_ASSERT(watch.TimeElapsed() == XDuration::Zero());
}

void XStopWatchTest::TestAssignment()
{
    XStopWatch watch1(true);
    XStopWatch watch2(true);

    x_sleep(XDuration(MSECS, 100));
    watch1.Stop();
    x_sleep(XDuration(MSECS, 100));
    watch2.Stop();

    UT_ASSERT(watch2.GetAsSeconds() > watch1.GetAsSeconds());
    UT_ASSERT(watch2.TimeElapsed() > watch1.TimeElapsed());
    watch1 = watch2;
    UT_ASSERT(watch2.GetAsSeconds() == watch1.GetAsSeconds());
    UT_ASSERT(watch2.TimeElapsed() == watch1.TimeElapsed());

    const XStopWatch watch3 = watch2;
    UT_ASSERT(watch2.GetAsSeconds() == watch3.GetAsSeconds());
    UT_ASSERT(watch2.TimeElapsed() == watch3.TimeElapsed());
}

void XStopWatchTest::TestAccumulation()
{
    XStopWatch watch;
    watch.Start();
    x_sleep(XDuration(MSECS, 100));
    watch.Stop();
    const XDuration interval1 = watch.TimeElapsed();

    // Make sure the stop watch is really stopped.
    x_sleep(XDuration(MSECS, 250));
    UT_ASSERT(interval1 == watch.TimeElapsed());

    watch.Start();
    x_sleep(XDuration(MSECS, 250));
    watch.Stop();
    const XDuration interval2 = watch.TimeElapsed();
    UT_ASSERT(interval2 > interval1);

    // Test reset after multiple intervals.
    watch.Reset();
    UT_ASSERT(watch.TimeElapsed() == XDuration::Zero());
}

void XStopWatchTest::TestEdgeCases()
{
    // Test multiple Start/Stop calls.
    XStopWatch watch;
    watch.Start();
    watch.Start();
    x_sleep(XDuration(MSECS, 100));
    watch.Stop();
    const XDuration time1 = watch.TimeElapsed();
    UT_ASSERT(time1 > XDuration::Zero());
    watch.Stop();
    const XDuration time2 = watch.TimeElapsed();
    UT_ASSERT(time1 == time2);
    watch.Stop();
    const XDuration time3 = watch.TimeElapsed();
    UT_ASSERT(time1 == time3);

    // Test multiple Reset calls.
    watch.Reset();
    const XDuration time4 = watch.TimeElapsed();
    watch.Reset();
    const XDuration time5 = watch.TimeElapsed();
    UT_ASSERT(time5 == XDuration::Zero());
    UT_ASSERT(time4 == time5);
}

void XStopWatchTest::TestRestart()
{
    XStopWatch watch;

    watch.Restart();
    x_sleep(XDuration(MSECS, 100));
    const XDuration time1 = watch.TimeElapsed();
    x_sleep(XDuration(MSECS, 100));
    const XDuration time2 = watch.TimeElapsed();
    UT_ASSERT(time2 > time1);

    watch.Restart();
    UT_ASSERT(watch.TimeElapsed() < time1);
    x_sleep(XDuration(MSECS, 50));
    UT_ASSERT(watch.TimeElapsed() < time1);
    x_sleep(XDuration(MSECS, 60));
    UT_ASSERT(watch.TimeElapsed() > time1);
    UT_ASSERT(watch.TimeElapsed() < time2);
    x_sleep(XDuration(MSECS, 100));
    UT_ASSERT(watch.TimeElapsed() > time2);

    watch.Restart();
    UT_ASSERT(watch.TimeElapsed() < time1);
}
