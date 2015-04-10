
#include "XGuardTest.h"

#include "XSDK/XGuard.h"
#include "XSDK/TimeUtils.h"

using namespace std;
using namespace XSDK;

REGISTER_TEST_FIXTURE(XGuardTest);

void* XGuardTest::EntryPoint()
{
    if( _testNum == 0 )
    {
        XGuard g( _lock );

        _val = 42;

        x_sleep( 2 );
    }
    else if( _testNum == 1 )
    {
        XGuard g( _lock );

        _val = 43;
    }

    return NULL;
}

void XGuardTest::setup()
{
}

void XGuardTest::teardown()
{
}

void XGuardTest::TestNormal()
{
    _testNum = 0;

    Start();

    x_sleep( 1 );

    XGuard g( _lock );

    UT_ASSERT( _val == 42 );

    Join();
}

void CleanFunction(char& foo)
{
  foo = 'B';
}


void CleanerBar(void* foo)
{
  char* bar = (char*)foo;
  *bar = 'X';
}

void CleanerBarWrapper(char& foo)
{
  char* x = &foo;
  CleanerBar(x);
}

void XGuardTest::TestGeneralCleaner()
{
    char foo = 'A';
    {
      XCleanerGuard<char&> xcg(foo,CleanFunction);
    }
    UT_ASSERT(foo == 'B');

    {
      XCleanerGuard<char&> xcg(foo,CleanerBarWrapper);
    }

    UT_ASSERT(foo == 'X');

}

void XGuardTest::TestDefaultCleaner()
{
    {
        XCleanerGuard<void *> a;
    }
}

void XGuardTest::TestSetCleaner()
{
    {
        XCleanerGuard<char*> xcg;

        UT_ASSERT_NO_THROW(xcg.Set(NULL,NULL));
    }

    char foo = 'A';
    {
        XCleanerGuard<char&> xcg(foo,CleanFunction);

        UT_ASSERT_NO_THROW(xcg.Set(foo,CleanerBarWrapper));

        UT_ASSERT(foo == 'B');
    }
    UT_ASSERT(foo == 'X');

    {
        XCleanerGuard<char&> xcg(foo,CleanerBarWrapper);

        UT_ASSERT_NO_THROW(xcg.Set(foo,CleanFunction));

        UT_ASSERT(foo == 'X');
    }

    UT_ASSERT(foo == 'B');

    foo = 'A';
    {
        XCleanerGuard<char&> xcg(foo,CleanFunction);

        UT_ASSERT_NO_THROW(xcg.Set(foo,CleanFunction));
    }
    UT_ASSERT(foo == 'B');

    {
        XCleanerGuard<char&> xcg(foo,CleanerBarWrapper);

        UT_ASSERT_NO_THROW(xcg.Set(foo,CleanerBarWrapper));
    }

    UT_ASSERT(foo == 'X');
}

void XGuardTest::TestClearCleaner()
{
    {
        XCleanerGuard<char*> xcg;

        xcg.Set(NULL,NULL);

        UT_ASSERT_NO_THROW(xcg.Clear());
    }

    {
        XCleanerGuard<char*> xcg;

        UT_ASSERT_NO_THROW(xcg.Clear());

        UT_ASSERT_NO_THROW(xcg.Clear());
    }

    char foo = 'A';
    {
        XCleanerGuard<char&> xcg(foo,CleanFunction);

        UT_ASSERT_NO_THROW(xcg.Clear());

        UT_ASSERT(foo == 'B');
    }
    UT_ASSERT(foo == 'B');

    {
        XCleanerGuard<char&> xcg(foo,CleanerBarWrapper);

        UT_ASSERT_NO_THROW(xcg.Clear());

        UT_ASSERT(foo == 'X');
    }

    UT_ASSERT(foo == 'X');
}

void XGuardTest::TestManual()
{
    _testNum = 1;

    XGuard g( _lock );

    Start();

    x_sleep( 1 );

    g.Release();

    x_sleep( 1 );

    g.Acquire();

    UT_ASSERT( _val == 43 );

    Join();
}
