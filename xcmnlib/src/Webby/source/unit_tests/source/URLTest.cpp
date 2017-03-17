//=============================================================================
//Copyright (c) 2013 Pelco. All rights reserved.
//
// This file contains trade secrets of Pelco.  No part may be reproduced or
// transmitted in any form by any means or for any purpose without the express
// written permission of Pelco.
//
// $File: URLTest.cpp $
// $Revision:$
// $Date:$
// $Author: Jonathan M Davis $
//=============================================================================

#include "URLTest.h"

#include "XSDK/XHash.h"
#include "Webby/URL.h"
#include "Webby/WebbyException.h"


using namespace std;
using namespace XSDK;
using namespace WEBBY;

REGISTER_TEST_FIXTURE(URLTest);


void URLTest::TestDefaultConstructor()
{
    const URL url;
    UT_ASSERT(url.GetProtocol() == "SetMe");
    UT_ASSERT(url.GetIP() == "0.0.0.0");
    UT_ASSERT(url.GetPortNum() == XNullable<int>());
    UT_ASSERT(url.GetGetArgs().Empty());
    UT_ASSERT(url.GetURI() == URI());
    UT_ASSERT(url.GetFullRawURL() == "SetMe://0.0.0.0/");
}


void URLTest::Test4ArgConstructor()
{
    UT_ASSERT_THROWS(URL("", "", 0, URI("")), WebbyException);
    UT_ASSERT_THROWS(URL("", "", XNullable<int>(), URI("")), WebbyException);
    UT_ASSERT_THROWS(URL("", "", 0, URI("hello")), WebbyException);
    UT_ASSERT_THROWS(URL("", "", XNullable<int>(), URI("hello")), WebbyException);
    UT_ASSERT_THROWS(URL("file", "", 0, URI("")), WebbyException);
    UT_ASSERT_THROWS(URL("file", "", 0, URI("/etc/fstab")), WebbyException);
    UT_ASSERT_THROWS(URL("", "127.0.0.1", 0, URI("hello")), WebbyException);
    UT_ASSERT_THROWS(URL("", "127.0.0.1", XNullable<int>(), URI("hello")), WebbyException);

    {
        const URL url = URL("file", "", XNullable<int>(), URI(""));
        UT_ASSERT(url.GetProtocol() == "file");
        UT_ASSERT(url.GetIP().empty());
        UT_ASSERT(url.GetPortNum().IsNull());
        UT_ASSERT(url.GetGetArgs().Empty());
        UT_ASSERT(url.GetURI() == URI());
        UT_ASSERT(url.GetFullRawURL() == "file:///");
    }
    {
        const URI uri = "/etc/fstab";
        const URL url = URL("file", "", XNullable<int>(), uri);
        UT_ASSERT(url.GetProtocol() == "file");
        UT_ASSERT(url.GetIP().empty());
        UT_ASSERT(url.GetPortNum().IsNull());
        UT_ASSERT(url.GetGetArgs().Empty());
        UT_ASSERT(url.GetURI() == uri);
        UT_ASSERT(url.GetFullRawURL() == "file:///etc/fstab");
    }
    {
        const URI uri = URI("hello");
        const URL url = URL("http", "127.0.0.1", 0, uri);
        UT_ASSERT(url.GetProtocol() == "http");
        UT_ASSERT(url.GetIP() == "127.0.0.1");
        UT_ASSERT(url.GetPortNum().Value() == 0);
        UT_ASSERT(url.GetGetArgs().Empty());
        UT_ASSERT(url.GetURI() == uri);
        UT_ASSERT(url.GetFullRawURL() == "http://127.0.0.1:0/hello");
    }
    {
        const URI uri = URI("hello");
        const URL url = URL("http", "127.0.0.1", XNullable<int>(), uri);
        UT_ASSERT(url.GetProtocol() == "http");
        UT_ASSERT(url.GetIP() == "127.0.0.1");
        UT_ASSERT(url.GetPortNum().IsNull());
        UT_ASSERT(url.GetGetArgs().Empty());
        UT_ASSERT(url.GetURI() == uri);
        UT_ASSERT(url.GetFullRawURL() == "http://127.0.0.1/hello");
    }
    {
        const URI uri = URI("hello/world?foo=bar%20boo&a=27");
        const URL url = URL("https", "www.google.com", 42, uri);
        UT_ASSERT(url.GetProtocol() == "https");
        UT_ASSERT(url.GetIP() == "www.google.com");
        UT_ASSERT(url.GetPortNum().Value() == 42);

        UT_ASSERT(url.GetGetArgs().GetElemCount() == 2);
        const XString* const a = url.GetGetArgs().Find("a");
        const XString* const foo = url.GetGetArgs().Find("foo");
        UT_ASSERT(a);
        UT_ASSERT(foo);
        UT_ASSERT(*a == "27");
        UT_ASSERT(*foo == "bar boo");

        UT_ASSERT(url.GetURI() == uri);
        UT_ASSERT(url.GetFullRawURL() == "https://www.google.com:42/hello/world?a=27&foo=bar%20boo");
    }
    {
        const URI uri = URI("hello/world?foo=bar%20boo&a=27");
        const URL url = URL("https", "www.google.com", XNullable<int>(), uri);
        UT_ASSERT(url.GetProtocol() == "https");
        UT_ASSERT(url.GetIP() == "www.google.com");
        UT_ASSERT(url.GetPortNum().IsNull());

        UT_ASSERT(url.GetGetArgs().GetElemCount() == 2);
        const XString* const a = url.GetGetArgs().Find("a");
        const XString* const foo = url.GetGetArgs().Find("foo");
        UT_ASSERT(a);
        UT_ASSERT(foo);
        UT_ASSERT(*a == "27");
        UT_ASSERT(*foo == "bar boo");

        UT_ASSERT(url.GetURI() == uri);
        UT_ASSERT(url.GetFullRawURL() == "https://www.google.com/hello/world?a=27&foo=bar%20boo");
    }

    const URI longNastyURI = XString("/system/datasources/uuid:9487e434-1c1e-4422-a42c-08e3e7b6c3e1%2curn:") +
                             XString("pelco-com:serviceId:VideoOutput-1%2fmedia%2fserver%2fstreamInterfaces?") +
                             XString("sourceId=uuid:9487e434-1c1e-4422-a42c-08e3e7b6c3e1%2curn:pelco-com:serviceId") +
                             XString(":VideoOutput-1&userId=Cory%20Maddox&version=1.2&some%2cthing=blah%20bleh+bloke");
    {
        const URL url = URL("blah", "fc00:1:2:10:8a51:fbff:fec6:d992", 57342, longNastyURI);
        UT_ASSERT(url.GetProtocol() == "blah");
        UT_ASSERT(url.GetIP() == "fc00:1:2:10:8a51:fbff:fec6:d992");
        UT_ASSERT(url.GetPortNum().Value() == 57342);

        UT_ASSERT(url.GetGetArgs().GetElemCount() == 4);
        const XString* const something = url.GetGetArgs().Find("some,thing");
        const XString* const sourceID = url.GetGetArgs().Find("sourceId");
        const XString* const userID = url.GetGetArgs().Find("userId");
        const XString* const version = url.GetGetArgs().Find("version");
        UT_ASSERT(something);
        UT_ASSERT(sourceID);
        UT_ASSERT(userID);
        UT_ASSERT(version);
        UT_ASSERT(*something == "blah bleh bloke");
        UT_ASSERT(*sourceID == "uuid:9487e434-1c1e-4422-a42c-08e3e7b6c3e1,urn:pelco-com:serviceId:VideoOutput-1");
        UT_ASSERT(*userID == "Cory Maddox");
        UT_ASSERT(*version == "1.2");

        UT_ASSERT(url.GetURI() == longNastyURI);
        UT_ASSERT(url.GetFullRawURL() == XString("blah://[fc00:1:2:10:8a51:fbff:fec6:d992]:57342/") +
                                              XString("system/datasources/uuid:9487e434-1c1e-4422-a42c-08e3e7b6c3e1%2curn:") +
                                              XString("pelco-com:serviceId:VideoOutput-1%2fmedia%2fserver%2fstreamInterfaces?") +
                                              XString("some%2Cthing=blah%20bleh%20bloke&sourceId=uuid%3A9487e434-1c1e-4422-a42c-08e3e7b6c3e1") +
                                              XString("%2Curn%3Apelco-com%3AserviceId%3AVideoOutput-1&userId=Cory%20Maddox&version=1.2"));
    }
    {
        const URL url = URL("blah", "fc00:1:2:10:8a51:fbff:fec6:d992", XNullable<int>(), longNastyURI);
        UT_ASSERT(url.GetProtocol() == "blah");
        UT_ASSERT(url.GetIP() == "fc00:1:2:10:8a51:fbff:fec6:d992");
        UT_ASSERT(url.GetPortNum().IsNull());

        UT_ASSERT(url.GetGetArgs().GetElemCount() == 4);
        const XString* const something = url.GetGetArgs().Find("some,thing");
        const XString* const sourceID = url.GetGetArgs().Find("sourceId");
        const XString* const userID = url.GetGetArgs().Find("userId");
        const XString* const version = url.GetGetArgs().Find("version");
        UT_ASSERT(something);
        UT_ASSERT(sourceID);
        UT_ASSERT(userID);
        UT_ASSERT(version);
        UT_ASSERT(*something == "blah bleh bloke");
        UT_ASSERT(*sourceID == "uuid:9487e434-1c1e-4422-a42c-08e3e7b6c3e1,urn:pelco-com:serviceId:VideoOutput-1");
        UT_ASSERT(*userID == "Cory Maddox");
        UT_ASSERT(*version == "1.2");

        UT_ASSERT(url.GetURI() == longNastyURI);
        UT_ASSERT(url.GetFullRawURL() == XString("blah://[fc00:1:2:10:8a51:fbff:fec6:d992]/") +
                                              XString("system/datasources/uuid:9487e434-1c1e-4422-a42c-08e3e7b6c3e1%2curn:") +
                                              XString("pelco-com:serviceId:VideoOutput-1%2fmedia%2fserver%2fstreamInterfaces?") +
                                              XString("some%2Cthing=blah%20bleh%20bloke&sourceId=uuid%3A9487e434-1c1e-4422-a42c-08e3e7b6c3e1") +
                                              XString("%2Curn%3Apelco-com%3AserviceId%3AVideoOutput-1&userId=Cory%20Maddox&version=1.2"));
    }
}


