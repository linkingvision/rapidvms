/*
    This file is part of libmicrospdy
    Copyright Copyright (C) 2012 Andrey Uzunov

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
 * @file structures.h
 * @brief  internal and public structures -- most of the structs used by
 * 			the library are defined here
 * @author Andrey Uzunov
 */

#ifndef STRUCTURES_H
#define STRUCTURES_H

#include "platform.h"
#include "microspdy.h"
#include "io.h"


/**
 * All possible SPDY control frame types. The number is used in the header
 * of the control frame.
 */
enum SPDY_CONTROL_FRAME_TYPES
{
	/**
	 * The SYN_STREAM control frame allows the sender to asynchronously
	 * create a stream between the endpoints.
	 */
	SPDY_CONTROL_FRAME_TYPES_SYN_STREAM = 1,
	
	/**
	 * SYN_REPLY indicates the acceptance of a stream creation by
	 * the recipient of a SYN_STREAM frame.
	 */
	SPDY_CONTROL_FRAME_TYPES_SYN_REPLY = 2,
	
	/**
	 * The RST_STREAM frame allows for abnormal termination of a stream.
	 * When sent by the creator of a stream, it indicates the creator
	 * wishes to cancel the stream. When sent by the recipient of a
	 * stream, it indicates an error or that the recipient did not want
	 * to accept the stream, so the stream should be closed.
	 */
	SPDY_CONTROL_FRAME_TYPES_RST_STREAM = 3,
	
	/**
	 * A SETTINGS frame contains a set of id/value pairs for
	 * communicating configuration data about how the two endpoints may
	 * communicate. SETTINGS frames can be sent at any time by either
	 * endpoint, are optionally sent, and are fully asynchronous. When
	 * the server is the sender, the sender can request that
	 * configuration data be persisted by the client across SPDY
	 * sessions and returned to the server in future communications.
	 */
	SPDY_CONTROL_FRAME_TYPES_SETTINGS = 4,
	
	/**
	 * The PING control frame is a mechanism for measuring a minimal
	 * round-trip time from the sender. It can be sent from the client
	 * or the server. Recipients of a PING frame should send an
	 * identical frame to the sender as soon as possible (if there is
	 * other pending data waiting to be sent, PING should take highest
	 * priority). Each ping sent by a sender should use a unique ID.
	 */
	SPDY_CONTROL_FRAME_TYPES_PING = 6,
	
	/**
	 * The GOAWAY control frame is a mechanism to tell the remote side
	 * of the connection to stop creating streams on this session. It
	 * can be sent from the client or the server.
	 */
	SPDY_CONTROL_FRAME_TYPES_GOAWAY = 7,
	
	/**
	 * The HEADERS frame augments a stream with additional headers. It
	 * may be optionally sent on an existing stream at any time.
	 * Specific application of the headers in this frame is
	 * application-dependent. The name/value header block within this
	 * frame is compressed.
	 */
	SPDY_CONTROL_FRAME_TYPES_HEADERS = 8,
	
	/**
	 * The WINDOW_UPDATE control frame is used to implement per stream
	 * flow control in SPDY. Flow control in SPDY is per hop, that is,
	 * only between the two endpoints of a SPDY connection. If there are
	 * one or more intermediaries between the client and the origin
	 * server, flow control signals are not explicitly forwarded by the
	 * intermediaries.
	 */
	SPDY_CONTROL_FRAME_TYPES_WINDOW_UPDATE = 9,
	
	/**
	 * The CREDENTIAL control frame is used by the client to send
	 * additional client certificates to the server. A SPDY client may
	 * decide to send requests for resources from different origins on
	 * the same SPDY session if it decides that that server handles both
	 * origins. For example if the IP address associated with both
	 * hostnames matches and the SSL server certificate presented in the
	 * initial handshake is valid for both hostnames. However, because
	 * the SSL connection can contain at most one client certificate,
	 * the client needs a mechanism to send additional client
	 * certificates to the server.
	 */
	SPDY_CONTROL_FRAME_TYPES_CREDENTIAL = 11
};


/**
 * SPDY_SESSION_STATUS is used to show the current receiving state 
 * of each session, i.e. what is expected to come now, and how it should
 * be handled.
 */
