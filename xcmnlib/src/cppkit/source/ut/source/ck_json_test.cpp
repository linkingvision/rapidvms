
#include "ck_json_test.h"

#include "cppkit/ck_string.h"
#include "cppkit/ck_json.h"

using namespace cppkit;
using namespace std;

REGISTER_TEST_FIXTURE(ck_json_test)

void ck_json_test::TestT247_2()
{
    {
        ck_string doc =  "{ \"data\": { \"motion_events\": [ ] } }";
        shared_ptr<ck_json_item> item = NULL;
        UT_ASSERT_NO_THROW( item = ck_json_item::parse_document(doc) );
        UT_ASSERT(item.get() != NULL);
        shared_ptr<ck_json_object> obj = static_pointer_cast<ck_json_object>(item);
        shared_ptr<ck_json_object> obj2 = static_pointer_cast<ck_json_object>(obj->get_object_member("data"));
        shared_ptr<ck_json_array> ary = static_pointer_cast<ck_json_array>(obj2->get_object_member("motion_events"));
        UT_ASSERT( ary->size() == 0 );
    }
    {
        ck_string doc =  "{ \"data\": { \"motion_events\": [] } }";
        shared_ptr<ck_json_item> item = NULL;
        UT_ASSERT_NO_THROW( item = ck_json_item::parse_document(doc) );
        UT_ASSERT(item.get() != NULL);
        shared_ptr<ck_json_object> obj = static_pointer_cast<ck_json_object>(item);
        shared_ptr<ck_json_object> obj2 = static_pointer_cast<ck_json_object>(obj->get_object_member("data"));
        shared_ptr<ck_json_array> ary = static_pointer_cast<ck_json_array>(obj2->get_object_member("motion_events"));
        UT_ASSERT( ary->size() == 0 );
    }
    {
        ck_string doc =  "{ \"data\": { \"motion_events\": [ ], \"motion_events2\": [ ] } }";
        shared_ptr<ck_json_item> item = NULL;
        UT_ASSERT_NO_THROW( item = ck_json_item::parse_document(doc) );
        UT_ASSERT(item.get() != NULL);
        shared_ptr<ck_json_object> obj = static_pointer_cast<ck_json_object>(item);
        shared_ptr<ck_json_object> obj2 = static_pointer_cast<ck_json_object>(obj->get_object_member("data"));
        shared_ptr<ck_json_array> ary = static_pointer_cast<ck_json_array>(obj2->get_object_member("motion_events"));
        UT_ASSERT( ary->size() == 0 );
        ary = static_pointer_cast<ck_json_array>(obj2->get_object_member("motion_events2"));
        UT_ASSERT( ary->size() == 0 );
    }
    {
        ck_string doc =  "{ \"data\": { \"motion_events\": [], \"motion_events2\": [ ] } }";
        shared_ptr<ck_json_item> item = NULL;
        UT_ASSERT_NO_THROW( item = ck_json_item::parse_document(doc) );
        UT_ASSERT(item.get() != NULL);
        shared_ptr<ck_json_object> obj = static_pointer_cast<ck_json_object>(item);
        shared_ptr<ck_json_object> obj2 = static_pointer_cast<ck_json_object>(obj->get_object_member("data"));
        shared_ptr<ck_json_array> ary = static_pointer_cast<ck_json_array>(obj2->get_object_member("motion_events"));
        UT_ASSERT( ary->size() == 0 );
        ary = static_pointer_cast<ck_json_array>(obj2->get_object_member("motion_events2"));
        UT_ASSERT( ary->size() == 0 );
    }
    {
        ck_string doc =  "{ \"data\": { \"motion_events\": [ ], \"motion_events2\": [] } }";
        shared_ptr<ck_json_item> item = NULL;
        UT_ASSERT_NO_THROW( item = ck_json_item::parse_document(doc) );
        UT_ASSERT(item.get() != NULL);
        shared_ptr<ck_json_object> obj = static_pointer_cast<ck_json_object>(item);
        shared_ptr<ck_json_object> obj2 = static_pointer_cast<ck_json_object>(obj->get_object_member("data"));
        shared_ptr<ck_json_array> ary = static_pointer_cast<ck_json_array>(obj2->get_object_member("motion_events"));
        UT_ASSERT( ary->size() == 0 );
        ary = static_pointer_cast<ck_json_array>(obj2->get_object_member("motion_events2"));
        UT_ASSERT( ary->size() == 0 );
    }
    {
        ck_string doc =  "{ \"data\": { \"motion_events\": [], \"motion_events2\": [] } }";
        shared_ptr<ck_json_item> item = NULL;
        UT_ASSERT_NO_THROW( item = ck_json_item::parse_document(doc) );
        UT_ASSERT(item.get() != NULL);
        shared_ptr<ck_json_object> obj = static_pointer_cast<ck_json_object>(item);
        shared_ptr<ck_json_object> obj2 = static_pointer_cast<ck_json_object>(obj->get_object_member("data"));
        shared_ptr<ck_json_array> ary = static_pointer_cast<ck_json_array>(obj2->get_object_member("motion_events"));
        UT_ASSERT( ary->size() == 0 );
        ary = static_pointer_cast<ck_json_array>(obj2->get_object_member("motion_events2"));
        UT_ASSERT( ary->size() == 0 );
    }
    {
        ck_string doc =  "{ \"data\": [ {} ]}";
        shared_ptr<ck_json_item> item = NULL;
        UT_ASSERT_NO_THROW( item = ck_json_item::parse_document(doc) );
        UT_ASSERT(item.get() != NULL);
        shared_ptr<ck_json_object> obj = static_pointer_cast<ck_json_object>(item);
        shared_ptr<ck_json_array> ary = static_pointer_cast<ck_json_array>(obj->get_object_member("data"));
        UT_ASSERT( ary->size() == 1 );
        obj = static_pointer_cast<ck_json_object>(ary->index(0));
        UT_ASSERT(obj.get() != NULL);
        UT_ASSERT(obj->GetObjectContents().size() == 0 );
    }
}

