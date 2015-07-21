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
 * @file applicationlayer.c
 * @brief  SPDY application or HTTP layer
 * @author Andrey Uzunov
 */
 
#include "platform.h"
#include "applicationlayer.h"
#include "alstructures.h"
#include "structures.h"
#include "internal.h"
#include "daemon.h"
#include "session.h"


void
spdy_callback_response_done(void *cls,
						struct SPDY_Response *response,
						struct SPDY_Request *request,
						enum SPDY_RESPONSE_RESULT status,
						bool streamopened)
{
	(void)cls;
	(void)status;
	(void)streamopened;
  
	SPDY_destroy_request(request);
	SPDY_destroy_response(response);
}


/**
 * Callback called when new stream is created. It extracts the info from
 * the stream to create (HTTP) request object and pass it to the client.
 *
 * @param cls
 * @param stream the new SPDY stream
 * @return SPDY_YES on success, SPDY_NO on memomry error
 */
static int
spdy_handler_new_stream (void *cls,
						struct SPDYF_Stream * stream)
{
	(void)cls;
	unsigned int i;
	char *method = NULL;
	char *path = NULL;
	char *version = NULL;
	char *host = NULL;
	char *scheme = NULL;
	struct SPDY_Request * request = NULL;
	struct SPDY_NameValue * headers = NULL;
	struct SPDY_NameValue * iterator = stream->headers;
	struct SPDY_Daemon *daemon;
	
	daemon = stream->session->daemon;
	
	//if the user doesn't care, ignore it
	if(NULL == daemon->new_request_cb)
		return SPDY_YES;
	
	if(NULL == (headers=SPDY_name_value_create()))
		goto free_and_fail;
	
	if(NULL==(request = malloc(sizeof(struct SPDY_Request))))
		goto free_and_fail;
	
	memset(request, 0, sizeof(struct SPDY_Request));
	request->stream = stream;
	
	/* extract the mandatory fields from stream->headers' structure
	 * to pass them to the client */
	while(iterator != NULL)
	{
		if(strcmp(":method",iterator->name) == 0)
		{
			if(1 != iterator->num_values)
				break;
			method = iterator->value[0];
		}
		else if(strcmp(":path",iterator->name) == 0)
		{
			if(1 != iterator->num_values)
				break;
			path = iterator->value[0];
		}
		else if(strcmp(":version",iterator->name) == 0)
		{
			if(1 != iterator->num_values)
				break;
			version = iterator->value[0];
		}
		else if(strcmp(":host",iterator->name) == 0)
		{
			//TODO can it have more values?
			if(1 != iterator->num_values)
				break;
			host = iterator->value[0];
		}
		else if(strcmp(":scheme",iterator->name) == 0)
		{
			if(1 != iterator->num_values)
				break;
			scheme = iterator->value[0];
		}
		else
			for(i=0; i<iterator->num_values; ++i)
				if (SPDY_YES != SPDY_name_value_add(headers,iterator->name,iterator->value[i]))
        {
          SPDY_destroy_request(request);
					goto free_and_fail;
        }
		
		iterator = iterator->next;
	}
	
	request->method=method;
  request->path=path;
  request->version=version;
  request->host=host;
  request->scheme=scheme;
  request->headers=headers;
	
	//check request validity, all these fields are mandatory for a request
	if(NULL == method || strlen(method) == 0
		|| NULL == path || strlen(path) == 0
		|| NULL == version || strlen(version) == 0
		|| NULL == host || strlen(host) == 0
		|| NULL == scheme || strlen(scheme) == 0
		)
	{
		//TODO HTTP 400 Bad Request must be answered
		
		SPDYF_DEBUG("Bad request");
		
		SPDY_destroy_request(request);
		
		return SPDY_YES;
	}
	
	//call client's callback function to notify
	daemon->new_request_cb(daemon->cls,
						request,
						stream->priority,
                        method,
                        path,
                        version,
                        host,
                        scheme,
						headers,
            !stream->is_in_closed);
            
  stream->cls = request;

	return SPDY_YES;

	//for GOTO
	free_and_fail:
	
	SPDY_name_value_destroy(headers);
	return SPDY_NO;
}


/**
 * TODO
 */
static int
spdy_handler_new_data (void * cls,
					 struct SPDYF_Stream *stream,
					 const void * buf,
					 size_t size,
					 bool more)
{
  return stream->session->daemon->received_data_cb(cls, stream->cls, buf, size, more);
}



/**
 * Callback to be called when the response queue object was handled and 
 * the data was already sent or discarded. 
 *
 * @param cls
 * @param response_queue the object which is being handled
 * @param status shows if actually the response was sent or it was
 * 			discarded by the lib for any reason (e.g., closing session,
 * 			closing stream, stopping daemon, etc.). It is possible that
 * 			status indicates an error but parts of the response headers
 * 			and/or body (in one
 * 			or several frames) were already sent to the client.
 */
