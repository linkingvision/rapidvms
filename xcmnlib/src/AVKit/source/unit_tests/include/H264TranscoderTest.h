
#ifndef _H264TranscoderTest_H_
#define _H264TranscoderTest_H_

#include <cppunit/extensions/HelperMacros.h>
#include "XSDK/XMemory.h"

class H264TranscoderTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(H264TranscoderTest);
        CPPUNIT_TEST(TestConstructor);
        CPPUNIT_TEST(TestBasic);
    CPPUNIT_TEST_SUITE_END();

public:

    virtual ~H264TranscoderTest() throw() {}

    void setUp();
    void tearDown();

protected:
    void TestConstructor();
    void TestBasic();
};

#endif

