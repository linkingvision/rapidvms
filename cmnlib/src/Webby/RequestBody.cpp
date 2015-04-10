
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "Webby/RequestBody.h"

using namespace WEBBY;
using namespace XSDK;

RequestBody::RequestBody( ServerSideRequest& request, XRef<XStreamIO> clientSok )
{
}

RequestBody::RequestBody( const RequestBody& obj )
{
}

RequestBody::~RequestBody() throw()
{
}

RequestBody& RequestBody::operator = ( const RequestBody& obj )
{
    return *this;
}
