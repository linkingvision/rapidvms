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
 * @file io.h
 * @brief  Signatures for IO functions.
 * @author Andrey Uzunov
 */

#ifndef IO_H
#define IO_H

#include "platform.h"
#include "io_openssl.h"
#include "io_raw.h"


/**
 * Used for return code when reading and writing to the TLS socket.
 */
enum SPDY_IO_ERROR
{
	/**
	 * The connection was closed by the other party.
	 */
	SPDY_IO_ERROR_CLOSED = 0,
	
	/**
	 * Any kind of error ocurred. The session has to be closed.
	 */
	SPDY_IO_ERROR_ERROR = -2,
	
	/**
	 * The function had to return without processing any data. The whole
	 * cycle of events has to be called again (SPDY_run) as something
	 * either has to be written or read or the the syscall was
	 * interrupted by a signal.
	 */
	SPDY_IO_ERROR_AGAIN = -3,
};


/**
 * Global initializing. Must be called only once in the program.
 *
 */
typedef void
(*SPDYF_IOGlobalInit) ();


/**
 * Global deinitializing for the whole program. Should be called
 * at the end of the program.
 *
 */
typedef void
(*SPDYF_IOGlobalDeinit) ();


/**
 * Initializing of io context for a specific daemon.
 * Must be called when the daemon starts.
 *
 * @param daemon SPDY_Daemon for which io will be used. Daemon's
 * 				certificate and key file are used for tls.
 * @return SPDY_YES on success or SPDY_NO on error
 */
typedef int
(*SPDYF_IOInit) (struct SPDY_Daemon *daemon);


/**
 * Deinitializing io context for a daemon. Should be called
 * when the deamon is stopped.
 *
 * @param daemon SPDY_Daemon which is being stopped
 */
typedef void
(*SPDYF_IODeinit) (struct SPDY_Daemon *daemon);


/**
 * Initializing io for a specific connection. Must be called
 * after the connection has been accepted.
 *
 * @param session SPDY_Session whose socket will be used
 * @return SPDY_NO if some funcs inside fail. SPDY_YES otherwise
 */
typedef int
(*SPDYF_IONewSession) (struct SPDY_Session *session);


/**
 * Deinitializing io for a specific connection. Should be called
 * closing session's socket.
 *
 * @param session SPDY_Session whose socket is used
 */
typedef void
(*SPDYF_IOCloseSession) (struct SPDY_Session *session);


/**
 * Reading from session's socket. Reads available data and put it to the
 * buffer.
 *
 * @param session for which data is received
 * @param buffer where data from the socket will be written to
 * @param size of the buffer
 * @return number of bytes (at most size) read from the connection
 *         0 if the other party has closed the connection
 *         SPDY_IO_ERROR code on error
 */
typedef int
(*SPDYF_IORecv) (struct SPDY_Session *session,
				void * buffer,
				size_t size);


/**
 * Writing to session's socket. Writes the data given into the buffer to the
 *  socket.
 *
 * @param session whose context is used
 * @param buffer from where data will be written to the socket
 * @param size number of bytes to be taken from the buffer
 * @return number of bytes (at most size) from the buffer that has been
 * 			written to the connection
 *         0 if the other party has closed the connection
 *         SPDY_IO_ERROR code on error
 */
typedef int
(*SPDYF_IOSend) (struct SPDY_Session *session,
				const void * buffer,
				size_t size);


/**
 * Checks if there is data staying in the buffers of the underlying
 * system that waits to be read. In case of TLS, this will call
 * something like SSL_pending().
 *
 * @param session which is checked
 * @return SPDY_YES if data is pending or SPDY_NO otherwise
 */
typedef int
(*SPDYF_IOIsPending) (struct SPDY_Session *session);


/**
 * Called just before frames are about to be processed and written
 * to the socket.
 *
 * @param session
 * @return SPDY_NO if writing must not happen in the call;
 *         SPDY_YES otherwise
 */
typedef int
(*SPDYF_IOBeforeWrite) (struct SPDY_Session *session);


/**
 * Called just after frames have been processed and written
 * to the socket.
 *
 * @param session
 * @param was_written has the same value as the write function for the
 *        session will return 
 * @return returned value will be used by the write function to return
 */
typedef int
(*SPDYF_IOAfterWrite) (struct SPDY_Session *session,
                       int was_written);


/**
 * Sets callbacks for the daemon with regard to the IO subsystem.
 *
 * @param daemon
 * @param io_subsystem the IO subsystem that will
 *        be initialized and used by daemon.
 * @return SPDY_YES on success or SPDY_NO otherwise
 */
int
SPDYF_io_set_daemon(struct SPDY_Daemon *daemon,
                    enum SPDY_IO_SUBSYSTEM io_subsystem);


/**
 * Sets callbacks for the session with regard to the IO subsystem.
 *
 * @param session
 * @param io_subsystem the IO subsystem that will
 *        be initialized and used by session.
 * @return SPDY_YES on success or SPDY_NO otherwise
 */
int
SPDYF_io_set_session(struct SPDY_Session *session,
                     enum SPDY_IO_SUBSYSTEM io_subsystem);

#endif