enum SPDY_SESSION_STATUS
{
	/**
	 * The session is in closing state, do not read read anything from
	 * it. Do not write anything to it.
	 */
	SPDY_SESSION_STATUS_CLOSING = 0,
	
	/**
	 * Wait for new SPDY frame to come.
	 */
	SPDY_SESSION_STATUS_WAIT_FOR_HEADER = 1,
	
	/**
	 * The standard 8 byte header of the SPDY frame was received and
	 * handled. Wait for the specific (sub)headers according to the
	 * frame type.
	 */
	SPDY_SESSION_STATUS_WAIT_FOR_SUBHEADER = 2,
	
	/**
	 * The specific (sub)headers were received and handled. Wait for the
	 * "body", i.e. wait for the name/value pairs compressed by zlib.
	 */
	SPDY_SESSION_STATUS_WAIT_FOR_BODY = 3,
	
	/**
	 * Ignore all the bytes read from the socket, e.g. larger frames.
	 */
	SPDY_SESSION_STATUS_IGNORE_BYTES= 4,
	
	/**
	 * The session is in pre-closing state, do not read read anything
	 * from it. In this state the output queue will be written to the
	 * socket.
	 */
	SPDY_SESSION_STATUS_FLUSHING = 5,
};


/**
 * Specific flags for the SYN_STREAM control frame.
 */
enum SPDY_SYN_STREAM_FLAG
{
	/**
	 * The sender won't send any more frames on this stream.
	 */
	SPDY_SYN_STREAM_FLAG_FIN = 1,
	
	/**
	 * The sender creates this stream as unidirectional.
	 */
	SPDY_SYN_STREAM_FLAG_UNIDIRECTIONAL = 2
};


/**
 * Specific flags for the SYN_REPLY control frame.
 */
enum SPDY_SYN_REPLY_FLAG
{
	/**
	 * The sender won't send any more frames on this stream.
	 */
	SPDY_SYN_REPLY_FLAG_FIN = 1
};


/**
 * Specific flags for the data frame.
 */
enum SPDY_DATA_FLAG
{
	/**
	 * The sender won't send any more frames on this stream.
	 */
	SPDY_DATA_FLAG_FIN = 1,
	
	/**
	 * The data in the frame is compressed. 
	 * This flag appears only in the draft on ietf.org but not on
	 * chromium.org.
	 */
	SPDY_DATA_FLAG_COMPRESS = 2
};

/**
 * Status code within RST_STREAM control frame.
 */
enum SPDY_RST_STREAM_STATUS
{
	/**
	 * This is a generic error, and should only be used if a more
	 * specific error is not available.
	 */
	SPDY_RST_STREAM_STATUS_PROTOCOL_ERROR = 1,
	
	/**
	 * This is returned when a frame is received for a stream which is
	 * not active.
	 */
	SPDY_RST_STREAM_STATUS_INVALID_STREAM = 2,
	
	/**
	 * Indicates that the stream was refused before any processing has
	 * been done on the stream.
	 */
	SPDY_RST_STREAM_STATUS_REFUSED_STREAM = 3,
	
	/**
	 * Indicates that the recipient of a stream does not support the
	 * SPDY version requested.
	 */
	SPDY_RST_STREAM_STATUS_UNSUPPORTED_VERSION = 4,
	
	/**
	 * Used by the creator of a stream to indicate that the stream is
	 * no longer needed.
	 */
	SPDY_RST_STREAM_STATUS_CANCEL = 5,
	
	/**
	 * This is a generic error which can be used when the implementation
	 * has internally failed, not due to anything in the protocol.
	 */
	SPDY_RST_STREAM_STATUS_INTERNAL_ERROR = 6,
	
	/**
	 * The endpoint detected that its peer violated the flow control
	 * protocol.
	 */
	SPDY_RST_STREAM_STATUS_FLOW_CONTROL_ERROR = 7,
	
	/**
	 * The endpoint received a SYN_REPLY for a stream already open.
	 */
	SPDY_RST_STREAM_STATUS_STREAM_IN_USE = 8,
	
	/**
	 * The endpoint received a data or SYN_REPLY frame for a stream
	 * which is half closed.
	 */
	SPDY_RST_STREAM_STATUS_STREAM_ALREADY_CLOSED = 9,
	
