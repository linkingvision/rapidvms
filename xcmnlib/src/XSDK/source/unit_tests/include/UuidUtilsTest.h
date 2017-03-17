
#ifndef _UUID_UTILS_TEST_H_
#define _UUID_UTILS_TEST_H_

//============================================================================
// Include files
//============================================================================
#include "framework.h"
#include "XSDK/UuidUtils.h"
#include "XSDK/XUuid.h"

//============================================================================
// @brief UuidUtilsTest unit test class.
//============================================================================
class UuidUtilsTest:
    public test_fixture
{
public:
    //========================================================================
    // Test declarations
    //========================================================================
    TEST_SUITE(UuidUtilsTest);
        TEST(UuidUtilsTest::testXUuidClear);
        TEST(UuidUtilsTest::testXUuidCopy);
        TEST(UuidUtilsTest::testXUuidIsEqual);
        TEST(UuidUtilsTest::testXUuidIsEmpty);
        TEST(UuidUtilsTest::testXUuidIsStringValid);
        TEST(UuidUtilsTest::testXUuidFromStringToString);
        TEST(UuidUtilsTest::testClassUuidValidity);
        TEST(UuidUtilsTest::testXUuidGenerate);
    TEST_SUITE_END();

    //========================================================================
    /// @brief Test unit construction.
    ///
    /// This routine is called by the test unit framework. The method is
    /// called in order to allow the test object to create any environment
    /// needed to complete the tests.
    //========================================================================
    void setup();

    //========================================================================
    /// @brief Test unit destruction.
    ///
    /// This routine is called by the test unit framework. The method is
    /// to be used by the object to cleanup after all tests have been run.
    //========================================================================
    void teardown();

protected:
    void testXUuidGenerate();

    //========================================================================
    /// @brief Tests XUuidClear function.
    ///
    /// This routine extensively tests the XUuidClear function.
    //========================================================================
    void testXUuidClear();

    //========================================================================
    /// @brief Tests XUuidCopy function.
    ///
    /// This routine extensively tests the XUuidCopy function.
    //========================================================================
    void testXUuidCopy();

    //========================================================================
    /// @brief Tests XUuidIsEqual function.
    ///
    /// This routine extensively tests the XUuidIsEqual function.
    //========================================================================
    void testXUuidIsEqual();

    //========================================================================
    /// @brief Tests XUuidIsEmpty function.
    ///
    /// This routine extensively tests the XUuidIsEmpty function.
    //========================================================================
    void testXUuidIsEmpty();

    //========================================================================
    /// @brief Tests XUuidIsStringValid function.
    ///
    /// This routine extensively tests the XUuidIsStringValid function.
    //========================================================================
    void testXUuidIsStringValid();

    //========================================================================
    /// @brief Tests XUuidFromString and XUuidToString functions.
    ///
    /// This routine extensively tests the XUuidFromString and
    /// XUuidToString functions.
    //========================================================================
    void testXUuidFromStringToString();

    //========================================================================
    /// @brief Tests the validity of a class uuid set using a number of
    /// methods.
    ///
    /// This routine sets the uuid of a class using a number of methods and
    /// verifies correctness.
    //========================================================================
    void testClassUuidValidity();
};

#endif // _UUID_UTILS_TEST_H_

