
#include "XStatisticsTest.h"

#include <vector>
#include <algorithm>
#include "XSDK/XStatistics.h"
#include "XSDK/XRef.h"

using namespace std;
using namespace XSDK;

const double g_test[] = { 7.0, 3.8, 9.4, 6.5, 1.2, 2.1, 5.6, 4.9, 8.3, 0.7 };
const size_t g_testLen = sizeof(g_test)/sizeof(g_test[0]);

#define MAX_SAMPLES         4
#define MAX_DURATION_SEC    4


REGISTER_TEST_FIXTURE(XStatisticsTest);

void XStatisticsTest::TestAverage()
{
    XRef<XStatistics<Average,double> > avg = new XStatistics<Average,double>;
    UT_ASSERT( avg.IsValid() == true );
    for (size_t ii=0; ii<g_testLen; ++ii)
        avg->AddSample(g_test[ii]);
    double result = 0.0;
    UT_ASSERT_NO_THROW( avg->GetResult(result) );

    // "Hand-calculate" the average
    double answer = 0.0;
    for (size_t ii=0; ii<g_testLen; ++ii)
        answer += g_test[ii];
    answer /= g_testLen;

    UT_ASSERT( answer == result );
}

void XStatisticsTest::TestMedian()
{
    XRef<XStatistics<Median,double> > med = new XStatistics<Median,double>;
    UT_ASSERT( med.IsValid() == true );
    for (size_t ii=0; ii<g_testLen; ++ii)
        med->AddSample(g_test[ii]);
    double result = 0.0;
    UT_ASSERT_NO_THROW( med->GetResult(result) );

    // "Hand-calculate" the median
    vector<double> sorted(g_testLen);
    for (size_t ii=0; ii<g_testLen; ++ii)
        sorted[ii] = g_test[ii];
    sort(sorted.begin(), sorted.end());
    double answer = sorted[g_testLen/2];

    UT_ASSERT( answer == result );
}

void XStatisticsTest::TestMaxSamples()
{
    XRef<XStatistics<Average,double> > avg = new XStatistics<Average,double>(MAX_SAMPLES);
    UT_ASSERT( avg.IsValid() == true );
    for (size_t ii=0; ii<g_testLen; ++ii)
        avg->AddSample(g_test[ii]);
    double result = 0.0;
    UT_ASSERT_NO_THROW( avg->GetResult(result) );

    // "Hand-calculate" the average
    double answer = 0.0;
    for (size_t ii=g_testLen-MAX_SAMPLES; ii<g_testLen; ++ii)
        answer += g_test[ii];
    answer /= MAX_SAMPLES;

    UT_ASSERT( answer == result );

    int count = -1;
    UT_ASSERT_NO_THROW( count = avg->GetNumSamples() );
    UT_ASSERT( count == MAX_SAMPLES );
    UT_ASSERT_NO_THROW( count = avg->GetMaxSamples() );
    UT_ASSERT( count == MAX_SAMPLES );

    UT_ASSERT_NO_THROW( avg->SetMaxSamples( MAX_SAMPLES + 1 ) );
    UT_ASSERT( count = avg->GetMaxSamples() );
    UT_ASSERT( count == MAX_SAMPLES + 1 );
    for (size_t ii=0; ii<g_testLen; ++ii)
        avg->AddSample(g_test[ii]);
    UT_ASSERT_NO_THROW( count = avg->GetNumSamples() );
    UT_ASSERT( count == MAX_SAMPLES + 1 );
}

void XStatisticsTest::TestSampleTimeout()
{
    // Add samples, wait half the expiration time, add more samples,
    // wait a little over half the expiration time, make sure half
    // the sample have been pruned.
    XRef<XStatistics<Average,double> > avg = new XStatistics<Average,double>(g_testLen*10, XDuration(XSDK::SECONDS, MAX_DURATION_SEC));
    UT_ASSERT( avg.IsValid() == true );
    for (size_t ii=0; ii<g_testLen; ++ii)
        avg->AddSample(g_test[ii]);

    x_sleep(MAX_DURATION_SEC / 2);

    for (size_t ii=0; ii<g_testLen; ++ii)
        avg->AddSample(g_test[ii]);

    x_sleep( (MAX_DURATION_SEC / 2) + (MAX_DURATION_SEC / 4) );

    double result = 0.0;
    UT_ASSERT_NO_THROW( avg->GetResult(result) );

    int count = -1;
    UT_ASSERT_NO_THROW( count = avg->GetNumSamples() );
    UT_ASSERT( count == (int)g_testLen );

    // "Hand-calculate" the average
    double answer = 0.0;
    for (size_t ii=0; ii<g_testLen; ++ii)
        answer += g_test[ii];
    answer /= g_testLen;

    UT_ASSERT( answer == result );
}

