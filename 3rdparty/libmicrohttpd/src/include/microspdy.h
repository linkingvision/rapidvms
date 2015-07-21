/*
    This file is part of libmicrospdy
    Copyright Copyright (C) 2012, 2013 Christian Grothoff

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file microspdy.h
 * @brief public interface to libmicrospdy
 * @author Andrey Uzunov
 * @author Christian Grothoff
 *
 * All symbols defined in this header start with SPDY_.  libmisrospdy is a small
 * SPDY daemon library. The application can start multiple daemons
 * and they are independent.<p>
 *
 * The header file defines various constants used by the SPDY and the HTTP protocol.
 * This does not mean that the lib actually interprets all of these
 * values. Not everything is implemented. The provided constants are exported as a convenience
 * for users of the library.  The lib does not verify that provided
 * HTTP headers and if their values conform to the SPDY protocol,
 * it only checks if the required headers for the SPDY requests and
 * responses are provided.<p>
 *
 * The library uses just a single thread.<p>
 *
 * Before including "microspdy.h" you should add the necessary
 * includes to define the types used in this file (which headers are needed may
 * depend on your platform; for possible suggestions consult
 * "platform.h" in the libmicrospdy distribution).<p>
 *
 * All of the functions returning SPDY_YES/SPDY_NO return
 * SPDY_INPUT_ERROR when any of the parameters are invalid, e.g.
 * required parameter is NULL.<p>
 *
 * The library does not check if anything at the application layer --
 * requests and responses -- is correct. For example, it
 * is up to the user to check if a client is sending HTTP body but the
 * method is GET.<p>
 *
 * The SPDY flow control is just partially implemented: the receiving
 * window is updated, and the client is notified, to prevent a client
 * from stop sending POST body data, for example.
 */
#ifndef SPDY_MICROSPDY_H
#define SPDY_MICROSPDY_H

#include <zlib.h>
#include <stdbool.h>

/* While we generally would like users to use a configure-driven
   build process which detects which headers are present and
   hence works on any platform, we use "standard" includes here
   to build out-of-the-box for beginning users on common systems.

   Once you have a proper build system and go for more exotic
   platforms, you should define MHD_PLATFORM_H in some header that
   you always include *before* "microhttpd.h".  Then the following
   "standard" includes won't be used (which might be a good
   idea, especially on platforms where they do not exist). */
#ifndef MHD_PLATFORM_H
#include <unistd.h>
#include <stdarg.h>
#include <stdint.h>
#ifdef __MINGW32__
#include <ws2tcpip.h>
#else
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#endif
#endif

#ifndef _MHD_EXTERN
#define _MHD_EXTERN extern
#endif

/**
 * return code for "YES".
 */
#define SPDY_YES 1

/**
 * return code for "NO".
 */
#define SPDY_NO 0

/**
 * return code for error when input parameters are wrong. To be returned
 * only by functions which return int. The others will return NULL on
 * input error.
 */
#define SPDY_INPUT_ERROR -1

/**
 * SPDY version supported by the lib.
 */
#define SPDY_VERSION 3

/**
 * The maximum allowed size (without 8 byte headers) of
 * SPDY frames (value length) is 8192. The lib will accept and
 * send frames with length at most this value here.
 */
#define SPDY_MAX_SUPPORTED_FRAME_SIZE 8192

/**
 * HTTP response codes.
 */
#define SPDY_HTTP_CONTINUE 100
#define SPDY_HTTP_SWITCHING_PROTOCOLS 101
#define SPDY_HTTP_PROCESSING 102

#define SPDY_HTTP_OK 200
#define SPDY_HTTP_CREATED 201
#define SPDY_HTTP_ACCEPTED 202
#define SPDY_HTTP_NON_AUTHORITATIVE_INFORMATION 203
#define SPDY_HTTP_NO_CONTENT 204
#define SPDY_HTTP_RESET_CONTENT 205
#define SPDY_HTTP_PARTIAL_CONTENT 206
#define SPDY_HTTP_MULTI_STATUS 207

#define SPDY_HTTP_MULTIPLE_CHOICES 300
#define SPDY_HTTP_MOVED_PERMANENTLY 301
#define SPDY_HTTP_FOUND 302
#define SPDY_HTTP_SEE_OTHER 303
#define SPDY_HTTP_NOT_MODIFIED 304
#define SPDY_HTTP_USE_PROXY 305
#define SPDY_HTTP_SWITCH_PROXY 306
#define SPDY_HTTP_TEMPORARY_REDIRECT 307

#define SPDY_HTTP_BAD_REQUEST 400
#define SPDY_HTTP_UNAUTHORIZED 401
#define SPDY_HTTP_PAYMENT_REQUIRED 402
#define SPDY_HTTP_FORBIDDEN 403
#define SPDY_HTTP_NOT_FOUND 404
#define SPDY_HTTP_METHOD_NOT_ALLOWED 405
#define SPDY_HTTP_METHOD_NOT_ACCEPTABLE 406
#define SPDY_HTTP_PROXY_AUTHENTICATION_REQUIRED 407
#define SPDY_HTTP_REQUEST_TIMEOUT 408
#define SPDY_HTTP_CONFLICT 409
#define SPDY_HTTP_GONE 410
#define SPDY_HTTP_LENGTH_REQUIRED 411
#define SPDY_HTTP_PRECONDITION_FAILED 412
#define SPDY_HTTP_REQUEST_ENTITY_TOO_LARGE 413
#define SPDY_HTTP_REQUEST_URI_TOO_LONG 414
#define SPDY_HTTP_UNSUPPORTED_MEDIA_TYPE 415
#define SPDY_HTTP_REQUESTED_RANGE_NOT_SATISFIABLE 416
#define SPDY_HTTP_EXPECTATION_FAILED 417
#define SPDY_HTTP_UNPROCESSABLE_ENTITY 422
#define SPDY_HTTP_LOCKED 423
#define SPDY_HTTP_FAILED_DEPENDENCY 424
#define SPDY_HTTP_UNORDERED_COLLECTION 425
#define SPDY_HTTP_UPGRADE_REQUIRED 426
#define SPDY_HTTP_NO_RESPONSE 444
#define SPDY_HTTP_RETRY_WITH 449
#define SPDY_HTTP_BLOCKED_BY_WINDOWS_PARENTAL_CONTROLS 450
#define SPDY_HTTP_UNAVAILABLE_FOR_LEGAL_REASONS 451

