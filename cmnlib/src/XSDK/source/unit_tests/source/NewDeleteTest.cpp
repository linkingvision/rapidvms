
#include "NewDeleteTest.h"
#include "XSDK/XBaseObject.h"
#include "XSDK/XIRef.h"

using namespace XSDK;

REGISTER_TEST_FIXTURE(NewDeleteTest);

void NewDeleteTest::TestNewDeleteAligned()
{
    size_t size = 1234;
    size_t alignment = 0x10;
    unsigned int mask = 0xF;
    uint8_t* p = 0;
    UT_ASSERT_NO_THROW( X_NEW_ALIGNED_BUFFER( p, size, alignment ) );
    UT_ASSERT( p != 0 );
    UT_ASSERT( ((size_t)p & mask) == 0 ); // Verify alignment
    UT_ASSERT_NO_THROW( X_DELETE_ALIGNED_BUFFER( p ) );
}
