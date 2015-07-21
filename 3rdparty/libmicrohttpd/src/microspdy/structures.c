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
 * @file structures.c
 * @brief  Functions for handling most of the structures in defined
 * 			in structures.h
 * @author Andrey Uzunov
 */

#include "platform.h"
#include "structures.h"
#include "internal.h"
#include "session.h"
//TODO not for here?
#include <ctype.h>


int
SPDYF_name_value_is_empty(struct SPDY_NameValue *container)
{
  SPDYF_ASSERT(NULL != container, "NULL is not an empty container!");
  return (NULL == container->name && NULL == container->value) ? SPDY_YES : SPDY_NO;
}

struct SPDY_NameValue *
SPDY_name_value_create ()
{
	struct SPDY_NameValue *pair;

	if(NULL == (pair = malloc(sizeof(struct SPDY_NameValue))))
		return NULL;

	memset (pair, 0, sizeof (struct SPDY_NameValue));

	return pair;
}


int
SPDY_name_value_add (struct SPDY_NameValue *container,
					const char *name,
					const char *value)
{
	unsigned int i;
	unsigned int len;
	struct SPDY_NameValue *pair;
	struct SPDY_NameValue *temp;
	char **temp_value;
	char *temp_string;

	if(NULL == container || NULL == name || NULL == value || 0 == (len = strlen(name)))
		return SPDY_INPUT_ERROR;
  //TODO there is old code handling value==NULL
  //update it to handle strlen(value)==0

	for(i=0; i<len; ++i)
	{
	  if(isupper((int) name[i]))
			return SPDY_INPUT_ERROR;
	}

	if(SPDYF_name_value_is_empty(container))
	{
		//container is empty/just created
		if (NULL == (container->name = strdup (name)))
		{
			return SPDY_NO;
		}
		if (NULL == (container->value = malloc(sizeof(char *))))
		{
			free(container->name);
			return SPDY_NO;
		}
    /*if(NULL == value)
      container->value[0] = NULL;
		else */if (NULL == (container->value[0] = strdup (value)))
		{
			free(container->value);
			free(container->name);
			return SPDY_NO;
		}
		container->num_values = 1;
		return SPDY_YES;
	}

	pair = container;
	while(NULL != pair)
	{
		if(0 == strcmp(pair->name, name))
		{
			//the value will be added to this pair
			break;
		}
		pair = pair->next;
	}

	if(NULL == pair)
	{
		//the name doesn't exist in container, add new pair
		if(NULL == (pair = malloc(sizeof(struct SPDY_NameValue))))
			return SPDY_NO;

		memset(pair, 0, sizeof(struct SPDY_NameValue));

		if (NULL == (pair->name = strdup (name)))
		{
			free(pair);
			return SPDY_NO;
		}
		if (NULL == (pair->value = malloc(sizeof(char *))))
		{
			free(pair->name);
			free(pair);
			return SPDY_NO;
		}
    /*if(NULL == value)
      pair->value[0] = NULL;
		else */if (NULL == (pair->value[0] = strdup (value)))
		{
			free(pair->value);
			free(pair->name);
			free(pair);
			return SPDY_NO;
		}
		pair->num_values = 1;

		temp = container;
		while(NULL != temp->next)
			temp = temp->next;
		temp->next = pair;
		pair->prev = temp;

		return SPDY_YES;
	}

	//check for duplication (case sensitive)
	for(i=0; i<pair->num_values; ++i)
		if(0 == strcmp(pair->value[i], value))
			return SPDY_NO;

	if(strlen(pair->value[0]) > 0)
	{
		//the value will be appended to the others for this name
		if (NULL == (temp_value = malloc((pair->num_values + 1) * sizeof(char *))))
		{
			return SPDY_NO;
		}
		memcpy(temp_value, pair->value, pair->num_values * sizeof(char *));
		if (NULL == (temp_value[pair->num_values] = strdup (value)))
		{
			free(temp_value);
			return SPDY_NO;
		}
		free(pair->value);
		pair->value = temp_value;
		++pair->num_values;
		return SPDY_YES;
	}

	//just replace the empty value

	if (NULL == (temp_string = strdup (value)))
	{
		return SPDY_NO;
	}
	free(pair->value[0]);
	pair->value[0] = temp_string;

	return SPDY_YES;
}


