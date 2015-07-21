/*
    This file is part of libmicrospdy
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
 * @file io.c
 * @brief Generic functions for IO.
 * @author Andrey Uzunov
 */

#include "platform.h"
#include "structures.h"
#include "internal.h"
#include "io.h"


int
SPDYF_io_set_daemon(struct SPDY_Daemon *daemon,
                    enum SPDY_IO_SUBSYSTEM io_subsystem)
{
  switch(io_subsystem)
  {
    case SPDY_IO_SUBSYSTEM_OPENSSL:
      daemon->fio_init = &SPDYF_openssl_init;
      daemon->fio_deinit = &SPDYF_openssl_deinit;
      break;
      
    case SPDY_IO_SUBSYSTEM_RAW:
      daemon->fio_init = &SPDYF_raw_init;
      daemon->fio_deinit = &SPDYF_raw_deinit;
      break;
      
    case SPDY_IO_SUBSYSTEM_NONE:
    default:
      SPDYF_DEBUG("Unsupported subsystem");
      return SPDY_NO;
  }
  
  return SPDY_YES;
}


int
SPDYF_io_set_session(struct SPDY_Session *session,
                     enum SPDY_IO_SUBSYSTEM io_subsystem)
{
  switch(io_subsystem)
  {
    case SPDY_IO_SUBSYSTEM_OPENSSL:
      session->fio_new_session = &SPDYF_openssl_new_session;
      session->fio_close_session = &SPDYF_openssl_close_session;
      session->fio_is_pending = &SPDYF_openssl_is_pending;
      session->fio_recv = &SPDYF_openssl_recv;
      session->fio_send = &SPDYF_openssl_send;
      session->fio_before_write = &SPDYF_openssl_before_write;
      session->fio_after_write = &SPDYF_openssl_after_write;
      break;
      
    case SPDY_IO_SUBSYSTEM_RAW:
      session->fio_new_session = &SPDYF_raw_new_session;
      session->fio_close_session = &SPDYF_raw_close_session;
      session->fio_is_pending = &SPDYF_raw_is_pending;
      session->fio_recv = &SPDYF_raw_recv;
      session->fio_send = &SPDYF_raw_send;
      session->fio_before_write = &SPDYF_raw_before_write;
      session->fio_after_write = &SPDYF_raw_after_write;
      break;
      
    case SPDY_IO_SUBSYSTEM_NONE:
    default:
      SPDYF_DEBUG("Unsupported subsystem");
      return SPDY_NO;
  }
  
  return SPDY_YES;
}
