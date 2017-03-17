
#include "XDateTupleTest.h"

#include <vector>

using namespace std;
using namespace XSDK;

REGISTER_TEST_FIXTURE(XDateTupleTest);
REGISTER_TEST_FIXTURE(XTimeTupleTest);


void XDateTupleTest::TestConstructor()
{
    UT_ASSERT(XDateTuple(1) == XDateTuple(1, 1, 1));
    UT_ASSERT(XDateTuple(2) == XDateTuple(1, 1, 2));
    UT_ASSERT(XDateTuple(32) == XDateTuple(1, 2, 1));
    UT_ASSERT(XDateTuple(365) == XDateTuple(1, 12, 31));
    UT_ASSERT(XDateTuple(366) == XDateTuple(2, 1, 1));
    UT_ASSERT(XDateTuple(731) == XDateTuple(3, 1, 1));
    UT_ASSERT(XDateTuple(1096) == XDateTuple(4, 1, 1));
    UT_ASSERT(XDateTuple(1097) == XDateTuple(4, 1, 2));
    UT_ASSERT(XDateTuple(1460) == XDateTuple(4, 12, 30));
    UT_ASSERT(XDateTuple(1461) == XDateTuple(4, 12, 31));
    UT_ASSERT(XDateTuple(1462) == XDateTuple(5, 1, 1));

    UT_ASSERT(XDateTuple(584023) == XDateTuple(1600, 1, 1));
    UT_ASSERT(XDateTuple(584389) == XDateTuple(1601, 1, 1));
    UT_ASSERT(XDateTuple(693596) == XDateTuple(1900, 1, 1));
    UT_ASSERT(XDateTuple(693961) == XDateTuple(1901, 1, 1));
    UT_ASSERT(XDateTuple(729755) == XDateTuple(1999, 1, 1));
    UT_ASSERT(XDateTuple(730120) == XDateTuple(2000, 1, 1));
    UT_ASSERT(XDateTuple(730121) == XDateTuple(2000, 1, 2));
    UT_ASSERT(XDateTuple(730484) == XDateTuple(2000, 12, 30));
    UT_ASSERT(XDateTuple(730485) == XDateTuple(2000, 12, 31));
    UT_ASSERT(XDateTuple(730486) == XDateTuple(2001, 1, 1));

    UT_ASSERT(XDateTuple(733773) == XDateTuple(2010, 1, 1));
    UT_ASSERT(XDateTuple(733774) == XDateTuple(2010, 1, 2));
    UT_ASSERT(XDateTuple(733803) == XDateTuple(2010, 1, 31));
    UT_ASSERT(XDateTuple(733804) == XDateTuple(2010, 2, 1));
    UT_ASSERT(XDateTuple(733831) == XDateTuple(2010, 2, 28));
    UT_ASSERT(XDateTuple(733832) == XDateTuple(2010, 3, 1));
    UT_ASSERT(XDateTuple(733862) == XDateTuple(2010, 3, 31));
    UT_ASSERT(XDateTuple(733863) == XDateTuple(2010, 4, 1));
    UT_ASSERT(XDateTuple(733892) == XDateTuple(2010, 4, 30));
    UT_ASSERT(XDateTuple(733893) == XDateTuple(2010, 5, 1));
    UT_ASSERT(XDateTuple(733923) == XDateTuple(2010, 5, 31));
    UT_ASSERT(XDateTuple(733924) == XDateTuple(2010, 6, 1));
    UT_ASSERT(XDateTuple(733953) == XDateTuple(2010, 6, 30));
    UT_ASSERT(XDateTuple(733954) == XDateTuple(2010, 7, 1));
    UT_ASSERT(XDateTuple(733984) == XDateTuple(2010, 7, 31));
    UT_ASSERT(XDateTuple(733985) == XDateTuple(2010, 8, 1));
    UT_ASSERT(XDateTuple(734015) == XDateTuple(2010, 8, 31));
    UT_ASSERT(XDateTuple(734016) == XDateTuple(2010, 9, 1));
    UT_ASSERT(XDateTuple(734045) == XDateTuple(2010, 9, 30));
    UT_ASSERT(XDateTuple(734046) == XDateTuple(2010, 10, 1));
    UT_ASSERT(XDateTuple(734076) == XDateTuple(2010, 10, 31));
    UT_ASSERT(XDateTuple(734077) == XDateTuple(2010, 11, 1));
    UT_ASSERT(XDateTuple(734106) == XDateTuple(2010, 11, 30));
    UT_ASSERT(XDateTuple(734107) == XDateTuple(2010, 12, 1));
    UT_ASSERT(XDateTuple(734136) == XDateTuple(2010, 12, 30));
    UT_ASSERT(XDateTuple(734137) == XDateTuple(2010, 12, 31));

    UT_ASSERT(XDateTuple(734169) == XDateTuple(2011, 2, 1));
    UT_ASSERT(XDateTuple(734196) == XDateTuple(2011, 2, 28));
    UT_ASSERT(XDateTuple(734197) == XDateTuple(2011, 3, 1));

    UT_ASSERT(XDateTuple(734534) == XDateTuple(2012, 2, 1));
    UT_ASSERT(XDateTuple(734561) == XDateTuple(2012, 2, 28));
    UT_ASSERT(XDateTuple(734562) == XDateTuple(2012, 2, 29));
    UT_ASSERT(XDateTuple(734563) == XDateTuple(2012, 3, 1));

    UT_ASSERT_THROWS(XDateTuple(-1, 1, 1), XSDK::XException);
    UT_ASSERT_THROWS(XDateTuple(0, 1, 1), XSDK::XException);
    UT_ASSERT_THROWS(XDateTuple(1900, 2, 29), XSDK::XException);
    UT_ASSERT_THROWS(XDateTuple(1999, 2, 29), XSDK::XException);
}


void XDateTupleTest::TestEquality()
{
    vector<XDateTuple> dates;
    dates.push_back(XDateTuple());
    dates.push_back(XDateTuple(1, 1, 2));
    dates.push_back(XDateTuple(1, 2, 1));
    dates.push_back(XDateTuple(1, 2, 2));
    dates.push_back(XDateTuple(2, 1, 1));
    dates.push_back(XDateTuple(2, 2, 1));
    dates.push_back(XDateTuple(2, 1, 2));
    dates.push_back(XDateTuple(2, 2, 2));
    dates.push_back(XDateTuple(1999, 7, 6));
    dates.push_back(XDateTuple(2012, 12, 21));

    for(size_t i = 0; i < dates.size(); ++i)
    {
        for(size_t j = 0; i < dates.size(); ++i)
        {
            try
            {
                if(i == j)
                {
                    UT_ASSERT(dates[i] == dates[j]);
                    UT_ASSERT(!(dates[i] != dates[j]));
                    UT_ASSERT(!(dates[i] < dates[j]));
                    UT_ASSERT(dates[i] <= dates[j]);
                    UT_ASSERT(dates[i] >= dates[j]);
                    UT_ASSERT(!(dates[i] > dates[j]));
                }
                else
                {
                    UT_ASSERT(!(dates[i] == dates[j]));
                    UT_ASSERT(dates[i] != dates[j]);

                    if(i < j)
                    {
                        UT_ASSERT(dates[i] < dates[j]);
                        UT_ASSERT(dates[i] <= dates[j]);
                    }
                    else if(i > j)
                    {
                        UT_ASSERT(dates[i] >= dates[j]);
                        UT_ASSERT(dates[i] > dates[j]);
                    }
                }
            }
            catch(...)
            {
                printf("lhs [%s], rhs [%s]\n", dates[i].ToISOExtString().c_str(), dates[j].ToISOExtString().c_str());
                throw;
            }
        }
    }
}


