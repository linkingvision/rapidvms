
#ifndef _AVMuxerTest_H_
#define _AVMuxerTest_H_

#include <cppunit/extensions/HelperMacros.h>
#include "XSDK/XMemory.h"

class AVMuxerTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(AVMuxerTest);
        CPPUNIT_TEST(TestConstructor);
        CPPUNIT_TEST(TestMP4);
        CPPUNIT_TEST(TestReContainerize);
    CPPUNIT_TEST_SUITE_END();

public:

    virtual ~AVMuxerTest() throw() {}

    void setUp();
    void tearDown();

protected:
    void TestConstructor();
    void TestMP4();
    void TestReContainerize();
private:
    XIRef<XSDK::XMemory> _pic;
};

#endif

