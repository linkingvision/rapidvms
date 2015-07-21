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
 * @file session.c
 * @brief  TCP connection/SPDY session handling. So far most of the
 * 			functions for handling SPDY framing layer are here.
 * @author Andrey Uzunov
 */

#include "platform.h"
#include "structures.h"
#include "internal.h"
#include "session.h"
#include "compression.h"
#include "stream.h"
#include "io.h"


/**
 * Handler for reading the full SYN_STREAM frame after we know that
 * the frame is such.
 * The function waits for the full frame and then changes status
 * of the session. New stream is created.
 *
 * @param session SPDY_Session whose read buffer is used.
 */
static void
spdyf_handler_read_syn_stream (struct SPDY_Session *session)
{
  size_t name_value_strm_size = 0;
  unsigned int compressed_data_size;
  int ret;
  void *name_value_strm = NULL;
  struct SPDYF_Control_Frame *frame;
  struct SPDY_NameValue *headers;

  SPDYF_ASSERT(SPDY_SESSION_STATUS_WAIT_FOR_SUBHEADER == session->status
               || SPDY_SESSION_STATUS_WAIT_FOR_BODY == session->status,
               "the function is called wrong");

  frame = (struct SPDYF_Control_Frame *)session->frame_handler_cls;

  //handle subheaders
  if(SPDY_SESSION_STATUS_WAIT_FOR_SUBHEADER == session->status)
    {
      if(0 == frame->length)
        {
          //protocol error: incomplete frame
          //we just ignore it since there is no stream id for which to
          //send RST_STREAM
          //TODO maybe GOAWAY and closing session is appropriate
          SPDYF_DEBUG("zero long SYN_STREAM received");
          session->status = SPDY_SESSION_STATUS_WAIT_FOR_HEADER;
          free(frame);
          return;
        }

      if(SPDY_YES != SPDYF_stream_new(session))
        {
          /* waiting for some more fields to create new stream
             or something went wrong, SPDYF_stream_new has handled the
             situation */
          return;
        }

      session->current_stream_id = session->streams_head->stream_id;
      if(frame->length > SPDY_MAX_SUPPORTED_FRAME_SIZE)
        {
          //TODO no need to create stream if this happens
          session->status = SPDY_SESSION_STATUS_IGNORE_BYTES;
          return;
        }
      else
        session->status = SPDY_SESSION_STATUS_WAIT_FOR_BODY;
    }

  //handle body

  //start reading the compressed name/value pairs (http headers)
  compressed_data_size = frame->length //everything after length field
    - 10;//4B stream id, 4B assoc strem id, 2B priority, unused and slot

  if(session->read_buffer_offset - session->read_buffer_beginning < compressed_data_size)
    {
      // the full frame is not yet here, try later
      return;
    }

  if ( (compressed_data_size > 0) &&
       (SPDY_YES !=
        SPDYF_zlib_inflate(&session->zlib_recv_stream,
                           session->read_buffer + session->read_buffer_beginning,
                           compressed_data_size,
                           &name_value_strm,
                           &name_value_strm_size)) )
    {
      /* something went wrong on inflating,
       * the state of the stream for decompression is unknown
       * and we may not be able to read anything more received on
       * this session,
       * so it is better to close the session */
      free(name_value_strm);
      free(frame);

      /* mark the session for closing and close it, when
       * everything on the output queue is already written */
      session->status = SPDY_SESSION_STATUS_FLUSHING;

      SPDYF_prepare_goaway(session, SPDY_GOAWAY_STATUS_INTERNAL_ERROR, false);

      return;
    }

  if(0 == name_value_strm_size || 0 == compressed_data_size)
    {
      //Protocol error: send RST_STREAM
      if(SPDY_YES != SPDYF_prepare_rst_stream(session, session->streams_head,
                                              SPDY_RST_STREAM_STATUS_PROTOCOL_ERROR))
        {
          //no memory, try later to send RST
          free(name_value_strm);
          return;
        }
    }
  else
    {
      ret = SPDYF_name_value_from_stream(name_value_strm, name_value_strm_size, &headers);
      if(SPDY_NO == ret)
        {
          //memory error, try later
          free(name_value_strm);
          return;
        }

      session->streams_head->headers = headers;
      //inform the application layer for the new stream received
      if(SPDY_YES != session->daemon->fnew_stream_cb(session->daemon->fcls, session->streams_head))
        {
          //memory error, try later
          free(name_value_strm);
          return;
        }

      session->read_buffer_beginning += compressed_data_size;
    }

  //SPDYF_DEBUG("syn_stream received: id %i", session->current_stream_id);

  //change state to wait for new frame
  free(name_value_strm);
  session->status = SPDY_SESSION_STATUS_WAIT_FOR_HEADER;
  free(frame);
}


/**
 * Handler for reading the GOAWAY frame after we know that
 * the frame is such.
 * The function waits for the full frame and then changes status
 * of the session.
 *
 * @param session SPDY_Session whose read buffer is used.
 */
static void
spdyf_handler_read_goaway (struct SPDY_Session *session)
{
	struct SPDYF_Control_Frame *frame;
	uint32_t last_good_stream_id;
	uint32_t status_int;
	enum SPDY_GOAWAY_STATUS status;

	SPDYF_ASSERT(SPDY_SESSION_STATUS_WAIT_FOR_SUBHEADER == session->status,
		"the function is called wrong");

	frame = (struct SPDYF_Control_Frame *)session->frame_handler_cls;

	if(frame->length > SPDY_MAX_SUPPORTED_FRAME_SIZE)
	{
		//this is a protocol error/attack
		session->status = SPDY_SESSION_STATUS_IGNORE_BYTES;
		return;
	}

	if(0 != frame->flags || 8 != frame->length)
	{
		//this is a protocol error
		SPDYF_DEBUG("wrong GOAWAY received");
		//anyway, it will be handled
	}

	if((session->read_buffer_offset - session->read_buffer_beginning) < frame->length)
	{
		//not all fields are received
		//try later
		return;
	}

	//mark that the session is almost closed
	session->is_goaway_received = true;

	if(8 == frame->length)
	{
		memcpy(&last_good_stream_id, session->read_buffer + session->read_buffer_beginning, 4);
		last_good_stream_id = NTOH31(last_good_stream_id);
		session->read_buffer_beginning += 4;

		memcpy(&status_int, session->read_buffer + session->read_buffer_beginning, 4);
		status = ntohl(status_int);
		session->read_buffer_beginning += 4;

		//TODO do something with last_good

		//SPDYF_DEBUG("Received GOAWAY; status=%i; lastgood=%i",status,last_good_stream_id);

		//do something according to the status
		//TODO
		switch(status)
		{
			case SPDY_GOAWAY_STATUS_OK:
				break;
			case SPDY_GOAWAY_STATUS_PROTOCOL_ERROR:
				break;
			case SPDY_GOAWAY_STATUS_INTERNAL_ERROR:
				break;
		}

    //SPDYF_DEBUG("goaway received: status %i", status);
	}

	session->status = SPDY_SESSION_STATUS_WAIT_FOR_HEADER;
	free(frame);
}


