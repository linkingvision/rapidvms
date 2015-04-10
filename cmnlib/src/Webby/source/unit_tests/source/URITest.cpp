
#include "URITest.h"

#include "XSDK/XHash.h"
#include "Webby/URI.h"
#include "Webby/WebbyException.h"


using namespace std;
using namespace XSDK;
using namespace WEBBY;

REGISTER_TEST_FIXTURE(URITest);


void URITest::TestDefaultConstructor()
{
    const URI uri;

    UT_ASSERT(uri.GetFullRawURI() == "/");
    UT_ASSERT(uri.GetGetArgs().Empty());
    UT_ASSERT(uri.GetResource().empty());
    UT_ASSERT(uri.GetResourcePath() == "/");
}


void URITest::TestConstructor()
{
    {
        const URI uri("hello");

        UT_ASSERT(uri.GetFullRawURI() == "/hello");
        UT_ASSERT(uri.GetGetArgs().Empty());
        UT_ASSERT(uri.GetResource() == "hello");
        UT_ASSERT(uri.GetResourcePath() == "/");
    }
    {
        const URI uri("/hello");

        UT_ASSERT(uri.GetFullRawURI() == "/hello");
        UT_ASSERT(uri.GetGetArgs().Empty());
        UT_ASSERT(uri.GetResource() == "hello");
        UT_ASSERT(uri.GetResourcePath() == "/");
    }
    {
        const URI uri("/hello/");

        UT_ASSERT(uri.GetFullRawURI() == "/hello");
        UT_ASSERT(uri.GetGetArgs().Empty());
        UT_ASSERT(uri.GetResource() == "hello");
        UT_ASSERT(uri.GetResourcePath() == "/");
    }
    {
        const URI uri("hello/world");

        UT_ASSERT(uri.GetFullRawURI() == "/hello/world");
        UT_ASSERT(uri.GetGetArgs().Empty());
        UT_ASSERT(uri.GetResource() == "world");
        UT_ASSERT(uri.GetResourcePath() == "/hello/");
    }
    {
        const URI uri("/hello/world");

        UT_ASSERT(uri.GetFullRawURI() == "/hello/world");
        UT_ASSERT(uri.GetGetArgs().Empty());
        UT_ASSERT(uri.GetResource() == "world");
        UT_ASSERT(uri.GetResourcePath() == "/hello/");
    }
    {
        const URI uri("hello/world/goodbye");

        UT_ASSERT(uri.GetFullRawURI() == "/hello/world/goodbye");
        UT_ASSERT(uri.GetGetArgs().Empty());
        UT_ASSERT(uri.GetResource() == "goodbye");
        UT_ASSERT(uri.GetResourcePath() == "/hello/world/");
    }
    {
        const URI uri("hello/world/goodbye/");

        UT_ASSERT(uri.GetFullRawURI() == "/hello/world/goodbye");
        UT_ASSERT(uri.GetGetArgs().Empty());
        UT_ASSERT(uri.GetResource() == "goodbye");
        UT_ASSERT(uri.GetResourcePath() == "/hello/world/");
    }
    {
        const URI uri("/hello/world?");

        UT_ASSERT(uri.GetFullRawURI() == "/hello/world");
        UT_ASSERT(uri.GetGetArgs().Empty());
        UT_ASSERT(uri.GetResource() == "world");
        UT_ASSERT(uri.GetResourcePath() == "/hello/");
    }
    {
        const URI uri("/hello/world?&");

        UT_ASSERT(uri.GetFullRawURI() == "/hello/world");
        UT_ASSERT(uri.GetGetArgs().Empty());
        UT_ASSERT(uri.GetResource() == "world");
        UT_ASSERT(uri.GetResourcePath() == "/hello/");
    }
    {
        const URI uri("/hello/world?silly=dog&go=ho%2fme");

        UT_ASSERT(uri.GetFullRawURI() == "/hello/world?go=ho%2Fme&silly=dog");

        UT_ASSERT(uri.GetResource() == "world");
        UT_ASSERT(uri.GetResourcePath() == "/hello/");

        UT_ASSERT(uri.GetGetArgs().GetElemCount() == 2);

        const XString* const pArg1 = uri.GetGetArgs().Find("silly");
        const XString* const pArg2 = uri.GetGetArgs().Find("go");

        UT_ASSERT(pArg1);
        UT_ASSERT(pArg2);

        UT_ASSERT(*pArg1 == "dog");
        UT_ASSERT(*pArg2 == "ho/me");
    }
    {
        const URI uri("/hello/world?sillydog&go=ho%2fme");

        UT_ASSERT(uri.GetFullRawURI() == "/hello/world?go=ho%2Fme");
        UT_ASSERT(uri.GetResource() == "world");
        UT_ASSERT(uri.GetResourcePath() == "/hello/");

        UT_ASSERT(uri.GetGetArgs().GetElemCount() == 1);

        const XString* const pArg1 = uri.GetGetArgs().Find("silly");
        const XString* const pArg2 = uri.GetGetArgs().Find("go");

        UT_ASSERT(!pArg1);
        UT_ASSERT(pArg2);

        UT_ASSERT(*pArg2 == "ho/me");
    }
    {
        const URI uri("/hello/world?silly=dog&go=ho%2fme&");

        UT_ASSERT(uri.GetFullRawURI() == "/hello/world?go=ho%2Fme&silly=dog");

        UT_ASSERT(uri.GetResource() == "world");
        UT_ASSERT(uri.GetResourcePath() == "/hello/");

        UT_ASSERT(uri.GetGetArgs().GetElemCount() == 2);

        const XString* const pArg1 = uri.GetGetArgs().Find("silly");
        const XString* const pArg2 = uri.GetGetArgs().Find("go");

        UT_ASSERT(pArg1);
        UT_ASSERT(pArg2);

        UT_ASSERT(*pArg1 == "dog");
        UT_ASSERT(*pArg2 == "ho/me");
    }
    {
        const URI uri("/hello/world?silly=dog&go=ho%2fme?why=so&many=questions");

        UT_ASSERT(uri.GetResource() == "world");
        UT_ASSERT(uri.GetResourcePath() == "/hello/");

        UT_ASSERT(uri.GetGetArgs().GetElemCount() == 4);

        const XString* const pArg1 = uri.GetGetArgs().Find("silly");
        const XString* const pArg2 = uri.GetGetArgs().Find("go");
        const XString* const pArg3 = uri.GetGetArgs().Find("why");
        const XString* const pArg4 = uri.GetGetArgs().Find("many");

        UT_ASSERT(pArg1);
        UT_ASSERT(pArg2);
        UT_ASSERT(pArg3);
        UT_ASSERT(pArg4);

        UT_ASSERT(*pArg1 == "dog");
        UT_ASSERT(*pArg2 == "ho/me");
        UT_ASSERT(*pArg3 == "so");
        UT_ASSERT(*pArg4 == "questions");
    }
    {
        const URI uri("/files/uuid:88274e37-3096-d24d-ad57-d48ddfa2a204%2Furn:pelco-com:serviceId:VideoOutput-1");
        UT_ASSERT(uri.GetResource() == "uuid:88274e37-3096-d24d-ad57-d48ddfa2a204%2Furn:pelco-com:serviceId:VideoOutput-1");
        UT_ASSERT(uri.GetResourcePath() == "/files/");
    }
    {
        const URI uri("/contents/?sourceId=uuid:459d7e6a-0383-f808-86ce-55baa383bc04/urn:pelco-com:serviceId:VideoOutput-1");
        UT_ASSERT(uri.GetResource() == "contents");
        UT_ASSERT(uri.GetResourcePath() == "/");

        const XString* const arg = uri.GetGetArgs().Find("sourceId");
        UT_ASSERT(arg != NULL);
        UT_ASSERT(*arg == "uuid:459d7e6a-0383-f808-86ce-55baa383bc04/urn:pelco-com:serviceId:VideoOutput-1");
    }
    {
        const URI uri = XString("/system/datasources/uuid:9487e434-1c1e-4422-a42c-08e3e7b6c3e1%2curn:") +
                        XString("pelco-com:serviceId:VideoOutput-1%2fmedia%2fserver%2fstreamInterfaces?") +
                        XString("sourceId=uuid:9487e434-1c1e-4422-a42c-08e3e7b6c3e1%2curn:pelco-com:serviceId") +
                        XString(":VideoOutput-1&userId=Cory%20Maddox&version=1.2&some%2cthing=blah%20bleh+bloke");
        UT_ASSERT(uri.GetResource() == XString("uuid:9487e434-1c1e-4422-a42c-08e3e7b6c3e1%2curn:") +
                                            XString("pelco-com:serviceId:VideoOutput-1%2fmedia%2fserver%2fstreamInterfaces"));
        UT_ASSERT(uri.GetResourcePath() == "/system/datasources/");

        UT_ASSERT(uri.GetGetArgs().GetElemCount() == 4);

        const XString* const something = uri.GetGetArgs().Find("some,thing");
        const XString* const sourceID = uri.GetGetArgs().Find("sourceId");
        const XString* const userID = uri.GetGetArgs().Find("userId");
        const XString* const version = uri.GetGetArgs().Find("version");

        UT_ASSERT(something);
        UT_ASSERT(sourceID);
        UT_ASSERT(userID);
        UT_ASSERT(version);

        UT_ASSERT(*something == "blah bleh bloke");
        UT_ASSERT(*sourceID == "uuid:9487e434-1c1e-4422-a42c-08e3e7b6c3e1,urn:pelco-com:serviceId:VideoOutput-1");
        UT_ASSERT(*userID == "Cory Maddox");
        UT_ASSERT(*version == "1.2");

        UT_ASSERT(uri.GetFullRawURI() == XString("/system/datasources/uuid:9487e434-1c1e-4422-a42c-08e3e7b6c3e1%2curn:") +
                                              XString("pelco-com:serviceId:VideoOutput-1%2fmedia%2fserver%2fstreamInterfaces?") +
                                              XString("some%2Cthing=blah%20bleh%20bloke&sourceId=uuid%3A9487e434-1c1e-4422-a42c-08e3e7b6c3e1") +
                                              XString("%2Curn%3Apelco-com%3AserviceId%3AVideoOutput-1&userId=Cory%20Maddox&version=1.2"));
    }
}


