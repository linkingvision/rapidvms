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
 * @file response_with_callback.c
 * @brief  shows how to create responses with callbacks
 * @author Andrey Uzunov
 */
 
//for asprintf
#define _GNU_SOURCE 

#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include "microspdy.h"

static int run = 1;


static ssize_t
response_callback (void *cls,
						void *buffer,
						size_t max,
						bool *more)
{
	FILE *fd =(FILE*)cls;
	
	int ret = fread(buffer,1,max,fd);
	*more = feof(fd) == 0;
	
	if(!(*more))
		fclose(fd);
	
	return ret;
}


static void
response_done_callback(void *cls,
		       struct SPDY_Response *response,
		       struct SPDY_Request *request,
		       enum SPDY_RESPONSE_RESULT status,
		       bool streamopened)
{
	(void)streamopened;
	(void)status;
  
	printf("answer for %s was sent\n", (char *)cls);
	
	SPDY_destroy_request(request);
	SPDY_destroy_response(response);
	free(cls);
}


static void
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
	(void)more;
	
	char *html;
	struct SPDY_Response *response=NULL;
	struct SPDY_NameValue *resp_headers;
	
	printf("received request for '%s %s %s'\n", method, path, version);
	if(strcmp(path,"/spdy-draft.txt")==0)
	{
		FILE *fd = fopen(DATA_DIR "spdy-draft.txt","r");
		
		if(NULL == (resp_headers = SPDY_name_value_create()))
		{
			fprintf(stdout,"SPDY_name_value_create failed\n");
			abort();
		}
		if(SPDY_YES != SPDY_name_value_add(resp_headers,SPDY_HTTP_HEADER_CONTENT_TYPE,"text/plain"))
		{
			fprintf(stdout,"SPDY_name_value_add failed\n");
			abort();
		}
		
		response = SPDY_build_response_with_callback(200,NULL,
			SPDY_HTTP_VERSION_1_1,resp_headers,&response_callback,fd,SPDY_MAX_SUPPORTED_FRAME_SIZE);
		SPDY_name_value_destroy(resp_headers);
	}
	else
	{
		if(strcmp(path,"/close")==0)
		{
			asprintf(&html,"<html>"
		"<body><b>Closing now!</body></html>");
			run = 0;
		}
		else
		{
			asprintf(&html,"<html>"
		"<body><a href=\"/spdy-draft.txt\">/spdy-draft.txt</a><br></body></html>");
		}
		
		response = SPDY_build_response(200,NULL,SPDY_HTTP_VERSION_1_1,NULL,html,strlen(html));
		free(html);
	}
	
	if(NULL==response){
		fprintf(stdout,"no response obj\n");
		abort();
	}
	
	void *clspath = strdup(path);
	
	if(SPDY_queue_response(request,response,true,false,&response_done_callback,clspath)!=SPDY_YES)
	{
		fprintf(stdout,"queue\n");
		abort();
	}
}


int
main (int argc, char *const *argv)
{	
	unsigned long long timeoutlong=0;
	struct timeval timeout;
	int ret;
	fd_set read_fd_set;
	fd_set write_fd_set;
	fd_set except_fd_set;
	int maxfd = -1;
	struct SPDY_Daemon *daemon;
	
	if(argc != 2)
	{
		return 1;
	}

	SPDY_init();
	
	daemon = SPDY_start_daemon(atoi(argv[1]),
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
	while(run);

	SPDY_stop_daemon(daemon);
	
	SPDY_deinit();
	
	return 0;
}