static void
spdy_handler_response_queue_result(void * cls,
								struct SPDYF_Response_Queue *response_queue,
								enum SPDY_RESPONSE_RESULT status)
{
	int streamopened;
	struct SPDY_Request *request = (struct SPDY_Request *)cls;
	
	SPDYF_ASSERT( ( (NULL == response_queue->data_frame) &&
			(NULL != response_queue->control_frame) ) ||
		      ( (NULL != response_queue->data_frame) &&
			(NULL == response_queue->control_frame) ),
		     "response queue must have either control frame or data frame");
	
	streamopened = !response_queue->stream->is_out_closed;
	
	response_queue->rrcb(response_queue->rrcb_cls, response_queue->response, request, status, streamopened);
}


int
(SPDY_init) (enum SPDY_IO_SUBSYSTEM io_subsystem, ...)
{
	SPDYF_ASSERT(SPDYF_BUFFER_SIZE >= SPDY_MAX_SUPPORTED_FRAME_SIZE,
		"Buffer size is less than max supported frame size!");
	SPDYF_ASSERT(SPDY_MAX_SUPPORTED_FRAME_SIZE >= 32,
		"Max supported frame size must be bigger than the minimal value!");
	SPDYF_ASSERT(SPDY_IO_SUBSYSTEM_NONE == spdyf_io_initialized,
		"SPDY_init must be called only once per program or after SPDY_deinit");
    
  if(SPDY_IO_SUBSYSTEM_OPENSSL & io_subsystem)
  {
    SPDYF_openssl_global_init();
    spdyf_io_initialized |= SPDY_IO_SUBSYSTEM_OPENSSL;
  }
  else if(SPDY_IO_SUBSYSTEM_RAW & io_subsystem)
  {
    SPDYF_raw_global_init();
    spdyf_io_initialized |= SPDY_IO_SUBSYSTEM_RAW;
  }
  
	SPDYF_ASSERT(SPDY_IO_SUBSYSTEM_NONE != spdyf_io_initialized,
		"SPDY_init could not find even one IO subsystem");
    
	return SPDY_YES;
}


void
SPDY_deinit ()
{
	SPDYF_ASSERT(SPDY_IO_SUBSYSTEM_NONE != spdyf_io_initialized,
		"SPDY_init has not been called!");
    
  if(SPDY_IO_SUBSYSTEM_OPENSSL & spdyf_io_initialized)
    SPDYF_openssl_global_deinit();
  else if(SPDY_IO_SUBSYSTEM_RAW & spdyf_io_initialized)
    SPDYF_raw_global_deinit();
  
  spdyf_io_initialized = SPDY_IO_SUBSYSTEM_NONE;
}


void 
SPDY_run (struct SPDY_Daemon *daemon)
{
	if(NULL == daemon)
	{
		SPDYF_DEBUG("daemon is NULL");
		return;
	}
	
	SPDYF_run(daemon);
}


int
SPDY_get_timeout (struct SPDY_Daemon *daemon, 
		     unsigned long long *timeout)
{
	if(NULL == daemon)
	{
		SPDYF_DEBUG("daemon is NULL");
		return SPDY_INPUT_ERROR;
	}
	
	return SPDYF_get_timeout(daemon,timeout);
}


int
SPDY_get_fdset (struct SPDY_Daemon *daemon,
				fd_set *read_fd_set,
				fd_set *write_fd_set, 
				fd_set *except_fd_set)
{
	if(NULL == daemon
		|| NULL == read_fd_set
		|| NULL == write_fd_set
		|| NULL == except_fd_set)
	{
		SPDYF_DEBUG("a parameter is NULL");
		return SPDY_INPUT_ERROR;
	}
	
	return SPDYF_get_fdset(daemon,
				read_fd_set,
				write_fd_set, 
				except_fd_set,
				false);
}


struct SPDY_Daemon *
SPDY_start_daemon (uint16_t port,
				const char *certfile,
				const char *keyfile,
		     SPDY_NewSessionCallback nscb,
		     SPDY_SessionClosedCallback sccb,
		     SPDY_NewRequestCallback nrcb,
		     SPDY_NewDataCallback npdcb,
		     void * cls,
		     ...)
{
	struct SPDY_Daemon *daemon;
	va_list valist;
	
	if(SPDY_IO_SUBSYSTEM_NONE == spdyf_io_initialized)
	{
		SPDYF_DEBUG("library not initialized");
		return NULL;
	}
  /*
   * for now make this checks in framing layer
	if(NULL == certfile)
	{
		SPDYF_DEBUG("certfile is NULL");
		return NULL;
	}
	if(NULL == keyfile)
	{
		SPDYF_DEBUG("keyfile is NULL");
		return NULL;
	}
  */
	
	va_start(valist, cls);
	daemon = SPDYF_start_daemon_va ( port,
				certfile,
				keyfile,
		      nscb,
		      sccb,
		      nrcb,
		      npdcb,
		      &spdy_handler_new_stream,
		      &spdy_handler_new_data,
		      cls,
		      NULL,
		      valist
		     );
	va_end(valist);
	
	return daemon;
}