void URLTest::Test3ArgConstructor()
{
    UT_ASSERT_THROWS(URL("", "", URI("")), WebbyException);
    UT_ASSERT_THROWS(URL("", "", URI("hello")), WebbyException);
    UT_ASSERT_THROWS(URL("", "127.0.0.1", URI("hello")), WebbyException);

    {
        const URL url = URL("file", "", URI(""));
        UT_ASSERT(url.GetProtocol() == "file");
        UT_ASSERT(url.GetIP().empty());
        UT_ASSERT(url.GetPortNum().IsNull());
        UT_ASSERT(url.GetGetArgs().Empty());
        UT_ASSERT(url.GetURI() == URI());
        UT_ASSERT(url.GetFullRawURL() == "file:///");
    }
    {
        const URI uri = "/etc/fstab";
        const URL url = URL("file", "", uri);
        UT_ASSERT(url.GetProtocol() == "file");
        UT_ASSERT(url.GetIP().empty());
        UT_ASSERT(url.GetPortNum().IsNull());
        UT_ASSERT(url.GetGetArgs().Empty());
        UT_ASSERT(url.GetURI() == uri);
        UT_ASSERT(url.GetFullRawURL() == "file:///etc/fstab");
    }
    {
        const URI uri = URI("hello");
        const URL url = URL("http", "127.0.0.1", uri);
        UT_ASSERT(url.GetProtocol() == "http");
        UT_ASSERT(url.GetIP() == "127.0.0.1");
        UT_ASSERT(url.GetPortNum().IsNull());
        UT_ASSERT(url.GetGetArgs().Empty());
        UT_ASSERT(url.GetURI() == uri);
        UT_ASSERT(url.GetFullRawURL() == "http://127.0.0.1/hello");
    }
    {
        const URI uri = URI("hello/world?foo=bar%20boo&a=27");
        const URL url = URL("https", "www.google.com", uri);
        UT_ASSERT(url.GetProtocol() == "https");
        UT_ASSERT(url.GetIP() == "www.google.com");
        UT_ASSERT(url.GetPortNum().IsNull());

        UT_ASSERT(url.GetGetArgs().GetElemCount() == 2);
        const XString* const a = url.GetGetArgs().Find("a");
        const XString* const foo = url.GetGetArgs().Find("foo");
        UT_ASSERT(a);
        UT_ASSERT(foo);
        UT_ASSERT(*a == "27");
        UT_ASSERT(*foo == "bar boo");

        UT_ASSERT(url.GetURI() == uri);
        UT_ASSERT(url.GetFullRawURL() == "https://www.google.com/hello/world?a=27&foo=bar%20boo");
    }

    const URI longNastyURI = XString("/system/datasources/uuid:9487e434-1c1e-4422-a42c-08e3e7b6c3e1%2curn:") +
                             XString("pelco-com:serviceId:VideoOutput-1%2fmedia%2fserver%2fstreamInterfaces?") +
                             XString("sourceId=uuid:9487e434-1c1e-4422-a42c-08e3e7b6c3e1%2curn:pelco-com:serviceId") +
                             XString(":VideoOutput-1&userId=Cory%20Maddox&version=1.2&some%2cthing=blah%20bleh+bloke");
    {
        const URL url = URL("blah", "fc00:1:2:10:8a51:fbff:fec6:d992", longNastyURI);
        UT_ASSERT(url.GetProtocol() == "blah");
        UT_ASSERT(url.GetIP() == "fc00:1:2:10:8a51:fbff:fec6:d992");
        UT_ASSERT(url.GetPortNum().IsNull());

        UT_ASSERT(url.GetGetArgs().GetElemCount() == 4);
        const XString* const something = url.GetGetArgs().Find("some,thing");
        const XString* const sourceID = url.GetGetArgs().Find("sourceId");
        const XString* const userID = url.GetGetArgs().Find("userId");
        const XString* const version = url.GetGetArgs().Find("version");
        UT_ASSERT(something);
        UT_ASSERT(sourceID);
        UT_ASSERT(userID);
        UT_ASSERT(version);
        UT_ASSERT(*something == "blah bleh bloke");
        UT_ASSERT(*sourceID == "uuid:9487e434-1c1e-4422-a42c-08e3e7b6c3e1,urn:pelco-com:serviceId:VideoOutput-1");
        UT_ASSERT(*userID == "Cory Maddox");
        UT_ASSERT(*version == "1.2");

        UT_ASSERT(url.GetURI() == longNastyURI);
        UT_ASSERT(url.GetFullRawURL() == XString("blah://[fc00:1:2:10:8a51:fbff:fec6:d992]/") +
                                              XString("system/datasources/uuid:9487e434-1c1e-4422-a42c-08e3e7b6c3e1%2curn:") +
                                              XString("pelco-com:serviceId:VideoOutput-1%2fmedia%2fserver%2fstreamInterfaces?") +
                                              XString("some%2Cthing=blah%20bleh%20bloke&sourceId=uuid%3A9487e434-1c1e-4422-a42c-08e3e7b6c3e1") +
                                              XString("%2Curn%3Apelco-com%3AserviceId%3AVideoOutput-1&userId=Cory%20Maddox&version=1.2"));
    }
}


