
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "Webby/ClientSideRequest.h"
#include "Webby/WebbyException.h"
#include "Webby/WebbyUtils.h"
#include "XSDK/XSocket.h"
#include "XSDK/XString.h"

#include <iostream>

using namespace WEBBY;
using namespace XSDK;
using namespace std;

static const unsigned int MAX_HEADER_LINE = 2048;

ClientSideRequest::ClientSideRequest() :
    _uri(),
    _acceptType(),
    _contentType(),
    _requestType(kWebbyGETRequest),
    _authData(),
    _postVars(),
    _headerParts(),
    _body()
{
}

ClientSideRequest::ClientSideRequest(const ClientSideRequest& rhs) :
    _uri(rhs._uri),
    _acceptType(rhs._acceptType),
    _contentType(rhs._contentType),
    _requestType(rhs._requestType),
    _authData(rhs._authData),
    _postVars(rhs._postVars),
    _headerParts(rhs._headerParts),
    _body(rhs._body)
{
}

ClientSideRequest& ClientSideRequest::operator =(const ClientSideRequest& rhs)
{
    if (this != &rhs)
    {
        _uri = rhs._uri;
        _acceptType = rhs._acceptType;
        _contentType = rhs._contentType;
        _requestType = rhs._requestType;
        _authData = rhs._authData;
        _postVars = rhs._postVars;
        _headerParts = rhs._headerParts;
        _body = rhs._body;
    }

    return *this;
}

ClientSideRequest::~ClientSideRequest() throw()
{
}

void ClientSideRequest::SetAcceptType(const XString& acceptType)
{
    _acceptType = acceptType;
}

void ClientSideRequest::SetContentType(const XString& contentType)
{
    _contentType = contentType;
}

void ClientSideRequest::SetRequestType(const XString& requestType)
{
    if ((kWebbyGETRequest == requestType) ||
         (kWebbyPOSTRequest == requestType) ||
         (kWebbyPUTRequest == requestType) ||
         (kWebbyPATCHRequest == requestType) ||
         (kWebbyDELETERequest == requestType))
    {
        _requestType = requestType;
    }
    else
        X_STHROW(WebbyException, ("Request Type Does Not Exist."));
}

void ClientSideRequest::SetURI(const URI& uri)
{
    _uri = uri;
}

bool ClientSideRequest::WriteRequest(XRef<XStreamIO> socket) const
{
    XString msgHeader = GetHeaderAsString(socket);

    ssize_t bytesWritten = socket->Send(msgHeader.c_str(), msgHeader.length());
    if(!socket->Valid() || (bytesWritten != (int32_t)msgHeader.length()))
        return false;

    if((_requestType == kWebbyPOSTRequest || _requestType == kWebbyPATCHRequest || _requestType == kWebbyPUTRequest)  &&
        !_contentType.Contains("x-www-form-urlencoded") &&
        _body.GetDataSize())
    {
        bytesWritten = socket->Send(_body.Map(), _body.GetDataSize());
    if(!socket->Valid() || (bytesWritten != (int32_t)_body.GetDataSize()))
        return false;
    }

    return true;
}

