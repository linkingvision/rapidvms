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
 * @file microspdy/daemon.c
 * @brief  daemon functionality
 * @author Andrey Uzunov
 */
 
#include "platform.h"
#include "structures.h"
#include "internal.h"
#include "session.h"
#include "io.h"


/**
 * Default implementation of the panic function,
 * prints an error message and aborts.
 *
 * @param cls unused
 * @param file name of the file with the problem
 * @param line line number with the problem
 * @param reason error message with details
 */
static void 
spdyf_panic_std (void *cls,
	       const char *file,
	       unsigned int line,
	       const char *reason)
{
	(void)cls;
	fprintf (stdout, "Fatal error in libmicrospdy %s:%u: %s\n",
		file, line, reason);
	//raise(SIGINT); //used for gdb
	abort ();
}


/**
 * Global handler for fatal errors.
 */
SPDY_PanicCallback spdyf_panic = &spdyf_panic_std;


/**
 * Global closure argument for "spdyf_panic".
 */
void *spdyf_panic_cls;


/**
 * Free resources associated with all closed connections.
 * (destroy responses, free buffers, etc.).
 *
 * @param daemon daemon to clean up
 */
static void
spdyf_cleanup_sessions (struct SPDY_Daemon *daemon)
{
	struct SPDY_Session *session;
	
	while (NULL != (session = daemon->cleanup_head))
	{
		DLL_remove (daemon->cleanup_head,
			daemon->cleanup_tail,
			session);
			
		SPDYF_session_destroy(session);
	}
}


/**
 * Closing of all connections handled by the daemon.
 *
 * @param daemon SPDY daemon
 */
static void
spdyf_close_all_sessions (struct SPDY_Daemon *daemon)
{
	struct SPDY_Session *session;
	
	while (NULL != (session = daemon->sessions_head))
	{	
		//prepare GOAWAY frame
		SPDYF_prepare_goaway(session, SPDY_GOAWAY_STATUS_OK, true);
		//try to send the frame (it is best effort, so it will maybe sent)
		SPDYF_session_write(session,true);
		SPDYF_session_close(session);
	}
	
	spdyf_cleanup_sessions(daemon);
}


/**
 * Parse a list of options given as varargs.
 * 
 * @param daemon the daemon to initialize
 * @param valist the options
 * @return SPDY_YES on success, SPDY_NO on error
 */
static int
spdyf_parse_options_va (struct SPDY_Daemon *daemon,
		  va_list valist)
{
	enum SPDY_DAEMON_OPTION opt;

	while (SPDY_DAEMON_OPTION_END != (opt = (enum SPDY_DAEMON_OPTION) va_arg (valist, int)))
	{
		if(opt & daemon->options)
		{
			SPDYF_DEBUG("Daemon option %i used twice",opt);
			return SPDY_NO;
		}
		daemon->options |= opt;
		
		switch (opt)
		{
			case SPDY_DAEMON_OPTION_SESSION_TIMEOUT:
				daemon->session_timeout = va_arg (valist, unsigned int) * 1000;
				break;
			case SPDY_DAEMON_OPTION_SOCK_ADDR:
				daemon->address = va_arg (valist, struct sockaddr *);
				break;
			case SPDY_DAEMON_OPTION_FLAGS:
				daemon->flags = va_arg (valist, enum SPDY_DAEMON_FLAG);
				break;
			case SPDY_DAEMON_OPTION_IO_SUBSYSTEM:
				daemon->io_subsystem = va_arg (valist, enum SPDY_IO_SUBSYSTEM);
				break;
			case SPDY_DAEMON_OPTION_MAX_NUM_FRAMES:
				daemon->max_num_frames = va_arg (valist, uint32_t);
				break;
			default:
				SPDYF_DEBUG("Wrong option for the daemon %i",opt);
				return SPDY_NO;
		}
	}
	return SPDY_YES;
}