const char * const *
SPDY_name_value_lookup (struct SPDY_NameValue *container,
						const char *name,
						int *num_values)
{
	struct SPDY_NameValue *temp = container;

	if(NULL == container || NULL == name || NULL == num_values)
		return NULL;
	if(SPDYF_name_value_is_empty(container))
		return NULL;

	do
	{
		if(strcmp(name, temp->name) == 0)
		{
			*num_values = temp->num_values;
			return (const char * const *)temp->value;
		}

		temp = temp->next;
	}
	while(NULL != temp);

	return NULL;
}


void
SPDY_name_value_destroy (struct SPDY_NameValue *container)
{
	unsigned int i;
	struct SPDY_NameValue *temp = container;

	while(NULL != temp)
	{
		container = container->next;
		free(temp->name);
		for(i=0; i<temp->num_values; ++i)
			free(temp->value[i]);
		free(temp->value);
		free(temp);
		temp=container;
	}
}


int
SPDY_name_value_iterate (struct SPDY_NameValue *container,
                           SPDY_NameValueIterator iterator,
                           void *iterator_cls)
{
	int count;
	int ret;
	struct SPDY_NameValue *temp = container;

	if(NULL == container)
		return SPDY_INPUT_ERROR;

	//check if container is an empty struct
	if(SPDYF_name_value_is_empty(container))
		return 0;

	count = 0;

	if(NULL == iterator)
	{
		do
		{
			++count;
			temp=temp->next;
		}
		while(NULL != temp);

		return count;
	}

	//code duplication for avoiding if here
	do
	{
		++count;
		ret = iterator(iterator_cls, temp->name, (const char * const *)temp->value, temp->num_values);
		temp=temp->next;
	}
	while(NULL != temp && SPDY_YES == ret);

	return count;
}

void
SPDY_destroy_response(struct SPDY_Response *response)
{
  if(NULL == response)
    return;
	free(response->data);
	free(response->headers);
	free(response);
}