void ck_json_test::TestFind()
{
    {
        ck_string doc = "{\"menu\": {"
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
        shared_ptr<ck_json_item> item = NULL;
        UT_ASSERT_NO_THROW( item = ck_json_item::parse_document(doc) );
        UT_ASSERT(item.get() != NULL);
        shared_ptr<ck_json_value> value;
        UT_ASSERT_NO_THROW(value = static_pointer_cast<ck_json_value>(ck_json_item::find(item,"menu/items/3/id")));
        ck_string temp = value->get_value().get<ck_string>();
        UT_ASSERT( temp == "ZoomIn" );
    }
    {
        ck_string doc = "{\"menu\": {"
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
        shared_ptr<ck_json_item> item = NULL;
        UT_ASSERT_NO_THROW( item = ck_json_item::parse_document(doc) );
        UT_ASSERT(item.get() != NULL);
        shared_ptr<ck_json_value> value;
        UT_ASSERT_THROWS( value = static_pointer_cast<ck_json_value>(ck_json_item::find(item,"menu/items/Tim/id")), ck_json_exception );
    }
    {
        ck_string doc = "["
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
        shared_ptr<ck_json_item> item = NULL;
        UT_ASSERT_NO_THROW( item = ck_json_item::parse_document(doc) );
        UT_ASSERT(item.get() != NULL);
        shared_ptr<ck_json_value> value;
        UT_ASSERT_NO_THROW( value = static_pointer_cast<ck_json_value>(ck_json_item::find(item,"0/0/0")) );
        int x = value->get_value();
        UT_ASSERT( x == 23 );
    }
    {
        ck_string doc = "["
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
        shared_ptr<ck_json_item> item = NULL;
        UT_ASSERT_NO_THROW( item = ck_json_item::parse_document(doc) );
        UT_ASSERT(item.get() != NULL);
        shared_ptr<ck_json_value> value;
        UT_ASSERT_NO_THROW( value = static_pointer_cast<ck_json_value>(ck_json_item::find(item,"0/0/2")) );
        UT_ASSERT( value.get() == NULL );
    }
    {
        ck_string doc = "{\"menu\": {"
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
        shared_ptr<ck_json_item> item = NULL;
        UT_ASSERT_NO_THROW( item = ck_json_item::parse_document(doc) );
        UT_ASSERT(item.get() != NULL);
        shared_ptr<ck_json_value> value;
        UT_ASSERT_NO_THROW( value = static_pointer_cast<ck_json_value>(ck_json_item::find(item,"menu/items/1/Naboo")) );
        UT_ASSERT( value.get() == NULL );
    }
}

void ck_json_test::TestForQuotedContent()
{
    ck_string doc = "{\"foo\":\"%s\"}";
    doc = ck_string::format(doc.c_str(),"\\\"temp\\\"");
    shared_ptr<ck_json_item> item = NULL;
    UT_ASSERT_NO_THROW( item = ck_json_item::parse_document(doc) );
    UT_ASSERT(item.get() != NULL);
    UT_ASSERT(item->get_type() == CK_JSON_Object_Type);
    shared_ptr<ck_json_object> obj = static_pointer_cast<ck_json_object>(item);
    unordered_map<string, shared_ptr<ck_json_item> > contents = obj->GetObjectContents();
    UT_ASSERT( contents.size() == 1 );
    ck_string foo = obj->get_object_member("foo")->get<ck_string>();
    ck_string bar = "\\\"temp\\\"";
    UT_ASSERT( foo == bar );
}