void 
SPDY_set_panic_func (SPDY_PanicCallback cb,
					void *cls)
{
	spdyf_panic = cb;
	spdyf_panic_cls = cls;
}


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
					va_list valist)
{
	struct SPDY_Daemon *daemon = NULL;
	int afamily;
	int option_on = 1;
	int ret;
	struct sockaddr_in* servaddr4 = NULL;
#if HAVE_INET6
	struct sockaddr_in6* servaddr6 = NULL;
#endif
	socklen_t addrlen;

	if (NULL == (daemon = malloc (sizeof (struct SPDY_Daemon))))
	{
		SPDYF_DEBUG("malloc");
		return NULL;
	}
	memset (daemon, 0, sizeof (struct SPDY_Daemon));
	daemon->socket_fd = -1;
	daemon->port = port;

	if(SPDY_YES != spdyf_parse_options_va (daemon, valist))
	{
		SPDYF_DEBUG("parse");
		goto free_and_fail;
	}
  
  if(0 == daemon->max_num_frames)
    daemon->max_num_frames = SPDYF_NUM_SENT_FRAMES_AT_ONCE;
	
	if(!port && NULL == daemon->address)
	{
		SPDYF_DEBUG("Port is 0");
		goto free_and_fail;
	}
  if(0 == daemon->io_subsystem)
    daemon->io_subsystem = SPDY_IO_SUBSYSTEM_OPENSSL;
  
  if(SPDY_YES != SPDYF_io_set_daemon(daemon, daemon->io_subsystem))
		goto free_and_fail;
  
  if(SPDY_IO_SUBSYSTEM_RAW != daemon->io_subsystem)
  {
    if (NULL == certfile
      || NULL == (daemon->certfile = strdup (certfile)))
    {
      SPDYF_DEBUG("strdup (certfile)");
      goto free_and_fail;
    }
    if (NULL == keyfile
      || NULL == (daemon->keyfile = strdup (keyfile)))
    {
      SPDYF_DEBUG("strdup (keyfile)");
      goto free_and_fail;
    }
  }
  
	daemon->new_session_cb = nscb;
	daemon->session_closed_cb = sccb;
	daemon->new_request_cb = nrcb;
	daemon->received_data_cb = npdcb;
	daemon->cls = cls;
	daemon->fcls = fcls;
	daemon->fnew_stream_cb = fnscb;
	daemon->freceived_data_cb = fndcb;

#if HAVE_INET6
	//handling IPv6
	if((daemon->flags & SPDY_DAEMON_FLAG_ONLY_IPV6)
		&& NULL != daemon->address && AF_INET6 != daemon->address->sa_family)
	{
		SPDYF_DEBUG("SPDY_DAEMON_FLAG_ONLY_IPV6 set but IPv4 address provided");
		goto free_and_fail;
	}
  
  addrlen = sizeof (struct sockaddr_in6);
    
	if(NULL == daemon->address)
	{		
		if (NULL == (servaddr6 = malloc (addrlen)))
		{
			SPDYF_DEBUG("malloc");
			goto free_and_fail;
		}
		memset (servaddr6, 0, addrlen);
		servaddr6->sin6_family = AF_INET6;
		servaddr6->sin6_addr = in6addr_any;
		servaddr6->sin6_port = htons (port);
		daemon->address = (struct sockaddr *) servaddr6;
	}
	
  if(AF_INET6 == daemon->address->sa_family)
  {
    afamily = PF_INET6;
  }
  else
  {
    afamily = PF_INET;
  }
#else
	//handling IPv4
	if(daemon->flags & SPDY_DAEMON_FLAG_ONLY_IPV6)
	{
		SPDYF_DEBUG("SPDY_DAEMON_FLAG_ONLY_IPV6 set but no support");
		goto free_and_fail;
	}
	
  addrlen = sizeof (struct sockaddr_in);
    
	if(NULL == daemon->address)
	{		
		if (NULL == (servaddr4 = malloc (addrlen)))
		{
			SPDYF_DEBUG("malloc");
			goto free_and_fail;
		}
		memset (servaddr4, 0, addrlen);
		servaddr4->sin_family = AF_INET;
		servaddr4->sin_addr = INADDR_ANY;
		servaddr4->sin_port = htons (port);
		daemon->address = (struct sockaddr *) servaddr4;
	}
	
	afamily = PF_INET;
#endif	

	daemon->socket_fd = socket (afamily, SOCK_STREAM, 0);
	if (-1 == daemon->socket_fd)
	{
		SPDYF_DEBUG("sock");
		goto free_and_fail;
	}

	//setting option for the socket to reuse address
	ret = setsockopt(daemon->socket_fd, SOL_SOCKET, SO_REUSEADDR, &option_on, sizeof(option_on));
	if(ret)
	{
		SPDYF_DEBUG("WARNING: SO_REUSEADDR was not set for the server");
	}
	
#if HAVE_INET6
	if(daemon->flags & SPDY_DAEMON_FLAG_ONLY_IPV6)
	{
		ret = setsockopt(daemon->socket_fd, IPPROTO_IPV6, IPV6_V6ONLY, &option_on, sizeof(option_on));
		if(ret)
		{
			SPDYF_DEBUG("setsockopt with IPPROTO_IPV6 failed");
			goto free_and_fail;
		}
	}
#endif
	
	if (-1 == bind (daemon->socket_fd, daemon->address, addrlen))
	{
		SPDYF_DEBUG("bind %i",errno);
		goto free_and_fail;
	}

	if (listen (daemon->socket_fd, 20) < 0)
	{
		SPDYF_DEBUG("listen %i",errno);
		goto free_and_fail;
	}

	if(SPDY_YES != daemon->fio_init(daemon))
	{
		SPDYF_DEBUG("tls");
		goto free_and_fail;
	}

	return daemon;

	//for GOTO
	free_and_fail:
	if(daemon->socket_fd > 0)
		(void)close (daemon->socket_fd);
		
	free(servaddr4);
#if HAVE_INET6
	free(servaddr6);
#endif
	if(NULL != daemon->certfile)
		free(daemon->certfile);
	if(NULL != daemon->keyfile)
		free(daemon->keyfile);
	free (daemon);
	
	return NULL;
}


