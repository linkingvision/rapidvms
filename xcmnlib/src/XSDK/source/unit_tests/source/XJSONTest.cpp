
#include "XJSONTest.h"
#include "XSDK/OS/STL.h"
#include "XSDK/XString.h"
#include "XSDK/XJSON.h"

using namespace XSDK;
using namespace std;

REGISTER_TEST_FIXTURE(XJSONTest);

void XJSONTest::TestT247_2()
{
    {
        XString doc =  "{ \"data\": { \"motion_events\": [ ] } }";
        XIRef<XJSONItem> item = NULL;
        UT_ASSERT_NO_THROW( item = XJSONItem::ParseDocument(doc) );
        UT_ASSERT(item.IsValid());
        XIRef<XJSONObject> obj = item;
        XIRef<XJSONObject> obj2 = obj->GetObjectMember("data");
        XIRef<XJSONArray> ary = obj2->GetObjectMember("motion_events");
        UT_ASSERT( ary->Count() == 0 );
    }
    {
        XString doc =  "{ \"data\": { \"motion_events\": [] } }";
        XIRef<XJSONItem> item = NULL;
        UT_ASSERT_NO_THROW( item = XJSONItem::ParseDocument(doc) );
        UT_ASSERT(item.IsValid());
        XIRef<XJSONObject> obj = item;
        XIRef<XJSONObject> obj2 = obj->GetObjectMember("data");
        XIRef<XJSONArray> ary = obj2->GetObjectMember("motion_events");
        UT_ASSERT( ary->Count() == 0 );
    }
    {
        XString doc =  "{ \"data\": { \"motion_events\": [ ], \"motion_events2\": [ ] } }";
        XIRef<XJSONItem> item = NULL;
        UT_ASSERT_NO_THROW( item = XJSONItem::ParseDocument(doc) );
        UT_ASSERT(item.IsValid());
        XIRef<XJSONObject> obj = item;
        XIRef<XJSONObject> obj2 = obj->GetObjectMember("data");
        XIRef<XJSONArray> ary = obj2->GetObjectMember("motion_events");
        UT_ASSERT( ary->Count() == 0 );
        ary = obj2->GetObjectMember("motion_events2");
        UT_ASSERT( ary->Count() == 0 );
    }
    {
        XString doc =  "{ \"data\": { \"motion_events\": [], \"motion_events2\": [ ] } }";
        XIRef<XJSONItem> item = NULL;
        UT_ASSERT_NO_THROW( item = XJSONItem::ParseDocument(doc) );
        UT_ASSERT(item.IsValid());
        XIRef<XJSONObject> obj = item;
        XIRef<XJSONObject> obj2 = obj->GetObjectMember("data");
        XIRef<XJSONArray> ary = obj2->GetObjectMember("motion_events");
        UT_ASSERT( ary->Count() == 0 );
        ary = obj2->GetObjectMember("motion_events2");
        UT_ASSERT( ary->Count() == 0 );
    }
    {
        XString doc =  "{ \"data\": { \"motion_events\": [ ], \"motion_events2\": [] } }";
        XIRef<XJSONItem> item = NULL;
        UT_ASSERT_NO_THROW( item = XJSONItem::ParseDocument(doc) );
        UT_ASSERT(item.IsValid());
        XIRef<XJSONObject> obj = item;
        XIRef<XJSONObject> obj2 = obj->GetObjectMember("data");
        XIRef<XJSONArray> ary = obj2->GetObjectMember("motion_events");
        UT_ASSERT( ary->Count() == 0 );
        ary = obj2->GetObjectMember("motion_events2");
        UT_ASSERT( ary->Count() == 0 );
    }
    {
        XString doc =  "{ \"data\": { \"motion_events\": [], \"motion_events2\": [] } }";
        XIRef<XJSONItem> item = NULL;
        UT_ASSERT_NO_THROW( item = XJSONItem::ParseDocument(doc) );
        UT_ASSERT(item.IsValid());
        XIRef<XJSONObject> obj = item;
        XIRef<XJSONObject> obj2 = obj->GetObjectMember("data");
        XIRef<XJSONArray> ary = obj2->GetObjectMember("motion_events");
        UT_ASSERT( ary->Count() == 0 );
        ary = obj2->GetObjectMember("motion_events2");
        UT_ASSERT( ary->Count() == 0 );
    }

    {
        XString doc =  "{ \"data\": [ {} ]}";
        XIRef<XJSONItem> item = NULL;
        UT_ASSERT_NO_THROW( item = XJSONItem::ParseDocument(doc) );
        UT_ASSERT(item.IsValid());
        XIRef<XJSONObject> obj = item;
        XIRef<XJSONArray> ary = obj->GetObjectMember("data");
        UT_ASSERT( ary->Count() == 1 );
        obj = ary->Index(0);
        UT_ASSERT(obj.IsValid());
        UT_ASSERT(obj->GetObjectContents().GetElemCount() == 0 );
    }
}