void XDateTupleTest::TestDayOfGregorianCal()
{
    UT_ASSERT(XDateTuple(1, 1, 1).DayOfGregorianCal() == 1);
    UT_ASSERT(XDateTuple(1, 1, 2).DayOfGregorianCal() == 2);
    UT_ASSERT(XDateTuple(1, 2, 1).DayOfGregorianCal() == 32);
    UT_ASSERT(XDateTuple(2, 1, 1).DayOfGregorianCal() == 366);

    UT_ASSERT(XDateTuple(1945, 11, 12).DayOfGregorianCal() == 710347);
    UT_ASSERT(XDateTuple(1999, 1, 1).DayOfGregorianCal() == 729755);
    UT_ASSERT(XDateTuple(2000, 1, 1).DayOfGregorianCal() == 730120);
    UT_ASSERT(XDateTuple(2001, 1, 1).DayOfGregorianCal() == 730486);

    UT_ASSERT(XDateTuple(2010, 1, 1).DayOfGregorianCal() == 733773);
    UT_ASSERT(XDateTuple(2010, 1, 31).DayOfGregorianCal() == 733803);
    UT_ASSERT(XDateTuple(2010, 2, 1).DayOfGregorianCal() == 733804);
    UT_ASSERT(XDateTuple(2010, 2, 28).DayOfGregorianCal() == 733831);
    UT_ASSERT(XDateTuple(2010, 3, 1).DayOfGregorianCal() == 733832);
    UT_ASSERT(XDateTuple(2010, 3, 31).DayOfGregorianCal() == 733862);
    UT_ASSERT(XDateTuple(2010, 4, 1).DayOfGregorianCal() == 733863);
    UT_ASSERT(XDateTuple(2010, 4, 30).DayOfGregorianCal() == 733892);
    UT_ASSERT(XDateTuple(2010, 5, 1).DayOfGregorianCal() == 733893);
    UT_ASSERT(XDateTuple(2010, 5, 31).DayOfGregorianCal() == 733923);
    UT_ASSERT(XDateTuple(2010, 6, 1).DayOfGregorianCal() == 733924);
    UT_ASSERT(XDateTuple(2010, 6, 30).DayOfGregorianCal() == 733953);
    UT_ASSERT(XDateTuple(2010, 7, 1).DayOfGregorianCal() == 733954);
    UT_ASSERT(XDateTuple(2010, 7, 31).DayOfGregorianCal() == 733984);
    UT_ASSERT(XDateTuple(2010, 8, 1).DayOfGregorianCal() == 733985);
    UT_ASSERT(XDateTuple(2010, 8, 31).DayOfGregorianCal() == 734015);
    UT_ASSERT(XDateTuple(2010, 9, 1).DayOfGregorianCal() == 734016);
    UT_ASSERT(XDateTuple(2010, 9, 30).DayOfGregorianCal() == 734045);
    UT_ASSERT(XDateTuple(2010, 10, 1).DayOfGregorianCal() == 734046);
    UT_ASSERT(XDateTuple(2010, 10, 31).DayOfGregorianCal() == 734076);
    UT_ASSERT(XDateTuple(2010, 11, 1).DayOfGregorianCal() == 734077);
    UT_ASSERT(XDateTuple(2010, 11, 30).DayOfGregorianCal() == 734106);
    UT_ASSERT(XDateTuple(2010, 12, 1).DayOfGregorianCal() == 734107);
    UT_ASSERT(XDateTuple(2010, 12, 31).DayOfGregorianCal() == 734137);

    UT_ASSERT(XDateTuple(2012, 2, 1).DayOfGregorianCal() == 734534);
    UT_ASSERT(XDateTuple(2012, 2, 28).DayOfGregorianCal() == 734561);
    UT_ASSERT(XDateTuple(2012, 2, 29).DayOfGregorianCal() == 734562);
    UT_ASSERT(XDateTuple(2012, 3, 1).DayOfGregorianCal() == 734563);
}


void XDateTupleTest::TestDayOfYear()
{
    UT_ASSERT(XDateTuple(1999, 1, 1).DayOfYear() == 1);
    UT_ASSERT(XDateTuple(1999, 1, 2).DayOfYear() == 2);
    UT_ASSERT(XDateTuple(1999, 1, 3).DayOfYear() == 3);
    UT_ASSERT(XDateTuple(1999, 1, 31).DayOfYear() == 31);
    UT_ASSERT(XDateTuple(1999, 2, 1).DayOfYear() == 32);
    UT_ASSERT(XDateTuple(1999, 2, 28).DayOfYear() == 59);
    UT_ASSERT(XDateTuple(1999, 3, 1).DayOfYear() == 60);
    UT_ASSERT(XDateTuple(1999, 3, 31).DayOfYear() == 90);
    UT_ASSERT(XDateTuple(1999, 4, 1).DayOfYear() == 91);
    UT_ASSERT(XDateTuple(1999, 4, 30).DayOfYear() == 120);
    UT_ASSERT(XDateTuple(1999, 5, 1).DayOfYear() == 121);
    UT_ASSERT(XDateTuple(1999, 5, 31).DayOfYear() == 151);
    UT_ASSERT(XDateTuple(1999, 6, 1).DayOfYear() == 152);
    UT_ASSERT(XDateTuple(1999, 6, 30).DayOfYear() == 181);
    UT_ASSERT(XDateTuple(1999, 7, 1).DayOfYear() == 182);
    UT_ASSERT(XDateTuple(1999, 7, 31).DayOfYear() == 212);
    UT_ASSERT(XDateTuple(1999, 8, 1).DayOfYear() == 213);
    UT_ASSERT(XDateTuple(1999, 8, 31).DayOfYear() == 243);
    UT_ASSERT(XDateTuple(1999, 9, 1).DayOfYear() == 244);
    UT_ASSERT(XDateTuple(1999, 9, 30).DayOfYear() == 273);
    UT_ASSERT(XDateTuple(1999, 10, 1).DayOfYear() == 274);
    UT_ASSERT(XDateTuple(1999, 10, 31).DayOfYear() == 304);
    UT_ASSERT(XDateTuple(1999, 11, 1).DayOfYear() == 305);
    UT_ASSERT(XDateTuple(1999, 11, 30).DayOfYear() == 334);
    UT_ASSERT(XDateTuple(1999, 12, 1).DayOfYear() == 335);
    UT_ASSERT(XDateTuple(1999, 12, 31).DayOfYear() == 365);

    UT_ASSERT(XDateTuple(2000, 1, 1).DayOfYear() == 1);
    UT_ASSERT(XDateTuple(2000, 2, 1).DayOfYear() == 32);
    UT_ASSERT(XDateTuple(2000, 2, 29).DayOfYear() == 60);
    UT_ASSERT(XDateTuple(2000, 3, 1).DayOfYear() == 61);
    UT_ASSERT(XDateTuple(2000, 3, 31).DayOfYear() == 91);
    UT_ASSERT(XDateTuple(2000, 4, 1).DayOfYear() == 92);
    UT_ASSERT(XDateTuple(2000, 4, 30).DayOfYear() == 121);
    UT_ASSERT(XDateTuple(2000, 5, 1).DayOfYear() == 122);
    UT_ASSERT(XDateTuple(2000, 5, 31).DayOfYear() == 152);
    UT_ASSERT(XDateTuple(2000, 6, 1).DayOfYear() == 153);
    UT_ASSERT(XDateTuple(2000, 6, 30).DayOfYear() == 182);
    UT_ASSERT(XDateTuple(2000, 7, 1).DayOfYear() == 183);
    UT_ASSERT(XDateTuple(2000, 7, 31).DayOfYear() == 213);
    UT_ASSERT(XDateTuple(2000, 8, 1).DayOfYear() == 214);
    UT_ASSERT(XDateTuple(2000, 8, 31).DayOfYear() == 244);
    UT_ASSERT(XDateTuple(2000, 9, 1).DayOfYear() == 245);
    UT_ASSERT(XDateTuple(2000, 9, 30).DayOfYear() == 274);
    UT_ASSERT(XDateTuple(2000, 10, 1).DayOfYear() == 275);
    UT_ASSERT(XDateTuple(2000, 10, 31).DayOfYear() == 305);
    UT_ASSERT(XDateTuple(2000, 11, 1).DayOfYear() == 306);
    UT_ASSERT(XDateTuple(2000, 11, 30).DayOfYear() == 335);
    UT_ASSERT(XDateTuple(2000, 12, 1).DayOfYear() == 336);
    UT_ASSERT(XDateTuple(2000, 12, 31).DayOfYear() == 366);
}


void XDateTupleTest::TestDayOfWeek_test(XDateTuple date, int day, int expectedMonth, int expectedDay)
{
    try
    {
        date.SetDayOfYear(day);
        UT_ASSERT(date.Month() == expectedMonth);
        UT_ASSERT(date.Day() == expectedDay);
    }
    catch(...)
    {
        printf("orig date [%s], new day [%d]\n", date.ToISOExtString().c_str(), day);
        throw;
    }
}


