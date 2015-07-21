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
 * @file alstructures.h
 * @brief  structures only for the application layer
 * @author Andrey Uzunov
 */

#ifndef ALSTRUCTURES_H
#define ALSTRUCTURES_H

#include "platform.h"


/**
 * Represents a SPDY request.
 */
struct SPDY_Request
{
	/**
	 * SPDY stream in whose context the request was received
	 */
	struct SPDYF_Stream *stream;
	
	/**
	 * Other HTTP headers from the request
	 */
	struct SPDY_NameValue *headers;
	
	/**
	 * HTTP method
	 */
	char *method;
	
	/**
	 * HTTP path
	 */
	char *path;
	
	/**
	 * HTTP version just like in HTTP request/response: 
	 * 			"HTTP/1.0" or "HTTP/1.1" currently
	 */
	char *version;
	
	/**
	 * called host as in HTTP
	 */
	char *host;
	
	/**
	 * The scheme used ("http" or "https")
	 */
	char *scheme;

	/**
	 * Extra field to be used by the user with set/get func for whatever
	 * purpose he wants.
	 */
	void *user_cls;
};

#endif
