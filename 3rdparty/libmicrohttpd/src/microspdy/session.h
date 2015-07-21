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
 * @file session.h
 * @brief  TCP connection/SPDY session handling
 * @author Andrey Uzunov
 */

#ifndef SESSION_H
#define SESSION_H

#include "platform.h"
#include "structures.h"

/**
 * Called by the daemon when the socket for the session has available
 * data to be read. Reads data from the TLS socket and puts it to the
 * session's read buffer. The latte
 *
 * @param session SPDY_Session for which data will be read.
 * @return SPDY_YES if something was read or session's status was
 *         changed. It is possible that error occurred but was handled
 *         and the status was therefore changed.
 *         SPDY_NO if nothing happened, e.g. the subsystem wants read/
 *         write to be called again.
 */
int
SPDYF_session_read (struct SPDY_Session *session);


/**
 * Called by the daemon when the socket for the session is ready for some
 * data to be written to it. For one or more objects on the response
 * queue tries to fill in the write buffer, based on the frame on the
 * queue, and to write data to the TLS socket. 
 *
 * @param session SPDY_Session for which data will be written.
 * @param only_one_frame when true, the function will write at most one
 *        SPDY frame to the underlying IO subsystem;
 *        when false, the function will write up to
 *        session->max_num_frames SPDY frames
 * @return SPDY_YES if the session's internal writing state has changed:
 *         something was written and/or session's status was
 *         changed and/or response callback was called but did not provide
 *         data. It is possible that error occurred but was handled
 *         and the status was therefore changed.
 *         SPDY_NO if nothing happened. However, it is possible that some
 *         frames were discarded within the call, e.g. frames belonging
 *         to a closed stream.
 */
int
SPDYF_session_write (struct SPDY_Session *session,
                     bool only_one_frame);


/**
 * Called by the daemon on SPDY_run to handle the data in the read and write
 * buffer of a session. Based on the state and the content of the read
 * buffer new frames are received and interpreted, appropriate user
 * callbacks are called and maybe something is put on the response queue
 * ready to be handled by session_write.
 * 
 * @param session SPDY_Session which will be handled.
 * @return SPDY_YES if something from the read buffers was processed,
 *         session's status was changed and/or the session was closed.
 *         SPDY_NO if nothing happened, e.g. the session is in a state,
 *         not allowing processing read buffers.
 */
int
SPDYF_session_idle (struct SPDY_Session *session);


/**
 * This function shutdowns the socket, moves the session structure to
 * daemon's queue for sessions to be cleaned up.
 * 
 * @param session SPDY_Session which will be handled.
 */
void
SPDYF_session_close (struct SPDY_Session *session);


/**
 * Called to accept new TCP connection and create SPDY session.
 * 
 * @param daemon SPDY_Daemon whose listening socket is used.
 * @return SPDY_NO on any kind of error while accepting new TCP connection
 * 			and initializing new SPDY_Session.
 *         SPDY_YES otherwise.
 */
int
SPDYF_session_accept(struct SPDY_Daemon *daemon);


/**
 * Puts SPDYF_Response_Queue object on the queue to be sent to the
 * client later.
 *
 * @param response_to_queue linked list of objects containing SPDY
 * 			frame and data to be added to the queue
 * @param session SPDY session for which the response is sent
 * @param consider_priority if SPDY_NO, the list will be added to the
 * 			end of the queue.
 * 			If SPDY_YES, the response will be added after
 * 			the last previously added response with priority of the
 * 			request grater or equal to that of the current one.
 * 			If -1, the object will be put at the head of the queue.
 */
void
SPDYF_queue_response (struct SPDYF_Response_Queue *response_to_queue,
						struct SPDY_Session *session,
						int consider_priority);


/**
 * Cleans up the TSL context for the session, closes the TCP connection,
 * cleans up any data pointed by members of the session structure
 * (buffers, queue of responses, etc.) and frees the memory allocated by
 * the session itself.
 */						
void
SPDYF_session_destroy(struct SPDY_Session *session);


