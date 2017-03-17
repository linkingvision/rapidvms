
#ifndef _AVDeMuxerTest_H_
#define _AVDeMuxerTest_H_

#include <cppunit/extensions/HelperMacros.h>
#include "XSDK/XMemory.h"

class AVDeMuxerTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(AVDeMuxerTest);
        CPPUNIT_TEST(TestConstructor);
        CPPUNIT_TEST(TestExamineFile);
        CPPUNIT_TEST(TestFileFromMemory);
        CPPUNIT_TEST(TestGetContainerStatistics);
    CPPUNIT_TEST_SUITE_END();

public:

    virtual ~AVDeMuxerTest() throw() {}

    void setUp();
    void tearDown();

protected:
    void TestConstructor();
    void TestExamineFile();
    void TestFileFromMemory();
    void TestGetContainerStatistics();
};

#endif

