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
 * @file misc.c
 * @brief  tests a lot of small calls and callbacks. TODO mention what
 * @author Andrey Uzunov
 */

#include "platform.h"
#include "microspdy.h"
#include "stdio.h"
#include <sys/wait.h>
#include "common.h"

int port;

#define HTML "<html><head>\
<link href=\"main.css\" rel=\"stylesheet\" type=\"text/css\" />\
</head><body>This is libmicrospdy</body></html>"

#define CSS "body{font-size:15px}"

#define SESSION_CLS "1234567890"

#define REQUEST_CLS "1234567890REQ"

pid_t parent;
pid_t child;

struct SPDY_Session *session1;
struct SPDY_Session *session2;

void
killchild()
{
	kill(child, SIGKILL);
	exit(1);
}

void
killparent()
{
	kill(parent, SIGKILL);
	_exit(1);
}


void
create_child()
{
	parent = getpid();

	child = fork();
	if (-1 == child)
	{
		fprintf(stderr, "can't fork, error %d\n", errno);
		exit(EXIT_FAILURE);
	}

	if (child == 0)
	{
		int devnull;
		char *uri;
		fflush(stdout);
		devnull = open("/dev/null", O_WRONLY);
                if (-1 == devnull)
                  abort ();
		if (1 != devnull)
		{
			dup2(devnull, 1);
			close(devnull);
		}
		asprintf(&uri,"https://127.0.0.1:%i/",port);
		execlp("spdycat", "spdycat","-anv",uri,NULL );
		printf("execlp failed\n");
		killparent();
	}
}

void
response_done_callback(void *cls,
								struct SPDY_Response * response,
								struct SPDY_Request * request,
								enum SPDY_RESPONSE_RESULT status,
						bool streamopened)
{
  (void)status;
  (void)streamopened;

	if(strcmp(cls,"/main.css"))
	{
		session1 = SPDY_get_session_for_request(request);
		if(NULL == session1)
		{
			printf("SPDY_get_session_for_request failed\n");
			killchild();
		}

		char *session_cls = strdup(SESSION_CLS);
		SPDY_set_cls_to_session(session1,session_cls);
	}
	else
	{
		session2 = SPDY_get_session_for_request(request);
		if(session1 != session2)
		{
			printf("SPDY_get_session_for_request failed the second time\n");
			killchild();
		}
		printf("SPDY_get_session_for_request tested...\n");

		void *session_cls = SPDY_get_cls_from_session(session2);
		if(NULL == session_cls || strcmp(session_cls, SESSION_CLS))
		{
			printf("SPDY_get_cls_from_session failed\n");
			killchild();
		}
		printf("SPDY_set_cls_to_session tested...\n");
		printf("SPDY_get_cls_from_session tested...\n");

		void *request_cls = SPDY_get_cls_from_request(request);
		if(NULL == request_cls || strcmp(request_cls, REQUEST_CLS))
		{
			printf("SPDY_get_cls_from_request failed\n");
			killchild();
		}
		printf("SPDY_set_cls_to_request tested...\n");
		printf("SPDY_get_cls_from_request tested...\n");
	}

	SPDY_destroy_request(request);
	SPDY_destroy_response(response);
	free(cls);
}

void
standard_request_handler(void *cls,
						struct SPDY_Request * request,
						uint8_t priority,
                        const char *method,
                        const char *path,
                        const char *version,
                        const char *host,
                        const char *scheme,
						struct SPDY_NameValue * headers,
            bool more)
{
	(void)cls;
	(void)request;
	(void)priority;
	(void)host;
	(void)scheme;
	(void)headers;
	(void)method;
	(void)version;
	(void)more;

	struct SPDY_Response *response=NULL;
	char *cls_path = strdup(path);

	if(strcmp(path,"/main.css")==0)
	{
		char *request_cls = strdup(REQUEST_CLS);
		SPDY_set_cls_to_request(request,request_cls);
		response = SPDY_build_response(200,NULL,SPDY_HTTP_VERSION_1_1,NULL,CSS,strlen(CSS));
	}
	else
	{
		response = SPDY_build_response(200,NULL,SPDY_HTTP_VERSION_1_1,NULL,HTML,strlen(HTML));
	}

	if(NULL==response){
		fprintf(stdout,"no response obj\n");
		killchild();
	}

	if(SPDY_queue_response(request,response,true,false,&response_done_callback,cls_path)!=SPDY_YES)
	{
		fprintf(stdout,"queue\n");
		killchild();
	}
}

int
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

	daemon = SPDY_start_daemon(port,
								DATA_DIR "cert-and-key.pem",
								DATA_DIR "cert-and-key.pem",
								NULL,
								NULL,
								&standard_request_handler,
								NULL,
								NULL,
								SPDY_DAEMON_OPTION_SESSION_TIMEOUT,
								1800,
								SPDY_DAEMON_OPTION_END);

	if(NULL==daemon){
		printf("no daemon\n");
		return 1;
	}

	create_child();

	do
	{
		FD_ZERO(&read_fd_set);
		FD_ZERO(&write_fd_set);
		FD_ZERO(&except_fd_set);

		ret = SPDY_get_timeout(daemon, &timeoutlong);
		if(SPDY_NO == ret || timeoutlong > 1000)
		{
			timeout.tv_sec = 1;
      timeout.tv_usec = 0;
		}
		else
		{
			timeout.tv_sec = timeoutlong / 1000;
			timeout.tv_usec = (timeoutlong % 1000) * 1000;
		}

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

				break;
			default:
				SPDY_run(daemon);

			break;
		}
	}
	while(waitpid(child,&childstatus,WNOHANG) != child);

	SPDY_stop_daemon(daemon);

	return WEXITSTATUS(childstatus);
}


int
main()
{
	port = get_port(13123);
	SPDY_init();

	int ret = parentproc();

	SPDY_deinit();

	return ret;
}