#define SPDY_HTTP_INTERNAL_SERVER_ERROR 500
#define SPDY_HTTP_NOT_IMPLEMENTED 501
#define SPDY_HTTP_BAD_GATEWAY 502
#define SPDY_HTTP_SERVICE_UNAVAILABLE 503
#define SPDY_HTTP_GATEWAY_TIMEOUT 504
#define SPDY_HTTP_HTTP_VERSION_NOT_SUPPORTED 505
#define SPDY_HTTP_VARIANT_ALSO_NEGOTIATES 506
#define SPDY_HTTP_INSUFFICIENT_STORAGE 507
#define SPDY_HTTP_BANDWIDTH_LIMIT_EXCEEDED 509
#define SPDY_HTTP_NOT_EXTENDED 510

/**
 * HTTP headers are used in SPDY, but all of them MUST be lowercase.
 * Some are not valid in SPDY and MUST not be used
 */
#define SPDY_HTTP_HEADER_ACCEPT "accept"
#define SPDY_HTTP_HEADER_ACCEPT_CHARSET "accept-charset"
#define SPDY_HTTP_HEADER_ACCEPT_ENCODING "accept-encoding"
#define SPDY_HTTP_HEADER_ACCEPT_LANGUAGE "accept-language"
#define SPDY_HTTP_HEADER_ACCEPT_RANGES "accept-ranges"
#define SPDY_HTTP_HEADER_AGE "age"
#define SPDY_HTTP_HEADER_ALLOW "allow"
#define SPDY_HTTP_HEADER_AUTHORIZATION "authorization"
#define SPDY_HTTP_HEADER_CACHE_CONTROL "cache-control"
/* Connection header is forbidden in SPDY */
#define SPDY_HTTP_HEADER_CONNECTION "connection"
#define SPDY_HTTP_HEADER_CONTENT_ENCODING "content-encoding"
#define SPDY_HTTP_HEADER_CONTENT_LANGUAGE "content-language"
#define SPDY_HTTP_HEADER_CONTENT_LENGTH "content-length"
#define SPDY_HTTP_HEADER_CONTENT_LOCATION "content-location"
#define SPDY_HTTP_HEADER_CONTENT_MD5 "content-md5"
#define SPDY_HTTP_HEADER_CONTENT_RANGE "content-range"
#define SPDY_HTTP_HEADER_CONTENT_TYPE "content-type"
#define SPDY_HTTP_HEADER_COOKIE "cookie"
#define SPDY_HTTP_HEADER_DATE "date"
#define SPDY_HTTP_HEADER_ETAG "etag"
#define SPDY_HTTP_HEADER_EXPECT "expect"
#define SPDY_HTTP_HEADER_EXPIRES "expires"
#define SPDY_HTTP_HEADER_FROM "from"
/* Host header is forbidden in SPDY */
#define SPDY_HTTP_HEADER_HOST "host"
#define SPDY_HTTP_HEADER_IF_MATCH "if-match"
#define SPDY_HTTP_HEADER_IF_MODIFIED_SINCE "if-modified-since"
#define SPDY_HTTP_HEADER_IF_NONE_MATCH "if-none-match"
#define SPDY_HTTP_HEADER_IF_RANGE "if-range"
#define SPDY_HTTP_HEADER_IF_UNMODIFIED_SINCE "if-unmodified-since"
/* Keep-Alive header is forbidden in SPDY */
#define SPDY_HTTP_HEADER_KEEP_ALIVE "keep-alive"
#define SPDY_HTTP_HEADER_LAST_MODIFIED "last-modified"
#define SPDY_HTTP_HEADER_LOCATION "location"
#define SPDY_HTTP_HEADER_MAX_FORWARDS "max-forwards"
#define SPDY_HTTP_HEADER_PRAGMA "pragma"
#define SPDY_HTTP_HEADER_PROXY_AUTHENTICATE "proxy-authenticate"
#define SPDY_HTTP_HEADER_PROXY_AUTHORIZATION "proxy-authorization"
/* Proxy-Connection header is forbidden in SPDY */
#define SPDY_HTTP_HEADER_PROXY_CONNECTION "proxy-connection"
#define SPDY_HTTP_HEADER_RANGE "range"
#define SPDY_HTTP_HEADER_REFERER "referer"
#define SPDY_HTTP_HEADER_RETRY_AFTER "retry-after"
#define SPDY_HTTP_HEADER_SERVER "server"
#define SPDY_HTTP_HEADER_SET_COOKIE "set-cookie"
#define SPDY_HTTP_HEADER_SET_COOKIE2 "set-cookie2"
#define SPDY_HTTP_HEADER_TE "te"
#define SPDY_HTTP_HEADER_TRAILER "trailer"
/* Transfer-Encoding header is forbidden in SPDY */
#define SPDY_HTTP_HEADER_TRANSFER_ENCODING "transfer-encoding"
#define SPDY_HTTP_HEADER_UPGRADE "upgrade"
#define SPDY_HTTP_HEADER_USER_AGENT "user-agent"
#define SPDY_HTTP_HEADER_VARY "vary"
#define SPDY_HTTP_HEADER_VIA "via"
#define SPDY_HTTP_HEADER_WARNING "warning"
#define SPDY_HTTP_HEADER_WWW_AUTHENTICATE "www-authenticate"

/**
 * HTTP versions (a value must be provided in SPDY requests/responses).
 */
#define SPDY_HTTP_VERSION_1_0 "HTTP/1.0"
#define SPDY_HTTP_VERSION_1_1 "HTTP/1.1"