void ck_json_test::TestEmptyObjInArray()
{
    ck_string doc = "{\"foo\":[]}";
    shared_ptr<ck_json_item> item = NULL;
    UT_ASSERT_NO_THROW( item = ck_json_item::parse_document(doc) );
    UT_ASSERT(item.get() != NULL);
    UT_ASSERT(item->get_type() == CK_JSON_Object_Type);
    shared_ptr<ck_json_object> obj = static_pointer_cast<ck_json_object>(item);
    unordered_map<string, shared_ptr<ck_json_item> > contents = obj->GetObjectContents();
    UT_ASSERT( contents.size() == 1 );
    ck_string foo = "foo";
    shared_ptr<ck_json_array> ary = static_pointer_cast<ck_json_array>(obj->get_object_member(foo));
    UT_ASSERT( ary->get_array().size() == 0 );
}

void ck_json_test::TestEmptyObjInObj()
{
    ck_string doc = "{\"foo\":{}}";
    shared_ptr<ck_json_item> item = NULL;
    UT_ASSERT_NO_THROW( item = ck_json_item::parse_document(doc) );
    UT_ASSERT(item.get() != NULL);
    UT_ASSERT(item->get_type() == CK_JSON_Object_Type);
    shared_ptr<ck_json_object> obj = static_pointer_cast<ck_json_object>(item);
    unordered_map<string, shared_ptr<ck_json_item> > contents = obj->GetObjectContents();
    UT_ASSERT( contents.size() == 1 );
    ck_string foo = "foo";
    shared_ptr<ck_json_object> obj2 = static_pointer_cast<ck_json_object>(obj->get_object_member(foo));
    UT_ASSERT( obj2->GetObjectContents().size() == 0 );
}

void ck_json_test::TestEmptyArrayInObj()
{
    ck_string doc = "[{}]";
    shared_ptr<ck_json_item> item = NULL;
    UT_ASSERT_NO_THROW( item = ck_json_item::parse_document(doc) );
    UT_ASSERT(item.get() != NULL);
    shared_ptr<ck_json_array> ary = static_pointer_cast<ck_json_array>(item);
    UT_ASSERT( ary->get_array().size() == 1 );
    shared_ptr<ck_json_object> obj = static_pointer_cast<ck_json_object>(ary->access_element(0));
    unordered_map<string, shared_ptr<ck_json_item> > contents = obj->GetObjectContents();
    UT_ASSERT( contents.size() == 0 );
}

void ck_json_test::TestEmptyArrayInArray()
{
    ck_string doc = "[[]]";
    shared_ptr<ck_json_item> item = NULL;
    UT_ASSERT_NO_THROW( item = ck_json_item::parse_document(doc) );
    UT_ASSERT(item.get() != NULL);
    shared_ptr<ck_json_array> ary = static_pointer_cast<ck_json_array>(item);
    UT_ASSERT( ary->get_array().size() == 1 );
    shared_ptr<ck_json_array> ary2 = static_pointer_cast<ck_json_array>(ary->access_element(0));
    UT_ASSERT( ary2->get_array().size() == 0 );
}

void ck_json_test::TestEmptyObj()
{
    ck_string doc = "{}";
    shared_ptr<ck_json_item> item = NULL;
    UT_ASSERT_NO_THROW( item = ck_json_item::parse_document(doc) );
    UT_ASSERT(item.get() != NULL);
    UT_ASSERT(item->get_type() == CK_JSON_Object_Type);
    shared_ptr<ck_json_object> obj = static_pointer_cast<ck_json_object>(item);
    unordered_map<string, shared_ptr<ck_json_item> > contents = obj->GetObjectContents();
    UT_ASSERT( contents.size() == 0 );
}

void ck_json_test::TestEmptyArray()
{
    ck_string doc = "[]";
    shared_ptr<ck_json_item> item = NULL;
    UT_ASSERT_NO_THROW( item = ck_json_item::parse_document(doc) );
    UT_ASSERT(item.get() != NULL);
    UT_ASSERT(item->get_type() == CK_JSON_Array_Type);
    shared_ptr<ck_json_array> ary = static_pointer_cast<ck_json_array>(item);
    std::vector< shared_ptr<ck_json_item> > contents = ary->get_array();
    UT_ASSERT( contents.size() == 0 );
}

