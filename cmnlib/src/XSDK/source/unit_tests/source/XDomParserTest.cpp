#include "XDomParserTest.h"
#include "XSDK/XDomParser.h"
#include "XSDK/XDomParserNode.h"
#include "XSDK/XString.h"
#include "XSDK/OS.h"

#include <cstdlib>

using namespace std;
using namespace XSDK;

REGISTER_TEST_FIXTURE(XDomParserNodeTest);

void XDomParserNodeTest::TestDefaultConstructor(void)
{
    XIRef<XDomParserNode> a = new XDomParserNode;
    UT_ASSERT(a->_tagName.empty());
    UT_ASSERT(a->_data.empty());
    UT_ASSERT(a->_parent == NULL);
    UT_ASSERT(a->_children.empty());
}

/*
void XDomParserNodeTest::TestCopyConstructor(void)
{
    printf("XDomParserNodeTest::TestCopyConstructor()\n");
    fflush(stdout);

    XIRef<XDomParserNode> a = new XDomParserNode;
    XIRef<XDomParserNode> c new XDomParserNode;

    std::list<XIRef<XDomParserNode> > childrenTest;
    std::list<XIRef<XDomParserNode> >::iterator childIter;

    childrenTest.push_back( c );

    a->_tagName = "thisTag";
    a->_data = "stuff in here";
    a->_parent = c;
    a->_children = childrenTest;

    XIRef<XDomParserNode> b = new XDomParserNode( a );

    UT_ASSERT(a._children.empty() == false);
    UT_ASSERT(a._children.size() == 1);
    childIter = a._children.begin();
    UT_ASSERT(a._tagName == "thisTag");
    UT_ASSERT(a._data == "stuff in here");
    UT_ASSERT(a._parent == &c);
    UT_ASSERT(*(childIter) == &c);

    UT_ASSERT(b._children.empty() == false);
    UT_ASSERT(b._children.size() == 1);
    childIter = b._children.begin();
    UT_ASSERT(b._tagName == "thisTag");
    UT_ASSERT(b._data == "stuff in here");
    UT_ASSERT(b._parent == &c);
    UT_ASSERT(*(childIter) == &c);
}

void XDomParserNodeTest::TestAssignmentOperator(void)
{
    printf("XDomParserNodeTest::TestAssignmentOperator()\n");
    fflush(stdout);

    XDomParserNode a;
    XDomParserNode b;
    XDomParserNode c;
    std::list<XDomParserNode*> childrenTest;
    std::list<XDomParserNode*>::iterator childIter;

    childrenTest.push_back(&c);

    a._tagName = "thisTag";
    a._data = "stuff in here";
    a._parent = &c;
    a._children = childrenTest;

    b = a;

    UT_ASSERT(a._children.empty() == false);
    UT_ASSERT(a._children.size() == 1);
    childIter = a._children.begin();
    UT_ASSERT(a._tagName == "thisTag");
    UT_ASSERT(a._data == "stuff in here");
    UT_ASSERT(a._parent == &c);
    UT_ASSERT(*(childIter) == &c);

    UT_ASSERT(b._children.empty() == false);
    UT_ASSERT(b._children.size() == 1);
    childIter = b._children.begin();
    UT_ASSERT(b._tagName == "thisTag");
    UT_ASSERT(b._data == "stuff in here");
    UT_ASSERT(b._parent == &c);
    UT_ASSERT(*(childIter) == &c);
}
*/

void XDomParserNodeTest::TestGetMetaData(void)
{
    {
        XString tag = "<tagName meta1=\"one\" meta2=\"two\" meta3=\"three\" meta4=\"one two three four\"></tagName>";
        XIRef<XDomParserNode> a = new XDomParserNode( tag );

        UT_ASSERT(a->GetMetaData("meta1") == "one");
        UT_ASSERT(a->GetMetaData("meta2") == "two");
        UT_ASSERT(a->GetMetaData("meta3") == "three");
        UT_ASSERT(a->GetMetaData("meta4") == "one two three four");    }
    {
        XString tag = L"<имя_тега מיטאַ1=\"一\" מיטאַ2=\"二\" מיטאַ3=\"三\" מיטאַ4=\"一,二,三,四\" ></имя_тега>";
        XIRef<XDomParserNode> a = new XDomParserNode( tag );
        UT_ASSERT(a->GetMetaData(L"מיטאַ1") == L"一");
        UT_ASSERT(a->GetMetaData(L"מיטאַ2") == L"二");
        UT_ASSERT(a->GetMetaData(L"מיטאַ3") == L"三");
        UT_ASSERT(a->GetMetaData(L"מיטאַ4") == L"一,二,三,四");


    }

}