void URLTest::TestStringConstructor()
{
    UT_ASSERT_THROWS(URL(""), WebbyException);
    UT_ASSERT_THROWS(URL("hello"), WebbyException);
    UT_ASSERT_THROWS(URL("http"), WebbyException);
    UT_ASSERT_THROWS(URL("http:"), WebbyException);
    UT_ASSERT_THROWS(URL("127.0.0.1"), WebbyException);
    UT_ASSERT_THROWS(URL("127.0.0.1/hello"), WebbyException);

    {
        const URL url = URL("http://");
        UT_ASSERT(url.GetProtocol() == "http");
        UT_ASSERT(url.GetIP().empty());
        UT_ASSERT(url.GetPortNum() == XNullable<int>());
        UT_ASSERT(url.GetGetArgs().Empty());
        UT_ASSERT(url.GetURI() == URI());
        UT_ASSERT(url.GetFullRawURL() == "http:///");
    }
    {
        const URL url = URL("file:///etc/fstab");
        UT_ASSERT(url.GetProtocol() == "file");
        UT_ASSERT(url.GetIP().empty());
        UT_ASSERT(url.GetPortNum() == XNullable<int>());
        UT_ASSERT(url.GetGetArgs().Empty());
        UT_ASSERT(url.GetURI() == URI("/etc/fstab"));
        UT_ASSERT(url.GetFullRawURL() == "file:///etc/fstab");
    }
    {
        const URL url = URL("file://etc/fstab");
        UT_ASSERT(url.GetProtocol() == "file");
        UT_ASSERT(url.GetIP() == "etc");
        UT_ASSERT(url.GetPortNum() == XNullable<int>());
        UT_ASSERT(url.GetGetArgs().Empty());
        UT_ASSERT(url.GetURI() == URI("fstab"));
        UT_ASSERT(url.GetFullRawURL() == "file://etc/fstab");
    }
    {
        const URL url = URL("http://127.0.0.1:0/hello");
        UT_ASSERT(url.GetProtocol() == "http");
        UT_ASSERT(url.GetIP() == "127.0.0.1");
        UT_ASSERT(url.GetPortNum().Value() == 0);
        UT_ASSERT(url.GetGetArgs().Empty());
        UT_ASSERT(url.GetURI() == URI("hello"));
        UT_ASSERT(url.GetFullRawURL() == "http://127.0.0.1:0/hello");
    }
    {
        const URL url = URL("http://127.0.0.1/hello");
        UT_ASSERT(url.GetProtocol() == "http");
        UT_ASSERT(url.GetIP() == "127.0.0.1");
        UT_ASSERT(url.GetPortNum().IsNull());
        UT_ASSERT(url.GetGetArgs().Empty());
        UT_ASSERT(url.GetURI() == URI("hello"));
        UT_ASSERT(url.GetFullRawURL() == "http://127.0.0.1/hello");
    }

    {
        const URL url = URL("https://www.google.com/hello/world?foo=bar%20boo&a=27");
        UT_ASSERT(url.GetProtocol() == "https");
        UT_ASSERT(url.GetIP() == "www.google.com");
        UT_ASSERT(url.GetPortNum().IsNull());

        UT_ASSERT(url.GetGetArgs().GetElemCount() == 2);
        const XString* const a = url.GetGetArgs().Find("a");
        const XString* const foo = url.GetGetArgs().Find("foo");
        UT_ASSERT(a);
        UT_ASSERT(foo);
        UT_ASSERT(*a == "27");
        UT_ASSERT(*foo == "bar boo");

        UT_ASSERT(url.GetURI() == URI("/hello/world?foo=bar%20boo&a=27"));
        UT_ASSERT(url.GetFullRawURL() == "https://www.google.com/hello/world?a=27&foo=bar%20boo");
    }

    const URI longNastyURI = URI(XString("/system/datasources/uuid:9487e434-1c1e-4422-a42c-08e3e7b6c3e1%2curn:") +
                                 XString("pelco-com:serviceId:VideoOutput-1%2fmedia%2fserver%2fstreamInterfaces?") +
                                 XString("sourceId=uuid:9487e434-1c1e-4422-a42c-08e3e7b6c3e1%2curn:pelco-com:serviceId") +
                                 XString(":VideoOutput-1&userId=Cory%20Maddox&version=1.2&some%2cthing=blah%20bleh+bloke"));
    {
        const URL url = XString("blah://10.221.218.75:57342") + longNastyURI.GetFullRawURI();
        UT_ASSERT(url.GetProtocol() == "blah");
        UT_ASSERT(url.GetIP() == "10.221.218.75");
        UT_ASSERT(url.GetPortNum().Value() == 57342);

        UT_ASSERT(url.GetGetArgs().GetElemCount() == 4);
        const XString* const something = url.GetGetArgs().Find("some,thing");
        const XString* const sourceID = url.GetGetArgs().Find("sourceId");
        const XString* const userID = url.GetGetArgs().Find("userId");
        const XString* const version = url.GetGetArgs().Find("version");
        UT_ASSERT(something);
        UT_ASSERT(sourceID);
        UT_ASSERT(userID);
        UT_ASSERT(version);
        UT_ASSERT(*something == "blah bleh bloke");
        UT_ASSERT(*sourceID == "uuid:9487e434-1c1e-4422-a42c-08e3e7b6c3e1,urn:pelco-com:serviceId:VideoOutput-1");
        UT_ASSERT(*userID == "Cory Maddox");
        UT_ASSERT(*version == "1.2");

        UT_ASSERT(url.GetURI() == longNastyURI);
        UT_ASSERT(url.GetFullRawURL() == XString("blah://10.221.218.75:57342/") +
                                              XString("system/datasources/uuid:9487e434-1c1e-4422-a42c-08e3e7b6c3e1%2curn:") +
                                              XString("pelco-com:serviceId:VideoOutput-1%2fmedia%2fserver%2fstreamInterfaces?") +
                                              XString("some%2Cthing=blah%20bleh%20bloke&sourceId=uuid%3A9487e434-1c1e-4422-a42c-08e3e7b6c3e1") +
                                              XString("%2Curn%3Apelco-com%3AserviceId%3AVideoOutput-1&userId=Cory%20Maddox&version=1.2"));
    }
    {
        const URL url = XString("blah://10.221.218.75") + longNastyURI.GetFullRawURI();
        UT_ASSERT(url.GetProtocol() == "blah");
        UT_ASSERT(url.GetIP() == "10.221.218.75");
        UT_ASSERT(url.GetPortNum().IsNull());

        UT_ASSERT(url.GetGetArgs().GetElemCount() == 4);
        const XString* const something = url.GetGetArgs().Find("some,thing");
        const XString* const sourceID = url.GetGetArgs().Find("sourceId");
        const XString* const userID = url.GetGetArgs().Find("userId");
        const XString* const version = url.GetGetArgs().Find("version");
        UT_ASSERT(something);
        UT_ASSERT(sourceID);
        UT_ASSERT(userID);
        UT_ASSERT(version);
        UT_ASSERT(*something == "blah bleh bloke");
        UT_ASSERT(*sourceID == "uuid:9487e434-1c1e-4422-a42c-08e3e7b6c3e1,urn:pelco-com:serviceId:VideoOutput-1");
        UT_ASSERT(*userID == "Cory Maddox");
        UT_ASSERT(*version == "1.2");

        UT_ASSERT(url.GetURI() == longNastyURI);
        UT_ASSERT(url.GetFullRawURL() == XString("blah://10.221.218.75/") +
                                              XString("system/datasources/uuid:9487e434-1c1e-4422-a42c-08e3e7b6c3e1%2curn:") +
                                              XString("pelco-com:serviceId:VideoOutput-1%2fmedia%2fserver%2fstreamInterfaces?") +
                                              XString("some%2Cthing=blah%20bleh%20bloke&sourceId=uuid%3A9487e434-1c1e-4422-a42c-08e3e7b6c3e1") +
                                              XString("%2Curn%3Apelco-com%3AserviceId%3AVideoOutput-1&userId=Cory%20Maddox&version=1.2"));
    }
    {
        const URL url = XString("blah://[fc00:1:2:10:8a51:fbff:fec6:d992]:57342") + longNastyURI.GetFullRawURI();
        UT_ASSERT(url.GetProtocol() == "blah");
        UT_ASSERT(url.GetIP() == "fc00:1:2:10:8a51:fbff:fec6:d992");
        UT_ASSERT(url.GetPortNum().Value() == 57342);

        UT_ASSERT(url.GetGetArgs().GetElemCount() == 4);
        const XString* const something = url.GetGetArgs().Find("some,thing");
        const XString* const sourceID = url.GetGetArgs().Find("sourceId");
        const XString* const userID = url.GetGetArgs().Find("userId");
        const XString* const version = url.GetGetArgs().Find("version");
        UT_ASSERT(something);
        UT_ASSERT(sourceID);
        UT_ASSERT(userID);
        UT_ASSERT(version);
        UT_ASSERT(*something == "blah bleh bloke");
        UT_ASSERT(*sourceID == "uuid:9487e434-1c1e-4422-a42c-08e3e7b6c3e1,urn:pelco-com:serviceId:VideoOutput-1");
        UT_ASSERT(*userID == "Cory Maddox");
        UT_ASSERT(*version == "1.2");

        UT_ASSERT(url.GetURI() == longNastyURI);
        UT_ASSERT(url.GetFullRawURL() == XString("blah://[fc00:1:2:10:8a51:fbff:fec6:d992]:57342/") +
                                              XString("system/datasources/uuid:9487e434-1c1e-4422-a42c-08e3e7b6c3e1%2curn:") +
                                              XString("pelco-com:serviceId:VideoOutput-1%2fmedia%2fserver%2fstreamInterfaces?") +
                                              XString("some%2Cthing=blah%20bleh%20bloke&sourceId=uuid%3A9487e434-1c1e-4422-a42c-08e3e7b6c3e1") +
                                              XString("%2Curn%3Apelco-com%3AserviceId%3AVideoOutput-1&userId=Cory%20Maddox&version=1.2"));
    }
    {
        const URL url = XString("blah://[fc00:1:2:10:8a51:fbff:fec6:d992]") + longNastyURI.GetFullRawURI();
        UT_ASSERT(url.GetProtocol() == "blah");
        UT_ASSERT(url.GetIP() == "fc00:1:2:10:8a51:fbff:fec6:d992");
        UT_ASSERT(url.GetPortNum().IsNull());

        UT_ASSERT(url.GetGetArgs().GetElemCount() == 4);
        const XString* const something = url.GetGetArgs().Find("some,thing");
        const XString* const sourceID = url.GetGetArgs().Find("sourceId");
        const XString* const userID = url.GetGetArgs().Find("userId");
        const XString* const version = url.GetGetArgs().Find("version");
        UT_ASSERT(something);
        UT_ASSERT(sourceID);
        UT_ASSERT(userID);
        UT_ASSERT(version);
        UT_ASSERT(*something == "blah bleh bloke");
        UT_ASSERT(*sourceID == "uuid:9487e434-1c1e-4422-a42c-08e3e7b6c3e1,urn:pelco-com:serviceId:VideoOutput-1");
        UT_ASSERT(*userID == "Cory Maddox");
        UT_ASSERT(*version == "1.2");

        UT_ASSERT(url.GetURI() == longNastyURI);
        UT_ASSERT(url.GetFullRawURL() == XString("blah://[fc00:1:2:10:8a51:fbff:fec6:d992]/") +
                                              XString("system/datasources/uuid:9487e434-1c1e-4422-a42c-08e3e7b6c3e1%2curn:") +
                                              XString("pelco-com:serviceId:VideoOutput-1%2fmedia%2fserver%2fstreamInterfaces?") +
                                              XString("some%2Cthing=blah%20bleh%20bloke&sourceId=uuid%3A9487e434-1c1e-4422-a42c-08e3e7b6c3e1") +
                                              XString("%2Curn%3Apelco-com%3AserviceId%3AVideoOutput-1&userId=Cory%20Maddox&version=1.2"));
    }
}


