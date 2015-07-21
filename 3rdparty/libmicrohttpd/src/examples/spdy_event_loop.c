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
 * @file event_loop.c
 * @brief  shows how to use the daemon. THIS IS MAINLY A TEST AND DEBUG
 * 		 PROGRAM
 * @author Andrey Uzunov
 */

#include "platform.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include "microspdy.h"
#include <sys/time.h>
#include <time.h>
#ifndef MINGW
#include <arpa/inet.h>
#endif
//#include "../framinglayer/structures.h"
//#include "../applicationlayer/alstructures.h"

static int run = 1;

static int run2 = 1;

	
static uint64_t loops;

static time_t start;


static void
new_session_callback (void *cls,
						struct SPDY_Session * session)
{
  (void)cls;
  
	char ipstr[1024];
		
	struct sockaddr *addr;
	socklen_t addr_len = SPDY_get_remote_addr(session, &addr);	
	
	if(!addr_len)
	{
		printf("SPDY_get_remote_addr");
		abort();
	}
	
	if(AF_INET == addr->sa_family)
	{
		struct sockaddr_in * addr4 = (struct sockaddr_in *) addr;
		if(NULL == inet_ntop(AF_INET, &(addr4->sin_addr), ipstr, sizeof(ipstr)))
		{
			printf("inet_ntop");
			abort();
		}
		printf("New connection from: %s:%i\n", ipstr, ntohs(addr4->sin_port));
		
	}
	else if(AF_INET6 == addr->sa_family)
	{
		struct sockaddr_in6 * addr6 = (struct sockaddr_in6 *) addr;
		if(NULL == inet_ntop(AF_INET6, &(addr6->sin6_addr), ipstr, sizeof(ipstr)))
		{
			printf("inet_ntop");
			abort();
		}
		printf("New connection from: %s:%i\n", ipstr, ntohs(addr6->sin6_port));
		
	}
}


static void
session_closed_handler (void *cls,
						struct SPDY_Session * session,
						int by_client)
{
  (void)cls;
  (void)session;
  
	//printf("session_closed_handler called\n");
	
	if(SPDY_YES != by_client)
	{
		//killchild(child,"wrong by_client");
		printf("session closed by server\n");
	}
	else
	{
		printf("session closed by client\n");
	}
	
	//session_closed_called = 1;
}


static void
response_done_callback(void *cls,
						struct SPDY_Response *response,
						struct SPDY_Request *request,
						enum SPDY_RESPONSE_RESULT status,
						bool streamopened)
{
	(void)streamopened;
	if(strcmp(cls, "/close (daemon1)") == 0)
		run = 0;
	else {
		if(strcmp(cls, "/close (daemon2)") == 0) run2 = 0;
		loops = 0;
		start = time(NULL);
	}
	if(SPDY_RESPONSE_RESULT_SUCCESS != status)
	{
		printf("not sent frame cause %i", status);
	}
	printf("answer for %s was sent\n", (char*)cls);
	//printf("raw sent headers %s\n", (char *)(response->headers)+8);
	
	SPDY_destroy_request(request);
	SPDY_destroy_response(response);
	free(cls);
}

/*
static int
print_headers (void *cls,
                           const char *name, const char *value)
{
	(void)cls;
	printf("%s: %s\n",name,value);
	return SPDY_YES;
}
 */
 
 
/*       
void
new_request_cb (void *cls,
						struct SPDY_Request * request,
						uint8_t priority,
                        const char *method,
                        const char *path,
                        const char *version,
                        const char *host,
                        const char *scheme,
						struct SPDY_NameValue * headers)
{
	(void)cls;
	(void)request;
	printf("Priority: %i\nHTTP headers, scheme: %s\n\n%s %s %s\nHost: %s\n", priority,scheme,method,path,version,host);
	SPDY_name_value_iterate(headers, &print_headers, NULL);
}
*/