void URITest::TestConstructURI2Args()
{
    {
        const URI uri = URI::ConstructURI("", "hello");

        UT_ASSERT(uri.GetFullRawURI() == "/hello");
        UT_ASSERT(uri.GetGetArgs().Empty());
        UT_ASSERT(uri.GetResource() == "hello");
        UT_ASSERT(uri.GetResourcePath() == "/");
    }
    {
        const URI uri = URI::ConstructURI("/", "hello");

        UT_ASSERT(uri.GetFullRawURI() == "/hello");
        UT_ASSERT(uri.GetGetArgs().Empty());
        UT_ASSERT(uri.GetResource() == "hello");
        UT_ASSERT(uri.GetResourcePath() == "/");
    }
    {
        UT_ASSERT_THROWS(URI::ConstructURI("", "/"), WebbyException);
        UT_ASSERT_THROWS(URI::ConstructURI("", "/hello"), WebbyException);
        UT_ASSERT_THROWS(URI::ConstructURI("", "hello/"), WebbyException);
        UT_ASSERT_THROWS(URI::ConstructURI("?", ""), WebbyException);
        UT_ASSERT_THROWS(URI::ConstructURI("?", "?"), WebbyException);
    }
    {
        const URI uri = URI::ConstructURI("/hello/", "");

        UT_ASSERT(uri.GetFullRawURI() == "/hello/");
        UT_ASSERT(uri.GetGetArgs().Empty());
        UT_ASSERT(uri.GetResource().empty());
        UT_ASSERT(uri.GetResourcePath() == "/hello/");
    }
    {
        const URI uri = URI::ConstructURI("hello/", "world");

        UT_ASSERT(uri.GetFullRawURI() == "/hello/world");
        UT_ASSERT(uri.GetGetArgs().Empty());
        UT_ASSERT(uri.GetResource() == "world");
        UT_ASSERT(uri.GetResourcePath() == "/hello/");
    }
    {
        const URI uri = URI::ConstructURI("/hello/", "world");

        UT_ASSERT(uri.GetFullRawURI() == "/hello/world");
        UT_ASSERT(uri.GetGetArgs().Empty());
        UT_ASSERT(uri.GetResource() == "world");
        UT_ASSERT(uri.GetResourcePath() == "/hello/");
    }
    {
        const URI uri = URI::ConstructURI("/hello/world", "");

        UT_ASSERT(uri.GetFullRawURI() == "/hello/world/");
        UT_ASSERT(uri.GetGetArgs().Empty());
        UT_ASSERT(uri.GetResource().empty());
        UT_ASSERT(uri.GetResourcePath() == "/hello/world/");
    }
    {
        XHash<XString> getArgs;

        getArgs.Add("silly", "dog");
        getArgs.Add("go", "ho/me");

        const URI uri1("/hello/world?silly=dog&go=ho%2fme");
        URI uri2 = URI::ConstructURI("hello", "world", getArgs);

        UT_ASSERT(uri1 == uri2);
        UT_ASSERT(uri1.GetFullRawURI() == uri2.GetFullRawURI());
        UT_ASSERT(uri1.GetResource() == uri2.GetResource());
        UT_ASSERT(uri1.GetResourcePath() == uri2.GetResourcePath());

        UT_ASSERT(uri1.GetGetArgs().GetElemCount() == uri2.GetGetArgs().GetElemCount());

        XHash<XString> getArgs1 = uri1.GetGetArgs();
        XHash<XString> getArgs2 = uri2.GetGetArgs();
        x_foreach(getArgs1)
        {
            const XString* const pArgs2Val = getArgs2.Find(getArgs1.IterKey());

            UT_ASSERT(pArgs2Val);
            UT_ASSERT(*getArgs1.IterData() == *getArgs2.Find(getArgs1.IterKey()));
        }
    }
}


