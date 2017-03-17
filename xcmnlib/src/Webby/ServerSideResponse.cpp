
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "Webby/ServerSideResponse.h"
#include "Webby/WebbyException.h"

#include "XSDK/XSocket.h"
#include "XSDK/XString.h"

#include <ctime>

using namespace WEBBY;
using namespace XSDK;
using namespace std;

ServerSideResponse::ServerSideResponse(ServerSideResponse::StatusCode status,
                                        const XString& contentType) :
    _status(status),
    _contentType(contentType),
    _body(new XMemory),
    _headerWritten(false),
    _additionalHeaders()
{
}

ServerSideResponse::ServerSideResponse(const ServerSideResponse& obj) :
    _status(obj._status),
    _contentType(obj._contentType),
    _body(obj._body->Clone()),
    _headerWritten(obj._headerWritten),
    _additionalHeaders(obj._additionalHeaders)
{
}

ServerSideResponse::~ServerSideResponse() throw()
{
}

ServerSideResponse& ServerSideResponse::operator = (const ServerSideResponse& obj)
{
    _status = obj._status;
    _contentType = obj._contentType;
    _body = obj._body->Clone();
    _headerWritten = obj._headerWritten;
    _additionalHeaders = obj._additionalHeaders;

    return *this;
}

void ServerSideResponse::SetStatusCode(ServerSideResponse::StatusCode status)
{
    _status = status;
}

ServerSideResponse::StatusCode ServerSideResponse::GetStatusCode()
{
    return _status;
}

void ServerSideResponse::SetContentType(const XString& contentType)
{
    _contentType = contentType;
}

XString ServerSideResponse::GetContentType()
{
    return _contentType;
}

void ServerSideResponse::SetBody(const XString& body)
{
    size_t newBodyLength = body.length();

    if(newBodyLength <= 0)
        X_STHROW(WebbyException, ("Cannot specify 0 length body."));

    _body->Clear();

    memcpy(&_body->Extend(body.length()), body.c_str(), body.length());
}

void ServerSideResponse::SetBody(XIRef<XMemory> body)
{
    _body = body->Clone();
}

XString ServerSideResponse::GetBodyAsString()
{
    return XString((char*)_body->Map(), _body->GetDataSize());
}

XIRef<XMemory> ServerSideResponse::GetBody()
{
    return _body;
}

void ServerSideResponse::ClearAdditionalHeaders()
{
    _additionalHeaders.Clear();
}

void ServerSideResponse::AddAdditionalHeader(const XString& headerName,
                                              const XString& headerValue)
{
    XString *oldValue = _additionalHeaders.Find(headerName);
    if (oldValue != NULL)
    {
        X_LOG_WARNING("[WEBBY] Header Replaced: %s [old:%s] [new:%s]", headerName.c_str(), oldValue->c_str(), headerValue.c_str());
        _additionalHeaders.Remove(headerName);
    }

    _additionalHeaders.Add(headerName, headerValue);
}

XString ServerSideResponse::GetAdditionalHeader(const XString& headerName)
{
    XString* foundHeader = _additionalHeaders.Find(headerName);

    if(foundHeader)
        return *foundHeader;

    return XString();
}

bool ServerSideResponse::WriteResponse(XRef<XStreamIO> socket)
{
    time_t now = time(0);
    char* cstr = ctime(&now);

    if( cstr == NULL )
        X_STHROW(WebbyException, ("Please set Content-Type: before calling WriteResponse()."));

    // RStrip to remove \n added by ctime
    XString timeString = XString(cstr).RStripInPlace();

    if( (_body->GetDataSize() > 0) && (_contentType.length() <= 0) )
        X_STHROW(WebbyException, ("Please set Content-Type: before calling WriteResponse()."));

    XString responseHeader = XString::Format("HTTP/1.1 %d %s\r\nDate: %s\r\n",
                                              _status,
                                              _GetStatusMessage(_status).c_str(),
                                             timeString.c_str() );

    if( _contentType.length() > 0 )
        responseHeader += XString::Format( "Content-Type: %s\r\n",
                                           _contentType.c_str() );

    if (_body->GetDataSize() != 0)
        responseHeader += XString::Format("Content-Length: %d\r\n", _body->GetDataSize());

    XHash<XString>::XHashIter i = _additionalHeaders.GetIterator();

    i.IterStart();
    while(i.IterValid())
    {
        XString key = i.IterKey();
        XString value = *i.IterData();

        responseHeader += XString::Format("%s: %s\r\n", key.c_str(), value.c_str());

        i.Iterate();
    }

    responseHeader += XString::Format("\r\n");

    if(!_SendString(socket, responseHeader))
        return false;

    if(_body->GetDataSize() > 0)
    {
        if(!_SendData(socket, _body->Map(), _body->GetDataSize()))
            return false;
    }

    return true;
}

bool ServerSideResponse::WriteChunk(XRef<XStreamIO> socket, XIRef<XMemory> chunk)
{
    if(!_headerWritten)
    {
        if(!_WriteHeader(socket))
            return false;
    }

    const XString chunkSizeString = XString::Format("%s;\r\n", XString::FromUInt(chunk->GetDataSize(), 16).c_str());

    if(!_SendString(socket, chunkSizeString))
        return false;

    if(!_SendData(socket, chunk->Map(), chunk->GetDataSize()))
        return false;

    if(!_SendString(socket, "\r\n"))
        return false;

    return true;
}

