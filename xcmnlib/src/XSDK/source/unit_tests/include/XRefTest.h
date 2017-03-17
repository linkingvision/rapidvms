#ifndef _X_SHARED_PTR_TEST_H_
#define _X_SHARED_PTR_TEST_H_

#include "framework.h"

class XRefTest : public test_fixture
{
public:

TEST_SUITE(XRefTest);
    TEST(XRefTest::TestDefaultConstructor);
    TEST(XRefTest::TestPointerAssignment);
    TEST(XRefTest::TestObjectAssignment);
    TEST(XRefTest::TestRelatedTypeAssignment);
    TEST(XRefTest::TestWeakRefAssignment);
    TEST(XRefTest::TestAssignmentOfSelf);
    TEST(XRefTest::TestComparisons);
    TEST(XRefTest::TestPointerOperator);
    TEST(XRefTest::TestCastOperatorNonInstrusive);
    TEST(XRefTest::TestStateMethods);
TEST_SUITE_END();

    void setup();
    void teardown();
protected:
    void TestDefaultConstructor();
    void TestPointerAssignment();
    void TestObjectAssignment();
    void TestRelatedTypeAssignment();
    void TestWeakRefAssignment();
    void TestAssignmentOfSelf();
    void TestComparisons();
    void TestPointerOperator();
    void TestCastOperatorNonInstrusive();
    void TestStateMethods();
};

#endif