void URITest::TestConstructURI3Args()
{
    XHash<XString> noArgs;

    {
        const URI uri = URI::ConstructURI("", "hello", noArgs);

        UT_ASSERT(uri.GetFullRawURI() == "/hello");
        UT_ASSERT(uri.GetGetArgs().Empty());
        UT_ASSERT(uri.GetResource() == "hello");
        UT_ASSERT(uri.GetResourcePath() == "/");
    }
    {
        const URI uri = URI::ConstructURI("/", "hello", noArgs);

        UT_ASSERT(uri.GetFullRawURI() == "/hello");
        UT_ASSERT(uri.GetGetArgs().Empty());
        UT_ASSERT(uri.GetResource() == "hello");
        UT_ASSERT(uri.GetResourcePath() == "/");
    }
    {
        UT_ASSERT_THROWS(URI::ConstructURI("", "/"), WebbyException);
        UT_ASSERT_THROWS(URI::ConstructURI("", "/hello"), WebbyException);
        UT_ASSERT_THROWS(URI::ConstructURI("", "hello/"), WebbyException);
        UT_ASSERT_THROWS(URI::ConstructURI("?", ""), WebbyException);
        UT_ASSERT_THROWS(URI::ConstructURI("?", "?"), WebbyException);
    }
    {
        const URI uri = URI::ConstructURI("/hello/", "", noArgs);

        UT_ASSERT(uri.GetFullRawURI() == "/hello/");
        UT_ASSERT(uri.GetGetArgs().Empty());
        UT_ASSERT(uri.GetResource().empty());
        UT_ASSERT(uri.GetResourcePath() == "/hello/");
    }
    {
        const URI uri = URI::ConstructURI("hello/", "world", noArgs);

        UT_ASSERT(uri.GetFullRawURI() == "/hello/world");
        UT_ASSERT(uri.GetGetArgs().Empty());
        UT_ASSERT(uri.GetResource() == "world");
        UT_ASSERT(uri.GetResourcePath() == "/hello/");
    }
    {
        const URI uri = URI::ConstructURI("/hello/", "world", noArgs);

        UT_ASSERT(uri.GetFullRawURI() == "/hello/world");
        UT_ASSERT(uri.GetGetArgs().Empty());
        UT_ASSERT(uri.GetResource() == "world");
        UT_ASSERT(uri.GetResourcePath() == "/hello/");
    }
    {
        const URI uri = URI::ConstructURI("/hello/world", "", noArgs);

        UT_ASSERT(uri.GetFullRawURI() == "/hello/world/");
        UT_ASSERT(uri.GetGetArgs().Empty());
        UT_ASSERT(uri.GetResource().empty());
        UT_ASSERT(uri.GetResourcePath() == "/hello/world/");
    }
    {
        XHash<XString> getArgs;

        getArgs.Add("silly", "dog");
        getArgs.Add("go", "ho/me");

        const URI uri1("/hello/world?silly=dog&go=ho%2fme");
        const URI uri2 = URI::ConstructURI("hello", "world", getArgs);

        UT_ASSERT(uri1 == uri2);
        UT_ASSERT(uri1.GetFullRawURI() == uri2.GetFullRawURI());
        UT_ASSERT(uri1.GetResource() == uri2.GetResource());
        UT_ASSERT(uri1.GetResourcePath() == uri2.GetResourcePath());

        UT_ASSERT(uri1.GetGetArgs().GetElemCount() == uri2.GetGetArgs().GetElemCount());

        XHash<XString> getArgs1 = uri1.GetGetArgs();
        XHash<XString> getArgs2 = uri2.GetGetArgs();
        x_foreach(getArgs1)
        {
            const XString* const pArgs2Val = getArgs2.Find(getArgs1.IterKey());

            UT_ASSERT(pArgs2Val);
            UT_ASSERT(*getArgs1.IterData() == *getArgs2.Find(getArgs1.IterKey()));
        }
    }
    {
        XHash<XString> getArgs;

        getArgs.Add("silly", "");

        UT_ASSERT_THROWS(URI::ConstructURI("hello", "world", getArgs), WebbyException);
    }
}