/**
 * Handler for reading RST_STREAM frames. After receiving the frame
 * the stream moves into closed state and status
 * of the session is changed. Frames, belonging to this stream, which
 * are still at the output queue, will be ignored later.
 *
 * @param session SPDY_Session whose read buffer is used.
 */
static void
spdyf_handler_read_rst_stream (struct SPDY_Session *session)
{
	struct SPDYF_Control_Frame *frame;
	uint32_t stream_id;
	int32_t status_int;
	//enum SPDY_RST_STREAM_STATUS status; //for debug
	struct SPDYF_Stream *stream;

	SPDYF_ASSERT(SPDY_SESSION_STATUS_WAIT_FOR_SUBHEADER == session->status,
		"the function is called wrong");

	frame = (struct SPDYF_Control_Frame *)session->frame_handler_cls;

	if(0 != frame->flags || 8 != frame->length)
	{
		//this is a protocol error
		SPDYF_DEBUG("wrong RST_STREAM received");
		//ignore as a large frame
		session->status = SPDY_SESSION_STATUS_IGNORE_BYTES;
		return;
	}

	if((session->read_buffer_offset - session->read_buffer_beginning) < frame->length)
	{
		//not all fields are received
		//try later
		return;
	}

    memcpy(&stream_id, session->read_buffer + session->read_buffer_beginning, 4);
	stream_id = NTOH31(stream_id);
	session->read_buffer_beginning += 4;

    memcpy(&status_int, session->read_buffer + session->read_buffer_beginning, 4);
	//status = ntohl(status_int); //for debug
	session->read_buffer_beginning += 4;

	session->status = SPDY_SESSION_STATUS_WAIT_FOR_HEADER;
	free(frame);

	//mark the stream as closed
	stream = session->streams_head;
	while(NULL != stream)
	{
		if(stream_id == stream->stream_id)
		{
			stream->is_in_closed = true;
			stream->is_out_closed = true;
			break;
		}
		stream = stream->next;
	}

	//SPDYF_DEBUG("Received RST_STREAM; status=%i; id=%i",status,stream_id);

	//do something according to the status
	//TODO
	/*switch(status)
	{
		case SPDY_RST_STREAM_STATUS_PROTOCOL_ERROR:
			break;
	}*/
}


/**
 * Handler for reading DATA frames. In requests they are used for POST
 * arguments.
 *
 * @param session SPDY_Session whose read buffer is used.
 */
static void
spdyf_handler_read_data (struct SPDY_Session *session)
{
  int ret;
  struct SPDYF_Data_Frame * frame;
  struct SPDYF_Stream * stream;

	SPDYF_ASSERT(SPDY_SESSION_STATUS_WAIT_FOR_SUBHEADER == session->status
		|| SPDY_SESSION_STATUS_WAIT_FOR_BODY == session->status,
		"the function is called wrong");

  //SPDYF_DEBUG("DATA frame received (POST?). Ignoring");

  //SPDYF_SIGINT("");

	frame = (struct SPDYF_Data_Frame *)session->frame_handler_cls;

	//handle subheaders
	if(SPDY_SESSION_STATUS_WAIT_FOR_SUBHEADER == session->status)
	{
		if(frame->length > SPDY_MAX_SUPPORTED_FRAME_SIZE)
		{
			session->status = SPDY_SESSION_STATUS_IGNORE_BYTES;
			return;
		}
		else
			session->status = SPDY_SESSION_STATUS_WAIT_FOR_BODY;
	}

	//handle body

	if(session->read_buffer_offset - session->read_buffer_beginning
		>= frame->length)
	{
    stream = SPDYF_stream_find(frame->stream_id, session);

    if(NULL == stream || stream->is_in_closed || NULL == session->daemon->received_data_cb)
    {
      if(NULL == session->daemon->received_data_cb)
      SPDYF_DEBUG("No callback for DATA frame set; Ignoring DATA frame!");

      //TODO send error?

      //TODO for now ignore frame
      session->read_buffer_beginning += frame->length;
      session->status = SPDY_SESSION_STATUS_WAIT_FOR_HEADER;
      free(frame);
      return;
    }

    ret = session->daemon->freceived_data_cb(session->daemon->cls,
                                      stream,
                                      session->read_buffer + session->read_buffer_beginning,
                                      frame->length,
                                      0 == (SPDY_DATA_FLAG_FIN & frame->flags));

    session->read_buffer_beginning += frame->length;

    stream->window_size -= frame->length;

    //TODO close in and send rst maybe
    SPDYF_ASSERT(SPDY_YES == ret, "Cancel POST data is not yet implemented");

    if(SPDY_DATA_FLAG_FIN & frame->flags)
    {
      stream->is_in_closed = true;
    }
    else if(stream->window_size < SPDYF_INITIAL_WINDOW_SIZE / 2)
    {
      //very simple implementation of flow control
      //when the window's size is under the half of the initial value,
      //increase it again up to the initial value

      //prepare WINDOW_UPDATE
      if(SPDY_YES == SPDYF_prepare_window_update(session, stream,
            SPDYF_INITIAL_WINDOW_SIZE - stream->window_size))
      {
        stream->window_size = SPDYF_INITIAL_WINDOW_SIZE;
      }
      //else: do it later
    }

    //SPDYF_DEBUG("data received: id %i", frame->stream_id);

    session->status = SPDY_SESSION_STATUS_WAIT_FOR_HEADER;
    free(frame);
	}
}


