#ifndef XHashTest_h
#define XHashTest_h

#include "framework.h"

class XHashTest : public test_fixture
{
public:
    TEST_SUITE(XHashTest);
        TEST(XHashTest::TestDefaultConstructor);
        TEST(XHashTest::TestCopyConstructor);
        TEST(XHashTest::TestAssignmentOperator);
        TEST(XHashTest::TestAdd);
        TEST(XHashTest::TestFind);
        TEST(XHashTest::TestRemove);
        TEST(XHashTest::TestClear);
        TEST(XHashTest::TestIterationExternal);
        TEST(XHashTest::TestIterationInternal);
        TEST(XHashTest::TestHash);
        TEST(XHashTest::TestIterationOrderUnsorted);
        TEST(XHashTest::TestIterationOrderSorted);
        TEST(XHashTest::TestAddUnicode);
        TEST(XHashTest::TestFindUnicode);
        TEST(XHashTest::TestRemoveUnicode);
        TEST(XHashTest::TestSubscriptOperator);
    TEST_SUITE_END();

    virtual ~XHashTest() throw()
    {}

    void setup();
    void teardown();

protected:

    void TestDefaultConstructor();
    void TestCopyConstructor();
    void TestAssignmentOperator();
    void TestAdd();
    void TestFind();
    void TestRemove();
    void TestClear();
    void TestIterationExternal();
    void TestIterationInternal();
    void TestHash();
    void TestIterationOrderUnsorted();
    void TestIterationOrderSorted();
    void TestAddUnicode();
    void TestFindUnicode();
    void TestRemoveUnicode();
    void TestSubscriptOperator();
};

class foo
{
public:
    foo(int d = 0)
    {
        data = d;
    }

    foo(const foo& obj)
    {
        copy(obj);
    }

    virtual ~foo() throw()
    {
    }

    foo& operator =(const foo& obj)
    {
        return copy(obj);
    }

    bool operator ==(const foo& obj)
    {
        return(obj.data == data) ? true : false;
    }

    int get()
    {
        return data;
    }

private:

    foo& copy(const foo& obj)
    {
        data = obj.data; return *this;
    }

    int data;
};


#endif
