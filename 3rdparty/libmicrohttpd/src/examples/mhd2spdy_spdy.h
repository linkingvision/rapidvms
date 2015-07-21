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
 * @file mhd2spdy_spdy.h
 * @brief  SPDY part of the proxy. libspdylay is used for the client side.
 * @author Andrey Uzunov
 */
 
#ifndef SPDY_H
#define SPDY_H

#include "mhd2spdy_structures.h"


struct SPDY_Connection *
spdy_connect(const struct URI *uri,
             uint16_t port,
             bool is_tls);


void
spdy_ctl_poll(struct pollfd *pollfd,
              struct SPDY_Connection *connection);


bool
spdy_ctl_select(fd_set * read_fd_set,
                fd_set * write_fd_set, 
                fd_set * except_fd_set,
                struct SPDY_Connection *connection);


int
spdy_exec_io(struct SPDY_Connection *connection);


void
spdy_diec(const char *func,
          int error_code);


int 
spdy_request(const char **nv,
             struct Proxy *proxy,
             bool with_body);


void
spdy_ssl_init_ssl_ctx(SSL_CTX *ssl_ctx,
                      uint16_t *spdy_proto_version);


void
spdy_free_connection(struct SPDY_Connection * connection);


void
spdy_get_pollfdset(struct pollfd fds[],
                   struct SPDY_Connection *connections[],
                   unsigned int max_size,
                   nfds_t *real_size);


int
spdy_get_selectfdset(fd_set * read_fd_set,
                    fd_set * write_fd_set, 
                    fd_set * except_fd_set,
                    struct SPDY_Connection *connections[],
                    unsigned int max_size,
                    nfds_t *real_size);


void
spdy_run(struct pollfd fds[],
        struct SPDY_Connection *connections[],
        int size);


void
spdy_run_select(fd_set * read_fd_set,
                fd_set * write_fd_set, 
                fd_set * except_fd_set,
                struct SPDY_Connection *connections[],
                int size);


#endif