struct SPDYF_Response_Queue *
SPDYF_response_queue_create(bool is_data,
						void *data,
						size_t data_size,
						struct SPDY_Response *response,
						struct SPDYF_Stream *stream,
						bool closestream,
						SPDYF_ResponseQueueResultCallback frqcb,
						void *frqcb_cls,
						SPDY_ResponseResultCallback rrcb,
						void *rrcb_cls)
{
	struct SPDYF_Response_Queue *head = NULL;
	struct SPDYF_Response_Queue *prev;
	struct SPDYF_Response_Queue *response_to_queue;
	struct SPDYF_Control_Frame *control_frame;
	struct SPDYF_Data_Frame *data_frame;
	unsigned int i;
	bool is_last;

	SPDYF_ASSERT((! is_data)
		     || ((0 == data_size) && (NULL != response->rcb))
		     || ((0 < data_size) && (NULL == response->rcb)),
		     "either data or request->rcb must not be null");

	if (is_data && (data_size > SPDY_MAX_SUPPORTED_FRAME_SIZE))
	{
		//separate the data in more frames and add them to the queue

		prev=NULL;
		for(i = 0; i < data_size; i += SPDY_MAX_SUPPORTED_FRAME_SIZE)
		{
			is_last = (i + SPDY_MAX_SUPPORTED_FRAME_SIZE) >= data_size;

			if(NULL == (response_to_queue = malloc(sizeof(struct SPDYF_Response_Queue))))
				goto free_and_fail;

			memset(response_to_queue, 0, sizeof(struct SPDYF_Response_Queue));
			if(0 == i)
				head = response_to_queue;

			if(NULL == (data_frame = malloc(sizeof(struct SPDYF_Data_Frame))))
			{
				free(response_to_queue);
				goto free_and_fail;
			}
			memset(data_frame, 0, sizeof(struct SPDYF_Data_Frame));
			data_frame->control_bit = 0;
			data_frame->stream_id = stream->stream_id;
			if(is_last && closestream)
				data_frame->flags |= SPDY_DATA_FLAG_FIN;

			response_to_queue->data_frame = data_frame;
			response_to_queue->process_response_handler = &SPDYF_handler_write_data;
			response_to_queue->is_data = is_data;
			response_to_queue->stream = stream;
			if(is_last)
			{
				response_to_queue->frqcb = frqcb;
				response_to_queue->frqcb_cls = frqcb_cls;
				response_to_queue->rrcb = rrcb;
				response_to_queue->rrcb_cls = rrcb_cls;
			}
			response_to_queue->data = data + i;
			response_to_queue->data_size = is_last
				? (data_size - 1) % SPDY_MAX_SUPPORTED_FRAME_SIZE + 1
				: SPDY_MAX_SUPPORTED_FRAME_SIZE;
			response_to_queue->response = response;

			response_to_queue->prev = prev;
			if(NULL != prev)
				prev->next = response_to_queue;
			prev = response_to_queue;
		}

		return head;

		//for GOTO
		free_and_fail:
		while(NULL != head)
		{
			response_to_queue = head;
			head = head->next;
			free(response_to_queue->data_frame);
			free(response_to_queue);
		}
		return NULL;
	}

	//create only one frame for data, data with callback or control frame

	if(NULL == (response_to_queue = malloc(sizeof(struct SPDYF_Response_Queue))))
	{
		return NULL;
	}
	memset(response_to_queue, 0, sizeof(struct SPDYF_Response_Queue));

	if(is_data)
	{
		if(NULL == (data_frame = malloc(sizeof(struct SPDYF_Data_Frame))))
		{
			free(response_to_queue);
			return NULL;
		}
		memset(data_frame, 0, sizeof(struct SPDYF_Data_Frame));
		data_frame->control_bit = 0;
		data_frame->stream_id = stream->stream_id;
		if(closestream && NULL == response->rcb)
			data_frame->flags |= SPDY_DATA_FLAG_FIN;

		response_to_queue->data_frame = data_frame;
		response_to_queue->process_response_handler = &SPDYF_handler_write_data;
	}
	else
	{
		if(NULL == (control_frame = malloc(sizeof(struct SPDYF_Control_Frame))))
		{
			free(response_to_queue);
			return NULL;
		}
		memset(control_frame, 0, sizeof(struct SPDYF_Control_Frame));
		control_frame->control_bit = 1;
		control_frame->version = SPDY_VERSION;
		control_frame->type = SPDY_CONTROL_FRAME_TYPES_SYN_REPLY;
		if(closestream)
			control_frame->flags |= SPDY_SYN_REPLY_FLAG_FIN;

		response_to_queue->control_frame = control_frame;
		response_to_queue->process_response_handler = &SPDYF_handler_write_syn_reply;
	}

	response_to_queue->is_data = is_data;
	response_to_queue->stream = stream;
	response_to_queue->frqcb = frqcb;
	response_to_queue->frqcb_cls = frqcb_cls;
	response_to_queue->rrcb = rrcb;
	response_to_queue->rrcb_cls = rrcb_cls;
	response_to_queue->data = data;
	response_to_queue->data_size = data_size;
	response_to_queue->response = response;

	return response_to_queue;
}


void
SPDYF_response_queue_destroy(struct SPDYF_Response_Queue *response_queue)
{
	//data is not copied to the struct but only linked
	//but this is not valid for GOAWAY and RST_STREAM
	if(!response_queue->is_data
		&& (SPDY_CONTROL_FRAME_TYPES_RST_STREAM == response_queue->control_frame->type
		|| SPDY_CONTROL_FRAME_TYPES_GOAWAY == response_queue->control_frame->type))
	{
		free(response_queue->data);
	}
	if(response_queue->is_data)
		free(response_queue->data_frame);
	else
		free(response_queue->control_frame);

	free(response_queue);
}


/* Needed by testcase to be extern -- should this be
   in the header? */