void XDomParserNodeTest::TestAddMetaData(void)
{
    XString tag = "<TestTag></TestTag>";
    XIRef<XDomParserNode> a = new XDomParserNode;

    a->AddMetaData( "test", "testValue" );
    UT_ASSERT( a->GetMetaData( "test" ) == "testValue" );

    a->AddMetaData( "anotherTest", "anotherValue" );
    UT_ASSERT( a->GetMetaData( "anotherTest" ) == "anotherValue" );

    a->AddMetaData( "spacesTest", "This attribute has spaces!" );
    UT_ASSERT( a->GetMetaData( "spacesTest" ) == "This attribute has spaces!" );

    a->AddMetaData(L"名字",L"王");
    UT_ASSERT( a->GetMetaData( L"名字" ) == L"王" );

}

void XDomParserNodeTest::TestGetData(void)
{
    {
        XString tag = "<TEST_TAG>SOME DATA IT'S SooooOOOOO IMPORTANT YEAH!!!!</TEST_TAG>";
        XIRef<XDomParserNode> a = new XDomParserNode( tag );
        UT_ASSERT(a->GetData() == "SOME DATA IT'S SooooOOOOO IMPORTANT YEAH!!!!");
    }

    {
        XString tag = "<TEST_TAG_PART_DEUX>INFO ... Zelda Rulez</TEST_TAG_PART_DEUX>";
        XIRef<XDomParserNode> a = new XDomParserNode( tag );
        UT_ASSERT(a->GetData() == "INFO ... Zelda Rulez");
    }

    {
        XString tag = "<DILBERT_TAG>\n\t\n\ndogBERT\n\tROX</DILBERT_TAG>";
        XIRef<XDomParserNode> a = new XDomParserNode( tag );
        UT_ASSERT(a->GetData() == "dogBERT\n\tROX");
    }

    {
        XString tag = "<EmptyDataTag></EmptyDataTag>";
        XIRef<XDomParserNode> a = new XDomParserNode( tag );
        UT_ASSERT(a->GetData().empty());
    }

    {
        XString tag = "   \n \r\n<TagWithJunk>\n\n\t\r\nMy Data Section   \n  </TagWithJunk>  ";
        XIRef<XDomParserNode> a = new XDomParserNode( tag );
        UT_ASSERT(a->GetData() == "My Data Section");
    }

    {
        XString tag = "<metaDataTag value=\"x\" another=\" something here \">DataSection Here</metaDataTag>";
        XIRef<XDomParserNode> a = new XDomParserNode( tag );
        UT_ASSERT(a->GetData() == "DataSection Here");
    }

    {
        XString tag = L"<ετικέτα มูลค่า=\"मूल्य\" >数据段</ετικέτα>";
        XIRef<XDomParserNode> a = new XDomParserNode( tag );
        UT_ASSERT(a->GetData() == L"数据段");
    }
}



void XDomParserNodeTest::TestSetData(void)
{
    XString tag = "<tag></tag>";
    XIRef<XDomParserNode> a = new XDomParserNode( tag );

    a->SetData( "Tag Data Section" );
    UT_ASSERT( a->_data == "Tag Data Section" );
    UT_ASSERT( a->GetData() == "Tag Data Section" );

    a->SetData( "Another Data Section" );
    UT_ASSERT( a->_data == "Another Data Section" );
    UT_ASSERT( a->GetData() == "Another Data Section" );

    a->SetData( L"数据段" );
    UT_ASSERT( a->_data == L"数据段" );
    UT_ASSERT( a->GetData() == L"数据段" );
}

void XDomParserNodeTest::TestGetChildren()
{
    XIRef<XDomParserNode> a = new XDomParserNode;
    XIRef<XDomParserNode> b = new XDomParserNode;

    std::list<XIRef<XDomParserNode> > childrenTest;
    childrenTest.push_back( b );
    a->_children = childrenTest;
    UT_ASSERT(a->GetChildren() == childrenTest);
}

void XDomParserNodeTest::TestGetParent()
{
    XIRef<XDomParserNode> parent;
    XIRef<XDomParserNode> child = new XDomParserNode;

    {
        child->_parent = parent = new XDomParserNode;
        UT_ASSERT(child->GetParent() == parent);
    }

    {
        XIRef<XDomParserNode> parentNode = child->GetParent();
        UT_ASSERT( parentNode.IsValid() );
    }
}