void XDateTupleTest::TestDayOfWeek()
{
    UT_ASSERT(XDateTuple(1, 1, 1).DayOfWeek() == 1);
    UT_ASSERT(XDateTuple(1, 1, 2).DayOfWeek() == 2);
    UT_ASSERT(XDateTuple(1, 1, 3).DayOfWeek() == 3);
    UT_ASSERT(XDateTuple(1, 1, 4).DayOfWeek() == 4);
    UT_ASSERT(XDateTuple(1, 1, 5).DayOfWeek() == 5);
    UT_ASSERT(XDateTuple(1, 1, 6).DayOfWeek() == 6);
    UT_ASSERT(XDateTuple(1, 1, 7).DayOfWeek() == 0);
    UT_ASSERT(XDateTuple(1, 1, 8).DayOfWeek() == 1);
    UT_ASSERT(XDateTuple(1, 1, 9).DayOfWeek() == 2);
    UT_ASSERT(XDateTuple(2, 1, 1).DayOfWeek() == 2);
    UT_ASSERT(XDateTuple(3, 1, 1).DayOfWeek() == 3);
    UT_ASSERT(XDateTuple(4, 1, 1).DayOfWeek() == 4);
    UT_ASSERT(XDateTuple(5, 1, 1).DayOfWeek() == 6);
    UT_ASSERT(XDateTuple(2000, 1, 1).DayOfWeek() == 6);
    UT_ASSERT(XDateTuple(2010, 8, 22).DayOfWeek() == 0);
    UT_ASSERT(XDateTuple(2010, 8, 23).DayOfWeek() == 1);
    UT_ASSERT(XDateTuple(2010, 8, 24).DayOfWeek() == 2);
    UT_ASSERT(XDateTuple(2010, 8, 25).DayOfWeek() == 3);
    UT_ASSERT(XDateTuple(2010, 8, 26).DayOfWeek() == 4);
    UT_ASSERT(XDateTuple(2010, 8, 27).DayOfWeek() == 5);
    UT_ASSERT(XDateTuple(2010, 8, 28).DayOfWeek() == 6);
    UT_ASSERT(XDateTuple(2010, 8, 29).DayOfWeek() == 0);

    TestDayOfWeek_test(XDateTuple(1999, 1, 9), 1, 1, 1);
    TestDayOfWeek_test(XDateTuple(1999, 2, 8), 2, 1, 2);
    TestDayOfWeek_test(XDateTuple(1999, 3, 7), 3, 1, 3);
    TestDayOfWeek_test(XDateTuple(1999, 5, 6), 31, 1, 31);

    TestDayOfWeek_test(XDateTuple(1999, 6, 5), 32, 2, 1);
    TestDayOfWeek_test(XDateTuple(1999, 7, 4), 59, 2, 28);
    TestDayOfWeek_test(XDateTuple(1999, 8, 3), 60, 3, 1);
    TestDayOfWeek_test(XDateTuple(1999, 9, 2), 90, 3, 31);
    TestDayOfWeek_test(XDateTuple(1999, 1, 1), 91, 4, 1);
    TestDayOfWeek_test(XDateTuple(1999, 2, 1), 120, 4, 30);
    TestDayOfWeek_test(XDateTuple(1999, 3, 2), 121, 5, 1);
    TestDayOfWeek_test(XDateTuple(1999, 4, 2), 151, 5, 31);
    TestDayOfWeek_test(XDateTuple(1999, 5, 3), 152, 6, 1);
    TestDayOfWeek_test(XDateTuple(1999, 6, 3), 181, 6, 30);
    TestDayOfWeek_test(XDateTuple(1999, 7, 4), 182, 7, 1);
    TestDayOfWeek_test(XDateTuple(1999, 8, 4), 212, 7, 31);
    TestDayOfWeek_test(XDateTuple(1999, 9, 5), 213, 8, 1);
    TestDayOfWeek_test(XDateTuple(1999, 1, 5), 243, 8, 31);
    TestDayOfWeek_test(XDateTuple(1999, 2, 6), 244, 9, 1);
    TestDayOfWeek_test(XDateTuple(1999, 3, 6), 273, 9, 30);
    TestDayOfWeek_test(XDateTuple(1999, 4, 7), 274, 10, 1);
    TestDayOfWeek_test(XDateTuple(1999, 5, 7), 304, 10, 31);
    TestDayOfWeek_test(XDateTuple(1999, 6, 8), 305, 11, 1);
    TestDayOfWeek_test(XDateTuple(1999, 7, 8), 334, 11, 30);
    TestDayOfWeek_test(XDateTuple(1999, 8, 9), 335, 12, 1);
    TestDayOfWeek_test(XDateTuple(1999, 9, 9), 365, 12, 31);

    TestDayOfWeek_test(XDateTuple(2000, 1, 1), 1, 1, 1);
    TestDayOfWeek_test(XDateTuple(2000, 2, 2), 32, 2, 1);
    TestDayOfWeek_test(XDateTuple(2000, 3, 3), 60, 2, 29);
    TestDayOfWeek_test(XDateTuple(2000, 4, 4), 61, 3, 1);
    TestDayOfWeek_test(XDateTuple(2000, 5, 5), 91, 3, 31);
    TestDayOfWeek_test(XDateTuple(2000, 6, 6), 92, 4, 1);
    TestDayOfWeek_test(XDateTuple(2000, 7, 7), 121, 4, 30);
    TestDayOfWeek_test(XDateTuple(2000, 8, 8), 122, 5, 1);
    TestDayOfWeek_test(XDateTuple(2000, 9, 9), 152, 5, 31);
    TestDayOfWeek_test(XDateTuple(2000, 1, 9), 153, 6, 1);
    TestDayOfWeek_test(XDateTuple(2000, 2, 8), 182, 6, 30);
    TestDayOfWeek_test(XDateTuple(2000, 3, 7), 183, 7, 1);
    TestDayOfWeek_test(XDateTuple(2000, 4, 6), 213, 7, 31);
    TestDayOfWeek_test(XDateTuple(2000, 5, 5), 214, 8, 1);
    TestDayOfWeek_test(XDateTuple(2000, 6, 4), 244, 8, 31);
    TestDayOfWeek_test(XDateTuple(2000, 7, 3), 245, 9, 1);
    TestDayOfWeek_test(XDateTuple(2000, 8, 2), 274, 9, 30);
    TestDayOfWeek_test(XDateTuple(2000, 9, 1), 275, 10, 1);
    TestDayOfWeek_test(XDateTuple(2000, 1, 2), 305, 10, 31);
    TestDayOfWeek_test(XDateTuple(2000, 2, 3), 306, 11, 1);
    TestDayOfWeek_test(XDateTuple(2000, 3, 4), 335, 11, 30);
    TestDayOfWeek_test(XDateTuple(2000, 4, 5), 336, 12, 1);
    TestDayOfWeek_test(XDateTuple(2000, 5, 6), 366, 12, 31);
}


void XDateTupleTest::TestSub()
{
    UT_ASSERT(XDateTuple() - XDateTuple() == XDuration::Zero());
    UT_ASSERT(XDateTuple(2012, 12, 21) - XDateTuple(2012, 12, 21) == XDuration::Zero());
    UT_ASSERT(XDateTuple(2000, 7, 6) - XDateTuple(1998, 7, 6) == XDuration(DAYS, 731));
    UT_ASSERT(XDateTuple(1998, 7, 6) - XDateTuple(2000, 7, 6) == XDuration(DAYS, -731));
    UT_ASSERT(XDateTuple(1999, 7, 6) - XDateTuple(1998, 7, 6) == XDuration(DAYS, 365));
    UT_ASSERT(XDateTuple(1998, 7, 6) - XDateTuple(1999, 7, 6) == XDuration(DAYS, -365));
    UT_ASSERT(XDateTuple(1999, 6, 6) - XDateTuple(1999, 5, 6) == XDuration(DAYS, 31));
    UT_ASSERT(XDateTuple(1999, 5, 6) - XDateTuple(1999, 6, 6) == XDuration(DAYS, -31));
}


void XDateTupleTest::TestYear()
{
    UT_ASSERT(XDateTuple().Year() == 1);
    UT_ASSERT(XDateTuple(1999, 1, 1).Year() == 1999);
    UT_ASSERT(XDateTuple(1999, 12, 31).Year() == 1999);
    UT_ASSERT(XDateTuple(2000, 1, 1).Year() == 2000);
    UT_ASSERT(XDateTuple(2000, 12, 31).Year() == 2000);
    UT_ASSERT(XDateTuple(2011, 3, 9).Year() == 2011);

    {
        XDateTuple xdt;
        xdt.SetYear(1999);
        UT_ASSERT(xdt == XDateTuple(1999, 1, 1));

        xdt.SetYear(2012);
        UT_ASSERT(xdt == XDateTuple(2012, 1, 1));
    }

    {
        XDateTuple xdt;
        xdt.SetYear(2012);
        UT_ASSERT(xdt == XDateTuple(2012, 1, 1));

        xdt.SetYear(1999);
        UT_ASSERT(xdt == XDateTuple(1999, 1, 1));
    }

    {
        XDateTuple xdt = XDateTuple(2000, 7, 4);
        xdt.SetYear(1999);
        UT_ASSERT(xdt == XDateTuple(1999, 7, 4));
    }

    {
        XDateTuple xdt = XDateTuple(2000, 2, 29);
        UT_ASSERT_THROWS(xdt.SetYear(1999), XSDK::XException);
        UT_ASSERT(xdt == XDateTuple(2000, 2, 29));
        xdt.SetYear(2012);
        UT_ASSERT(xdt == XDateTuple(2012, 2, 29));
    }

    {
        XDateTuple xdt = XDateTuple(2000, 2, 28);
        xdt.SetYear(1999);
        UT_ASSERT(xdt == XDateTuple(1999, 2, 28));
        xdt.SetYear(2012);
        UT_ASSERT(xdt == XDateTuple(2012, 2, 28));
    }

    {
        XDateTuple xdt;
        UT_ASSERT_THROWS(xdt.SetYear(0), XSDK::XException);
        UT_ASSERT_THROWS(xdt.SetYear(-1), XSDK::XException);
    }
}


