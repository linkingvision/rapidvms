
//============================================================================
// Include files
//============================================================================
#include <string>
#include <cstdio>
#include <cstdarg>
#include <cstring>
#include "XSDK/Types.h"
#include "XSDK/XString.h"
#include "XSDK/Object.h"
#include "XSDK/UuidUtils.h"
#include "UuidUtilsTest.h"

//============================================================================
// Test registration
//============================================================================
REGISTER_TEST_FIXTURE(UuidUtilsTest);

#ifdef IS_WINDOWS
        // Disable a compiler warning from Microsoft that simply indicates
        // that the compiler does not enforce the throw specifications
        #pragma warning(push)
        #pragma warning(disable : 4290)

        // Define the stricmp APIs to use
        #define X_SS_STRICMP    stricmp
#else
        // Define the stricmp APIs to use
        #define X_SS_STRICMP    strcasecmp
#endif

class TestClass {

public:
    // 6FABB492-0C1B-4e81-8A6B-8C4D542B65F9
    DEFINE_CLASS_ID(0x6F,0xAB,0xB4,0x92,0x0C,0x1B,0x4e,
        0x81,0x8A,0x6B,0x8C,0x4D,0x54,0x2B,0x65,0xF9)
};

//============================================================================
// @brief Test unit construction.
//
// This routine is called by the test unit framework. The method is
// called in order to allow the test object to create any environment
// needed to complete the tests.
//============================================================================
void UuidUtilsTest::setup()
{
    // Nothing to do
}

//============================================================================
// @brief Tests data storage and retention.
//
// This routine stores data into the object being tested and then makes
// sure the data is preserved as would be expected.
//============================================================================
void UuidUtilsTest::teardown()
{
    // Nothing to do
}

void UuidUtilsTest::testXUuidGenerate()
{
  for(int i=0; i < 10; ++i)
    {
      XSDK::XString res = XSDK::XUuid::Generate();
      UT_ASSERT(XSDK::XUuid::IsStringValid(res));
    }
}

//========================================================================
/// @brief Tests XUuidClear function.
///
/// This routine extensively tests the XUuidClear function.
//========================================================================
void UuidUtilsTest::testXUuidClear()
{

     X_UUID uuid = {{0xff,0xff,0xff,0xff,0xff,0xff,0xff,
        0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff}};

    // Clear the uuid
    XUuidClear(&uuid);

    // Verify that we have the expected uuid values
    XUuidIsEqual(&XSDK::X_EMPTY_UUID,&uuid);
}

//========================================================================
/// @brief Tests XUuidCopy function.
///
/// This routine extensively tests the XUuidCopy function.
//========================================================================
void UuidUtilsTest::testXUuidCopy()
{
    X_UUID uuid1 = {{0xff,0xff,0xff,0xff,0xff,0xff,0xff,
        0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff}};
    X_UUID uuid2 = {{ 0 }};

    // Copy the uuid
    XUuidCopy(&uuid1,&uuid2);
    UT_ASSERT(XUuidIsEqual(&uuid1,&uuid2));

    X_UUID uuid3 = {{0x01,0x02,0x03,0x04,0x05,0x06,0x07,
        0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x10}};
    X_UUID uuid4 = {{ 0 }};

    // Copy the uuid
    XUuidCopy(&uuid3,&uuid4);
    UT_ASSERT(XUuidIsEqual(&uuid3,&uuid4));
}

//========================================================================
/// @brief Tests XUuidIsEqual function.
///
/// This routine extensively tests the XUuidIsEqual function.
//========================================================================
void UuidUtilsTest::testXUuidIsEqual()
{
    X_UUID uuid1 = {{0x01,0x02,0x03,0x04,0x05,0x06,0x07,
        0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x10}};
    X_UUID uuid2 = {{0x10,0x0f,0x0e,0x0d,0x0c,0x0b,0x0a,
        0x09,0x08,0x07,0x06,0x05,0x04,0x03,0x02,0x01}};
    bool result = false;

    // Check for uuid equality
    result = XUuidIsEqual(&uuid1,&uuid1);

    // Verify that we have the expected result
    UT_ASSERT(result == true);

    // Check for uuid equality
    result = XUuidIsEqual(&uuid1,&uuid2);

    // Verify that we have the expected result
    UT_ASSERT(result == false);

    // Check for uuid equality
    result = XUuidIsEqual(&uuid2,&uuid1);

    // Verify that we have the expected result
    UT_ASSERT(result == false);

    // Check for uuid equality
    result = XUuidIsEqual(&uuid2,&uuid2);

    // Verify that we have the expected result
    UT_ASSERT(result == true);
}

