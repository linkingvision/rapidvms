#ifndef _MPEG_4_PARSER_TEST_H_
#define _MPEG_4_PARSER_TEST_H_

#include <cppunit/extensions/HelperMacros.h>

class MPEG4MediaParserTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(MPEG4MediaParserTest);
        CPPUNIT_TEST(TestTypeCheck);
        CPPUNIT_TEST(TestParse);
        CPPUNIT_TEST(TestMPF1481);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp() {}
    void tearDown() {}
protected:
    void TestTypeCheck();
    void TestParse();
    void TestMPF1481();
};

#endif