	/**
	 * The server received a request for a resource whose origin does
	 * not have valid credentials in the client certificate vector.
	 */
	SPDY_RST_STREAM_STATUS_INVALID_CREDENTIALS = 10,
	
	/**
	 * The endpoint received a frame which this implementation could not
	 * support. If FRAME_TOO_LARGE is sent for a SYN_STREAM, HEADERS,
	 * or SYN_REPLY frame without fully processing the compressed
	 * portion of those frames, then the compression state will be
	 * out-of-sync with the other endpoint. In this case, senders of
	 * FRAME_TOO_LARGE MUST close the session.
	 */
	SPDY_RST_STREAM_STATUS_FRAME_TOO_LARGE = 11
};


/**
 * Status code within GOAWAY control frame.
 */
enum SPDY_GOAWAY_STATUS
{
	/**
	 * This is a normal session teardown.
	 */
	SPDY_GOAWAY_STATUS_OK = 0,
	
	/**
	 * This is a generic error, and should only be used if a more
	 * specific error is not available.
	 */
	SPDY_GOAWAY_STATUS_PROTOCOL_ERROR = 1,
	
	/**
	 * This is a generic error which can be used when the implementation
	 * has internally failed, not due to anything in the protocol.
	 */
	SPDY_GOAWAY_STATUS_INTERNAL_ERROR = 11
};


struct SPDYF_Stream;

struct SPDYF_Response_Queue;


/**
 * Callback for received new data chunk.
 *
 * @param cls client-defined closure
 * @param stream handler
 * @param buf data chunk from the data
 * @param size the size of the data chunk 'buf' in bytes
 * @param more false if this is the last frame received on this stream. Note:
 *             true does not mean that more data will come, exceptional
 *             situation is possible
 * @return SPDY_YES to continue calling the function,
 *         SPDY_NO to stop calling the function for this stream
 */
typedef int
(*SPDYF_NewDataCallback) (void * cls,
					 struct SPDYF_Stream *stream,
					 const void * buf,
					 size_t size,
					 bool more);
           
           
/**
 * Callback for new stream. To be used in the application layer of the
 * lib.
 *
 * @param cls
 * @param stream the new stream
 * @return SPDY_YES on success,
 *         SPDY_NO if error occurs
 */
typedef int
(*SPDYF_NewStreamCallback) (void *cls,
						struct SPDYF_Stream * stream);


/**
 * Callback to be called when the response queue object was handled and 
 * the data was already sent. 
 *
 * @param cls
 * @param response_queue the SPDYF_Response_Queue structure which will
 * 			be cleaned very soon
 * @param status shows if actually the response was sent or it was
 * 			discarded by the lib for any reason (e.g., closing session,
 * 			closing stream, stopping daemon, etc.). It is possible that
 * 			status indicates an error but part of the response (in one
 * 			or several frames) was sent to the client.
 */
typedef void
(*SPDYF_ResponseQueueResultCallback) (void * cls,
								struct SPDYF_Response_Queue *response_queue,
								enum SPDY_RESPONSE_RESULT status);


/**
 * Representation of the control frame's headers, which are common for
 * all types.
 */
struct __attribute__((__packed__)) SPDYF_Control_Frame
{
	uint16_t version : 15;
	uint16_t control_bit : 1; /* always 1 for control frames */
	uint16_t type;
	uint32_t flags : 8;
	uint32_t length : 24;
};


/**
 * Representation of the data frame's headers.
 */
struct __attribute__((__packed__)) SPDYF_Data_Frame
{
	uint32_t stream_id : 31;
	uint32_t control_bit : 1; /* always 0 for data frames */
	uint32_t flags : 8;
	uint32_t length : 24;
};


/**
 * Queue of the responses, to be handled (e.g. compressed) and sent later.
 */
struct SPDYF_Response_Queue
{
	/**
	 * This is a doubly-linked list.
	 */
	struct SPDYF_Response_Queue *next;

	/**
	 * This is a doubly-linked list.
	 */
	struct SPDYF_Response_Queue *prev;

	/**
	 * Stream (Request) for which is the response.
	 */
	struct SPDYF_Stream *stream;