void XDomParserNodeTest::TestGetTagName(void)
{
    {
        XString tag = "<TEST_TAG>SOME DATA IT'S SooooOOOOO IMPORTANT YEAH!!!!</TEST_TAG>";
        XIRef<XDomParserNode> a = new XDomParserNode( tag );
        UT_ASSERT(a->GetTagName() == "TEST_TAG");
    }

    {
        XString tag = "<TEST_TAG_PART_DEUX>INFO ... Zelda Rulez</TEST_TAG_PART_DEUX>";
        XIRef<XDomParserNode> a = new XDomParserNode( tag );
        UT_ASSERT(a->GetTagName() == "TEST_TAG_PART_DEUX");
    }

    {
        XString tag = "<DILBERT_TAG>\n\t\n\ndogBERT\n\tROX</DILBERT_TAG>";
        XIRef<XDomParserNode> a = new XDomParserNode( tag );
        UT_ASSERT(a->GetTagName() == "DILBERT_TAG");
    }

    {
        XString tag = "<testTag one=\"1\" two=\"2\">some data here</testTag>";
        XIRef<XDomParserNode> a = new XDomParserNode( tag );
        UT_ASSERT(a->GetTagName() == "testTag");
    }

    {
        XString tag = "<noData></noData>";
        XIRef<XDomParserNode> a = new XDomParserNode( tag );
        UT_ASSERT(a->GetTagName() == "noData");
    }
    {
        XString tag = L"<ετικέτα มูลค่า=\"मूल्य\" >数据段</ετικέτα>";
        XIRef<XDomParserNode> a = new XDomParserNode( tag );
        UT_ASSERT(a->GetTagName() == L"ετικέτα");
    }

    {
        XString tag = L"<ετικέτα></ετικέτα>";
        XIRef<XDomParserNode> a = new XDomParserNode( tag );
        UT_ASSERT(a->GetTagName() == L"ετικέτα");
    }
}


void XDomParserNodeTest::TestParsePath()
{
    {
        vector<XString> result = XDomParserNode::_ParsePath("");
        UT_ASSERT(result.empty());
    }

    {
        vector<XString> result = XDomParserNode::_ParsePath("hello");
        UT_ASSERT(result.size() == 1);
        UT_ASSERT(result[0] == "hello");
    }

    {
        vector<XString> result = XDomParserNode::_ParsePath("hello.world");
        UT_ASSERT(result.size() == 2);
        UT_ASSERT(result[0] == "hello");
        UT_ASSERT(result[1] == "world");
    }

    {
        vector<XString> result = XDomParserNode::_ParsePath("hello.world.goodbye");
        UT_ASSERT(result.size() == 3);
        UT_ASSERT(result[0] == "hello");
        UT_ASSERT(result[1] == "world");
        UT_ASSERT(result[2] == "goodbye");
    }

    {
        vector<XString> result = XDomParserNode::_ParsePath("hello\\.world.goodbye");
        UT_ASSERT(result.size() == 2);
        UT_ASSERT(result[0] == "hello.world");
        UT_ASSERT(result[1] == "goodbye");
    }
}


void XDomParserNodeTest::TestNodeNSIgnore(void)
{
    // The vanilla case...
    {
        XString tag = "<MYNAMESPACE:tagname value=\"x\" another=\" something here \">DataSection Here</MYNAMESPACE:tagname>";
        XIRef<XDomParserNode> a = new XDomParserNode( tag );
        UT_ASSERT(a->GetTagName() == "tagname");
    }

    // The case where a closing tag doesn't quite match...
    {
        XString tag = "<MYNAMESPACE:tagname value=\"x\" another=\" something here \">DataSection Here</tagname>";
        XIRef<XDomParserNode> a = new XDomParserNode( tag );
        UT_ASSERT(a->GetTagName() == "tagname");
    }
}


REGISTER_TEST_FIXTURE(XDomParserTest);

void XDomParserTest::setup(void)
{
    FILE* file = x_fopen( TEST_FILE_NAME,"w" );
    XString document(TEST_DOCUMENT_CONTENTS);
    char * buf = (char *)malloc(document.length());
    memcpy(buf,document.c_str(),document.length());
    fwrite(buf,document.length(),1,file);
    free(buf);
    fclose(file);
}

void XDomParserTest::teardown(void)
{
    remove(TEST_FILE_NAME);
}

void XDomParserTest::TestDefaultConstructor(void)
{
    XIRef<XDomParser> a = new XDomParser;
    a->EnablePrettyPrint();

    UT_ASSERT(a->_doc.empty());
    UT_ASSERT(a->_tagList.empty());
    UT_ASSERT(a->_nodeSet.empty());
    UT_ASSERT(a->_matchingChildren.empty());
}

void XDomParserTest::TestOpenNonExistentDocument()
{
    XIRef<XDomParser> x = new XDomParser;
    x->EnablePrettyPrint();
    UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", x->OpenAndSetDocument( "fake.xml" ), XSDK::XException);
}

void XDomParserTest::TestInvalidDocument()
{
    XIRef<XDomParser> x = new XDomParser;
    x->EnablePrettyPrint();
    x->SetDocument( "some random text\n\nthat is not\nXML data" );
    UT_ASSERT_THROWS_MESSAGE("expected exception not thrown", x->Parse(), XSDK::XDomParserException);
}

void XDomParserTest::TestOpenAndSetDocument(void)
{
    XIRef<XDomParser> a = new XDomParser;
    a->EnablePrettyPrint();

    try
    {
        a->OpenAndSetDocument(TEST_FILE_NAME);
        XString document = TEST_DOCUMENT_CONTENTS;
        UT_ASSERT(a->_doc.length() == document.length());
    }
    catch(XSDK::XException & ex)
    {
        printf("File Name: %s\n",ex.GetSrcFile());
        printf("Line Number: %d\n",ex.GetLineNum());
        printf("Message: %s\n",ex.GetMsg());
        UT_ASSERT(!"OpenAndSetDocument failed!");
    }
}

