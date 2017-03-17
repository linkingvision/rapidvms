
#ifndef _NEW_DELETE_TEST_H_
#define _NEW_DELETE_TEST_H_

#include "framework.h"
#include "XSDK/OS.h"

class NewDeleteTest : public test_fixture
{
public:
    TEST_SUITE(NewDeleteTest);
        TEST(NewDeleteTest::TestNewDeleteAligned);
    TEST_SUITE_END();

    void setup() {}
    void teardown() {}

protected:
    void TestNewDeleteAligned();
};

#endif // _NEW_DELETE_TEST_H_