void URITest::TestCopyConstructor()
{
    {
        const URI uri1("/hello");
        const URI uri2(uri1);

        UT_ASSERT(uri1 == uri2);
        UT_ASSERT(uri1.GetFullRawURI() == uri2.GetFullRawURI());
        UT_ASSERT(uri1.GetResource() == uri2.GetResource());
        UT_ASSERT(uri1.GetResourcePath() == uri2.GetResourcePath());

        UT_ASSERT(uri1.GetGetArgs().GetElemCount() == uri2.GetGetArgs().GetElemCount());

        XHash<XString> getArgs1 = uri1.GetGetArgs();
        XHash<XString> getArgs2 = uri2.GetGetArgs();
        x_foreach(getArgs1)
        {
            const XString* const pArgs2Val = getArgs2.Find(getArgs1.IterKey());

            UT_ASSERT(pArgs2Val);
            UT_ASSERT(*getArgs1.IterData() == *getArgs2.Find(getArgs1.IterKey()));
        }
    }
    {
        const URI uri1("/hello/world");
        const URI uri2(uri1);

        UT_ASSERT(uri1 == uri2);
        UT_ASSERT(uri1.GetFullRawURI() == uri2.GetFullRawURI());
        UT_ASSERT(uri1.GetResource() == uri2.GetResource());
        UT_ASSERT(uri1.GetResourcePath() == uri2.GetResourcePath());

        UT_ASSERT(uri1.GetGetArgs().GetElemCount() == uri2.GetGetArgs().GetElemCount());

        XHash<XString> getArgs1 = uri1.GetGetArgs();
        XHash<XString> getArgs2 = uri2.GetGetArgs();
        x_foreach(getArgs1)
        {
            const XString* const pArgs2Val = getArgs2.Find(getArgs1.IterKey());

            UT_ASSERT(pArgs2Val);
            UT_ASSERT(*getArgs1.IterData() == *getArgs2.Find(getArgs1.IterKey()));
        }
    }
    {
        const URI uri1("/hello/world?silly=dog&go=ho%2fme");
        const URI uri2(uri1);

        UT_ASSERT(uri1 == uri2);
        UT_ASSERT(uri1.GetFullRawURI() == uri2.GetFullRawURI());
        UT_ASSERT(uri1.GetResource() == uri2.GetResource());
        UT_ASSERT(uri1.GetResourcePath() == uri2.GetResourcePath());

        UT_ASSERT(uri1.GetGetArgs().GetElemCount() == uri2.GetGetArgs().GetElemCount());

        XHash<XString> getArgs1 = uri1.GetGetArgs();
        XHash<XString> getArgs2 = uri2.GetGetArgs();
        x_foreach(getArgs1)
        {
            const XString* const pArgs2Val = getArgs2.Find(getArgs1.IterKey());

            UT_ASSERT(pArgs2Val);
            UT_ASSERT(*getArgs1.IterData() == *getArgs2.Find(getArgs1.IterKey()));
        }
    }
}


void URITest::TestAssignmentOperator()
{
    {
        const URI uri1("/hello");
        URI uri2;

        uri2 = uri1;

        UT_ASSERT(uri1 == uri2);
        UT_ASSERT(uri1.GetFullRawURI() == uri2.GetFullRawURI());
        UT_ASSERT(uri1.GetResource() == uri2.GetResource());
        UT_ASSERT(uri1.GetResourcePath() == uri2.GetResourcePath());

        UT_ASSERT(uri1.GetGetArgs().GetElemCount() == uri2.GetGetArgs().GetElemCount());

        XHash<XString> getArgs1 = uri1.GetGetArgs();
        XHash<XString> getArgs2 = uri2.GetGetArgs();
        x_foreach(getArgs1)
        {
            const XString* const pArgs2Val = getArgs2.Find(getArgs1.IterKey());

            UT_ASSERT(pArgs2Val);
            UT_ASSERT(*getArgs1.IterData() == *getArgs2.Find(getArgs1.IterKey()));
        }
    }
    {
        const URI uri1("/hello/world");
        URI uri2;

        uri2 = uri1;

        UT_ASSERT(uri1 == uri2);
        UT_ASSERT(uri1.GetFullRawURI() == uri2.GetFullRawURI());
        UT_ASSERT(uri1.GetResource() == uri2.GetResource());
        UT_ASSERT(uri1.GetResourcePath() == uri2.GetResourcePath());

        UT_ASSERT(uri1.GetGetArgs().GetElemCount() == uri2.GetGetArgs().GetElemCount());

        XHash<XString> getArgs1 = uri1.GetGetArgs();
        XHash<XString> getArgs2 = uri2.GetGetArgs();
        x_foreach(getArgs1)
        {
            const XString* const pArgs2Val = getArgs2.Find(getArgs1.IterKey());

            UT_ASSERT(pArgs2Val);
            UT_ASSERT(*getArgs1.IterData() == *getArgs2.Find(getArgs1.IterKey()));
        }
    }
    {
        const URI uri1("/hello/world?silly=dog&go=ho%2fme");
        URI uri2;

        uri2 = uri1;

        UT_ASSERT(uri1 == uri2);
        UT_ASSERT(uri1.GetFullRawURI() == uri2.GetFullRawURI());
        UT_ASSERT(uri1.GetResource() == uri2.GetResource());
        UT_ASSERT(uri1.GetResourcePath() == uri2.GetResourcePath());

        UT_ASSERT(uri1.GetGetArgs().GetElemCount() == uri2.GetGetArgs().GetElemCount());

        XHash<XString> getArgs1 = uri1.GetGetArgs();
        XHash<XString> getArgs2 = uri2.GetGetArgs();
        x_foreach(getArgs1)
        {
            const XString* const pArgs2Val = getArgs2.Find(getArgs1.IterKey());

            UT_ASSERT(pArgs2Val);
            UT_ASSERT(*getArgs1.IterData() == *getArgs2.Find(getArgs1.IterKey()));
        }
    }
}


