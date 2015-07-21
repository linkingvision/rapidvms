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
 * @file struct_namevalue.c
 * @brief  tests all the API functions for handling struct SPDY_NameValue
 * @author Andrey Uzunov
 */

#include "platform.h"
#include "microspdy.h"
#include "common.h"
#include "../microspdy/structures.h"
#include "../microspdy/alstructures.h"

char *pairs[] = {"one","1","two","2","three","3","four","4","five","5"};
char *pairs_with_dups[] = {"one","1","two","2","one","11","two","22","three","3","two","222","two","2222","four","","five","5"};//82
char *pairs_with_empty[] = {"name","","name","value"};
char *pairs_different[] = {"30","thirty","40","fouthy"};
int size;
int size2;
int brake_at = 3;
bool flag;


int
iterate_cb (void *cls, const char *name, const char * const * value, int num_values)
{
	int *c = (int*)cls;

	if(*c < 0 || *c > size)
		exit(11);

	if(strcmp(name,pairs[*c]) != 0)
	{
		FAIL_TEST("name is wrong\n");
	}

	if(1 != num_values)
	{
		FAIL_TEST("num_values is wrong\n");
	}

	if(strcmp(value[0],pairs[(*c)+1]) != 0)
	{
		FAIL_TEST("value is wrong\n");
	}

	(*c)+=2;

	return SPDY_YES;
}

int
iterate_brake_cb (void *cls, const char *name, const char * const *value, int num_values)
{
  (void)name;
  (void)value;
  (void)num_values;

	int *c = (int*)cls;

	if(*c < 0 || *c >= brake_at)
	{
		FAIL_TEST("iteration was not interrupted\n");
	}

	(*c)++;

	if(*c == brake_at) return SPDY_NO;

	return SPDY_YES;
}