//========================================================================
/// @brief Tests XUuidIsEmpty function.
///
/// This routine extensively tests the XUuidIsEmpty function.
//========================================================================
void UuidUtilsTest::testXUuidIsEmpty()
{
    X_UUID uuid = {{0xff,0xff,0xff,0xff,0xff,0xff,0xff,
        0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff}};
    bool result;

    // Check for uuid empty
    result = XUuidIsEmpty(&uuid);

    // Verify that we have the expected result
    UT_ASSERT(result == false);

    // Clear the uuid
    XUuidClear(&uuid);
    // Verify that we have the expected uuid values
    UT_ASSERT(uuid.words.w1 == 0);
    UT_ASSERT(uuid.words.w2 == 0);
    UT_ASSERT(uuid.words.w3 == 0);
    UT_ASSERT(uuid.words.w4 == 0);
}

//========================================================================
/// @brief Tests XUuidIsStringValid function.
///
/// This routine extensively tests the XUuidIsStringValid function.
//========================================================================
void UuidUtilsTest::testXUuidIsStringValid()
{
    char str1[] = "bef9cf75-1f0b-45d0-82c7-3fe8fe97de21";
    char str2[] = "bef9cf75-1f0b-45d082c7-3fe8fe97de21";
    char str3[] = "bef9cf75-1f0b-45d082c#-3fe8fe97de21";
    char str4[] = "bef9cf7z-1f0b-45d082c7-3fe8fe97de21";
    char str5[] = "bef9cf75-1f0b-45d082c7-e8fe97de21";
    bool result = false;

    // Check for uuid string valid
    result = XUuidIsStringValid(str1);

    // Verify that we have the expected result
    UT_ASSERT(result == true);

    // Check for uuid string valid
    result = XUuidIsStringValid(str2);

    // Verify that we have the expected result
    UT_ASSERT(result == false);

    // Check for uuid string valid
    result = XUuidIsStringValid(str3);

    // Verify that we have the expected result
    UT_ASSERT(result == false);

    // Check for uuid string valid
    result = XUuidIsStringValid(str4);

    // Verify that we have the expected result
    UT_ASSERT(result == false);

    // Check for uuid string valid
    result = XUuidIsStringValid(str5);

    // Verify that we have the expected result
    UT_ASSERT(result == false);
}

