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
 * @file fileserver.c
 * @brief   Simple example how the lib can be used for serving
 * 			files directly read from the system
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
#include "time.h"


int run = 1;
char* basedir;


#define GET_MIME_TYPE(fname, mime)	do {\
		unsigned int __len = strlen(fname);\
		if (__len < 4 || '.' != (fname)[__len - 4]) \
		{	\
			(mime) = strdup("application/octet-stream");\
			printf("MIME for %s is applic...\n", (fname));\
		}\
    else {\
      const char * __ext = &(fname)[__len - 3];\
      if(0 == strcmp(__ext, "jpg")) (mime) = strdup("image/jpeg");\
      else if(0 == strcmp(__ext, "png")) (mime) = strdup("image/png");\
      else if(0 == strcmp(__ext, "css")) (mime) = strdup("text/css");\
      else if(0 == strcmp(__ext, "gif")) (mime) = strdup("image/gif");\
      else if(0 == strcmp(__ext, "htm")) (mime) = strdup("text/html");\
      else \
      {	\
        (mime) = strdup("application/octet-stream");\
        printf("MIME for %s is applic...\n", (fname));\
      }\
    }\
		if(NULL == (mime))\
		{\
			printf("no memory\n");\
			abort();\
		}\
	} while (0)


static const char *DAY_NAMES[] =
  { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };

static const char *MONTH_NAMES[] =
  { "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

//taken from http://stackoverflow.com/questions/2726975/how-can-i-generate-an-rfc1123-date-string-from-c-code-win32
//and modified for linux
char *Rfc1123_DateTimeNow()
{
    const int RFC1123_TIME_LEN = 29;
    time_t t;
    struct tm tm;
    char * buf = malloc(RFC1123_TIME_LEN+1);

    if (NULL == buf)
      return NULL;
    time(&t);
    gmtime_r( &t, &tm);

    strftime(buf, RFC1123_TIME_LEN+1, "---, %d --- %Y %H:%M:%S GMT", &tm);
    memcpy(buf, DAY_NAMES[tm.tm_wday], 3);
    memcpy(buf+8, MONTH_NAMES[tm.tm_mon], 3);

    return buf;
}


ssize_t
response_callback (void *cls,
						void *buffer,
						size_t max,
						bool *more)
{
	FILE *fd =(FILE*)cls;

	int ret = fread(buffer,1,max,fd);
	*more = feof(fd) == 0;

	//if(!(*more))
	//	fclose(fd);

	return ret;
}


void
response_done_callback(void *cls,
						struct SPDY_Response *response,
						struct SPDY_Request *request,
						enum SPDY_RESPONSE_RESULT status,
						bool streamopened)
{
	(void)streamopened;
	(void)status;
	//printf("answer for %s was sent\n", (char *)cls);

	/*if(SPDY_RESPONSE_RESULT_SUCCESS != status)
	{
		printf("answer for %s was NOT sent, %i\n", (char *)cls,status);
	}*/

	SPDY_destroy_request(request);
	SPDY_destroy_response(response);
	if(NULL!=cls)fclose(cls);
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
	struct SPDY_NameValue *resp_headers;
	char *fname;
	char *fsize;
	char *mime=NULL;
	char *date=NULL;
	ssize_t filesize = -666;
	FILE *fd = NULL;
	int ret = -666;

	//printf("received request for '%s %s %s'\n", method, path, version);
	if(strlen(path) > 1 && NULL == strstr(path, "..") && '/' == path[0])
	{
		asprintf(&fname,"%s%s",basedir,path);
		if(0 == access(fname, R_OK))
		{
			if(NULL == (fd = fopen(fname,"r"))
				|| 0 != (ret = fseek(fd, 0L, SEEK_END))
				|| -1 == (filesize = ftell(fd))
				|| 0 != (ret = fseek(fd, 0L, SEEK_SET)))
			{
				printf("Error on opening %s\n%p %i %zd\n",fname, fd, ret, filesize);
				response = SPDY_build_response(SPDY_HTTP_INTERNAL_SERVER_ERROR,NULL,SPDY_HTTP_VERSION_1_1,NULL,NULL,0);
			}
			else
			{
				if(NULL == (resp_headers = SPDY_name_value_create()))
				{
					printf("SPDY_name_value_create failed\n");
					abort();
				}

				date = Rfc1123_DateTimeNow();
				if(NULL == date
					|| SPDY_YES != SPDY_name_value_add(resp_headers,SPDY_HTTP_HEADER_DATE,date))
				{
					printf("SPDY_name_value_add or Rfc1123_DateTimeNow failed\n");
					abort();
				}
				free(date);

				if(-1 == asprintf(&fsize, "%zd", filesize)
					|| SPDY_YES != SPDY_name_value_add(resp_headers,SPDY_HTTP_HEADER_CONTENT_LENGTH,fsize))
				{
					printf("SPDY_name_value_add or asprintf failed\n");
					abort();
				}
				free(fsize);

				GET_MIME_TYPE(path,mime);
				if(SPDY_YES != SPDY_name_value_add(resp_headers,SPDY_HTTP_HEADER_CONTENT_TYPE,mime))
				{
					printf("SPDY_name_value_add failed\n");
					abort();
				}
				free(mime);

				if(SPDY_YES != SPDY_name_value_add(resp_headers,SPDY_HTTP_HEADER_SERVER,"libmicrospdy/fileserver"))
				{
					printf("SPDY_name_value_add failed\n");
					abort();
				}

				response = SPDY_build_response_with_callback(200,NULL,
					SPDY_HTTP_VERSION_1_1,resp_headers,&response_callback,fd,SPDY_MAX_SUPPORTED_FRAME_SIZE);
				SPDY_name_value_destroy(resp_headers);
			}

			if(NULL==response){
				printf("no response obj\n");
				abort();
			}

			if(SPDY_queue_response(request,response,true,false,&response_done_callback,fd)!=SPDY_YES)
			{
				printf("queue\n");
				abort();
			}

			free(fname);
			return;
		}
		free(fname);
	}

	if(strcmp(path,"/close")==0)
	{
		run = 0;
	}

	response = SPDY_build_response(SPDY_HTTP_NOT_FOUND,NULL,SPDY_HTTP_VERSION_1_1,NULL,NULL,0);
	printf("Not found %s\n",path);

	if(NULL==response){
		printf("no response obj\n");
		abort();
	}

	if(SPDY_queue_response(request,response,true,false,&response_done_callback,NULL)!=SPDY_YES)
	{
		printf("queue\n");
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

	if(argc != 5)
	{
		printf("Usage: %s cert-file key-file base-dir port\n", argv[0]);
		return 1;
	}

	SPDY_init();

	daemon = SPDY_start_daemon(atoi(argv[4]),
								argv[1],
								argv[2],
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

	basedir = argv[3];

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