	/**
	 * Response structure with all the data (uncompressed headers) to be sent.
	 */
	struct SPDY_Response *response;

	/**
	 * Control frame. The length field should be set after compressing
	 * the headers!
	 */
	struct SPDYF_Control_Frame *control_frame;

	/**
	 * Data frame. The length field should be set after compressing
	 * the body!
	 */
	struct SPDYF_Data_Frame *data_frame;

	/**
	 * Data to be sent: name/value pairs in control frames or body in data frames.
	 */
	void *data;

	/**
	 * Specific handler for different frame types.
	 */
	int (* process_response_handler)(struct SPDY_Session *session);

	/**
	 * Callback to be called when the last bytes from the response was sent
	 * to the client.
	 */
	SPDYF_ResponseQueueResultCallback frqcb;
	
	/**
	 * Closure for frqcb.
	 */
	void *frqcb_cls;

	/**
	 * Callback to be used by the application layer.
	 */
	SPDY_ResponseResultCallback rrcb;
	
	/**
	 * Closure for rcb.
	 */
	void *rrcb_cls;

	/**
	 * Data size.
	 */
	size_t data_size;

	/**
	 * True if data frame should be sent. False if control frame should
	 * be sent.
	 */
	bool is_data;
};



/**
 * Collection of HTTP headers used in requests and responses.
 */
struct SPDY_NameValue
{
	/**
	* This is a doubly-linked list.
	*/
	struct SPDY_NameValue *next;

	/**
	* This is a doubly-linked list.
	*/
	struct SPDY_NameValue *prev;

	/**
	* Null terminated string for name.
	*/
    char *name;

	/**
	* Array of Null terminated strings for value. num_values is the
	* length of the array.
	*/
	char **value;

	/**
	* Number of values, this is >= 0.
	*/
	unsigned int num_values;
};


/**
 * Represents a SPDY stream
 */ 
struct SPDYF_Stream
{
	/**
	 * This is a doubly-linked list.
	 */
	struct SPDYF_Stream *next;

	/**
	 * This is a doubly-linked list.
	 */
	struct SPDYF_Stream *prev;

	/**
	 * Reference to the SPDY_Session struct.
	 */
	struct SPDY_Session *session;
	
	/**
	 * Name value pairs, sent within the frame which created the stream.
	 */
	struct SPDY_NameValue *headers;
	
	/**
	 * Any object to be used by the application layer.
	 */
	void *cls;
  
	/**
	 * This stream's ID.
	 */
	uint32_t stream_id;
	
	/**
	 * Stream to which this one is associated.
	 */
	uint32_t assoc_stream_id;
	
	/**
	 * The window of the data within data frames.
	 */
	uint32_t window_size;
	
	/**
	 * Stream priority. 0 is the highest, 7 is the lowest.
	 */
	uint8_t priority;
	
	/**
	 * Integer specifying the index in the server's CREDENTIAL vector of
	 * the client certificate to be used for this request The value 0
	 * means no client certificate should be associated with this stream.
	 */
	uint8_t slot;
	
	/**
	 * If initially the stream was created as unidirectional.
	 */
	bool flag_unidirectional;
	
	/**
	 * If the stream won't be used for receiving frames anymore. The 
	 * client has sent FLAG_FIN or the stream was terminated with
	 * RST_STREAM.
	 */
	bool is_in_closed;
	
	/**
	 * If the stream won't be used for sending out frames anymore. The 
	 * server has sent FLAG_FIN or the stream was terminated with
	 * RST_STREAM.
	 */
	bool is_out_closed;
	
	/**
	 * Which entity (server/client) has created the stream.
	 */
	bool is_server_initiator;
};


/**
 * Represents a SPDY session which is just a TCP connection
 */ 
struct SPDY_Session
{
	/**
	 * zlib stream for decompressing all the name/pair values from the
	 * received frames. All the received compressed data must be
	 * decompressed within one context: this stream. Thus, it should be
	 * unique for the session and initialized at its creation.
	 */
	z_stream zlib_recv_stream;

	/**
	 * zlib stream for compressing all the name/pair values from the
	 * frames to be sent. All the sent compressed data must be
	 * compressed within one context: this stream. Thus, it should be
	 * unique for the session and initialized at its creation.
	 */
	z_stream zlib_send_stream;
	