/**
 * HTTP methods
 */
#define SPDY_HTTP_METHOD_CONNECT "CONNECT"
#define SPDY_HTTP_METHOD_DELETE "DELETE"
#define SPDY_HTTP_METHOD_GET "GET"
#define SPDY_HTTP_METHOD_HEAD "HEAD"
#define SPDY_HTTP_METHOD_OPTIONS "OPTIONS"
#define SPDY_HTTP_METHOD_POST "POST"
#define SPDY_HTTP_METHOD_PUT "PUT"
#define SPDY_HTTP_METHOD_TRACE "TRACE"

/**
 * HTTP POST encodings, see also
 * http://www.w3.org/TR/html4/interact/forms.html#h-17.13.4
 */
#define SPDY_HTTP_POST_ENCODING_FORM_URLENCODED "application/x-www-form-urlencoded"
#define SPDY_HTTP_POST_ENCODING_MULTIPART_FORMDATA "multipart/form-data"


/**
 * Handle for the daemon (listening on a socket).
 */
struct SPDY_Daemon;


/**
 * Handle for a SPDY session/connection.
 */
struct SPDY_Session;


/**
 * Handle for a SPDY request sent by a client. The structure has pointer
 * to the session's handler
 */
struct SPDY_Request;


/**
 * Handle for a response containing HTTP headers and data to be sent.
 * The structure has pointer to the session's handler
 * for this response.
 */
struct SPDY_Response;


/**
 * Collection of tuples of an HTTP header and values used in requests
 * and responses.
 */
struct SPDY_NameValue;


/**
 * Collection of tuples of a SPDY setting ID, value
 * and flags used to control the sessions.
 */
struct SPDY_Settings;


/**
 * SPDY IO sybsystem flags used by SPDY_init() and SPDY_deinit().<p>
 *
 * The values are used internally as flags, that is why they must be
 * powers of 2.
 */
enum SPDY_IO_SUBSYSTEM
{

  /**
   * No subsystem. For internal use.
   */
  SPDY_IO_SUBSYSTEM_NONE = 0,

  /**
   * Default TLS implementation provided by openSSL/libssl.
   */
  SPDY_IO_SUBSYSTEM_OPENSSL = 1,

  /**
   * No TLS is used.
   */
  SPDY_IO_SUBSYSTEM_RAW = 2
};


/**
 * SPDY daemon options. Passed in the varargs portion of
 * SPDY_start_daemon to customize the daemon. Each option must
 * be followed by a value of a specific type.<p>
 *
 * The values are used internally as flags, that is why they must be
 * powers of 2.
 */
enum SPDY_DAEMON_OPTION
{

  /**
   * No more options / last option.  This is used
   * to terminate the VARARGs list.
   */
  SPDY_DAEMON_OPTION_END = 0,

  /**
   * Set a custom timeout for all connections.  Must be followed by
   * a number of seconds, given as an 'unsigned int'.  Use
   * zero for no timeout.
   */
  SPDY_DAEMON_OPTION_SESSION_TIMEOUT = 1,

  /**
   * Bind daemon to the supplied sockaddr. This option must be
   * followed by a 'struct sockaddr *'.  The 'struct sockaddr*'
   * should point to a 'struct sockaddr_in6' or to a
   * 'struct sockaddr_in'.
   */
  SPDY_DAEMON_OPTION_SOCK_ADDR = 2,

  /**
   * Flags for the daemon. Must be followed by a SPDY_DAEMON_FLAG value
   * which is the result of bitwise OR of desired flags.
   */
  SPDY_DAEMON_OPTION_FLAGS = 4,

  /**
   * IO subsystem type used by daemon and all its sessions. If not set,
   * TLS provided by openssl is used. Must be followed by a
   * SPDY_IO_SUBSYSTEM value.
   */
  SPDY_DAEMON_OPTION_IO_SUBSYSTEM = 8,

  /**
   * Maximum number of frames to be written to the socket at once. The
   * library tries to send max_num_frames in a single call to SPDY_run
   * for a single session. This means no requests can be received nor
   * other sessions can send data as long the current one has enough
   * frames to send and there is no error on writing. Thus, a big value
   * will affect the performance. Small value gives fairnes for sessions.
   * Must be followed by a positive integer (uin32_t). If not set, the
   * default value 10 will be used.
   */
  SPDY_DAEMON_OPTION_MAX_NUM_FRAMES = 16
};


/**
 * Flags for starting SPDY daemon. They are used to set some settings
 * for the daemon, which do not require values.
 */
enum SPDY_DAEMON_FLAG
{
  /**
   * No flags selected.
   */
  SPDY_DAEMON_FLAG_NO = 0,

  /**
   * The server will bind only on IPv6 addresses. If the flag is set and
   * the daemon is provided with IPv4 address or IPv6 is not supported,
   * starting daemon will fail.
   */
  SPDY_DAEMON_FLAG_ONLY_IPV6 = 1,

  /**
   * All sessions' sockets will be set with TCP_NODELAY if the flag is
   * used. Option considered only by SPDY_IO_SUBSYSTEM_RAW.
   */
  SPDY_DAEMON_FLAG_NO_DELAY = 2
};


/**
 * SPDY settings IDs sent by both client and server in SPDY SETTINGS frame.
 * They affect the whole SPDY session. Defined in SPDY Protocol - Draft 3.
 */
enum SPDY_SETTINGS
{

  /**
   * Allows the sender to send its expected upload bandwidth on this
   * channel. This number is an estimate. The value should be the
   * integral number of kilobytes per second that the sender predicts
   * as an expected maximum upload channel capacity.
   */
  SPDY_SETTINGS_UPLOAD_BANDWIDTH = 1,

  /**
   * Allows the sender to send its expected download bandwidth on this
   * channel. This number is an estimate. The value should be the
   * integral number of kilobytes per second that the sender predicts as
   * an expected maximum download channel capacity.
   */
  SPDY_SETTINGS_DOWNLOAD_BANDWIDTH = 2,

