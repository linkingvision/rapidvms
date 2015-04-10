
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef _Webby_ServerSideResponse_h
#define _Webby_ServerSideResponse_h

#include "XSDK/XStreamIO.h"
#include "XSDK/XBaseObject.h"
#include "XSDK/XHash.h"
#include "XSDK/XString.h"
#include "XSDK/XMemory.h"

#include <list>

namespace WEBBY
{

class ServerSideResponse : public XSDK::XBaseObject
{
public:

    enum StatusCode
    {
        SC_100_Continue = 100,
        SC_101_Switching_Protocols = 101,
        SC_200_OK = 200,
        SC_201_Created = 201,
        SC_202_Accepted = 202,
        SC_204_No_Content = 204,
        SC_400_Bad_Request = 400,
        SC_401_Unauthorized = 401,
        SC_403_Forbidden = 403,
        SC_404_Not_Found = 404,
        SC_406_Not_Acceptable = 406,
        SC_412_Precondition_Failed = 412,
        SC_415_Unsupported_Media_Type = 415,
        SC_418_Im_A_Teapot = 418,
        SC_453_Not_Enough_Bandwidth = 453,
        SC_454_Session_Not_Found = 454,
        SC_500_Internal_Server_Error = 500,
        SC_501_Not_Implemented = 501
    };

    X_API ServerSideResponse(StatusCode status = SC_200_OK,
                             const XSDK::XString& contentType = "text/plain");

    X_API ServerSideResponse(const ServerSideResponse& obj);

    X_API virtual ~ServerSideResponse() throw();

    X_API ServerSideResponse& operator=(const ServerSideResponse& obj);

    X_API void SetStatusCode(StatusCode status);
    X_API StatusCode GetStatusCode();

//
// Possible Content-Type: values...
//     "text/plain"
//     "image/jpeg"
//     "application/xhtml+xml"
//     "application/x-rtsp-tunnelled"
//     "multipart/mixed; boundary="foo"

    X_API void SetContentType(const XSDK::XString& contentType);
    X_API XSDK::XString GetContentType();

    X_API void SetBody(const XSDK::XString& body);
    X_API void SetBody(XIRef<XSDK::XMemory> body);

    X_API XSDK::XString GetBodyAsString();
    X_API XIRef<XSDK::XMemory> GetBody();

    X_API void ClearAdditionalHeaders();
    X_API void AddAdditionalHeader(const XSDK::XString& headerName,
                                    const XSDK::XString& headerValue);
    X_API XSDK::XString GetAdditionalHeader(const XSDK::XString& headerName);

    X_API bool WriteResponse(XRef<XSDK::XStreamIO> socket);

    // Chunked transfer encoding support...
    X_API bool WriteChunk(XRef<XSDK::XStreamIO> socket, XIRef<XSDK::XMemory> chunk);
    X_API bool WriteChunkFinalizer(XRef<XSDK::XStreamIO> socket);

    // Multipart mimetype support
    // WritePart() will automaticaly add a Content-Length header per
    // part.
    X_API bool WritePart( XRef<XSDK::XStreamIO> socket,
                          const XSDK::XString& boundary,
                          const XSDK::XHash<XSDK::XString>& partHeaders,
                          XIRef<XSDK::XMemory> chunk );

    X_API bool WritePart( XRef<XSDK::XStreamIO> socket,
                          const XSDK::XString& boundary,
                          const XSDK::XHash<XSDK::XString>& partHeaders,
                          void* chunk,
                          uint32_t size );

    X_API bool WritePartFinalizer(const XSDK::XString& boundary, XRef<XSDK::XStreamIO> socket);

private:
    XSDK::XString _GetStatusMessage(StatusCode sc);
    bool _WriteHeader(XRef<XSDK::XStreamIO> socket);
    bool _SendString(XRef<XSDK::XStreamIO> socket, const XSDK::XString& line);
    bool _SendData(XRef<XSDK::XStreamIO> socket, const void* data, size_t dataLen);

    StatusCode _status;
    XSDK::XString _contentType;
    XIRef<XSDK::XMemory> _body;
    bool _headerWritten;
    XSDK::XHash<XSDK::XString> _additionalHeaders;
};

}

#endif