void XDomParserTest::TestParse(void)
{
    XIRef<XDomParser> a = new XDomParser;
    a->EnablePrettyPrint();
    XIRef<XDomParserNode> b = new XDomParserNode;
    list<XIRef<XDomParserNode> > bChildren;
    list<XIRef<XDomParserNode> >::iterator bChildrenIter;
    try
    {
        a->OpenAndSetDocument(TEST_FILE_NAME);
        b = a->Parse();

        UT_ASSERT( b.IsValid() );
        UT_ASSERT(b->GetTagName() == "TEST_MAJOR");

        bChildren = b->GetChildren();
        UT_ASSERT(bChildren.size() == 5);

        bChildrenIter = bChildren.begin();
        UT_ASSERT((*bChildrenIter)->GetTagName() == "TEST1");
        UT_ASSERT((*bChildrenIter)->GetData() == "TEST_DATA1");

        bChildren = (*bChildrenIter)->GetChildren();
        bChildrenIter = bChildren.begin();
        UT_ASSERT((*bChildrenIter)->GetTagName() == "TEST2");
        UT_ASSERT((*bChildrenIter)->GetData() == "TEST_DATA2");

        bChildren = (*bChildrenIter)->GetChildren();
        bChildrenIter = bChildren.begin();
        UT_ASSERT((*bChildrenIter)->GetTagName() == "TEST3");
        UT_ASSERT((*bChildrenIter)->GetData() == "TEST_DATA3");

        bChildren = b->GetChildren();
        bChildrenIter = bChildren.begin();
        bChildrenIter++;
        UT_ASSERT((*bChildrenIter)->GetTagName() == "test4");
        UT_ASSERT((*bChildrenIter)->GetMetaData("testMetaData1") == "1");

        bChildren = (*bChildrenIter)->GetChildren();
        bChildrenIter = bChildren.begin();
        UT_ASSERT((*bChildrenIter)->GetTagName() == "test5");
        UT_ASSERT((*bChildrenIter)->GetMetaData("testMetaData2") == "2");
        UT_ASSERT((*bChildrenIter)->GetData() == "");

        bChildren = (*bChildrenIter)->GetChildren();
        bChildrenIter = bChildren.begin();
        UT_ASSERT((*bChildrenIter)->GetTagName() == "test6");
        UT_ASSERT((*bChildrenIter)->GetData() == "");

        bChildrenIter++;
        UT_ASSERT((*bChildrenIter)->GetTagName() == "test7");
        UT_ASSERT((*bChildrenIter)->GetData() == "");

        bChildrenIter++;
        UT_ASSERT((*bChildrenIter)->GetTagName() == "test8");
        UT_ASSERT((*bChildrenIter)->GetMetaData("testMetaData3") == "3");
        UT_ASSERT((*bChildrenIter)->GetData() == "");

        bChildren = b->GetChildren();
        bChildrenIter = bChildren.begin();
        bChildrenIter++;
        bChildrenIter++;
        UT_ASSERT((*bChildrenIter)->GetTagName() == "TEST_MINOR");
        UT_ASSERT((*bChildrenIter)->GetData().empty());

        bChildrenIter++;
        UT_ASSERT((*bChildrenIter)->GetTagName() == L"ετικέτα");

        bChildrenIter--;
        bChildren = (*bChildrenIter)->GetChildren();
        bChildrenIter = bChildren.begin();
        //Check size for comments
        UT_ASSERT(bChildren.size() == 2);
        UT_ASSERT((*bChildrenIter)->GetTagName() == "test9");
        UT_ASSERT((*bChildrenIter)->GetMetaData("testMetaData4") == "4");
        UT_ASSERT((*bChildrenIter)->GetData() == "test_data6");

        bChildrenIter++;
        UT_ASSERT((*bChildrenIter)->GetTagName() == "test10");
        //Make sure data is commented out
        UT_ASSERT((*bChildrenIter)->GetData() == "");
    }
    catch(XSDK::XException & ex)
    {
        printf("File Name: %s\n",ex.GetSrcFile());
        printf("Line Number: %d\n",ex.GetLineNum());
        printf("Message: %s\n",ex.GetMsg());
        UT_ASSERT(!"Parse failed!");
    }
}