int
main()
{
	SPDY_init();

	const char *const*value;
	const char *const*value2;
	int i;
	int j;
	int cls = 0;
	int ret;
	int ret2;
	void *ob1;
	void *ob2;
	void *ob3;
	void *stream;
	char data[] = "anything";
	struct SPDY_NameValue *container;
	struct SPDY_NameValue *container2;
	struct SPDY_NameValue *container3;
	struct SPDY_NameValue *container_arr[2];

	size = sizeof(pairs)/sizeof(pairs[0]);

	if(NULL == (container = SPDY_name_value_create ()))
	{
		FAIL_TEST("SPDY_name_value_create failed\n");
	}

	if(NULL != SPDY_name_value_lookup (container, "anything", &ret))
	{
		FAIL_TEST("SPDY_name_value_lookup failed\n");
	}

	if(SPDY_name_value_iterate (container, NULL, NULL) != 0)
	{
		FAIL_TEST("SPDY_name_value_iterate failed\n");
	}

	for(i=0;i<size; i+=2)
	{
		if(SPDY_YES != SPDY_name_value_add(container,pairs[i],pairs[i+1]))
		{
			FAIL_TEST("SPDY_name_value_add failed\n");
		}

		if(SPDY_name_value_iterate (container, NULL, NULL) != ((i / 2) + 1))
		{
			FAIL_TEST("SPDY_name_value_iterate failed\n");
		}
	}

	if(NULL != SPDY_name_value_lookup (container, "anything", &ret))
	{
		FAIL_TEST("SPDY_name_value_lookup failed\n");
	}

	for(i=size - 2; i >= 0; i-=2)
	{
		value = SPDY_name_value_lookup(container,pairs[i], &ret);
		if(NULL == value || 1 !=ret || strcmp(value[0], pairs[i+1]) != 0)
		{
			printf("%p; %i; %i\n", value, ret,
                               (NULL == value) ? -1 : strcmp(value[0], pairs[i+1]));
			FAIL_TEST("SPDY_name_value_lookup failed\n");
		}
	}

	SPDY_name_value_iterate (container, &iterate_cb, &cls);

	cls = 0;
	if(SPDY_name_value_iterate (container, &iterate_brake_cb, &cls) != brake_at)
	{
		FAIL_TEST("SPDY_name_value_iterate with brake failed\n");
	}

	SPDY_name_value_destroy(container);

	//check everything with NULL values
	for(i=0; i<7; ++i)
	{
		printf("%i ",i);
		ob1 = (i & 4) ? data : NULL;
		ob2 = (i & 2) ? data : NULL;
		ob3 = (i & 1) ? data : NULL;
		if(SPDY_INPUT_ERROR != SPDY_name_value_add(ob1,ob2,ob3))
		{
			FAIL_TEST("SPDY_name_value_add with NULLs failed\n");
		}
	}
	printf("\n");
	fflush(stdout);

	if(SPDY_INPUT_ERROR != SPDY_name_value_iterate(NULL,NULL,NULL))
	{
		FAIL_TEST("SPDY_name_value_iterate with NULLs failed\n");
	}

	for(i=0; i<7; ++i)
	{
		printf("%i ",i);
		ob1 = (i & 4) ? data : NULL;
		ob2 = (i & 2) ? data : NULL;
		ob3 = (i & 1) ? data : NULL;
		if(NULL != SPDY_name_value_lookup(ob1,ob2,ob3))
		{
			FAIL_TEST("SPDY_name_value_lookup with NULLs failed\n");
		}
	}
	printf("\n");
	SPDY_name_value_destroy(NULL);

	if(NULL == (container = SPDY_name_value_create ()))
	{
		FAIL_TEST("SPDY_name_value_create failed\n");
	}

	size = sizeof(pairs_with_dups)/sizeof(pairs_with_dups[0]);

	for(i=0;i<size; i+=2)
	{
		if(SPDY_YES != SPDY_name_value_add(container,pairs_with_dups[i],pairs_with_dups[i+1]))
		{
			FAIL_TEST("SPDY_name_value_add failed\n");
		}
	}
	if(SPDY_name_value_iterate (container, NULL, NULL) != atoi(pairs_with_dups[size - 1]))
	{
		FAIL_TEST("SPDY_name_value_iterate failed\n");
	}
	for(i=size - 2; i >= 0; i-=2)
	{
		value = SPDY_name_value_lookup(container,pairs_with_dups[i], &ret);
		if(NULL == value)
		{
			FAIL_TEST("SPDY_name_value_lookup failed\n");
		}
		flag = false;
		for(j=0; j<ret; ++j)
			if(0 == strcmp(pairs_with_dups[i + 1], value[j]))
			{
				if(flag)
					FAIL_TEST("SPDY_name_value_lookup failed\n");
				flag=true;
			}

		if(!flag)
			FAIL_TEST("SPDY_name_value_lookup failed\n");
	}
	if(SPDY_NO != SPDY_name_value_add(container,pairs_with_dups[0],pairs_with_dups[1]))
		FAIL_TEST("SPDY_name_value_add failed\n");

	SPDY_name_value_destroy(container);

	if(NULL == (container = SPDY_name_value_create ()))
	{
		FAIL_TEST("SPDY_name_value_create failed\n");
	}

	size = sizeof(pairs_with_empty)/sizeof(pairs_with_empty[0]);

	for(i=0;i<size; i+=2)
	{
		if(SPDY_YES != SPDY_name_value_add(container,pairs_with_empty[i],pairs_with_empty[i+1]))
		{
			FAIL_TEST("SPDY_name_value_add failed\n");
		}
		value = SPDY_name_value_lookup(container,pairs_with_empty[i], &ret);
		if(NULL == value || 1 != ret)
		{
			printf("%p; %i\n", value, ret);
			FAIL_TEST("SPDY_name_value_lookup failed\n");
		}
	}

	ret = SPDY_name_value_iterate(container, NULL, NULL);
	if(SPDY_INPUT_ERROR != SPDY_name_value_add(container, "capitalLeter","anything")
		|| SPDY_name_value_iterate(container, NULL, NULL) != ret)
	{
		FAIL_TEST("SPDY_name_value_add failed\n");
	}

	SPDY_name_value_destroy(container);

	if(NULL == (container = SPDY_name_value_create ()))
	{
		FAIL_TEST("SPDY_name_value_create failed\n");
	}

	size = sizeof(pairs_with_dups)/sizeof(pairs_with_dups[0]);

	for(i=0;i<size; i+=2)
	{
		if(SPDY_YES != SPDY_name_value_add(container,pairs_with_dups[i],pairs_with_dups[i+1]))
		{
			FAIL_TEST("SPDY_name_value_add failed\n");
		}
	}

	if(NULL == (container2 = SPDY_name_value_create ()))
	{
		FAIL_TEST("SPDY_name_value_create failed\n");
	}

	size2 = sizeof(pairs_different)/sizeof(pairs_different[0]);

	for(i=0;i<size2; i+=2)
	{
		if(SPDY_YES != SPDY_name_value_add(container2,pairs_different[i],pairs_different[i+1]))
		{
			FAIL_TEST("SPDY_name_value_add failed\n");
		}
	}

	container_arr[0] = container;
	container_arr[1] = container2;
	if(0 > (ret = SPDYF_name_value_to_stream(container_arr, 2, &stream)) || NULL == stream)
		FAIL_TEST("SPDYF_name_value_to_stream failed\n");
	ret = SPDYF_name_value_from_stream(stream, ret, &container3);
	if(SPDY_YES != ret)
		FAIL_TEST("SPDYF_name_value_from_stream failed\n");

	if(SPDY_name_value_iterate(container3, NULL, NULL)
		!= (SPDY_name_value_iterate(container, NULL, NULL) + SPDY_name_value_iterate(container2, NULL, NULL)))
		FAIL_TEST("SPDYF_name_value_from_stream failed\n");

	for(i=size - 2; i >= 0; i-=2)
	{
		value = SPDY_name_value_lookup(container,pairs_with_dups[i], &ret);
		if(NULL == value)
			FAIL_TEST("SPDY_name_value_lookup failed\n");
		value2 = SPDY_name_value_lookup(container3,pairs_with_dups[i], &ret2);
		if(NULL == value2)
			FAIL_TEST("SPDY_name_value_lookup failed\n");

		for(j=0; j<ret; ++j)
			if(0 != strcmp(value2[j], value[j]))
				FAIL_TEST("SPDY_name_value_lookup failed\n");
	}
	for(i=size2 - 2; i >= 0; i-=2)
	{
		value = SPDY_name_value_lookup(container2,pairs_different[i], &ret);
		if(NULL == value)
			FAIL_TEST("SPDY_name_value_lookup failed\n");
		value2 = SPDY_name_value_lookup(container3,pairs_different[i], &ret2);
		if(NULL == value2)
			FAIL_TEST("SPDY_name_value_lookup failed\n");

		for(j=0; j<ret; ++j)
			if(0 != strcmp(value2[j], value[j]))
				FAIL_TEST("SPDY_name_value_lookup failed\n");
	}

	SPDY_deinit();

	return 0;
}