	/**
	 * This is a doubly-linked list.
	 */
	struct SPDY_Session *next;

	/**
	 * This is a doubly-linked list.
	 */
	struct SPDY_Session *prev;

	/**
	 * Reference to the SPDY_Daemon struct.
	 */
	struct SPDY_Daemon *daemon;

	/**
	 * Foreign address (of length addr_len).
	 */
	struct sockaddr *addr;

	/**
	 * Head of doubly-linked list of the SPDY streams belonging to the
	 * session.
	 */
	struct SPDYF_Stream *streams_head;

	/**
	 * Tail of doubly-linked list of the streams.
	 */
	struct SPDYF_Stream *streams_tail;

	/**
	 * Unique IO context for the session. Initialized on each creation
	 * (actually when the TCP connection is established).
	 */
	void *io_context;
	
	/**
	 * Head of doubly-linked list of the responses.
	 */
	struct SPDYF_Response_Queue *response_queue_head;
	
	/**
	 * Tail of doubly-linked list of the responses.
	 */
	struct SPDYF_Response_Queue *response_queue_tail;

	/**
	 * Buffer for reading requests.
	 */
	void *read_buffer;

	/**
	 * Buffer for writing responses.
	 */
	void *write_buffer;

	/**
	 * Specific handler for the frame that is currently being received.
	 */
	void (*frame_handler) (struct SPDY_Session * session);

	/**
	 * Closure for frame_handler.
	 */
	void *frame_handler_cls;

	/**
	 * Extra field to be used by the user with set/get func for whatever
	 * purpose he wants.
	 */
	void *user_cls;

	/**
	 * Function to initialize the IO context for a new session.
	 */
	SPDYF_IONewSession fio_new_session;

	/**
	 * Function to deinitialize the IO context for a session.
	 */
	SPDYF_IOCloseSession fio_close_session;

	/**
	 * Function to read data from socket.
	 */
	SPDYF_IORecv fio_recv;

	/**
	 * Function to write data to socket.
	 */
	SPDYF_IOSend fio_send;

	/**
	 * Function to check for pending data in IO buffers.
	 */
	SPDYF_IOIsPending fio_is_pending;

	/**
	 * Function to call before writing set of frames.
	 */
	SPDYF_IOBeforeWrite fio_before_write;

	/**
	 * Function to call after writing set of frames.
	 */
	SPDYF_IOAfterWrite fio_after_write;

	/**
	 * Number of bytes that the lib must ignore immediately after they 
	 * are read from the TLS socket without adding them to the read buf.
	 * This is needed, for instance, when receiving frame bigger than
	 * the buffer to avoid deadlock situations.
	 */
	size_t read_ignore_bytes;

	/**
	 * Size of read_buffer (in bytes).  This value indicates
	 * how many bytes we're willing to read into the buffer;
	 * the real buffer is one byte longer to allow for
	 * adding zero-termination (when needed).
	 */
	size_t read_buffer_size;

	/**
	 * Position where we currently append data in
	 * read_buffer (last valid position).
	 */
	size_t read_buffer_offset;

	/**
	 * Position until where everything was already read
	 */
	size_t read_buffer_beginning;

	/**
	 * Size of write_buffer (in bytes).  This value indicates
	 * how many bytes we're willing to prepare for writing.
	 */
	size_t write_buffer_size;

	/**
	 * Position where we currently append data in
	 * write_buffer (last valid position).
	 */
	size_t write_buffer_offset;

	/**
	 * Position until where everything was already written to the socket
	 */
	size_t write_buffer_beginning;
	
	/**
	 * Last time this connection had any activity
	 * (reading or writing). In milliseconds.
	 */
	unsigned long long last_activity;

	/**
	 * Socket for this connection.  Set to -1 if
	 * this connection has died (daemon should clean
	 * up in that case).
	 */
	int socket_fd;

	/**
	 * Length of the foreign address.
	 */
	socklen_t addr_len;
	
	/**
	 * The biggest stream ID for this session for streams initiated
	 * by the client.
	 */
	uint32_t last_in_stream_id;
	