int
SPDYF_handler_write_syn_reply (struct SPDY_Session *session)
{
	struct SPDYF_Response_Queue *response_queue = session->response_queue_head;
	struct SPDYF_Stream *stream = response_queue->stream;
	struct SPDYF_Control_Frame control_frame;
	void *compressed_headers = NULL;
	size_t compressed_headers_size=0;
	size_t used_data=0;
	size_t total_size;
	uint32_t stream_id_nbo;

	SPDYF_ASSERT(NULL == session->write_buffer, "the function is called not in the correct moment");

	memcpy(&control_frame, response_queue->control_frame, sizeof(control_frame));

	if(SPDY_YES != SPDYF_zlib_deflate(&session->zlib_send_stream,
		response_queue->data,
		response_queue->data_size,
		&used_data,
		&compressed_headers,
		&compressed_headers_size))
	{
		/* something went wrong on compressing,
		* the state of the stream for compression is unknown
		* and we may not be able to send anything more on
		* this session,
		* so it is better to close the session right now */
		session->status = SPDY_SESSION_STATUS_CLOSING;

		free(compressed_headers);

		return SPDY_NO;
	}

	//TODO do we need this used_Data
	SPDYF_ASSERT(used_data == response_queue->data_size, "not everything was used by zlib");

	total_size = sizeof(struct SPDYF_Control_Frame) //SPDY header
		+ 4 // stream id as "subheader"
		+ compressed_headers_size;

	if(NULL == (session->write_buffer = malloc(total_size)))
	{
		/* no memory
		 * since we do not save the compressed data anywhere and
		 * the sending zlib stream is already in new state, we must
		 * close the session */
		session->status = SPDY_SESSION_STATUS_CLOSING;

		free(compressed_headers);

		return SPDY_NO;
	}
	session->write_buffer_beginning = 0;
	session->write_buffer_offset = 0;
	session->write_buffer_size = total_size;

	control_frame.length = compressed_headers_size + 4; // compressed data + stream_id
	SPDYF_CONTROL_FRAME_HTON(&control_frame);

	//put frame headers to write buffer
	memcpy(session->write_buffer + session->write_buffer_offset,&control_frame,sizeof(struct SPDYF_Control_Frame));
	session->write_buffer_offset +=  sizeof(struct SPDYF_Control_Frame);

	//put stream id to write buffer
	stream_id_nbo = HTON31(stream->stream_id);
	memcpy(session->write_buffer + session->write_buffer_offset, &stream_id_nbo, 4);
	session->write_buffer_offset += 4;

	//put compressed name/value pairs to write buffer
	memcpy(session->write_buffer + session->write_buffer_offset, compressed_headers, compressed_headers_size);
	session->write_buffer_offset +=  compressed_headers_size;

	SPDYF_ASSERT(0 == session->write_buffer_beginning, "bug1");
	SPDYF_ASSERT(session->write_buffer_offset == session->write_buffer_size, "bug2");

	//DEBUG CODE, break compression state to see what happens
/*	SPDYF_zlib_deflate(&session->zlib_send_stream,
		"1234567890",
		10,
		&used_data,
		&compressed_headers,
		&compressed_headers_size);
*/
	free(compressed_headers);

	session->last_replied_to_stream_id = stream->stream_id;

  //SPDYF_DEBUG("syn_reply sent: id %i", stream->stream_id);

	return SPDY_YES;
}


int
SPDYF_handler_write_goaway (struct SPDY_Session *session)
{
	struct SPDYF_Response_Queue *response_queue = session->response_queue_head;
	struct SPDYF_Control_Frame control_frame;
	size_t total_size;
	int last_good_stream_id;

	SPDYF_ASSERT(NULL == session->write_buffer, "the function is called not in the correct moment");

	memcpy(&control_frame, response_queue->control_frame, sizeof(control_frame));

	session->is_goaway_sent = true;

	total_size = sizeof(struct SPDYF_Control_Frame) //SPDY header
		+ 4 // last good stream id as "subheader"
		+ 4; // status code as "subheader"

	if(NULL == (session->write_buffer = malloc(total_size)))
	{
		return SPDY_NO;
	}
	session->write_buffer_beginning = 0;
	session->write_buffer_offset = 0;
	session->write_buffer_size = total_size;

	control_frame.length = 8; // always for GOAWAY
	SPDYF_CONTROL_FRAME_HTON(&control_frame);

	//put frame headers to write buffer
	memcpy(session->write_buffer + session->write_buffer_offset,&control_frame,sizeof(struct SPDYF_Control_Frame));
	session->write_buffer_offset +=  sizeof(struct SPDYF_Control_Frame);

	//put last good stream id to write buffer
	last_good_stream_id = HTON31(session->last_replied_to_stream_id);
	memcpy(session->write_buffer + session->write_buffer_offset, &last_good_stream_id, 4);
	session->write_buffer_offset +=  4;

	//put "data" to write buffer. This is the status
	memcpy(session->write_buffer + session->write_buffer_offset, response_queue->data, 4);
	session->write_buffer_offset +=  4;
	//data is not freed by the destroy function so:
	//free(response_queue->data);

  //SPDYF_DEBUG("goaway sent: status %i", NTOH31(*(uint32_t*)(response_queue->data)));

	SPDYF_ASSERT(0 == session->write_buffer_beginning, "bug1");
	SPDYF_ASSERT(session->write_buffer_offset == session->write_buffer_size, "bug2");

	return SPDY_YES;
}


int
SPDYF_handler_write_data (struct SPDY_Session *session)
{
	struct SPDYF_Response_Queue *response_queue = session->response_queue_head;
	struct SPDYF_Response_Queue *new_response_queue;
	size_t total_size;
	struct SPDYF_Data_Frame data_frame;
	ssize_t ret;
	bool more;

	SPDYF_ASSERT(NULL == session->write_buffer, "the function is called not in the correct moment");

	memcpy(&data_frame, response_queue->data_frame, sizeof(data_frame));

	if(NULL == response_queue->response->rcb)
	{
		//standard response with data into the struct
		SPDYF_ASSERT(NULL != response_queue->data, "no data for the response");

		total_size = sizeof(struct SPDYF_Data_Frame) //SPDY header
			+ response_queue->data_size;

		if(NULL == (session->write_buffer = malloc(total_size)))
		{
			return SPDY_NO;
		}
		session->write_buffer_beginning = 0;
		session->write_buffer_offset = 0;
		session->write_buffer_size = total_size;

		data_frame.length = response_queue->data_size;
		SPDYF_DATA_FRAME_HTON(&data_frame);

		//put SPDY headers to the writing buffer
		memcpy(session->write_buffer + session->write_buffer_offset,&data_frame,sizeof(struct SPDYF_Data_Frame));
		session->write_buffer_offset +=  sizeof(struct SPDYF_Data_Frame);

		//put data to the writing buffer
		memcpy(session->write_buffer + session->write_buffer_offset, response_queue->data, response_queue->data_size);
		session->write_buffer_offset +=  response_queue->data_size;
	}
	else
	{
		/* response with callbacks. The lib will produce more than 1
		 * data frames
		 */

		total_size = sizeof(struct SPDYF_Data_Frame) //SPDY header
			+ SPDY_MAX_SUPPORTED_FRAME_SIZE; //max possible size

		if(NULL == (session->write_buffer = malloc(total_size)))
		{
			return SPDY_NO;
		}
		session->write_buffer_beginning = 0;
		session->write_buffer_offset = 0;
		session->write_buffer_size = total_size;

		ret = response_queue->response->rcb(response_queue->response->rcb_cls,
			session->write_buffer + sizeof(struct SPDYF_Data_Frame),
			response_queue->response->rcb_block_size,
			&more);

		if(ret < 0 || ret > response_queue->response->rcb_block_size)
		{
			free(session->write_buffer);
      session->write_buffer = NULL;

      //send RST_STREAM
      if(SPDY_YES == (ret = SPDYF_prepare_rst_stream(session,
        response_queue->stream,
        SPDY_RST_STREAM_STATUS_INTERNAL_ERROR)))
      {
        return SPDY_NO;
      }

      //else no memory
			//for now close session
			//TODO what?
			session->status = SPDY_SESSION_STATUS_CLOSING;

			return SPDY_NO;
		}
		if(0 == ret && more)
		{
			//the app couldn't write anything to buf but later will
			free(session->write_buffer);
			session->write_buffer = NULL;
			session->write_buffer_size = 0;

			if(NULL != response_queue->next)
			{
				//put the frame at the end of the queue
				//otherwise - head of line blocking
				session->response_queue_head = response_queue->next;
				session->response_queue_head->prev = NULL;
				session->response_queue_tail->next = response_queue;
				response_queue->prev = session->response_queue_tail;
				response_queue->next = NULL;
				session->response_queue_tail = response_queue;
			}

			return SPDY_YES;
		}

		if(more)
		{
			//create another response queue object to call the user cb again
			if(NULL == (new_response_queue = SPDYF_response_queue_create(true,
							NULL,
							0,
							response_queue->response,
							response_queue->stream,
							false,
							response_queue->frqcb,
							response_queue->frqcb_cls,
							response_queue->rrcb,
							response_queue->rrcb_cls)))
			{
				//TODO send RST_STREAM
				//for now close session
				session->status = SPDY_SESSION_STATUS_CLOSING;

				free(session->write_buffer);
        session->write_buffer = NULL;
				return SPDY_NO;
			}

			//put it at second position on the queue
			new_response_queue->prev = response_queue;
			new_response_queue->next = response_queue->next;
			if(NULL == response_queue->next)
			{
				session->response_queue_tail = new_response_queue;
			}
			else
			{
				response_queue->next->prev = new_response_queue;
			}
			response_queue->next = new_response_queue;

			response_queue->frqcb = NULL;
			response_queue->frqcb_cls = NULL;
			response_queue->rrcb = NULL;
			response_queue->rrcb_cls = NULL;
		}
		else
		{
			data_frame.flags |= SPDY_DATA_FLAG_FIN;
		}

		data_frame.length = ret;
		SPDYF_DATA_FRAME_HTON(&data_frame);

		//put SPDY headers to the writing buffer
		memcpy(session->write_buffer + session->write_buffer_offset,
			&data_frame,
			sizeof(struct SPDYF_Data_Frame));
		session->write_buffer_offset +=  sizeof(struct SPDYF_Data_Frame);
		session->write_buffer_offset +=  ret;
		session->write_buffer_size = session->write_buffer_offset;
	}

  //SPDYF_DEBUG("data sent: id %i", NTOH31(data_frame.stream_id));

	SPDYF_ASSERT(0 == session->write_buffer_beginning, "bug1");
	SPDYF_ASSERT(session->write_buffer_offset == session->write_buffer_size, "bug2");

	return SPDY_YES;
}