void XJSONTest::TestFind()
{
    {
        XString doc = "{\"menu\": {"
            "   \"header\": \"SVG Viewer\","
            "   \"items\": ["
            "       {\"id\": \"Open\"},"
            "       {\"id\": \"OpenNew\", \"label\": \"Open New\"},"
            "       null,"
            "       {\"id\": \"ZoomIn\", \"label\": \"Zoom In\"},"
            "       {\"id\": \"ZoomOut\", \"label\": \"Zoom Out\"},"
            "       {\"id\": \"OriginalView\", \"label\": \"Original View\"},"
            "       true,"
            "       {\"id\": \"Quality\"},"
            "       {\"id\": \"Pause\"},"
            "       {\"id\": \"Mute\"},"
            "       false,"
            "       {\"id\": \"Find\", \"label\": \"Find...\"},"
            "       {\"id\": \"FindAgain\", \"label\": \"Find Again\"},"
            "       {\"id\": \"Copy\"},"
            "       {\"id\": \"CopyAgain\", \"label\": \"Copy Again\"},"
            "       {\"id\": \"CopySVG\", \"label\": \"Copy SVG\"},"
            "       {\"id\": \"ViewSVG\", \"label\": \"View SVG\"},"
            "       {\"id\": \"ViewSource\", \"label\": \"View Source\"},"
            "       {\"id\": \"SaveAs\", \"label\": \"Save As\"},"
            "       null,"
            "       {\"id\": \"Help\"},"
            "       {\"id\": \"About\", \"label\": \"About Adobe CVG Viewer...\"}"
            "   ]"
            "}}";
        XIRef<XJSONItem> item = NULL;
        UT_ASSERT_NO_THROW( item = XJSONItem::ParseDocument(doc) );
        UT_ASSERT(item.IsValid());
        XIRef<XJSONValue> value;
        UT_ASSERT_NO_THROW( value = XJSONItem::Find(item,"menu/items/3/id"));
        XString temp = value->GetValue().Get<XSDK::XString>();
        UT_ASSERT( temp == "ZoomIn" );
    }
    {
        XString doc = "{\"menu\": {"
            "   \"header\": \"SVG Viewer\","
            "   \"items\": ["
            "       {\"id\": \"Open\"},"
            "       {\"id\": \"OpenNew\", \"label\": \"Open New\"},"
            "       null,"
            "       {\"id\": \"ZoomIn\", \"label\": \"Zoom In\"},"
            "       {\"id\": \"ZoomOut\", \"label\": \"Zoom Out\"},"
            "       {\"id\": \"OriginalView\", \"label\": \"Original View\"},"
            "       true,"
            "       {\"id\": \"Quality\"},"
            "       {\"id\": \"Pause\"},"
            "       {\"id\": \"Mute\"},"
            "       false,"
            "       {\"id\": \"Find\", \"label\": \"Find...\"},"
            "       {\"id\": \"FindAgain\", \"label\": \"Find Again\"},"
            "       {\"id\": \"Copy\"},"
            "       {\"id\": \"CopyAgain\", \"label\": \"Copy Again\"},"
            "       {\"id\": \"CopySVG\", \"label\": \"Copy SVG\"},"
            "       {\"id\": \"ViewSVG\", \"label\": \"View SVG\"},"
            "       {\"id\": \"ViewSource\", \"label\": \"View Source\"},"
            "       {\"id\": \"SaveAs\", \"label\": \"Save As\"},"
            "       null,"
            "       {\"id\": \"Help\"},"
            "       {\"id\": \"About\", \"label\": \"About Adobe CVG Viewer...\"}"
            "   ]"
            "}}";
        XIRef<XJSONItem> item = NULL;
        UT_ASSERT_NO_THROW( item = XJSONItem::ParseDocument(doc) );
        UT_ASSERT(item.IsValid());
        XIRef<XJSONValue> value;
        UT_ASSERT_THROWS( value = XJSONItem::Find(item,"menu/items/Tim/id"), XJSONException );
    }
    {
        XString doc = "["
                  "  ["
                  "     ["
                  "      23"
                  "     ],"
                  "     3,"
                  "     3,"
                  "     7"
                  "  ],"
                  "  43,"
                  "  44"
                  "]";;
        XIRef<XJSONItem> item = NULL;
        UT_ASSERT_NO_THROW( item = XJSONItem::ParseDocument(doc) );
        UT_ASSERT(item.IsValid());
        XIRef<XJSONValue> value;
        UT_ASSERT_NO_THROW( value = XJSONItem::Find(item,"0/0/0"));
        int x = value->GetValue();
        UT_ASSERT( x == 23 );
    }
    {
        XString doc = "["
                  "  ["
                  "     ["
                  "      23"
                  "     ],"
                  "     3,"
                  "     3,"
                  "     7"
                  "  ],"
                  "  43,"
                  "  44"
                  "]";;
        XIRef<XJSONItem> item = NULL;
        UT_ASSERT_NO_THROW( item = XJSONItem::ParseDocument(doc) );
        UT_ASSERT(item.IsValid());
        XIRef<XJSONValue> value;
        UT_ASSERT_NO_THROW( value = XJSONItem::Find(item,"0/0/2"));
        UT_ASSERT( value.IsEmpty() );
    }
    {
        XString doc = "{\"menu\": {"
            "   \"header\": \"SVG Viewer\","
            "   \"items\": ["
            "       {\"id\": \"Open\"},"
            "       {\"id\": \"OpenNew\", \"label\": \"Open New\"},"
            "       null,"
            "       {\"id\": \"ZoomIn\", \"label\": \"Zoom In\"},"
            "       {\"id\": \"ZoomOut\", \"label\": \"Zoom Out\"},"
            "       {\"id\": \"OriginalView\", \"label\": \"Original View\"},"
            "       true,"
            "       {\"id\": \"Quality\"},"
            "       {\"id\": \"Pause\"},"
            "       {\"id\": \"Mute\"},"
            "       false,"
            "       {\"id\": \"Find\", \"label\": \"Find...\"},"
            "       {\"id\": \"FindAgain\", \"label\": \"Find Again\"},"
            "       {\"id\": \"Copy\"},"
            "       {\"id\": \"CopyAgain\", \"label\": \"Copy Again\"},"
            "       {\"id\": \"CopySVG\", \"label\": \"Copy SVG\"},"
            "       {\"id\": \"ViewSVG\", \"label\": \"View SVG\"},"
            "       {\"id\": \"ViewSource\", \"label\": \"View Source\"},"
            "       {\"id\": \"SaveAs\", \"label\": \"Save As\"},"
            "       null,"
            "       {\"id\": \"Help\"},"
            "       {\"id\": \"About\", \"label\": \"About Adobe CVG Viewer...\"}"
            "   ]"
            "}}";
        XIRef<XJSONItem> item = NULL;
        UT_ASSERT_NO_THROW( item = XJSONItem::ParseDocument(doc) );
        UT_ASSERT(item.IsValid());
        XIRef<XJSONValue> value;
        UT_ASSERT_NO_THROW( value = XJSONItem::Find(item,"menu/items/1/Naboo") );
        UT_ASSERT( value.IsEmpty() );
    }

}
void XJSONTest::TestForQuotedContent()
{
    XString doc = "{\"foo\":\"%s\"}";
    doc = XString::Format(doc.c_str(),"\\\"temp\\\"");
    XIRef<XJSONItem> item = NULL;
    UT_ASSERT_NO_THROW( item = XJSONItem::ParseDocument(doc) );
    UT_ASSERT(item.IsValid());
    UT_ASSERT(item->GetType() == XJSONObjectType);
    XIRef<XJSONObject> obj(item);
    XHash< XIRef<XJSONItem> > contents = obj->GetObjectContents();
    UT_ASSERT( contents.GetElemCount() == 1 );
    XString foo = obj->GetObjectMember("foo")->Get<XString>();
    XString bar = "\\\"temp\\\"";
    UT_ASSERT( foo == bar );
}

