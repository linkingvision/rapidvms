
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "Webby/ServerSideRequest.h"
#include "Webby/ClientSideRequest.h"
#include "Webby/WebbyException.h"
#include "Webby/WebbyUtils.h"
#include "XSDK/XSocket.h"
#include "XSDK/XString.h"

using namespace WEBBY;
using namespace XSDK;
using namespace std;

static const unsigned int MAX_HEADER_LINE = 16384;

ServerSideRequest::ServerSideRequest() :
    _initialLine(),
    _headerParts(),
    _postVars(),
    _body(),
    _contentType()
{
}

ServerSideRequest::ServerSideRequest(const ServerSideRequest& obj) :
    _initialLine(obj._initialLine),
    _headerParts(obj._headerParts),
    _postVars(obj._postVars),
    _body(obj._body),
    _contentType()
{
}

ServerSideRequest::~ServerSideRequest() throw()
{
}

ServerSideRequest& ServerSideRequest::operator = (const ServerSideRequest& obj)
{
    _initialLine = obj._initialLine;
    _headerParts = obj._headerParts;
    _postVars = obj._postVars;
    _body = obj._body;
    _contentType = obj._contentType;

    return *this;
}

void ServerSideRequest::ReadRequest(XRef<XSDK::XStreamIO> socket)
{
    list<XString> requestLines;

    {
        char lineBuf[MAX_HEADER_LINE+1];
        memset(lineBuf, 0, MAX_HEADER_LINE+1);

        {
            char* writer = &lineBuf[0];
            _CleanSocket(socket, &writer);

            // Get initial header line
            _ReadHeaderLine(socket, writer, true);
        }

        _initialLine = XString(lineBuf).StripEOL();

        /// Now, read the rest of the header lines...
        do
        {
            memset(lineBuf, 0, MAX_HEADER_LINE);
            _ReadHeaderLine(socket, lineBuf, false);

        } while(!_AddLine(requestLines, lineBuf));
    }

    /// Now, populate our header hash...

    _headerParts.Clear();

    const vector<XString> initialLineParts = _initialLine.Split(' ');

    if(initialLineParts.size() != 3)
        X_STHROW(WebbyException, ("HTTP request initial line exceeded 3 parts"));

    SetHeader("method", initialLineParts[0]);
    SetHeader("uri", initialLineParts[1]);
    SetHeader("http_version", initialLineParts[2].StripEOL());

    _ProcessRequestLines(requestLines);

    const XString method = initialLineParts[0].ToLower();
    if(method == kWebbyPOSTRequest.ToLower() || method == kWebbyPUTRequest.ToLower() || method == kWebbyPATCHRequest.ToLower())
        _ProcessBody(socket);
}

void ServerSideRequest::SetContentType(const XSDK::XString& type)
{
    _contentType = type;
}

bool ServerSideRequest::IsPatchRequest()
{
    return GetMethod() == kWebbyPATCHRequest.ToLower();
}
bool ServerSideRequest::IsPostRequest()
{
    return GetMethod() == kWebbyPOSTRequest.ToLower();
}

bool ServerSideRequest::IsGetRequest()
{
    return GetMethod() == kWebbyGETRequest.ToLower();
}

bool ServerSideRequest::IsPutRequest()
{
    return GetMethod() == kWebbyPUTRequest.ToLower();
}

bool ServerSideRequest::IsDeleteRequest()
{
    return GetMethod() == kWebbyDELETERequest.ToLower();
}

XString ServerSideRequest::GetMethod() const
{
    if(GetHeader("method"))
        return GetHeader("method")->ToLower();

    X_STHROW(WebbyException, ("Request has no method."));
}

URI ServerSideRequest::GetURI() const
{
    if(GetHeader((char*)"uri"))
        return URI(*GetHeader((char*)"uri"));

    X_STHROW(WebbyException, ("Request has no uri."));
}

XString ServerSideRequest::GetContentType() const
{
    return _contentType;
}

void ServerSideRequest::SetInitialLine(const XSDK::XString& initialLine)
{
    _initialLine = initialLine;
}

void ServerSideRequest::SetHeader(const XString& name, const XString& value)
{
    const XString adjName = AdjustHeaderName(name);
    const XString adjValue = AdjustHeaderValue(value);

    if(GetHeader(adjName) != NULL)
        _headerParts.Remove(adjName);

    _headerParts.Add(adjName, adjValue);
}

XString* ServerSideRequest::GetHeader(XString key) const
{
    return _headerParts.Find(key.ToLower());
}

XHash<XString> ServerSideRequest::GetHeaders() const
{
    return _headerParts;
}


