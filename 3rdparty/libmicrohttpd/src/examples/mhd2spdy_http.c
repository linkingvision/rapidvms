/*
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
 * @file mhd2spdy_http.c
 * @brief  HTTP part of the proxy. libmicrohttpd is used for the server side.
 * @author Andrey Uzunov
 */
 
#include "mhd2spdy_structures.h"
#include "mhd2spdy_http.h"
#include "mhd2spdy_spdy.h"


void *
http_cb_log(void * cls,
const char * uri)
{
  (void)cls;
  
  struct HTTP_URI * http_uri;
  
  PRINT_INFO2("log uri '%s'\n", uri);
  
  //TODO not freed once in a while
  if(NULL == (http_uri = au_malloc(sizeof(struct HTTP_URI ))))
    return NULL;
  http_uri->uri = strdup(uri);
  return http_uri;
}


static int
http_cb_iterate(void *cls,
                 enum MHD_ValueKind kind,
                 const char *name,
                 const char *value)
{
  (void)kind;
  
  static char * const forbidden[] = {"Transfer-Encoding",
    "Proxy-Connection",
    "Keep-Alive",
    "Connection"};
  static int forbidden_size = 4;
  int i;
	struct SPDY_Headers *spdy_headers = (struct SPDY_Headers *)cls;
	
	if(0 == strcasecmp(name, "Host"))
    spdy_headers->nv[9] = (char *)value;
  else
  {
    for(i=0; i<forbidden_size; ++i)
      if(0 == strcasecmp(forbidden[i], name))
        return MHD_YES;
    spdy_headers->nv[spdy_headers->cnt++] = (char *)name;
    spdy_headers->nv[spdy_headers->cnt++] = (char *)value;
  }
	
	return MHD_YES;
}


static ssize_t
http_cb_response (void *cls,
                        uint64_t pos,
                        char *buffer,
                        size_t max)
{
  (void)pos;
  
	int ret;
	struct Proxy *proxy = (struct Proxy *)cls;
	void *newbody;
  const union MHD_ConnectionInfo *info;
  int val = 1;
  
  PRINT_INFO2("http_cb_response for %s", proxy->url);
  
  if(proxy->spdy_error)
    return MHD_CONTENT_READER_END_WITH_ERROR;
  
	if(0 == proxy->http_body_size && (proxy->done || !proxy->spdy_active))
  {
    PRINT_INFO("sent end of stream");
    return MHD_CONTENT_READER_END_OF_STREAM;
  }
	
	if(!proxy->http_body_size)//nothing to write now
  {
    //flush data
    info = MHD_get_connection_info (proxy->http_connection,
         MHD_CONNECTION_INFO_CONNECTION_FD);
    ret = setsockopt(info->connect_fd, IPPROTO_TCP, TCP_NODELAY, &val, (socklen_t)sizeof(val));
    if(ret == -1) {
      DIE("setsockopt");
    }
    
    PRINT_INFO("FLUSH data");
		return 0;
  }
	
	if(max >= proxy->http_body_size)
	{
		ret = proxy->http_body_size;
		newbody = NULL;
	}
	else
	{
		ret = max;
		if(NULL == (newbody = au_malloc(proxy->http_body_size - max)))
		{
			PRINT_INFO("no memory");
			return MHD_CONTENT_READER_END_WITH_ERROR;
		}
		memcpy(newbody, proxy->http_body + max, proxy->http_body_size - max);
	}
	memcpy(buffer, proxy->http_body, ret);
	free(proxy->http_body);
	proxy->http_body = newbody;
	proxy->http_body_size -= ret;
	
	if(proxy->length >= 0)
	{
		proxy->length -= ret;
	}
	
	PRINT_INFO2("response_callback, size: %i",ret);
	
	return ret;
}


static void
http_cb_response_done(void *cls)
{
  (void)cls;
  //TODO remove
}

