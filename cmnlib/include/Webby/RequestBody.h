
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef Webby_RequestBody_h
#define Webby_RequestBody_h

#include "XSDK/XStreamIO.h"
#include "XSDK/XBaseObject.h"

#include "Webby/ServerSideRequest.h"

namespace WEBBY
{

class RequestBody : public XSDK::XBaseObject
{
 public:
    X_API RequestBody( WEBBY::ServerSideRequest& request, XRef<XSDK::XStreamIO> clientSok );

    X_API RequestBody( const RequestBody& obj );

    X_API virtual ~RequestBody() throw();

    X_API RequestBody& operator = ( const RequestBody& obj );
};

};

#endif