	/**
	 * The biggest stream ID for this session for streams initiated
	 * by the server.
	 */
	uint32_t last_out_stream_id;
	
	/**
	 * This value is updated whenever SYN_REPLY or RST_STREAM are sent
	 * and is used later in GOAWAY frame.
	 * TODO it is not clear in the draft what happens when streams are
	 * not answered in the order of their IDs. Moreover, why should we
	 * send GOAWAY with the ID of received bogus SYN_STREAM with huge ID?
	 */
	uint32_t last_replied_to_stream_id;
	
	/**
	 * Shows the stream id of the currently handled frame. This value is
	 * to be used when sending RST_STREAM in answer to a problematic
	 * frame, e.g. larger than supported.
	 */
	uint32_t current_stream_id;
	
	/**
	 * Maximum number of frames to be written to the socket at once. The
   * library tries to send max_num_frames in a single call to SPDY_run
   * for a single session. This means no requests can be received nor
   * other sessions can send data as long the current one has enough
   * frames to send and there is no error on writing.
	 */
	uint32_t max_num_frames;

	/**
	 * Shows the current receiving state the session, i.e. what is
	 * expected to come now, and how it shold be handled.
	 */
	enum SPDY_SESSION_STATUS status;

	/**
	 * Has this socket been closed for reading (i.e.
	 * other side closed the connection)?  If so,
	 * we must completely close the connection once
	 * we are done sending our response (and stop
	 * trying to read from this socket).
	 */
	bool read_closed;

	/**
	 * If the server sends GOAWAY, it must ignore all SYN_STREAMS for
	 * this session. Normally the server will soon close the TCP session.
	 */
	bool is_goaway_sent;

	/**
	 * If the server receives GOAWAY, it must not send new SYN_STREAMS 
	 * on this session. Normally the client will soon close the TCP
	 * session.
	 */
	bool is_goaway_received;
};


/**
 * State and settings kept for each SPDY daemon.
 */
struct SPDY_Daemon
{

	/**
	 * Tail of doubly-linked list of our current, active sessions.
	 */
	struct SPDY_Session *sessions_head;

	/**
	 * Tail of doubly-linked list of our current, active sessions.
	 */
	struct SPDY_Session *sessions_tail;
	
	/**
	 * Tail of doubly-linked list of connections to clean up.
	 */
	struct SPDY_Session *cleanup_head;

	/**
	 * Tail of doubly-linked list of connections to clean up.
	 */
	struct SPDY_Session *cleanup_tail;

	/**
	 * Unique IO context for the daemon. Initialized on daemon start.
	 */
	void *io_context;

	/**
	 * Certificate file of the server. File path is kept here.
	 */
	char *certfile;

	/**
	 * Key file for the certificate of the server. File path is
	 * kept here.
	 */
	char *keyfile;
	

	/**
	 * The address to which the listening socket is bound.
	 */
	struct sockaddr *address;
	
	/**
	 * Callback called when a new SPDY session is
	 * established by a client
	 */
	SPDY_NewSessionCallback new_session_cb;

	/**
	 * Callback called when a client closes the session
	 */
	SPDY_SessionClosedCallback session_closed_cb;

	/**
	 * Callback called when a client sends request
	 */
	SPDY_NewRequestCallback new_request_cb;

	/**
	* Callback called when HTTP POST params are received
	* after request. To be used by the application layer
	*/
	SPDY_NewDataCallback received_data_cb;

	/**
	* Callback called when DATA frame is received.
	*/
	SPDYF_NewDataCallback freceived_data_cb;

	/**
	 * Closure argument for all the callbacks that can be used by the client.
	 */
	void *cls;

	/**
	 * Callback called when new stream is created.
	 */
	SPDYF_NewStreamCallback fnew_stream_cb;

	/**
	 * Closure argument for all the callbacks defined in the framing layer.
	 */
	void *fcls;

	/**
	 * Function to initialize the IO context for the daemon.
	 */
	SPDYF_IOInit fio_init;

	/**
	 * Function to deinitialize the IO context for the daemon.
	 */
	SPDYF_IODeinit fio_deinit;

	/**
	 * After how many milliseconds of inactivity should
	 * connections time out? Zero for no timeout.
	 */
	unsigned long long session_timeout;

