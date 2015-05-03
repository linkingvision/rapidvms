
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef _Webby_ServerSideRequest_h
#define _Webby_ServerSideRequest_h

#include <list>

#include "XSDK/XStreamIO.h"
#include "XSDK/XBaseObject.h"
#include "XSDK/XHash.h"
#include "XSDK/XString.h"
#include "XSDK/XMemory.h"

#include "Webby/URI.h"

namespace WEBBY
{

class ServerSideRequest : public XSDK::XBaseObject
{
public:
    X_API ServerSideRequest();

    X_API ServerSideRequest(const ServerSideRequest& rhs);

    X_API virtual ~ServerSideRequest() throw();

    X_API ServerSideRequest& operator = (const ServerSideRequest& rhs);

    X_API void ReadRequest(XRef<XSDK::XStreamIO> socket);

    X_API XSDK::XString GetMethod() const;

    X_API URI GetURI() const;

    X_API XSDK::XString GetContentType() const;

    X_API void SetInitialLine(const XSDK::XString& initialLine);

    X_API void SetContentType(const XSDK::XString& type);

    X_API void SetHeader(const XSDK::XString& name, const XSDK::XString& value);

    X_API XSDK::XString* GetHeader(XSDK::XString key) const;
    X_API XSDK::XHash<XSDK::XString> GetHeaders() const;

    X_API const void *GetBody() const;

    X_API size_t GetBodySize() const;

    X_API XSDK::XString GetBodyAsString() const;

    X_API XSDK::XHash<XSDK::XString>& GetPostVars();

    X_API void DebugPrintRequest();

    X_API bool IsPatchRequest();
    X_API bool IsPutRequest();
    X_API bool IsPostRequest();
    X_API bool IsGetRequest();
    X_API bool IsDeleteRequest();

private:

    bool _ReceiveData(XRef<XSDK::XStreamIO> socket, void* data, size_t dataLen);
    void _CleanSocket(XRef<XSDK::XStreamIO> socket, char** writer);
    void _ReadHeaderLine(XRef<XSDK::XStreamIO> socket, char* writer, bool firstLine);
    bool _AddLine(std::list<XSDK::XString>& lines, const XSDK::XString& line);
    void _ProcessRequestLines(const std::list<XSDK::XString>& requestLines);
    void _ProcessBody(XRef<XSDK::XStreamIO> socket);


    XSDK::XString _initialLine;
    XSDK::XHash<XSDK::XString> _headerParts;
    XSDK::XHash<XSDK::XString> _postVars;
    XSDK::XMemory _body;
    XSDK::XString _contentType;
};

}

#endif
