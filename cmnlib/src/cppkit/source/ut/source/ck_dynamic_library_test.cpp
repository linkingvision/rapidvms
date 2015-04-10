
#include "ck_dynamic_library_test.h"
#include "cppkit/ck_exception.h"
#include <fstream>

#ifdef IS_LINUX
// Needed for 'system' call
#include <cstdlib>
#endif

using namespace cppkit;
using namespace std;

REGISTER_TEST_FIXTURE(ck_dynamic_library_test);

#ifdef IS_WINDOWS
cppkit::ck_string VALID_DYNAMIC_LIB = "cppkit.dll";
cppkit::ck_string INVALID_DYNAMIC_LIB = "invalid.dll";
#else
cppkit::ck_string VALID_DYNAMIC_LIB = "../../../devel_artifacts/lib/libcppkit.so";
cppkit::ck_string INVALID_DYNAMIC_LIB = "invalid.so";
#endif

cppkit::ck_string VALID_FUNC_NAME = "CK_LIBRARY_ID";
cppkit::ck_string INVALID_FUNC_NAME = "Invalid";

typedef void (*Valid)();
typedef void (*Invalid)(OUT cppkit::ck_exception** ppException);

void ck_dynamic_library_test::setup()
{
}

void ck_dynamic_library_test::teardown()
{
}

void ck_dynamic_library_test::test_load()
{
    ck_dynamic_library dl( VALID_DYNAMIC_LIB );

    UT_ASSERT_NO_THROW( dl.load( VALID_DYNAMIC_LIB ) );

    UT_ASSERT_NO_THROW( dl.unload() );

    UT_ASSERT_THROWS( dl.load( INVALID_DYNAMIC_LIB ), cppkit::ck_exception );
}

void ck_dynamic_library_test::test_resolve_symbol()
{
    ck_dynamic_library dl( VALID_DYNAMIC_LIB );
    Valid validFunc;

    UT_ASSERT_NO_THROW( dl.load() );
    UT_ASSERT_NO_THROW( validFunc = (Valid)dl.resolve_symbol( VALID_FUNC_NAME ) );
    UT_ASSERT( validFunc != 0 );
    Invalid invalidFunc = (Invalid)dl.resolve_symbol( INVALID_FUNC_NAME );
    UT_ASSERT( invalidFunc == 0 );
}