void ck_json_test::TestSimpleArray()
{
    ck_string doc = "[42,43,44]";
    shared_ptr<ck_json_item> item = ck_json_item::parse_document(doc);
    UT_ASSERT(item.get() != NULL);
    UT_ASSERT(item->get_type() == CK_JSON_Array_Type);
    std::vector<shared_ptr<ck_json_item> > contents = static_pointer_cast<ck_json_array>(item)->get_array();
    UT_ASSERT( contents.size() == 3 );
    for( size_t i = 0; i < contents.size(); ++i )
    {
        double d = i + 42;
        UT_ASSERT( contents[i]->get_type() == CK_JSON_Value_Type );
        UT_ASSERT( static_pointer_cast<ck_json_value>(contents[i])->get_value().get_type() == CK_VARTYPE_DOUBLE );
        UT_ASSERT( static_pointer_cast<ck_json_value>(contents[i])->get_value().get<double>() == d );
    }
}

void ck_json_test::TestSimpleObject()
{
    ck_string doc = "{\"foo\":42,\"bar\":44}";
    shared_ptr<ck_json_item> item = ck_json_item::parse_document(doc);
    UT_ASSERT(item.get() != NULL);
    UT_ASSERT(item->get_type() == CK_JSON_Object_Type);
    unordered_map<string, shared_ptr<ck_json_item> > contents = static_pointer_cast<ck_json_object>(item)->GetObjectContents();
    UT_ASSERT( contents.size() == 2 );

    shared_ptr<ck_json_value> value = static_pointer_cast<ck_json_value>(contents["foo"]);
    UT_ASSERT( value->get_value().get_type() == CK_VARTYPE_DOUBLE );
    UT_ASSERT( ((double)(value->get_value())) == 42 );
    value = static_pointer_cast<ck_json_value>(contents["bar"]);
    UT_ASSERT( value->get_value().get_type() == CK_VARTYPE_DOUBLE );
    UT_ASSERT( ((double)(value->get_value())) == 44 );
}

void ck_json_test::TestArrayContainArray()
{
    ck_string doc = "["
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
    shared_ptr<ck_json_item> item = ck_json_item::parse_document(doc);
    UT_ASSERT(item.get() != NULL);
    UT_ASSERT(item->get_type() == CK_JSON_Array_Type);
    std::vector<shared_ptr<ck_json_item> > contents = static_pointer_cast<ck_json_array>(item)->get_array();

    UT_ASSERT( contents.size() == 3 );

    shared_ptr<ck_json_item> elem1 = contents[0];
    UT_ASSERT( elem1->get_type() == CK_JSON_Array_Type );
    std::vector< shared_ptr<ck_json_item> > subContents = static_pointer_cast<ck_json_array>(elem1)->get_array();
    UT_ASSERT( subContents.size() == 4 );

    shared_ptr<ck_json_item> subElem0 = subContents[0];
    shared_ptr<ck_json_item> subElem1 = subContents[1];
    shared_ptr<ck_json_item> subElem2 = subContents[2];
    shared_ptr<ck_json_item> subElem3 = subContents[3];

    UT_ASSERT( subElem0->get_type() == CK_JSON_Array_Type );
    std::vector<shared_ptr<ck_json_item> > subSubContents = static_pointer_cast<ck_json_array>(subElem0)->get_array();
    UT_ASSERT( subSubContents.size() == 1 );

    shared_ptr<ck_json_item> subSubElem0 = subSubContents[0];
    UT_ASSERT( subSubElem0->get_type() == CK_JSON_Value_Type );
    UT_ASSERT( static_pointer_cast<ck_json_value>(subSubElem0)->get_value().get_type() == CK_VARTYPE_DOUBLE );
    UT_ASSERT( static_pointer_cast<ck_json_value>(subSubElem0)->get_value().get<double>() == 23);

    UT_ASSERT( subElem1->get_type() == CK_JSON_Value_Type );
    UT_ASSERT( static_pointer_cast<ck_json_value>(subElem1)->get_value().get_type() == CK_VARTYPE_DOUBLE );
    UT_ASSERT( static_pointer_cast<ck_json_value>(subElem1)->get_value().get<double>() == 3 );
    UT_ASSERT( subElem2->get_type() == CK_JSON_Value_Type );
    UT_ASSERT( static_pointer_cast<ck_json_value>(subElem2)->get_value().get_type() == CK_VARTYPE_DOUBLE );
    UT_ASSERT( static_pointer_cast<ck_json_value>(subElem2)->get_value().get<double>() == 3 );
    UT_ASSERT( subElem3->get_type() == CK_JSON_Value_Type );
    UT_ASSERT( static_pointer_cast<ck_json_value>(subElem3)->get_value().get_type() == CK_VARTYPE_DOUBLE );
    UT_ASSERT( static_pointer_cast<ck_json_value>(subElem3)->get_value().get<double>() == 7 );

    shared_ptr<ck_json_item> elem2 = contents[1];
    UT_ASSERT( elem2->get_type() == CK_JSON_Value_Type );
    UT_ASSERT( static_pointer_cast<ck_json_value>(elem2)->get_value().get_type() == CK_VARTYPE_DOUBLE );
    UT_ASSERT( static_pointer_cast<ck_json_value>(elem2)->get_value().get<double>() == 43 );

    shared_ptr<ck_json_item> elem3 = contents[2];
    UT_ASSERT( elem3->get_type() == CK_JSON_Value_Type );
    UT_ASSERT( static_pointer_cast<ck_json_value>(elem3)->get_value().get_type() == CK_VARTYPE_DOUBLE );
    UT_ASSERT( static_pointer_cast<ck_json_value>(elem3)->get_value().get<double>() == 44 );
}