void URLTest::TestEquality()
{
    UT_ASSERT( URL() == URL());
    UT_ASSERT( URL("http", "foo", "boo") == URL("http", "foo", "boo"));
    UT_ASSERT(!(URL("http", "foo", "boo") == URL("http", "foo", "bar")));
    UT_ASSERT(!(URL("http", "fol", "boo") == URL("http", "foo", "boo")));
    UT_ASSERT(!(URL("https", "foo", "boo") == URL("http", "foo", "boo")));

    UT_ASSERT( URL("http", "foo", 42, "boo") == URL("http", "foo", 42, "boo"));
    UT_ASSERT(!(URL("http", "foo", 42, "boo") == URL("http", "foo", 42, "bar")));
    UT_ASSERT(!(URL("http", "fol", 42, "boo") == URL("http", "foo", 42, "boo")));
    UT_ASSERT(!(URL("https", "foo", 42, "boo") == URL("http", "foo", 42, "boo")));
    UT_ASSERT(!(URL("http", "foo", 42, "boo") == URL("http", "foo", 43, "boo")));
    UT_ASSERT(!(URL("http", "foo", 42, "boo") == URL("http", "foo", "boo")));

    UT_ASSERT(!(URL() != URL()));
    UT_ASSERT(!(URL("http", "foo", "boo") != URL("http", "foo", "boo")));
    UT_ASSERT( URL("http", "foo", "boo") != URL("http", "foo", "bar"));
    UT_ASSERT( URL("http", "fol", "boo") != URL("http", "foo", "boo"));
    UT_ASSERT( URL("https", "foo", "boo") != URL("http", "foo", "boo"));

    UT_ASSERT(!(URL("http", "foo", 42, "boo") != URL("http", "foo", 42, "boo")));
    UT_ASSERT( URL("http", "foo", 42, "boo") != URL("http", "foo", 42, "bar"));
    UT_ASSERT( URL("http", "fol", 42, "boo") != URL("http", "foo", 42, "boo"));
    UT_ASSERT( URL("https", "foo", 42, "boo") != URL("http", "foo", 42, "boo"));
    UT_ASSERT( URL("http", "foo", 42, "boo") != URL("http", "foo", 43, "boo"));
    UT_ASSERT( URL("http", "foo", 42, "boo") != URL("http", "foo", "boo"));
}


