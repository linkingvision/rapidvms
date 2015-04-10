#ifndef _XMEMORYTEST_H_
#define _XMEMORYTEST_H_

#include "framework.h"

class XMemoryTest : public test_fixture
{
public:
    TEST_SUITE(XMemoryTest);
#ifndef ARM
        TEST(XMemoryTest::TestMPF_1023);
#endif//ARM
        TEST(XMemoryTest::TestDefaultConstructor);
        TEST(XMemoryTest::TestSizeConstructor);
        TEST(XMemoryTest::TestCopyConstructor);
        TEST(XMemoryTest::TestAssignmentOperator);
        TEST(XMemoryTest::TestExtend);
        TEST(XMemoryTest::TestMap);
        TEST(XMemoryTest::TestClone);
        TEST(XMemoryTest::TestResizeBuffer);
        TEST(XMemoryTest::TestResizeData);
        TEST(XMemoryTest::TestRead);
        TEST(XMemoryTest::TestWrite);
        TEST(XMemoryTest::TestAppend);
        TEST(XMemoryTest::TestPoolConstructor);
        TEST(XMemoryTest::TestPoolGetAndReturn);
    TEST_SUITE_END();

    void setup();
    void teardown();

protected:

    void TestDefaultConstructor();
    void TestSizeConstructor();
    void TestCopyConstructor();
    void TestAssignmentOperator();
    void TestExtend();
    void TestMap();
    void TestClone();
    void TestResizeBuffer();
    void TestResizeData();
    void TestRead();
    void TestWrite();
    void TestAppend();
#ifndef ARM
    void TestMPF_1023();
#endif//ARM
    void TestPoolConstructor();
    void TestPoolGetAndReturn();
};

#endif /* _XMEMORYTEST_H_ */