void XDateTupleTest::TestMonth()
{
    UT_ASSERT(XDateTuple().Month() == 1);
    UT_ASSERT(XDateTuple(1999, 1, 1).Month() == 1);
    UT_ASSERT(XDateTuple(1999, 1, 31).Month() == 1);
    UT_ASSERT(XDateTuple(1999, 2, 28).Month() == 2);
    UT_ASSERT(XDateTuple(1999, 3, 1).Month() == 3);
    UT_ASSERT(XDateTuple(1999, 7, 4).Month() == 7);
    UT_ASSERT(XDateTuple(1999, 12, 1).Month() == 12);
    UT_ASSERT(XDateTuple(1999, 12, 31).Month() == 12);

    UT_ASSERT(XDateTuple().Month() == 1);
    UT_ASSERT(XDateTuple(2000, 1, 1).Month() == 1);
    UT_ASSERT(XDateTuple(2000, 1, 31).Month() == 1);
    UT_ASSERT(XDateTuple(2000, 2, 28).Month() == 2);
    UT_ASSERT(XDateTuple(2000, 2, 29).Month() == 2);
    UT_ASSERT(XDateTuple(2000, 3, 1).Month() == 3);
    UT_ASSERT(XDateTuple(2000, 7, 4).Month() == 7);
    UT_ASSERT(XDateTuple(2000, 12, 1).Month() == 12);
    UT_ASSERT(XDateTuple(2000, 12, 31).Month() == 12);

    {
        XDateTuple xdt;
        xdt.SetMonth(1);
        UT_ASSERT(xdt == XDateTuple(1, 1, 1));

        xdt.SetMonth(2);
        UT_ASSERT(xdt == XDateTuple(1, 2, 1));

        xdt.SetMonth(7);
        UT_ASSERT(xdt == XDateTuple(1, 7, 1));

        xdt.SetMonth(12);
        UT_ASSERT(xdt == XDateTuple(1, 12, 1));
    }

    {
        XDateTuple xdt = XDateTuple(1999, 7, 31);
        xdt.SetMonth(7);
        UT_ASSERT(xdt == XDateTuple(1999, 7, 31));

        xdt.SetMonth(1);
        UT_ASSERT(xdt == XDateTuple(1999, 1, 31));

        xdt.SetMonth(12);
        UT_ASSERT(xdt == XDateTuple(1999, 12, 31));
    }

    {
        XDateTuple xdt = XDateTuple(2000, 1, 29);
        xdt.SetMonth(1);
        UT_ASSERT(xdt == XDateTuple(2000, 1, 29));

        xdt.SetMonth(2);
        UT_ASSERT(xdt == XDateTuple(2000, 2, 29));

        xdt.SetMonth(3);
        UT_ASSERT(xdt == XDateTuple(2000, 3, 29));
    }

    {
        XDateTuple xdt = XDateTuple(1999, 3, 29);
        UT_ASSERT_THROWS(xdt.SetMonth(2), XSDK::XException);
        UT_ASSERT(xdt == XDateTuple(1999, 3, 29));
        xdt.SetMonth(4);
        UT_ASSERT(xdt == XDateTuple(1999, 4, 29));
    }

    {
        XDateTuple xdt = XDateTuple(2000, 3, 30);
        UT_ASSERT_THROWS(xdt.SetMonth(2), XSDK::XException);
        UT_ASSERT(xdt == XDateTuple(2000, 3, 30));
        xdt.SetMonth(4);
        UT_ASSERT(xdt == XDateTuple(2000, 4, 30));
    }

    {
        XDateTuple xdt = XDateTuple(2000, 3, 28);
        xdt.SetMonth(2);
        UT_ASSERT(xdt == XDateTuple(2000, 2, 28));
        xdt.SetMonth(4);
        UT_ASSERT(xdt == XDateTuple(2000, 4, 28));
    }

    {
        XDateTuple xdt = XDateTuple(2011, 1, 31);
        UT_ASSERT_THROWS(xdt.SetMonth(2), XSDK::XException);
        UT_ASSERT_THROWS(xdt.SetMonth(4), XSDK::XException);
        UT_ASSERT_THROWS(xdt.SetMonth(6), XSDK::XException);
        UT_ASSERT_THROWS(xdt.SetMonth(9), XSDK::XException);
        UT_ASSERT_THROWS(xdt.SetMonth(11), XSDK::XException);
        UT_ASSERT(xdt == XDateTuple(2011, 1, 31));
    }
}


void XDateTupleTest::TestDay()
{
    UT_ASSERT(XDateTuple().Day() == 1);
    UT_ASSERT(XDateTuple(1999, 1, 1).Day() == 1);
    UT_ASSERT(XDateTuple(1999, 1, 4).Day() == 4);
    UT_ASSERT(XDateTuple(1999, 1, 12).Day() == 12);
    UT_ASSERT(XDateTuple(1999, 1, 31).Day() == 31);
    UT_ASSERT(XDateTuple(1999, 2, 1).Day() == 1);
    UT_ASSERT(XDateTuple(1999, 2, 28).Day() == 28);
    UT_ASSERT(XDateTuple(1999, 3, 1).Day() == 1);
    UT_ASSERT(XDateTuple(1999, 7, 4).Day() == 4);
    UT_ASSERT(XDateTuple(1999, 12, 31).Day() == 31);

    UT_ASSERT(XDateTuple(2000, 1, 1).Day() == 1);
    UT_ASSERT(XDateTuple(2000, 1, 4).Day() == 4);
    UT_ASSERT(XDateTuple(2000, 1, 12).Day() == 12);
    UT_ASSERT(XDateTuple(2000, 1, 31).Day() == 31);
    UT_ASSERT(XDateTuple(2000, 2, 1).Day() == 1);
    UT_ASSERT(XDateTuple(2000, 2, 28).Day() == 28);
    UT_ASSERT(XDateTuple(2000, 2, 29).Day() == 29);
    UT_ASSERT(XDateTuple(2000, 3, 1).Day() == 1);
    UT_ASSERT(XDateTuple(2000, 7, 4).Day() == 4);
    UT_ASSERT(XDateTuple(2000, 12, 31).Day() == 31);

    {
        XDateTuple xdt;
        xdt.SetDay(1);
        UT_ASSERT(xdt == XDateTuple(1, 1, 1));

        xdt.SetDay(2);
        UT_ASSERT(xdt == XDateTuple(1, 1, 2));

        xdt.SetDay(7);
        UT_ASSERT(xdt == XDateTuple(1, 1, 7));

        xdt.SetDay(31);
        UT_ASSERT(xdt == XDateTuple(1, 1, 31));
    }

    {
        XDateTuple xdt = XDateTuple(1999, 7, 31);
        xdt.SetDay(31);
        UT_ASSERT(xdt == XDateTuple(1999, 7, 31));

        xdt.SetDay(1);
        UT_ASSERT(xdt == XDateTuple(1999, 7, 1));

        xdt.SetDay(12);
        UT_ASSERT(xdt == XDateTuple(1999, 7, 12));
    }

    {
        XDateTuple xdt = XDateTuple(2000, 2, 29);
        xdt.SetDay(7);
        UT_ASSERT(xdt == XDateTuple(2000, 2, 7));

        xdt.SetDay(29);
        UT_ASSERT(xdt == XDateTuple(2000, 2, 29));

        xdt.SetDay(28);
        UT_ASSERT(xdt == XDateTuple(2000, 2, 28));
    }

    {
        XDateTuple xdt = XDateTuple(2011, 1, 31);
        UT_ASSERT_THROWS(xdt.SetDay(32), XSDK::XException);
        UT_ASSERT(xdt == XDateTuple(2011, 1, 31));
    }

    {
        XDateTuple xdt = XDateTuple(1999, 2, 5);
        UT_ASSERT_THROWS(xdt.SetDay(29), XSDK::XException);
        UT_ASSERT(xdt == XDateTuple(1999, 2, 5));
        xdt.SetDay(28);
        UT_ASSERT(xdt == XDateTuple(1999, 2, 28));
    }

    {
        XDateTuple xdt = XDateTuple(2000, 2, 5);
        UT_ASSERT_THROWS(xdt.SetDay(30), XSDK::XException);
        UT_ASSERT(xdt == XDateTuple(2000, 2, 5));
        xdt.SetDay(29);
        UT_ASSERT(xdt == XDateTuple(2000, 2, 29));
    }

    {
        XDateTuple xdt = XDateTuple(2011, 3, 31);
        UT_ASSERT_THROWS(xdt.SetDay(32), XSDK::XException);
    }

    {
        XDateTuple xdt = XDateTuple(2011, 4, 30);
        UT_ASSERT_THROWS(xdt.SetDay(31), XSDK::XException);
    }

    {
        XDateTuple xdt = XDateTuple(2011, 5, 30);
        UT_ASSERT_THROWS(xdt.SetDay(32), XSDK::XException);
    }

    {
        XDateTuple xdt = XDateTuple(2011, 6, 7);
        UT_ASSERT_THROWS(xdt.SetDay(31), XSDK::XException);
    }

    {
        XDateTuple xdt = XDateTuple(2011, 7, 7);
        UT_ASSERT_THROWS(xdt.SetDay(32), XSDK::XException);
    }

    {
        XDateTuple xdt = XDateTuple(2011, 8, 7);
        UT_ASSERT_THROWS(xdt.SetDay(32), XSDK::XException);
    }

    {
        XDateTuple xdt = XDateTuple(2011, 9, 22);
        UT_ASSERT_THROWS(xdt.SetDay(31), XSDK::XException);
    }

    {
        XDateTuple xdt = XDateTuple(2011, 10, 22);
        UT_ASSERT_THROWS(xdt.SetDay(32), XSDK::XException);
    }

    {
        XDateTuple xdt = XDateTuple(2011, 11, 22);
        UT_ASSERT_THROWS(xdt.SetDay(31), XSDK::XException);
    }

    {
        XDateTuple xdt = XDateTuple(2011, 12, 31);
        UT_ASSERT_THROWS(xdt.SetDay(32), XSDK::XException);
    }
}


void XDateTupleTest::TestEndOfMonth()
{
    UT_ASSERT(XDateTuple(1999, 1, 1).EndOfMonth() == XDateTuple(1999, 1, 31));
    UT_ASSERT(XDateTuple(1999, 2, 1).EndOfMonth() == XDateTuple(1999, 2, 28));
    UT_ASSERT(XDateTuple(2000, 2, 1).EndOfMonth() == XDateTuple(2000, 2, 29));
    UT_ASSERT(XDateTuple(1999, 3, 1).EndOfMonth() == XDateTuple(1999, 3, 31));
    UT_ASSERT(XDateTuple(1999, 4, 1).EndOfMonth() == XDateTuple(1999, 4, 30));
    UT_ASSERT(XDateTuple(1999, 5, 1).EndOfMonth() == XDateTuple(1999, 5, 31));
    UT_ASSERT(XDateTuple(1999, 6, 1).EndOfMonth() == XDateTuple(1999, 6, 30));
    UT_ASSERT(XDateTuple(1999, 7, 1).EndOfMonth() == XDateTuple(1999, 7, 31));
    UT_ASSERT(XDateTuple(1999, 8, 1).EndOfMonth() == XDateTuple(1999, 8, 31));
    UT_ASSERT(XDateTuple(1999, 9, 1).EndOfMonth() == XDateTuple(1999, 9, 30));
    UT_ASSERT(XDateTuple(1999, 10, 1).EndOfMonth() == XDateTuple(1999, 10, 31));
    UT_ASSERT(XDateTuple(1999, 11, 1).EndOfMonth() == XDateTuple(1999, 11, 30));
    UT_ASSERT(XDateTuple(1999, 12, 1).EndOfMonth() == XDateTuple(1999, 12, 31));
}