void XJSONTest::TestEmptyObjInArray()
{
    XString doc = "{\"foo\":[]}";
    XIRef<XJSONItem> item = NULL;
    UT_ASSERT_NO_THROW( item = XJSONItem::ParseDocument(doc) );
    UT_ASSERT(item.IsValid());
    UT_ASSERT(item->GetType() == XJSONObjectType);
    XIRef<XJSONObject> obj(item);
    XHash< XIRef<XJSONItem> > contents = obj->GetObjectContents();
    UT_ASSERT( contents.GetElemCount() == 1 );
    XString foo = "foo";
    XIRef<XJSONArray> ary = obj->GetObjectMember(foo);
    UT_ASSERT( ary->GetArray().size() == 0 );
}
void XJSONTest::TestEmptyObjInObj()
{
    XString doc = "{\"foo\":{}}";
    XIRef<XJSONItem> item = NULL;
    UT_ASSERT_NO_THROW( item = XJSONItem::ParseDocument(doc) );
    UT_ASSERT(item.IsValid());
    UT_ASSERT(item->GetType() == XJSONObjectType);
    XIRef<XJSONObject> obj(item);
    XHash< XIRef<XJSONItem> > contents = obj->GetObjectContents();
    UT_ASSERT( contents.GetElemCount() == 1 );
    XString foo = "foo";
    XIRef<XJSONObject> obj2 = obj->GetObjectMember(foo);
    UT_ASSERT( obj2->GetObjectContents().GetElemCount() == 0 );
}

void XJSONTest::TestEmptyArrayInObj()
{
    XString doc = "[{}]";
    XIRef<XJSONItem> item = NULL;
    UT_ASSERT_NO_THROW( item = XJSONItem::ParseDocument(doc) );
    UT_ASSERT(item.IsValid());
    XIRef<XJSONArray> ary(item);
    UT_ASSERT( ary->GetArray().size() == 1 );
    XIRef<XJSONObject> obj(ary->AccessElement(0));
    XHash< XIRef<XJSONItem> > contents = obj->GetObjectContents();
    UT_ASSERT( contents.GetElemCount() == 0 );
}
void XJSONTest::TestEmptyArrayInArray()
{
    XString doc = "[[]]";
    XIRef<XJSONItem> item = NULL;
    UT_ASSERT_NO_THROW( item = XJSONItem::ParseDocument(doc) );
    UT_ASSERT(item.IsValid());
    XIRef<XJSONArray> ary(item);
    UT_ASSERT( ary->GetArray().size() == 1 );
    XIRef<XJSONArray> ary2(ary->AccessElement(0));
    UT_ASSERT( ary2->GetArray().size() == 0 );
}

