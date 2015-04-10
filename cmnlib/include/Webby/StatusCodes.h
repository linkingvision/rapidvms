
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef _Webby_StatusCodes_h
#define _Webby_StatusCodes_h

namespace WEBBY
{

/// SUCCESS RESPONSES
const int32_t kWebbyResponseOk                      = 200; ///< Request has succeeded.
const int32_t kWebbyResponseCreated                 = 201; ///< Request succeeded and resource created.
const int32_t kWebbyResponseAccepted                = 202; ///< Request accepted for processing.
const int32_t kWebbyResponseNoContent               = 204; ///< Request succeeded, but no content.
const int32_t kWebbyResponseResetContent            = 205; ///< Agent should reset its document view.

/// MISC RESPONSES
const int32_t kWebbyResponseMultipleChoices         = 300; ///< Resource has multiple choices.
const int32_t kWebbyResponseMoved                   = 301; ///< Resource moved permanently.
const int32_t kWebbyResponseFound                   = 302; ///< Response body should contain temporary URI.

/// ERROR RESPONSES
const int32_t kWebbyResponseBadRequest              = 400; ///< Request could not be understood by server.
const int32_t kWebbyResponseUnauthorized            = 401; ///< Request requires user authentication.
const int32_t kWebbyResponseForbidden               = 403; ///< Request was refused.
const int32_t kWebbyResponseNotFound                = 404; ///< Resource not found.
const int32_t kWebbyResponseMethodNotAllowed        = 405; ///< Could not use specified method for this resource.
const int32_t kWebbyResponseGone                    = 410; ///< Resource no longer available, no forwarding address.
const int32_t kWebbyResponseLengthRequired          = 411; ///< Request requires Content-Length header field.

/// SERVER ERROR RESPONSES
const int32_t kWebbyResponseInt32_TernalServerError = 500; ///< The server encountered an unexpected condition.
const int32_t kWebbyResponseNotImplemented          = 501; ///< Server does not support the functionality to fulfill the request.
const int32_t kWebbyResponseBadGateway              = 502; ///< Server received an invalid response from upstream server.
const int32_t kWebbyResponseServiceUnavailable      = 503; ///< Server temporarily unable to handle request.
const int32_t kWebbyResponseHTTPVersionNotSupported = 505; ///< Server does not support requested HTTP protocol version.

}

#endif