void XDateTupleTest::TestToISOExtString()
{
    UT_ASSERT(XDateTuple().ToISOExtString() == "0001-01-01");
    UT_ASSERT(XDateTuple(2, 3, 4).ToISOExtString() == "0002-03-04");
    UT_ASSERT(XDateTuple(40, 7, 7).ToISOExtString() == "0040-07-07");
    UT_ASSERT(XDateTuple(602, 9, 17).ToISOExtString() == "0602-09-17");
    UT_ASSERT(XDateTuple(2012, 12, 21).ToISOExtString() == "2012-12-21");
    UT_ASSERT(XDateTuple(12345, 12, 21).ToISOExtString() == "+12345-12-21");
}


void XDateTupleTest::TestToCustomString()
{
    UT_ASSERT(XDateTuple().ToCustomString("YYYY-MM-DD") == "0001-01-01");
    UT_ASSERT(XDateTuple(2, 3, 4).ToCustomString("YYYY-MM-DD") == "0002-03-04");
    UT_ASSERT(XDateTuple(40, 7, 7).ToCustomString("YYYY-MM-DD") == "0040-07-07");
    UT_ASSERT(XDateTuple(602, 9, 17).ToCustomString("YYYY-MM-DD") == "0602-09-17");
    UT_ASSERT(XDateTuple(2012, 12, 21).ToCustomString("YYYY-MM-DD") == "2012-12-21");

    UT_ASSERT(XDateTuple().ToCustomString("YYYYMMDD") == "00010101");
    UT_ASSERT(XDateTuple(2, 3, 4).ToCustomString("YYYYMMDD") == "00020304");
    UT_ASSERT(XDateTuple(40, 7, 7).ToCustomString("YYYYMMDD") == "00400707");
    UT_ASSERT(XDateTuple(602, 9, 17).ToCustomString("YYYYMMDD") == "06020917");
    UT_ASSERT(XDateTuple(2012, 12, 21).ToCustomString("YYYYMMDD") == "20121221");

    UT_ASSERT(XDateTuple().ToCustomString("YY-MM-DD") == "01-01-01");
    UT_ASSERT(XDateTuple(2, 3, 4).ToCustomString("YY-MM-DD") == "02-03-04");
    UT_ASSERT(XDateTuple(40, 7, 7).ToCustomString("YY-MM-DD") == "40-07-07");
    UT_ASSERT(XDateTuple(602, 9, 17).ToCustomString("YY-MM-DD") == "02-09-17");
    UT_ASSERT(XDateTuple(2012, 12, 21).ToCustomString("YY-MM-DD") == "12-12-21");

    UT_ASSERT(XDateTuple().ToCustomString("MM/DD/YYYY") == "01/01/0001");
    UT_ASSERT(XDateTuple(2, 3, 4).ToCustomString("MM/DD/YYYY") == "03/04/0002");
    UT_ASSERT(XDateTuple(40, 7, 7).ToCustomString("MM/DD/YYYY") == "07/07/0040");
    UT_ASSERT(XDateTuple(602, 9, 17).ToCustomString("MM/DD/YYYY") == "09/17/0602");
    UT_ASSERT(XDateTuple(2012, 12, 21).ToCustomString("MM/DD/YYYY") == "12/21/2012");

    UT_ASSERT_THROWS(XDateTuple(12345, 12, 21).ToCustomString("YYYY-MM-DD"), XException);
}


void XDateTupleTest::TestFromISOExtString()
{
    UT_ASSERT_THROWS(XDateTuple::FromISOExtString(""), XException);
    UT_ASSERT_THROWS(XDateTuple::FromISOExtString("990704"), XException);
    UT_ASSERT_THROWS(XDateTuple::FromISOExtString("0100704"), XException);
    UT_ASSERT_THROWS(XDateTuple::FromISOExtString("2010070"), XException);
    UT_ASSERT_THROWS(XDateTuple::FromISOExtString("2010070 "), XException);
    UT_ASSERT_THROWS(XDateTuple::FromISOExtString("120100704"), XException);
    UT_ASSERT_THROWS(XDateTuple::FromISOExtString("-0100704"), XException);
    UT_ASSERT_THROWS(XDateTuple::FromISOExtString("+0100704"), XException);
    UT_ASSERT_THROWS(XDateTuple::FromISOExtString("2010070a"), XException);
    UT_ASSERT_THROWS(XDateTuple::FromISOExtString("20100a04"), XException);
    UT_ASSERT_THROWS(XDateTuple::FromISOExtString("2010a704"), XException);

    UT_ASSERT_THROWS(XDateTuple::FromISOExtString("99-07-04"), XException);
    UT_ASSERT_THROWS(XDateTuple::FromISOExtString("010-07-04"), XException);
    UT_ASSERT_THROWS(XDateTuple::FromISOExtString("2010-07-0"), XException);
    UT_ASSERT_THROWS(XDateTuple::FromISOExtString("2010-07-0 "), XException);
    UT_ASSERT_THROWS(XDateTuple::FromISOExtString("12010-07-04"), XException);
    UT_ASSERT_THROWS(XDateTuple::FromISOExtString("-010-07-04"), XException);
    UT_ASSERT_THROWS(XDateTuple::FromISOExtString("+010-07-04"), XException);
    UT_ASSERT_THROWS(XDateTuple::FromISOExtString("2010-07-0a"), XException);
    UT_ASSERT_THROWS(XDateTuple::FromISOExtString("2010-0a-04"), XException);
    UT_ASSERT_THROWS(XDateTuple::FromISOExtString("2010-a7-04"), XException);
    UT_ASSERT_THROWS(XDateTuple::FromISOExtString("2010/07/04"), XException);
    UT_ASSERT_THROWS(XDateTuple::FromISOExtString("2010/7/04"), XException);
    UT_ASSERT_THROWS(XDateTuple::FromISOExtString("2010/7/4"), XException);
    UT_ASSERT_THROWS(XDateTuple::FromISOExtString("2010/07/4"), XException);
    UT_ASSERT_THROWS(XDateTuple::FromISOExtString("2010-7-04"), XException);
    UT_ASSERT_THROWS(XDateTuple::FromISOExtString("2010-7-4"), XException);
    UT_ASSERT_THROWS(XDateTuple::FromISOExtString("2010-07-4"), XException);

    UT_ASSERT_THROWS(XDateTuple::FromISOExtString("99-Jul-04"), XException);
    UT_ASSERT_THROWS(XDateTuple::FromISOExtString("010-Jul-04"), XException);
    UT_ASSERT_THROWS(XDateTuple::FromISOExtString("2010-Jul-0"), XException);
    UT_ASSERT_THROWS(XDateTuple::FromISOExtString("2010Jul0 "), XException);
    UT_ASSERT_THROWS(XDateTuple::FromISOExtString("12010-Jul-04"), XException);
    UT_ASSERT_THROWS(XDateTuple::FromISOExtString("-010-Jul-04"), XException);
    UT_ASSERT_THROWS(XDateTuple::FromISOExtString("+010-Jul-04"), XException);
    UT_ASSERT_THROWS(XDateTuple::FromISOExtString("2010-Jul-0a"), XException);
    UT_ASSERT_THROWS(XDateTuple::FromISOExtString("2010-Jua-04"), XException);
    UT_ASSERT_THROWS(XDateTuple::FromISOExtString("2010-Jal-04"), XException);
    UT_ASSERT_THROWS(XDateTuple::FromISOExtString("2010-aul-04"), XException);

    UT_ASSERT(XDateTuple::FromISOExtString("1999-07-06") == XDateTuple(1999, 7, 6));
    UT_ASSERT(XDateTuple::FromISOExtString("+01999-07-06") == XDateTuple(1999, 7, 6));
    UT_ASSERT(XDateTuple::FromISOExtString("1999-07-06 ") == XDateTuple(1999, 7, 6));
    UT_ASSERT(XDateTuple::FromISOExtString(" 1999-07-06") == XDateTuple(1999, 7, 6));
    UT_ASSERT(XDateTuple::FromISOExtString(" 1999-07-06 ") == XDateTuple(1999, 7, 6));
}


void XTimeTupleTest::TestConstructor()
{
    UT_ASSERT_THROWS(XTimeTuple(-1, 0, 0), XSDK::XException);
    UT_ASSERT_THROWS(XTimeTuple(24, 0, 0), XSDK::XException);
    UT_ASSERT_THROWS(XTimeTuple(0, -1, 0), XSDK::XException);
    UT_ASSERT_THROWS(XTimeTuple(0, 60, 0), XSDK::XException);
    UT_ASSERT_THROWS(XTimeTuple(0, 0, -1), XSDK::XException);
    UT_ASSERT_THROWS(XTimeTuple(0, 0, 60), XSDK::XException);
}