/**
 * Prepares GOAWAY frame to tell the client to stop creating new streams.
 * The session should be closed soon after this call.
 * 
 * @param session SPDY session
 * @param status code for the GOAWAY frame
 * @param in_front whether or not to put the frame in front of everything
 * 			on the response queue
 * @return SPDY_NO on error (not enough memory) or
 * 			SPDY_YES on success
 */
int
SPDYF_prepare_goaway (struct SPDY_Session *session,
					enum SPDY_GOAWAY_STATUS status,
					bool in_front);


/**
 * Prepares RST_STREAM frame to terminate a stream. This frame may or
 * not indicate an error. The frame will be put at the head of the queue.
 * This means that frames for this stream which are still in the queue
 * will be discarded soon.
 * 
 * @param session SPDY session
 * @param stream stream to terminate
 * @param status code for the RST_STREAM frame
 * @return SPDY_NO on memory error or
 * 			SPDY_YES on success
 */
int
SPDYF_prepare_rst_stream (struct SPDY_Session *session,
					struct SPDYF_Stream * stream,
					enum SPDY_RST_STREAM_STATUS status);


/**
 * Prepares WINDOW_UPDATE frame to tell the other party that more
 * data can be sent on the stream. The frame will be put at the head of
 * the queue.
 * 
 * @param session SPDY session
 * @param stream stream to which the changed window will apply
 * @param delta_window_size how much the window grows
 * @return SPDY_NO on memory error or
 * 			SPDY_YES on success
 */
int
SPDYF_prepare_window_update (struct SPDY_Session *session,
					struct SPDYF_Stream * stream,
					int32_t delta_window_size);
          

/**
 * Handler called by session_write to fill the write buffer according to
 * the data frame waiting in the response queue.
 * When response data is given by user callback, the lib does not know
 * how many frames are needed. In such case this call produces
 * another ResponseQueue object and puts it on the queue while the the
 * user callback says that there will be more data.
 * 
 * @return SPDY_NO on error (not enough memory or the user calback for
 *         providing response data did something wrong). If
 *         the error is unrecoverable the handler changes session's
 *         status.
 *         SPDY_YES on success
 */	
int
SPDYF_handler_write_data (struct SPDY_Session *session);


/**
 * Handler called by session_write to fill the write buffer based on the
 * control frame (SYN_REPLY) waiting in the response queue.
 * 
 * @param session SPDY session
 * @return SPDY_NO on error (zlib state is broken; the session MUST be
 *         closed). If
 *         the error is unrecoverable the handler changes session's
 *         status.
 * 			SPDY_YES on success
 */ 			
int
SPDYF_handler_write_syn_reply (struct SPDY_Session *session);


/**
 * Handler called by session_write to fill the write buffer based on the
 * control frame (GOAWAY) waiting in the response queue.
 * 
 * @param session SPDY session
 * @return SPDY_NO on error (not enough memory; by specification the
 *         session must be closed
 *         soon, thus there is no need to handle the error) or
 * 			SPDY_YES on success
 */					
int
SPDYF_handler_write_goaway (struct SPDY_Session *session);


/**
 * Handler called by session_write to fill the write buffer based on the
 * control frame (RST_STREAM) waiting in the response queue.
 * 
 * @param session SPDY session
 * @return SPDY_NO on error (not enough memory). If
 *         the error is unrecoverable the handler changes session's
 *         status.
 * 			SPDY_YES on success
 */				
int
SPDYF_handler_write_rst_stream (struct SPDY_Session *session);


/**
 * Handler called by session_write to fill the write buffer based on the
 * control frame (WINDOW_UPDATE) waiting in the response queue.
 * 
 * @param session SPDY session
 * @return SPDY_NO on error (not enough memory). If
 *         the error is unrecoverable the handler changes session's
 *         status.
 * 			SPDY_YES on success
 */			
int
SPDYF_handler_write_window_update (struct SPDY_Session *session);


/**
 * Carefully ignore the full size of frames which are not yet supported
 * by the lib.
 * TODO Ignoring frames containing compressed bodies means that the
 * compress state will be corrupted on next received frame. According to
 * the draft the lib SHOULD try to decompress data also in corrupted
 * frames just to keep right compression state.
 * 
 * @param session SPDY_Session whose read buffer is used.
 */
void
SPDYF_handler_ignore_frame (struct SPDY_Session *session);

#endif