  /**
   * Allows the sender to send its expected round-trip-time on this
   * channel. The round trip time is defined as the minimum amount of
   * time to send a control frame from this client to the remote and
   * receive a response. The value is represented in milliseconds.
   */
  SPDY_SETTINGS_ROUND_TRIP_TIME = 3,

  /**
   * Allows the sender to inform the remote endpoint the maximum number
   * of concurrent streams which it will allow. By default there is no
   * limit. For implementors it is recommended that this value be no
   * smaller than 100.
   */
  SPDY_SETTINGS_MAX_CONCURRENT_STREAMS = 4,

  /**
   * Allows the sender to inform the remote endpoint of the current TCP
   * CWND value.
   */
  SPDY_SETTINGS_CURRENT_CWND = 5,

  /**
   * Allows the sender to inform the remote endpoint the retransmission
   * rate (bytes retransmitted / total bytes transmitted).
   */
  SPDY_SETTINGS_DOWNLOAD_RETRANS_RATE = 6,

  /**
   * Allows the sender to inform the remote endpoint the initial window
   * size (in bytes) for new streams.
   */
  SPDY_SETTINGS_INITIAL_WINDOW_SIZE = 7,

  /**
   * Allows the server to inform the client if the new size of the
   * client certificate vector.
   */
  SPDY_SETTINGS_CLIENT_CERTIFICATE_VECTOR_SIZE = 8
};


/**
 * Flags for each individual SPDY setting in the SPDY SETTINGS frame.
 * They affect only one setting to which they are set.
 * Defined in SPDY Protocol - Draft 3.
 */
enum SPDY_FLAG_SETTINGS
{

  /**
   * When set, the sender of this SETTINGS frame is requesting that the
   * recipient persist the ID/Value and return it in future SETTINGS
   * frames sent from the sender to this recipient. Because persistence
   * is only implemented on the client, this flag is only sent by the
   * server.
   */
  SPDY_FLAG_SETTINGS_PERSIST_VALUE = 1,

  /**
   * When set, the sender is notifying the recipient that this ID/Value
   * pair was previously sent to the sender by the recipient with the
   * #SPDY_FLAG_SETTINGS_PERSIST_VALUE, and the sender is returning it.
   * Because persistence is only implemented on the client, this flag is
   * only sent by the client.
   */
  SPDY_FLAG_SETTINGS_PERSISTED = 2
};


/**
 * Flag associated with a whole SPDY SETTINGS frame. Affect all the
 * settings in the frame. Defined in SPDY Protocol - Draft 3.
 */
enum SPDY_FLAG_SETTINGS_FRAME
{

  /**
   * When set, the client should clear any previously persisted SETTINGS
   * ID/Value pairs. If this frame contains ID/Value pairs with the
   * #SPDY_FLAG_SETTINGS_PERSIST_VALUE set, then the client will first
   * clear its existing, persisted settings, and then persist the values
   * with the flag set which are contained within this frame. Because
   * persistence is only implemented on the client, this flag can only
   * be used when the sender is the server.
   */
  SPDY_FLAG_SETTINGS_CLEAR_SETTINGS = 1
};


/**
 * SPDY settings function options. Passed in the varargs portion of
 * SPDY_SettingsReceivedCallback and SPDY_send_settings to customize
 * more the settings handling. Each option must
 * be followed by a value of a specific type.<p>
 *
 * The values are used internally as flags, that is why they must be
 * powers of 2.
 */
enum SPDY_SETTINGS_OPTION
{

  /**
   * No more options / last option.  This is used
   * to terminate the VARARGs list.
   */
  SPDY_SETTINGS_OPTION_END = 0
};


/**
 * Used as a parameter for SPDY_ResponseResultCallback and shows if the
 * response was actually written to the TLS socket or discarded by the
 * lib for any reason (and respectively the reason).
 */
enum SPDY_RESPONSE_RESULT
{

  /**
   * The lib has written the full response to the TLS socket.
   */
  SPDY_RESPONSE_RESULT_SUCCESS = 0,

  /**
   * The session is being closed, so the data is being discarded
   */
  SPDY_RESPONSE_RESULT_SESSION_CLOSED = 1,

  /**
   * The stream for this response has been closed. May happen when the
   * sender had sent first SYN_STREAM and after that RST_STREAM.
   */
  SPDY_RESPONSE_RESULT_STREAM_CLOSED = 2
};


/**
 * Callback for serious error condition. The default action is to print
 * an error message and abort().
 *
 * @param cls user specified value
 * @param file where the error occured
 * @param line where the error occured
 * @param reason error details message, may be NULL
 */
typedef void
(*SPDY_PanicCallback) (void * cls,
                       const char *file,
                       unsigned int line,
                       const char *reason);


/**
 * Callback for new SPDY session established by a client. Called
 * immediately after the TCP connection was established.
 *
 * @param cls client-defined closure
 * @param session handler for the new SPDY session
 */
typedef void
(*SPDY_NewSessionCallback) (void * cls,
                            struct SPDY_Session * session);


/**
 * Callback for closed session. Called after the TCP connection was
 * closed. In this callback function the user has the last
 * chance to access the SPDY_Session structure. After that the latter
 * will be cleaned!
 *
 * @param cls client-defined closure
 * @param session handler for the closed SPDY session
 * @param by_client #SPDY_YES if the session close was initiated by the
 * 					client;
 * 		    #SPDY_NO if closed by the server
 */
typedef void
(*SPDY_SessionClosedCallback) (void *cls,
                               struct SPDY_Session *session,
                               int by_client);


/**
 * Iterator over name-value pairs.
 *
 * @param cls client-defined closure
 * @param name of the pair
 * @param value of the pair
 * @return #SPDY_YES to continue iterating,
 *         #SPDY_NO to abort the iteration
 */
typedef int
(*SPDY_NameValueIterator) (void *cls,
                           const char *name,
                           const char * const * value,
                           int num_values);


