#ifndef _XWeakRefTest_h_
#define _XWeakRefTest_h_

#include "framework.h"

class XWeakRefTest : public test_fixture
{
public:
    TEST_SUITE(XWeakRefTest);
        TEST(XWeakRefTest::TestDefaultConstructor);
        TEST(XWeakRefTest::TestXRefAssignment);
        TEST(XWeakRefTest::TestWeakRefAssignment);
        TEST(XWeakRefTest::TestAssignmentOfSelf);
        TEST(XWeakRefTest::TestPointerOperator);
        TEST(XWeakRefTest::TestIndirectionOperator);
        TEST(XWeakRefTest::TestComparisons);
        TEST(XWeakRefTest::TestStateMethods);
        TEST(XWeakRefTest::TestObjectDeletion);
        TEST(XWeakRefTest::TestClear);
    TEST_SUITE_END();

    void setup();
    void teardown();

protected:
    void TestDefaultConstructor();
    void TestXRefAssignment();
    void TestWeakRefAssignment();
    void TestAssignmentOfSelf();
    void TestPointerOperator();
    void TestIndirectionOperator();
    void TestComparisons();
    void TestStateMethods();
    void TestObjectDeletion();
    void TestClear();
};

#endif
