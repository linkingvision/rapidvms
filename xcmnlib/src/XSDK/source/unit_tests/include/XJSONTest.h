#ifndef XJSONTest_h
#define XJSONTest_h

#include "framework.h"
#include <vector>

class XJSONTest : public test_fixture
{
public:
    TEST_SUITE(XJSONTest);
        TEST(XJSONTest::TestForQuotedContent);
        TEST(XJSONTest::TestT247_2);
        TEST(XJSONTest::TestFind);
        TEST(XJSONTest::TestShortHand);
        TEST(XJSONTest::TestEmptyObjInArray);
        TEST(XJSONTest::TestEmptyArrayInObj);
        TEST(XJSONTest::TestEmptyObjInObj);
        TEST(XJSONTest::TestEmptyArrayInArray);
        TEST(XJSONTest::TestJsonOrgExample);
        TEST(XJSONTest::TestSimpleArray);
        TEST(XJSONTest::TestSimpleObject);
        TEST(XJSONTest::TestArrayContainArray);
        TEST(XJSONTest::TestArrayContainObject);
        TEST(XJSONTest::TestObjectContainObject);
        TEST(XJSONTest::TestObjectContainArray);
        TEST(XJSONTest::TestEmptyObj);
        TEST(XJSONTest::TestEmptyArray);
    TEST_SUITE_END();

    virtual ~XJSONTest() throw()
    {}

protected:
    void TestFind();
    void TestT247_2();
    void TestForQuotedContent();
    void TestEmptyObjInArray();
    void TestEmptyArrayInObj();
    void TestEmptyObjInObj();
    void TestEmptyArrayInArray();
    void TestEmptyObj();
    void TestEmptyArray();
    void TestJsonOrgExample();
    void TestSimpleArray();
    void TestSimpleObject();
    void TestArrayContainArray();
    void TestShortHand();
    void TestArrayContainObject();
    void TestObjectContainObject();
    void TestObjectContainArray();
};


#endif
