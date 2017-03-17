
#ifndef _H264EncoderTest_H_
#define _H264EncoderTest_H_

#include <cppunit/extensions/HelperMacros.h>
#include "XSDK/XMemory.h"

class H264EncoderTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(H264EncoderTest);
        CPPUNIT_TEST(TestConstructor);
        CPPUNIT_TEST(TestEncodeKey);
        CPPUNIT_TEST(TestEncodeGOP);
    CPPUNIT_TEST_SUITE_END();

public:

    virtual ~H264EncoderTest() throw()
    {}

    void setUp();
    void tearDown();

protected:
    void TestConstructor();
    void TestEncodeKey();
    void TestEncodeGOP();
private:
    XIRef<XSDK::XMemory> _pic;
};

#endif