/**
 * Callback for received SPDY request. The functions is called whenever
 * a reqest comes, but will also be called if more headers/trailers are
 * received.
 *
 * @param cls client-defined closure
 * @param request handler. The request object is required for
 * 			sending responses.
 * @param priority of the SPDY stream which the request was
 * 			sent over
 * @param method HTTP method
 * @param path HTTP path
 * @param version HTTP version just like in HTTP request/response:
 * 			"HTTP/1.0" or "HTTP/1.1" currently
 * @param host called host as in HTTP
 * @param scheme used ("http" or "https"). In SPDY 3 it is only "https".
 * @param headers other HTTP headers from the request
 * @param more a flag saying if more data related to the request is
 *        expected to be received. HTTP body may arrive (e.g. POST data);
 *        then SPDY_NewDataCallback will be called for the connection.
 *        It is also possible that more headers/trailers arrive;
 *        then the same callback will be invoked. The user should detect
 *        that it is not the first invocation of the function for that
 *        request.
 */
typedef void
(*SPDY_NewRequestCallback) (void *cls,
                            struct SPDY_Request *request,
                            uint8_t priority,
                            const char *method,
                            const char *path,
                            const char *version,
                            const char *host,
                            const char *scheme,
                            struct SPDY_NameValue *headers,
                            bool more);


/**
 * Callback for received new data chunk (HTTP body) from a given
 * request (e.g. POST data).
 *
 * @param cls client-defined closure
 * @param request handler
 * @param buf data chunk from the POST data
 * @param size the size of the data chunk 'buf' in bytes. Note that it
 *             may be 0.
 * @param more false if this is the last chunk from the data. Note:
 *             true does not mean that more data will come, exceptional
 *             situation is possible
 * @return #SPDY_YES to continue calling the function,
 *         #SPDY_NO to stop calling the function for this request
 */
typedef int
(*SPDY_NewDataCallback) (void *cls,
                         struct SPDY_Request *request,
                         const void *buf,
                         size_t size,
                         bool more);
// How about passing POST encoding information
// here as well?
//TODO


/**
 * Callback to be used with SPDY_build_response_with_callback. The
 * callback will be called when the lib wants to write to the TLS socket.
 * The application should provide the data to be sent.
 *
 * @param cls client-defined closure
 * @param max maximum number of bytes that are allowed to be written
 * 			to the buffer.
 * @param more true if more data will be sent (i.e. the function must
 * 				be calleed again),
 *             false if this is the last chunk, the lib will close
 * 				the stream
 * @return number of bytes written to buffer. On error the call MUST
 * 			return value less than 0 to indicate the library.
 */
typedef ssize_t
(*SPDY_ResponseCallback) (void *cls,
                          void *buffer,
                          size_t max,
                          bool *more);


/**
 * Callback to be called when the last bytes from the response was sent
 * to the client or when the response was discarded from the lib. This
 * callback is a very good place to discard the request and the response
 * objects, if they will not be reused (e.g., sending the same response
 * again). If the stream is closed it is safe to discard the request
 * object.
 *
 * @param cls client-defined closure
 * @param response handler to the response that was just sent
 * @param request handler to the request for which the response was sent
 * @param status shows if actually the response was sent or it was
 * 			discarded by the lib for any reason (e.g., closing session,
 * 			closing stream, stopping daemon, etc.). It is possible that
 * 			status indicates an error but parts of the response headers
 * 			and/or body (in one
 * 			or several frames) were already sent to the client.
 * @param streamopened indicates if the the stream for this request/
 * 			response pair is still opened. If yes, the server may want
 * 			to use SPDY push to send something additional to the client
 * 			and/or close the stream.
 */
typedef void
(*SPDY_ResponseResultCallback) (void * cls,
                                struct SPDY_Response *response,
                                struct SPDY_Request *request,
                                enum SPDY_RESPONSE_RESULT status,
                                bool streamopened);


/**
 * Callback to notify when SPDY ping response is received.
 *
 * @param session handler for which the ping request was sent
 * @param rtt the timespan between sending ping request and receiving it
 * 			from the library
 */
typedef void
(*SPDY_PingCallback) (void * cls,
                      struct SPDY_Session *session,
                      struct timeval *rtt);


/**
 * Iterator over settings ID/Value/Flags tuples.
 *
 * @param cls client-defined closure
 * @param id SPDY settings ID
 * @param value value for this setting
 * @param flags flags for this tuple; use
 * 			`enum SPDY_FLAG_SETTINGS`
 * @return #SPDY_YES to continue iterating,
 *         #SPDY_NO to abort the iteration
 */
typedef int
(*SPDY_SettingsIterator) (void *cls,
                          enum SPDY_SETTINGS id,
                          int32_t value,
                          uint8_t flags);


/**
 * Callback to notify when SPDY SETTINGS are received from the client.
 *
 * @param session handler for which settings are received
 * @param settings ID/value/flags tuples of the settings
 * @param flags for the whole settings frame; use
 * 			enum SPDY_FLAG_SETTINGS_FRAME
 * @param ... list of options (type-value pairs,
 *        terminated with #SPDY_SETTINGS_OPTION_END).
 */
typedef void
(*SPDY_SettingsReceivedCallback) (struct SPDY_Session *session,
                                  struct SPDY_Settings *settings,
                                  uint8_t flags,
                                  ...);


/* Global functions for the library */


/**
 * Init function for the whole library. It MUST be called before any
 * other function of the library to initialize things like TLS context
 * and possibly other stuff needed by the lib. Currently the call
 * always returns #SPDY_YES.
 *
 * @param io_subsystem the IO subsystem that will
 *        be initialized. Several can be used with bitwise OR. If no
 *        parameter is set, the default openssl subsystem will be used.
 * @return #SPDY_YES if the library was correctly initialized and its
 * 			functions can be used now;
 * 			#SPDY_NO on error
 */
_MHD_EXTERN int
(SPDY_init) (enum SPDY_IO_SUBSYSTEM io_subsystem, ...);
#define SPDY_init() SPDY_init (SPDY_IO_SUBSYSTEM_OPENSSL)