void ck_json_test::TestShortHand()
{
    ck_string doc = "{\"menu\": {"
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
    shared_ptr<ck_json_item> item = NULL;
    UT_ASSERT_NO_THROW( item = ck_json_item::parse_document(doc) );
    UT_ASSERT(item.get() != NULL);

    UT_ASSERT( item->index("menu")->index("header")->get<ck_string>() == "SVG Viewer" );
    UT_ASSERT( item->index("menu")->index("items")->index(1)->index("label")->get<ck_string>() == "Open New" );
    UT_ASSERT_THROWS( item->index("menu")->index(0),ck_json_exception );
}

void ck_json_test::TestArrayContainObject()
{
    ck_string doc = "["
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
    shared_ptr<ck_json_item> item = ck_json_item::parse_document(doc);
    UT_ASSERT(item.get() != NULL);
    UT_ASSERT(item->get_type() == CK_JSON_Array_Type);
    std::vector<shared_ptr<ck_json_item> > contents = static_pointer_cast<ck_json_array>(item)->get_array();
    UT_ASSERT( contents.size() == 3 );

    shared_ptr<ck_json_item> elem1 = contents[0];
    UT_ASSERT( elem1->get_type() == CK_JSON_Array_Type );

    std::vector< shared_ptr<ck_json_item> > subContents = static_pointer_cast<ck_json_array>(elem1)->get_array();
    UT_ASSERT( subContents.size() == 4 );

    shared_ptr<ck_json_item> subElem0 = subContents[0];
    shared_ptr<ck_json_item> subElem1 = subContents[1];
    shared_ptr<ck_json_item> subElem2 = subContents[2];
    shared_ptr<ck_json_item> subElem3 = subContents[3];

    UT_ASSERT( subElem0->get_type() == CK_JSON_Object_Type );

    unordered_map<string, shared_ptr<ck_json_item> > subSubContents = static_pointer_cast<ck_json_object>(subElem0)->GetObjectContents();
    UT_ASSERT( subSubContents.size() == 1 );
    shared_ptr<ck_json_item> subSubElem0 = subSubContents["foo"];

    UT_ASSERT( subSubElem0->get_type() == CK_JSON_Value_Type );
    UT_ASSERT( static_pointer_cast<ck_json_value>(subSubElem0)->get_value().get_type() == CK_VARTYPE_DOUBLE );
    UT_ASSERT( static_pointer_cast<ck_json_value>(subSubElem0)->get_value().get<double>() == 23 );

    UT_ASSERT( subElem1->get_type() == CK_JSON_Value_Type );
    UT_ASSERT( static_pointer_cast<ck_json_value>(subElem1)->get_value().get_type() == CK_VARTYPE_DOUBLE );
    UT_ASSERT( static_pointer_cast<ck_json_value>(subElem1)->get_value().get<double>() == 3 );

    UT_ASSERT( subElem2->get_type() == CK_JSON_Value_Type );
    UT_ASSERT( static_pointer_cast<ck_json_value>(subElem2)->get_value().get_type() == CK_VARTYPE_DOUBLE );
    UT_ASSERT( static_pointer_cast<ck_json_value>(subElem2)->get_value().get<double>() == 3 );

    UT_ASSERT( subElem3->get_type() == CK_JSON_Value_Type );
    UT_ASSERT( static_pointer_cast<ck_json_value>(subElem3)->get_value().get_type() == CK_VARTYPE_DOUBLE );
    UT_ASSERT( static_pointer_cast<ck_json_value>(subElem3)->get_value().get<double>() == 7 );

    shared_ptr<ck_json_item> elem2 = contents[1];
    UT_ASSERT( elem2->get_type() == CK_JSON_Value_Type );
    UT_ASSERT( static_pointer_cast<ck_json_value>(elem2)->get_value().get_type() == CK_VARTYPE_DOUBLE );
    UT_ASSERT( static_pointer_cast<ck_json_value>(elem2)->get_value().get<double>() == 43 );

    shared_ptr<ck_json_item> elem3 = contents[2];
    UT_ASSERT( elem3->get_type() == CK_JSON_Value_Type );
    UT_ASSERT( static_pointer_cast<ck_json_value>(elem3)->get_value().get_type() == CK_VARTYPE_DOUBLE );
    UT_ASSERT( static_pointer_cast<ck_json_value>(elem3)->get_value().get<double>() == 44 );
}