int
SPDYF_handler_write_rst_stream (struct SPDY_Session *session)
{
	struct SPDYF_Response_Queue *response_queue = session->response_queue_head;
	struct SPDYF_Control_Frame control_frame;
	size_t total_size;

	SPDYF_ASSERT(NULL == session->write_buffer, "the function is called not in the correct moment");

	memcpy(&control_frame, response_queue->control_frame, sizeof(control_frame));

	total_size = sizeof(struct SPDYF_Control_Frame) //SPDY header
		+ 4 // stream id as "subheader"
		+ 4; // status code as "subheader"

	if(NULL == (session->write_buffer = malloc(total_size)))
	{
		return SPDY_NO;
	}
	session->write_buffer_beginning = 0;
	session->write_buffer_offset = 0;
	session->write_buffer_size = total_size;

	control_frame.length = 8; // always for RST_STREAM
	SPDYF_CONTROL_FRAME_HTON(&control_frame);

	//put frame headers to write buffer
	memcpy(session->write_buffer + session->write_buffer_offset,&control_frame,sizeof(struct SPDYF_Control_Frame));
	session->write_buffer_offset +=  sizeof(struct SPDYF_Control_Frame);

	//put stream id to write buffer. This is the status
	memcpy(session->write_buffer + session->write_buffer_offset, response_queue->data, 8);
	session->write_buffer_offset +=  8;
	//data is not freed by the destroy function so:
	//free(response_queue->data);

  //SPDYF_DEBUG("rst_stream sent: id %i", NTOH31((((uint64_t)response_queue->data) & 0xFFFF0000) >> 32));

	SPDYF_ASSERT(0 == session->write_buffer_beginning, "bug1");
	SPDYF_ASSERT(session->write_buffer_offset == session->write_buffer_size, "bug2");

	return SPDY_YES;
}


int
SPDYF_handler_write_window_update (struct SPDY_Session *session)
{
	struct SPDYF_Response_Queue *response_queue = session->response_queue_head;
	struct SPDYF_Control_Frame control_frame;
	size_t total_size;

	SPDYF_ASSERT(NULL == session->write_buffer, "the function is called not in the correct moment");

	memcpy(&control_frame, response_queue->control_frame, sizeof(control_frame));

	total_size = sizeof(struct SPDYF_Control_Frame) //SPDY header
		+ 4 // stream id as "subheader"
		+ 4; // delta-window-size as "subheader"

	if(NULL == (session->write_buffer = malloc(total_size)))
	{
		return SPDY_NO;
	}
	session->write_buffer_beginning = 0;
	session->write_buffer_offset = 0;
	session->write_buffer_size = total_size;

	control_frame.length = 8; // always for WINDOW_UPDATE
	SPDYF_CONTROL_FRAME_HTON(&control_frame);

	//put frame headers to write buffer
	memcpy(session->write_buffer + session->write_buffer_offset,&control_frame,sizeof(struct SPDYF_Control_Frame));
	session->write_buffer_offset +=  sizeof(struct SPDYF_Control_Frame);

	//put stream id and delta-window-size to write buffer
	memcpy(session->write_buffer + session->write_buffer_offset, response_queue->data, 8);
	session->write_buffer_offset +=  8;

  //SPDYF_DEBUG("window_update sent: id %i", NTOH31((((uint64_t)response_queue->data) & 0xFFFF0000) >> 32));

	SPDYF_ASSERT(0 == session->write_buffer_beginning, "bug1");
	SPDYF_ASSERT(session->write_buffer_offset == session->write_buffer_size, "bug2");

	return SPDY_YES;
}


void
SPDYF_handler_ignore_frame (struct SPDY_Session *session)
{
	struct SPDYF_Control_Frame *frame;

	SPDYF_ASSERT(SPDY_SESSION_STATUS_WAIT_FOR_SUBHEADER == session->status
		|| SPDY_SESSION_STATUS_WAIT_FOR_BODY == session->status,
		"the function is called wrong");


	frame = (struct SPDYF_Control_Frame *)session->frame_handler_cls;

	//handle subheaders
	if(SPDY_SESSION_STATUS_WAIT_FOR_SUBHEADER == session->status)
	{
		if(frame->length > SPDY_MAX_SUPPORTED_FRAME_SIZE)
		{
			session->status = SPDY_SESSION_STATUS_IGNORE_BYTES;
			return;
		}
		else
			session->status = SPDY_SESSION_STATUS_WAIT_FOR_BODY;
	}

	//handle body

	if(session->read_buffer_offset - session->read_buffer_beginning
		>= frame->length)
	{
		session->read_buffer_beginning += frame->length;
		session->status = SPDY_SESSION_STATUS_WAIT_FOR_HEADER;
		free(frame);
	}
}


