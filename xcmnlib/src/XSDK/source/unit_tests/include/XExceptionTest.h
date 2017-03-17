
#ifndef _X_EXCEPTION_TEST_H_
#define _X_EXCEPTOIN_TEST_H_

#include "framework.h"
#include "XSDK/XException.h"

class XExceptionTest :
    public test_fixture
{
public:
    TEST_SUITE(XExceptionTest);
        TEST(XExceptionTest::testConstructors);
        TEST(XExceptionTest::testSetThrowPoint);
        TEST(XExceptionTest::testWhat);
        TEST(XExceptionTest::testGetters);
        TEST(XExceptionTest::testAssignment);
    TEST_SUITE_END();

    void setup();
    void teardown();

protected:
    void testConstructors();
    void testSetThrowPoint();
    void testWhat();
    void testGetters();
    void testAssignment();
};

#endif