void XDomParserTest::TestSearchForAll(void)
{
    XIRef<XDomParser> a = new XDomParser;
    a->EnablePrettyPrint();
    XIRef<XDomParserNode> b = new XDomParserNode;

    list<XIRef<XDomParserNode> > bChildren;
    list<XIRef<XDomParserNode> >::iterator bChildrenIter;
    try
    {
        a->OpenAndSetDocument(TEST_FILE_NAME);
        b = a->Parse();
        UT_ASSERT( b.IsValid() );

        bChildren = a->SearchForAll("test10",b);
        bChildrenIter = bChildren.begin();
        UT_ASSERT((*bChildrenIter)->GetTagName() == "test10");
        UT_ASSERT((*bChildrenIter)->GetData().empty());

        bChildren = a->SearchForAll("test9",b);
        bChildrenIter = bChildren.begin();
        UT_ASSERT((*bChildrenIter)->GetTagName() == "test9");
        UT_ASSERT((*bChildrenIter)->GetMetaData("testMetaData4") == "4");
        UT_ASSERT((*bChildrenIter)->GetData() == "test_data6");

        bChildren = a->SearchForAll("TEST_MINOR",b);
        bChildrenIter = bChildren.begin();
        UT_ASSERT((*bChildrenIter)->GetTagName() == "TEST_MINOR");
        UT_ASSERT((*bChildrenIter)->GetData().empty());

        bChildren = a->SearchForAll("test8",b);
        bChildrenIter = bChildren.begin();
        UT_ASSERT((*bChildrenIter)->GetTagName() == "test8");
        UT_ASSERT((*bChildrenIter)->GetMetaData("testMetaData3") == "3");
        UT_ASSERT((*bChildrenIter)->GetData().empty());

        bChildren = a->SearchForAll("test7",b);
        bChildrenIter = bChildren.begin();
        UT_ASSERT((*bChildrenIter)->GetTagName() == "test7");
        UT_ASSERT((*bChildrenIter)->GetData().empty());

        bChildren = a->SearchForAll("test5",b);
        bChildrenIter = bChildren.begin();
        UT_ASSERT((*bChildrenIter)->GetTagName() == "test5");
        UT_ASSERT((*bChildrenIter)->GetMetaData("testMetaData2") == "2");
        UT_ASSERT((*bChildrenIter)->GetData().empty());

        bChildren = a->SearchForAll("test6",b);
        bChildrenIter = bChildren.begin();
        UT_ASSERT((*bChildrenIter)->GetTagName() == "test6");
        UT_ASSERT((*bChildrenIter)->GetData().empty());

        bChildren = a->SearchForAll("TEST2",b);
        bChildrenIter = bChildren.begin();
        UT_ASSERT((*bChildrenIter)->GetTagName() == "TEST2");
        UT_ASSERT((*bChildrenIter)->GetData() == "TEST_DATA2");

        bChildren = a->SearchForAll("TEST1",b);
        bChildrenIter = bChildren.begin();
        UT_ASSERT((*bChildrenIter)->GetTagName() == "TEST1");
        UT_ASSERT((*bChildrenIter)->GetData() == "TEST_DATA1");

        bChildren = a->SearchForAll("TEST3",b);
        bChildrenIter = bChildren.begin();
        UT_ASSERT((*bChildrenIter)->GetTagName() == "TEST3");
        UT_ASSERT((*bChildrenIter)->GetData() == "TEST_DATA3");

        bChildren = a->SearchForAll("test4",b);
        bChildrenIter = bChildren.begin();
        UT_ASSERT((*bChildrenIter)->GetTagName() == "test4");
        UT_ASSERT((*bChildrenIter)->GetMetaData("testMetaData1") == "1");

        bChildren = a->SearchForAll(L"ετικέτα",b);
        bChildrenIter = bChildren.begin();
        UT_ASSERT(bChildren.size() == 2);
        UT_ASSERT((*bChildrenIter)->GetTagName() == L"ετικέτα");
        XString test = (*bChildrenIter)->GetMetaData(L"มูลค่า");
        XString ref =  L"मूल्य";
        UT_ASSERT(test == ref);

    }
    catch(XSDK::XException & ex)
    {
        printf("File Name: %s\n",ex.GetSrcFile());
        printf("Line Number: %d\n",ex.GetLineNum());
        printf("Message: %s\n",ex.GetMsg());
        UT_ASSERT(!"SearchForAll failed!");
    }
}

void XDomParserTest::TestSetDocument()
{
    XIRef<XDomParser> a = new XDomParser;
    a->EnablePrettyPrint();

    try
    {
        XString xmlText = "<tag><foo></foo><bar></bar><bar></bar></tag>";

        a->SetDocument( xmlText );

        XIRef<XDomParserNode> root = a->Parse();

        list<XIRef<XDomParserNode> > foos = a->SearchForAll( "foo", root );
        list<XIRef<XDomParserNode> > bars = a->SearchForAll( "bar", root );

        UT_ASSERT( foos.size() == 1 );
        UT_ASSERT( bars.size() == 2 );
    }
    catch(XSDK::XException & ex)
    {
        printf("File Name: %s\n",ex.GetSrcFile());
        printf("Line Number: %d\n",ex.GetLineNum());
        printf("Message: %s\n",ex.GetMsg());
        UT_ASSERT(!"SearchForAll failed!");
    }
}

