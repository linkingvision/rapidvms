
#ifndef XSDK_XDomParserTest_h
#define XSDK_XDomParserTest_h

#include "framework.h"

#include "XSDK/XIRef.h"
#include "XSDK/XDomParserNode.h"

class XDomParserNodeTest : public test_fixture
{
public:
    TEST_SUITE(XDomParserNodeTest);
        TEST(XDomParserNodeTest::TestDefaultConstructor);
        // TEST(TestCopyConstructor);
        // TEST(TestAssignmentOperator);
        TEST(XDomParserNodeTest::TestGetMetaData);
        TEST(XDomParserNodeTest::TestAddMetaData);
        TEST(XDomParserNodeTest::TestGetData);
        TEST(XDomParserNodeTest::TestSetData);
        TEST(XDomParserNodeTest::TestGetChildren);
        TEST(XDomParserNodeTest::TestGetParent);
        TEST(XDomParserNodeTest::TestGetTagName);
        TEST(XDomParserNodeTest::TestParsePath);
        TEST(XDomParserNodeTest::TestNodeNSIgnore);
    TEST_SUITE_END();

    void setup() {}
    void teardown() {}

protected:
    void TestDefaultConstructor(void);
    // void TestCopyConstructor(void);
    // void TestAssignmentOperator(void);
    void TestGetMetaData(void);
    void TestAddMetaData(void);
    void TestGetData(void);
    void TestSetData(void);
    void TestGetChildren(void);
    void TestGetParent(void);
    void TestGetTagName(void);
    void TestParsePath();
    void TestNodeNSIgnore();
};

class XDomParserTest : public test_fixture
{
public:
    TEST_SUITE(XDomParserTest);
        TEST(XDomParserTest::TestDefaultConstructor);
        TEST(XDomParserTest::TestOpenNonExistentDocument);
        TEST(XDomParserTest::TestInvalidDocument);
        TEST(XDomParserTest::TestOpenAndSetDocument);
        TEST(XDomParserTest::TestParse);
        TEST(XDomParserTest::TestSearchForAll);
        TEST(XDomParserTest::TestSetDocument);
        TEST(XDomParserTest::TestComment);
        TEST(XDomParserTest::TestIgnoreSpecialTags);
        TEST(XDomParserTest::TestGetDocument);
        TEST(XDomParserTest::TestGetXmlString);
        TEST(XDomParserTest::TestGetXmlStringUnicode);
        TEST(XDomParserTest::TestNoNulls);
        TEST(XDomParserTest::TestGetPath);
        TEST(XDomParserTest::TestBrokenXml);
        TEST(XDomParserTest::TestNonTerminatingXML);
    TEST_SUITE_END();

    void setup();
    void teardown();

protected:
    void TestDefaultConstructor(void);
    void TestOpenNonExistentDocument(void);
    void TestInvalidDocument(void);
    void TestOpenAndSetDocument(void);
    void TestParse(void);
    void TestSearchForAll(void);
    void TestSetDocument(void);
    void TestComment(void);
    void TestIgnoreSpecialTags(void);
    void TestGetDocument(void);
    void TestGetXmlString( void );
    void TestGetXmlStringUnicode();
    void TestNoNulls();
    void TestGetPath();
    void TestBrokenXml();
    void TestNonTerminatingXML();

    void InnerTestNoNulls(XIRef<XSDK::XDomParserNode> node);
};

#define TEST_FILE_NAME "__TestXml.xml__"

#define TEST_DOCUMENT_CONTENTS L"<TEST_MAJOR>\n<TEST1>\nTEST_DATA1\n<TEST2>\nTEST_DATA2<TEST3>TEST_DATA3</TEST3>\n</TEST2></TEST1>\n\n<test4 testMetaData1=\"1\">\n<test5 testMetaData2=\"2\"><test6></test6><test7></test7><test8 testMetaData3=\"3\"></test8>\n</test5>\n</test4>\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n<TEST_MINOR>\n<test9 testMetaData4=\"4\">test_data6</test9>\n\n<!--<THIS_TAG_IS_COMMENTED_OUTcommentedOutMetaData=\"metaData\">COMMENTED_OUT_DATA</THIS_TAG_IS_COMMENTED_OUT> --><test10><!--commented out data --></test10></TEST_MINOR><ετικέτα มูลค่า=\"मूल्य\" >数据段</ετικέτα><ετικέτα มูลค่า=\"मूल्य\" >数据段</ετικέτα></TEST_MAJOR>"

#endif
