#ifndef _MJPEG_PARSER_TEST_H_
#define _MJPEG_PARSER_TEST_H_

#include <cppunit/extensions/HelperMacros.h>

class MJPEGMediaParserTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(MJPEGMediaParserTest);
        CPPUNIT_TEST(TestTypeCheck);
        CPPUNIT_TEST(TestParse);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp() {}
    void tearDown() {}
protected:
    void TestTypeCheck();
    void TestParse();
};

#endif