void XDomParserTest::TestComment()
{
    XIRef<XDomParser> a = new XDomParser;
    a->EnablePrettyPrint();

    try
    {
        XString xmlText = "<tag><foo><!-- This is a comment--></foo><!-- This is another comment --><bar></bar><bar></bar></tag>";

        a->SetDocument( xmlText );

        XIRef<XDomParserNode> root = a->Parse();

        list<XIRef<XDomParserNode> > foos = a->SearchForAll( "foo", root );
        list<XIRef<XDomParserNode> > bars = a->SearchForAll( "bar", root );

        UT_ASSERT( foos.size() == 1 );
        UT_ASSERT( bars.size() == 2 );
    }
    catch(XSDK::XException & ex)
    {
        printf("File Name: %s\n",ex.GetSrcFile());
        printf("Line Number: %d\n",ex.GetLineNum());
        printf("Message: %s\n",ex.GetMsg());
        UT_ASSERT(!"SearchForAll failed!");
    }
}

void XDomParserTest::TestIgnoreSpecialTags()
{
    XIRef<XDomParser> a = new XDomParser;
    a->EnablePrettyPrint();

    try
    {
        XString xmlText = "<tag><foo><?xml This is something we ignore?></foo><?ver this is something else we ignore?><bar></bar><bar></bar></tag>";

        a->SetDocument( xmlText );

        XIRef<XDomParserNode> root = a->Parse();

        list<XIRef<XDomParserNode> > foos = a->SearchForAll( "foo", root );
        list<XIRef<XDomParserNode> > bars = a->SearchForAll( "bar", root );

        UT_ASSERT( foos.size() == 1 );
        UT_ASSERT( bars.size() == 2 );
    }
    catch(XSDK::XException & ex)
    {
        printf("File Name: %s\n",ex.GetSrcFile());
        printf("Line Number: %d\n",ex.GetLineNum());
        printf("Message: %s\n",ex.GetMsg());
        UT_ASSERT(!"SearchForAll failed!");
    }
}

void XDomParserTest::TestGetDocument()
{
    XIRef<XDomParser> x = new XDomParser;
    x->EnablePrettyPrint();

    XIRef<XDomParserNode> a = new XDomParserNode;
    a->SetTagName( "parentTag" );

    XIRef<XDomParserNode> b = new XDomParserNode;
    b->SetTagName( "child1" );
    a->AppendChild( b );

    XIRef<XDomParserNode> ba = new XDomParserNode;
    ba->SetTagName( "childchild" );
    ba->SetData( "This is my data section" );
    ba->AddMetaData( "long", "very long attribute value" );
    ba->AddMetaData( "one", "1" );
    ba->AddMetaData( "two", "2" );
    b->AppendChild( ba );

    XIRef<XDomParserNode> c = new XDomParserNode;
    c->SetTagName( "child2" );
    a->AppendChild( c );
    c->AddMetaData( "data", "value" );

    XString doc = x->GetDocument( a );
    const XString actualDoc = \
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<parentTag>\n"
        "    <child1>\n"
        "        <childchild long=\"very long attribute value\" one=\"1\" two=\"2\">\n"
        "            This is my data section\n"
        "        </childchild>\n"
        "    </child1>\n"
        "    <child2 data=\"value\">\n"
        "    </child2>\n"
        "</parentTag>\n";

    UT_ASSERT(doc == actualDoc);

    XIRef<XDomParser> y = new XDomParser;
    y->EnablePrettyPrint();
    y->SetDocument( actualDoc );
    XIRef<XDomParserNode> root = y->Parse();
    XString newActualDoc = y->GetDocument( root );

    UT_ASSERT(newActualDoc == actualDoc);
}

void XDomParserTest::TestGetXmlString()
{
    XIRef<XDomParser> x = new XDomParser;
    x->EnablePrettyPrint();

    XIRef<XDomParserNode> a = new XDomParserNode;
    a->SetTagName( "parentTag" );

    XIRef<XDomParserNode> b = new XDomParserNode;
    b->SetTagName( "child1" );
    a->AppendChild( b );

    XIRef<XDomParserNode> ba = new XDomParserNode;
    ba->SetTagName( "childchild" );
    ba->SetData( "This is my data section" );
    ba->AddMetaData( "long", "very long attribute value" );
    ba->AddMetaData( "one", "1" );
    ba->AddMetaData( "two", "2" );
    b->AppendChild( ba );

    XIRef<XDomParserNode> c = new XDomParserNode;
    c->SetTagName( "child2" );
    a->AppendChild( c );
    c->AddMetaData( "data", "value" );

    XString doc = x->GetXmlString( a );
    const XString actualDoc = \
        "<parentTag>\n"
        "    <child1>\n"
        "        <childchild long=\"very long attribute value\" one=\"1\" two=\"2\">\n"
        "            This is my data section\n"
        "        </childchild>\n"
        "    </child1>\n"
        "    <child2 data=\"value\">\n"
        "    </child2>\n"
        "</parentTag>\n";

    UT_ASSERT(doc == actualDoc);
}

