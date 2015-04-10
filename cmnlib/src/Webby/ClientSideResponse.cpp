
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "Webby/ClientSideResponse.h"
#include "Webby/StatusCodes.h"
#include "Webby/WebbyException.h"
#include "Webby/WebbyUtils.h"
#include "XSDK/XSocket.h"
#include "XSDK/XString.h"

#include <iostream>

using namespace WEBBY;
using namespace XSDK;
using namespace std;

static const unsigned int MAX_HEADER_LINE = 2048;

ClientSideResponse::ClientSideResponse() :
    _initialLine(),
    _headerParts(),
    _bodyContents(new XMemory),
    _success(false),
    _statusCode(-1),
    _ch(NULL),
    _ph(NULL),
    _chunkHandlerContext(NULL),
    _partHandlerContext(NULL),
    _chunk(new XMemory),
    _streaming(false)
{
}

ClientSideResponse::ClientSideResponse(const ClientSideResponse& rhs)
    : _initialLine(rhs._initialLine),
      _headerParts(rhs._headerParts),
      _bodyContents(rhs._bodyContents->Clone()),
      _success(rhs._success),
      _statusCode(-1),
      _ch(rhs._ch),
      _ph(rhs._ph),
      _chunkHandlerContext(rhs._chunkHandlerContext),
      _partHandlerContext(rhs._partHandlerContext),
      _chunk(rhs._chunk),
      _streaming(rhs._streaming)
{
}

ClientSideResponse::~ClientSideResponse() throw()
{
}

ClientSideResponse& ClientSideResponse::operator = (const ClientSideResponse& rhs)
{
    if(&rhs != this)
    {
        _initialLine = rhs._initialLine;
        _headerParts = rhs._headerParts;
        _bodyContents = rhs._bodyContents->Clone();
        _success = rhs._success;
        _statusCode = -1;
        _ch = rhs._ch;
        _ph = rhs._ph;
        _chunkHandlerContext = rhs._chunkHandlerContext;
        _partHandlerContext = rhs._partHandlerContext;
        _chunk = rhs._chunk;
        _streaming = rhs._streaming;
    }
    return *this;
}

void ClientSideResponse::ReadResponse(XRef<XStreamIO> socket)
{
READ_BEGIN:

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

    if(initialLineParts.size() <= 2)
        X_STHROW(WebbyException, ("HTTP request initial line doesn't have enough parts."));

    _AddHeader(XString("http_version"), initialLineParts[0]);
    _AddHeader(XString("response_code"), initialLineParts[1]);

    // After response code, we have a message, usually either "OK" or "Not Found", this code appends all the initial line
    // pieces after the first two parts so that we end up with a complete "message".

    XString msg = initialLineParts[2];

    for(int i = 3, e = initialLineParts.size(); i < e; ++i)
    {
        msg += " ";
        msg += initialLineParts[i];
    }

    _AddHeader(XString("message"), msg);

    if(const list<XString>* found = _headerParts.Find("response_code"))
    {
        _statusCode = found->front().ToInt();
        if(kWebbyResponseOk <= _statusCode && _statusCode < kWebbyResponseMultipleChoices)
            _success = true;
    }

    // Handling a "100 continue" initial line, as per http 1.1 spec; we basically
    // just restart... A 100 continue means another complete header and body follows...
    if(msg.ToLower().Contains ("continue"))
        goto READ_BEGIN;

    _ProcessRequestLines(requestLines);
    _ProcessBody(socket);
}

bool ClientSideResponse::_ReceiveData(XRef<XStreamIO> socket, void* data, size_t dataLen)
{
    const ssize_t bytesSent = socket->Recv(data, dataLen);

    return socket->Valid() && bytesSent == (ssize_t)dataLen;
}

void ClientSideResponse::_CleanSocket(XRef<XStreamIO> socket, char** writer)
{
    if ( socket.IsEmpty() )
        X_STHROW(WebbyException, ("StreamIO is null"));

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

void ClientSideResponse::_ReadHeaderLine(XRef<XStreamIO> socket, char* writer, bool firstLine)
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

bool ClientSideResponse::_AddLine(std::list<XString>& lines, const XString& line)
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

void ClientSideResponse::_ProcessRequestLines(const list<XString>& requestLines)
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

            _AddHeader(key.ToLower(), val.StripEOL());
        }
    }
}

