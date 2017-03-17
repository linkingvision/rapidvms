#ifndef _X_INTRUSIVE_PTR_TEST_H_
#define _X_INTRUSIVE_PTR_TEST_H_

#include "framework.h"

class XIRefTest : public test_fixture
{
public:
TEST_SUITE(XIRefTest);
    TEST(XIRefTest::TestAddRefRelease);
    TEST(XIRefTest::TestPointerAssignment);
    TEST(XIRefTest::TestObjectAssignment);
    TEST(XIRefTest::TestAssignmentOfSelf);
    TEST(XIRefTest::TestStateMethods);
    TEST(XIRefTest::TestEvilMethod);
    TEST(XIRefTest::TestGetCounterMethod);
    TEST(XIRefTest::TestComparisons);
    TEST(XIRefTest::TestPointerOperator);
    TEST(XIRefTest::TestWeakRefAssignment);
TEST_SUITE_END();

    void setup();
    void teardown();
protected:
    void TestAddRefRelease();
    void TestPointerAssignment();
    void TestObjectAssignment();
    void TestAssignmentOfSelf();
    void TestStateMethods();
    void TestEvilMethod();
    void TestGetCounterMethod();
    void TestComparisons();
    void TestPointerOperator();
    void TestWeakRefAssignment();
};

#endif