	/**
	 * Listen socket.
	 */
	int socket_fd;
	
	/**
   * This value is inherited by all sessions of the daemon.
	 * Maximum number of frames to be written to the socket at once. The
   * library tries to send max_num_frames in a single call to SPDY_run
   * for a single session. This means no requests can be received nor
   * other sessions can send data as long the current one has enough
   * frames to send and there is no error on writing.
	 */
	uint32_t max_num_frames;

	/**
	 * Daemon's options.
	 */
	enum SPDY_DAEMON_OPTION options;

	/**
	 * Daemon's flags.
	 */
	enum SPDY_DAEMON_FLAG flags;

	/**
	 * IO subsystem type used by daemon and all its sessions.
	 */
	enum SPDY_IO_SUBSYSTEM io_subsystem;

	/**
	 * Listen port.
	 */
	uint16_t port;
};


/**
 * Represents a SPDY response.
 */
struct SPDY_Response
{
	/**
	 * Raw uncompressed stream of the name/value pairs in SPDY frame
	 * used for the HTTP headers.
	 */
    void *headers;
	
	/**
	 * Raw stream of the data to be sent. Equivalent to the body in HTTP
	 * response.
	 */
	void *data;
	
	/**
	 * Callback function to be used when the response data is provided
	 * with callbacks. In this case data must be NULL and data_size must
	 * be 0.
	 */
	SPDY_ResponseCallback rcb;
	
	/**
	 * Extra argument to rcb.
	 */
	void *rcb_cls;
	
	/**
	 * Length of headers.
	 */
	size_t headers_size;
	
	/**
	 * Length of data.
	 */
	size_t data_size;
	
	/**
	 * The callback func will be called to get that amount of bytes to
	 * put them into a DATA frame. It is either user preffered or
	 * the maximum supported by the lib value.
	 */
	uint32_t rcb_block_size;
};


/* Macros for handling data and structures */


/**
 * Insert an element at the head of a DLL. Assumes that head, tail and
 * element are structs with prev and next fields.
 *
 * @param head pointer to the head of the DLL (struct ? *)
 * @param tail pointer to the tail of the DLL (struct ? *)
 * @param element element to insert (struct ? *)
 */
#define DLL_insert(head,tail,element) do { \
	(element)->next = (head); \
	(element)->prev = NULL; \
	if ((tail) == NULL) \
		(tail) = element; \
	else \
		(head)->prev = element; \
	(head) = (element); } while (0)


/**
 * Remove an element from a DLL. Assumes
 * that head, tail and element are structs
 * with prev and next fields.
 *
 * @param head pointer to the head of the DLL (struct ? *)
 * @param tail pointer to the tail of the DLL (struct ? *)
 * @param element element to remove (struct ? *)
 */
#define DLL_remove(head,tail,element) do { \
	if ((element)->prev == NULL) \
		(head) = (element)->next;  \
	else \
		(element)->prev->next = (element)->next; \
	if ((element)->next == NULL) \
		(tail) = (element)->prev;  \
	else \
		(element)->next->prev = (element)->prev; \
	(element)->next = NULL; \
	(element)->prev = NULL; } while (0)


/**
 * Convert all integers in a SPDY control frame headers structure from
 * host byte order to network byte order.
 *
 * @param frame input and output structure (struct SPDY_Control_Frame *)
 */
#if HAVE_BIG_ENDIAN
#define SPDYF_CONTROL_FRAME_HTON(frame)
#else
#define SPDYF_CONTROL_FRAME_HTON(frame) do { \
	(*((uint16_t *) frame  )) = (*((uint8_t *) (frame) +1 )) | ((*((uint8_t *) frame  ))<<8);\
	(frame)->type = htons((frame)->type); \
	(frame)->length = HTON24((frame)->length); \
	} while (0)
#endif


/**
 * Convert all integers in a SPDY control frame headers structure from
 * network byte order to host byte order.
 *
 * @param frame input and output structure (struct SPDY_Control_Frame *)
 */	
