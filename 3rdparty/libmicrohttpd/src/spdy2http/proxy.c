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
 * @file proxy.c
 * @brief   Translates incoming SPDY requests to http server on localhost.
 * 			Uses libcurl.
 * 			No error handling for curl requests.
 *      TODO:
 * - test all options!
 * - don't abort on lack of memory
 * - Correct recapitalizetion of header names before giving the headers
 * to curl.
 * - curl does not close sockets when connection is closed and no
 * new sockets are opened (they stay in CLOSE_WAIT)
 * - add '/' when a user requests http://example.com . Now this is a bad
 * request
 * - curl returns 0 or 1 ms for timeout even when nothing will be done;
 * thus the loop uses CPU for nothing
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
#include <curl/curl.h>
#include <assert.h>
#include <getopt.h>
#include <regex.h>

#define ERROR_RESPONSE "502 Bad Gateway"


struct global_options
{
  char *http_backend;
  char *cert;
  char *cert_key;
  char *listen_host;
  unsigned int timeout;
  uint16_t listen_port;
  bool verbose;
  bool curl_verbose;
  bool transparent;
  bool http10;
  bool notls;
  bool nodelay;
  bool ipv4;
  bool ipv6;
} glob_opt;


struct URI
{
  char * full_uri;
  char * scheme;
  char * host_and_port;
  //char * host_and_port_for_connecting;
  char * host;
  char * path;
  char * path_and_more;
  char * query;
  char * fragment;
  uint16_t port;
};


#define PRINT_INFO(msg) do{\
	fprintf(stdout, "%i:%s\n", __LINE__, msg);\
	fflush(stdout);\
	}\
	while(0)


#define PRINT_INFO2(fmt, ...) do{\
	fprintf(stdout, "%i\n", __LINE__);\
	fprintf(stdout, fmt,##__VA_ARGS__);\
	fprintf(stdout, "\n");\
	fflush(stdout);\
	}\
	while(0)


#define PRINT_VERBOSE(msg) do{\
  if(glob_opt.verbose){\
	fprintf(stdout, "%i:%s\n", __LINE__, msg);\
	fflush(stdout);\
	}\
  }\
	while(0)


#define PRINT_VERBOSE2(fmt, ...) do{\
  if(glob_opt.verbose){\
	fprintf(stdout, "%i\n", __LINE__);\
	fprintf(stdout, fmt,##__VA_ARGS__);\
	fprintf(stdout, "\n");\
	fflush(stdout);\
	}\
	}\
	while(0)


#define CURL_SETOPT(handle, opt, val) do{\
	int ret; \
	if(CURLE_OK != (ret = curl_easy_setopt(handle, opt, val))) \
	{ \
		PRINT_INFO2("curl_easy_setopt failed (%i = %i)", opt, ret); \
		abort(); \
	} \
	}\
	while(0)


#define DIE(msg) do{\
	printf("FATAL ERROR (line %i): %s\n", __LINE__, msg);\
	fflush(stdout);\
  exit(EXIT_FAILURE);\
	}\
	while(0)


static int loop = 1;

static CURLM *multi_handle;

static int still_running = 0; /* keep number of running handles */

static regex_t uri_preg;

static bool call_spdy_run;
static bool call_curl_run;

int debug_num_curls;


struct Proxy
{
	char *url;
	struct SPDY_Request *request;
	struct SPDY_Response *response;
	CURL *curl_handle;
	struct curl_slist *curl_headers;
	struct SPDY_NameValue *headers;
	char *version;
	char *status_msg;
	void *http_body;
	void *received_body;
  bool *session_alive;
	size_t http_body_size;
	size_t received_body_size;
	//ssize_t length;
	int status;
  //bool done;
  bool receiving_done;
  bool is_curl_read_paused;
  bool is_with_body_data;
  //bool error;
  bool curl_done;
  bool curl_error;
  bool spdy_done;
  bool spdy_error;
};


static void
free_uri(struct URI * uri)
{
  if(NULL != uri)
  {
    free(uri->full_uri);
    free(uri->scheme);
    free(uri->host_and_port);
    //free(uri->host_and_port_for_connecting);
    free(uri->host);
    free(uri->path);
    free(uri->path_and_more);
    free(uri->query);
    free(uri->fragment);
    uri->port = 0;
    free(uri);
  }
}


static int
init_parse_uri(regex_t * preg)
{
  // RFC 2396
  // ^(([^:/?#]+):)?(//([^/?#]*))?([^?#]*)(\?([^#]*))?(#(.*))?
      /*
        scheme    = $2
      authority = $4
      path      = $5
      query     = $7
      fragment  = $9
      */

  return regcomp(preg, "^(([^:/?#]+):)?(//([^/?#]*))?([^?#]*)(\\?([^#]*))?(#(.*))?", REG_EXTENDED);
}


static void
deinit_parse_uri(regex_t * preg)
{
  regfree(preg);
}