void ClientSideResponse::_ProcessBody(XRef<XStreamIO> socket)
{
    /// Get the body if we were given a Content Length
    if(const list<XString>* found = _headerParts.Find(XString("content-length")))
    {
        const uint32_t contentLength = found->front().ToUInt32();

        if(contentLength > 0)
        {
            if(!_ReceiveData(socket, &_bodyContents->Extend(contentLength), contentLength))
                X_STHROW(WebbyException, ("Failed to read data from socket->"));
        }
    }
    else if(const list<XString>* found = _headerParts.Find(XString("transfer-encoding")))
    {
        if(found->front().ToLower().Contains("chunked"))
        {
            _ReadChunkedBody(socket);
            return;
        }
    }

    if(const list<XString>* found = _headerParts.Find(XString("content-type")))
    {
        if(found->front().ToLower().Contains("multipart"))
        {
            _ReadMultiPart(socket);
            return;
        }
    }

    if(GetHeader("content-type").ToLower().Contains("multipart"))
        _ReadMultiPart(socket);
}

const void* ClientSideResponse::GetBody() const
{
    return &(_bodyContents->Index(0));
}

size_t ClientSideResponse::GetBodySize() const
{
    return _bodyContents->GetDataSize();
}

XString ClientSideResponse::GetBodyAsString() const
{
    return XString((char*)_bodyContents->Map(), _bodyContents->GetDataSize());
}

XString ClientSideResponse::GetHeader(const XString& name) const
{
    list<XString>* values = _headerParts.Find(name.ToLower());

    if(values)
        return values->front();

    return "";
}

vector<XString> ClientSideResponse::GetAllMatchingHeaders(const XString& header) const
{
    vector<XString> matchingHeaders;

    list<XString>* matches = _headerParts.Find(header.ToLower());
    if(matches)
    {
        for(list<XString>::iterator i = matches->begin(); i != matches->end(); ++i)
        {
            matchingHeaders.push_back(*i);
        }
    }

    return matchingHeaders;
}

XString ClientSideResponse::GetMessage()
{
    return GetHeader("message");
}

bool ClientSideResponse::IsSuccess() const
{
    return _success;
}

bool ClientSideResponse::IsFailure() const
{
    return !_success;
}

void ClientSideResponse::RegisterChunkHandler(ChunkHandler ch, void* context, bool streaming)
{
    _ch = ch;
    _chunkHandlerContext = context;
    _streaming = streaming;
}

void ClientSideResponse::RegisterPartHandler(PartHandler ph, void* context)
{
    _ph = ph;
    _partHandlerContext = context;
}

void ClientSideResponse::_ReadChunkedBody(XRef<XStreamIO> socket)
{
    char lineBuf[MAX_HEADER_LINE+1];
    bool moreChunks = true;

    while(moreChunks)
    {
        memset(lineBuf, 0, MAX_HEADER_LINE+1);

        _ReadHeaderLine(socket, lineBuf, false);

        if(lineBuf[0] == '0')
        {
            _ConsumeFooter(socket);
            return;
        }

        if(_EmbedNull(lineBuf))
        {
            const XString chunkLenS = lineBuf;
            const uint32_t chunkLen = chunkLenS.ToUInt(16);

            // We read our chunk into a temporary "chunk" XMemory object, we then optionally
            // call our "chunk callback" function... Finally, we copy the new chunk into the
            // main body contents object.

            _chunk->Clear();
            if(!_ReceiveData(socket, &_chunk->Extend(chunkLen), chunkLen))
                X_STHROW(WebbyException, ("Failed to read data from socket->"));

            // call callback here...
            if(_ch)
                moreChunks = _ch(_chunkHandlerContext, _chunk, *this);

            // We only append a chunk to our "_bodyContents" if we are not streaming (
            // because "streams" potentially have no end, so an XMemory that contains the
            // complete body contents would just grow forever).
            if(!_streaming)
                memcpy(&(_bodyContents->Extend(chunkLen)), _chunk->Map(), chunkLen);

            _ReadEndOfLine(socket);
        }
    }
}