int
SPDYF_session_read (struct SPDY_Session *session)
{
	int bytes_read;
	bool reallocate;
	size_t actual_buf_size;

	if(SPDY_SESSION_STATUS_CLOSING == session->status
		|| SPDY_SESSION_STATUS_FLUSHING == session->status)
		return SPDY_NO;

	//if the read buffer is full to the end, we need to reallocate space
	if (session->read_buffer_size == session->read_buffer_offset)
	{
		//but only if the state of the session requires it
		//i.e. no further proceeding is possible without reallocation
		reallocate = false;
		actual_buf_size = session->read_buffer_offset
			- session->read_buffer_beginning;
		switch(session->status)
		{
			case SPDY_SESSION_STATUS_WAIT_FOR_HEADER:

			case SPDY_SESSION_STATUS_IGNORE_BYTES:
				//we need space for a whole control frame header
				if(actual_buf_size < sizeof(struct SPDYF_Control_Frame))
					reallocate = true;
				break;

			case SPDY_SESSION_STATUS_WAIT_FOR_SUBHEADER:

			case SPDY_SESSION_STATUS_WAIT_FOR_BODY:
				//we need as many bytes as set in length field of the
				//header
				SPDYF_ASSERT(NULL != session->frame_handler_cls,
					"no frame for session");
				if(session->frame_handler != &spdyf_handler_read_data)
				{
					if(actual_buf_size
						< ((struct SPDYF_Control_Frame *)session->frame_handler_cls)->length)
						reallocate = true;
				}
				else
				{
					if(actual_buf_size
						< ((struct SPDYF_Data_Frame *)session->frame_handler_cls)->length)
						reallocate = true;
				}
				break;

			case SPDY_SESSION_STATUS_CLOSING:
			case SPDY_SESSION_STATUS_FLUSHING:
				//nothing needed
				break;
		}

		if(reallocate)
		{
			//reuse the space in the buffer that was already read by the lib
			memmove(session->read_buffer,
				session->read_buffer + session->read_buffer_beginning,
				session->read_buffer_offset - session->read_buffer_beginning);

			session->read_buffer_offset -= session->read_buffer_beginning;
			session->read_buffer_beginning = 0;
		}
		else
		{
			//will read next time
			//TODO optimize it, memmove more often?
			return SPDY_NO;
		}
	}

	session->last_activity = SPDYF_monotonic_time();

	//actual read from the TLS socket
	bytes_read = session->fio_recv(session,
					session->read_buffer + session->read_buffer_offset,
					session->read_buffer_size - session->read_buffer_offset);

	switch(bytes_read)
	{
		case SPDY_IO_ERROR_CLOSED:
			//The TLS connection was closed by the other party, clean
			//or not
			shutdown (session->socket_fd, SHUT_RD);
			session->read_closed = true;
			session->status = SPDY_SESSION_STATUS_CLOSING;
			return SPDY_YES;

		case SPDY_IO_ERROR_ERROR:
			//any kind of error in the TLS subsystem
			//try to prepare GOAWAY frame
			SPDYF_prepare_goaway(session, SPDY_GOAWAY_STATUS_INTERNAL_ERROR, false);
			//try to flush the queue when write is called
			session->status = SPDY_SESSION_STATUS_FLUSHING;
			return SPDY_YES;

		case SPDY_IO_ERROR_AGAIN:
			//read or write should be called again; leave it for the
			//next time
			return SPDY_NO;

		//default:
			//something was really read from the TLS subsystem
			//just continue
	}

	session->read_buffer_offset += bytes_read;

	return SPDY_YES;
}


int
SPDYF_session_write (struct SPDY_Session *session,
                     bool only_one_frame)
{
	unsigned int i;
	int bytes_written;
	struct SPDYF_Response_Queue *queue_head;
	struct SPDYF_Response_Queue *response_queue;

	if(SPDY_SESSION_STATUS_CLOSING == session->status)
		return SPDY_NO;

  if(SPDY_NO == session->fio_before_write(session))
    return SPDY_NO;

	for(i=0;
		only_one_frame
		? i < 1
		: i < session->max_num_frames;
		++i)
	{
		//if the buffer is not null, part of the last frame is still
		//pending to be sent
		if(NULL == session->write_buffer)
		{
			//discard frames on closed streams
			response_queue = session->response_queue_head;

			while(NULL != response_queue)
			{
				//if stream is closed, remove not yet sent frames
				//associated with it
				//GOAWAY frames are not associated to streams
				//and still need to be sent
				if(NULL == response_queue->stream
					|| !response_queue->stream->is_out_closed)
					break;

				DLL_remove(session->response_queue_head,session->response_queue_tail,response_queue);

				if(NULL != response_queue->frqcb)
				{
					response_queue->frqcb(response_queue->frqcb_cls, response_queue, SPDY_RESPONSE_RESULT_STREAM_CLOSED);
				}

				SPDYF_response_queue_destroy(response_queue);
				response_queue = session->response_queue_head;
			}

			if(NULL == session->response_queue_head)
				break;//nothing on the queue

			//get next data from queue and put it to the write buffer
			// to send it
			if(SPDY_NO == session->response_queue_head->process_response_handler(session))
			{
				//error occured and the handler changed or not the
				//session's status appropriately
				if(SPDY_SESSION_STATUS_CLOSING == session->status)
				{
					//try to send GOAWAY first if the current frame is different
					if(session->response_queue_head->is_data
						|| SPDY_CONTROL_FRAME_TYPES_GOAWAY
							!= session->response_queue_head->control_frame->type)
					{
						session->status = SPDY_SESSION_STATUS_FLUSHING;
						SPDYF_prepare_goaway(session, SPDY_GOAWAY_STATUS_INTERNAL_ERROR, true);
						SPDYF_session_write(session,true);
						session->status = SPDY_SESSION_STATUS_CLOSING;
					}
					return SPDY_YES;
				}

				//just return from the loop to return from this function
        ++i;
				break;
			}

			//check if something was prepared for writing
			//on respones with callbacks it is possible that their is no
			//data available
			if(0 == session->write_buffer_size)//nothing to write
      {
				if(response_queue != session->response_queue_head)
				{
					//the handler modified the queue
					continue;
				}
				else
				{
					//no need to try the same frame again
          ++i;
					break;
				}
      }
		}

		session->last_activity = SPDYF_monotonic_time();

		//actual write to the IO
		bytes_written = session->fio_send(session,
			session->write_buffer + session->write_buffer_beginning,
			session->write_buffer_offset - session->write_buffer_beginning);

		switch(bytes_written)
		{
			case SPDY_IO_ERROR_CLOSED:
				//The TLS connection was closed by the other party, clean
				//or not
				shutdown (session->socket_fd, SHUT_RD);
				session->read_closed = true;
				session->status = SPDY_SESSION_STATUS_CLOSING;
				return SPDY_YES;

			case SPDY_IO_ERROR_ERROR:
				//any kind of error in the TLS subsystem
				//forbid more writing
				session->status = SPDY_SESSION_STATUS_CLOSING;
				return SPDY_YES;

			case SPDY_IO_ERROR_AGAIN:
				//read or write should be called again; leave it for the
				//next time; return from the function as we do not now
				//whether reading or writing is needed
				return i>0 ? SPDY_YES : SPDY_NO;

			//default:
				//something was really read from the TLS subsystem
				//just continue
		}

		session->write_buffer_beginning += bytes_written;

		//check if the full buffer was written
		if(session->write_buffer_beginning == session->write_buffer_size)
		{
			//that response is handled, remove it from queue
      free(session->write_buffer);
			session->write_buffer = NULL;
			session->write_buffer_size = 0;
			queue_head = session->response_queue_head;
			if(NULL == queue_head->next)
			{
				session->response_queue_head = NULL;
				session->response_queue_tail = NULL;
			}
			else
			{
				session->response_queue_head = queue_head->next;
				session->response_queue_head->prev = NULL;
			}

			//set stream to closed if the frame's fin flag is set
			SPDYF_stream_set_flags_on_write(queue_head);

			if(NULL != queue_head->frqcb)
			{
				//application layer callback to notify sending of the response
				queue_head->frqcb(queue_head->frqcb_cls, queue_head, SPDY_RESPONSE_RESULT_SUCCESS);
			}

			SPDYF_response_queue_destroy(queue_head);
		}
	}

	if(SPDY_SESSION_STATUS_FLUSHING == session->status
		&& NULL == session->response_queue_head)
		session->status = SPDY_SESSION_STATUS_CLOSING;

	//return i>0 ? SPDY_YES : SPDY_NO;
	return session->fio_after_write(session, i>0 ? SPDY_YES : SPDY_NO);
}