const void* ServerSideRequest::GetBody() const
{
    return _body.Map();
}

size_t ServerSideRequest::GetBodySize() const
{
    return _body.GetDataSize();
}

XSDK::XString ServerSideRequest::GetBodyAsString() const
{
    return XString((char*)_body.Map(), _body.GetDataSize());
}

XSDK::XHash<XSDK::XString>& ServerSideRequest::GetPostVars()
{
    return _postVars;
}

void ServerSideRequest::DebugPrintRequest()
{
    printf("method = %s\n", GetHeader((char*)"method")->c_str());
    printf("uri = %s\n", GetHeader((char*)"uri")->c_str());
    printf("http_version = %s\n", GetHeader((char*)"http_version")->c_str());
    fflush(stdout);
}


bool ServerSideRequest::_ReceiveData(XRef<XStreamIO> socket, void* data, size_t dataLen)
{
    const ssize_t bytesSent = socket->Recv(data, dataLen);

    return socket->Valid() && bytesSent == (ssize_t)dataLen;
}


void ServerSideRequest::_CleanSocket(XRef<XStreamIO> socket, char** writer)
{
    if(!socket->Valid())
        X_STHROW(WebbyException, ("Invalid Socket"));

    char tempBuffer[1];

    // Clear junk off the socket
    while(true)
    {
        if(!_ReceiveData(socket, tempBuffer, 1))
            X_STHROW(WebbyException, ("Failed to read data from socket->"));

        if(!XString::IsSpace(tempBuffer[0]))
        {
            **writer = tempBuffer[0];
            ++*writer;
            break;
        }
    }
}


void ServerSideRequest::_ReadHeaderLine(XRef<XStreamIO> socket, char* writer, bool firstLine)
{
    bool lineDone = false;
    size_t bytesReadThisLine = 0;

    // Get initial header line
    while(!lineDone && bytesReadThisLine + 1 < MAX_HEADER_LINE)
    {
        if(!_ReceiveData(socket, writer, 1))
            X_STHROW(WebbyException, ("Failed to read data from socket->"));

        ++bytesReadThisLine;

        if(*writer == '\n')
            lineDone = true;

        ++writer;
    }

    if(!lineDone)
    {
        XString msg = firstLine ? "The HTTP initial request line exceeded our max."
                                : "The HTTP initial request line exceeded our max.";

        X_STHROW(WebbyException, (msg));
    }
}


bool ServerSideRequest::_AddLine(std::list<XString>& lines, const XString& line)
{
    if(line.StartsWith("\r\n") || line.StartsWith("\n"))
        return true;

    if(line.StartsWith(" ") || line.StartsWith("\t"))
    {
        if(!lines.empty())
            lines.back() += line;
        else
            X_STHROW(WebbyException, ("First line of header missing needed seperator."));
    }
    else
        lines.push_back(line);

    return false;
}


void ServerSideRequest::_ProcessRequestLines(const list<XString>& requestLines)
{
    // Now, iterate on the header lines...

    for(list<XString>::const_iterator iter = requestLines.begin(),
                                      end  = requestLines.end();
        iter != end;
        ++iter)
    {
        const size_t firstColon = iter->find(':');

        if(firstColon != string::npos)
        {
            const XString key = iter->substr(0, firstColon);
            const XString val = firstColon + 1 < iter->size() ? iter->substr(firstColon + 1) : "";

            SetHeader(key, val.StripEOL());
        }
    }
}


void ServerSideRequest::_ProcessBody(XRef<XStreamIO> socket)
{
    XString* contentLengthString = GetHeader("Content-Length");

    if(contentLengthString)
    {
        contentLengthString->StripInPlace();
        uint32_t contentLength = contentLengthString->ToUInt();

        if(!contentLength)
            return;

        _body.Clear();

        unsigned char* buffer = &_body.Extend(contentLength);

        if(!_ReceiveData(socket, buffer, contentLength))
            X_STHROW(WebbyException, ("Failed to read data from socket->"));

        XString* contentType = GetHeader("Content-Type");

        if(contentType)
        {
            _contentType = (*contentType).LStrip();

            if(contentType->Contains("x-www-form-urlencoded"))
            {
                XString rawBody((char*)_body.Map(), (int)_body.GetDataSize());

                vector<XString> parts;
                rawBody.Split("&", parts);

                for(size_t i = 0; i < parts.size(); ++i)
                {
                    XString nvPair = parts[i];

                    vector<XString> nameAndValue;
                    nvPair.Split("=", nameAndValue);

                    if(nameAndValue.size() == 2)
                        _postVars.Add(nameAndValue[0], nameAndValue[1]);
                }
            }
        }
    }
}