void XDomParserTest::TestGetXmlStringUnicode()
{
    XIRef<XDomParser> x = new XDomParser;
    x->EnablePrettyPrint();

    XIRef<XDomParserNode> a = new XDomParserNode;
    a->SetTagName( L"父亲" );

    XIRef<XDomParserNode> b = new XDomParserNode;
    b->SetTagName( L"儿子" );
    a->AppendChild( b );

    XIRef<XDomParserNode> ba = new XDomParserNode;
    ba->SetTagName( L"孙子" );
    ba->SetData( L"甜辣鸡" );
    ba->AddMetaData( L"δεδομένα", L"βάση" );
    ba->AddMetaData( L"データ", L"ベース" );
    ba->AddMetaData( "two", "2" );
    b->AppendChild( ba );

    XIRef<XDomParserNode> c = new XDomParserNode;
    c->SetTagName( L"女儿" );
    a->AppendChild( c );
    c->AddMetaData( "data", "value" );

    XString doc = x->GetXmlString( a );
    const XString actualDoc = \
        L"<父亲>\n"
        L"    <儿子>\n"
        L"        <孙子 δεδομένα=\"βάση\" データ=\"ベース\" two=\"2\">\n"
        L"            甜辣鸡\n"
        L"        </孙子>\n"
        L"    </儿子>\n"
        L"    <女儿 data=\"value\">\n"
        L"    </女儿>\n"
        L"</父亲>\n";

    UT_ASSERT(doc == actualDoc);
}


void XDomParserTest::TestNoNulls()
{
    try
    {
        XIRef<XDomParser> parser = new XDomParser;
        parser->EnablePrettyPrint();

        parser->OpenAndSetDocument(TEST_FILE_NAME);

        InnerTestNoNulls(parser->Parse());
    }
    catch(XSDK::XException & ex)
    {
        printf("File Name: %s\n",ex.GetSrcFile());
        printf("Line Number: %d\n",ex.GetLineNum());
        printf("Message: %s\n",ex.GetMsg());
        UT_ASSERT(!"Parse failed!");
    }
}


void XDomParserTest::InnerTestNoNulls(XIRef<XDomParserNode> node)
{
    UT_ASSERT(node.IsValid());

    for(list<XIRef<XDomParserNode> >::const_iterator iter = node->GetChildren().begin(),
                                                              end  = node->GetChildren().end();
        iter != end;
        ++iter)
    {
        InnerTestNoNulls(*iter);
    }
}


void XDomParserTest::TestGetPath()
{
    try
    {
        XIRef<XDomParser> parser = new XDomParser;
        parser->EnablePrettyPrint();

        parser->OpenAndSetDocument(TEST_FILE_NAME);

        XIRef<XDomParserNode> root = parser->Parse();
        UT_ASSERT(root.IsValid());

        UT_ASSERT(root->GetPath("").empty());
        UT_ASSERT(root->GetPath("hello").empty());
        UT_ASSERT(root->GetPath("TEST").empty());
        UT_ASSERT(root->GetPath("test").empty());
        UT_ASSERT(root->GetPath("hello").empty());
        UT_ASSERT(root->GetPath("TEST_MAJOR.TEST").empty());
        UT_ASSERT(root->GetPath("TEST_MAJOR.test").empty());
        UT_ASSERT(root->GetPath("TEST_MAJOR.TEST1.TEST_DATA1").empty());

        {
            vector<XIRef<XDomParserNode> > result = root->GetPath("TEST_MAJOR.TEST1");
            UT_ASSERT(result.size() == 1);
            UT_ASSERT(result.front()->GetTagName() == "TEST1");
            UT_ASSERT(result.front()->GetChildren().size() == 1);
        }

        {
            vector<XIRef<XDomParserNode> > result = root->GetPath("TEST_MAJOR.TEST1.TEST2");
            UT_ASSERT(result.size() == 1);
            UT_ASSERT(result.front()->GetTagName() == "TEST2");
            UT_ASSERT(result.front()->GetChildren().size() == 1);
        }

        {
            vector<XIRef<XDomParserNode> > result = root->GetPath("TEST_MAJOR.TEST_MINOR");
            UT_ASSERT(result.size() == 1);
            UT_ASSERT(result.front()->GetTagName() == "TEST_MINOR");
            UT_ASSERT(result.front()->GetChildren().size() == 2);
        }

        {
            vector<XIRef<XDomParserNode> > result = root->GetPath(L"TEST_MAJOR.ετικέτα");
            UT_ASSERT(result.size() == 2);
            UT_ASSERT(result[0]->GetTagName() == L"ετικέτα");
            UT_ASSERT(result[0]->GetData() == L"数据段");
            UT_ASSERT(result[0]->GetChildren().empty());
            UT_ASSERT(result[1]->GetTagName() == L"ετικέτα");
            UT_ASSERT(result[1]->GetData() == L"数据段");
            UT_ASSERT(result[1]->GetChildren().empty());
        }
    }
    catch(XSDK::XException & ex)
    {
        printf("File Name: %s\n",ex.GetSrcFile());
        printf("Line Number: %d\n",ex.GetLineNum());
        printf("Message: %s\n",ex.GetMsg());
        UT_ASSERT(!"Parse failed!");
    }
}

