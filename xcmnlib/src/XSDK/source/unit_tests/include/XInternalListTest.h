#ifndef XInternalListTest_h
#define XInternalListTest_h

#include "framework.h"

//XInternalListTest obviously requires XInternalList
#define STDUTILS_REQUIRES_INTERNAL

class XInternalListTest : public test_fixture
{
public:
    TEST_SUITE(XInternalListTest);
        TEST(XInternalListTest::TestIterationExternal);
        TEST(XInternalListTest::TestIterationInternal);
        TEST(XInternalListTest::TestNonPODAndMethodOrdersExternal);
        TEST(XInternalListTest::TestNonPODAndMethodOrdersInternal);
        TEST(XInternalListTest::TestAppendAndGet);
        TEST(XInternalListTest::TestPrependAndGet);
        TEST(XInternalListTest::TestAppendListAndSubList);
        TEST(XInternalListTest::TestListInsertion);
        TEST(XInternalListTest::TestInsertBeforeIterExternal);
        TEST(XInternalListTest::TestInsertBeforeIterInternal);
        TEST(XInternalListTest::TestPop);
        TEST(XInternalListTest::TestContains);
        TEST(XInternalListTest::TestRemove);
        TEST(XInternalListTest::TestRemoveAtIterExternal);
        TEST(XInternalListTest::TestRemoveAtIterInternal);
        TEST(XInternalListTest::TestCtorForIterAndBadIter);
        TEST(XInternalListTest::TestAllowedOperations);
        TEST(XInternalListTest::TestIsSorted);
        TEST(XInternalListTest::TestBackwardIterPermissionsExternal);
        TEST(XInternalListTest::TestBackwardIterPermissionsInternal);
    TEST_SUITE_END();

    virtual ~XInternalListTest() throw()
    {}

    void setup();
    void teardown();

protected:
    void TestIterationExternal();
    void TestIterationInternal();
    void TestNonPODAndMethodOrdersExternal();
    void TestNonPODAndMethodOrdersInternal();
    void TestAppendAndGet();
    void TestPrependAndGet();
    void TestAppendListAndSubList();
    void TestListInsertion();
    void TestInsertBeforeIterExternal();
    void TestInsertBeforeIterInternal();
    void TestInsertSortedAndGet();
    void TestPop();
    void TestContains();
    void TestRemove();
    void TestRemoveAtIterExternal();
    void TestRemoveAtIterInternal();
    void TestCtorForIterAndBadIter();
    void TestAllowedOperations();
    void TestIsSorted();
    void TestBackwardIterPermissionsExternal();
    void TestBackwardIterPermissionsInternal();
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

    virtual ~foo()
    {
    }

    foo& operator =(const foo& obj)
    {
        return copy(obj);
    }

    bool operator ==(const foo& obj) const
    {
        return(obj.data == data) ? true : false;
    }

    int get() const
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