void ck_json_test::TestObjectContainObject()
{
    ck_string doc = "{"
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
    shared_ptr<ck_json_item> item = ck_json_item::parse_document(doc);
    UT_ASSERT(item.get() != NULL);
    UT_ASSERT(item->get_type() == CK_JSON_Object_Type);
    unordered_map<string, shared_ptr<ck_json_item> > contents = static_pointer_cast<ck_json_object>(item)->GetObjectContents();
    UT_ASSERT( contents.size() == 3 );

    shared_ptr<ck_json_item> elem1 = contents["A"];
    UT_ASSERT( elem1->get_type() == CK_JSON_Object_Type );
    unordered_map<string, shared_ptr<ck_json_item> > subContents = static_pointer_cast<ck_json_object>(elem1)->GetObjectContents();
    UT_ASSERT( subContents.size() == 4 );
    shared_ptr<ck_json_item> subElem0 = subContents["tim"];
    shared_ptr<ck_json_item> subElem1 = subContents["jack"];
    shared_ptr<ck_json_item> subElem2 = subContents["ryan"];
    shared_ptr<ck_json_item> subElem3 = subContents["dingo"];

    UT_ASSERT( subElem0->get_type() == CK_JSON_Object_Type );
    unordered_map<string, shared_ptr<ck_json_item> > subSubContents = static_pointer_cast<ck_json_object>(subElem0)->GetObjectContents();
    UT_ASSERT( subSubContents.size() == 1 );

    shared_ptr<ck_json_item> subSubElem0 = subSubContents["foo"];

    UT_ASSERT( subSubElem0->get_type() == CK_JSON_Value_Type );
    UT_ASSERT( static_pointer_cast<ck_json_value>(subSubElem0)->get_value().get_type() == CK_VARTYPE_DOUBLE );
    UT_ASSERT( static_pointer_cast<ck_json_value>(subSubElem0)->get_value().get<double>() == 23 );

    UT_ASSERT( subElem1->get_type() == CK_JSON_Value_Type );
    UT_ASSERT( static_pointer_cast<ck_json_value>(subElem1)->get_value().get_type() == CK_VARTYPE_DOUBLE );
    UT_ASSERT( static_pointer_cast<ck_json_value>(subElem1)->get_value().get<double>() == 3 );

    UT_ASSERT( subElem2->get_type() == CK_JSON_Value_Type );
    UT_ASSERT( static_pointer_cast<ck_json_value>(subElem2)->get_value().get_type() == CK_VARTYPE_DOUBLE );
    UT_ASSERT( static_pointer_cast<ck_json_value>(subElem2)->get_value().get<double>() == 3 );

    UT_ASSERT( subElem3->get_type() == CK_JSON_Value_Type );
    UT_ASSERT( static_pointer_cast<ck_json_value>(subElem3)->get_value().get_type() == CK_VARTYPE_DOUBLE );
    UT_ASSERT( static_pointer_cast<ck_json_value>(subElem3)->get_value().get<double>() == 7 );

    shared_ptr<ck_json_item> elem2 = contents["B"];
    UT_ASSERT( elem2->get_type() == CK_JSON_Value_Type );
    UT_ASSERT( static_pointer_cast<ck_json_value>(elem2)->get_value().get_type() == CK_VARTYPE_DOUBLE );
    UT_ASSERT( static_pointer_cast<ck_json_value>(elem2)->get_value().get<double>() == 43 );

    shared_ptr<ck_json_item> elem3 = contents["C"];
    UT_ASSERT( elem3->get_type() == CK_JSON_Value_Type );
    UT_ASSERT( static_pointer_cast<ck_json_value>(elem3)->get_value().get_type() == CK_VARTYPE_DOUBLE );
    UT_ASSERT( static_pointer_cast<ck_json_value>(elem3)->get_value().get<double>() == 44 );
}

