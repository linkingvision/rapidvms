#ifndef XListBoxTest_h
#define XListBoxTest_h

#include "framework.h"

class XListBoxTest : public test_fixture
{
public:
    TEST_SUITE(XListBoxTest);
        TEST(XListBoxTest::TestExternalIteration);
        TEST(XListBoxTest::TestInternalIteration);
        TEST(XListBoxTest::TestNonPODAndOrdersExternal);
        TEST(XListBoxTest::TestNonPODAndOrdersInternal);
        TEST(XListBoxTest::TestAppendListAndSubList);
        TEST(XListBoxTest::TestInsertBeforeIterAndClearExternal);
        TEST(XListBoxTest::TestInsertBeforeIterAndClearInternal);
        TEST(XListBoxTest::TestInsertSortedAndGet);
        TEST(XListBoxTest::TestInsertListSortedEmptyList);
        TEST(XListBoxTest::TestInsertListSortedWithSorted);
        TEST(XListBoxTest::TestInsertListSortedWithUnsorted);
        TEST(XListBoxTest::TestPop);
        TEST(XListBoxTest::TestContains);
        TEST(XListBoxTest::TestRemove);
        TEST(XListBoxTest::TestRemoveAtIterExternal);
        TEST(XListBoxTest::TestRemoveAtIterInternal);
        TEST(XListBoxTest::TestSameStorageAndBadIter);
        TEST(XListBoxTest::TestAllowedOperations);
        TEST(XListBoxTest::TestIsSorted);
        TEST(XListBoxTest::TestBackwardIterPermissionsExternal);
        TEST(XListBoxTest::TestBackwardIterPermissionsInternal);
    TEST_SUITE_END();

    virtual ~XListBoxTest() throw()
    {}

    void setup();
    void teardown();

protected:

    void TestExternalIteration();
    void TestInternalIteration();
    void TestNonPODAndOrdersExternal();
    void TestNonPODAndOrdersInternal();
    void TestAppendListAndSubList();
    void TestInsertBeforeIterAndClearExternal();
    void TestInsertBeforeIterAndClearInternal();
    void TestInsertSortedAndGet();
    void TestInsertListSortedEmptyList();
    void TestInsertListSortedWithSorted();
    void TestInsertListSortedWithUnsorted();
    void TestPop();
    void TestContains();
    void TestRemove();
    void TestRemoveAtIterExternal();
    void TestRemoveAtIterInternal();
    void TestSameStorageAndBadIter();
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