void URITest::TestGetFullResourcePath()
{
    {
        const URI uri("/hello/world");

        UT_ASSERT(uri.GetFullResourcePath() == "/hello/world");
    }
    {
        const URI uri("/hello/world/");

        UT_ASSERT(uri.GetFullResourcePath() == "/hello/world");
    }
    {
        const URI uri("/hello");

        UT_ASSERT(uri.GetFullResourcePath() == "/hello");
    }
    {
        const URI uri("/hello/");

        UT_ASSERT(uri.GetFullResourcePath() == "/hello");
    }
    {
        const URI uri;

        UT_ASSERT(uri.GetFullResourcePath() == "/");
    }
}


void URITest::TestAddGetArg()
{
    {
        const URI uri1("/hello/world?silly=dog&go=ho%2fme");
        URI uri2("/hello/world");

        uri2.AddGetArg("silly", "dog");
        uri2.AddGetArg("go", "ho/me");

        UT_ASSERT(uri1 == uri2);
        UT_ASSERT(uri1.GetFullRawURI() == uri2.GetFullRawURI());
        UT_ASSERT(uri1.GetResource() == uri2.GetResource());
        UT_ASSERT(uri1.GetResourcePath() == uri2.GetResourcePath());

        UT_ASSERT(uri1.GetGetArgs().GetElemCount() == uri2.GetGetArgs().GetElemCount());

        XHash<XString> getArgs1 = uri1.GetGetArgs();
        XHash<XString> getArgs2 = uri2.GetGetArgs();
        x_foreach(getArgs1)
        {
            const XString* const pArgs2Val = getArgs2.Find(getArgs1.IterKey());

            UT_ASSERT(pArgs2Val);
            UT_ASSERT(*getArgs1.IterData() == *getArgs2.Find(getArgs1.IterKey()));
        }
    }
    {
        URI uri1("/hello/world");
        URI uri2("/hello/world");

        UT_ASSERT_THROWS(uri2.AddGetArg("", "dog"), WebbyException);

        UT_ASSERT(uri1 == uri2);
        UT_ASSERT(uri1.GetFullRawURI() == uri2.GetFullRawURI());
        UT_ASSERT(uri1.GetResource() == uri2.GetResource());
        UT_ASSERT(uri1.GetResourcePath() == uri2.GetResourcePath());

        UT_ASSERT(uri1.GetGetArgs().GetElemCount() == uri2.GetGetArgs().GetElemCount());

        XHash<XString> getArgs1 = uri1.GetGetArgs();
        XHash<XString> getArgs2 = uri2.GetGetArgs();
        x_foreach(getArgs1)
        {
            const XString* const pArgs2Val = getArgs2.Find(getArgs1.IterKey());

            UT_ASSERT(pArgs2Val);
            UT_ASSERT(*getArgs1.IterData() == *getArgs2.Find(getArgs1.IterKey()));
        }
    }
    {
        URI uri1("/hello/world");
        URI uri2("/hello/world");

        UT_ASSERT_THROWS(uri2.AddGetArg("silly", ""), WebbyException);

        UT_ASSERT(uri1 == uri2);
        UT_ASSERT(uri1.GetFullRawURI() == uri2.GetFullRawURI());
        UT_ASSERT(uri1.GetResource() == uri2.GetResource());
        UT_ASSERT(uri1.GetResourcePath() == uri2.GetResourcePath());

        UT_ASSERT(uri1.GetGetArgs().GetElemCount() == uri2.GetGetArgs().GetElemCount());

        XHash<XString> getArgs1 = uri1.GetGetArgs();
        XHash<XString> getArgs2 = uri2.GetGetArgs();
        x_foreach(getArgs1)
        {
            const XString* const pArgs2Val = getArgs2.Find(getArgs1.IterKey());

            UT_ASSERT(pArgs2Val);
            UT_ASSERT(*getArgs1.IterData() == *getArgs2.Find(getArgs1.IterKey()));
        }
    }
}


void URITest::TestClearArgs()
{
    {
        URI uri1("/hello/world");
        URI uri2("/hello/world?silly=dog&go=ho%2fme");

        uri2.ClearGetArgs();

        UT_ASSERT(uri1.GetGetArgs().Empty());
        UT_ASSERT(uri2.GetGetArgs().Empty());
        UT_ASSERT(uri1 == uri2);
        UT_ASSERT(uri1.GetFullRawURI() == uri2.GetFullRawURI());
        UT_ASSERT(uri1.GetResource() == uri2.GetResource());
        UT_ASSERT(uri1.GetResourcePath() == uri2.GetResourcePath());
    }
    {
        URI uri1("/");
        URI uri2;

        uri2.ClearGetArgs();

        UT_ASSERT(uri1.GetGetArgs().Empty());
        UT_ASSERT(uri2.GetGetArgs().Empty());
        UT_ASSERT(uri1 == uri2);
        UT_ASSERT(uri1.GetFullRawURI() == uri2.GetFullRawURI());
        UT_ASSERT(uri1.GetResource() == uri2.GetResource());
        UT_ASSERT(uri1.GetResourcePath() == uri2.GetResourcePath());
    }
}


