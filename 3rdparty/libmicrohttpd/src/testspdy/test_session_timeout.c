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
 * @file session_timeout.c
 * @brief  tests closing sessions after set timeout. Openssl is used for
 * 			client
 * @author Andrey Uzunov
 */

#include "platform.h"
#include "microspdy.h"
#include "stdio.h"
#include <sys/wait.h>
#include <ctype.h>
#include "common.h"
#include <sys/time.h>
#include <sys/stat.h>
#include "../microspdy/internal.h"

#define TIMEOUT 2
#define SELECT_MS_TIMEOUT 20

int port;

pid_t parent;
pid_t child;

int run = 1;
int chunk_size=1;
int new_session;
int closed_session;
int do_sleep;



static unsigned long long
monotonic_time (void)
{
#ifdef HAVE_CLOCK_GETTIME
#ifdef CLOCK_MONOTONIC
  struct timespec ts;
  if (0 == clock_gettime (CLOCK_MONOTONIC, &ts))
    return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
#endif
#endif
  return time (NULL) * 1000;
}


static void
killchild(char *msg)
{
	printf("%s\n",msg);
	kill(child, SIGKILL);
	exit(1);
}


static void
killparent(char *msg)
{
	printf("%s\n",msg);
	kill(parent, SIGKILL);
	_exit(1);
}


static void
new_session_cb (void *cls,
                struct SPDY_Session * session)
{
  (void)cls;
  (void)session;

	if(!new_session)do_sleep = 1;
	new_session = 1;
	printf("new session\n");
}


static void
closed_session_cb (void *cls,
                   struct SPDY_Session * session,
                   int by_client)
{
  (void)cls;
  (void)session;

	printf("closed_session_cb called\n");

	if(SPDY_YES == by_client)
	{
		killchild("closed by the client");
	}
	if(closed_session)
	{
		killchild("closed_session_cb called twice");
	}

	closed_session = 1;
}


static int
parentproc()
{
	int childstatus;
	unsigned long long timeoutlong=0;
	struct timeval timeout;
	int ret;
	fd_set read_fd_set;
	fd_set write_fd_set;
	fd_set except_fd_set;
	int maxfd = -1;
	struct SPDY_Daemon *daemon;
  unsigned long long  beginning = 0;
	unsigned long long now;

	SPDY_init();

	daemon = SPDY_start_daemon(port,
								DATA_DIR "cert-and-key.pem",
								DATA_DIR "cert-and-key.pem",
								&new_session_cb,
								&closed_session_cb,
								NULL,
								NULL,
								NULL,
								SPDY_DAEMON_OPTION_SESSION_TIMEOUT,
								TIMEOUT,
								SPDY_DAEMON_OPTION_END);

	if(NULL==daemon){
		printf("no daemon\n");
		return 1;
	}

	do
	{
		do_sleep=0;
		FD_ZERO(&read_fd_set);
		FD_ZERO(&write_fd_set);
		FD_ZERO(&except_fd_set);

		ret = SPDY_get_timeout(daemon, &timeoutlong);

		if(new_session && !closed_session)
		{
			if(SPDY_NO == ret)
			{
				killchild("SPDY_get_timeout returned wrong SPDY_NO");
			}
			/*if(timeoutlong)
			{
				killchild("SPDY_get_timeout returned wrong timeout");
			}*/
			now = monotonic_time ();
      if(now - beginning > TIMEOUT*1000 + SELECT_MS_TIMEOUT)
      {
        printf("Started at: %llums\n",beginning);
        printf("Now is: %llums\n",now);
        printf("Timeout is: %i\n",TIMEOUT);
        printf("Select Timeout is: %ims\n",SELECT_MS_TIMEOUT);
        printf("SPDY_get_timeout gave: %llums\n",timeoutlong);
				killchild("Timeout passed but session was not closed");
      }
      if(timeoutlong > beginning + TIMEOUT *1000)
      {
        printf("Started at: %llums\n",beginning);
        printf("Now is: %llums\n",now);
        printf("Timeout is: %i\n",TIMEOUT);
        printf("Select Timeout is: %ims\n",SELECT_MS_TIMEOUT);
        printf("SPDY_get_timeout gave: %llums\n",timeoutlong);
				killchild("SPDY_get_timeout returned wrong timeout");
      }
		}
		else
		{
			if(SPDY_YES == ret)
			{
				killchild("SPDY_get_timeout returned wrong SPDY_YES");
			}
		}

		if(SPDY_NO == ret || timeoutlong >= 1000)
		{
			timeout.tv_sec = 1;
      timeout.tv_usec = 0;
		}
		else
		{
			timeout.tv_sec = timeoutlong / 1000;
      timeout.tv_usec = (timeoutlong % 1000) * 1000;
		}

    //ignore values
    timeout.tv_sec = 0;
    timeout.tv_usec = SELECT_MS_TIMEOUT * 1000;

		maxfd = SPDY_get_fdset (daemon,
								&read_fd_set,
								&write_fd_set,
								&except_fd_set);

		ret = select(maxfd+1, &read_fd_set, &write_fd_set, &except_fd_set, &timeout);

		switch(ret) {
			case -1:
				printf("select error: %i\n", errno);
				break;
			case 0:
				/*if(new_session)
				{
					killchild("select returned wrong number");
				}*/
				break;
			default:
				SPDY_run(daemon);
        if(0 == beginning)
        {
	  beginning = monotonic_time ();
        }
				/*if(do_sleep)
				{
					sleep(TIMEOUT);
					do_sleep = 0;
				}*/
			break;
		}
	}
	while(waitpid(child,&childstatus,WNOHANG) != child);

	if(!new_session || !closed_session)
	{
		killchild("child is dead, callback wasn't called");
	}

	ret = SPDY_get_timeout(daemon, &timeoutlong);

	if(SPDY_YES == ret)
	{
		killchild("SPDY_get_timeout returned wrong SPDY_YES after child died");
	}

	SPDY_stop_daemon(daemon);

	SPDY_deinit();

	return 0;
}


static int
childproc()
{
	pid_t devnull;
	int out;

	out=dup(1);
        if (-1 == out)
          abort();
	//close(0);
	close(1);
	close(2);
	/*devnull = open("/dev/null", O_RDONLY);
	if (0 != devnull)
	{
		dup2(devnull, 0);
		close(devnull);
	}*/
	devnull = open("/dev/null", O_WRONLY);
        if (-1 == devnull)
          abort ();
	if (1 != devnull)
	{
		dup2(devnull, 1);
		close(devnull);
	}
	devnull = open("/dev/null", O_WRONLY);
        if (-1 == devnull)
          abort ();
	if (2 != devnull)
	{
		dup2(devnull, 2);
		close(devnull);
	}
	char *uri;
	asprintf (&uri, "127.0.0.1:%i", port);
	execlp ("openssl", "openssl", "s_client", "-connect", uri, NULL);
	close(1);
	dup2(out,1);
	close(out);
	killparent ("executing openssl failed");
	return 1;
}


int
main()
{
	port = get_port(11123);
	parent = getpid();

	child = fork();
	if (-1 == child)
	{
		fprintf(stderr, "can't fork, error %d\n", errno);
		exit(EXIT_FAILURE);
	}

	if (child == 0)
	{
		int ret = childproc();
		_exit(ret);
	}
	else
	{
		int ret = parentproc();
		exit(ret);
	}
	return 1;
}