//========================================================================
/// @brief Tests XUuidFromString function.
///
/// This routine extensively tests the XUuidFromString function.
//========================================================================
void UuidUtilsTest::testXUuidFromStringToString()
{
    char str1[] = "bef9cf75-1f0b-45d0-82c7-3fe8fe97de21";
    char str2[] = "74357C33-9715-4501-A590-C1EB9D580BCF";
    char str3[] = "D9082D30-1714-46d1-9EA4-DB13CC9001CD";
    char str4[] = "35C01e5e-FA37-4111-A670-5AD1D1A95168";
    char str5[] = "43AC426b-a1B5-4f69-9EC7-ddB78E68581B";
    char buf[X_UUID_STRING_LENGTH + 1];
    uint32_t size = 0;
    X_UUID uuid = {{ 0 }};
    bool result = false;

    // Get the size of our UUID string buffer
    size = sizeof(buf) / sizeof(*buf);

    // Clear the buffer memory
    memset(buf,0,size);

    // Convert string to uuid
    result = XUuidFromString(str1,&uuid);

    // Verify that we have the expected result
    UT_ASSERT(result == true);

    // Need to match: bef9cf75-1f0b-45d0-82c7-3fe8fe97de21;
    UT_ASSERT(uuid.uc[ 0] == 0xbe);
    UT_ASSERT(uuid.uc[ 1] == 0xf9);
    UT_ASSERT(uuid.uc[ 2] == 0xcf);
    UT_ASSERT(uuid.uc[ 3] == 0x75);
    UT_ASSERT(uuid.uc[ 4] == 0x1f);
    UT_ASSERT(uuid.uc[ 5] == 0x0b);
    UT_ASSERT(uuid.uc[ 6] == 0x45);
    UT_ASSERT(uuid.uc[ 7] == 0xd0);
    UT_ASSERT(uuid.uc[ 8] == 0x82);
    UT_ASSERT(uuid.uc[ 9] == 0xc7);
    UT_ASSERT(uuid.uc[10] == 0x3f);
    UT_ASSERT(uuid.uc[11] == 0xe8);
    UT_ASSERT(uuid.uc[12] == 0xfe);
    UT_ASSERT(uuid.uc[13] == 0x97);
    UT_ASSERT(uuid.uc[14] == 0xde);
    UT_ASSERT(uuid.uc[15] == 0x21);

    // Convert uuid to string
    result = XUuidToString(&uuid,buf,size);

    // Verify that we have the expected result
    UT_ASSERT(result == true);

    UT_ASSERT(strlen(str1) == strlen(buf));
    UT_ASSERT(X_SS_STRICMP(str1,buf) == 0);

    // Convert string to uuid
    result = XUuidFromString(str2,&uuid);

    // Verify that we have the expected result
    UT_ASSERT(result == true);

    // Need to match: 74357C33-9715-4501-A590-C1EB9D580BCF
    UT_ASSERT(uuid.uc[ 0] == 0x74);
    UT_ASSERT(uuid.uc[ 1] == 0x35);
    UT_ASSERT(uuid.uc[ 2] == 0x7c);
    UT_ASSERT(uuid.uc[ 3] == 0x33);
    UT_ASSERT(uuid.uc[ 4] == 0x97);
    UT_ASSERT(uuid.uc[ 5] == 0x15);
    UT_ASSERT(uuid.uc[ 6] == 0x45);
    UT_ASSERT(uuid.uc[ 7] == 0x01);
    UT_ASSERT(uuid.uc[ 8] == 0xa5);
    UT_ASSERT(uuid.uc[ 9] == 0x90);
    UT_ASSERT(uuid.uc[10] == 0xc1);
    UT_ASSERT(uuid.uc[11] == 0xeb);
    UT_ASSERT(uuid.uc[12] == 0x9d);
    UT_ASSERT(uuid.uc[13] == 0x58);
    UT_ASSERT(uuid.uc[14] == 0x0b);
    UT_ASSERT(uuid.uc[15] == 0xcf);

    // Convert uuid to string
    result = XUuidToString(&uuid,buf,-1);

    // Verify that we have the expected result
    UT_ASSERT(result == true);

    UT_ASSERT(strlen(str2) == strlen(buf));
    UT_ASSERT(X_SS_STRICMP(str2,buf) == 0);

    // Convert string to uuid
    result = XUuidFromString(str3,&uuid);

    // Verify that we have the expected result
    UT_ASSERT(result == true);

    // Need to match: D9082D30-1714-46d1-9EA4-DB13CC9001CD
    UT_ASSERT(uuid.uc[ 0] == 0xd9);
    UT_ASSERT(uuid.uc[ 1] == 0x08);
    UT_ASSERT(uuid.uc[ 2] == 0x2d);
    UT_ASSERT(uuid.uc[ 3] == 0x30);
    UT_ASSERT(uuid.uc[ 4] == 0x17);
    UT_ASSERT(uuid.uc[ 5] == 0x14);
    UT_ASSERT(uuid.uc[ 6] == 0x46);
    UT_ASSERT(uuid.uc[ 7] == 0xd1);
    UT_ASSERT(uuid.uc[ 8] == 0x9e);
    UT_ASSERT(uuid.uc[ 9] == 0xa4);
    UT_ASSERT(uuid.uc[10] == 0xdb);
    UT_ASSERT(uuid.uc[11] == 0x13);
    UT_ASSERT(uuid.uc[12] == 0xcc);
    UT_ASSERT(uuid.uc[13] == 0x90);
    UT_ASSERT(uuid.uc[14] == 0x01);
    UT_ASSERT(uuid.uc[15] == 0xcd);

    // Convert uuid to string
    result = XUuidToString(&uuid,buf,-1);

    // Verify that we have the expected result
    UT_ASSERT(result == true);

    UT_ASSERT(strlen(str3) == strlen(buf));
    UT_ASSERT(X_SS_STRICMP(str3,buf) == 0);

    // Convert string to uuid
    result = XUuidFromString(str4,&uuid);

    // Verify that we have the expected result
    UT_ASSERT(result == true);

    // Need to match: 35C01e5e-FA37-4111-A670-5AD1D1A95168
    UT_ASSERT(uuid.uc[ 0] == 0x35);
    UT_ASSERT(uuid.uc[ 1] == 0xc0);
    UT_ASSERT(uuid.uc[ 2] == 0x1e);
    UT_ASSERT(uuid.uc[ 3] == 0x5e);
    UT_ASSERT(uuid.uc[ 4] == 0xfa);
    UT_ASSERT(uuid.uc[ 5] == 0x37);
    UT_ASSERT(uuid.uc[ 6] == 0x41);
    UT_ASSERT(uuid.uc[ 7] == 0x11);
    UT_ASSERT(uuid.uc[ 8] == 0xa6);
    UT_ASSERT(uuid.uc[ 9] == 0x70);
    UT_ASSERT(uuid.uc[10] == 0x5a);
    UT_ASSERT(uuid.uc[11] == 0xd1);
    UT_ASSERT(uuid.uc[12] == 0xd1);
    UT_ASSERT(uuid.uc[13] == 0xa9);
    UT_ASSERT(uuid.uc[14] == 0x51);
    UT_ASSERT(uuid.uc[15] == 0x68);

    // Convert uuid to string
    result = XUuidToString(&uuid,buf,-1);

    // Verify that we have the expected result
    UT_ASSERT(result == true);

    UT_ASSERT(strlen(str4) == strlen(buf));
    UT_ASSERT(X_SS_STRICMP(str4,buf) == 0);

    // Convert string to uuid
    result = XUuidFromString(str5,&uuid);

    // Verify that we have the expected result
    UT_ASSERT(result == true);

    // Need to match: 43AC426b-a1B5-4f69-9EC7-ddB78E68581B
    UT_ASSERT(uuid.uc[ 0] == 0x43);
    UT_ASSERT(uuid.uc[ 1] == 0xac);
    UT_ASSERT(uuid.uc[ 2] == 0x42);
    UT_ASSERT(uuid.uc[ 3] == 0x6b);
    UT_ASSERT(uuid.uc[ 4] == 0xa1);
    UT_ASSERT(uuid.uc[ 5] == 0xb5);
    UT_ASSERT(uuid.uc[ 6] == 0x4f);
    UT_ASSERT(uuid.uc[ 7] == 0x69);
    UT_ASSERT(uuid.uc[ 8] == 0x9e);
    UT_ASSERT(uuid.uc[ 9] == 0xc7);
    UT_ASSERT(uuid.uc[10] == 0xdd);
    UT_ASSERT(uuid.uc[11] == 0xb7);
    UT_ASSERT(uuid.uc[12] == 0x8e);
    UT_ASSERT(uuid.uc[13] == 0x68);
    UT_ASSERT(uuid.uc[14] == 0x58);
    UT_ASSERT(uuid.uc[15] == 0x1b);

    // Convert uuid to string
    result = XUuidToString(&uuid,buf,-1);

    // Verify that we have the expected result
    UT_ASSERT(result == true);

    UT_ASSERT(strlen(str5) == strlen(buf));
    UT_ASSERT(X_SS_STRICMP(str5,buf) == 0);
}

//========================================================================
/// @brief Tests the validity of a class uuid set using a number of methods.
///
/// This routine sets the uuid of a class using a number of methods and
/// verifies correctness.
//========================================================================
void UuidUtilsTest::testClassUuidValidity()
{
    char str[] = "6FABB492-0C1B-4e81-8A6B-8C4D542B65F9";
    X_UUID uuid = TestClass::ClassId();
    char buf[1024];
    bool result = false;

    // Clear the buffer memory
    memset(buf,0,sizeof(buf)/sizeof(*buf));

    // Convert uuid to string
    result = XUuidToString(&uuid,buf,-1);

    // Verify that we have the expected result
    UT_ASSERT(result == true);

    UT_ASSERT(strlen(str) == strlen(buf));
    UT_ASSERT(X_SS_STRICMP(str,buf) == 0);
}