void XJSONTest::TestEmptyObj()
{
    XString doc = "{}";
    XIRef<XJSONItem> item = NULL;
    UT_ASSERT_NO_THROW( item = XJSONItem::ParseDocument(doc) );
    UT_ASSERT(item.IsValid());
    UT_ASSERT(item->GetType() == XJSONObjectType);
    XIRef<XJSONObject> obj(item);
    XHash< XIRef<XJSONItem> > contents = obj->GetObjectContents();
    UT_ASSERT( contents.GetElemCount() == 0 );
}
void XJSONTest::TestEmptyArray()
{
    XString doc = "[]";
    XIRef<XJSONItem> item = NULL;
    UT_ASSERT_NO_THROW( item = XJSONItem::ParseDocument(doc) );
    UT_ASSERT(item.IsValid());
    UT_ASSERT(item->GetType() == XJSONArrayType);
    XIRef<XJSONArray> ary(item);
    std::vector< XIRef<XJSONItem> > contents = ary->GetArray();
    UT_ASSERT( contents.size() == 0 );
}


void XJSONTest::TestSimpleArray()
{
    XString doc = "[42,43,44]";
    XIRef<XJSONItem> item = XJSONItem::ParseDocument(doc);
    UT_ASSERT(item.IsValid());
    UT_ASSERT(item->GetType() == XJSONArrayType);
    std::vector< XIRef<XJSONItem> > contents = ((XJSONArray*)item.Raw())->GetArray();
    UT_ASSERT( contents.size() == 3 );
    for( size_t i = 0; i < contents.size(); ++i )
    {
        double d = i + 42;
        UT_ASSERT( contents[i]->GetType() == XJSONValueType );
        UT_ASSERT( ((XJSONValue*)contents[i].Raw())->GetValue().GetType() == XVARTYPE_DOUBLE );
        UT_ASSERT( (double)(((XJSONValue*)contents[i].Raw())->GetValue()) == d );
    }
}

void XJSONTest::TestSimpleObject()
{
    XString doc = "{\"foo\":42,\"bar\":44}";
    XIRef<XJSONItem> item = XJSONItem::ParseDocument(doc);
    UT_ASSERT(item.IsValid());
    UT_ASSERT(item->GetType() == XJSONObjectType);
    XHash< XIRef<XJSONItem> > contents = ((XJSONObject*)item.Raw())->GetObjectContents();
    UT_ASSERT( contents.GetElemCount() == 2 );

    XIRef<XJSONValue> value = *contents.Find("foo");
    UT_ASSERT( value->GetValue().GetType() == XVARTYPE_DOUBLE );
    UT_ASSERT( ((double)(value->GetValue())) == 42 );
    value = *contents.Find("bar");
    UT_ASSERT( value->GetValue().GetType() == XVARTYPE_DOUBLE );
    UT_ASSERT( ((double)(value->GetValue())) == 44 );
}

