
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "XSDK/XException.h"
#include "XSDK/XString.h"
#include "XExceptionTest.h"

using namespace XSDK;

REGISTER_TEST_FIXTURE(XExceptionTest);

void XExceptionTest::setup()
{
}

void XExceptionTest::teardown()
{
}

void XExceptionTest::testConstructors()
{
    XException e( "Hello, Exceptions!");

    XString whatMsg = e.GetMsg();

    UT_ASSERT( whatMsg == "Hello, Exceptions!" );
}

void XExceptionTest::testSetThrowPoint()
{
    XException e( "Hello, Exceptions!");

    e.SetThrowPoint( 42, "coolfile.cpp" );

    XString whatMsg = e.what();

    UT_ASSERT( whatMsg.Contains( "thrown from" ) );
}

void XExceptionTest::testWhat()
{
    XException e( "Hello, Exceptions!");

    XString whatMsg = e.what();

    UT_ASSERT( whatMsg.Contains( "Hello, Exceptions!" ) );
}

void XExceptionTest::testGetters()
{
}

void XExceptionTest::testAssignment()
{
    XException e( "Hello, Exceptions!");
    XException e2 = e;

    XString whatMsg = e2.GetMsg();

    UT_ASSERT( whatMsg == "Hello, Exceptions!" );
}