void ck_json_test::TestObjectContainArray()
{

    ck_string doc = "{"
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
    shared_ptr<ck_json_item> item = ck_json_item::parse_document(doc);
    UT_ASSERT(item.get() != NULL);
    UT_ASSERT(item->get_type() == CK_JSON_Object_Type);
    unordered_map<string, shared_ptr<ck_json_item> > contents = dynamic_pointer_cast<ck_json_object>(item)->GetObjectContents();
    UT_ASSERT( contents.size() == 3 );

    shared_ptr<ck_json_item> elem1 = contents["A"];
    UT_ASSERT( elem1->get_type() == CK_JSON_Object_Type );
    unordered_map<string, shared_ptr<ck_json_item> > subContents = dynamic_pointer_cast<ck_json_object>(elem1)->GetObjectContents();
    UT_ASSERT( subContents.size() == 4 );
    shared_ptr<ck_json_item> subElem0 = subContents["tim"];
    shared_ptr<ck_json_item> subElem1 = subContents["jack"];
    shared_ptr<ck_json_item> subElem2 = subContents["ryan"];
    shared_ptr<ck_json_item> subElem3 = subContents["dingo"];

    UT_ASSERT( subElem0->get_type() == CK_JSON_Array_Type );
    std::vector< shared_ptr<ck_json_item> > subSubContents = dynamic_pointer_cast<ck_json_array>(subElem0)->get_array();
    UT_ASSERT( subSubContents.size() == 1 );
    shared_ptr<ck_json_item> subSubElem0 = subSubContents[0];
    UT_ASSERT( subSubElem0->get_type() == CK_JSON_Value_Type );
    UT_ASSERT( dynamic_pointer_cast<ck_json_value>(subSubElem0)->get_value().get_type() == CK_VARTYPE_DOUBLE );
    UT_ASSERT( dynamic_pointer_cast<ck_json_value>(subSubElem0)->get_value().get<double>() == 23 );

    UT_ASSERT( subElem1->get_type() == CK_JSON_Value_Type );
    UT_ASSERT( dynamic_pointer_cast<ck_json_value>(subElem1)->get_value().get_type() == CK_VARTYPE_DOUBLE );
    UT_ASSERT( dynamic_pointer_cast<ck_json_value>(subElem1)->get_value().get<double>() == 3 );
    UT_ASSERT( subElem2->get_type() == CK_JSON_Value_Type );
    UT_ASSERT( dynamic_pointer_cast<ck_json_value>(subElem2)->get_value().get_type() == CK_VARTYPE_DOUBLE );
    UT_ASSERT( dynamic_pointer_cast<ck_json_value>(subElem2)->get_value().get<double>() == 3 );
    UT_ASSERT( subElem3->get_type() == CK_JSON_Value_Type );
    UT_ASSERT( dynamic_pointer_cast<ck_json_value>(subElem3)->get_value().get_type() == CK_VARTYPE_DOUBLE );
    UT_ASSERT( dynamic_pointer_cast<ck_json_value>(subElem3)->get_value().get<double>() == 7 );

    shared_ptr<ck_json_item> elem2 = contents["B"];
    UT_ASSERT( elem2->get_type() == CK_JSON_Value_Type );
    UT_ASSERT( dynamic_pointer_cast<ck_json_value>(elem2)->get_value().get_type() == CK_VARTYPE_DOUBLE );
    UT_ASSERT( dynamic_pointer_cast<ck_json_value>(elem2)->get_value().get<double>() == 43 );

    shared_ptr<ck_json_item> elem3 = contents["C"];
    UT_ASSERT( elem3->get_type() == CK_JSON_Value_Type );
    UT_ASSERT( dynamic_pointer_cast<ck_json_value>(elem3)->get_value().get_type() == CK_VARTYPE_DOUBLE );
    UT_ASSERT( dynamic_pointer_cast<ck_json_value>(elem3)->get_value().get<double>() == 44 );
}