void URITest::TestSetFullRawURI()
{
    {
        const URI uri1("/hello");
        URI uri2;

        uri2.SetFullRawURI("hello");

        UT_ASSERT(uri1 == uri2);
        UT_ASSERT(uri1.GetFullRawURI() == uri2.GetFullRawURI());
        UT_ASSERT(uri1.GetResource() == uri2.GetResource());
        UT_ASSERT(uri1.GetResourcePath() == uri2.GetResourcePath());

        UT_ASSERT(uri1.GetGetArgs().GetElemCount() == uri2.GetGetArgs().GetElemCount());

        XHash<XString> getArgs1 = uri1.GetGetArgs();
        XHash<XString> getArgs2 = uri2.GetGetArgs();
        x_foreach(getArgs1)
        {
            const XString* const pArgs2Val = getArgs2.Find(getArgs1.IterKey());

            UT_ASSERT(pArgs2Val);
            UT_ASSERT(*getArgs1.IterData() == *getArgs2.Find(getArgs1.IterKey()));
        }
    }
    {
        const URI uri1("/hello");
        URI uri2;

        uri2.SetFullRawURI("/hello");

        UT_ASSERT(uri1 == uri2);
        UT_ASSERT(uri1.GetFullRawURI() == uri2.GetFullRawURI());
        UT_ASSERT(uri1.GetResource() == uri2.GetResource());
        UT_ASSERT(uri1.GetResourcePath() == uri2.GetResourcePath());

        UT_ASSERT(uri1.GetGetArgs().GetElemCount() == uri2.GetGetArgs().GetElemCount());

        XHash<XString> getArgs1 = uri1.GetGetArgs();
        XHash<XString> getArgs2 = uri2.GetGetArgs();
        x_foreach(getArgs1)
        {
            const XString* const pArgs2Val = getArgs2.Find(getArgs1.IterKey());

            UT_ASSERT(pArgs2Val);
            UT_ASSERT(*getArgs1.IterData() == *getArgs2.Find(getArgs1.IterKey()));
        }
    }
    {
        const URI uri1("/hello/world/goodbye");
        URI uri2;

        uri2.SetFullRawURI("/hello/world/goodbye");

        UT_ASSERT(uri1 == uri2);
        UT_ASSERT(uri1.GetFullRawURI() == uri2.GetFullRawURI());
        UT_ASSERT(uri1.GetResource() == uri2.GetResource());
        UT_ASSERT(uri1.GetResourcePath() == uri2.GetResourcePath());

        UT_ASSERT(uri1.GetGetArgs().GetElemCount() == uri2.GetGetArgs().GetElemCount());

        XHash<XString> getArgs1 = uri1.GetGetArgs();
        XHash<XString> getArgs2 = uri2.GetGetArgs();
        x_foreach(getArgs1)
        {
            const XString* const pArgs2Val = getArgs2.Find(getArgs1.IterKey());

            UT_ASSERT(pArgs2Val);
            UT_ASSERT(*getArgs1.IterData() == *getArgs2.Find(getArgs1.IterKey()));
        }
    }
    {
        const URI uri1("/hello/world/goodbye/");
        URI uri2;

        uri2.SetFullRawURI("/hello/world/goodbye/");

        UT_ASSERT(uri1 == uri2);
        UT_ASSERT(uri1.GetFullRawURI() == uri2.GetFullRawURI());
        UT_ASSERT(uri1.GetResource() == uri2.GetResource());
        UT_ASSERT(uri1.GetResourcePath() == uri2.GetResourcePath());

        UT_ASSERT(uri1.GetGetArgs().GetElemCount() == uri2.GetGetArgs().GetElemCount());

        XHash<XString> getArgs1 = uri1.GetGetArgs();
        XHash<XString> getArgs2 = uri2.GetGetArgs();
        x_foreach(getArgs1)
        {
            const XString* const pArgs2Val = getArgs2.Find(getArgs1.IterKey());

            UT_ASSERT(pArgs2Val);
            UT_ASSERT(*getArgs1.IterData() == *getArgs2.Find(getArgs1.IterKey()));
        }
    }
    {
        const URI uri1("/hello/world?");
        URI uri2;

        uri2.SetFullRawURI("/hello/world");

        UT_ASSERT(uri1 == uri2);
        UT_ASSERT(uri1.GetFullRawURI() == uri2.GetFullRawURI());
        UT_ASSERT(uri1.GetResource() == uri2.GetResource());
        UT_ASSERT(uri1.GetResourcePath() == uri2.GetResourcePath());

        UT_ASSERT(uri1.GetGetArgs().GetElemCount() == uri2.GetGetArgs().GetElemCount());

        XHash<XString> getArgs1 = uri1.GetGetArgs();
        XHash<XString> getArgs2 = uri2.GetGetArgs();
        x_foreach(getArgs1)
        {
            const XString* const pArgs2Val = getArgs2.Find(getArgs1.IterKey());

            UT_ASSERT(pArgs2Val);
            UT_ASSERT(*getArgs1.IterData() == *getArgs2.Find(getArgs1.IterKey()));
        }
    }
    {
        const URI uri1("/hello/world?&");
        URI uri2;

        uri2.SetFullRawURI("/hello/world");

        UT_ASSERT(uri1 == uri2);
        UT_ASSERT(uri1.GetFullRawURI() == uri2.GetFullRawURI());
        UT_ASSERT(uri1.GetResource() == uri2.GetResource());
        UT_ASSERT(uri1.GetResourcePath() == uri2.GetResourcePath());

        UT_ASSERT(uri1.GetGetArgs().GetElemCount() == uri2.GetGetArgs().GetElemCount());

        XHash<XString> getArgs1 = uri1.GetGetArgs();
        XHash<XString> getArgs2 = uri2.GetGetArgs();
        x_foreach(getArgs1)
        {
            const XString* const pArgs2Val = getArgs2.Find(getArgs1.IterKey());

            UT_ASSERT(pArgs2Val);
            UT_ASSERT(*getArgs1.IterData() == *getArgs2.Find(getArgs1.IterKey()));
        }
    }
    {
        const URI uri1("/hello/world");
        URI uri2;

        uri2.SetFullRawURI("/hello/world");

        UT_ASSERT(uri1 == uri2);
        UT_ASSERT(uri1.GetFullRawURI() == uri2.GetFullRawURI());
        UT_ASSERT(uri1.GetResource() == uri2.GetResource());
        UT_ASSERT(uri1.GetResourcePath() == uri2.GetResourcePath());

        UT_ASSERT(uri1.GetGetArgs().GetElemCount() == uri2.GetGetArgs().GetElemCount());

        XHash<XString> getArgs1 = uri1.GetGetArgs();
        XHash<XString> getArgs2 = uri2.GetGetArgs();
        x_foreach(getArgs1)
        {
            const XString* const pArgs2Val = getArgs2.Find(getArgs1.IterKey());

            UT_ASSERT(pArgs2Val);
            UT_ASSERT(*getArgs1.IterData() == *getArgs2.Find(getArgs1.IterKey()));
        }
    }
    {
        const URI uri1("/hello/world?silly=dog&go=ho%2fme");
        URI uri2;

        uri2 = uri1;
        uri2.SetFullRawURI("/hello/world?silly=dog&go=ho%2fme");

        UT_ASSERT(uri1 == uri2);
        UT_ASSERT(uri1.GetFullRawURI() == uri2.GetFullRawURI());
        UT_ASSERT(uri1.GetResource() == uri2.GetResource());
        UT_ASSERT(uri1.GetResourcePath() == uri2.GetResourcePath());

        UT_ASSERT(uri1.GetGetArgs().GetElemCount() == uri2.GetGetArgs().GetElemCount());

        XHash<XString> getArgs1 = uri1.GetGetArgs();
        XHash<XString> getArgs2 = uri2.GetGetArgs();
        x_foreach(getArgs1)
        {
            const XString* const pArgs2Val = getArgs2.Find(getArgs1.IterKey());

            UT_ASSERT(pArgs2Val);
            UT_ASSERT(*getArgs1.IterData() == *getArgs2.Find(getArgs1.IterKey()));
        }
    }
    {
        const URI uri1("/hello/world?sillydog&go=ho%2fme");
        URI uri2;

        uri2.SetFullRawURI("/hello/world?go=ho%2fme");

        UT_ASSERT(uri1 == uri2);
        UT_ASSERT(uri1.GetFullRawURI() == uri2.GetFullRawURI());
        UT_ASSERT(uri1.GetResource() == uri2.GetResource());
        UT_ASSERT(uri1.GetResourcePath() == uri2.GetResourcePath());

        UT_ASSERT(uri1.GetGetArgs().GetElemCount() == uri2.GetGetArgs().GetElemCount());

        XHash<XString> getArgs1 = uri1.GetGetArgs();
        XHash<XString> getArgs2 = uri2.GetGetArgs();
        x_foreach(getArgs1)
        {
            const XString* const pArgs2Val = getArgs2.Find(getArgs1.IterKey());

            UT_ASSERT(pArgs2Val);
            UT_ASSERT(*getArgs1.IterData() == *getArgs2.Find(getArgs1.IterKey()));
        }
    }
    {
        const URI uri1("/hello/world?silly=dog&go=ho%2fme&");
        URI uri2;

        uri2.SetFullRawURI("/hello/world?silly=dog&go=ho%2fme");

        UT_ASSERT(uri1 == uri2);
        UT_ASSERT(uri1.GetFullRawURI() == uri2.GetFullRawURI());
        UT_ASSERT(uri1.GetResource() == uri2.GetResource());
        UT_ASSERT(uri1.GetResourcePath() == uri2.GetResourcePath());

        UT_ASSERT(uri1.GetGetArgs().GetElemCount() == uri2.GetGetArgs().GetElemCount());

        XHash<XString> getArgs1 = uri1.GetGetArgs();
        XHash<XString> getArgs2 = uri2.GetGetArgs();
        x_foreach(getArgs1)
        {
            const XString* const pArgs2Val = getArgs2.Find(getArgs1.IterKey());

            UT_ASSERT(pArgs2Val);
            UT_ASSERT(*getArgs1.IterData() == *getArgs2.Find(getArgs1.IterKey()));
        }
    }
}


