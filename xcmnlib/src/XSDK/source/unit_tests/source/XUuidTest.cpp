
#include "XUuidTest.h"

#include "XSDK/XUuid.h"
#include "XSDK/XException.h"

using namespace std;
using namespace XSDK;

REGISTER_TEST_FIXTURE(XUuidTest);

void XUuidTest::TestConstructor()
{
        UT_ASSERT(XUuid("F89D1973-CA31-4AA4-8F53-836631929CAE").ToString() ==
                           "F89D1973-CA31-4AA4-8F53-836631929CAE");

        XSDK::XUuid testId("F89D1973-CA31-4AA4-8F53-836631929CAE");
        UT_ASSERT_THROWS(testId.FromString("hello world"), XException);
}

void XUuidTest::TestGetUuid()
{
        XSDK::XUuid testId("F89D1973-CA31-4AA4-8F53-836631929CAE");
        X_UUID result = testId.GetUuid();
        UT_ASSERT(result.uc[0] == 0xF8);
        UT_ASSERT(result.uc[1] == 0x9D);
        UT_ASSERT(result.uc[2] == 0x19);
        UT_ASSERT(result.uc[3] == 0x73);
        UT_ASSERT(result.uc[4] == 0xCA);
        UT_ASSERT(result.uc[5] == 0x31);
        UT_ASSERT(result.uc[6] == 0x4A);
        UT_ASSERT(result.uc[7] == 0xA4);
        UT_ASSERT(result.uc[8] == 0x8F);
        UT_ASSERT(result.uc[9] == 0x53);
        UT_ASSERT(result.uc[10] == 0x83);
        UT_ASSERT(result.uc[11] == 0x66);
        UT_ASSERT(result.uc[12] == 0x31);
        UT_ASSERT(result.uc[13] == 0x92);
        UT_ASSERT(result.uc[14] == 0x9C);
        UT_ASSERT(result.uc[15] == 0xAE);
}

void XUuidTest::TestIsEmpty()
{
        XSDK::XUuid testId("F89D1973-CA31-4AA4-8F53-836631929CAE");
        UT_ASSERT(!testId.IsEmpty());
        testId.Clear();
        UT_ASSERT(testId.IsEmpty());
}

void XUuidTest::TestToString()
{
        XSDK::XUuid testId("F89D1973-CA31-4AA4-8F53-836631929CAE");
        UT_ASSERT(testId.ToString() == "F89D1973-CA31-4AA4-8F53-836631929CAE");
}

void XUuidTest::TestIsStringValid()
{
        UT_ASSERT(XUuid::IsStringValid("F89D1973-CA31-4AA4-8F53-836631929CAE"));

        UT_ASSERT(!XUuid::IsStringValid("F89D1973C-A31-4AA4-8F53-836631929CAE"));
        UT_ASSERT(!XUuid::IsStringValid("F89D1973-CA31-4AA4-8F53-836631929CA"));
        UT_ASSERT(!XUuid::IsStringValid("F89D1973CA314AA48F53836631929CAE"));
}

void XUuidTest::TestFromString()
{
        XSDK::XUuid testId("F89D1973-CA31-4aa4-8F53-836631929CAE");
        testId.FromString("61C11390-08CE-403B-B6AA-9483A2F50194");
        UT_ASSERT(testId.ToString() == "61C11390-08CE-403B-B6AA-9483A2F50194");

        UT_ASSERT_THROWS(testId.FromString("hello world"), XException);
}

void XUuidTest::TestGenerate()
{
    XSDK::XUuid testId("F89D1973-CA31-4AA4-8F53-836631929CAE");

    for (int i=0; i<=100; i++)
    {
        UT_ASSERT(XUuid::IsStringValid(XUuid::Generate()));
    }
}

void XUuidTest::TestClear()
{
    XSDK::XUuid testId("F89D1973-CA31-4AA4-8F53-836631929CAE");
    UT_ASSERT(!testId.IsEmpty());
    testId.Clear();
    UT_ASSERT(testId.IsEmpty());
}