static int
parse_uri(regex_t * preg, const char * full_uri, struct URI ** uri)
{
  //TODO memeory checks
  int ret;
  char *colon;
  long long port;
  size_t nmatch = 10;
  regmatch_t pmatch[10];

  if (0 != (ret = regexec(preg, full_uri, nmatch, pmatch, 0)))
    return ret;

  *uri = malloc(sizeof(struct URI));
  if(NULL == *uri)
    return -200;

  (*uri)->full_uri = strdup(full_uri);

  asprintf(&((*uri)->scheme),
	   "%.*s",
	   (int) (pmatch[2].rm_eo - pmatch[2].rm_so),
	   &full_uri[pmatch[2].rm_so]);
  asprintf(&((*uri)->host_and_port), "%.*s",
	   (int) (pmatch[4].rm_eo - pmatch[4].rm_so),
	   &full_uri[pmatch[4].rm_so]);
  asprintf(&((*uri)->path),
	   "%.*s",
	   (int) (pmatch[5].rm_eo - pmatch[5].rm_so),
	   &full_uri[pmatch[5].rm_so]);
  asprintf(&((*uri)->path_and_more),
	   "%.*s",
	   (int) (pmatch[9].rm_eo - pmatch[5].rm_so),
	   &full_uri[pmatch[5].rm_so]);
  asprintf(&((*uri)->query),
	   "%.*s",
	   (int) (pmatch[7].rm_eo - pmatch[7].rm_so),
	   &full_uri[pmatch[7].rm_so]);
  asprintf(&((*uri)->fragment),
	   "%.*s",
	   (int) (pmatch[9].rm_eo - pmatch[9].rm_so),
	   &full_uri[pmatch[9].rm_so]);

  colon = strrchr((*uri)->host_and_port, ':');
  if(NULL == colon)
  {
    (*uri)->host = strdup((*uri)->host_and_port);
    /*if(0 == strcasecmp("http", uri->scheme))
    {
      uri->port = 80;
      asprintf(&(uri->host_and_port_for_connecting), "%s:80", uri->host_and_port);
    }
    else if(0 == strcasecmp("https", uri->scheme))
    {
      uri->port = 443;
      asprintf(&(uri->host_and_port_for_connecting), "%s:443", uri->host_and_port);
    }
    else
    {
      PRINT_INFO("no standard scheme!");
      */(*uri)->port = 0;
      /*uri->host_and_port_for_connecting = strdup(uri->host_and_port);
    }*/
    return 0;
  }

  port = atoi(colon  + 1);
  if(port<1 || port >= 256 * 256)
  {
    free_uri(*uri);
    return -100;
  }
  (*uri)->port = port;
  asprintf(&((*uri)->host), "%.*s", (int)(colon - (*uri)->host_and_port), (*uri)->host_and_port);

  return 0;
}


static bool
store_in_buffer(const void *src, size_t src_size, void **dst, size_t *dst_size)
{
  if(0 == src_size)
    return true;

  if(NULL == *dst)
		*dst = malloc(src_size);
	else
		*dst = realloc(*dst, src_size + *dst_size);
	if(NULL == *dst)
		return false;

	memcpy(*dst + *dst_size, src, src_size);
	*dst_size += src_size;

  return true;
}


static ssize_t
get_from_buffer(void **src, size_t *src_size, void *dst, size_t max_size)
{
  size_t ret;
  void *newbody;

	if(max_size >= *src_size)
	{
		ret = *src_size;
		newbody = NULL;
	}
	else
	{
		ret = max_size;
		if(NULL == (newbody = malloc(*src_size - max_size)))
			return -1;
		memcpy(newbody, *src + ret, *src_size - ret);
	}
	memcpy(dst, *src, ret);
	free(*src);
	*src = newbody;
	*src_size -= ret;

  return ret;
}


static void
catch_signal(int signal)
{
  (void)signal;

  loop = 0;
}

static void
new_session_cb (void * cls,
							struct SPDY_Session * session)
{
  (void)cls;

  bool *session_alive;

  PRINT_VERBOSE("new session");
  //TODO clean this memory
  if(NULL == (session_alive = malloc(sizeof(bool))))
  {
			DIE("no memory");
  }
  *session_alive = true;
  SPDY_set_cls_to_session(session,
						session_alive);
}

static void
session_closed_cb (void * cls,
								struct SPDY_Session * session,
								int by_client)
{
  (void)cls;

  bool *session_alive;

  PRINT_VERBOSE2("session closed; by client: %i", by_client);

  session_alive = SPDY_get_cls_from_session(session);
  assert(NULL != session_alive);

  *session_alive = false;
}


static int
spdy_post_data_cb (void * cls,
					 struct SPDY_Request *request,
					 const void * buf,
					 size_t size,
					 bool more)
{
  (void)cls;
  int ret;
	struct Proxy *proxy = (struct Proxy *)SPDY_get_cls_from_request(request);

  if(!store_in_buffer(buf, size, &proxy->received_body, &proxy->received_body_size))
	{
		PRINT_INFO("not enough memory (malloc/realloc returned NULL)");
		return 0;
	}

  proxy->receiving_done = !more;

  PRINT_VERBOSE2("POST bytes from SPDY: %zu", size);

  call_curl_run = true;

  if(proxy->is_curl_read_paused)
  {
    if(CURLE_OK != (ret = curl_easy_pause(proxy->curl_handle, CURLPAUSE_CONT)))
    {
      PRINT_INFO2("curl_easy_pause returned %i", ret);
      abort();
    }
    PRINT_VERBOSE("curl_read_cb pause resumed");
  }

  return SPDY_YES;
}