bool ClientSideResponse::_EmbedNull(char* lineBuf)
{
    // Scan forward in the chunk header line and embed a NULL on the first non legal chunk size char.
    for(size_t i = 0; i < MAX_HEADER_LINE; ++i)
    {
        if(!_IsLegalChunkSizeChar(lineBuf[i]))
        {
            lineBuf[i] = 0;
            return true;
        }
    }

    return false;
}

void ClientSideResponse::_ReadMultiPart(XRef<XStreamIO> socket)
{
    char lineBuf[MAX_HEADER_LINE+1];
    bool moreParts = true;

    while(moreParts)
    {
        // First, read the boundary line...
        memset(lineBuf, 0, MAX_HEADER_LINE);

        _ReadHeaderLine(socket, lineBuf, false);

        if(XString(lineBuf).StripEOL().EndsWith("--"))
            break;

        XHash<XString> partHeaders = _ReadMultiHeaderLines(socket, lineBuf);

        if(XString* found = partHeaders.Find(XString("Content-Length").ToLower()))
        {
            const int partContentLength = found->ToInt();
            _chunk->Clear();

            if(!_ReceiveData(socket, &_chunk->Extend(partContentLength), partContentLength))
                X_STHROW(WebbyException, ("Failed to read data from socket->"));

            // call callback here...
            if(_ph)
                moreParts = _ph(_partHandlerContext, _chunk, partHeaders, *this);

            _ReadEndOfLine(socket);
        }
        else
            X_STHROW(WebbyException, ("Oops. Mime multipart without a Content-Length!"));
    }
}


void ClientSideResponse::_ReadEndOfLine(XRef<XStreamIO> socket)
{
    char lineEnd[2] = {0, 0};

    {
        if(!_ReceiveData(socket, &lineEnd[0], 1))
            X_STHROW(WebbyException, ("Failed to read data from socket->"));
    }

    if(lineEnd[0] == '\r')
    {
        if(!_ReceiveData(socket, &lineEnd[1], 1))
            X_STHROW(WebbyException, ("Failed to read data from socket->"));
    }

    if(!_IsEndOfLine(lineEnd))
        X_STHROW(WebbyException, ("A chunk line didn't end with appropriate terminator."));
}


XHash<XString> ClientSideResponse::_ReadMultiHeaderLines(XRef<XStreamIO> socket, char* lineBuf)
{
    std::list<XString> partLines;

    /// Now, read the rest of the header lines...
    while(true)
    {
        memset(lineBuf, 0, MAX_HEADER_LINE);

        _ReadHeaderLine(socket, lineBuf, false);

        if(_AddLine(partLines, lineBuf))
            break;
    }

    XHash<XString> partHeaders;

    for(list<XString>::const_iterator iter = partLines.begin(),
                                      end  = partLines.end();
        iter != end;
        ++iter)
    {
        const vector<XString> lineParts = iter->Split(':');
        partHeaders.Add(AdjustHeaderName(lineParts[0]), AdjustHeaderValue(lineParts[1].StripEOL()));
    }

    return partHeaders;
}

void ClientSideResponse::DebugPrintRequest()
{
    XString method = GetHeader(XString("method"));
    XString response_code = GetHeader(XString("response_code"));
    XString http_version = GetHeader(XString("http_version"));
    printf("method = %s\n", method.c_str());
    printf("response_code = %s\n", response_code.c_str());
    printf("http_version = %s\n", http_version.c_str());
    fflush(stdout);
}

void ClientSideResponse::_ConsumeFooter(XRef<XStreamIO> socket)
{
    char lineBuf[MAX_HEADER_LINE+1];

    /// Now, read the rest of the header lines...
    while(true)
    {
        memset(lineBuf, 0, MAX_HEADER_LINE);

        _ReadHeaderLine(socket, lineBuf, false);

        if(_IsEndOfLine(lineBuf))
            break;
    }
}

void ClientSideResponse::_AddHeader(const XString& name, const XString& value)
{
    const XString adjName = AdjustHeaderName(name);
    const XString adjValue = AdjustHeaderValue(value);

    if(list<XString>* found = _headerParts.Find(adjName))
        found->push_back(adjValue);
    else
    {
        list<XString> values;
        values.push_back(adjValue);
        _headerParts.Add(adjName, values);
    }
}
