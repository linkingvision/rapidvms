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
 * @file io_raw.c
 * @brief  IO for SPDY without TLS.
 * @author Andrey Uzunov
 */

#include "platform.h"
#include "internal.h"
#include "session.h"
#include "io_raw.h"
//TODO put in in the right place
#include <netinet/tcp.h>


void
SPDYF_raw_global_init()
{
}


void
SPDYF_raw_global_deinit()
{
}


int
SPDYF_raw_init(struct SPDY_Daemon *daemon)
{
  (void)daemon;
  
	return SPDY_YES;
}


void
SPDYF_raw_deinit(struct SPDY_Daemon *daemon)
{
  (void)daemon;
}


int
SPDYF_raw_new_session(struct SPDY_Session *session)
{	
  int fd_flags;
  int val = 1;
  int ret;
  
	//setting the socket to be non-blocking
	fd_flags = fcntl (session->socket_fd, F_GETFL);
	if ( -1 == fd_flags
		|| 0 != fcntl (session->socket_fd, F_SETFL, fd_flags | O_NONBLOCK))
		SPDYF_DEBUG("WARNING: Couldn't set the new connection to be non-blocking");
  
  if(SPDY_DAEMON_FLAG_NO_DELAY & session->daemon->flags)
  {
    ret = setsockopt(session->socket_fd, IPPROTO_TCP, TCP_NODELAY, &val, (socklen_t)sizeof(val));
    if(-1 == ret)
      SPDYF_DEBUG("WARNING: Couldn't set the new connection to TCP_NODELAY");
  }
  
	return SPDY_YES;
}


void
SPDYF_raw_close_session(struct SPDY_Session *session)
{
  (void)session;
}


int
SPDYF_raw_recv(struct SPDY_Session *session,
				void * buffer,
				size_t size)
{
	int n = read(session->socket_fd, 
					buffer,
					size);
	//if(n > 0) SPDYF_DEBUG("recvd: %i",n);
	if (n < 0)
	{
		switch(errno)
		{				
			case EAGAIN:
#if EAGAIN != EWOULDBLOCK
      case EWOULDBLOCK:
#endif
			case EINTR:
        return SPDY_IO_ERROR_AGAIN;
				
			default:
				return SPDY_IO_ERROR_ERROR;
		}
	}

	return n;
}


int
SPDYF_raw_send(struct SPDY_Session *session,
				const void * buffer,
				size_t size)
{
	int n = write(session->socket_fd, 
					buffer,
					size);
	//if(n > 0) SPDYF_DEBUG("sent: %i",n);
	if (n < 0)
	{
		switch(errno)
		{				
			case EAGAIN:
#if EAGAIN != EWOULDBLOCK
      case EWOULDBLOCK:
#endif
			case EINTR:
        return SPDY_IO_ERROR_AGAIN;
				
			default:
				return SPDY_IO_ERROR_ERROR;
		}
	}
	
	return n;
}


int
SPDYF_raw_is_pending(struct SPDY_Session *session)
{
  (void)session;
  
	return SPDY_NO;
}


int
SPDYF_raw_before_write(struct SPDY_Session *session)
{
#if HAVE_DECL_TCP_CORK
  if(0 == (SPDY_DAEMON_FLAG_NO_DELAY & session->daemon->flags))
  {
    int val = 1;
    int ret;
    
    ret = setsockopt(session->socket_fd, IPPROTO_TCP, TCP_CORK, &val, (socklen_t)sizeof(val));
    if(-1 == ret)
      SPDYF_DEBUG("WARNING: Couldn't set the new connection to TCP_CORK");
  }
#endif
  
	return SPDY_YES;
}


int
SPDYF_raw_after_write(struct SPDY_Session *session, int was_written)
{
#if HAVE_DECL_TCP_CORK
  if(SPDY_YES == was_written && 0 == (SPDY_DAEMON_FLAG_NO_DELAY & session->daemon->flags))
  {
    int val = 0;
    int ret;
    
    ret = setsockopt(session->socket_fd, IPPROTO_TCP, TCP_CORK, &val, (socklen_t)sizeof(val));
    if(-1 == ret)
      SPDYF_DEBUG("WARNING: Couldn't unset the new connection to TCP_CORK");
  }
  
#endif
	return was_written;
}