void ck_json_test::TestJsonOrgExample()
{
    ck_string doc = "{\"menu\": {"
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
    ck_string str;

    shared_ptr<ck_json_item> item = ck_json_item::parse_document(doc);
    UT_ASSERT(item.get() != NULL);
    UT_ASSERT(item->get_type() == CK_JSON_Object_Type);

    unordered_map<string, shared_ptr<ck_json_item> > contents = dynamic_pointer_cast<ck_json_object>(item)->GetObjectContents();
    UT_ASSERT( contents.size() == 1 );

    shared_ptr<ck_json_item> menu = contents["menu"];
    UT_ASSERT( menu->get_type() == CK_JSON_Object_Type );
    unordered_map<string, shared_ptr<ck_json_item> > subContents = dynamic_pointer_cast<ck_json_object>(menu)->GetObjectContents();
    UT_ASSERT( subContents.size() == 2 );

    shared_ptr<ck_json_item> header = subContents["header"];
    UT_ASSERT( header->get_type() == CK_JSON_Value_Type );
    UT_ASSERT( dynamic_pointer_cast<ck_json_value>(header)->get_value().get_type() == CK_VARTYPE_TEXT );

    str = dynamic_pointer_cast<ck_json_value>(header)->get_value().get<ck_string>();

    UT_ASSERT( str == "SVG Viewer" );

    shared_ptr<ck_json_item> items = subContents["items"];
    UT_ASSERT( items->get_type() == CK_JSON_Array_Type );
    std::vector< shared_ptr<ck_json_item> > subSubContents = dynamic_pointer_cast<ck_json_array>(items)->get_array();
    UT_ASSERT( subSubContents.size() == 22 );
    shared_ptr<ck_json_item> subSubElem0 = subSubContents[0];
    shared_ptr<ck_json_item> subSubElem1 = subSubContents[1];
    shared_ptr<ck_json_item> subSubElem2 = subSubContents[2];
    shared_ptr<ck_json_item> subSubElem6 = subSubContents[6];
    shared_ptr<ck_json_item> subSubElem10 = subSubContents[10];
    UT_ASSERT( subSubElem0->get_type() == CK_JSON_Object_Type );
    UT_ASSERT( subSubElem1->get_type() == CK_JSON_Object_Type );
    UT_ASSERT( subSubElem2->get_type() == CK_JSON_Value_Type );
    UT_ASSERT( subSubElem6->get_type() == CK_JSON_Value_Type );
    UT_ASSERT( subSubElem10->get_type() == CK_JSON_Value_Type );
    UT_ASSERT( dynamic_pointer_cast<ck_json_value>(subSubElem2)->get_value().get_type() == CK_VARTYPE_EMPTY );


    UT_ASSERT( dynamic_pointer_cast<ck_json_value>(subSubElem6)->get_value().get_type() == CK_VARTYPE_BOOL );
    UT_ASSERT( dynamic_pointer_cast<ck_json_value>(subSubElem6)->get_value().get<bool>() );

    UT_ASSERT( dynamic_pointer_cast<ck_json_value>(subSubElem10)->get_value().get_type() == CK_VARTYPE_BOOL );
    UT_ASSERT( !dynamic_pointer_cast<ck_json_value>(subSubElem10)->get_value().get<bool>() );

    UT_ASSERT( dynamic_pointer_cast<ck_json_object>(subSubElem0)->GetObjectContents().size() == 1 );
    UT_ASSERT( dynamic_pointer_cast<ck_json_object>(subSubElem0)->GetObjectContents()["id"]->get_type() == CK_JSON_Value_Type );
    UT_ASSERT( dynamic_pointer_cast<ck_json_value>( dynamic_pointer_cast<ck_json_object>(subSubElem0)->GetObjectContents()["id"] )->get_value().get<ck_string>() == "Open" );

    UT_ASSERT( dynamic_pointer_cast<ck_json_object>(subSubElem1)->GetObjectContents().size() == 2 );


    str = dynamic_pointer_cast<ck_json_value>( dynamic_pointer_cast<ck_json_object>(subSubElem1)->GetObjectContents()["id"] )->get_value().get<ck_string>();
    UT_ASSERT( str == "OpenNew" );

    UT_ASSERT( dynamic_pointer_cast<ck_json_object>(subSubElem1)->GetObjectContents()["id"]->get_type() == CK_JSON_Value_Type );

    UT_ASSERT( dynamic_pointer_cast<ck_json_object>(subSubElem1)->GetObjectContents()["label"]->get_type() == CK_JSON_Value_Type );
    str = dynamic_pointer_cast<ck_json_value>( dynamic_pointer_cast<ck_json_object>(subSubElem1)->GetObjectContents()["label"] )->get_value().get<ck_string>();
    UT_ASSERT( str == "Open New"  );
}