#if HAVE_BIG_ENDIAN
#define SPDYF_CONTROL_FRAME_NTOH(frame)
#else
#define SPDYF_CONTROL_FRAME_NTOH(frame) do { \
	(*((uint16_t *) frame  )) = (*((uint8_t *) (frame) +1 )) | ((*((uint8_t *) frame  ))<<8);\
	(frame)->type = ntohs((frame)->type); \
	(frame)->length = NTOH24((frame)->length); \
	} while (0)
#endif


/**
 * Convert all integers in a SPDY data frame headers structure from
 * host byte order to network byte order.
 *
 * @param frame input and output structure (struct SPDY_Data_Frame *)
 */
#if HAVE_BIG_ENDIAN
#define SPDYF_DATA_FRAME_HTON(frame)
#else
#define SPDYF_DATA_FRAME_HTON(frame) do { \
	*((uint32_t *) frame  ) = htonl(*((uint32_t *) frame  ));\
	(frame)->length = HTON24((frame)->length); \
	} while (0)
#endif


/**
 * Convert all integers in a SPDY data frame headers structure from
 * network byte order to host byte order.
 *
 * @param frame input and output structure (struct SPDY_Data_Frame *)
 */	
#if HAVE_BIG_ENDIAN
#define SPDYF_DATA_FRAME_NTOH(frame)
#else
#define SPDYF_DATA_FRAME_NTOH(frame) do { \
	*((uint32_t *) frame  ) = ntohl(*((uint32_t *) frame  ));\
	(frame)->length = NTOH24((frame)->length); \
	} while (0)
#endif


/**
 * Creates one or more new SPDYF_Response_Queue object to be put on the
 * response queue.
 *
 * @param is_data whether new data frame or new control frame will be
 *                crerated
 * @param data the row stream which will be used as the body of the frame
 * @param data_size length of data
 * @param response object, part of which is the frame
 * @param stream on which data is to be sent 
 * @param closestream TRUE if the frame must close the stream (with flag) 
 * @param frqcb callback to notify application layer when the frame
 *              has been sent or discarded 
 * @param frqcb_cls closure for frqcb
 * @param rrcb callback used by the application layer to notify the
 *             application when the frame has been sent or discarded.
 *             frqcb will call it 
 * @param rrcb_cls closure for rrcb
 * @return double linked list of SPDYF_Response_Queue structures: one or
 *         more frames are returned based on the size of the data
 */
struct SPDYF_Response_Queue *
SPDYF_response_queue_create(bool is_data,
						void *data,
						size_t data_size,
						struct SPDY_Response *response,
						struct SPDYF_Stream *stream,
						bool closestream,
						SPDYF_ResponseQueueResultCallback frqcb,
						void *frqcb_cls,
						SPDY_ResponseResultCallback rrcb,
						void *rrcb_cls);


/**
 * Destroys SPDYF_Response_Queue structure and whatever is in it.
 *
 * @param response_queue to destroy
 */
void
SPDYF_response_queue_destroy(struct SPDYF_Response_Queue *response_queue);


/**
 * Checks if the container is empty, i.e. created but no values were
 * added to it.
 *
 * @param container
 * @return SPDY_YES if empty
 *         SPDY_NO if not
 */
int
SPDYF_name_value_is_empty(struct SPDY_NameValue *container);


/**
 * Transforms raw binary decomressed stream of headers
 * into SPDY_NameValue, containing all of the headers and values.
 *
 * @param stream that is to be transformed
 * @param size length of the stream
 * @param container will contain the newly created SPDY_NameValue
 *        container. Should point to NULL.
 * @return SPDY_YES on success
 *         SPDY_NO on memory error
 *         SPDY_INPUT_ERROR if the provided stream is not valid
 */
int
SPDYF_name_value_from_stream(void *stream,
							size_t size,
							struct SPDY_NameValue ** container);


/**
 * Transforms array of objects of name/values tuples, containing HTTP
 * headers, into raw binary stream. The resulting stream is ready to
 * be compressed and sent.
 *
 * @param container one or more SPDY_NameValue objects. Each object
 *        contains multiple number of name/value tuples.
 * @param num_containers length of the array
 * @param stream will contain the resulting stream. Should point to NULL.
 * @return length of stream or value less than 0 indicating error
 */
ssize_t
SPDYF_name_value_to_stream(struct SPDY_NameValue * container[],
							int num_containers,
							void **stream);

#endif