void 
SPDY_stop_daemon (struct SPDY_Daemon *daemon)
{	
	if(NULL == daemon)
	{
		SPDYF_DEBUG("daemon is NULL");
		return;
	}

	SPDYF_stop_daemon(daemon);
}


struct SPDY_Response *
SPDY_build_response(int status,
					const char * statustext,
					const char * version,
					struct SPDY_NameValue * headers,
					const void * data,
					size_t size)
{
	struct SPDY_Response *response = NULL;
	struct SPDY_NameValue ** all_headers = NULL; //TODO maybe array in stack is enough
	char *fullstatus = NULL;
	int ret;
	int num_hdr_containers = 1;
	
	if(NULL == version)
	{
		SPDYF_DEBUG("version is NULL");
		return NULL;
	}
	
	if(NULL == (response = malloc(sizeof(struct SPDY_Response))))
		goto free_and_fail;
	memset(response, 0, sizeof(struct SPDY_Response));
	
	if(NULL != headers && !SPDYF_name_value_is_empty(headers))
		num_hdr_containers = 2;
	
	if(NULL == (all_headers = malloc(num_hdr_containers * sizeof(struct SPDY_NameValue *))))
		goto free_and_fail;
	memset(all_headers, 0, num_hdr_containers * sizeof(struct SPDY_NameValue *));
	
	if(2 == num_hdr_containers)
		all_headers[1] = headers;
	
	if(NULL == (all_headers[0] = SPDY_name_value_create()))
		goto free_and_fail;
	
	if(NULL == statustext)
		ret = asprintf(&fullstatus, "%i", status);
	else
		ret = asprintf(&fullstatus, "%i %s", status, statustext); 
	if(-1 == ret)
		goto free_and_fail;
		
	if(SPDY_YES != SPDY_name_value_add(all_headers[0], ":status", fullstatus))
		goto free_and_fail;
		
	free(fullstatus);
	fullstatus = NULL;
	
	if(SPDY_YES != SPDY_name_value_add(all_headers[0], ":version", version))
		goto free_and_fail;
	
	if(0 >= (response->headers_size = SPDYF_name_value_to_stream(all_headers,
												num_hdr_containers,
												&(response->headers))))
		goto free_and_fail;
		
	SPDY_name_value_destroy(all_headers[0]);
	free(all_headers);
  all_headers = NULL;
	
	if(size > 0)
	{
		//copy the data to the response object
		if(NULL == (response->data = malloc(size)))
		{
			free(response->headers);
			goto free_and_fail;
		}
		memcpy(response->data, data, size);
		response->data_size = size;
	}
	
	return response;
	
	//for GOTO
	free_and_fail:
	
	free(fullstatus);
	if(NULL != all_headers)
		SPDY_name_value_destroy(all_headers[0]);
	free(all_headers);
	free(response);
	
	return NULL;
}


struct SPDY_Response *
SPDY_build_response_with_callback(int status,
					const char * statustext,
					const char * version,
					struct SPDY_NameValue * headers,
					SPDY_ResponseCallback rcb,
					void *rcb_cls,
					uint32_t block_size)
{
	struct SPDY_Response *response;
	
	if(NULL == rcb)
	{
		SPDYF_DEBUG("rcb is NULL");
		return NULL;
	}
	if(block_size > SPDY_MAX_SUPPORTED_FRAME_SIZE)
	{
		SPDYF_DEBUG("block_size is wrong");
		return NULL;
	}
	
	if(0 == block_size)
		block_size = SPDY_MAX_SUPPORTED_FRAME_SIZE;
	
	response = SPDY_build_response(status,
					statustext,
					version,
					headers,
					NULL,
					0);
	
	if(NULL == response)
	{
		return NULL;
	}
			
	response->rcb = rcb;
	response->rcb_cls = rcb_cls;
	response->rcb_block_size = block_size;
	
	return response;
}


