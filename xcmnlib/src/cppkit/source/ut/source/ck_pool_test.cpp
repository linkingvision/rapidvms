
#include "ck_pool_test.h"
#include "cppkit/ck_pool.h"
#include "cppkit/ck_string.h"
#include <list>

using namespace std;
using namespace cppkit;

REGISTER_TEST_FIXTURE(ck_pool_test);

void ck_pool_test::test_constructor()
{
    ck_pool<ck_string> pool( 100 );
    UT_ASSERT( pool._free.size() == 100 );
    UT_ASSERT( pool._all.size() == 100 );
    UT_ASSERT( pool.get_num_free() == 100 );
    UT_ASSERT( !pool.empty() );
}

void ck_pool_test::test_get()
{
    ck_pool<ck_string> pool( 100, "Hola!" );
    UT_ASSERT( pool._free.size() == 100 );
    UT_ASSERT( pool._all.size() == 100 );
    UT_ASSERT( pool.get_num_free() == 100 );
    UT_ASSERT( !pool.empty() );

    {
        shared_ptr<ck_string> s1 = pool.get();
        UT_ASSERT( *s1 == "Hola!" );
        UT_ASSERT( pool.get_num_free() == 99 );
    }

    UT_ASSERT( pool.get_num_free() == 100 );
}

void ck_pool_test::test_deleter_transfer()
{
    ck_pool<ck_string> pool( 100, "Hola!" );
    UT_ASSERT( pool._free.size() == 100 );
    UT_ASSERT( pool._all.size() == 100 );
    UT_ASSERT( pool.get_num_free() == 100 );
    UT_ASSERT( !pool.empty() );

    {
        shared_ptr<ck_string> s2;

        {
            shared_ptr<ck_string> s1 = pool.get();
            UT_ASSERT( *s1 == "Hola!" );
            UT_ASSERT( pool.get_num_free() == 99 );
            s2 = s1; // s2 (at wider scope) now references the string.
        }

        UT_ASSERT( pool.get_num_free() == 99 ); // because s2 still exists, the string is not back in the pool
    }

    // but now s2 is destroyed, so it should be back in the pool.
    UT_ASSERT( pool.get_num_free() == 100 );
}

void ck_pool_test::test_empty()
{
    ck_pool<int> pool( 100, 42 );

    UT_ASSERT( pool.get_num_free() == 100 );

    {
        list<shared_ptr<int> > l1;
        while( !pool.empty() )
            l1.push_back( pool.get() );
        UT_ASSERT( pool.empty() );
    }

    UT_ASSERT( !pool.empty() );
    UT_ASSERT( pool.get_num_free() == 100 );
}
