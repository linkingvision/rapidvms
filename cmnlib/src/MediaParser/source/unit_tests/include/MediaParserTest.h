
#ifndef _MEDIA_PARSER_TEST_H_
#define _MEDIA_PARSER_TEST_H_

#include <cppunit/extensions/HelperMacros.h>

class MediaParserTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(MediaParserTest);
        CPPUNIT_TEST(TestCreateMediaInfo);
        CPPUNIT_TEST(TestTypeCheck);
        CPPUNIT_TEST(TestGetMediaTypeString);
        CPPUNIT_TEST(TestGetMediaType);
        CPPUNIT_TEST(TestGetMimeType);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp() {}
    void tearDown() {}

protected:
    void TestCreateMediaInfo();
    void TestTypeCheck();
    void TestGetMediaTypeString();
    void TestGetMediaType();
    void TestGetMimeType();
};

#endif
