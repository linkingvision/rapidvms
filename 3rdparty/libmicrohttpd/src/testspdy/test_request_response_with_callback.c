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
 * @file request_response_with_callback.c
 * @brief  tests responses with callbacks
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

int port;

pid_t parent;
pid_t child;

int run = 1;
int chunk_size=1;

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

ssize_t
response_callback (void *cls,
						void *buffer,
						size_t max,
						bool *more)
{
	FILE *fd =(FILE*)cls;
	
	size_t n;
	if(chunk_size % 2)
		n = chunk_size;
	else
		n = max - chunk_size;
	
	if(n < 1) n = 1;
	else if (n > max) n=max;
	chunk_size++;
	
	int ret = fread(buffer,1,n,fd);
	*more = feof(fd) == 0;
	
	//printf("more is %i\n",*more);
	
	if(!(*more))
		fclose(fd);
	
	return ret;
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
  
	printf("answer for %s was sent\n", (char*)cls);
	
	SPDY_destroy_request(request);
	SPDY_destroy_response(response);
	free(cls);
	
	run = 0;
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
	
	printf("received request for '%s %s %s'\n", method, path, version);
	
		FILE *fd = fopen(DATA_DIR "spdy-draft.txt","r");
		
		if(NULL == (resp_headers = SPDY_name_value_create()))
		{
			fprintf(stdout,"SPDY_name_value_create failed\n");
			killchild();
		}
		if(SPDY_YES != SPDY_name_value_add(resp_headers,SPDY_HTTP_HEADER_CONTENT_TYPE,"text/plain"))
		{
			fprintf(stdout,"SPDY_name_value_add failed\n");
			killchild();
		}
		
		response = SPDY_build_response_with_callback(200,NULL,
			SPDY_HTTP_VERSION_1_1,resp_headers,&response_callback,fd,SPDY_MAX_SUPPORTED_FRAME_SIZE);
		SPDY_name_value_destroy(resp_headers);
	
	if(NULL==response){
		fprintf(stdout,"no response obj\n");
			killchild();
	}
	
	void *clspath = strdup(path);
	
	if(SPDY_queue_response(request,response,true,false,&response_done_callback,clspath)!=SPDY_YES)
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
	
	SPDY_init();
	
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
	
	SPDY_deinit();
	
	return WEXITSTATUS(childstatus);
}

#define MD5_LEN 32

int
md5(char *cmd, char *md5_sum)
{
    FILE *p = popen(cmd, "r");
    if (p == NULL) return 0;

    int i, ch;
    for (i = 0; i < MD5_LEN && isxdigit(ch = fgetc(p)); i++) {
        *md5_sum++ = ch;
    }

    *md5_sum = '\0';
    pclose(p);
    return i == MD5_LEN;
}

int
childproc()
{
	char *cmd1;
	char *cmd2;
	char md5_sum1[33];
	char md5_sum2[33];
	int ret;
	struct timeval tv1;
	struct timeval tv2;
	struct stat st;
	//int secs;
	uint64_t usecs;
	
	asprintf(&cmd1, "spdycat https://127.0.0.1:%i/ | md5sum",port);
	asprintf(&cmd2, "md5sum " DATA_DIR "spdy-draft.txt");
	
	gettimeofday(&tv1, NULL);
	md5(cmd1,md5_sum1);
	gettimeofday(&tv2, NULL);
	md5(cmd2,md5_sum2);
	
	printf("downloaded file md5: %s\n", md5_sum1);
	printf("original   file md5: %s\n", md5_sum2);
	ret = strcmp(md5_sum1, md5_sum2);
	
	if(0 == ret && 0 == stat(DATA_DIR "spdy-draft.txt", &st))
	{
		usecs = (uint64_t)1000000 * (uint64_t)(tv2.tv_sec - tv1.tv_sec) + tv2.tv_usec - tv1.tv_usec;
		printf("%lld bytes read in %llu usecs\n", (long long)st.st_size, (long long unsigned )usecs);
	}
	
	return ret;
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
