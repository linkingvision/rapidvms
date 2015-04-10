
#include "framework.h"

class ck_json_test : public test_fixture
{
public:
    TEST_SUITE(ck_json_test);
        TEST(ck_json_test::TestForQuotedContent);
        TEST(ck_json_test::TestT247_2);
        TEST(ck_json_test::TestFind);
        TEST(ck_json_test::TestShortHand);
        TEST(ck_json_test::TestEmptyObjInArray);
        TEST(ck_json_test::TestEmptyArrayInObj);
        TEST(ck_json_test::TestEmptyObjInObj);
        TEST(ck_json_test::TestEmptyArrayInArray);
        TEST(ck_json_test::TestJsonOrgExample);
        TEST(ck_json_test::TestSimpleArray);
        TEST(ck_json_test::TestSimpleObject);
        TEST(ck_json_test::TestArrayContainArray);
        TEST(ck_json_test::TestArrayContainObject);
        TEST(ck_json_test::TestObjectContainObject);
        TEST(ck_json_test::TestObjectContainArray);
        TEST(ck_json_test::TestEmptyObj);
        TEST(ck_json_test::TestEmptyArray);
    TEST_SUITE_END();

    virtual ~ck_json_test() throw() {}

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