void XJSONTest::TestArrayContainArray()
{
    XString doc = "["
                  "  ["
                  "     ["
                  "      23"
                  "     ],"
                  "     3,"
                  "     3,"
                  "     7"
                  "  ],"
                  "  43,"
                  "  44"
                  "]";
    XIRef<XJSONItem> item = XJSONItem::ParseDocument(doc);
    UT_ASSERT(item.IsValid());
    UT_ASSERT(item->GetType() == XJSONArrayType);
    std::vector< XIRef<XJSONItem> > contents = item->GetInterface<XJSONArray>()->GetArray();
    UT_ASSERT( contents.size() == 3 );

    XIRef<XJSONItem> elem1 = contents[0];
    UT_ASSERT( elem1->GetType() == XJSONArrayType );
    std::vector< XIRef<XJSONItem> > subContents = elem1->GetInterface<XJSONArray>()->GetArray();
    UT_ASSERT( subContents.size() == 4 );
    XIRef<XJSONItem> subElem0 = subContents[0];
    XIRef<XJSONItem> subElem1 = subContents[1];
    XIRef<XJSONItem> subElem2 = subContents[2];
    XIRef<XJSONItem> subElem3 = subContents[3];

    UT_ASSERT( subElem0->GetType() == XJSONArrayType );
    std::vector< XIRef<XJSONItem> > subSubContents = subElem0->GetInterface<XJSONArray>()->GetArray();
    UT_ASSERT( subSubContents.size() == 1 );
    XIRef<XJSONItem> subSubElem0 = subSubContents[0];
    UT_ASSERT( subSubElem0->GetType() == XJSONValueType );
    UT_ASSERT( (subSubElem0->GetInterface<XJSONValue>())->GetValue().GetType() == XVARTYPE_DOUBLE );
    UT_ASSERT( (double)(subSubElem0->GetInterface<XJSONValue>())->GetValue() == 23 );

    UT_ASSERT( subElem1->GetType() == XJSONValueType );
    UT_ASSERT( (subElem1->GetInterface<XJSONValue>())->GetValue().GetType() == XVARTYPE_DOUBLE );
    UT_ASSERT( (double)(subElem1->GetInterface<XJSONValue>())->GetValue() == 3 );
    UT_ASSERT( subElem2->GetType() == XJSONValueType );
    UT_ASSERT( (subElem2->GetInterface<XJSONValue>())->GetValue().GetType() == XVARTYPE_DOUBLE );
    UT_ASSERT( (double)(subElem2->GetInterface<XJSONValue>())->GetValue() == 3 );
    UT_ASSERT( subElem3->GetType() == XJSONValueType );
    UT_ASSERT( (subElem3->GetInterface<XJSONValue>())->GetValue().GetType() == XVARTYPE_DOUBLE );
    UT_ASSERT( (double)(subElem3->GetInterface<XJSONValue>())->GetValue() == 7 );

    XIRef<XJSONItem> elem2 = contents[1];
    UT_ASSERT( elem2->GetType() == XJSONValueType );
    UT_ASSERT( (elem2->GetInterface<XJSONValue>())->GetValue().GetType() == XVARTYPE_DOUBLE );
    UT_ASSERT( (double)(elem2->GetInterface<XJSONValue>())->GetValue() == 43 );

    XIRef<XJSONItem> elem3 = contents[2];
    UT_ASSERT( elem3->GetType() == XJSONValueType );
    UT_ASSERT( (elem3->GetInterface<XJSONValue>())->GetValue().GetType() == XVARTYPE_DOUBLE );
    UT_ASSERT( (double)(elem3->GetInterface<XJSONValue>())->GetValue() == 44 );
}
void XJSONTest::TestShortHand()
{

    XString doc = "{\"menu\": {"
       "   \"header\": \"SVG Viewer\","
       "   \"items\": ["
       "       {\"id\": \"Open\"},"
       "       {\"id\": \"OpenNew\", \"label\": \"Open New\"},"
       "       null,"
       "       {\"id\": \"ZoomIn\", \"label\": \"Zoom In\"},"
       "       {\"id\": \"ZoomOut\", \"label\": \"Zoom Out\"},"
       "       {\"id\": \"OriginalView\", \"label\": \"Original View\"},"
       "       true,"
       "       {\"id\": \"Quality\"},"
       "       {\"id\": \"Pause\"},"
       "       {\"id\": \"Mute\"},"
       "       false,"
       "       {\"id\": \"Find\", \"label\": \"Find...\"},"
       "       {\"id\": \"FindAgain\", \"label\": \"Find Again\"},"
       "       {\"id\": \"Copy\"},"
       "       {\"id\": \"CopyAgain\", \"label\": \"Copy Again\"},"
       "       {\"id\": \"CopySVG\", \"label\": \"Copy SVG\"},"
       "       {\"id\": \"ViewSVG\", \"label\": \"View SVG\"},"
       "       {\"id\": \"ViewSource\", \"label\": \"View Source\"},"
       "       {\"id\": \"SaveAs\", \"label\": \"Save As\"},"
       "       null,"
       "       {\"id\": \"Help\"},"
       "       {\"id\": \"About\", \"label\": \"About Adobe CVG Viewer...\"}"
       "   ]"
       "}}";
    XIRef<XJSONItem> item = NULL;
    UT_ASSERT_NO_THROW( item = XJSONItem::ParseDocument(doc) );
    UT_ASSERT(item.IsValid());

    UT_ASSERT( item->Index("menu")->Index("header")->Get<XString>() == "SVG Viewer" );
    UT_ASSERT( item->Index("menu")->Index("items")->Index(1)->Index("label")->Get<XString>() == "Open New" );
    UT_ASSERT_THROWS( item->Index("menu")->Index(0),XJSONException );



}
void XJSONTest::TestArrayContainObject()
{
    XString doc = "["
                  "  ["
                  "     {"
                  "      \"foo\":23"
                  "     },"
                  "     3,"
                  "     3,"
                  "     7"
                  "  ],"
                  "  43,"
                  "  44"
                  "]";
    XIRef<XJSONItem> item = XJSONItem::ParseDocument(doc);
    UT_ASSERT(item.IsValid());
    UT_ASSERT(item->GetType() == XJSONArrayType);
    std::vector< XIRef<XJSONItem> > contents = (item->GetInterface<XJSONArray>())->GetArray();
    UT_ASSERT( contents.size() == 3 );

    XIRef<XJSONItem> elem1 = contents[0];
    UT_ASSERT( elem1->GetType() == XJSONArrayType );
    std::vector< XIRef<XJSONItem> > subContents = (elem1->GetInterface<XJSONArray>())->GetArray();
    UT_ASSERT( subContents.size() == 4 );
    XIRef<XJSONItem> subElem0 = subContents[0];
    XIRef<XJSONItem> subElem1 = subContents[1];
    XIRef<XJSONItem> subElem2 = subContents[2];
    XIRef<XJSONItem> subElem3 = subContents[3];

    UT_ASSERT( subElem0->GetType() == XJSONObjectType );
    XHash< XIRef<XJSONItem> > subSubContents = (subElem0->GetInterface<XJSONObject>())->GetObjectContents();
    UT_ASSERT( subSubContents.GetElemCount() == 1 );
    XIRef<XJSONItem> subSubElem0 = *(subSubContents.Find("foo"));
    UT_ASSERT( subSubElem0->GetType() == XJSONValueType );
    UT_ASSERT( (subSubElem0->GetInterface<XJSONValue>())->GetValue().GetType() == XVARTYPE_DOUBLE );
    UT_ASSERT( (double)(subSubElem0->GetInterface<XJSONValue>())->GetValue() == 23 );

    UT_ASSERT( subElem1->GetType() == XJSONValueType );
    UT_ASSERT( (subElem1->GetInterface<XJSONValue>())->GetValue().GetType() == XVARTYPE_DOUBLE );
    UT_ASSERT( (double)(subElem1->GetInterface<XJSONValue>())->GetValue() == 3 );
    UT_ASSERT( subElem2->GetType() == XJSONValueType );
    UT_ASSERT( (subElem2->GetInterface<XJSONValue>())->GetValue().GetType() == XVARTYPE_DOUBLE );
    UT_ASSERT( (double)(subElem2->GetInterface<XJSONValue>())->GetValue() == 3 );
    UT_ASSERT( subElem3->GetType() == XJSONValueType );
    UT_ASSERT( (subElem3->GetInterface<XJSONValue>())->GetValue().GetType() == XVARTYPE_DOUBLE );
    UT_ASSERT( (double)(subElem3->GetInterface<XJSONValue>())->GetValue() == 7 );

    XIRef<XJSONItem> elem2 = contents[1];
    UT_ASSERT( elem2->GetType() == XJSONValueType );
    UT_ASSERT( (elem2->GetInterface<XJSONValue>())->GetValue().GetType() == XVARTYPE_DOUBLE );
    UT_ASSERT( (double)(elem2->GetInterface<XJSONValue>())->GetValue() == 43 );

    XIRef<XJSONItem> elem3 = contents[2];
    UT_ASSERT( elem3->GetType() == XJSONValueType );
    UT_ASSERT( (elem3->GetInterface<XJSONValue>())->GetValue().GetType() == XVARTYPE_DOUBLE );
    UT_ASSERT( (double)(elem3->GetInterface<XJSONValue>())->GetValue() == 44 );
}