ssize_t
response_callback (void *cls,
						void *buffer,
						size_t max,
						bool *more)
{
	ssize_t ret;
	struct Proxy *proxy = (struct Proxy *)cls;

  *more = true;

  assert(!proxy->spdy_error);

  if(proxy->curl_error)
  {
    PRINT_VERBOSE("tell spdy about the error");
    return -1;
  }

	if(!proxy->http_body_size)//nothing to write now
  {
    PRINT_VERBOSE("nothing to write now");
    if(proxy->curl_done || proxy->curl_error) *more = false;
		return 0;
  }

  ret = get_from_buffer(&(proxy->http_body), &(proxy->http_body_size), buffer, max);
  if(ret < 0)
  {
    PRINT_INFO("no memory");
    //TODO error?
    return -1;
  }

  if((proxy->curl_done || proxy->curl_error) && 0 == proxy->http_body_size) *more = false;

  PRINT_VERBOSE2("given bytes to microspdy: %zd", ret);

	return ret;
}


static void
cleanup(struct Proxy *proxy)
{
  int ret;

  //fprintf(stderr, "free proxy for %s\n", proxy->url);

	if(CURLM_OK != (ret = curl_multi_remove_handle(multi_handle, proxy->curl_handle)))
	{
		PRINT_INFO2("curl_multi_remove_handle failed (%i)", ret);
    DIE("bug in cleanup");
	}
  debug_num_curls--;
  //TODO bug on ku6.com or amazon.cn
  // after curl_multi_remove_handle returned CURLM_BAD_EASY_HANDLE
	curl_slist_free_all(proxy->curl_headers);
	curl_easy_cleanup(proxy->curl_handle);

	free(proxy->url);
	free(proxy);
}


static void
response_done_callback(void *cls,
						struct SPDY_Response *response,
						struct SPDY_Request *request,
						enum SPDY_RESPONSE_RESULT status,
						bool streamopened)
{
	(void)streamopened;
	struct Proxy *proxy = (struct Proxy *)cls;

	if(SPDY_RESPONSE_RESULT_SUCCESS != status)
	{
    free(proxy->http_body);
    proxy->http_body = NULL;
    proxy->spdy_error = true;
	}
	cleanup(proxy);
	SPDY_destroy_request(request);
	SPDY_destroy_response(response);
}


static size_t
curl_header_cb(void *ptr, size_t size, size_t nmemb, void *userp)
{
	size_t realsize = size * nmemb;
	struct Proxy *proxy = (struct Proxy *)userp;
	char *line = (char *)ptr;
	char *name;
	char *value;
	char *status;
	unsigned int i;
	unsigned int pos;
	int ret;
  int num_values;
  const char * const * values;
  bool abort_it;

	//printf("curl_header_cb %s\n", line);
  if(!*(proxy->session_alive))
  {
    PRINT_VERBOSE("headers received, but session is dead");
    proxy->spdy_error = true;
    proxy->curl_error = true;
    return 0;
  }

  //trailer
  if(NULL != proxy->response) return 0;

	if('\r' == line[0] || '\n' == line[0])
	{
		//all headers were already handled; prepare spdy frames
		if(NULL == (proxy->response = SPDY_build_response_with_callback(proxy->status,
							proxy->status_msg,
							proxy->version,
							proxy->headers,
							&response_callback,
							proxy,
							0)))
							//256)))
			DIE("no response");

		SPDY_name_value_destroy(proxy->headers);
    proxy->headers = NULL;
		free(proxy->status_msg);
    proxy->status_msg = NULL;
		free(proxy->version);
    proxy->version = NULL;

		if(SPDY_YES != SPDY_queue_response(proxy->request,
							proxy->response,
							true,
							false,
							&response_done_callback,
							proxy))
    {
			//DIE("no queue");
      //TODO right?
      proxy->spdy_error = true;
      proxy->curl_error = true;
      PRINT_VERBOSE2("no queue in curl_header_cb for %s", proxy->url);
      SPDY_destroy_response(proxy->response);
      proxy->response = NULL;
      return 0;
    }

    call_spdy_run = true;

		return realsize;
	}

	pos = 0;
	if(NULL == proxy->version)
	{
		//first line from headers
		//version
		for(i=pos; i<realsize && ' '!=line[i]; ++i);
		if(i == realsize)
			DIE("error on parsing headers");
		if(NULL == (proxy->version = strndup(line, i - pos)))
        DIE("No memory");
		pos = i+1;

		//status (number)
		for(i=pos; i<realsize && ' '!=line[i] && '\r'!=line[i] && '\n'!=line[i]; ++i);
		if(NULL == (status = strndup(&(line[pos]), i - pos)))
        DIE("No memory");
		proxy->status = atoi(status);
		free(status);
		if(i<realsize && '\r'!=line[i] && '\n'!=line[i])
		{
			//status (message)
			pos = i+1;
			for(i=pos; i<realsize && '\r'!=line[i] && '\n'!=line[i]; ++i);
			if(NULL == (proxy->status_msg = strndup(&(line[pos]), i - pos)))
        DIE("No memory");
		}
    PRINT_VERBOSE2("Header line received '%s' '%i' '%s' ", proxy->version, proxy->status, proxy->status_msg);
		return realsize;
	}

	//other lines
	//header name
	for(i=pos; i<realsize && ':'!=line[i] && '\r'!=line[i] && '\n'!=line[i]; ++i)
		line[i] = tolower(line[i]); //spdy requires lower case
	if(NULL == (name = strndup(line, i - pos)))
        DIE("No memory");
	if(0 == strcmp(SPDY_HTTP_HEADER_CONNECTION, name)
		|| 0 == strcmp(SPDY_HTTP_HEADER_KEEP_ALIVE, name)
		|| 0 == strcmp(SPDY_HTTP_HEADER_TRANSFER_ENCODING, name)
    )
	{
		//forbidden in spdy, ignore
		free(name);
		return realsize;
	}
	if(i == realsize || '\r'==line[i] || '\n'==line[i])
	{
		//no value. is it possible?
		if(SPDY_YES != SPDY_name_value_add(proxy->headers, name, ""))
			DIE("SPDY_name_value_add failed");
		return realsize;
	}

	//header value
	pos = i+1;
	while(pos<realsize && isspace(line[pos])) ++pos; //remove leading space
	for(i=pos; i<realsize && '\r'!=line[i] && '\n'!=line[i]; ++i);
	if(NULL == (value = strndup(&(line[pos]), i - pos)))
        DIE("No memory");
  PRINT_VERBOSE2("Adding header: '%s': '%s'", name, value);
	if(SPDY_YES != (ret = SPDY_name_value_add(proxy->headers, name, value)))
	{
    abort_it=true;
    if(NULL != (values = SPDY_name_value_lookup(proxy->headers, name, &num_values)))
      for(i=0; i<(unsigned int)num_values; ++i)
        if(0 == strcasecmp(value, values[i]))
        {
          abort_it=false;
          PRINT_VERBOSE2("header appears more than once with same value '%s: %s'", name, value);
          break;
        }

    if(abort_it)
    {
      PRINT_INFO2("SPDY_name_value_add failed (%i) for '%s'", ret, name);
      abort();
    }
	}
	free(name);
	free(value);

	return realsize;
}


