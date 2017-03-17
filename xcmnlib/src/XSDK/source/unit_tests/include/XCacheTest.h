#ifndef XCacheTest_h
#define XCacheTest_h

#include "framework.h"

class XCacheTest : public test_fixture
{
public:
    TEST_SUITE(XCacheTest);
        TEST(XCacheTest::TestDefaultConstructor);
        TEST(XCacheTest::TestAdd);
        TEST(XCacheTest::TestAddTillFull);
        TEST(XCacheTest::TestDestroysCorrectItem);
    TEST_SUITE_END();

    virtual ~XCacheTest() throw() {}

    void setup();
    void teardown();

protected:

    void TestDefaultConstructor();
    void TestAdd();
    void TestAddTillFull();
    void TestDestroysCorrectItem();
};

#endif