void 
SPDYF_stop_daemon (struct SPDY_Daemon *daemon)
{
	daemon->fio_deinit(daemon);
	
	shutdown (daemon->socket_fd, SHUT_RDWR);
	spdyf_close_all_sessions (daemon);
	(void)close (daemon->socket_fd);
	
	if(!(SPDY_DAEMON_OPTION_SOCK_ADDR & daemon->options))
		free(daemon->address);
	
	free(daemon->certfile);
	free(daemon->keyfile);
	
	free(daemon);
}


int
SPDYF_get_timeout (struct SPDY_Daemon *daemon, 
		     unsigned long long *timeout)
{
	unsigned long long earliest_deadline = 0;
	unsigned long long now;
	struct SPDY_Session *pos;
	bool have_timeout;
	
	if(0 == daemon->session_timeout)
		return SPDY_NO;

	now = SPDYF_monotonic_time();
	have_timeout = false;
	for (pos = daemon->sessions_head; NULL != pos; pos = pos->next)
	{
		if ( (! have_timeout) ||
			(earliest_deadline > pos->last_activity + daemon->session_timeout) )
			earliest_deadline = pos->last_activity + daemon->session_timeout;

		have_timeout = true;
		
		if (SPDY_YES == pos->fio_is_pending(pos))
		{
			earliest_deadline = 0;
			break;
		}
	}
	
	if (!have_timeout)
		return SPDY_NO;
	if (earliest_deadline <= now)
		*timeout = 0;
	else
		*timeout = earliest_deadline - now;
		
	return SPDY_YES;
}


int
SPDYF_get_fdset (struct SPDY_Daemon *daemon,
				fd_set *read_fd_set,
				fd_set *write_fd_set, 
				fd_set *except_fd_set,
				bool all)
{
	(void)except_fd_set;
	struct SPDY_Session *pos;
	int fd;
	int max_fd = -1;

	fd = daemon->socket_fd;
	if (-1 != fd)
	{
		FD_SET (fd, read_fd_set);
		/* update max file descriptor */
		max_fd = fd;
	}

	for (pos = daemon->sessions_head; NULL != pos; pos = pos->next)
	{
		fd = pos->socket_fd;
		FD_SET(fd, read_fd_set);
		if (all
		    || (NULL != pos->response_queue_head) //frames pending
		    || (NULL != pos->write_buffer) //part of last frame pending
		    || (SPDY_SESSION_STATUS_CLOSING == pos->status) //the session is about to be closed
		    || (daemon->session_timeout //timeout passed for the session
			&& (pos->last_activity + daemon->session_timeout < SPDYF_monotonic_time()))
		    || (SPDY_YES == pos->fio_is_pending(pos)) //data in TLS' read buffer pending
		    || ((pos->read_buffer_offset - pos->read_buffer_beginning) > 0) // data in lib's read buffer pending
		    )
			FD_SET(fd, write_fd_set);
		if(fd > max_fd)
			max_fd = fd;
	}

	return max_fd;
}


void 
SPDYF_run (struct SPDY_Daemon *daemon)
{
	struct SPDY_Session *pos;
	struct SPDY_Session *next;
	int num_ready;
	fd_set rs;
	fd_set ws;
	fd_set es;
	int max;
	struct timeval timeout;
	int ds;

	timeout.tv_sec = 0;
	timeout.tv_usec = 0;
	FD_ZERO (&rs);
	FD_ZERO (&ws);
	FD_ZERO (&es);
	//here we need really all descriptors to see later which are ready
	max = SPDYF_get_fdset(daemon,&rs,&ws,&es, true);

	num_ready = select (max + 1, &rs, &ws, &es, &timeout);

	if(num_ready < 1)
		return;

	if ( (-1 != (ds = daemon->socket_fd)) &&
		(FD_ISSET (ds, &rs)) ){
		SPDYF_session_accept(daemon);
	}

	next = daemon->sessions_head;
	while (NULL != (pos = next))
	{
		next = pos->next;
		ds = pos->socket_fd;
		if (ds != -1)
		{
			//fill the read buffer
			if (FD_ISSET (ds, &rs) || pos->fio_is_pending(pos)){
				SPDYF_session_read(pos);
			}
			
			//do something with the data in read buffer
			if(SPDY_NO == SPDYF_session_idle(pos))
			{
				//the session was closed, cannot write anymore
				//continue;
			}
			
			//write whatever has been put to the response queue
			//during read or idle operation, something might be put
			//on the response queue, thus call write operation
			if (FD_ISSET (ds, &ws)){
				if(SPDY_NO == SPDYF_session_write(pos, false))
				{
					//SPDYF_session_close(pos);
					//continue;
				}
			}
			
			/* the response queue has been flushed for half closed
			 * connections, so let close them */
			/*if(pos->read_closed)
			{
				SPDYF_session_close(pos);
			}*/
		}
	}
	
	spdyf_cleanup_sessions(daemon);
}