static size_t
curl_write_cb(void *contents, size_t size, size_t nmemb, void *userp)
{
	size_t realsize = size * nmemb;
	struct Proxy *proxy = (struct Proxy *)userp;

	//printf("curl_write_cb %i\n", realsize);
  if(!*(proxy->session_alive))
  {
    PRINT_VERBOSE("data received, but session is dead");
      proxy->spdy_error = true;
      proxy->curl_error = true;
    return 0;
  }

  if(!store_in_buffer(contents, realsize, &proxy->http_body, &proxy->http_body_size))
	{
		PRINT_INFO("not enough memory (malloc/realloc returned NULL)");
      proxy->curl_error = true;
		return 0;
	}
  /*
	if(NULL == proxy->http_body)
		proxy->http_body = malloc(realsize);
	else
		proxy->http_body = realloc(proxy->http_body, proxy->http_body_size + realsize);
	if(NULL == proxy->http_body)
	{
		PRINT_INFO("not enough memory (realloc returned NULL)");
		return 0;
	}

	memcpy(proxy->http_body + proxy->http_body_size, contents, realsize);
	proxy->http_body_size += realsize;
  */

  PRINT_VERBOSE2("received bytes from curl: %zu", realsize);

  call_spdy_run = true;

	return realsize;
}


static size_t
curl_read_cb(void *ptr, size_t size, size_t nmemb, void *userp)
{
	ssize_t ret;
	size_t max = size * nmemb;
	struct Proxy *proxy = (struct Proxy *)userp;
	//void *newbody;


  if((proxy->receiving_done && !proxy->received_body_size) || !proxy->is_with_body_data || max < 1)
  {
    PRINT_VERBOSE("curl_read_cb last call");
    return 0;
  }

  if(!*(proxy->session_alive))
  {
    PRINT_VERBOSE("POST is still being sent, but session is dead");
    return CURL_READFUNC_ABORT;
  }

	if(!proxy->received_body_size)//nothing to write now
  {
    PRINT_VERBOSE("curl_read_cb called paused");
    proxy->is_curl_read_paused = true;
		return CURL_READFUNC_PAUSE;//TODO curl pause should be used
  }

  ret = get_from_buffer(&(proxy->received_body), &(proxy->received_body_size), ptr, max);
  if(ret < 0)
  {
    PRINT_INFO("no memory");
    return CURL_READFUNC_ABORT;
  }

	/*
	if(max >= proxy->received_body_size)
	{
		ret = proxy->received_body_size;
		newbody = NULL;
	}
	else
	{
		ret = max;
		if(NULL == (newbody = malloc(proxy->received_body_size - max)))
		{
			PRINT_INFO("no memory");
			return CURL_READFUNC_ABORT;
		}
		memcpy(newbody, proxy->received_body + max, proxy->received_body_size - max);
	}
	memcpy(ptr, proxy->received_body, ret);
	free(proxy->received_body);
	proxy->received_body = newbody;
	proxy->received_body_size -= ret;
  * */

  PRINT_VERBOSE2("given POST bytes to curl: %zd", ret);

	return ret;
}


