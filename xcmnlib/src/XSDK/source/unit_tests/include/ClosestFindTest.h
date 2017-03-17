#ifndef ClosestFindTest_h
#define ClosestFindTest_h

#include "framework.h"
#include <vector>

class ClosestFindTest : public test_fixture
{
public:
    TEST_SUITE(ClosestFindTest);
        TEST(ClosestFindTest::TestFindExact);
        TEST(ClosestFindTest::TestFindIntermediateLargeDifference);
        TEST(ClosestFindTest::TestFindIntermediateSmallDifference);
        TEST(ClosestFindTest::TestFindIntermediateValueInMiddle);
        TEST(ClosestFindTest::TestFindInSetExact);
        TEST(ClosestFindTest::TestFindInSetIntermediate);
    TEST_SUITE_END();

    virtual ~ClosestFindTest() throw()
    {}

    void setup();
    void teardown();

protected:
    std::vector<double> _values;
    static double compare(const double& a, const double& b);
    void TestFindExact();
    void TestFindIntermediateLargeDifference();
    void TestFindIntermediateSmallDifference();
    void TestFindIntermediateValueInMiddle();
    void TestFindInSetExact();
    void TestFindInSetIntermediate();
};


#endif
