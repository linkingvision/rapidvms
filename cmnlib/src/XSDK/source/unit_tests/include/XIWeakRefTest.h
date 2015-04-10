#ifndef _X_I_WEAK_REF_TEST_H_
#define _X_I_WEAK_REF_TEST_H_

#include "framework.h"

class XIWeakRefTest : public test_fixture
{
public:
    TEST_SUITE(XIWeakRefTest);
        TEST(XIWeakRefTest::TestPointerAssignment);
        TEST(XIWeakRefTest::TestWeakRefAssignment);
        TEST(XIWeakRefTest::TestAutoRefAssignment);
        TEST(XIWeakRefTest::TestAssignmentOfSelf);
        TEST(XIWeakRefTest::TestStateMethods);
        TEST(XIWeakRefTest::TestClear);
        TEST(XIWeakRefTest::TestEvilMethod);
        TEST(XIWeakRefTest::TestComparisons);
        TEST(XIWeakRefTest::TestPointerOperator);
        TEST(XIWeakRefTest::TestIndirectionOperator);
        TEST(XIWeakRefTest::TestObjectDeletion);
    TEST_SUITE_END();

    void setup();
    void teardown();
protected:
    void TestPointerAssignment();
    void TestWeakRefAssignment();
    void TestAutoRefAssignment();
    void TestAssignmentOfSelf();
    void TestStateMethods();
    void TestClear();
    void TestEvilMethod();
    void TestComparisons();
    void TestPointerOperator();
    void TestIndirectionOperator();
    void TestObjectDeletion();
    void TestAutoRefToWeakRefConversion();
};

#endif // _X_I_WEAK_REF_TEST_H_