void URLTest::TestAddGetArgs()
{
    {
        URL url = URL("http://127.0.0.1/hello/world");
        UT_ASSERT(url.GetFullRawURL() == "http://127.0.0.1/hello/world");
        url.AddGetArg("foo bar", "doodly");
        UT_ASSERT(url.GetFullRawURL() == "http://127.0.0.1/hello/world?foo%20bar=doodly");
        url.AddGetArg("aaa", "car/bleh\nblah?");
        UT_ASSERT(url.GetFullRawURL() == "http://127.0.0.1/hello/world?aaa=car%2Fbleh%0Ablah%3F&foo%20bar=doodly");
    }

    {
        URL url = URL("rtsp://[fe80::2677:3ff:feec:d748]:22/nowhere?igor=left");
        UT_ASSERT(url.GetFullRawURL() == "rtsp://[fe80::2677:3ff:feec:d748]:22/nowhere?igor=left");
        url.AddGetArg("a", "b");
        UT_ASSERT(url.GetFullRawURL() == "rtsp://[fe80::2677:3ff:feec:d748]:22/nowhere?a=b&igor=left");
        url.AddGetArg("z", "w");
        UT_ASSERT(url.GetFullRawURL() == "rtsp://[fe80::2677:3ff:feec:d748]:22/nowhere?a=b&igor=left&z=w");
    }
}


