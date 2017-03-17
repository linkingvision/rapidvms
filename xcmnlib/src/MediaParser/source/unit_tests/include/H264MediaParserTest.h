#ifndef _H264_MEDIA_PARSER_TEST_H_
#define _H264_MEDIA_PARSER_TEST_H_

#include <cppunit/extensions/HelperMacros.h>

class H264MediaParserTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(H264MediaParserTest);
        CPPUNIT_TEST(TestParseUEV);
        CPPUNIT_TEST(TestParseSEV);
        CPPUNIT_TEST(TestTypeCheck);
        CPPUNIT_TEST(TestParse);
        CPPUNIT_TEST(TestResolution);
        CPPUNIT_TEST(TestBitrate);
        CPPUNIT_TEST(TestChromaFormat);
        CPPUNIT_TEST(TestLevelIDC);
        CPPUNIT_TEST(TestUserData);
        CPPUNIT_TEST(TestMPF1481);
        CPPUNIT_TEST(TestSDPCodecData);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp() {}
    void tearDown() {}
protected:
    void TestParseUEV();
    void TestParseSEV();
    void TestTypeCheck();
    void TestParse();
    void TestResolution();
    void TestFramerate();
    void TestBitrate();
    void TestChromaFormat();
    void TestLevelIDC();
    void TestUserData();
    void TestMPF1481();
    void TestSDPCodecData();
};

#endif