static int
append_headers_to_data (void *cls,
                           const char *name, const char * const *value, int num_values)
{
	char **data = cls;
	void *tofree = *data;
	int i;
	
	if(num_values)
	for(i=0;i<num_values;++i)
	{
	asprintf(data,"%s%s: %s\n", *data,name,value[i]);
	}
	else
	asprintf(data,"%s%s: \n", *data,name);
	
	free(tofree);
	return SPDY_YES;
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
  (void)more;
  
	char *html;
	char *data;
	struct SPDY_Response *response=NULL;
	
	printf("received request for '%s %s %s'\n", method, path, version);
	if(strcmp(path,"/main.css")==0)
	{
		if(NULL != cls)
			asprintf(&html,"body{color:green;}");
		else
			asprintf(&html,"body{color:red;}");
				
		//struct SPDY_NameValue *headers=SPDY_name_value_create();
		//SPDY_name_value_add(headers,"content-type","text/css");
		
		response = SPDY_build_response(200,NULL,SPDY_HTTP_VERSION_1_1,NULL,html,strlen(html));
		free(html);
	}
	else
	{
		asprintf(&data,"Priority: %i\nHTTP headers, scheme: %s\n\n%s %s %s\nHost: %s\n", priority,scheme,method,path,version,host);
		
		SPDY_name_value_iterate(headers, &append_headers_to_data, &data);
		
		if(strcmp(path,"/close")==0)
		{
			asprintf(&html,"<html>"
		"<body><b>Closing now!<br>This is an answer to the following "
		"request:</b><br><br><pre>%s</pre></body></html>",data);
		}
		else
		{
			asprintf(&html,"<html><link href=\"main.css\" rel=\"stylesheet\" type=\"text/css\" />"
		"<body><b>This is an answer to the following "
		"request:</b><br><br><pre>%s</pre></body></html>",data);
		}
		
		free(data);
		
		response = SPDY_build_response(200,NULL,SPDY_HTTP_VERSION_1_1,NULL,html,strlen(html));
		free(html);
	}
	
	if(NULL==response){
		fprintf(stdout,"no response obj\n");
		abort();
	}
	
	char *pathcls;
	asprintf(&pathcls, "%s (daemon%i)",path,NULL==cls ? 1 : 2);
	if(SPDY_queue_response(request,response,true,false,&response_done_callback,pathcls)!=SPDY_YES)
	{
		fprintf(stdout,"queue\n");
		abort();
	}
}


static int
new_post_data_cb (void * cls,
					 struct SPDY_Request *request,
					 const void * buf,
					 size_t size,
					 bool more)
{
  (void)cls;
  (void)request;
  (void)more;
  
	printf("DATA:\n===============================\n");
  write(0, buf, size);
	printf("\n===============================\n");
  return SPDY_YES;
}


static void 
sig_handler(int signo)
{
  (void)signo;
  
  printf("received signal\n");
}