_MHD_EXTERN ssize_t
SPDYF_name_value_to_stream(struct SPDY_NameValue * container[],
                           int num_containers,
                           void **stream)
{
	size_t size;
	int32_t num_pairs = 0;
	int32_t value_size;
	int32_t name_size;
	int32_t temp;
	unsigned int i;
	unsigned int offset;
	unsigned int value_offset;
	struct SPDY_NameValue * iterator;
	int j;

	size = 4; //for num pairs

	for(j=0; j<num_containers; ++j)
	{
    iterator = container[j];
    while(iterator != NULL)
    {
      ++num_pairs;
      size += 4 + strlen(iterator->name); //length + string

      SPDYF_ASSERT(iterator->num_values>0, "num_values is 0");

      size += 4; //value length

      for(i=0; i<iterator->num_values; ++i)
      {
        //if(NULL == iterator->value[i])
        //  continue;
        size += strlen(iterator->value[i]); // string
        if(i/* || !strlen(iterator->value[i])*/) ++size; //NULL separator
      }

      iterator = iterator->next;
    }
  }

	if(NULL == (*stream = malloc(size)))
	{
		return -1;
	}

	//put num_pairs to the stream
	num_pairs = htonl(num_pairs);
	memcpy(*stream, &num_pairs, 4);
	offset = 4;

	//put all other headers to the stream
	for(j=0; j<num_containers; ++j)
	{
    iterator = container[j];
    while(iterator != NULL)
    {
      name_size = strlen(iterator->name);
      temp = htonl(name_size);
      memcpy(*stream + offset, &temp, 4);
      offset += 4;
      strncpy(*stream + offset, iterator->name, name_size);
      offset += name_size;

      value_offset = offset;
      offset += 4;
      for(i=0; i<iterator->num_values; ++i)
      {
        if(i /*|| !strlen(iterator->value[0])*/)
        {
          memset(*stream + offset, 0, 1);
          ++offset;
          //if(!i) continue;
        }
        //else if(NULL != iterator->value[i])
        //{
          strncpy(*stream + offset, iterator->value[i], strlen(iterator->value[i]));
          offset += strlen(iterator->value[i]);
        //}
      }
      value_size = offset - value_offset - 4;
      value_size = htonl(value_size);
      memcpy(*stream + value_offset, &value_size, 4);

      iterator = iterator->next;
    }
  }

	SPDYF_ASSERT(offset == size,"offset is wrong");

	return size;
}


/* Needed by testcase to be extern -- should this be
   in the header? */
_MHD_EXTERN int
SPDYF_name_value_from_stream(void *stream,
							size_t size,
							struct SPDY_NameValue ** container)
{
	int32_t num_pairs;
	int32_t value_size;
	int32_t name_size;
	int i;
	unsigned int offset = 0;
	unsigned int value_end_offset;
	char *name;
	char *value;

	if(NULL == (*container = SPDY_name_value_create ()))
	{
		return SPDY_NO;
	}

	//get number of pairs
	memcpy(&num_pairs, stream, 4);
	offset = 4;
	num_pairs = ntohl(num_pairs);

	if(num_pairs > 0)
	{
		for(i = 0; i < num_pairs; ++i)
		{
			//get name size
			memcpy(&name_size, stream + offset, 4);
			offset += 4;
			name_size = ntohl(name_size);
			//get name
			if(NULL == (name = strndup(stream + offset, name_size)))
			{
				SPDY_name_value_destroy(*container);
				return SPDY_NO;
			}
			offset+=name_size;

			//get value size
			memcpy(&value_size, stream + offset, 4);
			offset += 4;
			value_size = ntohl(value_size);
			value_end_offset = offset + value_size;
			//get value
			do
			{
				if(NULL == (value = strndup(stream + offset, value_size)))
				{
					free(name);
					SPDY_name_value_destroy(*container);
					return SPDY_NO;
				}
				offset += strlen(value);
				if(offset < value_end_offset)
					++offset; //NULL separator

				//add name/value to the struct
				if(SPDY_YES != SPDY_name_value_add(*container, name, value))
				{
					free(name);
					free(value);
					SPDY_name_value_destroy(*container);
					return SPDY_NO;
				}
				free(value);
			}
			while(offset < value_end_offset);

			free(name);

			if(offset != value_end_offset)
			{
				SPDY_name_value_destroy(*container);
				return SPDY_INPUT_ERROR;
			}
		}
	}

	if(offset == size)
		return SPDY_YES;

	SPDY_name_value_destroy(*container);
	return SPDY_INPUT_ERROR;
}