void URLTest::TestClearGetArgs()
{
    {
        URL url;
        url.ClearGetArgs();
        UT_ASSERT(url.GetGetArgs().Empty());
    }

    {
        URL url = "rtsp://[fe80::2677:3ff:feec:d748]:22/nowhere?a=b&igor=left&z=w";
        url.ClearGetArgs();
        UT_ASSERT(url.GetGetArgs().Empty());
    }
}


void URLTest::TestIsValidProtocol()
{
    UT_ASSERT(!URL::IsValidProtocol(""));
    UT_ASSERT(!URL::IsValidProtocol(":"));
    UT_ASSERT(!URL::IsValidProtocol("."));
    UT_ASSERT(!URL::IsValidProtocol("+"));
    UT_ASSERT(!URL::IsValidProtocol("-"));
    UT_ASSERT(!URL::IsValidProtocol("0"));
    UT_ASSERT(!URL::IsValidProtocol(".:"));
    UT_ASSERT(!URL::IsValidProtocol("+:"));
    UT_ASSERT(!URL::IsValidProtocol("-:"));
    UT_ASSERT(!URL::IsValidProtocol("0:"));

    UT_ASSERT( URL::IsValidProtocol("a"));
    UT_ASSERT(!URL::IsValidProtocol("a:"));
    UT_ASSERT(!URL::IsValidProtocol(":a"));
    UT_ASSERT(!URL::IsValidProtocol("a:a"));
    UT_ASSERT(!URL::IsValidProtocol("a+.-0:"));
    UT_ASSERT(!URL::IsValidProtocol("a+.-0:a"));
    UT_ASSERT( URL::IsValidProtocol("a+.-0"));

    UT_ASSERT(!URL::IsValidProtocol("http:"));
    UT_ASSERT( URL::IsValidProtocol("http"));
    UT_ASSERT( URL::IsValidProtocol("https"));
    UT_ASSERT( URL::IsValidProtocol("rtsp"));
    UT_ASSERT( URL::IsValidProtocol("abcdefghijklmnopqrstuvwxyz"));
    UT_ASSERT( URL::IsValidProtocol("ABCDEFGHIJKLMNOPQRSTUVWXYZ"));
    UT_ASSERT( URL::IsValidProtocol("a1234567890"));
}