static int
iterate_cb (void *cls, const char *name, const char * const * value, int num_values)
{
	struct Proxy *proxy = (struct Proxy *)cls;
  struct curl_slist **curl_headers = (&(proxy->curl_headers));
  char *line;
  int line_len = strlen(name) + 3; //+ ": \0"
  int i;

  for(i=0; i<num_values; ++i)
  {
		if(i) line_len += 2; //", "
		line_len += strlen(value[i]);
	}

	if(NULL == (line = malloc(line_len)))//no recovery
    DIE("No memory");
	line[0] = 0;

  strcat(line, name);
  strcat(line, ": ");
  //all spdy header names are lower case;
  //for simplicity here we just capitalize the first letter
  line[0] = toupper(line[0]);

	for(i=0; i<num_values; ++i)
	{
		if(i) strcat(line, ", ");
  PRINT_VERBOSE2("Adding request header: '%s' len %ld", value[i], strlen(value[i]));
		strcat(line, value[i]);
	}
  PRINT_VERBOSE2("Adding request header: '%s'", line);
  if(NULL == (*curl_headers = curl_slist_append(*curl_headers, line)))
		DIE("curl_slist_append failed");
	free(line);

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
	(void)cls;
	(void)priority;
	(void)host;
	(void)scheme;

	struct Proxy *proxy;
	int ret;
  struct URI *uri;
  struct SPDY_Session *session;

  proxy = SPDY_get_cls_from_request(request);
  if(NULL != proxy)
  {
    //ignore trailers or more headers
    return;
  }

	PRINT_VERBOSE2("received request for '%s %s %s'\n", method, path, version);

  //TODO not freed once in a while
	if(NULL == (proxy = malloc(sizeof(struct Proxy))))
    DIE("No memory");
	memset(proxy, 0, sizeof(struct Proxy));

  //fprintf(stderr, "new  proxy for %s\n", path);

  session = SPDY_get_session_for_request(request);
  assert(NULL != session);
  proxy->session_alive = SPDY_get_cls_from_session(session);
  assert(NULL != proxy->session_alive);

  SPDY_set_cls_to_request(request, proxy);

	proxy->request = request;
	proxy->is_with_body_data = more;
	if(NULL == (proxy->headers = SPDY_name_value_create()))
        DIE("No memory");

  if(glob_opt.transparent)
  {
    if(NULL != glob_opt.http_backend) //use always same host
      ret = asprintf(&(proxy->url),"%s://%s%s", scheme, glob_opt.http_backend, path);
    else //use host header
      ret = asprintf(&(proxy->url),"%s://%s%s", scheme, host, path);
    if(-1 == ret)
        DIE("No memory");

    ret = parse_uri(&uri_preg, proxy->url, &uri);
    if(ret != 0)
      DIE("parsing built uri failed");
  }
  else
  {
    ret = parse_uri(&uri_preg, path, &uri);
    PRINT_INFO2("path %s '%s' '%s'", path, uri->scheme, uri->host);
    if(ret != 0 || !strlen(uri->scheme) || !strlen(uri->host))
      DIE("parsing received uri failed");

    if(NULL != glob_opt.http_backend) //use backend host
    {
      ret = asprintf(&(proxy->url),"%s://%s%s", uri->scheme, glob_opt.http_backend, uri->path_and_more);
      if(-1 == ret)
        DIE("No memory");
    }
    else //use request path
      if(NULL == (proxy->url = strdup(path)))
        DIE("No memory");
  }

  free_uri(uri);

  PRINT_VERBOSE2("curl will request '%s'", proxy->url);

  SPDY_name_value_iterate(headers, &iterate_cb, proxy);

	if(NULL == (proxy->curl_handle = curl_easy_init()))
    {
		PRINT_INFO("curl_easy_init failed");
		abort();
	}

	if(glob_opt.curl_verbose)
    CURL_SETOPT(proxy->curl_handle, CURLOPT_VERBOSE, 1);

  if(0 == strcmp(SPDY_HTTP_METHOD_POST,method))
  {
    if(NULL == (proxy->curl_headers = curl_slist_append(proxy->curl_headers, "Expect:")))
      DIE("curl_slist_append failed");
    CURL_SETOPT(proxy->curl_handle, CURLOPT_POST, 1);
    CURL_SETOPT(proxy->curl_handle, CURLOPT_READFUNCTION, curl_read_cb);
    CURL_SETOPT(proxy->curl_handle, CURLOPT_READDATA, proxy);
  }

  if(glob_opt.timeout)
    CURL_SETOPT(proxy->curl_handle, CURLOPT_TIMEOUT, glob_opt.timeout);
	CURL_SETOPT(proxy->curl_handle, CURLOPT_URL, proxy->url);
	if(glob_opt.http10)
		CURL_SETOPT(proxy->curl_handle, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_0);
	CURL_SETOPT(proxy->curl_handle, CURLOPT_WRITEFUNCTION, curl_write_cb);
	CURL_SETOPT(proxy->curl_handle, CURLOPT_WRITEDATA, proxy);
	CURL_SETOPT(proxy->curl_handle, CURLOPT_HEADERFUNCTION, curl_header_cb);
	CURL_SETOPT(proxy->curl_handle, CURLOPT_HEADERDATA, proxy);
	CURL_SETOPT(proxy->curl_handle, CURLOPT_PRIVATE, proxy);
	CURL_SETOPT(proxy->curl_handle, CURLOPT_HTTPHEADER, proxy->curl_headers);
  CURL_SETOPT(proxy->curl_handle, CURLOPT_SSL_VERIFYPEER, 0L);//TODO
  CURL_SETOPT(proxy->curl_handle, CURLOPT_SSL_VERIFYHOST, 0L);
  if(glob_opt.ipv4 && !glob_opt.ipv6)
    CURL_SETOPT(proxy->curl_handle, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);
  else if(glob_opt.ipv6 && !glob_opt.ipv4)
    CURL_SETOPT(proxy->curl_handle, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V6);

	if(CURLM_OK != (ret = curl_multi_add_handle(multi_handle, proxy->curl_handle)))
	{
		PRINT_INFO2("curl_multi_add_handle failed (%i)", ret);
		abort();
	}
  debug_num_curls++;

  //~5ms additional latency for calling this
	if(CURLM_OK != (ret = curl_multi_perform(multi_handle, &still_running))
		&& CURLM_CALL_MULTI_PERFORM != ret)
	{
		PRINT_INFO2("curl_multi_perform failed (%i)", ret);
		abort();
	}

  call_curl_run = true;
}


