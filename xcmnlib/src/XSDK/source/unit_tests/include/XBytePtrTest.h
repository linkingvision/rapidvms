
#ifndef _XBytePtrTest_H_
#define _XBytePtrTest_H_

#include "framework.h"

class XBytePtrTest : public test_fixture
{
public:
    TEST_SUITE(XBytePtrTest);
        TEST(XBytePtrTest::TestConstructor);
        TEST(XBytePtrTest::TestGetSetPtr);
        TEST(XBytePtrTest::TestIncDec);
        TEST(XBytePtrTest::TestPointer);
        TEST(XBytePtrTest::TestAddSub);
        TEST(XBytePtrTest::TestComparisons);
        TEST(XBytePtrTest::TestCasting);
        TEST(XBytePtrTest::TestOffset);
        TEST(XBytePtrTest::TestSeek);
        TEST(XBytePtrTest::TestReadWrite);
        TEST(XBytePtrTest::TestBulkWrite);
    TEST_SUITE_END();

    virtual ~XBytePtrTest() throw()
    {}

    void setup() {}
    void teardown() {}

protected:

    void TestConstructor();
    void TestGetSetPtr();
    void TestIncDec();
    void TestPointer();
    void TestAddSub();
    void TestComparisons();
    void TestCasting();
    void TestOffset();
    void TestSeek();
    void TestReadWrite();
    void TestBulkWrite();
};

#endif

