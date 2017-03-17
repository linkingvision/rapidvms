
#include "ck_statistics_test.h"
#include <memory>
#include <vector>
#include <algorithm>
#include "cppkit/ck_statistics.h"
#include "cppkit/os/ck_time_utils.h"

using namespace std;
using namespace cppkit;

REGISTER_TEST_FIXTURE(ck_statistics_test);

const double g_test[] = { 7.0, 3.8, 9.4, 6.5, 1.2, 2.1, 5.6, 4.9, 8.3, 0.7 };
const size_t g_testLen = sizeof(g_test)/sizeof(g_test[0]);

#define MAX_SAMPLES         4
#define MAX_DURATION_SEC    4

void ck_statistics_test::test_constructor()
{
    shared_ptr<ck_statistics<average,double> > avg = make_shared<ck_statistics<average,double> >();
}

void ck_statistics_test::test_average()
{
    shared_ptr<ck_statistics<average,double> > avg = make_shared<ck_statistics<average,double> >();
    for (size_t ii=0; ii<g_testLen; ++ii)
        avg->add_sample(g_test[ii]);
    double result = 0.0;
    UT_ASSERT_NO_THROW( avg->get_result( result ) );

    // "Hand-calculate" the average
    double answer = 0.0;
    for (size_t ii=0; ii<g_testLen; ++ii)
        answer += g_test[ii];
    answer /= g_testLen;

    UT_ASSERT( answer == result );
}

void ck_statistics_test::test_median()
{
    shared_ptr<ck_statistics<median,double> > med = make_shared<ck_statistics<median,double> >();
    for (size_t ii=0; ii<g_testLen; ++ii)
        med->add_sample(g_test[ii]);
    double result = 0.0;
    UT_ASSERT_NO_THROW( med->get_result(result) );

    // "Hand-calculate" the median
    vector<double> sorted(g_testLen);
    for (size_t ii=0; ii<g_testLen; ++ii)
        sorted[ii] = g_test[ii];
    sort(sorted.begin(), sorted.end());
    double answer = sorted[g_testLen/2];

    UT_ASSERT( answer == result );
}

void ck_statistics_test::test_max_samples()
{
    shared_ptr<ck_statistics<average,double> > avg = make_shared<ck_statistics<average,double> >(MAX_SAMPLES);

    for (size_t ii=0; ii<g_testLen; ++ii)
        avg->add_sample(g_test[ii]);
    double result = 0.0;
    UT_ASSERT_NO_THROW( avg->get_result(result) );

    // "Hand-calculate" the average
    double answer = 0.0;
    for (size_t ii=g_testLen-MAX_SAMPLES; ii<g_testLen; ++ii)
        answer += g_test[ii];
    answer /= MAX_SAMPLES;

    UT_ASSERT( answer == result );

    int count = -1;
    UT_ASSERT_NO_THROW( count = avg->get_num_samples() );
    UT_ASSERT( count == MAX_SAMPLES );
    UT_ASSERT_NO_THROW( count = avg->get_max_samples() );
    UT_ASSERT( count == MAX_SAMPLES );

    UT_ASSERT_NO_THROW( avg->set_max_samples( MAX_SAMPLES + 1 ) );
    UT_ASSERT( count = avg->get_max_samples() );
    UT_ASSERT( count == MAX_SAMPLES + 1 );
    for (size_t ii=0; ii<g_testLen; ++ii)
        avg->add_sample(g_test[ii]);
    UT_ASSERT_NO_THROW( count = avg->get_num_samples() );
    UT_ASSERT( count == MAX_SAMPLES + 1 );
}

void ck_statistics_test::test_sample_timeout()
{
    // Add samples, wait half the expiration time, add more samples,
    // wait a little over half the expiration time, make sure half
    // the sample have been pruned.
    shared_ptr<ck_statistics<average,double> > avg = make_shared<ck_statistics<average,double> >(g_testLen*10, std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::seconds(MAX_DURATION_SEC)));

    for (size_t ii=0; ii<g_testLen; ++ii)
        avg->add_sample(g_test[ii]);

    ck_sleep(MAX_DURATION_SEC / 2);

    for (size_t ii=0; ii<g_testLen; ++ii)
        avg->add_sample(g_test[ii]);

    ck_sleep( (MAX_DURATION_SEC / 2) + (MAX_DURATION_SEC / 4) );

    double result = 0.0;
    UT_ASSERT_NO_THROW( avg->get_result(result) );

    int count = -1;
    UT_ASSERT_NO_THROW( count = avg->get_num_samples() );
    UT_ASSERT( count == (int)g_testLen );

    // "Hand-calculate" the average
    double answer = 0.0;
    for (size_t ii=0; ii<g_testLen; ++ii)
        answer += g_test[ii];
    answer /= g_testLen;

    UT_ASSERT( answer == result );
}