int
SPDYF_session_idle (struct SPDY_Session *session)
{
	size_t read_buffer_beginning;
	size_t frame_length;
	struct SPDYF_Control_Frame* control_frame;
	struct SPDYF_Data_Frame *data_frame;

	//prepare session for closing if timeout is used and already passed
	if(SPDY_SESSION_STATUS_CLOSING != session->status
		&& session->daemon->session_timeout
		&& (session->last_activity + session->daemon->session_timeout < SPDYF_monotonic_time()))
	{
		session->status = SPDY_SESSION_STATUS_CLOSING;
		//best effort for sending GOAWAY
		SPDYF_prepare_goaway(session, SPDY_GOAWAY_STATUS_OK, true);
		SPDYF_session_write(session,true);
	}

	switch(session->status)
	{
		//expect new frame to arrive
		case SPDY_SESSION_STATUS_WAIT_FOR_HEADER:
			session->current_stream_id = 0;
			//check if the whole frame header is already here
			//both frame types have the same length
			if(session->read_buffer_offset - session->read_buffer_beginning
				< sizeof(struct SPDYF_Control_Frame))
				return SPDY_NO;

			/* check the first bit to see if it is data or control frame
			 * and also if the version is supported */
			if(0x80 == *(uint8_t *)(session->read_buffer + session->read_buffer_beginning)
				&& SPDY_VERSION == *((uint8_t *)session->read_buffer + session->read_buffer_beginning + 1))
			{
				//control frame
				if(NULL == (control_frame = malloc(sizeof(struct SPDYF_Control_Frame))))
				{
					SPDYF_DEBUG("No memory");
					return SPDY_NO;
				}

				//get frame headers
				memcpy(control_frame,
					session->read_buffer + session->read_buffer_beginning,
					sizeof(struct SPDYF_Control_Frame));
				session->read_buffer_beginning += sizeof(struct SPDYF_Control_Frame);
				SPDYF_CONTROL_FRAME_NTOH(control_frame);

				session->status = SPDY_SESSION_STATUS_WAIT_FOR_SUBHEADER;
				//assign different frame handler according to frame type
				switch(control_frame->type){
					case SPDY_CONTROL_FRAME_TYPES_SYN_STREAM:
						session->frame_handler = &spdyf_handler_read_syn_stream;
						break;
					case SPDY_CONTROL_FRAME_TYPES_GOAWAY:
						session->frame_handler = &spdyf_handler_read_goaway;
						break;
					case SPDY_CONTROL_FRAME_TYPES_RST_STREAM:
						session->frame_handler = &spdyf_handler_read_rst_stream;
						break;
					default:
						session->frame_handler = &SPDYF_handler_ignore_frame;
				}
				session->frame_handler_cls = control_frame;
				//DO NOT break the outer case
			}
			else if(0 == *(uint8_t *)(session->read_buffer + session->read_buffer_beginning))
			{
				//needed for POST
				//data frame
				if(NULL == (data_frame = malloc(sizeof(struct SPDYF_Data_Frame))))
				{
					SPDYF_DEBUG("No memory");
					return SPDY_NO;
				}

				//get frame headers
				memcpy(data_frame,
					session->read_buffer + session->read_buffer_beginning,
					sizeof(struct SPDYF_Data_Frame));
				session->read_buffer_beginning += sizeof(struct SPDYF_Data_Frame);
				SPDYF_DATA_FRAME_NTOH(data_frame);

				session->status = SPDY_SESSION_STATUS_WAIT_FOR_BODY;
				session->frame_handler = &spdyf_handler_read_data;
				session->frame_handler_cls = data_frame;
				//DO NOT brake the outer case
			}
			else
			{
				SPDYF_DEBUG("another protocol or version received!");

				/* According to the draft the lib should send here
				 * RST_STREAM with status UNSUPPORTED_VERSION. I don't
				 * see any sense of keeping the session open since
				 * we don't know how many bytes is the bogus "frame".
				 * And the latter normally will be HTTP request.
				 *
				 */

				//shutdown(session->socket_fd, SHUT_RD);
				session->status = SPDY_SESSION_STATUS_FLUSHING;
				SPDYF_prepare_goaway(session, SPDY_GOAWAY_STATUS_PROTOCOL_ERROR,false);
				//SPDYF_session_write(session,false);
				/* close connection since the client expects another
				protocol from us */
				//SPDYF_session_close(session);
				return SPDY_YES;
			}

		//expect specific header fields after the standard header
		case SPDY_SESSION_STATUS_WAIT_FOR_SUBHEADER:
			if(NULL!=session->frame_handler)
			{
				read_buffer_beginning = session->read_buffer_beginning;
				//if everything is ok, the "body" will also be processed
				//by the handler
				session->frame_handler(session);

				if(SPDY_SESSION_STATUS_IGNORE_BYTES == session->status)
				{
					//check for larger than max supported frame
					if(session->frame_handler != &spdyf_handler_read_data)
					{
						frame_length = ((struct SPDYF_Control_Frame *)session->frame_handler_cls)->length;
					}
					else
					{
						frame_length = ((struct SPDYF_Data_Frame *)session->frame_handler_cls)->length;
					}

					//if(SPDY_MAX_SUPPORTED_FRAME_SIZE < frame_length)
					{
						SPDYF_DEBUG("received frame with unsupported size: %zu", frame_length);
						//the data being received must be ignored and
						//RST_STREAM sent

						//ignore bytes that will arive later
						session->read_ignore_bytes = frame_length
							+ read_buffer_beginning
							- session->read_buffer_offset;
						//ignore what is already in read buffer
						session->read_buffer_beginning = session->read_buffer_offset;

						SPDYF_prepare_rst_stream(session,
							session->current_stream_id > 0 ? session->streams_head : NULL, //may be 0 here which is not good
							SPDY_RST_STREAM_STATUS_FRAME_TOO_LARGE);

						//actually the read buffer can be bigger than the
						//max supported size
						session->status = session->read_ignore_bytes
							? SPDY_SESSION_STATUS_IGNORE_BYTES
							: SPDY_SESSION_STATUS_WAIT_FOR_HEADER;

						free(session->frame_handler_cls);
					}
				}
			}

			if(SPDY_SESSION_STATUS_IGNORE_BYTES != session->status)
			{
				break;
			}

		//ignoring data in read buffer
		case SPDY_SESSION_STATUS_IGNORE_BYTES:
			SPDYF_ASSERT(session->read_ignore_bytes > 0,
				"Session is in wrong state");
			if(session->read_ignore_bytes
				> session->read_buffer_offset - session->read_buffer_beginning)
			{
				session->read_ignore_bytes -=
					session->read_buffer_offset - session->read_buffer_beginning;
				session->read_buffer_beginning = session->read_buffer_offset;
			}
			else
			{
				session->read_buffer_beginning += session->read_ignore_bytes;
				session->read_ignore_bytes = 0;
				session->status = SPDY_SESSION_STATUS_WAIT_FOR_HEADER;
			}
			break;

		//expect frame body (name/value pairs)
		case SPDY_SESSION_STATUS_WAIT_FOR_BODY:
			if(NULL!=session->frame_handler)
				session->frame_handler(session);
			break;

		case SPDY_SESSION_STATUS_FLUSHING:

			return SPDY_NO;

		//because of error the session needs to be closed
		case SPDY_SESSION_STATUS_CLOSING:
			//error should be already sent to the client
			SPDYF_session_close(session);
			return SPDY_YES;
	}

	return SPDY_YES;
}


