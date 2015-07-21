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
 * @file stream.h
 * @brief  SPDY streams handling
 * @author Andrey Uzunov
 */

#ifndef STREAM_H
#define STREAM_H

#include "platform.h"


/**
 * Reads data from session's read buffer and tries to create a new SPDY
 * stream. This function is called after control frame's header has been
 * read from the buffer (after the length field). If bogus frame is
 * received the function changes the read handler of the session and
 * fails, i.e. there is no need of further error handling by the caller.
 *
 * @param session SPDY_Session whose read buffer is being read
 * @return SPDY_YES if a new SPDY stream request was correctly received
 * 			and handled. SPDY_NO if the whole SPDY frame was not yet
 * 			received or memory error occurred.
 */
int
SPDYF_stream_new (struct SPDY_Session *session);


/**
 * Destroys stream structure and whatever is in it.
 *
 * @param stream SPDY_Stream to destroy
 */
void
SPDYF_stream_destroy(struct SPDYF_Stream *stream);


/**
 * Set stream flags if needed based on the type of the frame that was
 * just sent (e.g., close stream if it was RST_STREAM).
 *
 * @param response_queue sent for this stream
 */
void
SPDYF_stream_set_flags_on_write(struct SPDYF_Response_Queue *response_queue);


/**
 * Find and return a session's stream, based on stream's ID.
 *
 * @param stream_id to search for
 * @param session whose streams are considered
 * @return SPDY_Stream with the desired ID. Can be NULL.
 */
struct SPDYF_Stream * 
SPDYF_stream_find(uint32_t stream_id, struct SPDY_Session * session);

#endif