int
main (int argc, char *const *argv)
{	
	if(argc != 2) return 1;
	
	#ifndef MINGW
	if (signal(SIGPIPE, sig_handler) == SIG_ERR)
		printf("\ncan't catch SIGPIPE\n");
	#endif
	
	SPDY_init();
	
	/*
  struct sockaddr_in addr4;
	struct in_addr inaddr4;
	inaddr4.s_addr = htonl(INADDR_ANY);
	addr4.sin_family = AF_INET;
	addr4.sin_addr = inaddr4;
	addr4.sin_port = htons(atoi(argv[1]));
	*/
  
	struct SPDY_Daemon *daemon = SPDY_start_daemon(atoi(argv[1]),
	 DATA_DIR "cert-and-key.pem",
	 DATA_DIR "cert-and-key.pem",
	&new_session_callback,&session_closed_handler,&standard_request_handler,&new_post_data_cb,NULL,
	SPDY_DAEMON_OPTION_SESSION_TIMEOUT, 10,
	//SPDY_DAEMON_OPTION_SOCK_ADDR,  (struct sockaddr *)&addr4,
	SPDY_DAEMON_OPTION_END);
	
	if(NULL==daemon){
		printf("no daemon\n");
		return 1;
	}
	
  /*
	struct sockaddr_in6 addr6;
	addr6.sin6_family = AF_INET6;
	addr6.sin6_addr = in6addr_any;
	addr6.sin6_port = htons(atoi(argv[1]) + 1);
	*/
  
	struct SPDY_Daemon *daemon2 = SPDY_start_daemon(atoi(argv[1]) + 1,
	 DATA_DIR "cert-and-key.pem",
	 DATA_DIR "cert-and-key.pem",
	&new_session_callback,NULL,&standard_request_handler,&new_post_data_cb,&main,
	//SPDY_DAEMON_OPTION_SESSION_TIMEOUT, 0,
	//SPDY_DAEMON_OPTION_SOCK_ADDR,  (struct sockaddr *)&addr6,
	//SPDY_DAEMON_OPTION_FLAGS, SPDY_DAEMON_FLAG_ONLY_IPV6,
	SPDY_DAEMON_OPTION_END);
	
	if(NULL==daemon2){
		printf("no daemon\n");
		return 1;
	}
	
	do
	{
	unsigned long long timeoutlong=0;
	struct timeval timeout;
	volatile int rc; /* select() return code */ 
	volatile int ret;

	fd_set read_fd_set;
	fd_set write_fd_set;
	fd_set except_fd_set;
	int maxfd = -1;

	if(run && daemon != NULL)
	{
		loops++;
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
    
		printf("ret=%i; timeoutlong=%llu; sec=%llu; usec=%llu\n", ret, timeoutlong, (long long unsigned)timeout.tv_sec, (long long unsigned)timeout.tv_usec);
		//raise(SIGINT);

		/* get file descriptors from the transfers */ 
		maxfd = SPDY_get_fdset (daemon,
		&read_fd_set,
		&write_fd_set, 
		&except_fd_set);

//struct timeval ts1,ts2;
    //gettimeofday(&ts1, NULL);
		rc = select(maxfd+1, &read_fd_set, &write_fd_set, &except_fd_set, &timeout);
    //gettimeofday(&ts2, NULL);
    printf("rc %i\n",rc);
   // printf("time for select %i\n",ts2.tv_usec - ts1.tv_usec);
   // printf("%i %i %i %i\n",ts1.tv_sec, ts1.tv_usec,ts2.tv_sec, ts2.tv_usec);

		switch(rc) {
			case -1:
				/* select error */ 
				break;
			case 0:

				break;
			default:
				SPDY_run(daemon);

			break;
		}
	}
	else if(daemon != NULL){
	
	printf("%lu loops in %llu secs\n", loops, (long long unsigned)(time(NULL) - start));
		SPDY_stop_daemon(daemon);
		daemon=NULL;
	}

	if(run2)
	{
		FD_ZERO(&read_fd_set);
		FD_ZERO(&write_fd_set);
		FD_ZERO(&except_fd_set);

		ret = SPDY_get_timeout(daemon2, &timeoutlong);
		//printf("tout %i\n",timeoutlong);
		if(SPDY_NO == ret || timeoutlong > 1)
		{ 
			//do sth else
			//sleep(1);

			//try new connection
			timeout.tv_sec = 1;
			timeout.tv_usec = 0;
		}
		else
		{
			timeout.tv_sec = timeoutlong;
			timeout.tv_usec = 0;//(timeoutlong % 1000) * 1000;
		}

		//printf("ret=%i; timeoutlong=%i; sec=%i; usec=%i\n", ret, timeoutlong, timeout.tv_sec, timeout.tv_usec);
		//raise(SIGINT);

		/* get file descriptors from the transfers */ 
		maxfd = SPDY_get_fdset (daemon2,
		&read_fd_set,
		&write_fd_set, 
		&except_fd_set);

		rc = select(maxfd+1, &read_fd_set, &write_fd_set, &except_fd_set, &timeout);

		switch(rc) {
			case -1:
				/* select error */ 
				break;
			case 0:

				break;
			default:
				SPDY_run(daemon2);

				break;
		}
	}
	else if(daemon2 != NULL){
		SPDY_stop_daemon(daemon2);
		daemon2=NULL;
	}
	}
	while(run || run2);

	if(daemon != NULL){
		SPDY_stop_daemon(daemon);
	}
	if(daemon2 != NULL){
		SPDY_stop_daemon(daemon2);
	}
	
	SPDY_deinit();
	
	return 0;
}