void XTimeTupleTest::TestEquality()
{
    vector<XTimeTuple> times;
    times.push_back(XTimeTuple());
    times.push_back(XTimeTuple(0, 0, 1));
    times.push_back(XTimeTuple(0, 1, 0));
    times.push_back(XTimeTuple(0, 1, 1));
    times.push_back(XTimeTuple(1, 0, 0));
    times.push_back(XTimeTuple(1, 1, 0));
    times.push_back(XTimeTuple(1, 0, 1));
    times.push_back(XTimeTuple(1, 1, 1));
    times.push_back(XTimeTuple(12, 42, 3));
    times.push_back(XTimeTuple(23, 59, 59));

    for(size_t i = 0; i < times.size(); ++i)
    {
        for(size_t j = 0; i < times.size(); ++i)
        {
            try
            {
                if(i == j)
                {
                    UT_ASSERT(times[i] == times[j]);
                    UT_ASSERT(!(times[i] != times[j]));
                    UT_ASSERT(!(times[i] < times[j]));
                    UT_ASSERT(times[i] <= times[j]);
                    UT_ASSERT(times[i] >= times[j]);
                    UT_ASSERT(!(times[i] > times[j]));
                }
                else
                {
                    UT_ASSERT(!(times[i] == times[j]));
                    UT_ASSERT(times[i] != times[j]);

                    if(i < j)
                    {
                        UT_ASSERT(times[i] < times[j]);
                        UT_ASSERT(times[i] <= times[j]);
                    }
                    else if(i > j)
                    {
                        UT_ASSERT(times[i] >= times[j]);
                        UT_ASSERT(times[i] > times[j]);
                    }
                }
            }
            catch(...)
            {
                printf("lhs [%s], rhs [%s]\n", times[i].ToISOExtString().c_str(), times[j].ToISOExtString().c_str());
                throw;
            }
        }
    }
}


void XTimeTupleTest::TestSub()
{
    UT_ASSERT(XTimeTuple() - XTimeTuple() == XDuration::Zero());
    UT_ASSERT(XTimeTuple(23, 59, 59) - XTimeTuple(23, 59, 59) == XDuration::Zero());
    UT_ASSERT(XTimeTuple(23, 59, 59) - XTimeTuple(0, 0, 0) == XDuration(SECONDS, 86399));
    UT_ASSERT(XTimeTuple(0, 0, 0) - XTimeTuple(23, 59, 59) == XDuration(SECONDS, -86399));
    UT_ASSERT(XTimeTuple(7, 12, 52) - XTimeTuple(12, 30, 33) == XDuration(SECONDS, -19061));
    UT_ASSERT(XTimeTuple(12, 30, 33) - XTimeTuple(7, 12, 52) == XDuration(SECONDS, 19061));
    UT_ASSERT(XTimeTuple(12, 30, 33) - XTimeTuple(14, 30, 33) == XDuration(SECONDS, -7200));
    UT_ASSERT(XTimeTuple(14, 30, 33) - XTimeTuple(12, 30, 33) == XDuration(SECONDS, 7200));
    UT_ASSERT(XTimeTuple(12, 30, 33) - XTimeTuple(12, 34, 33) == XDuration(SECONDS, -240));
    UT_ASSERT(XTimeTuple(12, 34, 33) - XTimeTuple(12, 30, 33) == XDuration(SECONDS, 240));
    UT_ASSERT(XTimeTuple(12, 30, 33) - XTimeTuple(12, 30, 34) == XDuration(SECONDS, -1));
    UT_ASSERT(XTimeTuple(12, 30, 34) - XTimeTuple(12, 30, 33) == XDuration(SECONDS, 1));
}


void XTimeTupleTest::TestHour()
{
    UT_ASSERT(XTimeTuple().Hour() == 0);
    UT_ASSERT(XTimeTuple(5, 0, 0).Hour() == 5);
    UT_ASSERT(XTimeTuple(5, 59, 59).Hour() == 5);
    UT_ASSERT(XTimeTuple(23, 0, 0).Hour() == 23);
    UT_ASSERT(XTimeTuple(23, 59, 59).Hour() == 23);

    {
        XTimeTuple xtt;
        xtt.SetHour(0);
        UT_ASSERT(xtt == XTimeTuple(0, 0, 0));
    }

    {
        XTimeTuple xtt;
        xtt.SetHour(9);
        UT_ASSERT(xtt == XTimeTuple(9, 0, 0));
    }

    {
        XTimeTuple xtt;
        xtt.SetHour(23);
        UT_ASSERT(xtt == XTimeTuple(23, 0, 0));
    }

    {
        XTimeTuple xtt = XTimeTuple(23, 59, 59);
        xtt.SetHour(0);
        UT_ASSERT(xtt == XTimeTuple(0, 59, 59));
    }

    {
        XTimeTuple xtt = XTimeTuple(23, 59, 59);
        xtt.SetHour(9);
        UT_ASSERT(xtt == XTimeTuple(9, 59, 59));
    }

    {
        XTimeTuple xtt = XTimeTuple(23, 59, 59);
        xtt.SetHour(23);
        UT_ASSERT(xtt == XTimeTuple(23, 59, 59));
    }

    {
        XTimeTuple xtt;
        UT_ASSERT_THROWS(xtt.SetHour(-1), XSDK::XException);
        UT_ASSERT_THROWS(xtt.SetHour(24), XSDK::XException);
        UT_ASSERT(xtt == XTimeTuple());
    }
}


void XTimeTupleTest::TestMinute()
{
    UT_ASSERT(XTimeTuple().Minute() == 0);
    UT_ASSERT(XTimeTuple(0, 33, 0).Minute() == 33);
    UT_ASSERT(XTimeTuple(23, 33, 59).Minute() == 33);
    UT_ASSERT(XTimeTuple(0, 59, 0).Minute() == 59);
    UT_ASSERT(XTimeTuple(23, 59, 59).Minute() == 59);

    {
        XTimeTuple xtt;
        xtt.SetMinute(0);
        UT_ASSERT(xtt == XTimeTuple(0, 0, 0));
    }

    {
        XTimeTuple xtt;
        xtt.SetMinute(17);
        UT_ASSERT(xtt == XTimeTuple(0, 17, 0));
    }

    {
        XTimeTuple xtt;
        xtt.SetMinute(59);
        UT_ASSERT(xtt == XTimeTuple(0, 59, 0));
    }

    {
        XTimeTuple xtt = XTimeTuple(23, 59, 59);
        xtt.SetMinute(0);
        UT_ASSERT(xtt == XTimeTuple(23, 0, 59));
    }

    {
        XTimeTuple xtt = XTimeTuple(23, 59, 59);
        xtt.SetMinute(17);
        UT_ASSERT(xtt == XTimeTuple(23, 17, 59));
    }

    {
        XTimeTuple xtt = XTimeTuple(23, 59, 59);
        xtt.SetMinute(59);
        UT_ASSERT(xtt == XTimeTuple(23, 59, 59));
    }

    {
        XTimeTuple xtt;
        UT_ASSERT_THROWS(xtt.SetMinute(-1), XSDK::XException);
        UT_ASSERT_THROWS(xtt.SetMinute(60), XSDK::XException);
        UT_ASSERT(xtt == XTimeTuple());
    }
}


void XTimeTupleTest::TestSecond()
{
    UT_ASSERT(XTimeTuple().Second() == 0);
    UT_ASSERT(XTimeTuple(0, 0, 33).Second() == 33);
    UT_ASSERT(XTimeTuple(23, 59, 33).Second() == 33);
    UT_ASSERT(XTimeTuple(0, 0, 59).Second() == 59);
    UT_ASSERT(XTimeTuple(23, 59, 59).Second() == 59);

    {
        XTimeTuple xtt;
        xtt.SetSecond(0);
        UT_ASSERT(xtt == XTimeTuple(0, 0, 0));
    }

    {
        XTimeTuple xtt;
        xtt.SetSecond(17);
        UT_ASSERT(xtt == XTimeTuple(0, 0, 17));
    }

    {
        XTimeTuple xtt;
        xtt.SetSecond(59);
        UT_ASSERT(xtt == XTimeTuple(0, 0, 59));
    }

    {
        XTimeTuple xtt = XTimeTuple(23, 59, 59);
        xtt.SetSecond(0);
        UT_ASSERT(xtt == XTimeTuple(23, 59, 0));
    }

    {
        XTimeTuple xtt = XTimeTuple(23, 59, 59);
        xtt.SetSecond(17);
        UT_ASSERT(xtt == XTimeTuple(23, 59, 17));
    }

    {
        XTimeTuple xtt = XTimeTuple(23, 59, 59);
        xtt.SetSecond(59);
        UT_ASSERT(xtt == XTimeTuple(23, 59, 59));
    }

    {
        XTimeTuple xtt;
        UT_ASSERT_THROWS(xtt.SetSecond(-1), XSDK::XException);
        UT_ASSERT_THROWS(xtt.SetSecond(60), XSDK::XException);
        UT_ASSERT(xtt == XTimeTuple());
    }
}


void XTimeTupleTest::TestToISOExtString()
{
    UT_ASSERT(XTimeTuple().ToISOExtString() == "00:00:00");
    UT_ASSERT(XTimeTuple(1, 2, 3).ToISOExtString() == "01:02:03");
    UT_ASSERT(XTimeTuple(23, 59, 59).ToISOExtString() == "23:59:59");
}


