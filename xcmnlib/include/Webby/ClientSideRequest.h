
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef _Webby_ClientSideRequest_h
#define _Webby_ClientSideRequest_h

#include "XSDK/XStreamIO.h"
#include "XSDK/XHash.h"
#include "XSDK/XString.h"
#include "XSDK/XMemory.h"
#include "Webby/URI.h"
#include "Webby/SoapArgs.h"

namespace WEBBY
{

const XSDK::XString kWebbyGETRequest = "GET";
const XSDK::XString kWebbyPOSTRequest = "POST";
const XSDK::XString kWebbyPUTRequest = "PUT";
const XSDK::XString kWebbyDELETERequest = "DELETE";
const XSDK::XString kWebbyPATCHRequest = "PATCH";

class ClientSideRequest : public XSDK::XBaseObject
{

public:
    X_API ClientSideRequest();

    X_API virtual ~ClientSideRequest() throw();

    X_API ClientSideRequest( const ClientSideRequest& rhs );

    X_API ClientSideRequest& operator =( const ClientSideRequest& rhs );

    X_API void SetAcceptType( const XSDK::XString& acceptType );

    X_API void SetContentType( const XSDK::XString& contentType );

    X_API void SetRequestType( const XSDK::XString& requestType );

    X_API void SetURI( const WEBBY::URI& uri );

    X_API URI GetURI() const;

    X_API bool WriteRequest( XRef<XSDK::XStreamIO> socket ) const;

    X_API XSDK::XString GetHeaderAsString(const XRef<XSDK::XStreamIO> socket) const;

    X_API XSDK::XString GetBodyAsString() const;

    X_API void SetBasicAuthentication( const XSDK::XString& user,
                       const XSDK::XString& pass );

    X_API void AddPostVar( const XSDK::XString& name, const XSDK::XString& value );

    X_API void SetBody( const unsigned char* requestBody, size_t size );

    X_API void AddHeader( const XSDK::XString& name, const XSDK::XString& value );

    X_API XSDK::XString GetHeader( const XSDK::XString& name ) const;

    X_API void SetHeaderParts( const XSDK::XHash<XSDK::XString>& headerParts );

    X_API void GetHeaderParts( XSDK::XHash<XSDK::XString>& headerParts );

    X_API void DebugPrintRequest();

    /// \brief Creates a soap request.
    ///
    /// \param header The arguments that go in the soap header.
    /// \param action The soap action that the request is trying to do.
    /// \param actionNS The namespace for the soap action.
    /// \param args The arguments that go in the soap body.
    X_API static ClientSideRequest CreateSoapRequest( const SoapArgs& header,
                                                      const XSDK::XString& action,
                                                      const XSDK::XString& actionNS,
                                                      const SoapArgs& args );

    /// \brief Creates a soap request.
    ///
    /// This version takes the arguments as a string (of XML) rather than as
    /// SoapArgs. The rest of the soap message is created for you, but it
    /// gives you the flexibility of creating and manipulating the arguments
    /// as XML if you want to.
    ///
    /// \param header The arguments that go in the soap header.
    /// \param action The soap action that the request is trying to do.
    /// \param actionNS The namespace for the soap action.
    /// \param args The arguments that go in the soap body (as XML).
    X_API static ClientSideRequest CreateSoapRequest( const SoapArgs& header,
                                                      const XSDK::XString& action,
                                                      const XSDK::XString& actionNS,
                                                      const XSDK::XString& args );

private:
    URI _uri;
    XSDK::XString _acceptType;
    XSDK::XString _contentType;
    XSDK::XString _requestType;
    XSDK::XString _authData;
    XSDK::XHash<XSDK::XString> _postVars;
    XSDK::XHash<XSDK::XString> _headerParts;
    XSDK::XMemory _body;
};

}

#endif
