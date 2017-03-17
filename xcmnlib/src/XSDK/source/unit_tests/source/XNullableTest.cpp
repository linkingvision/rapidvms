
#include "XNullableTest.h"

#include "XSDK/XNullable.h"
#include "XSDK/XRef.h"
#include "XSDK/XString.h"

using namespace std;
using namespace XSDK;

REGISTER_TEST_FIXTURE(XNullableTest);


void XNullableTest::TestBool()
{
    XNullable<bool> obj1;
    UT_ASSERT(obj1.IsNull());
    obj1.SetValue(true);
    UT_ASSERT(!obj1.IsNull());
    UT_ASSERT(obj1.Value() == true);
    UT_ASSERT(obj1 == true);
    UT_ASSERT(obj1 != false);
    UT_ASSERT(true == obj1);
    UT_ASSERT(false != obj1);
    UT_ASSERT(obj1 == obj1);
    UT_ASSERT(obj1 != XNullable<bool>(false));
    obj1 = false;
    UT_ASSERT(!obj1.IsNull());
    UT_ASSERT(obj1.Value() == false);
    UT_ASSERT(obj1 == false);
    obj1.Clear();
    UT_ASSERT(obj1.IsNull());

    XNullable<bool> obj2 = false;
    UT_ASSERT(!obj2.IsNull());
    UT_ASSERT(obj2.Value() == false);
    UT_ASSERT(obj2 == false);

    bool b = obj2.Value();
    UT_ASSERT(b == false);

    obj2 = XNullable<bool>(true);
    UT_ASSERT(!obj2.IsNull());
    UT_ASSERT(obj2.Value() == true);
    UT_ASSERT(obj2 == true);
    UT_ASSERT(obj2 != false);

    obj2 = obj1;
    UT_ASSERT(obj2.IsNull());
}


void XNullableTest::TestInt()
{
    XNullable<int> obj1;
    UT_ASSERT(obj1.IsNull());
    obj1.SetValue(7);
    UT_ASSERT(!obj1.IsNull());
    UT_ASSERT(obj1.Value() == 7);
    UT_ASSERT(obj1 == 7);
    UT_ASSERT(obj1 != 42);
    UT_ASSERT(7 == obj1);
    UT_ASSERT(42 != obj1);
    UT_ASSERT(obj1 == obj1);
    UT_ASSERT(obj1 != XNullable<int>(99));
    obj1 = 42;
    UT_ASSERT(!obj1.IsNull());
    UT_ASSERT(obj1.Value() == 42);
    UT_ASSERT(obj1 == 42);
    obj1.Clear();
    UT_ASSERT(obj1.IsNull());

    XNullable<int> obj2 = 19;
    UT_ASSERT(!obj2.IsNull());
    UT_ASSERT(obj2.Value() == 19);
    UT_ASSERT(obj2 == 19);

    int i = obj2.Value();
    UT_ASSERT(i == 19);

    obj2 = XNullable<int>(45);
    UT_ASSERT(!obj2.IsNull());
    UT_ASSERT(obj2.Value() == 45);
    UT_ASSERT(obj2 == 45);
    UT_ASSERT(obj2 != 19);

    obj2 = obj1;
    UT_ASSERT(obj2.IsNull());
}


void XNullableTest::TestXString()
{
    XNullable<XString> obj1;
    UT_ASSERT(obj1.IsNull());
    obj1.SetValue("hello");
    UT_ASSERT(!obj1.IsNull());
    UT_ASSERT(obj1.Value() == "hello");
    UT_ASSERT(obj1 == "hello");
    UT_ASSERT(obj1 != "world");
    UT_ASSERT("hello" == obj1);
    UT_ASSERT("world" != obj1);
    UT_ASSERT(obj1 == obj1);
    UT_ASSERT(obj1 != XNullable<XString>("world"));
    obj1 = "world";
    UT_ASSERT(!obj1.IsNull());
    UT_ASSERT(obj1.Value() == "world");
    UT_ASSERT(obj1 == "world");
    obj1.Clear();
    UT_ASSERT(obj1.IsNull());

    XNullable<XString> obj2 = XString("foo");
    UT_ASSERT(!obj2.IsNull());
    UT_ASSERT(obj2.Value() == "foo");
    UT_ASSERT(obj2 == "foo");

    XString str = obj2.Value();
    UT_ASSERT(str == "foo");

    obj2 = XNullable<XString>("bar");
    UT_ASSERT(!obj2.IsNull());
    UT_ASSERT(obj2.Value() == "bar");
    UT_ASSERT(obj2 == "bar");
    UT_ASSERT(obj2 != "foo");

    obj2 = obj1;
    UT_ASSERT(obj2.IsNull());
}


void XNullableTest::TestXRef()
{
    XRef<int> seven = new int;
    *seven = 7;
    XRef<int> fourtyTwo = new int;
    *fourtyTwo = 42;
    XRef<int> nineteen = new int;
    *fourtyTwo = 19;
    XRef<int> fourtyFive = new int;
    *fourtyTwo = 45;

    XNullable<XRef<int> > obj1;
    UT_ASSERT(obj1.IsNull());
    obj1.SetValue(seven);
    UT_ASSERT(!obj1.IsNull());
    UT_ASSERT(obj1.Value() == seven);
    UT_ASSERT(obj1 == seven);
    UT_ASSERT(obj1 != fourtyTwo);
    UT_ASSERT(seven == obj1);
    UT_ASSERT(fourtyTwo != obj1);
    obj1 = fourtyTwo;
    UT_ASSERT(!obj1.IsNull());
    UT_ASSERT(obj1.Value() == fourtyTwo);
    UT_ASSERT(obj1 == fourtyTwo);
    obj1.Clear();
    UT_ASSERT(obj1.IsNull());

    XNullable<XRef<int> > obj2 = nineteen;
    UT_ASSERT(!obj2.IsNull());
    UT_ASSERT(obj2.Value() == nineteen);
    UT_ASSERT(obj2 == nineteen);

    XRef<int> x = obj2.Value();
    UT_ASSERT(x == obj2);

    obj2 = XNullable<XRef<int> >(fourtyFive);
    UT_ASSERT(!obj2.IsNull());
    UT_ASSERT(obj2.Value() == fourtyFive);
    UT_ASSERT(obj2 == fourtyFive);
    UT_ASSERT(obj2 != nineteen);

    obj2 = obj1;
    UT_ASSERT(obj2.IsNull());
}
