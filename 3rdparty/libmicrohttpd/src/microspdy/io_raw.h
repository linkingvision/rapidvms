/*
    This file is part of libmicrospdy
    Copyright Copyright (C) 2013 Andrey Uzunov

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
 * @file io_raw.h
 * @brief  IO for SPDY without TLS.
 * @author Andrey Uzunov
 */

#ifndef IO_RAW_H
#define IO_RAW_H

#include "platform.h"


/**
 * Must be called only once in the program.
 *
 */
void
SPDYF_raw_global_init();


/**
 * Should be called
 * at the end of the program.
 *
 */
void
SPDYF_raw_global_deinit();


/**
 * Must be called when the daemon starts.
 *
 * @param daemon SPDY_Daemon 
 * @return SPDY_YES on success or SPDY_NO on error
 */
int
SPDYF_raw_init(struct SPDY_Daemon *daemon);


/**
 * Should be called
 * when the deamon is stopped.
 *
 * @param daemon SPDY_Daemon which is being stopped
 */
void
SPDYF_raw_deinit(struct SPDY_Daemon *daemon);


/**
 * Must be called
 * after the connection has been accepted.
 *
 * @param session SPDY_Session whose socket will be used
 * @return SPDY_NO if some funcs fail. SPDY_YES otherwise
 */
int
SPDYF_raw_new_session(struct SPDY_Session *session);


/**
 * Should be called
 * closing session's socket.
 *
 * @param session SPDY_Session whose socket is used
 */
void
SPDYF_raw_close_session(struct SPDY_Session *session);


/**
 * Reading from socket. Reads available data and put it to the
 * buffer.
 *
 * @param session for which data is received
 * @param buffer where data from the socket will be written to
 * @param size of the buffer
 * @return number of bytes (at most size) read from the connection
 *         0 if the other party has closed the connection
 *         SPDY_IO_ERROR code on error
 */
int
SPDYF_raw_recv(struct SPDY_Session *session,
				void * buffer,
				size_t size);


/**
 * Writing to socket. Writes the data given into the buffer to the
 * socket.
 *
 * @param session whose context is used
 * @param buffer from where data will be written to the socket
 * @param size number of bytes to be taken from the buffer
 * @return number of bytes (at most size) from the buffer that has been
 * 			written to the connection
 *         0 if the other party has closed the connection
 *         SPDY_IO_ERROR code on error
 */
int
SPDYF_raw_send(struct SPDY_Session *session,
				const void * buffer,
				size_t size);


/**
 * Checks if there is data staying in the buffers of the underlying
 * system that waits to be read. Always returns SPDY_NO, as we do not
 * use a subsystem here.
 *
 * @param session which is checked
 * @return SPDY_YES if data is pending or SPDY_NO otherwise
 */
int
SPDYF_raw_is_pending(struct SPDY_Session *session);


/**
 * Sets TCP_CORK.
 *
 * @param session
 * @return SPDY_NO if writing must not happen in the call;
 *         SPDY_YES otherwise
 */
int
SPDYF_raw_before_write(struct SPDY_Session *session);


/**
 * Unsets TCP_CORK.
 *
 * @param session
 * @param was_written has the same value as the write function for the
 *        session will return 
 * @return returned value will be used by the write function to return
 */
int
SPDYF_raw_after_write(struct SPDY_Session *session, int was_written);

#endif
