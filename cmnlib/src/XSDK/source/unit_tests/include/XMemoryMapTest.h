#ifndef _XMemoryMapTest_H_
#define _XMemoryMapTest_H_

#include "framework.h"

class XMemoryMapTest : public test_fixture
{
public:
    TEST_SUITE(XMemoryMapTest);
        TEST(XMemoryMapTest::TestFileMapping);
        TEST(XMemoryMapTest::TestPersistToDisk);
    TEST_SUITE_END();

    void setup();
    void teardown();

protected:

    void TestFileMapping();
    void TestPersistToDisk();
};

#endif