int
SPDY_queue_response (struct SPDY_Request * request,
					struct SPDY_Response *response,
					bool closestream,
					bool consider_priority,
					SPDY_ResponseResultCallback rrcb,
					void * rrcb_cls)
{
	struct SPDYF_Response_Queue *headers_to_queue;
	struct SPDYF_Response_Queue *body_to_queue;
	SPDYF_ResponseQueueResultCallback frqcb = NULL;
	void *frqcb_cls = NULL;
	int int_consider_priority = consider_priority ? SPDY_YES : SPDY_NO;
	
	if(NULL == request)
	{
		SPDYF_DEBUG("request is NULL");
		return SPDY_INPUT_ERROR;
	}
	if(NULL == response)
	{
		SPDYF_DEBUG("request is NULL");
		return SPDY_INPUT_ERROR;
	}
	
	if(request->stream->is_out_closed
		|| SPDY_SESSION_STATUS_CLOSING == request->stream->session->status)
		return SPDY_NO;
	
	if(NULL != rrcb)
	{
		frqcb_cls = request;
		frqcb = &spdy_handler_response_queue_result;
	}
	
	if(response->data_size > 0)
	{	
		//SYN_REPLY and DATA will be queued
		
		if(NULL == (headers_to_queue = SPDYF_response_queue_create(false,
							response->headers,
							response->headers_size,
							response,
							request->stream,
							false,
							NULL,
							NULL,
							NULL,
							NULL)))
		{
			return SPDY_NO;
		}
		
		if(NULL == (body_to_queue = SPDYF_response_queue_create(true,
							response->data,
							response->data_size,
							response,
							request->stream,
							closestream,
							frqcb,
							frqcb_cls,
							rrcb,
							rrcb_cls)))
		{
			SPDYF_response_queue_destroy(headers_to_queue);
			return SPDY_NO;
		}
							
		SPDYF_queue_response (headers_to_queue,
							request->stream->session,
							int_consider_priority);
							
		SPDYF_queue_response (body_to_queue,
							request->stream->session,
							int_consider_priority);
	}
	else if(NULL == response->rcb)
	{
		//no "body" will be queued, e.g. HTTP 404 without body
		
		if(NULL == (headers_to_queue = SPDYF_response_queue_create(false,
							response->headers,
							response->headers_size,
							response,
							request->stream,
							closestream,
							frqcb,
							frqcb_cls,
							rrcb,
							rrcb_cls)))
		{
			return SPDY_NO;
		}
							
		SPDYF_queue_response (headers_to_queue,
							request->stream->session,
							int_consider_priority);
	}
	else
	{
		//response with callbacks
		
		if(NULL == (headers_to_queue = SPDYF_response_queue_create(false,
							response->headers,
							response->headers_size,
							response,
							request->stream,
							false,
							NULL,
							NULL,
							NULL,
							NULL)))
		{
			return SPDY_NO;
		}
		
		if(NULL == (body_to_queue = SPDYF_response_queue_create(true,
							response->data,
							response->data_size,
							response,
							request->stream,
							closestream,
							frqcb,
							frqcb_cls,
							rrcb,
							rrcb_cls)))
		{
			SPDYF_response_queue_destroy(headers_to_queue);
			return SPDY_NO;
		}
							
		SPDYF_queue_response (headers_to_queue,
							request->stream->session,
							int_consider_priority);
							
		SPDYF_queue_response (body_to_queue,
							request->stream->session,
							int_consider_priority);
	}
		
	return SPDY_YES;
}


socklen_t
SPDY_get_remote_addr(struct SPDY_Session * session,
					 struct sockaddr ** addr)
{
	if(NULL == session)
	{
		SPDYF_DEBUG("session is NULL");
		return 0;
	}
	
	*addr = session->addr;
	
	return session->addr_len;
}


struct SPDY_Session *
SPDY_get_session_for_request(const struct SPDY_Request * request)
{
	if(NULL == request)
	{
		SPDYF_DEBUG("request is NULL");
		return NULL;
	}
	
	return request->stream->session;
}


void *
SPDY_get_cls_from_session(struct SPDY_Session * session)
{
	if(NULL == session)
	{
		SPDYF_DEBUG("session is NULL");
		return NULL;
	}
	
	return session->user_cls;
}


void
SPDY_set_cls_to_session(struct SPDY_Session * session,
							void * cls)
{
	if(NULL == session)
	{
		SPDYF_DEBUG("session is NULL");
		return;
	}
	
	session->user_cls = cls;
}


void *
SPDY_get_cls_from_request(struct SPDY_Request * request)
{
	if(NULL == request)
	{
		SPDYF_DEBUG("request is NULL");
		return NULL;
	}
	
	return request->user_cls;
}


void
SPDY_set_cls_to_request(struct SPDY_Request * request,
							void * cls)
{
	if(NULL == request)
	{
		SPDYF_DEBUG("request is NULL");
		return;
	}
	
	request->user_cls = cls;
}