/**
 * Deinit function for the whole lib. It can be called after finishing
 * using the library. It frees and cleans up resources allocated in
 * SPDY_init. Currently the function does not do anything.
 */
_MHD_EXTERN void
SPDY_deinit (void);


/**
 * Sets the global error handler to a different implementation. "cb"
 * will only be called in the case of typically fatal, serious
 * internal consistency issues.  These issues should only arise in the
 * case of serious memory corruption or similar problems with the
 * architecture as well as failed assertions.  While "cb" is allowed to
 * return and the lib will then try to continue, this is never safe.
 *
 * The default implementation that is used if no panic function is set
 * simply prints an error message and calls "abort".  Alternative
 * implementations might call "exit" or other similar functions.
 *
 * @param cb new error handler
 * @param cls passed to error handler
 */
_MHD_EXTERN void
SPDY_set_panic_func (SPDY_PanicCallback cb,
                     void *cls);


/* Daemon functions */


/**
 * Start a SPDY webserver on the given port.
 *
 * @param port to bind to. The value is ignored if address structure
 * 			is passed as daemon option
 * @param certfile path to the certificate that will be used by server
 * @param keyfile path to the keyfile for the certificate
 * @param nscb callback called when a new SPDY session is
 * 			established	by a client
 * @param sccb callback called when a session is closed
 * @param nrcb callback called when a client sends request
 * @param npdcb callback called when HTTP body (POST data) is received
 * 			after request
 * @param cls common extra argument to all of the callbacks
 * @param ... list of options (type-value pairs,
 *        terminated with #SPDY_DAEMON_OPTION_END).
 * @return NULL on error, handle to daemon on success
 */
_MHD_EXTERN struct SPDY_Daemon *
SPDY_start_daemon (uint16_t port,
                   const char *certfile,
                   const char *keyfile,
                   SPDY_NewSessionCallback nscb,
                   SPDY_SessionClosedCallback sccb,
                   SPDY_NewRequestCallback nrcb,
                   SPDY_NewDataCallback npdcb,
                   void *cls,
                   ...);


/**
 * Shutdown the daemon. First all sessions are closed. It is NOT safe
 * to call this function in user callbacks.
 *
 * @param daemon to stop
 */
_MHD_EXTERN void
SPDY_stop_daemon (struct SPDY_Daemon *daemon);


/**
 * Obtain the select sets for this daemon. Only those are retrieved,
 * which some processing should be done for, i.e. not all sockets are
 * added to write_fd_set.<p>
 *
 * It is possible that there is
 * nothing to be read from a socket but there is data either in the
 * TLS subsystem's read buffers or in libmicrospdy's read buffers, which
 * waits for being processed. In such case the file descriptor will be
 * added to write_fd_set. Since it is very likely for the socket to be
 * ready for writing, the select used in the application's event loop
 * will return with success, SPDY_run will be called, the data will be
 * processed and maybe something will be written to the socket. Without
 * this behaviour, considering a proper event loop, data may stay in the
 * buffers, but run is never called.
 *
 * @param daemon to get sets from
 * @param read_fd_set read set
 * @param write_fd_set write set
 * @param except_fd_set except set
 * @return largest FD added to any of the sets
 */
_MHD_EXTERN int
SPDY_get_fdset (struct SPDY_Daemon *daemon,
                fd_set *read_fd_set,
                fd_set *write_fd_set,
                fd_set *except_fd_set);


/**
 * Obtain timeout value for select for this daemon. The returned value
 * is how long select
 * should at most block, not the timeout value set for connections.
 *
 * @param daemon to query for timeout
 * @param timeout will be set to the timeout value (in milliseconds)
 * @return #SPDY_YES on success
 *         #SPDY_NO if no connections exist that
 * 			would necessiate the use of a timeout right now
 */
_MHD_EXTERN int
SPDY_get_timeout (struct SPDY_Daemon *daemon,
                  unsigned long long *timeout);


/**
 * Run webserver operations. This method must be called in
 * the client event loop.
 *
 * @param daemon to run
 */
_MHD_EXTERN void
SPDY_run (struct SPDY_Daemon *daemon);


/* SPDY Session handling functions */


/**
 * Closes a SPDY session. SPDY clients and servers are expected to keep
 * sessions opened as long as possible. However, the server may want to
 * close some connections, e.g. if there are too many, to free some
 * resources. The function can also be used to close a specific session
 * if the client is not desired.
 *
 * @param session handler to be closed
 */
_MHD_EXTERN void
SPDY_close_session (struct SPDY_Session * session);


/**
 * Associate a void pointer with a session. The data accessible by the
 * pointer can later be used wherever the session handler is available.
 *
 * @param session handler
 * @param cls any data pointed by a pointer to be accessible later
 */
_MHD_EXTERN void
SPDY_set_cls_to_session (struct SPDY_Session *session,
                         void *cls);


/**
 * Retrieves the pointer associated with SPDY_set_cls_to_session().
 *
 * @param session handler to get its cls
 * @return same pointer added by SPDY_set_cls_to_session() or
 * 			NULL when nothing was associated
 */
_MHD_EXTERN void *
SPDY_get_cls_from_session (struct SPDY_Session *session);


/**
 * Retrieves the remote address of a given session.
 *
 * @param session handler to get its remote address
 * @param addr out parameter; pointing to remote address
 * @return length of the address structure
 */
_MHD_EXTERN socklen_t
SPDY_get_remote_addr (struct SPDY_Session *session,
                      struct sockaddr **addr);


/* SPDY name/value data structure handling functions */


/**
 * Create a new NameValue structure. It is needed for putting inside the
 * HTTP headers and their values for a response. The user should later
 * destroy alone the structure.
 *
 * @return handler to the new empty structure or NULL on error
 */
_MHD_EXTERN struct SPDY_NameValue *
SPDY_name_value_create (void);


