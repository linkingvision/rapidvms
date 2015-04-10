
#include "ck_nullable_test.h"

#include "cppkit/ck_nullable.h"
#include "cppkit/ck_string.h"
#include <memory>

using namespace std;
using namespace cppkit;

REGISTER_TEST_FIXTURE(ck_nullable_test);

void ck_nullable_test::test_bool()
{
    ck_nullable<bool> obj1;
    UT_ASSERT(obj1.is_null());
    obj1.set_value(true);
    UT_ASSERT(!obj1.is_null());
    UT_ASSERT(obj1.value() == true);
    UT_ASSERT(obj1 == true);
    UT_ASSERT(obj1 != false);
    UT_ASSERT(true == obj1);
    UT_ASSERT(false != obj1);
    UT_ASSERT(obj1 == obj1);
    UT_ASSERT(obj1 != ck_nullable<bool>(false));
    obj1 = false;
    UT_ASSERT(!obj1.is_null());
    UT_ASSERT(obj1.value() == false);
    UT_ASSERT(obj1 == false);
    obj1.clear();
    UT_ASSERT(obj1.is_null());

    ck_nullable<bool> obj2 = false;
    UT_ASSERT(!obj2.is_null());
    UT_ASSERT(obj2.value() == false);
    UT_ASSERT(obj2 == false);

    bool b = obj2.value();
    UT_ASSERT(b == false);

    obj2 = ck_nullable<bool>(true);
    UT_ASSERT(!obj2.is_null());
    UT_ASSERT(obj2.value() == true);
    UT_ASSERT(obj2 == true);
    UT_ASSERT(obj2 != false);

    obj2 = obj1;
    UT_ASSERT(obj2.is_null());
}

void ck_nullable_test::test_int()
{
    ck_nullable<int> obj1;
    UT_ASSERT(obj1.is_null());
    obj1.set_value(7);
    UT_ASSERT(!obj1.is_null());
    UT_ASSERT(obj1.value() == 7);
    UT_ASSERT(obj1 == 7);
    UT_ASSERT(obj1 != 42);
    UT_ASSERT(7 == obj1);
    UT_ASSERT(42 != obj1);
    UT_ASSERT(obj1 == obj1);
    UT_ASSERT(obj1 != ck_nullable<int>(99));
    obj1 = 42;
    UT_ASSERT(!obj1.is_null());
    UT_ASSERT(obj1.value() == 42);
    UT_ASSERT(obj1 == 42);
    obj1.clear();
    UT_ASSERT(obj1.is_null());

    ck_nullable<int> obj2 = 19;
    UT_ASSERT(!obj2.is_null());
    UT_ASSERT(obj2.value() == 19);
    UT_ASSERT(obj2 == 19);

    int i = obj2.value();
    UT_ASSERT(i == 19);

    obj2 = ck_nullable<int>(45);
    UT_ASSERT(!obj2.is_null());
    UT_ASSERT(obj2.value() == 45);
    UT_ASSERT(obj2 == 45);
    UT_ASSERT(obj2 != 19);

    obj2 = obj1;
    UT_ASSERT(obj2.is_null());
}

void ck_nullable_test::test_ck_string()
{
    ck_nullable<ck_string> obj1;
    UT_ASSERT(obj1.is_null());
    obj1.set_value("hello");
    UT_ASSERT(!obj1.is_null());
    UT_ASSERT(obj1.value() == "hello");
    UT_ASSERT(obj1 == "hello");
    UT_ASSERT(obj1 != "world");
    UT_ASSERT("hello" == obj1);
    UT_ASSERT("world" != obj1);
    UT_ASSERT(obj1 == obj1);
    UT_ASSERT(obj1 != ck_nullable<ck_string>("world"));
    obj1 = "world";
    UT_ASSERT(!obj1.is_null());
    UT_ASSERT(obj1.value() == "world");
    UT_ASSERT(obj1 == "world");
    obj1.clear();
    UT_ASSERT(obj1.is_null());

    ck_nullable<ck_string> obj2 = ck_string("foo");
    UT_ASSERT(!obj2.is_null());
    UT_ASSERT(obj2.value() == "foo");
    UT_ASSERT(obj2 == "foo");

    ck_string str = obj2.value();
    UT_ASSERT(str == "foo");

    obj2 = ck_nullable<ck_string>("bar");
    UT_ASSERT(!obj2.is_null());
    UT_ASSERT(obj2.value() == "bar");
    UT_ASSERT(obj2 == "bar");
    UT_ASSERT(obj2 != "foo");

    obj2 = obj1;
    UT_ASSERT(obj2.is_null());
}

void ck_nullable_test::test_shared_ptr()
{
    shared_ptr<int> seven = make_shared<int>();
    *seven = 7;
    shared_ptr<int> fourtyTwo = make_shared<int>();
    *fourtyTwo = 42;
    shared_ptr<int> nineteen = make_shared<int>();
    *fourtyTwo = 19;
    shared_ptr<int> fourtyFive = make_shared<int>();
    *fourtyTwo = 45;

    ck_nullable<shared_ptr<int> > obj1;
    UT_ASSERT(obj1.is_null());
    obj1.set_value(seven);
    UT_ASSERT(!obj1.is_null());
    UT_ASSERT(obj1.value() == seven);
    UT_ASSERT(obj1 == seven);
    UT_ASSERT(obj1 != fourtyTwo);
    UT_ASSERT(seven == obj1);
    UT_ASSERT(fourtyTwo != obj1);
    obj1 = fourtyTwo;
    UT_ASSERT(!obj1.is_null());
    UT_ASSERT(obj1.value() == fourtyTwo);
    UT_ASSERT(obj1 == fourtyTwo);
    obj1.clear();
    UT_ASSERT(obj1.is_null());

    ck_nullable<shared_ptr<int> > obj2 = nineteen;
    UT_ASSERT(!obj2.is_null());
    UT_ASSERT(obj2.value() == nineteen);
    UT_ASSERT(obj2 == nineteen);

    shared_ptr<int> x = obj2.value();
    UT_ASSERT(x == obj2);

    obj2 = ck_nullable<shared_ptr<int> >(fourtyFive);
    UT_ASSERT(!obj2.is_null());
    UT_ASSERT(obj2.value() == fourtyFive);
    UT_ASSERT(obj2 == fourtyFive);
    UT_ASSERT(obj2 != nineteen);

    obj2 = obj1;
    UT_ASSERT(obj2.is_null());
}
