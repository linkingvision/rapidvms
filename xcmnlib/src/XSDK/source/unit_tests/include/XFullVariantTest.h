#ifndef VariantTest_h
#define VariantTest_h

#include "framework.h"

class VariantTest : public test_fixture
{
public:
    TEST_SUITE(VariantTest);
        TEST(VariantTest::TestDefaultConstructor);
        TEST(VariantTest::TestCopyConstructor);
        TEST(VariantTest::TestAssignmentOperator);
        TEST(VariantTest::TestGetType);
        TEST(VariantTest::TestClear);
        TEST(VariantTest::TestVariant);
        TEST(VariantTest::TestVariant64);
    TEST_SUITE_END();

    virtual ~VariantTest() throw()
    {}

    void setup();
    void teardown();

protected:

    void TestDefaultConstructor();
    void TestCopyConstructor();
    void TestAssignmentOperator();
    void TestGetType();
    void TestClear();
    void TestVariant();
    void TestVariant64();
};

#endif