int
http_cb_request (void *cls,
                struct MHD_Connection *connection,
                const char *url,
                const char *method,
                const char *version,
                const char *upload_data,
                size_t *upload_data_size,
                void **ptr)
{
  (void)cls;
  (void)url;
  (void)upload_data;
  (void)upload_data_size;
  
  int ret;
  struct Proxy *proxy;
  struct SPDY_Headers spdy_headers;
  bool with_body = false;
  struct HTTP_URI *http_uri;
  const char *header_value;

  if (NULL == ptr || NULL == *ptr)
    return MHD_NO;
    
  http_uri = (struct HTTP_URI *)*ptr;
    
  if(NULL == http_uri->proxy)
  {
    //first call for this request
    if (0 != strcmp (method, MHD_HTTP_METHOD_GET) && 0 != strcmp (method, MHD_HTTP_METHOD_POST))
    {
      free(http_uri->uri);
      free(http_uri);
      PRINT_INFO2("unexpected method %s", method);
      return MHD_NO;
    }
    
    if(NULL == (proxy = au_malloc(sizeof(struct Proxy))))
    {
      free(http_uri->uri);
      free(http_uri);
      PRINT_INFO("No memory");
      return MHD_NO; 
    }
    
    ++glob_opt.responses_pending;
    proxy->id = rand();
    proxy->http_active = true;
    proxy->http_connection = connection;
    http_uri->proxy = proxy;
    return MHD_YES;
  }
  
  proxy = http_uri->proxy;
  
  if(proxy->spdy_error || proxy->http_error)
    return MHD_NO; // handled at different place TODO? leaks?

  if(proxy->spdy_active)
  {
    if(0 == strcmp (method, MHD_HTTP_METHOD_POST))
    {
      PRINT_INFO("POST processing");
        
      int rc= spdylay_session_resume_data(proxy->spdy_connection->session, proxy->stream_id);
      PRINT_INFO2("rc is %i stream is %i", rc, proxy->stream_id);
      proxy->spdy_connection->want_io |= WANT_WRITE;
      
      if(0 == *upload_data_size)
      {
      PRINT_INFO("POST http EOF");
        proxy->receiving_done = true;
        return MHD_YES;
      }
      
      if(!copy_buffer(upload_data, *upload_data_size, &proxy->received_body, &proxy->received_body_size))
      {
        //TODO handle it better?
        PRINT_INFO("not enough memory (malloc/realloc returned NULL)");
        return MHD_NO;
      }
      
      *upload_data_size = 0;
                               
      return MHD_YES;
    }
  
    //already handled
    PRINT_INFO("unnecessary call to http_cb_request");
    return MHD_YES;
  }
  
  //second call for this request

  PRINT_INFO2("received request for '%s %s %s'", method, http_uri->uri, version);

  proxy->url = http_uri->uri;
  
  header_value = MHD_lookup_connection_value(connection,
    MHD_HEADER_KIND, MHD_HTTP_HEADER_CONTENT_LENGTH);
  
  with_body = 0 == strcmp (method, MHD_HTTP_METHOD_POST)
    && (NULL == header_value || 0 != strcmp ("0", header_value));
    
  PRINT_INFO2("body will be sent %i", with_body);
    
  ret = parse_uri(&glob_opt.uri_preg, proxy->url, &proxy->uri);
  if(ret != 0)
    DIE("parse_uri failed");
  proxy->http_uri = http_uri;

  spdy_headers.num = MHD_get_connection_values (connection,
                       MHD_HEADER_KIND,
                       NULL,
                       NULL);
  if(NULL == (spdy_headers.nv = au_malloc(((spdy_headers.num + 5) * 2 + 1) * sizeof(char *))))
    DIE("no memory");
  spdy_headers.nv[0] = ":method";     spdy_headers.nv[1] = method;
  spdy_headers.nv[2] = ":path";       spdy_headers.nv[3] = proxy->uri->path_and_more;
  spdy_headers.nv[4] = ":version";    spdy_headers.nv[5] = (char *)version;
  spdy_headers.nv[6] = ":scheme";     spdy_headers.nv[7] = proxy->uri->scheme;
  spdy_headers.nv[8] = ":host";       spdy_headers.nv[9] = NULL;
  //nv[14] = NULL;
  spdy_headers.cnt = 10;
  MHD_get_connection_values (connection,
                       MHD_HEADER_KIND,
                       &http_cb_iterate,
                       &spdy_headers);
                       
  spdy_headers.nv[spdy_headers.cnt] = NULL;
  if(NULL == spdy_headers.nv[9])
    spdy_headers.nv[9] = proxy->uri->host_and_port;

  if(0 != spdy_request(spdy_headers.nv, proxy, with_body))
  {
    free(spdy_headers.nv);
    //free_proxy(proxy);
    
    return MHD_NO;
  }
  free(spdy_headers.nv);
  
  proxy->http_response = MHD_create_response_from_callback (MHD_SIZE_UNKNOWN,
                         4096,
                         &http_cb_response,
                         proxy,
                         &http_cb_response_done);

  if (NULL == proxy->http_response)
    DIE("no response");
  
  if(MHD_NO == MHD_add_response_header (proxy->http_response,
                 "Proxy-Connection", "keep-alive"))
    PRINT_INFO("SPDY_name_value_add failed: ");
  if(MHD_NO == MHD_add_response_header (proxy->http_response,
                 "Connection", "Keep-Alive"))
    PRINT_INFO("SPDY_name_value_add failed: ");
  if(MHD_NO == MHD_add_response_header (proxy->http_response,
                 "Keep-Alive", "timeout=5, max=100"))
    PRINT_INFO("SPDY_name_value_add failed: ");
    
  proxy->spdy_active = true;
  
  return MHD_YES;
}