void XJSONTest::TestObjectContainObject()
{
    XString doc = "{"
                  "  \"A\":{"
                  "        \"tim\":{"
                  "                  \"foo\":23"
                  "                },"
                  "        \"jack\":3,"
                  "        \"ryan\":3,"
                  "        \"dingo\":7"
                  "       },"
                  "  \"B\":43,"
                  "  \"C\":44"
                  "}";
    XIRef<XJSONItem> item = XJSONItem::ParseDocument(doc);
    UT_ASSERT(item.IsValid());
    UT_ASSERT(item->GetType() == XJSONObjectType);
    XHash< XIRef<XJSONItem> > contents = (item->GetInterface<XJSONObject>())->GetObjectContents();
    UT_ASSERT( contents.GetElemCount() == 3 );

    XIRef<XJSONItem> elem1 = *(contents.Find("A"));
    UT_ASSERT( elem1->GetType() == XJSONObjectType );
    XHash< XIRef<XJSONItem> > subContents = (elem1->GetInterface<XJSONObject>())->GetObjectContents();
    UT_ASSERT( subContents.GetElemCount() == 4 );
    XIRef<XJSONItem> subElem0 = *(subContents.Find("tim"));
    XIRef<XJSONItem> subElem1 = *(subContents.Find("jack"));
    XIRef<XJSONItem> subElem2 = *(subContents.Find("ryan"));
    XIRef<XJSONItem> subElem3 = *(subContents.Find("dingo"));

    UT_ASSERT( subElem0->GetType() == XJSONObjectType );
    XHash< XIRef<XJSONItem> > subSubContents = (subElem0->GetInterface<XJSONObject>())->GetObjectContents();
    UT_ASSERT( subSubContents.GetElemCount() == 1 );
    XIRef<XJSONItem> subSubElem0 = *(subSubContents.Find("foo"));
    UT_ASSERT( subSubElem0->GetType() == XJSONValueType );
    UT_ASSERT( (subSubElem0->GetInterface<XJSONValue>())->GetValue().GetType() == XVARTYPE_DOUBLE );
    UT_ASSERT( (double)(subSubElem0->GetInterface<XJSONValue>())->GetValue() == 23 );

    UT_ASSERT( subElem1->GetType() == XJSONValueType );
    UT_ASSERT( (subElem1->GetInterface<XJSONValue>())->GetValue().GetType() == XVARTYPE_DOUBLE );
    UT_ASSERT( (double)(subElem1->GetInterface<XJSONValue>())->GetValue() == 3 );
    UT_ASSERT( subElem2->GetType() == XJSONValueType );
    UT_ASSERT( (subElem2->GetInterface<XJSONValue>())->GetValue().GetType() == XVARTYPE_DOUBLE );
    UT_ASSERT( (double)(subElem2->GetInterface<XJSONValue>())->GetValue() == 3 );
    UT_ASSERT( subElem3->GetType() == XJSONValueType );
    UT_ASSERT( (subElem3->GetInterface<XJSONValue>())->GetValue().GetType() == XVARTYPE_DOUBLE );
    UT_ASSERT( (double)(subElem3->GetInterface<XJSONValue>())->GetValue() == 7 );

    XIRef<XJSONItem> elem2 = *(contents.Find("B"));
    UT_ASSERT( elem2->GetType() == XJSONValueType );
    UT_ASSERT( (elem2->GetInterface<XJSONValue>())->GetValue().GetType() == XVARTYPE_DOUBLE );
    UT_ASSERT( (double)(elem2->GetInterface<XJSONValue>())->GetValue() == 43 );

    XIRef<XJSONItem> elem3 = *(contents.Find("C"));
    UT_ASSERT( elem3->GetType() == XJSONValueType );
    UT_ASSERT( (elem3->GetInterface<XJSONValue>())->GetValue().GetType() == XVARTYPE_DOUBLE );
    UT_ASSERT( (double)(elem3->GetInterface<XJSONValue>())->GetValue() == 44 );
}

