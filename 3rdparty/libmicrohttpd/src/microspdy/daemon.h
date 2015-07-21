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
 * @file daemon.h
 * @brief  daemon functionality
 * @author Andrey Uzunov
 */

#ifndef DAEMON_H
#define DAEMON_H

#include "platform.h"


/**
 * Global flags containing the initialized IO subsystems.
 */
enum SPDY_IO_SUBSYSTEM spdyf_io_initialized;


/**
 * Start a SPDDY webserver on the given port.
 *
 * @param port port to bind to
 * @param certfile path to the certificate that will be used by server
 * @param keyfile path to the keyfile for the certificate
 * @param nscb callback called when a new SPDY session is
 * 			established	by a client
 * @param sccb callback called when a client closes the session
 * @param nrcb callback called when a client sends request
 * @param npdcb callback called when HTTP POST params are received
 * 			after request
 * @param fnscb callback called when new stream is opened by a client
 * @param fndcb callback called when new data -- within a data frame --
 *        is received by the server
 * @param cls extra argument to all of the callbacks without those
 * 				 specific only for the framing layer
 * @param fcls extra argument to all of the callbacks, specific only for
 * 				the framing layer (those vars starting with 'f').
 * @param valist va_list of options (type-value pairs,
 *        terminated with SPDY_DAEMON_OPTION_END).
 * @return NULL on error, handle to daemon on success
 */
struct SPDY_Daemon *
SPDYF_start_daemon_va (uint16_t port,
					const char *certfile,
					const char *keyfile,
					SPDY_NewSessionCallback nscb,
					SPDY_SessionClosedCallback sccb,
					SPDY_NewRequestCallback nrcb,
					SPDY_NewDataCallback npdcb,
					SPDYF_NewStreamCallback fnscb,
					SPDYF_NewDataCallback fndcb,
					void * cls,
					void * fcls,
					va_list valist);


/**
 * Run webserver operations (without blocking unless
 * in client callbacks). This method must be called in the client event
 * loop.
 *
 * @param daemon daemon to run
 */
void 
SPDYF_run (struct SPDY_Daemon *daemon);


/**
 * Obtain timeout value for select for this daemon. The returned value
 * is how long select
 * should at most block, not the timeout value set for connections.
 *
 * @param daemon daemon to query for timeout
 * @param timeout set to the timeout (in milliseconds)
 * @return SPDY_YES on success, SPDY_NO if no connections exist that
 * 			would necessiate the use of a timeout right now
 */
int
SPDYF_get_timeout (struct SPDY_Daemon *daemon, 
		     unsigned long long *timeout);


/**
 * Obtain the select sets for this daemon. The idea of SPDYF_get_fdset
 * is to return such descriptors that the select in the application can
 * return and SPDY_run can be called only when this is really needed.
 * That means not all sockets will be added to write_fd_set.
 *
 * @param daemon daemon to get sets from
 * @param read_fd_set read set
 * @param write_fd_set write set
 * @param except_fd_set except set
 * @param all add all session's descriptors to write_fd_set or not
 * @return largest FD added
 */
int
SPDYF_get_fdset (struct SPDY_Daemon *daemon,
				fd_set *read_fd_set,
				fd_set *write_fd_set, 
				fd_set *except_fd_set,
				bool all);


/**
 * Shutdown the daemon.
 *
 * @param daemon daemon to stop
 */				
void 
SPDYF_stop_daemon (struct SPDY_Daemon *daemon);

#endif
