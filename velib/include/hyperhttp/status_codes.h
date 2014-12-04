
/// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=--=-=-=-=-=-
/// hyperhttp - http://www.cppkit.org
/// Copyright (c) 2013, Tony Di Croce
/// All rights reserved.
///
/// Redistribution and use in source and binary forms, with or without modification, are permitted
/// provided that the following conditions are met:
///
/// 1. Redistributions of source code must retain the above copyright notice, this list of conditions and
///    the following disclaimer.
/// 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions
///    and the following disclaimer in the documentation and/or other materials provided with the
///    distribution.
///
/// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
/// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
/// PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
/// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
/// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
/// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
/// TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
/// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
///
/// The views and conclusions contained in the software and documentation are those of the authors and
/// should not be interpreted as representing official policies, either expressed or implied, of the cppkit
/// Project.
/// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=--=-=-=-=-=-

#ifndef hyperhttp_status_codes_h
#define hyperhttp_status_codes_h

namespace hyperhttp
{

/// SUCCESS RESPONSES
const int32_t response_ok                      = 200; ///< Request has succeeded.
const int32_t response_created                 = 201; ///< Request succeeded and resource created.
const int32_t response_accepted                = 202; ///< Request accepted for processing.
const int32_t response_no_content              = 204; ///< Request succeeded, but no content.
const int32_t response_reset_content           = 205; ///< Agent should reset its document view.

/// MISC RESPONSES
const int32_t response_multiple_choices        = 300; ///< Resource has multiple choices.
const int32_t response_moved                   = 301; ///< Resource moved permanently.
const int32_t response_found                   = 302; ///< Response body should contain temporary URI.

/// ERROR RESPONSES
const int32_t response_bad_reques              = 400; ///< Request could not be understood by server.
const int32_t response_inauthorized            = 401; ///< Request requires user authentication.
const int32_t response_forbidden               = 403; ///< Request was refused.
const int32_t response_not_found               = 404; ///< Resource not found.
const int32_t response_method_not_allowed      = 405; ///< Could not use specified method for this resource.
const int32_t response_gone                    = 410; ///< Resource no longer available, no forwarding address.
const int32_t response_length_required         = 411; ///< Request requires Content-Length header field.

/// SERVER ERROR RESPONSES
const int32_t response_internal_server_error   = 500; ///< The server encountered an unexpected condition.
const int32_t response_not_implemented         = 501; ///< Server does not support the functionality to fulfill the request.
const int32_t response_bad_gateway             = 502; ///< Server received an invalid response from upstream server.
const int32_t response_service_unavailable     = 503; ///< Server temporarily unable to handle request.
const int32_t response_http_version_not_supported = 505; ///< Server does not support requested HTTP protocol version.

}

#endif