void XTimeTupleTest::TestToCustomString()
{
    UT_ASSERT(XTimeTuple().ToCustomString("HH:MM:SS") == "00:00:00");
    UT_ASSERT(XTimeTuple(1, 2, 3).ToCustomString("HH:MM:SS") == "01:02:03");
    UT_ASSERT(XTimeTuple(23, 59, 59).ToCustomString("HH:MM:SS") == "23:59:59");

    UT_ASSERT(XTimeTuple().ToCustomString("SS:MM:HH") == "00:00:00");
    UT_ASSERT(XTimeTuple(1, 2, 3).ToCustomString("SS:MM:HH") == "03:02:01");
    UT_ASSERT(XTimeTuple(23, 59, 59).ToCustomString("SS:MM:HH") == "59:59:23");

    UT_ASSERT(XTimeTuple().ToCustomString("helloMMworld  SSasnethae HHHH") == "hello00world  00asnethae 00HH");
    UT_ASSERT(XTimeTuple(1, 2, 3).ToCustomString("helloMMworld  SSasnethae HHHH") == "hello02world  03asnethae 01HH");
    UT_ASSERT(XTimeTuple(23, 59, 59).ToCustomString("helloMMworld  SSasnethae HHHH") == "hello59world  59asnethae 23HH");
}


void XTimeTupleTest::TestFromISOExtString()
{
    UT_ASSERT_THROWS(XTimeTuple::FromISOExtString(""), XException);
    UT_ASSERT_THROWS(XTimeTuple::FromISOExtString("0"), XException);
    UT_ASSERT_THROWS(XTimeTuple::FromISOExtString("00"), XException);
    UT_ASSERT_THROWS(XTimeTuple::FromISOExtString("000"), XException);
    UT_ASSERT_THROWS(XTimeTuple::FromISOExtString("0000"), XException);
    UT_ASSERT_THROWS(XTimeTuple::FromISOExtString("00000"), XException);
    UT_ASSERT_THROWS(XTimeTuple::FromISOExtString("13033"), XException);
    UT_ASSERT_THROWS(XTimeTuple::FromISOExtString("1277"), XException);
    UT_ASSERT_THROWS(XTimeTuple::FromISOExtString("12707"), XException);
    UT_ASSERT_THROWS(XTimeTuple::FromISOExtString("12070"), XException);
    UT_ASSERT_THROWS(XTimeTuple::FromISOExtString("12303a"), XException);
    UT_ASSERT_THROWS(XTimeTuple::FromISOExtString("1230a3"), XException);
    UT_ASSERT_THROWS(XTimeTuple::FromISOExtString("123a33"), XException);
    UT_ASSERT_THROWS(XTimeTuple::FromISOExtString("12a033"), XException);
    UT_ASSERT_THROWS(XTimeTuple::FromISOExtString("1a0033"), XException);
    UT_ASSERT_THROWS(XTimeTuple::FromISOExtString("a20033"), XException);
    UT_ASSERT_THROWS(XTimeTuple::FromISOExtString("1200330"), XException);
    UT_ASSERT_THROWS(XTimeTuple::FromISOExtString("0120033"), XException);
    UT_ASSERT_THROWS(XTimeTuple::FromISOExtString("-120033"), XException);
    UT_ASSERT_THROWS(XTimeTuple::FromISOExtString("+120033"), XException);
    UT_ASSERT_THROWS(XTimeTuple::FromISOExtString("120033am"), XException);
    UT_ASSERT_THROWS(XTimeTuple::FromISOExtString("120033pm"), XException);

    UT_ASSERT_THROWS(XTimeTuple::FromISOExtString("0::"), XException);
    UT_ASSERT_THROWS(XTimeTuple::FromISOExtString(":0:"), XException);
    UT_ASSERT_THROWS(XTimeTuple::FromISOExtString("::0"), XException);
    UT_ASSERT_THROWS(XTimeTuple::FromISOExtString("0:0:0"), XException);
    UT_ASSERT_THROWS(XTimeTuple::FromISOExtString("0:0:00"), XException);
    UT_ASSERT_THROWS(XTimeTuple::FromISOExtString("0:00:0"), XException);
    UT_ASSERT_THROWS(XTimeTuple::FromISOExtString("00:0:0"), XException);
    UT_ASSERT_THROWS(XTimeTuple::FromISOExtString("00:00:0"), XException);
    UT_ASSERT_THROWS(XTimeTuple::FromISOExtString("00:0:00"), XException);
    UT_ASSERT_THROWS(XTimeTuple::FromISOExtString("13:0:33"), XException);
    UT_ASSERT_THROWS(XTimeTuple::FromISOExtString("12:7:7"), XException);
    UT_ASSERT_THROWS(XTimeTuple::FromISOExtString("12:7:07"), XException);
    UT_ASSERT_THROWS(XTimeTuple::FromISOExtString("12:07:0"), XException);
    UT_ASSERT_THROWS(XTimeTuple::FromISOExtString("12:30:3a"), XException);
    UT_ASSERT_THROWS(XTimeTuple::FromISOExtString("12:30:a3"), XException);
    UT_ASSERT_THROWS(XTimeTuple::FromISOExtString("12:3a:33"), XException);
    UT_ASSERT_THROWS(XTimeTuple::FromISOExtString("12:a0:33"), XException);
    UT_ASSERT_THROWS(XTimeTuple::FromISOExtString("1a:00:33"), XException);
    UT_ASSERT_THROWS(XTimeTuple::FromISOExtString("a2:00:33"), XException);
    UT_ASSERT_THROWS(XTimeTuple::FromISOExtString("12:003:30"), XException);
    UT_ASSERT_THROWS(XTimeTuple::FromISOExtString("120:03:30"), XException);
    UT_ASSERT_THROWS(XTimeTuple::FromISOExtString("012:00:33"), XException);
    UT_ASSERT_THROWS(XTimeTuple::FromISOExtString("01:200:33"), XException);
    UT_ASSERT_THROWS(XTimeTuple::FromISOExtString("-12:00:33"), XException);
    UT_ASSERT_THROWS(XTimeTuple::FromISOExtString("+12:00:33"), XException);
    UT_ASSERT_THROWS(XTimeTuple::FromISOExtString("12:00:33am"), XException);
    UT_ASSERT_THROWS(XTimeTuple::FromISOExtString("12:00:33pm"), XException);

    UT_ASSERT_THROWS(XTimeTuple::FromISOExtString("120033"), XException);

    UT_ASSERT(XTimeTuple::FromISOExtString("01:12:17") == XTimeTuple(1, 12, 17));
    UT_ASSERT(XTimeTuple::FromISOExtString("00:14:12") == XTimeTuple(0, 14, 12));
    UT_ASSERT(XTimeTuple::FromISOExtString("00:00:07") == XTimeTuple(0, 0, 7));
    UT_ASSERT(XTimeTuple::FromISOExtString("01:12:17 ") == XTimeTuple(1, 12, 17));
    UT_ASSERT(XTimeTuple::FromISOExtString(" 01:12:17") == XTimeTuple(1, 12, 17));
    UT_ASSERT(XTimeTuple::FromISOExtString(" 01:12:17 ") == XTimeTuple(1, 12, 17));
}


void XTimeTupleTest::TestCmpDateTime()
{
    UT_ASSERT(cmpDateTime(make_pair(XDateTuple(), XTimeTuple()),
                               make_pair(XDateTuple(), XTimeTuple())) == 0);
    UT_ASSERT(cmpDateTime(make_pair(XDateTuple(2012, 12, 21), XTimeTuple(12, 13, 14)),
                               make_pair(XDateTuple(2012, 12, 21), XTimeTuple(12, 13, 14))) == 0);
    UT_ASSERT(cmpDateTime(make_pair(XDateTuple(2012, 12, 22), XTimeTuple(12, 13, 14)),
                               make_pair(XDateTuple(2012, 12, 21), XTimeTuple(12, 13, 14))) > 0);
    UT_ASSERT(cmpDateTime(make_pair(XDateTuple(2012, 12, 21), XTimeTuple(12, 13, 14)),
                               make_pair(XDateTuple(2012, 12, 22), XTimeTuple(12, 13, 14))) < 0);
    UT_ASSERT(cmpDateTime(make_pair(XDateTuple(2012, 12, 21), XTimeTuple(12, 13, 15)),
                               make_pair(XDateTuple(2012, 12, 21), XTimeTuple(12, 13, 14))) > 0);
    UT_ASSERT(cmpDateTime(make_pair(XDateTuple(2012, 12, 21), XTimeTuple(12, 13, 14)),
                               make_pair(XDateTuple(2012, 12, 21), XTimeTuple(12, 13, 15))) < 0);
    UT_ASSERT(cmpDateTime(make_pair(XDateTuple(2012, 12, 22), XTimeTuple(12, 13, 14)),
                               make_pair(XDateTuple(2012, 12, 21), XTimeTuple(12, 13, 15))) > 0);
    UT_ASSERT(cmpDateTime(make_pair(XDateTuple(2012, 12, 21), XTimeTuple(12, 13, 15)),
                               make_pair(XDateTuple(2012, 12, 22), XTimeTuple(12, 13, 14))) < 0);
    UT_ASSERT(cmpDateTime(make_pair(XDateTuple(2012, 12, 22), XTimeTuple(12, 13, 15)),
                               make_pair(XDateTuple(2012, 12, 21), XTimeTuple(12, 13, 14))) > 0);
    UT_ASSERT(cmpDateTime(make_pair(XDateTuple(2012, 12, 21), XTimeTuple(12, 13, 14)),
                               make_pair(XDateTuple(2012, 12, 22), XTimeTuple(12, 13, 15))) < 0);
}