bool ServerSideResponse::WriteChunkFinalizer(XRef<XStreamIO> socket)
{
    return _SendString(socket, "0\r\n\r\n");
}

bool ServerSideResponse::WritePart(XRef<XSDK::XStreamIO> socket,
                                    const XSDK::XString& boundary,
                                    const XSDK::XHash<XSDK::XString>& partHeaders,
                                    XIRef<XSDK::XMemory> chunk)
{
    return WritePart(socket,
                      boundary,
                      partHeaders,
                      chunk->Map(),
                      chunk->GetDataSize());
}

bool ServerSideResponse::WritePart(XRef<XSDK::XStreamIO> socket,
                                   const XSDK::XString& boundary,
                                   const XSDK::XHash<XSDK::XString>& partHeaders,
                                   void* chunk,
                                   uint32_t size)
{
    if(!_SendString(socket, XString::Format("--%s\r\n", boundary.c_str())))
        return false;

    XHash<XString>::XHashIter headerIter = partHeaders.GetIterator();

    for(headerIter.IterStart();
         headerIter.IterValid();
         headerIter.Iterate())
    {
        XString headerName = headerIter.IterKey();
        XString headerValue = *headerIter.IterData();

        XString headerLine = XString::Format("%s: %s\r\n", headerName.c_str(), headerValue.c_str());

        if(!_SendString(socket, headerLine))
            return false;
    }

    if(!_SendString(socket, XString::Format("Content-Length: %d\r\n", size)))
        return false;

    if(!_SendString(socket, "\r\n"))
        return false;

    if(!_SendData(socket, chunk, size))
        return false;

    if(!_SendString(socket, "\r\n"))
        return false;

    return true;
}

bool ServerSideResponse::WritePartFinalizer(const XSDK::XString& boundary, XRef<XSDK::XStreamIO> socket)
{
    return _SendString(socket, XString::Format("--%s--\r\n", boundary.c_str()));
}

XString ServerSideResponse::_GetStatusMessage(StatusCode sc)
{
    switch(sc)
    {
    case SC_100_Continue:
        return XString("Continue");

    case SC_101_Switching_Protocols:
        return XString("Switching Protocols");

    case SC_200_OK:
        return XString("OK");

    case SC_201_Created:
        return XString("Created");

    case SC_202_Accepted:
        return XString("Accepted");

    case SC_204_No_Content:
        return XString("No Content");

    case SC_400_Bad_Request:
        return XString("Bad Request");

    case SC_401_Unauthorized:
        return XString("Unauthorized");

    case SC_403_Forbidden:
        return XString("Forbidden");

    case SC_404_Not_Found:
        return XString("Not Found");

    case SC_406_Not_Acceptable:
        return XString("Not Acceptable");

    case SC_412_Precondition_Failed:
        return XString("");

    case SC_415_Unsupported_Media_Type:
        return XString("Unsupported Media Type");

    case SC_418_Im_A_Teapot:
        return XString("I'm a Teapot");

    case SC_453_Not_Enough_Bandwidth:
        return XString("Not Enough Bandwidth");

    case SC_454_Session_Not_Found:
        return XString("Session Not Found");

    case SC_500_Internal_Server_Error:
        return XString("Internal Server Error");

    case SC_501_Not_Implemented:
        return XString("Not Implemented");

    default:
        break;
    };

    X_STHROW(WebbyException, ("Unknown status code."));
}


bool ServerSideResponse::_WriteHeader(XRef<XStreamIO> socket)
{
    time_t now = time(0);
    char* cstr = ctime(&now);

    if( cstr == NULL )
        X_STHROW(WebbyException, ("Please set Content-Type: before calling WriteResponse()."));

    // RStrip to remove \n added by ctime
    XString timeString = XString(cstr).RStripInPlace();

    if(_contentType.length() <= 0)
        X_STHROW(WebbyException, ("Please set Content-Type: before calling WriteChunk()."));

    XString responseHeader = XString::Format("HTTP/1.1 %d %s\r\nDate: %s\r\nContent-Type: %s\r\nTransfer-Encoding: chunked\r\n",
                                             _status,
                                             _GetStatusMessage(_status).c_str(),
                                             timeString.c_str(),
                                             _contentType.c_str());

    XHash<XString>::XHashIter i = _additionalHeaders.GetIterator();

    i.IterStart();
    while(i.IterValid())
    {
        XString key = i.IterKey();
        XString value = *i.IterData();

        responseHeader += XString::Format("%s: %s\r\n", key.c_str(), value.c_str());

        i.Iterate();
    }

    responseHeader += XString::Format("\r\n");

    if(!_SendString(socket, responseHeader))
        return false;

    _headerWritten = true;

    return true;
}


bool ServerSideResponse::_SendString(XRef<XStreamIO> socket, const XString& line)
{
    return _SendData(socket, line.c_str(), line.size());
}


bool ServerSideResponse::_SendData(XRef<XStreamIO> socket, const void* data, size_t dataLen)
{
    const ssize_t bytesSent = socket->Send(data, dataLen);

    return socket->Valid() && bytesSent == (ssize_t)dataLen;
}