/**
 * Add name/value pair to a NameValue structure. SPDY_NO will be returned
 * if the name/value pair is already in the structure. It is legal to
 * add different values for the same name.
 *
 * @param container structure to which the new pair is added
 * @param name for the value. Null-terminated string.
 * @param value the value itself. Null-terminated string.
 * @return #SPDY_NO on error or #SPDY_YES on success
 */
_MHD_EXTERN int
SPDY_name_value_add (struct SPDY_NameValue *container,
                     const char *name,
                     const char *value);


/**
 * Lookup value for a name in a name/value structure.
 *
 * @param container structure in which to lookup
 * @param name the name to look for
 * @param num_values length of the returned array with values
 * @return NULL if no such item was found, or an array containing the
 * 			values
 */
_MHD_EXTERN const char * const *
SPDY_name_value_lookup (struct SPDY_NameValue *container,
                        const char *name,
                        int *num_values);


/**
 * Iterate over name/value structure.
 *
 * @param container structure which to iterate over
 * @param iterator callback to call on each name/value pair;
 *        maybe NULL (then just count headers)
 * @param iterator_cls extra argument to @a iterator
 * @return number of entries iterated over
 */
_MHD_EXTERN int
SPDY_name_value_iterate (struct SPDY_NameValue *container,
                         SPDY_NameValueIterator iterator,
                         void *iterator_cls);


/**
 * Destroy a NameValue structure. Use this function to destroy only
 * objects which, after passed to, will not be destroied by other
 * functions.
 *
 */
_MHD_EXTERN void
SPDY_name_value_destroy (struct SPDY_NameValue *container);


/* SPDY request handling functions */


/**
 * Gets the session responsible for the given
 * request.
 *
 * @param request for which the session is wanted
 * @return session handler for the request
 */
_MHD_EXTERN struct SPDY_Session *
SPDY_get_session_for_request (const struct SPDY_Request *request);


/**
 * Associate a void pointer with a request. The data accessible by the
 * pointer can later be used wherever the request handler is available.
 *
 * @param request with which to associate a pointer
 * @param cls any data pointed by a pointer to be accessible later
 */
_MHD_EXTERN void
SPDY_set_cls_to_request (struct SPDY_Request *request,
                         void *cls);


/**
 * Retrieves the pointer associated with the request by
 * SPDY_set_cls_to_request().
 *
 * @param request to get its cls
 * @return same pointer added by SPDY_set_cls_to_request() or
 * 			NULL when nothing was associated
 */
_MHD_EXTERN void *
SPDY_get_cls_from_request (struct SPDY_Request *request);


/* SPDY response handling functions */


/**
 * Create response object containing all needed headers and data. The
 * response object is not bound to a request, so it can be used multiple
 * times with SPDY_queue_response() and schould be
 * destroied by calling the SPDY_destroy_response().<p>
 *
 * Currently the library does not provide compression of the body data.
 * It is up to the user to pass already compressed data and the
 * appropriate headers to this function when desired.
 *
 * @param status HTTP status code for the response (e.g. 404)
 * @param statustext HTTP status message for the response, which will
 * 			be appended to the status code (e.g. "OK"). Can be NULL
 * @param version HTTP version for the response (e.g. "http/1.1")
 * @param headers name/value structure containing additional HTTP headers.
 *                Can be NULL. Can be used multiple times, it is up to
 *                the user to destoy the object when not needed anymore.
 * @param data the body of the response. The lib will make a copy of it,
 *             so it is up to the user to take care of the memory
 *             pointed by data
 * @param size length of @a data. It can be 0, then the lib will send only
 * 				headers
 * @return NULL on error, handle to response object on success
 */
_MHD_EXTERN struct SPDY_Response *
SPDY_build_response (int status,
                     const char *statustext,
                     const char *version,
                     struct SPDY_NameValue *headers,
                     const void *data,
                     size_t size);


/**
 * Create response object containing all needed headers. The data will
 * be provided later when the lib calls the callback function (just
 * before writing it to the TLS socket). The
 * response object is not bound to a request, so it can be used multiple
 * times with SPDY_queue_response() and schould be
 * destroied by calling the SPDY_destroy_response().<p>
 *
 * Currently the library does not provide compression of the body data.
 * It is up to the user to pass already compressed data and the
 * appropriate headers to this function and the callback when desired.
 *
 * @param status HTTP status code for the response (e.g. 404)
 * @param statustext HTTP status message for the response, which will
 * 			be appended to the status code (e.g. "OK"). Can be NULL
 * @param version HTTP version for the response (e.g. "http/1.1")
 * @param headers name/value structure containing additional HTTP headers.
 *                Can be NULL. Can be used multiple times, it is up to
 *                the user to destoy the object when not needed anymore.
 * @param rcb callback to use to obtain response data
 * @param rcb_cls extra argument to @a rcb
 * @param block_size preferred block size for querying rcb (advisory only,
 *                   the lib will call rcb specifying the block size); clients
 *                   should pick a value that is appropriate for IO and
 *                   memory performance requirements. The function will
 *                   fail if the value is bigger than the maximum
 *                   supported value (SPDY_MAX_SUPPORTED_FRAME_SIZE).
 *                   Can be 0, then the lib will use
 *                   #SPDY_MAX_SUPPORTED_FRAME_SIZE instead.
 * @return NULL on error, handle to response object on success
 */
_MHD_EXTERN struct SPDY_Response *
SPDY_build_response_with_callback(int status,
                                  const char *statustext,
                                  const char *version,
                                  struct SPDY_NameValue *headers,
                                  SPDY_ResponseCallback rcb,
                                  void *rcb_cls,
                                  uint32_t block_size);