void
SPDYF_session_close (struct SPDY_Session *session)
{
	struct SPDY_Daemon *daemon = session->daemon;
	int by_client = session->read_closed ? SPDY_YES : SPDY_NO;

	//shutdown the tls and deinit the tls context
	session->fio_close_session(session);
	shutdown (session->socket_fd,
		session->read_closed ? SHUT_WR : SHUT_RDWR);
	session->read_closed = true;

	//remove session from the list
	DLL_remove (daemon->sessions_head,
		daemon->sessions_tail,
		session);
	//add the session for the list for cleaning up
	DLL_insert (daemon->cleanup_head,
		daemon->cleanup_tail,
		session);

	//call callback for closed session
	if(NULL != daemon->session_closed_cb)
	{
		daemon->session_closed_cb(daemon->cls, session, by_client);
	}
}


int
SPDYF_session_accept(struct SPDY_Daemon *daemon)
{
	int new_socket_fd;
  int ret;
	struct SPDY_Session *session = NULL;
	socklen_t addr_len;
	struct sockaddr *addr;

#if HAVE_INET6
	struct sockaddr_in6 addr6;

	addr = (struct sockaddr *)&addr6;
	addr_len = sizeof(addr6);
#else
	struct sockaddr_in addr4;

	addr = (struct sockaddr *)&addr4;
	addr_len = sizeof(addr6);
#endif

  new_socket_fd = accept (daemon->socket_fd, addr, &addr_len);

  if(new_socket_fd < 1)
		return SPDY_NO;

	if (NULL == (session = malloc (sizeof (struct SPDY_Session))))
  {
		goto free_and_fail;
	}
	memset (session, 0, sizeof (struct SPDY_Session));

	session->daemon = daemon;
	session->socket_fd = new_socket_fd;
  session->max_num_frames = daemon->max_num_frames;

  ret = SPDYF_io_set_session(session, daemon->io_subsystem);
  SPDYF_ASSERT(SPDY_YES == ret, "Somehow daemon->io_subsystem iswrong here");

	//init TLS context, handshake will be done
	if(SPDY_YES != session->fio_new_session(session))
	{
		goto free_and_fail;
	}

	//read buffer
	session->read_buffer_size = SPDYF_BUFFER_SIZE;
	if (NULL == (session->read_buffer = malloc (session->read_buffer_size)))
    {
		session->fio_close_session(session);
		goto free_and_fail;
	}

	//address of the client
	if (NULL == (session->addr = malloc (addr_len)))
    {
		session->fio_close_session(session);
		goto free_and_fail;
	}
	memcpy (session->addr, addr, addr_len);

	session->addr_len = addr_len;
	session->status = SPDY_SESSION_STATUS_WAIT_FOR_HEADER;

	//init zlib context for the whole session
	if(SPDY_YES != SPDYF_zlib_deflate_init(&session->zlib_send_stream))
    {
		session->fio_close_session(session);
		goto free_and_fail;
	}
	if(SPDY_YES != SPDYF_zlib_inflate_init(&session->zlib_recv_stream))
    {
		session->fio_close_session(session);
		SPDYF_zlib_deflate_end(&session->zlib_send_stream);
		goto free_and_fail;
	}

	//add it to daemon's list
	DLL_insert(daemon->sessions_head,daemon->sessions_tail,session);

	session->last_activity = SPDYF_monotonic_time();

	if(NULL != daemon->new_session_cb)
		daemon->new_session_cb(daemon->cls, session);

	return SPDY_YES;

	//for GOTO
	free_and_fail:
	/* something failed, so shutdown, close and free memory */
	shutdown (new_socket_fd, SHUT_RDWR);
	(void)close (new_socket_fd);

	if(NULL != session)
	{
		if(NULL != session->addr)
			free (session->addr);
		if(NULL != session->read_buffer)
			free (session->read_buffer);
		free (session);
	}
	return SPDY_NO;
}


void
SPDYF_queue_response (struct SPDYF_Response_Queue *response_to_queue,
						struct SPDY_Session *session,
						int consider_priority)
{
	struct SPDYF_Response_Queue *pos;
	struct SPDYF_Response_Queue *last;
	uint8_t priority;

	SPDYF_ASSERT(SPDY_YES != consider_priority || NULL != response_to_queue->stream,
		"called with consider_priority but no stream provided");

	last = response_to_queue;
	while(NULL != last->next)
	{
		last = last->next;
	}

	if(SPDY_NO == consider_priority)
	{
		//put it at the end of the queue
		response_to_queue->prev = session->response_queue_tail;
		if (NULL == session->response_queue_head)
			session->response_queue_head = response_to_queue;
		else
			session->response_queue_tail->next = response_to_queue;
		session->response_queue_tail = last;
		return;
	}
	else if(-1 == consider_priority)
	{
		//put it at the head of the queue
		last->next = session->response_queue_head;
		if (NULL == session->response_queue_tail)
			session->response_queue_tail = last;
		else
			session->response_queue_head->prev = response_to_queue;
		session->response_queue_head = response_to_queue;
		return;
	}

	if(NULL == session->response_queue_tail)
	{
		session->response_queue_head = response_to_queue;
		session->response_queue_tail = last;
		return;
	}

	//search for the right position to put it
	pos = session->response_queue_tail;
	priority = response_to_queue->stream->priority;
	while(NULL != pos
		&& pos->stream->priority > priority)
	{
		pos = pos->prev;
	}

	if(NULL == pos)
	{
		//put it on the head
		session->response_queue_head->prev = last;
		last->next = session->response_queue_head;
		session->response_queue_head = response_to_queue;
	}
	else if(NULL == pos->next)
	{
		//put it at the end
		response_to_queue->prev = pos;
		pos->next = response_to_queue;
		session->response_queue_tail = last;
	}
	else
	{
		response_to_queue->prev = pos;
		last->next = pos->next;
		pos->next = response_to_queue;
		last->next->prev = last;
	}
}