static int
run ()
{
  unsigned long long timeoutlong = 0;
  unsigned long long timeout_spdy = 0;
  long timeout_curl = -1;
	struct timeval timeout;
	int ret;
	int ret_curl;
	int ret_spdy;
	fd_set rs;
	fd_set ws;
	fd_set es;
	int maxfd = -1;
	int maxfd_curl = -1;
	struct SPDY_Daemon *daemon;
  CURLMsg *msg;
  int msgs_left;
  struct Proxy *proxy;
  struct sockaddr_in *addr;
  struct addrinfo hints;
  char service[NI_MAXSERV];
  struct addrinfo *gai;
  enum SPDY_IO_SUBSYSTEM io = glob_opt.notls ? SPDY_IO_SUBSYSTEM_RAW : SPDY_IO_SUBSYSTEM_OPENSSL;
  enum SPDY_DAEMON_FLAG flags = SPDY_DAEMON_FLAG_NO;
  //struct SPDY_Response *error_response;
  char *curl_private;

	signal(SIGPIPE, SIG_IGN);

  if (signal(SIGINT, catch_signal) == SIG_ERR)
    PRINT_VERBOSE("signal failed");

  srand(time(NULL));
  if(init_parse_uri(&uri_preg))
    DIE("Regexp compilation failed");

	SPDY_init();

  if(glob_opt.nodelay)
    flags |= SPDY_DAEMON_FLAG_NO_DELAY;

  if(NULL == glob_opt.listen_host)
	{
    daemon = SPDY_start_daemon(glob_opt.listen_port,
								glob_opt.cert,
								glob_opt.cert_key,
								&new_session_cb,
								&session_closed_cb,
								&standard_request_handler,
								&spdy_post_data_cb,
								NULL,
								SPDY_DAEMON_OPTION_SESSION_TIMEOUT,
								1800,
                SPDY_DAEMON_OPTION_IO_SUBSYSTEM,
                io,
                SPDY_DAEMON_OPTION_FLAGS,
                flags,
								SPDY_DAEMON_OPTION_END);
  }
  else
  {
    snprintf (service, sizeof(service), "%u", glob_opt.listen_port);
    memset (&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    ret = getaddrinfo(glob_opt.listen_host, service, &hints, &gai);
    if(ret != 0)
      DIE("problem with specified host");

    addr = (struct sockaddr_in *) gai->ai_addr;

    daemon = SPDY_start_daemon(0,
								glob_opt.cert,
								glob_opt.cert_key,
								&new_session_cb,
								&session_closed_cb,
								&standard_request_handler,
								&spdy_post_data_cb,
								NULL,
								SPDY_DAEMON_OPTION_SESSION_TIMEOUT,
								1800,
                SPDY_DAEMON_OPTION_IO_SUBSYSTEM,
                io,
                SPDY_DAEMON_OPTION_FLAGS,
                flags,
                SPDY_DAEMON_OPTION_SOCK_ADDR,
                addr,
                //SPDY_DAEMON_OPTION_MAX_NUM_FRAMES,
                //1,
								SPDY_DAEMON_OPTION_END);
  }

	if(NULL==daemon){
		printf("no daemon\n");
		return 1;
	}

	multi_handle = curl_multi_init();
	if(NULL==multi_handle)
		DIE("no multi_handle");

	timeout.tv_usec = 0;

	do
	{
		FD_ZERO(&rs);
		FD_ZERO(&ws);
		FD_ZERO(&es);

    PRINT_VERBOSE2("num  curls %i", debug_num_curls);

    ret_spdy = SPDY_get_timeout(daemon, &timeout_spdy);
    if(SPDY_NO == ret_spdy || timeout_spdy > 5000)
      timeoutlong = 5000;
    else
      timeoutlong = timeout_spdy;
    PRINT_VERBOSE2("SPDY timeout %lld; %i", timeout_spdy, ret_spdy);

    if(CURLM_OK != (ret_curl = curl_multi_timeout(multi_handle, &timeout_curl)))
    {
      PRINT_VERBOSE2("curl_multi_timeout failed (%i)", ret_curl);
      //curl_timeo = timeoutlong;
    }
    else if(timeout_curl >= 0 && timeoutlong > (unsigned long)timeout_curl)
      timeoutlong = (unsigned long)timeout_curl;

    PRINT_VERBOSE2("curl timeout %ld", timeout_curl);

    timeout.tv_sec = timeoutlong / 1000;
		timeout.tv_usec = (timeoutlong % 1000) * 1000;

		maxfd = SPDY_get_fdset (daemon,
								&rs,
								&ws,
								&es);
		assert(-1 != maxfd);

		if(CURLM_OK != (ret = curl_multi_fdset(multi_handle, &rs,
								&ws,
								&es, &maxfd_curl)))
		{
			PRINT_INFO2("curl_multi_fdset failed (%i)", ret);
			abort();
		}

    if(maxfd_curl > maxfd)
      maxfd = maxfd_curl;

    PRINT_VERBOSE2("timeout before %lld %lld", (unsigned long long)timeout.tv_sec, (unsigned long long)timeout.tv_usec);
    ret = select(maxfd+1, &rs, &ws, &es, &timeout);
    PRINT_VERBOSE2("timeout after %lld %lld; ret is %i", (unsigned long long)timeout.tv_sec, (unsigned long long)timeout.tv_usec, ret);

		/*switch(ret) {
			case -1:
				PRINT_INFO2("select error: %i", errno);
				break;
			case 0:
				break;
			default:*/

      //the second part should not happen with current implementation
      if(ret > 0 || (SPDY_YES == ret_spdy && 0 == timeout_spdy))
      {
				PRINT_VERBOSE("run spdy");
				SPDY_run(daemon);
        call_spdy_run = false;
      }

      //if(ret > 0 || (CURLM_OK == ret_curl && 0 == timeout_curl) || call_curl_run)
      {
				PRINT_VERBOSE("run curl");
				if(CURLM_OK != (ret = curl_multi_perform(multi_handle, &still_running))
					&& CURLM_CALL_MULTI_PERFORM != ret)
				{
					PRINT_INFO2("curl_multi_perform failed (%i)", ret);
					abort();
				}
        call_curl_run = false;
      }
			/*break;
		}*/

    while ((msg = curl_multi_info_read(multi_handle, &msgs_left))) {
      if (msg->msg == CURLMSG_DONE) {
        PRINT_VERBOSE("A curl handler is done");
        if(CURLE_OK != (ret = curl_easy_getinfo(msg->easy_handle, CURLINFO_PRIVATE, &curl_private)))
        {
          PRINT_INFO2("err %i",ret);
          abort();
        }
        assert(NULL != curl_private);
        proxy = (struct Proxy *)curl_private;
        if(CURLE_OK == msg->data.result)
        {
          proxy->curl_done = true;
          call_spdy_run = true;
          //TODO what happens with proxy when the client resets a stream
          //and response_done is not yet set for the last frame? is it
          //possible?
        }
        else
        {
          PRINT_VERBOSE2("bad curl result (%i) for '%s'", msg->data.result, proxy->url);
          if(proxy->spdy_done || proxy->spdy_error || (NULL == proxy->response && !*(proxy->session_alive)))
          {
            PRINT_VERBOSE("cleaning");
            SPDY_name_value_destroy(proxy->headers);
            SPDY_destroy_request(proxy->request);
            SPDY_destroy_response(proxy->response);
            cleanup(proxy);
          }
          else if(NULL == proxy->response && *(proxy->session_alive))
          {
            //generate error for the client
            PRINT_VERBOSE("will send Bad Gateway");
            SPDY_name_value_destroy(proxy->headers);
            proxy->headers = NULL;
            if(NULL == (proxy->response = SPDY_build_response(SPDY_HTTP_BAD_GATEWAY,
                  NULL,
                  SPDY_HTTP_VERSION_1_1,
                  NULL,
                  ERROR_RESPONSE,
                  strlen(ERROR_RESPONSE))))
              DIE("no response");
            if(SPDY_YES != SPDY_queue_response(proxy->request,
                      proxy->response,
                      true,
                      false,
                      &response_done_callback,
                      proxy))
            {
              //clean and forget
              PRINT_VERBOSE("cleaning");
              SPDY_destroy_request(proxy->request);
              SPDY_destroy_response(proxy->response);
              cleanup(proxy);
            }
          }
          else
          {
            proxy->curl_error = true;
          }
          call_spdy_run = true;
          //TODO spdy should be notified to send RST_STREAM
        }
      }
      else PRINT_INFO("shouldn't happen");
    }

    if(call_spdy_run)
    {
      PRINT_VERBOSE("second call to SPDY_run");
      SPDY_run(daemon);
      call_spdy_run = false;
    }

    if(glob_opt.verbose)
    {

#ifdef HAVE_CLOCK_GETTIME
#ifdef CLOCK_MONOTONIC
    struct timespec ts;

    if (0 == clock_gettime(CLOCK_MONOTONIC, &ts))
    PRINT_VERBOSE2 ("time now %lld %lld",
		    (unsigned long long) ts.tv_sec,
		    (unsigned long long) ts.tv_nsec);
#endif
#endif
    }

  }
  while(loop);

	SPDY_stop_daemon(daemon);

	curl_multi_cleanup(multi_handle);

	SPDY_deinit();

  deinit_parse_uri(&uri_preg);

	return 0;
}


static void
display_usage()
{
  printf(
    "Usage: microspdy2http -p <PORT> [-c <CERTIFICATE>] [-k <CERT-KEY>]\n"
    "                      [-rvh0DtT] [-b <HTTP-SERVER>] [-l <HOST>]\n\n"
    "OPTIONS:\n"
    "    -p, --port            Listening port.\n"
    "    -l, --host            Listening host. If not set, will listen on [::]\n"
    "    -c, --certificate     Path to a certificate file. Requiered if\n"
    "                          --no-tls is not set.\n"
    "    -k, --certificate-key Path to a key file for the certificate.\n"
    "                          Requiered if --no-tls is not set.\n"
    "    -b, --backend-server  If set, the proxy will connect always to it.\n"
    "                          Otherwise the proxy will connect to the URL\n"
    "                          which is specified in the path or 'Host:'.\n"
    "    -v, --verbose         Print debug information.\n"
    "    -r, --no-tls          Do not use TLS. Client must use SPDY/3.\n"
    "    -h, --curl-verbose    Print debug information for curl.\n"
    "    -0, --http10          Prefer HTTP/1.0 connections to the next hop.\n"
    "    -D, --no-delay        This makes sense only if --no-tls is used.\n"
    "                          TCP_NODELAY will be used for all sessions' sockets.\n"
    "    -4, --curl-ipv4       Curl may use IPv4 to connect to the final destination.\n"
    "    -6, --curl-ipv6       Curl may use IPv6 to connect to the final destination.\n"
    "                          If neither --curl-ipv4 nor --curl-ipv6 is set,\n"
    "                          both will be used by default.\n"
    "    -T, --timeout         Maximum time in seconds for each HTTP transfer.\n"
    "                          Use 0 for no timeout; this is the default value.\n"
    "    -t, --transparent     If set, the proxy will fetch an URL which\n"
    "                          is based on 'Host:' header and requested path.\n"
    "                          Otherwise, full URL in the requested path is required.\n\n"

  );
}


int
main (int argc, char *const *argv)
{

  int getopt_ret;
  int option_index;
  struct option long_options[] = {
    {"port",  required_argument, 0, 'p'},
    {"certificate",  required_argument, 0, 'c'},
    {"certificate-key",  required_argument, 0, 'k'},
    {"backend-server",  required_argument, 0, 'b'},
    {"no-tls",  no_argument, 0, 'r'},
    {"verbose",  no_argument, 0, 'v'},
    {"curl-verbose",  no_argument, 0, 'h'},
    {"http10",  no_argument, 0, '0'},
    {"no-delay",  no_argument, 0, 'D'},
    {"transparent",  no_argument, 0, 't'},
    {"curl-ipv4",  no_argument, 0, '4'},
    {"curl-ipv6",  no_argument, 0, '6'},
    {"timeout",  required_argument, 0, 'T'},
    {0, 0, 0, 0}
  };

  while (1)
  {
    getopt_ret = getopt_long( argc, argv, "p:l:c:k:b:rv0Dth46T:", long_options, &option_index);
    if (getopt_ret == -1)
      break;

    switch(getopt_ret)
    {
      case 'p':
        glob_opt.listen_port = atoi(optarg);
        break;

      case 'l':
        glob_opt.listen_host= strdup(optarg);
        if(NULL == glob_opt.listen_host)
          return 1;
        break;

      case 'c':
        glob_opt.cert = strdup(optarg);
        break;

      case 'k':
        glob_opt.cert_key = strdup(optarg);
        break;

      case 'b':
        glob_opt.http_backend = strdup(optarg);
        if(NULL == glob_opt.http_backend)
          return 1;
        break;

      case 'r':
        glob_opt.notls = true;
        break;

      case 'v':
        glob_opt.verbose = true;
        break;

      case 'h':
        glob_opt.curl_verbose = true;
        break;

      case '0':
        glob_opt.http10 = true;
        break;

      case 'D':
        glob_opt.nodelay = true;
        break;

      case 't':
        glob_opt.transparent = true;
        break;

      case '4':
        glob_opt.ipv4 = true;
        break;

      case '6':
        glob_opt.ipv6 = true;
        break;

      case 'T':
        glob_opt.timeout = atoi(optarg);
        break;

      case 0:
        PRINT_INFO("0 from getopt");
        break;

      case '?':
        display_usage();
        return 1;

      default:
        DIE("default from getopt");
    }
  }

  if(
    0 == glob_opt.listen_port
    || (!glob_opt.notls && (NULL == glob_opt.cert || NULL == glob_opt.cert_key))
    //|| !glob_opt.transparent && NULL != glob_opt.http_backend
    )
  {
    display_usage();
    return 1;
  }

  return run();
}