XString ClientSideRequest::GetHeaderAsString(const XRef<XStreamIO> socket) const
{
    XString msgHeader;
    msgHeader = _requestType + " " + _uri.GetFullRawURI() + " HTTP/1.1\r\nHost: " + socket->GetHost() + ":" + XString::FromUInt32(socket->GetHostPort()) + "\r\n";

    if (!_acceptType.empty())
        msgHeader += "Accept: " + _acceptType + "\r\n";

    if (!_contentType.empty())
        msgHeader += "Content-Type: " + _contentType + "\r\n";

    if (!_authData.empty())
        msgHeader += "Authorization: Basic " + _authData + "\r\n";

    XHash<XString>::XHashIter iter = _headerParts.GetIterator();

    for(XHash<XString>::XHashIter iter =  _headerParts.GetIterator();
         iter.IterValid();
         iter.Iterate())
    {
        XString key = iter.IterKey();
        XString val = *iter.IterData();

        msgHeader += XString::Format("%s: %s\r\n", key.c_str(), val.c_str());
    }

    if(_requestType == kWebbyPOSTRequest || _requestType == kWebbyPATCHRequest || _requestType == kWebbyPUTRequest)
    {
        if(_contentType.Contains("x-www-form-urlencoded"))
        {
            // Post, with url-encoded nv pairs...

            XString body;

            XHash<XString>::XHashIter iter = _postVars.GetIterator();
            iter.IterStart();

        size_t elemCount = iter.GetElemCount() - 1;
            for(size_t i = 0; i < elemCount; i++)
            {
                body += XString::Format("%s=%s&", iter.IterKey().c_str(), iter.IterData()->c_str());

                iter.Iterate();
            }

            body += XString::Format("%s=%s", iter.IterKey().c_str(), iter.IterData()->c_str());

            msgHeader += XString::Format("Content-Length: %d\r\n", body.length());

            msgHeader += "\r\n";

            msgHeader += body;
        }
        else
        {
            // Post, with an unknown body (this is the SOAP case)...

            if(_body.GetDataSize())
            {
                // If we have a body, add a Content-Length and write it...

                msgHeader += XString::Format("Content-Length: %d\r\n", _body.GetDataSize());
                msgHeader += "\r\n";
            }
            else
            {
                // No body in this request, so just write this...

                msgHeader += "\r\n";
            }
        }
    }
    else
    {
        // This is the GET case...

        msgHeader += "\r\n";
    }

    return msgHeader;
}

XString ClientSideRequest::GetBodyAsString() const
{
    return XString((const char*)_body.Map(), _body.GetDataSize());
}

URI ClientSideRequest::GetURI() const
{
    return _uri;
}

void ClientSideRequest::SetBasicAuthentication(const XString& user, const XString& pass)
{
    const XString auth = XString::Format("%s:%s", user.c_str(), pass.c_str());

    _authData = XString::Base64Encode(auth.c_str(), auth.length());
}

void ClientSideRequest::AddPostVar(const XString& name, const XString& value)
{
    _contentType = "x-www-form-urlencoded";
    _postVars.Add(name, value);
}

void ClientSideRequest::SetBody(const unsigned char* requestBody, size_t size)
{
    if(requestBody == NULL || size == 0)
        X_STHROW(WebbyException, ("Unable to attach empty body to request."));

    _body.Clear();

    unsigned char* writer = &_body.Extend(size);

    memcpy(writer, requestBody, size);
}

void ClientSideRequest::AddHeader(const XSDK::XString& name, const XSDK::XString& value)
{
    _headerParts.Add(AdjustHeaderName(name), AdjustHeaderValue(value));
}

XString ClientSideRequest::GetHeader(const XSDK::XString& name) const
{
    XString* value = _headerParts.Find(name.ToLower());
    return ((value != NULL) ? *value : "");
}

void ClientSideRequest::SetHeaderParts(const XSDK::XHash<XSDK::XString>& headerParts)
{
    _headerParts = headerParts;
}

void ClientSideRequest::GetHeaderParts(XSDK::XHash<XSDK::XString>& headerParts)
{
    headerParts = _headerParts;
}

void ClientSideRequest::DebugPrintRequest()
{
    printf("method = %s\n", _requestType.c_str());
    printf("uri = %s\n", _uri.GetFullRawURI().c_str());
    fflush(stdout);
}

ClientSideRequest ClientSideRequest::CreateSoapRequest(const SoapArgs& header,
                                                       const XString& action,
                                                       const XSDK::XString& actionNS,
                                                       const SoapArgs& args)
{
    return CreateSoapRequest(header, action, actionNS, args.ToXML());
}

ClientSideRequest ClientSideRequest::CreateSoapRequest(const SoapArgs& header,
                                                       const XString& action,
                                                       const XSDK::XString& actionNS,
                                                       const XSDK::XString& args)
{
    ClientSideRequest request;

    XString body = XString::Format("<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\"><s:Header>%s</s:Header><s:Body><u:%s xmlns:u=\"%s\">%s</u:%s></s:Body></s:Envelope>",
                                    header.ToXML().c_str(),
                                    action.c_str(),
                                    actionNS.c_str(),
                                    args.c_str(),
                                    action.c_str());

    request.SetContentType("text/xml; charset=\"utf-8\"");
    request.AddHeader("SOAPACTION", XString::Format("\"%s#%s\"", actionNS.c_str(), action.c_str()));
    request.SetBody((unsigned char*)body.c_str(), body.length());
    request.SetRequestType("POST");

    return request;

}