void URITest::TestSetResource()
{
    {
        const URI uri1("/hello/world");
        URI uri2("/hello/george");
        URI uri3("/hello/");
        URI uri4("/something/");

        uri2.SetResource("world");
        uri3.SetResource("world");
        uri4.SetResource("world");

        UT_ASSERT(uri1 == uri2);
        UT_ASSERT(uri4 == uri3);

        UT_ASSERT(uri1.GetResource() == uri2.GetResource());
        UT_ASSERT(uri4.GetResource() == uri3.GetResource());
    }
    {
        const URI uri1("/hello/world/goodbye");
        URI uri2("hello/world/george");
        URI uri3("hello/world/bob");
        URI uri4("hello/world/");
        URI uri5("hello/earth/");

        uri2.SetResource("goodbye");
        uri3.SetResource("goodbye");
        uri4.SetResource("earth");

        UT_ASSERT(uri1 == uri2);
        UT_ASSERT(uri1 == uri3);
        UT_ASSERT(uri4 == uri5);

        UT_ASSERT(uri1.GetResource() == uri2.GetResource());
        UT_ASSERT(uri1.GetResource() == uri3.GetResource());
        UT_ASSERT(uri4.GetResource() == uri5.GetResource());
    }
    {
        const URI uri1("/hello/world/goodbye/");
        URI uri2("hello/world/goodbye/george");
        URI uri3("hello/world/");

        uri2.SetResource("");

        UT_ASSERT(uri1 != uri2);
        UT_ASSERT(uri1.GetResource() != uri2.GetResource());

        UT_ASSERT_THROWS(uri3.SetResource("goodbye/george"), WebbyException);
        UT_ASSERT(uri1 != uri3);
        UT_ASSERT(uri1.GetResource() != uri3.GetResource());
        UT_ASSERT(uri2.GetResource().empty());
    }
    {
        const URI uri1("/hello/world/goodbye?silly=dog&go=ho%2fme");
        URI uri2("hello/world/george?silly=dog&go=ho%2fme");
        URI uri3;

        uri2.SetResource("goodbye");
        uri3.SetResource("goodbye");

        UT_ASSERT(uri1 == uri2);
        UT_ASSERT(uri1.GetResource() == uri2.GetResource());

        UT_ASSERT(uri1 != uri3);
        UT_ASSERT(uri1.GetResource() == uri3.GetResource());
    }
    {
        const URI uri1("/hello/world/?silly=dog&go=ho%2fme");
        URI uri2("hello/world/george?silly=dog&go=ho%2fme");
        URI uri3;

        uri2.SetResourcePath("/hello");
        uri2.SetResource("world");

        UT_ASSERT(uri1 == uri2);
        UT_ASSERT(uri1.GetResource() == uri2.GetResource());

        UT_ASSERT(uri1 != uri3);
        UT_ASSERT(uri1.GetResource() != uri3.GetResource());
    }
}


