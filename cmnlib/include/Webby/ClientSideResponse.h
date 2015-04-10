
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef _Webby_ClientSideResponse_h
#define _Webby_ClientSideResponse_h

#include <list>
#include <vector>

#include "XSDK/XStreamIO.h"
#include "XSDK/XBaseObject.h"
#include "XSDK/XHash.h"
#include "XSDK/XString.h"
#include "XSDK/XMemory.h"

namespace WEBBY
{

class ClientSideResponse;

typedef bool (*ChunkHandler)(void* context, XIRef<XSDK::XMemory> chunk, const ClientSideResponse& response);
typedef bool (*PartHandler)( void* context, XIRef<XSDK::XMemory> chunk, const XSDK::XHash<XSDK::XString>& partHeaders, const ClientSideResponse& response );

class ClientSideResponse : public XSDK::XBaseObject
{
public:
    X_API ClientSideResponse();

    X_API ClientSideResponse(const ClientSideResponse& obj);

    X_API virtual ~ClientSideResponse() throw();

    X_API ClientSideResponse& operator = (const ClientSideResponse& obj);

    X_API void ReadResponse(XRef<XSDK::XStreamIO> socket);

    X_API XSDK::XString GetMessage();

    X_API void DebugPrintRequest();

    X_API const void* GetBody() const;

    X_API size_t GetBodySize() const;

    X_API XSDK::XString GetBodyAsString() const;

    X_API XIRef<const XSDK::XMemory> GetBodyAsXMemory() const { return _bodyContents; }

    X_API XSDK::XString GetHeader(const XSDK::XString& header) const;

    X_API std::vector<XSDK::XString> GetAllMatchingHeaders(const XSDK::XString& header) const;

    X_API bool IsSuccess() const;

    X_API bool IsFailure() const;

    X_API int GetStatus() const { return _statusCode; }

    X_API void RegisterChunkHandler(ChunkHandler ch, void* context, bool streaming = false);
    X_API void RegisterPartHandler(PartHandler ph, void* context);

private:
    void _ReadChunkedBody(XRef<XSDK::XStreamIO> socket);
    void _ReadMultiPart(XRef<XSDK::XStreamIO> socket);
    XSDK::XHash<XSDK::XString> _ReadMultiHeaderLines(XRef<XSDK::XStreamIO> socket, char* lineBuf);
    void _ReadEndOfLine(XRef<XSDK::XStreamIO> socket);

    bool _IsLegalChunkSizeChar(char ch) { return isxdigit(ch) ? true : false; } // VS warning 4800: forcing int to bool.
    bool _EmbedNull(char* lineBuf);
    void _ConsumeFooter(XRef<XSDK::XStreamIO> socket);

    void _AddHeader(const XSDK::XString& name, const XSDK::XString& value);

    bool _ReceiveData(XRef<XSDK::XStreamIO> socket, void* data, size_t dataLen);
    void _CleanSocket(XRef<XSDK::XStreamIO> socket, char** writer);
    void _ReadHeaderLine(XRef<XSDK::XStreamIO> socket, char* writer, bool firstLine);
    bool _AddLine(std::list<XSDK::XString>& lines, const XSDK::XString& line);
    void _ProcessRequestLines(const std::list<XSDK::XString>& requestLines);
    void _ProcessBody(XRef<XSDK::XStreamIO> socket);

    bool _IsEndOfLine(char* buffer)
    {
        return (buffer[0] == '\r' && buffer[1] == '\n') || buffer[0] == '\n';
    }

    XSDK::XString _initialLine;
    XSDK::XHash<std::list<XSDK::XString> > _headerParts;
    XIRef<XSDK::XMemory> _bodyContents;
    bool _success;
    int  _statusCode;

    ChunkHandler _ch;
    PartHandler _ph;
    void* _chunkHandlerContext;
    void* _partHandlerContext;
    XIRef<XSDK::XMemory> _chunk;
    bool _streaming;
};

}

#endif