void
http_create_response(struct Proxy* proxy,
                     char **nv)
{
  size_t i;
  
  if(!proxy->http_active)
    return;
  
  for(i = 0; nv[i]; i += 2) {
    if(0 == strcmp(":status", nv[i]))
    {
      char tmp[4];
      memcpy(&tmp,nv[i+1],3);
      tmp[3]=0;
      proxy->status = atoi(tmp);
      continue;
    }
    else if(0 == strcmp(":version", nv[i]))
    {
      proxy->version = nv[i+1];
      continue;
    }
    else if(0 == strcmp("content-length", nv[i]))
    {
      continue;
    }

    char *header = *(nv+i);
    if(MHD_NO == MHD_add_response_header (proxy->http_response,
                   header, nv[i+1]))
    {
      PRINT_INFO2("SPDY_name_value_add failed: '%s' '%s'", header, nv[i+1]);
    }
    PRINT_INFO2("adding '%s: %s'",header, nv[i+1]);
  }
  
  if(MHD_NO == MHD_queue_response (proxy->http_connection, proxy->status, proxy->http_response)){
    PRINT_INFO("No queue");
    //TODO
    //abort();
    proxy->http_error = true;
  }
  
  MHD_destroy_response (proxy->http_response);
  proxy->http_response = NULL;
}

void
http_cb_request_completed (void *cls,
                                   struct MHD_Connection *connection,
                                   void **con_cls,
                                   enum MHD_RequestTerminationCode toe)
{
  (void)cls;
  (void)connection;
  struct HTTP_URI *http_uri;
  struct Proxy *proxy;
  
  http_uri = (struct HTTP_URI *)*con_cls;
  if(NULL == http_uri)
    return;
  proxy = (struct Proxy *)http_uri->proxy;
  assert(NULL != proxy);
  
  PRINT_INFO2("http_cb_request_completed %i for %s; id %i",toe, http_uri->uri, proxy->id);
  
  if(NULL != proxy->http_response)
  {
    MHD_destroy_response (proxy->http_response);
    proxy->http_response = NULL;
  }
  
  if(proxy->spdy_active)
  {
    proxy->http_active = false;
    if(MHD_REQUEST_TERMINATED_COMPLETED_OK != toe)
    {
      proxy->http_error = true;
      if(proxy->stream_id > 0 /*&& NULL != proxy->spdy_connection->session*/)
      {
        //send RST_STREAM_STATUS_CANCEL
        PRINT_INFO2("send rst_stream %i %i",proxy->spdy_active, proxy->stream_id );
        spdylay_submit_rst_stream(proxy->spdy_connection->session, proxy->stream_id, 5);
      }
      /*else
      {
        DLL_remove(proxy->spdy_connection->proxies_head, proxy->spdy_connection->proxies_tail, proxy); 
        free_proxy(proxy);
      }*/
    }
  }
  else
  {
    PRINT_INFO2("proxy free http id %i ", proxy->id);
    free_proxy(proxy);
  }
    
  --glob_opt.responses_pending;
}