void URITest::TestSetResourcePath()
{
    {
        const URI uri1("/");
        URI uri2;

        uri2.SetResourcePath("/");

        UT_ASSERT(uri1 == uri2);
        UT_ASSERT(uri1.GetResourcePath() == uri2.GetResourcePath());
    }
    {
        const URI uri1("/hello");
        URI uri2;

        uri2.SetResourcePath("/");

        UT_ASSERT(uri1 != uri2);
        UT_ASSERT(uri1.GetResourcePath() == uri2.GetResourcePath());
    }
    {
        const URI uri1("/hello/");
        URI uri2;
        URI uri3;

        uri2.SetResourcePath("/hello");
        uri3.SetResource("hello");

        UT_ASSERT(uri1 != uri2);
        UT_ASSERT(uri1 == uri3);
        UT_ASSERT(uri2.GetResourcePath() != uri3.GetResourcePath());
    }
    {
        const URI uri1("/hello/world/goodbye");
        URI uri2("/george/goodbye");

        uri2.SetResourcePath("hello/world");

        UT_ASSERT(uri1 == uri2);
        UT_ASSERT(uri1.GetResourcePath() == uri2.GetResourcePath());
    }
    {
        const URI uri1("/hello/world");
        URI uri2;

        uri2.SetResourcePath("hello");

        UT_ASSERT(uri1 != uri2);
        UT_ASSERT(uri1.GetResourcePath() == uri2.GetResourcePath());
    }
    {
        const URI uri1("/hello/world");
        URI uri2;

        uri2.SetResourcePath("/hello/world");

        UT_ASSERT(uri1 != uri2);
        UT_ASSERT(uri1.GetResourcePath() != uri2.GetResourcePath());
        UT_ASSERT(uri2.GetResourcePath() == "/hello/world/");
    }
    {
        const URI uri1("/hello/world?");
        URI uri2;

        uri2.SetResourcePath("hello");

        UT_ASSERT(uri1 != uri2);
        UT_ASSERT(uri1.GetResourcePath() == uri2.GetResourcePath());
    }

    {
        const URI uri1("/hello/world?&");
        URI uri2;

        uri2.SetResourcePath("hello");

        UT_ASSERT(uri1 != uri2);
        UT_ASSERT(uri1.GetResourcePath() == uri2.GetResourcePath());
    }
    {
        const URI uri1("/hello/world?silly=dog&go=ho%2fme");
        URI uri2;

        uri2.SetResourcePath("hello");

        UT_ASSERT(uri1 != uri2);
        UT_ASSERT(uri1.GetResourcePath() == uri2.GetResourcePath());
    }
    {
        const URI uri1("/hello/world?silly=dog&go=ho%2fme");
        URI uri2("/no/way/jose/world?silly=dog&go=ho%2fme");

        uri2.SetResourcePath("hello");

        UT_ASSERT(uri1 == uri2);
        UT_ASSERT(uri1.GetResourcePath() == uri2.GetResourcePath());
    }
    {
        const URI uri1;
        URI uri2;

        UT_ASSERT_THROWS(uri2.SetResourcePath("world?"), WebbyException);
        UT_ASSERT(uri1 == uri2);
        UT_ASSERT(uri1.GetResourcePath() == uri2.GetResourcePath());
    }
}


void URITest::TestSetGetArgs()
{
    {
        const URI uri1("/hello/world?silly=dog&go=ho%2fme");
        URI uri2("/hello/world");
        XHash<XString> getArgs;

        getArgs.Add("silly", "dog");
        getArgs.Add("go", "ho/me");
        uri2.SetGetArgs(getArgs);

        UT_ASSERT(uri1 == uri2);
        UT_ASSERT(uri1.GetFullRawURI() == uri2.GetFullRawURI());
        UT_ASSERT(uri1.GetResource() == uri2.GetResource());
        UT_ASSERT(uri1.GetResourcePath() == uri2.GetResourcePath());

        UT_ASSERT(uri1.GetGetArgs().GetElemCount() == uri2.GetGetArgs().GetElemCount());

        XHash<XString> getArgs1 = uri1.GetGetArgs();
        XHash<XString> getArgs2 = uri2.GetGetArgs();
        x_foreach(getArgs1)
        {
            const XString* const pArgs2Val = getArgs2.Find(getArgs1.IterKey());

            UT_ASSERT(pArgs2Val);
            UT_ASSERT(*getArgs1.IterData() == *getArgs2.Find(getArgs1.IterKey()));
        }
    }
    {
        URI uri1("/hello/world");
        URI uri2("/hello/world");
        XHash<XString> getArgs;

        getArgs.Add("silly", "");

        UT_ASSERT_THROWS(uri2.SetGetArgs(getArgs), WebbyException);

        UT_ASSERT(uri1 == uri2);
        UT_ASSERT(uri1.GetFullRawURI() == uri2.GetFullRawURI());
        UT_ASSERT(uri1.GetResource() == uri2.GetResource());
        UT_ASSERT(uri1.GetResourcePath() == uri2.GetResourcePath());

        UT_ASSERT(uri1.GetGetArgs().GetElemCount() == uri2.GetGetArgs().GetElemCount());

        XHash<XString> getArgs1 = uri1.GetGetArgs();
        XHash<XString> getArgs2 = uri2.GetGetArgs();
        x_foreach(getArgs1)
        {
            const XString* const pArgs2Val = getArgs2.Find(getArgs1.IterKey());

            UT_ASSERT(pArgs2Val);
            UT_ASSERT(*getArgs1.IterData() == *getArgs2.Find(getArgs1.IterKey()));
        }
    }
    {
        URI uri1("/hello/world");
        XHash<XString> getArgs;

        UT_ASSERT_THROWS(getArgs.Add("", "dog"), XException);
    }
    {
        URI uri1("/hello/world");
        URI uri2("/hello/world");

        XHash<XString> getArgs;

        uri2.SetGetArgs(getArgs);

        UT_ASSERT(uri1 == uri2);
        UT_ASSERT(uri1.GetFullRawURI() == uri2.GetFullRawURI());
        UT_ASSERT(uri1.GetResource() == uri2.GetResource());
        UT_ASSERT(uri1.GetResourcePath() == uri2.GetResourcePath());

        UT_ASSERT(uri1.GetGetArgs().Empty());
        UT_ASSERT(uri2.GetGetArgs().Empty());
    }
}