void XDomParserTest::TestBrokenXml()
{
    XString brokenXml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<SOAP-ENV:Envelope xmlns:SOAP-ENV=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:SOAP-ENC=\"http://schemas.xmlsoap.org/soap/encoding/\" xmlns:xsi=\"http://www.w3.org/2001\
/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:AlarmArrayConfiguration=\"urn:schemas-pelco-com:service:AlarmArrayConfiguration:1\" xmlns:AudioInputRtsp=\"urn:schemas-pelco-com:service:AudioInputRtsp:1\" xmlns:\
CameraConfiguration=\"urn:schemas-pelco-com:service:CameraConfiguration:1\" xmlns:DVR5100Registration=\"urn:schemas-pelco-com:service:DVR5100Registration:1\" xmlns:EncoderConfiguration=\"urn:schemas-pelco-com:service:EncoderConfiguration\
:1\" xmlns:LensControl=\"urn:schemas-pelco-com:service:LensControl:1\" xmlns:MenuControl=\"urn:schemas-pelco-com:service:MenuControl:1\" xmlns:MetaData=\"urn:schemas-pelco-com:service:MetaData:1\" xmlns:PositioningControl=\"urn:schemas-p\
elco-com:service:PositioningControl:1\" xmlns:RelayArrayConfiguration=\"urn:schemas-pelco-com:service:RelayArrayConfiguration:1\" xmlns:ScriptControl=\"urn:schemas-pelco-com:service:ScriptControl:1\" xmlns:StreamConfiguration=\"urn:schemp\
as-pelco-com:service:StreamConfiguration:1\" xmlns:StreamDiscovery=\"urn:schemas-pelco-com:service:StreamDiscovery:1\" xmlns:AudioOutput=\"urn:schemas-pelco-com:service:AudioOutput:1\" xmlns:MotionDetection=\"urn:schemas-pelco-com:servic\
e:MotionDetection:1\" xmlns:PelcoConfiguration=\"urn:schemas-pelco-com:service:PelcoConfiguration:1\" xmlns:SoftwareUpdate=\"urn:schemas-pelco-com:service:SoftwareUpdate:1\" xmlns:StreamControl=\"urn:schemas-pelco-com:service:StreamContr\
ol:1\" xmlns:VideoOutput=\"urn:schemas-pelco-com:service:VideoOutput:1\" xmlns:AudioConfiguration=\"urn:schemas-pelco-com:service:AudioConfiguration:1\" xmlns:SnmpConfiguration=\"urn:schemas-pelco-com:service:SnmpConfiguration:1\" xmlns:\
DeviceIO=\"http://www.onvif.org/deviceIO/wsdl\" xmlns:DeviceIOExt=\"http://www.pelco.com/api/extension/onvif-ver10/DeviceIO/1\" xmlns:wsnt=\"http://docs.oasis-open.org/wsn/b-2\" xmlns:wsrf-bf=\"http://docs.oasis-open.org/wsrf/bf-2\" xmln\
s:Device=\"http://www.onvif.org/ver10/device/wsdl\" xmlns:faultExt=\"urn:schemas-pelco-com:fault\" xmlns:xop=\"http://www.w3.org/2004/08/xop/include\" xmlns:Media=\"http://www.onvif.org/ver10/media/wsdl\" xmlns:tt=\"http://www.onvif.org/\
ver10/schema\" xmlns:wstop=\"http://docs.oasis-open.org/wsn/t-1\" xmlns:addressing=\"http://www.w3.org/2005/08/addressing\" xmlns:xmime5=\"http://www.w3.org/2005/05/xmlmime\"><SOAP-ENV:Header></SOAP-ENV:Header><SOAP-ENV:Body><VideoOutput\
:QueryResponse><streamCatalog><entries><streamParameters><preAlarmDurationInManualRecord>30</preAlarmDurationInManualRecord><streamSession><iFrameOnly>0</iFrameOnly><startTime>NOW</startTime><endTime>INFINITE</endTime><refreshInterval>90\
0</refreshInt";

    XSDK::XDomParser parser;
    parser.SetDocument( brokenXml );
    UT_ASSERT_THROWS(XIRef<XSDK::XDomParserNode> node = parser.Parse(), XSDK::XDomParserException);
}

void XDomParserTest::TestNonTerminatingXML()
{
    XSDK::XDomParser parser;
    parser.SetDocument( "<foo><bar>" );
    UT_ASSERT_NO_THROW(XIRef<XSDK::XDomParserNode> node = parser.Parse());
}
