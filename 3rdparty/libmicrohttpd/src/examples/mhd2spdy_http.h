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
 * @file mhd2spdy_http.h
 * @brief  HTTP part of the proxy. libmicrohttpd is used for the server side.
 * @author Andrey Uzunov
 */
 
#ifndef HTTP_H
#define HTTP_H

#include "mhd2spdy_structures.h"


int
http_cb_request (void *cls,
                struct MHD_Connection *connection,
                const char *url,
                const char *method,
                const char *version,
                const char *upload_data,
                size_t *upload_data_size,
                void **ptr);


void * http_cb_log(void * cls, const char * uri);


void
http_create_response(struct Proxy* proxy, char **nv);


void
http_cb_request_completed (void *cls,
                                   struct MHD_Connection *connection,
                                   void **con_cls,
                                   enum MHD_RequestTerminationCode toe);

#endif