void URLTest::TestMakeAbsolute()
{
    const URI longNastyURI = XString("/system/datasources/uuid:9487e434-1c1e-4422-a42c-08e3e7b6c3e1%2curn:") +
                             XString("pelco-com:serviceId:VideoOutput-1%2fmedia%2fserver%2fstreamInterfaces?") +
                             XString("sourceId=uuid:9487e434-1c1e-4422-a42c-08e3e7b6c3e1%2curn:pelco-com:serviceId") +
                             XString(":VideoOutput-1&userId=Cory%20Maddox&version=1.2&some%2cthing=blah%20bleh+bloke");

    UT_ASSERT(URL::MakeAbsolute("a", "b", 0, "c").GetFullRawURL() == "a://b:0/c");
    UT_ASSERT(URL::MakeAbsolute("rtsp", "127.0.0.1", 42, "hello/blah/%20%2F5").GetFullRawURL() ==
                   "rtsp://127.0.0.1:42/hello/blah/%20%2F5");
    UT_ASSERT(URL::MakeAbsolute("rtsp", "www.google.com", 99, "hello/blah/%20%2F5").GetFullRawURL() ==
                   "rtsp://www.google.com:99/hello/blah/%20%2F5");
    UT_ASSERT(URL::MakeAbsolute("http", "webmail.pelco.org", 80, "").GetFullRawURL() ==
                   "http://webmail.pelco.org:80/");
    UT_ASSERT(URL::MakeAbsolute("blah", "fc00:1:2:10:8a51:fbff:fec6:d992", 57342, longNastyURI.GetFullRawURI()) ==
                   URL("blah", "fc00:1:2:10:8a51:fbff:fec6:d992", 57342, longNastyURI));

    UT_ASSERT(URL::MakeAbsolute("rtsp", "127.0.0.1", 42, "http://foo.bar/hello?a=7").GetFullRawURL() ==
                   "http://foo.bar/hello?a=7");
    UT_ASSERT(URL::MakeAbsolute("rtsp", "www.google.com", 99, "rtsp://127.0.0.1").GetFullRawURL() ==
                   "rtsp://127.0.0.1/");
    UT_ASSERT(URL::MakeAbsolute("http", "webmail.pelco.org", 80, "https://10.221.218.75").GetFullRawURL()
                   == "https://10.221.218.75/");
    {
        const URL url = URL("foo", "127.0.0.1", 22, longNastyURI);
        UT_ASSERT(URL::MakeAbsolute("blah", "fc00:1:2:10:8a51:fbff:fec6:d992", 57342, url.GetFullRawURL()) == url);
    }

    UT_ASSERT(URL::MakeAbsolute("a", "b", "c").GetFullRawURL() == "a://b/c");
    UT_ASSERT(URL::MakeAbsolute("rtsp", "127.0.0.1", "hello/blah/%20%2F5").GetFullRawURL() ==
                   "rtsp://127.0.0.1/hello/blah/%20%2F5");
    UT_ASSERT(URL::MakeAbsolute("rtsp", "www.google.com", "hello/blah/%20%2F5").GetFullRawURL() ==
                   "rtsp://www.google.com/hello/blah/%20%2F5");
    UT_ASSERT(URL::MakeAbsolute("http", "webmail.pelco.org", "").GetFullRawURL() ==
                   "http://webmail.pelco.org/");
    UT_ASSERT(URL::MakeAbsolute("blah", "fc00:1:2:10:8a51:fbff:fec6:d992", longNastyURI.GetFullRawURI()) ==
                   URL("blah", "fc00:1:2:10:8a51:fbff:fec6:d992", longNastyURI));

    UT_ASSERT(URL::MakeAbsolute("rtsp", "127.0.0.1", "http://foo.bar/hello?a=7").GetFullRawURL() ==
                   "http://foo.bar/hello?a=7");
    UT_ASSERT(URL::MakeAbsolute("rtsp", "www.google.com", "rtsp://127.0.0.1").GetFullRawURL() ==
                   "rtsp://127.0.0.1/");
    UT_ASSERT(URL::MakeAbsolute("http", "webmail.pelco.org", "https://10.221.218.75").GetFullRawURL()
                   == "https://10.221.218.75/");
    {
        const URL url = URL("foo", "127.0.0.1", longNastyURI);
        UT_ASSERT(URL::MakeAbsolute("blah", "fc00:1:2:10:8a51:fbff:fec6:d992", url.GetFullRawURL()) == url);
    }
}