static void testAdd(const pair<XDateTuple, XTimeTuple>& orig,
                   XDuration duration,
                   const pair<XDateTuple, XTimeTuple>& expected,
                   int line)
{
    const pair<XDateTuple, XTimeTuple> result = addDuration(orig, duration);
    UT_ASSERT_MESSAGE(XString::Format("Line# %d: [%s, %s] [%s, %s]\n",
                                           line,
                                           expected.first.ToISOExtString().c_str(),
                                           expected.second.ToISOExtString().c_str(),
                                           result.first.ToISOExtString().c_str(),
                                           result.second.ToISOExtString().c_str()),
                           result == expected);
}


void XTimeTupleTest::TestAddDuration()
{
    testAdd(make_pair(XDateTuple(1999, 1, 1), XTimeTuple(0, 0, 0)), XDuration::Zero(), make_pair(XDateTuple(1999, 1, 1), XTimeTuple(0, 0, 0)), __LINE__);
    testAdd(make_pair(XDateTuple(1999, 1, 1), XTimeTuple(0, 0, 0)), XDuration(HNSECS, 1), make_pair(XDateTuple(1999, 1, 1), XTimeTuple(0, 0, 0)), __LINE__);
    testAdd(make_pair(XDateTuple(1999, 1, 1), XTimeTuple(0, 0, 0)), XDuration(HNSECS, -1), make_pair(XDateTuple(1998, 12, 31), XTimeTuple(23, 59, 59)), __LINE__);
    testAdd(make_pair(XDateTuple(1999, 1, 1), XTimeTuple(0, 0, 0)), XDuration(USECS, 1), make_pair(XDateTuple(1999, 1, 1), XTimeTuple(0, 0, 0)), __LINE__);
    testAdd(make_pair(XDateTuple(1999, 1, 1), XTimeTuple(0, 0, 0)), XDuration(USECS, -1), make_pair(XDateTuple(1998, 12, 31), XTimeTuple(23, 59, 59)), __LINE__);
    testAdd(make_pair(XDateTuple(1999, 1, 1), XTimeTuple(0, 0, 0)), XDuration(MSECS, 1), make_pair(XDateTuple(1999, 1, 1), XTimeTuple(0, 0, 0)), __LINE__);
    testAdd(make_pair(XDateTuple(1999, 1, 1), XTimeTuple(0, 0, 0)), XDuration(MSECS, -1), make_pair(XDateTuple(1998, 12, 31), XTimeTuple(23, 59, 59)), __LINE__);

    testAdd(make_pair(XDateTuple(1999, 1, 1), XTimeTuple(0, 0, 0)), XDuration(SECONDS, 1), make_pair(XDateTuple(1999, 1, 1), XTimeTuple(0, 0, 1)), __LINE__);
    testAdd(make_pair(XDateTuple(1999, 1, 1), XTimeTuple(0, 0, 0)), XDuration(SECONDS, -1), make_pair(XDateTuple(1998, 12, 31), XTimeTuple(23, 59, 59)), __LINE__);
    testAdd(make_pair(XDateTuple(1999, 1, 1), XTimeTuple(0, 0, 0)), XDuration(MINUTES, 1), make_pair(XDateTuple(1999, 1, 1), XTimeTuple(0, 1, 0)), __LINE__);
    testAdd(make_pair(XDateTuple(1999, 1, 1), XTimeTuple(0, 0, 0)), XDuration(MINUTES, -1), make_pair(XDateTuple(1998, 12, 31), XTimeTuple(23, 59, 0)), __LINE__);
    testAdd(make_pair(XDateTuple(1999, 1, 1), XTimeTuple(0, 0, 0)), XDuration(HOURS, 1), make_pair(XDateTuple(1999, 1, 1), XTimeTuple(1, 0, 0)), __LINE__);
    testAdd(make_pair(XDateTuple(1999, 1, 1), XTimeTuple(0, 0, 0)), XDuration(HOURS, -1), make_pair(XDateTuple(1998, 12, 31), XTimeTuple(23, 0, 0)), __LINE__);
    testAdd(make_pair(XDateTuple(1999, 1, 1), XTimeTuple(0, 0, 0)), XDuration(DAYS, 1), make_pair(XDateTuple(1999, 1, 2), XTimeTuple(0, 0, 0)), __LINE__);
    testAdd(make_pair(XDateTuple(1999, 1, 1), XTimeTuple(0, 0, 0)), XDuration(DAYS, -1), make_pair(XDateTuple(1998, 12, 31), XTimeTuple(0, 0, 0)), __LINE__);
    testAdd(make_pair(XDateTuple(1999, 1, 1), XTimeTuple(0, 0, 0)), XDuration(WEEKS, 1), make_pair(XDateTuple(1999, 1, 8), XTimeTuple(0, 0, 0)), __LINE__);
    testAdd(make_pair(XDateTuple(1999, 1, 1), XTimeTuple(0, 0, 0)), XDuration(WEEKS, -1), make_pair(XDateTuple(1998, 12, 25), XTimeTuple(0, 0, 0)), __LINE__);

    testAdd(make_pair(XDateTuple(1999, 12, 31), XTimeTuple(23, 59, 59)), XDuration(SECONDS, 1), make_pair(XDateTuple(2000, 1, 1), XTimeTuple(0, 0, 0)), __LINE__);
    testAdd(make_pair(XDateTuple(1999, 12, 31), XTimeTuple(23, 59, 59)), XDuration(SECONDS, -1), make_pair(XDateTuple(1999, 12, 31), XTimeTuple(23, 59, 58)), __LINE__);
    testAdd(make_pair(XDateTuple(1999, 12, 31), XTimeTuple(23, 59, 59)), XDuration(MINUTES, 1), make_pair(XDateTuple(2000, 1, 1), XTimeTuple(0, 0, 59)), __LINE__);
    testAdd(make_pair(XDateTuple(1999, 12, 31), XTimeTuple(23, 59, 59)), XDuration(MINUTES, -1), make_pair(XDateTuple(1999, 12, 31), XTimeTuple(23, 58, 59)), __LINE__);
    testAdd(make_pair(XDateTuple(1999, 12, 31), XTimeTuple(23, 59, 59)), XDuration(HOURS, 1), make_pair(XDateTuple(2000, 1, 1), XTimeTuple(0, 59, 59)), __LINE__);
    testAdd(make_pair(XDateTuple(1999, 12, 31), XTimeTuple(23, 59, 59)), XDuration(HOURS, -1), make_pair(XDateTuple(1999, 12, 31), XTimeTuple(22, 59, 59)), __LINE__);
    testAdd(make_pair(XDateTuple(1999, 12, 31), XTimeTuple(23, 59, 59)), XDuration(DAYS, 1), make_pair(XDateTuple(2000, 1, 1), XTimeTuple(23, 59, 59)), __LINE__);
    testAdd(make_pair(XDateTuple(1999, 12, 31), XTimeTuple(23, 59, 59)), XDuration(DAYS, -1), make_pair(XDateTuple(1999, 12, 30), XTimeTuple(23, 59, 59)), __LINE__);
    testAdd(make_pair(XDateTuple(1999, 12, 31), XTimeTuple(23, 59, 59)), XDuration(WEEKS, 1), make_pair(XDateTuple(2000, 1, 7), XTimeTuple(23, 59, 59)), __LINE__);
    testAdd(make_pair(XDateTuple(1999, 12, 31), XTimeTuple(23, 59, 59)), XDuration(WEEKS, -1), make_pair(XDateTuple(1999, 12, 24), XTimeTuple(23, 59, 59)), __LINE__);

    testAdd(make_pair(XDateTuple(1998, 12, 31), XTimeTuple(23, 59, 59)), XDuration(DAYS, 79) + XDuration(SECONDS, 512), make_pair(XDateTuple(1999, 3, 21), XTimeTuple(0, 8, 31)), __LINE__);
    testAdd(make_pair(XDateTuple(1999, 12, 31), XTimeTuple(23, 59, 59)), XDuration(DAYS, 79) + XDuration(SECONDS, 512), make_pair(XDateTuple(2000, 3, 20), XTimeTuple(0, 8, 31)), __LINE__);
    testAdd(make_pair(XDateTuple(1999, 3, 21), XTimeTuple(0, 8, 31)), -(XDuration(DAYS, 79) + XDuration(SECONDS, 512)), make_pair(XDateTuple(1998, 12, 31), XTimeTuple(23, 59, 59)), __LINE__);
    testAdd(make_pair(XDateTuple(2000, 3, 20), XTimeTuple(0, 8, 31)), -(XDuration(DAYS, 79) + XDuration(SECONDS, 512)), make_pair(XDateTuple(1999, 12, 31), XTimeTuple(23, 59, 59)), __LINE__);

    testAdd(make_pair(XDateTuple(1, 1, 1), XTimeTuple(0, 0, 0)), XDuration(SECONDS, 1), make_pair(XDateTuple(1, 1, 1), XTimeTuple(0, 0, 1)), __LINE__);
    testAdd(make_pair(XDateTuple(1, 1, 1), XTimeTuple(0, 0, 0)), XDuration(SECONDS, -1), make_pair(XDateTuple(1, 1, 1), XTimeTuple(0, 0, 0)), __LINE__);
    testAdd(make_pair(XDateTuple(2, 5, 12), XTimeTuple(7, 1, 5)), XDuration(DAYS, -5027), make_pair(XDateTuple(1, 1, 1), XTimeTuple(0, 0, 0)), __LINE__);
}
