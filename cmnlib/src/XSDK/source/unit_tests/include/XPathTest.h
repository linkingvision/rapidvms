#ifndef XPathTest_h
#define XPathTest_h

#include "framework.h"

class XPathTest : public test_fixture
{
public:
    TEST_SUITE(XPathTest);
        TEST(XPathTest::TestConstructor);
        TEST(XPathTest::TestCopyConstructor);
        TEST(XPathTest::TestAssignmentOperator);
        TEST(XPathTest::TestOpenPath);
        TEST(XPathTest::TestIsPath);
        TEST(XPathTest::TestIsReg);
        TEST(XPathTest::TestExists);
        TEST(XPathTest::TestClear);
        TEST(XPathTest::TestIter);
        TEST(XPathTest::TestFileSize);
    TEST_SUITE_END();

    virtual ~XPathTest() throw()
    {}

    void setup();
    void teardown();

protected:

    void TestConstructor();
    void TestCopyConstructor();
    void TestAssignmentOperator();
    void TestOpenPath();
    void TestIsPath();
    void TestIsReg();
    void TestExists();
    void TestClear();
    void TestIter();
    void TestFileSize();

private:
};

#endif