/**
 * Queue response object to be sent to the client. A successfully queued
 * response may never be sent, e.g. when the stream gets closed. The
 * data will be added to the output queue. The call will fail, if the
 * output for this session
 * is closed (i.e. the session is closed, half or full) or the output
 * channel for the stream, on which the request was received, is closed
 * (i.e. the stream is closed, half or full).
 *
 * @param request object identifying the request to which the
 * 			response is returned
 * @param response object containg headers and data to be sent
 * @param closestream TRUE if the server does NOT intend to PUSH
 * 			something more associated to this request/response later,
 * 			FALSE otherwise
 * @param consider_priority if FALSE, the response will be added to the
 * 			end of the queue. If TRUE, the response will be added after
 * 			the last previously added response with priority of the
 * 			request grater or equal to that of the current one. This
 * 			means that the function should be called with TRUE each time
 * 			if one wants to be sure that the output queue behaves like
 * 			a priority queue
 * @param rrcb callback called when all the data was sent (last frame
 * 			from response) or when that frame was discarded (e.g. the
 * 			stream has been closed meanwhile)
 * @param rrcb_cls extra argument to @a rrcb
 * @return #SPDY_NO on error or #SPDY_YES on success
 */
_MHD_EXTERN int
SPDY_queue_response (struct SPDY_Request *request,
                     struct SPDY_Response *response,
                     bool closestream,
                     bool consider_priority,
                     SPDY_ResponseResultCallback rrcb,
                     void *rrcb_cls);


/**
 * Destroy a response structure. It should be called for all objects
 * returned by SPDY_build_response*() functions to free the memory
 * associated with the prepared response. It is safe to call this
 * function not before being sure that the response will not be used by
 * the lib anymore, this means after SPDY_ResponseResultCallback
 * callbacks were called for all calls to SPDY_queue_response() passing
 * this response.
 *
 * @param response to destroy
 */
_MHD_EXTERN void
SPDY_destroy_response (struct SPDY_Response *response);


/* SPDY settings ID/value data structure handling functions */


/**
 * Create a new SettingsIDValue structure. It is needed for putting
 * inside tuples of SPDY option, flags and value for sending to the
 * client.
 *
 * @return hendler to the new empty structure or NULL on error
 */
_MHD_EXTERN const struct SPDY_Settings *
SPDY_settings_create (void);


/**
 * Add or update a tuple to a SettingsIDValue structure.
 *
 * @param container structure to which the new tuple is added
 * @param id SPDY settings ID that will be sent. If this ID already in
 *           container, the tupple for it will be updated (value and/or
 *           flags). If it is not in the container, a new tupple will be
 *           added.
 * @param flags SPDY settings flags applied only to this setting
 * @param value of the setting
 * @return #SPDY_NO on error
 * 			or #SPDY_YES if a new setting was added
 */
_MHD_EXTERN int
SPDY_settings_add (struct SPDY_Settings *container,
                   enum SPDY_SETTINGS id,
                   enum SPDY_FLAG_SETTINGS flags,
                   int32_t value);


/**
 * Lookup value and flags for an ID in a settings ID/value structure.
 *
 * @param container structure in which to lookup
 * @param id SPDY settings ID to search for
 * @param flags out param for SPDY settings flags for this setting;
 * 			check it against the flags in enum SPDY_FLAG_SETTINGS
 * @param value out param for the value of this setting
 * @return #SPDY_NO if the setting is not into the structure
 * 			or #SPDY_YES if it is into it
 */
_MHD_EXTERN int
SPDY_settings_lookup (const struct SPDY_Settings *container,
                      enum SPDY_SETTINGS id,
                      enum SPDY_FLAG_SETTINGS *flags,
                      int32_t *value);


/**
 * Iterate over settings ID/value structure.
 *
 * @param container structure which to iterate over
 * @param iterator callback to call on each ID/value pair;
 *        maybe NULL (then just count number of settings)
 * @param iterator_cls extra argument to iterator
 * @return number of entries iterated over
 */
_MHD_EXTERN int
SPDY_settings_iterate (const struct SPDY_Settings *container,
                       SPDY_SettingsIterator iterator,
                       void *iterator_cls);


/**
 * Destroy a settings ID/value structure. Use this function to destroy
 * only objects which, after passed to, will not be destroied by other
 * functions.
 *
 * @param container structure which to detroy
 */
_MHD_EXTERN void
SPDY_settings_destroy (struct SPDY_Settings * container);


/* SPDY SETTINGS handling functions */


/**
 * Send SPDY SETTINGS to the client. The call will return fail if there
 * in invald setting into the settings container (e.g. invalid setting
 * ID).
 *
 * @param session SPDY_Session handler for which settings are being sent
 * @param settings ID/value pairs of the settings to be sent.
 * 			Can be used multiple times, it is up to the user to destoy
 * 			the object when not needed anymore.
 * @param flags for the whole settings frame. They are valid for all tuples
 * @param ... list of options (type-value pairs,
 *        terminated with #SPDY_SETTINGS_OPTION_END).
 * @return SPDY_NO on error or SPDY_YES on
 * 			success
 */
_MHD_EXTERN int
SPDY_send_settings (struct SPDY_Session *session,
                    struct SPDY_Settings *settings,
                    enum SPDY_FLAG_SETTINGS_FRAME flags,
                    ...);


/* SPDY misc functions */


/**
 * Destroy a request structure. It should be called for all objects
 * received as a parameter in SPDY_NewRequestCallback to free the memory
 * associated with the request. It is safe to call this
 * function not before being sure that the request will not be used by
 * the lib anymore, this means after the stream, on which this request
 * had been sent, was closed and all SPDY_ResponseResultCallback
 * callbacks were called for all calls to SPDY_queue_response() passing
 * this request object.
 *
 * @param request to destroy
 */
_MHD_EXTERN void
SPDY_destroy_request (struct SPDY_Request * request);


/**
 * Send SPDY ping to the client
 *
 * @param session handler for which the ping request is sent
 * @param rttcb callback called when ping response to the request is
 * 			received
 * @param rttcb_cls extra argument to @a rttcb
 * @return #SPDY_NO on error or #SPDY_YES on success
 */
_MHD_EXTERN int
SPDY_send_ping (struct SPDY_Session *session,
                SPDY_PingCallback rttcb,
                void *rttcb_cls);

#endif
