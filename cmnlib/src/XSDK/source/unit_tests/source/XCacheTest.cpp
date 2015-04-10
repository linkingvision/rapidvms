
#include "XCacheTest.h"
#include "XSDK/XCache.h"

using namespace XSDK;

REGISTER_TEST_FIXTURE(XCacheTest);

void XCacheTest::setup()
{
}

void XCacheTest::teardown()
{
}

void XCacheTest::TestDefaultConstructor()
{
    XCache<int> cache( 10 );
}

void XCacheTest::TestAdd()
{
    XCache<int> cache( 5 );

    cache.Put( "1", 1 );
    cache.Put( "2", 2 );
    cache.Put( "3", 3 );
    cache.Put( "4", 4 );
    cache.Put( "5", 5 );

    int val = 0;
    bool found = cache.Get( "1", val );
    UT_ASSERT( (found == true) && (val == 1) );

    found = cache.Get( "2", val );
    UT_ASSERT( (found == true) && (val == 2) );

    found = cache.Get( "3", val );
    UT_ASSERT( (found == true) && (val == 3) );

    found = cache.Get( "4", val );
    UT_ASSERT( (found == true) && (val == 4) );

    found = cache.Get( "5", val );
    UT_ASSERT( (found == true) && (val == 5) );

    UT_ASSERT( cache.Get( "not_found", val ) == false );
}

void XCacheTest::TestAddTillFull()
{
    XCache<int> cache( 5 );

    cache.Put( "1", 1 );
    cache.Put( "2", 2 );
    cache.Put( "3", 3 );
    cache.Put( "4", 4 );
    cache.Put( "5", 5 );
    cache.Put( "6", 6 );

    int val = 0;
    bool found = cache.Get( "2", val );
    UT_ASSERT( (found == true) && (val == 2) );

    found = cache.Get( "3", val );
    UT_ASSERT( (found == true) && (val == 3) );

    found = cache.Get( "4", val );
    UT_ASSERT( (found == true) && (val == 4) );

    found = cache.Get( "5", val );
    UT_ASSERT( (found == true) && (val == 5) );

    found = cache.Get( "6", val );
    UT_ASSERT( (found == true) && (val == 6) );
}

void XCacheTest::TestDestroysCorrectItem()
{
    XCache<int> cache( 5 );

    cache.Put( "1", 1 );
    cache.Put( "2", 2 );
    cache.Put( "3", 3 );
    cache.Put( "4", 4 );
    cache.Put( "5", 5 );
    cache.Put( "6", 6 );

    int val = 0;
    UT_ASSERT( cache.Get( "1", val ) == false );

    cache.Get( "2", val );

    cache.Put( "7", 7 );

    UT_ASSERT( cache.Get( "3", val ) == false );
}