void XJSONTest::TestObjectContainArray()
{
    XString doc = "{"
                  "  \"A\":{"
                  "        \"tim\":["
                  "                  23"
                  "                ],"
                  "        \"jack\":3,"
                  "        \"ryan\":3,"
                  "        \"dingo\":7"
                  "       },"
                  "  \"B\":43,"
                  "  \"C\":44"
                  "}";
    XIRef<XJSONItem> item = XJSONItem::ParseDocument(doc);
    UT_ASSERT(item.IsValid());
    UT_ASSERT(item->GetType() == XJSONObjectType);
    XHash< XIRef<XJSONItem> > contents = (item->GetInterface<XJSONObject>())->GetObjectContents();
    UT_ASSERT( contents.GetElemCount() == 3 );

    XIRef<XJSONItem> elem1 = *(contents.Find("A"));
    UT_ASSERT( elem1->GetType() == XJSONObjectType );
    XHash< XIRef<XJSONItem> > subContents = (elem1->GetInterface<XJSONObject>())->GetObjectContents();
    UT_ASSERT( subContents.GetElemCount() == 4 );
    XIRef<XJSONItem> subElem0 = *(subContents.Find("tim"));
    XIRef<XJSONItem> subElem1 = *(subContents.Find("jack"));
    XIRef<XJSONItem> subElem2 = *(subContents.Find("ryan"));
    XIRef<XJSONItem> subElem3 = *(subContents.Find("dingo"));

    UT_ASSERT( subElem0->GetType() == XJSONArrayType );
    std::vector< XIRef<XJSONItem> > subSubContents = (subElem0->GetInterface<XJSONArray>())->GetArray();
    UT_ASSERT( subSubContents.size() == 1 );
    XIRef<XJSONItem> subSubElem0 = subSubContents[0];
    UT_ASSERT( subSubElem0->GetType() == XJSONValueType );
    UT_ASSERT( (subSubElem0->GetInterface<XJSONValue>())->GetValue().GetType() == XVARTYPE_DOUBLE );
    UT_ASSERT( (double)(subSubElem0->GetInterface<XJSONValue>())->GetValue() == 23 );

    UT_ASSERT( subElem1->GetType() == XJSONValueType );
    UT_ASSERT( (subElem1->GetInterface<XJSONValue>())->GetValue().GetType() == XVARTYPE_DOUBLE );
    UT_ASSERT( (double)(subElem1->GetInterface<XJSONValue>())->GetValue() == 3 );
    UT_ASSERT( subElem2->GetType() == XJSONValueType );
    UT_ASSERT( (subElem2->GetInterface<XJSONValue>())->GetValue().GetType() == XVARTYPE_DOUBLE );
    UT_ASSERT( (double)(subElem2->GetInterface<XJSONValue>())->GetValue() == 3 );
    UT_ASSERT( subElem3->GetType() == XJSONValueType );
    UT_ASSERT( (subElem3->GetInterface<XJSONValue>())->GetValue().GetType() == XVARTYPE_DOUBLE );
    UT_ASSERT( (double)(subElem3->GetInterface<XJSONValue>())->GetValue() == 7 );

    XIRef<XJSONItem> elem2 = *(contents.Find("B"));
    UT_ASSERT( elem2->GetType() == XJSONValueType );
    UT_ASSERT( (elem2->GetInterface<XJSONValue>())->GetValue().GetType() == XVARTYPE_DOUBLE );
    UT_ASSERT( (double)(elem2->GetInterface<XJSONValue>())->GetValue() == 43 );

    XIRef<XJSONItem> elem3 = *(contents.Find("C"));
    UT_ASSERT( elem3->GetType() == XJSONValueType );
    UT_ASSERT( (elem3->GetInterface<XJSONValue>())->GetValue().GetType() == XVARTYPE_DOUBLE );
    UT_ASSERT( (double)(elem3->GetInterface<XJSONValue>())->GetValue() == 44 );
}




