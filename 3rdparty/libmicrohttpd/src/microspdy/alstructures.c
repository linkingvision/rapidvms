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
 * @file alstructures.c
 * @brief  structures only for the application layer
 * @author Andrey Uzunov
 */
 
#include "platform.h"
#include "alstructures.h"
#include "internal.h"

void
SPDY_destroy_request (struct SPDY_Request *request)
{
	if(NULL == request)
	{
		SPDYF_DEBUG("request is NULL");
		return;
	}
	//strings into request struct are just references to strings in
	//headers, so no need to free them twice
	SPDY_name_value_destroy(request->headers);
	free(request);
}