void
SPDYF_session_destroy(struct SPDY_Session *session)
{
	struct SPDYF_Stream *stream;
	struct SPDYF_Response_Queue *response_queue;

	(void)close (session->socket_fd);
	SPDYF_zlib_deflate_end(&session->zlib_send_stream);
	SPDYF_zlib_inflate_end(&session->zlib_recv_stream);

	//clean up unsent data in the output queue
	while (NULL != (response_queue = session->response_queue_head))
	{
		DLL_remove (session->response_queue_head,
			session->response_queue_tail,
			response_queue);

		if(NULL != response_queue->frqcb)
		{
			response_queue->frqcb(response_queue->frqcb_cls, response_queue, SPDY_RESPONSE_RESULT_SESSION_CLOSED);
		}

		SPDYF_response_queue_destroy(response_queue);
	}

	//clean up the streams belonging to this session
	while (NULL != (stream = session->streams_head))
	{
		DLL_remove (session->streams_head,
			session->streams_tail,
			stream);

		SPDYF_stream_destroy(stream);
	}

	free(session->addr);
	free(session->read_buffer);
	free(session->write_buffer);
	free(session);
}


int
SPDYF_prepare_goaway (struct SPDY_Session *session,
					enum SPDY_GOAWAY_STATUS status,
					bool in_front)
{
	struct SPDYF_Response_Queue *response_to_queue;
	struct SPDYF_Control_Frame *control_frame;
	uint32_t *data;

	if(NULL == (response_to_queue = malloc(sizeof(struct SPDYF_Response_Queue))))
	{
		return SPDY_NO;
	}
	memset(response_to_queue, 0, sizeof(struct SPDYF_Response_Queue));

	if(NULL == (control_frame = malloc(sizeof(struct SPDYF_Control_Frame))))
	{
		free(response_to_queue);
		return SPDY_NO;
	}
	memset(control_frame, 0, sizeof(struct SPDYF_Control_Frame));

	if(NULL == (data = malloc(4)))
	{
		free(control_frame);
		free(response_to_queue);
		return SPDY_NO;
	}
	*(data) = htonl(status);

	control_frame->control_bit = 1;
	control_frame->version = SPDY_VERSION;
	control_frame->type = SPDY_CONTROL_FRAME_TYPES_GOAWAY;
	control_frame->flags = 0;

	response_to_queue->control_frame = control_frame;
	response_to_queue->process_response_handler = &SPDYF_handler_write_goaway;
	response_to_queue->data = data;
	response_to_queue->data_size = 4;

	SPDYF_queue_response (response_to_queue,
						session,
						in_front ? -1 : SPDY_NO);

	return SPDY_YES;
}


int
SPDYF_prepare_rst_stream (struct SPDY_Session *session,
					struct SPDYF_Stream * stream,
					enum SPDY_RST_STREAM_STATUS status)
{
	struct SPDYF_Response_Queue *response_to_queue;
	struct SPDYF_Control_Frame *control_frame;
	uint32_t *data;
	uint32_t stream_id;

  if(NULL == stream)
    stream_id = 0;
  else
    stream_id = stream->stream_id;

	if(NULL == (response_to_queue = malloc(sizeof(struct SPDYF_Response_Queue))))
	{
		return SPDY_NO;
	}
	memset(response_to_queue, 0, sizeof(struct SPDYF_Response_Queue));

	if(NULL == (control_frame = malloc(sizeof(struct SPDYF_Control_Frame))))
	{
		free(response_to_queue);
		return SPDY_NO;
	}
	memset(control_frame, 0, sizeof(struct SPDYF_Control_Frame));

	if(NULL == (data = malloc(8)))
	{
		free(control_frame);
		free(response_to_queue);
		return SPDY_NO;
	}
	*(data) = HTON31(stream_id);
	*(data + 1) = htonl(status);

	control_frame->control_bit = 1;
	control_frame->version = SPDY_VERSION;
	control_frame->type = SPDY_CONTROL_FRAME_TYPES_RST_STREAM;
	control_frame->flags = 0;

	response_to_queue->control_frame = control_frame;
	response_to_queue->process_response_handler = &SPDYF_handler_write_rst_stream;
	response_to_queue->data = data;
	response_to_queue->data_size = 8;
	response_to_queue->stream = stream;

	SPDYF_queue_response (response_to_queue,
						session,
						-1);

	return SPDY_YES;
}


int
SPDYF_prepare_window_update (struct SPDY_Session *session,
					struct SPDYF_Stream * stream,
					int32_t delta_window_size)
{
	struct SPDYF_Response_Queue *response_to_queue;
	struct SPDYF_Control_Frame *control_frame;
	uint32_t *data;

  SPDYF_ASSERT(NULL != stream, "stream cannot be NULL");

	if(NULL == (response_to_queue = malloc(sizeof(struct SPDYF_Response_Queue))))
	{
		return SPDY_NO;
	}
	memset(response_to_queue, 0, sizeof(struct SPDYF_Response_Queue));

	if(NULL == (control_frame = malloc(sizeof(struct SPDYF_Control_Frame))))
	{
		free(response_to_queue);
		return SPDY_NO;
	}
	memset(control_frame, 0, sizeof(struct SPDYF_Control_Frame));

	if(NULL == (data = malloc(8)))
	{
		free(control_frame);
		free(response_to_queue);
		return SPDY_NO;
	}
	*(data) = HTON31(stream->stream_id);
	*(data + 1) = HTON31(delta_window_size);

	control_frame->control_bit = 1;
	control_frame->version = SPDY_VERSION;
	control_frame->type = SPDY_CONTROL_FRAME_TYPES_WINDOW_UPDATE;
	control_frame->flags = 0;

	response_to_queue->control_frame = control_frame;
	response_to_queue->process_response_handler = &SPDYF_handler_write_window_update;
	response_to_queue->data = data;
	response_to_queue->data_size = 8;
	response_to_queue->stream = stream;

	SPDYF_queue_response (response_to_queue,
						session,
						-1);

	return SPDY_YES;
}
