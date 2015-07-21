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
 * @file stream.c
 * @brief  SPDY streams handling
 * @author Andrey Uzunov
 */

#include "platform.h"
#include "structures.h"
#include "internal.h"
#include "session.h"


int
SPDYF_stream_new (struct SPDY_Session *session)
{
	uint32_t stream_id;
	uint32_t assoc_stream_id;
	uint8_t priority;
	uint8_t slot;
	size_t buffer_pos = session->read_buffer_beginning;
	struct SPDYF_Stream *stream;
	struct SPDYF_Control_Frame *frame;
	
	if((session->read_buffer_offset - session->read_buffer_beginning) < 10)
	{
		//not all fields are received to create new stream
		return SPDY_NO;
	}
	
	frame = (struct SPDYF_Control_Frame *)session->frame_handler_cls;
	
	//get stream id of the new stream
    memcpy(&stream_id, session->read_buffer + session->read_buffer_beginning, 4);
	stream_id = NTOH31(stream_id);
	session->read_buffer_beginning += 4;
	if(stream_id <= session->last_in_stream_id || 0==(stream_id % 2))
	{
		//wrong stream id sent by client
		//GOAWAY with PROTOCOL_ERROR MUST be sent
		//TODO
		
		//ignore frame
		session->frame_handler = &SPDYF_handler_ignore_frame;
		return SPDY_NO;
	}
	else if(session->is_goaway_sent)
	{
		//the client is not allowed to create new streams anymore
		//we MUST ignore the frame
		session->frame_handler = &SPDYF_handler_ignore_frame;
		return SPDY_NO;
	}
	
	//set highest stream id for session
	session->last_in_stream_id = stream_id;
	
	//get assoc stream id of the new stream
	//this value is used with SPDY PUSH, thus nothing to do with it here
    memcpy(&assoc_stream_id, session->read_buffer + session->read_buffer_beginning, 4);
	assoc_stream_id = NTOH31(assoc_stream_id);
	session->read_buffer_beginning += 4;

	//get stream priority (3 bits)
	//after it there are 5 bits that are not used
	priority = *(uint8_t *)(session->read_buffer + session->read_buffer_beginning) >> 5;
	session->read_buffer_beginning++;
	
	//get slot (see SPDY draft)
	slot = *(uint8_t *)(session->read_buffer + session->read_buffer_beginning);
	session->read_buffer_beginning++;
	
	if(NULL == (stream = malloc(sizeof(struct SPDYF_Stream))))
	{
		SPDYF_DEBUG("No memory");
		//revert buffer state
		session->read_buffer_beginning = buffer_pos;
		return SPDY_NO;
	}
	memset(stream,0, sizeof(struct SPDYF_Stream));
	stream->session = session;
	stream->stream_id = stream_id;
	stream->assoc_stream_id = assoc_stream_id;
	stream->priority = priority;
	stream->slot = slot;
	stream->is_in_closed = (frame->flags & SPDY_SYN_STREAM_FLAG_FIN) != 0;
	stream->flag_unidirectional = (frame->flags & SPDY_SYN_STREAM_FLAG_UNIDIRECTIONAL) != 0;
	stream->is_out_closed = stream->flag_unidirectional;
	stream->is_server_initiator = false;
	stream->window_size = SPDYF_INITIAL_WINDOW_SIZE;
	
	//put the stream to the list of streams for the session
	DLL_insert(session->streams_head, session->streams_tail, stream);
	
	return SPDY_YES;
}


void
SPDYF_stream_destroy(struct SPDYF_Stream *stream)
{
	SPDY_name_value_destroy(stream->headers);
	free(stream);
	stream = NULL;
}


void
SPDYF_stream_set_flags_on_write(struct SPDYF_Response_Queue *response_queue)
{
	struct SPDYF_Stream * stream = response_queue->stream;
	
	if(NULL != response_queue->data_frame)
	{
		stream->is_out_closed = (bool)(response_queue->data_frame->flags & SPDY_DATA_FLAG_FIN);
	}
	else if(NULL != response_queue->control_frame)
	{
		switch(response_queue->control_frame->type)
		{
			case SPDY_CONTROL_FRAME_TYPES_SYN_REPLY:
				stream->is_out_closed = (bool)(response_queue->control_frame->flags & SPDY_SYN_REPLY_FLAG_FIN);
				break;
				
			case SPDY_CONTROL_FRAME_TYPES_RST_STREAM:
				if(NULL != stream)
				{
					stream->is_out_closed = true;
					stream->is_in_closed = true;
				}
				break;
				
		}
	}
}


//TODO add function *on_read


struct SPDYF_Stream * 
SPDYF_stream_find(uint32_t stream_id, struct SPDY_Session * session)
{
  struct SPDYF_Stream * stream = session->streams_head;
  
  while(NULL != stream && stream_id != stream->stream_id)
  {
    stream = stream->next;
  }
  
  return stream;
}