void XJSONTest::TestJsonOrgExample()
{
    XString doc = "{\"menu\": {"
    "   \"header\": \"SVG Viewer\","
    "   \"items\": ["
    "       {\"id\": \"Open\"},"
    "       {\"id\": \"OpenNew\", \"label\": \"Open New\"},"
    "       null,"
    "       {\"id\": \"ZoomIn\", \"label\": \"Zoom In\"},"
    "       {\"id\": \"ZoomOut\", \"label\": \"Zoom Out\"},"
    "       {\"id\": \"OriginalView\", \"label\": \"Original View\"},"
    "       true,"
    "       {\"id\": \"Quality\"},"
    "       {\"id\": \"Pause\"},"
    "       {\"id\": \"Mute\"},"
    "       false,"
    "       {\"id\": \"Find\", \"label\": \"Find...\"},"
    "       {\"id\": \"FindAgain\", \"label\": \"Find Again\"},"
    "       {\"id\": \"Copy\"},"
    "       {\"id\": \"CopyAgain\", \"label\": \"Copy Again\"},"
    "       {\"id\": \"CopySVG\", \"label\": \"Copy SVG\"},"
    "       {\"id\": \"ViewSVG\", \"label\": \"View SVG\"},"
    "       {\"id\": \"ViewSource\", \"label\": \"View Source\"},"
    "       {\"id\": \"SaveAs\", \"label\": \"Save As\"},"
    "       null,"
    "       {\"id\": \"Help\"},"
    "       {\"id\": \"About\", \"label\": \"About Adobe CVG Viewer...\"}"
    "   ]"
    "}}";
    XString str;

    XIRef<XJSONItem> item = XJSONItem::ParseDocument(doc);
    UT_ASSERT(item.IsValid());
    UT_ASSERT(item->GetType() == XJSONObjectType);

    XHash< XIRef<XJSONItem> > contents = (item->GetInterface<XJSONObject>())->GetObjectContents();
    UT_ASSERT( contents.GetElemCount() == 1 );

    XIRef<XJSONItem> menu = *(contents.Find("menu"));
    UT_ASSERT( menu->GetType() == XJSONObjectType );
    XHash< XIRef<XJSONItem> > subContents = (menu->GetInterface<XJSONObject>())->GetObjectContents();
    UT_ASSERT( subContents.GetElemCount() == 2 );

    XIRef<XJSONItem> header = *(subContents.Find("header"));
    UT_ASSERT( header->GetType() == XJSONValueType );
    UT_ASSERT( (header->GetInterface<XJSONValue>())->GetValue().GetType() == XVARTYPE_TEXT );

    str = (header->GetInterface<XJSONValue>())->GetValue().Get<XString>();

    UT_ASSERT( str == "SVG Viewer" );

    XIRef<XJSONItem> items = *(subContents.Find("items"));
    UT_ASSERT( items->GetType() == XJSONArrayType );
    std::vector< XIRef<XJSONItem> > subSubContents = (items->GetInterface<XJSONArray>())->GetArray();
    UT_ASSERT( subSubContents.size() == 22 );
    XIRef<XJSONItem> subSubElem0 = subSubContents[0];
    XIRef<XJSONItem> subSubElem1 = subSubContents[1];
    XIRef<XJSONItem> subSubElem2 = subSubContents[2];
    XIRef<XJSONItem> subSubElem6 = subSubContents[6];
    XIRef<XJSONItem> subSubElem10 = subSubContents[10];
    UT_ASSERT( subSubElem0->GetType() == XJSONObjectType );
    UT_ASSERT( subSubElem1->GetType() == XJSONObjectType );
    UT_ASSERT( subSubElem2->GetType() == XJSONValueType );
    UT_ASSERT( subSubElem6->GetType() == XJSONValueType );
    UT_ASSERT( subSubElem10->GetType() == XJSONValueType );
    UT_ASSERT( subSubElem2->GetInterface<XJSONValue>()->GetValue().GetType() == XVARTYPE_EMPTY );

    UT_ASSERT( subSubElem6->GetInterface<XJSONValue>()->GetValue().GetType() == XVARTYPE_BOOL );
    UT_ASSERT( subSubElem6->GetInterface<XJSONValue>()->GetValue().Get<bool>() );

    UT_ASSERT( subSubElem10->GetInterface<XJSONValue>()->GetValue().GetType() == XVARTYPE_BOOL );
    UT_ASSERT( !(subSubElem10->GetInterface<XJSONValue>()->GetValue().Get<bool>()) );

    UT_ASSERT( (subSubElem0->GetInterface<XJSONObject>())->GetObjectContents().GetElemCount() == 1 );
    UT_ASSERT( (*((subSubElem0->GetInterface<XJSONObject>())->GetObjectContents().Find("id")))->GetType() == XJSONValueType );

    UT_ASSERT( (*((subSubElem0->GetInterface<XJSONObject>())->GetObjectContents().Find("id")))->GetInterface<XJSONValue>()->GetValue().Get<XString>() == "Open"  );

    UT_ASSERT( subSubElem1->GetInterface<XJSONObject>()->GetObjectContents().GetElemCount() == 2 );

    str = (*(subSubElem1->GetInterface<XJSONObject>()->GetObjectContents().Find("id")))->GetInterface<XJSONValue>()->GetValue().Get<XString>();
    UT_ASSERT( str == "OpenNew"  );
    UT_ASSERT( (*(subSubElem1->GetInterface<XJSONObject>()->GetObjectContents().Find("id")))->GetType() == XJSONValueType );

    UT_ASSERT( (*(subSubElem1->GetInterface<XJSONObject>()->GetObjectContents().Find("label")))->GetType() == XJSONValueType );
    str = (*(subSubElem1->GetInterface<XJSONObject>()->GetObjectContents().Find("label")))->GetInterface<XJSONValue>()->GetValue().Get<XString>();
    UT_ASSERT( str == "Open New"  );
}































