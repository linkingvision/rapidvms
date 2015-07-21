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
 * @file microspdy/internal.h
 * @brief  internal functions and macros for the framing layer
 * @author Andrey Uzunov
 */

#ifndef INTERNAL_H_H
#define INTERNAL_H_H

#include "platform.h"
#include "microspdy.h"

/**
 * size of read buffers for each connection
 * must be at least the size of SPDY_MAX_SUPPORTED_FRAME_SIZE
 */
#define SPDYF_BUFFER_SIZE 8192

/**
 * initial size of window for each stream (this is for the data
 * within data frames that can be handled)
 */
#define SPDYF_INITIAL_WINDOW_SIZE 65536

/**
 * number of frames written to the socket at once. After X frames
 * everything should be run again. In this way the application can
 * response to more important requests while a big file is still
 * being transmitted to the client
 */
#define SPDYF_NUM_SENT_FRAMES_AT_ONCE 10


/**
 * Handler for fatal errors.
 */
extern SPDY_PanicCallback spdyf_panic;


/**
 * Closure argument for "mhd_panic".
 */
extern void *spdyf_panic_cls;


/**
 * Trigger 'panic' action based on fatal errors.
 * 
 * @param msg error message (const char *)
 */
#define SPDYF_PANIC(msg) \
	spdyf_panic (spdyf_panic_cls, __FILE__, __LINE__, msg)


/**
 * Asserts the validity of an expression.
 *
 * @param expr (bool)
 * @param msg message to print on error (const char *)
 */
#define SPDYF_ASSERT(expr, msg) \
	if(!(expr)){\
		SPDYF_PANIC(msg);\
		abort();\
	}


/**
 * Convert 24 bit integer from host byte order to network byte order.
 *
 * @param n input value (int32_t)
 * @return converted value (uint32_t)
 */
#if HAVE_BIG_ENDIAN
#define HTON24(n) n
#else
#define HTON24(n) (((((uint32_t)(n) & 0xFF)) << 16)\
	| (((uint32_t)(n) & 0xFF00))\
	| ((((uint32_t)(n) & 0xFF0000)) >> 16))
#endif


/**
 * Convert 24 bit integer from network byte order to host byte order.
 *
 * @param n input value (int32_t)
 * @return converted value (uint32_t)
 */	
#if HAVE_BIG_ENDIAN
#define NTOH24(n) n
#else
#define NTOH24(n) (((((uint32_t)(n) & 0xFF)) << 16)\
	| (((uint32_t)(n) & 0xFF00))\
	| ((((uint32_t)(n) & 0xFF0000)) >> 16))
#endif


/**
 * Convert 31 bit integer from network byte order to host byte order.
 *
 * @param n input value (int32_t)
 * @return converted value (uint32_t)
 */
#if HAVE_BIG_ENDIAN
#define NTOH31(n) n
#else
#define NTOH31(n) (((((uint32_t)(n) & 0x7F)) << 24) | \
                  ((((uint32_t)(n) & 0xFF00)) << 8) | \
                  ((((uint32_t)(n) & 0xFF0000)) >> 8) | \
                  ((((uint32_t)(n) & 0xFF000000)) >> 24))
#endif


/**
 * Convert 31 bit integer from host byte order to network byte order.
 *
 * @param n input value (int32_t)
 * @return converted value (uint32_t)
 */
#if HAVE_BIG_ENDIAN
#define HTON31(n) n
#else
#define HTON31(n) (((((uint32_t)(n) & 0xFF)) << 24) | \
                  ((((uint32_t)(n) & 0xFF00)) << 8) | \
                  ((((uint32_t)(n) & 0xFF0000)) >> 8) | \
                  ((((uint32_t)(n) & 0x7F000000)) >> 24))
#endif


/**
 * Print formatted debug value.
 *
 * @param fmt format (const char *)
 * @param ... args for format
 */
#define SPDYF_DEBUG(fmt, ...) do { \
	fprintf (stdout, "%s\n%u: ",__FILE__, __LINE__);\
	fprintf(stdout,fmt,##__VA_ARGS__);\
	fprintf(stdout,"\n");\
	fflush(stdout); } while (0)


/**
 * Print stream for debuging.
 *
 * @param strm (void *)
 * @param size (int)
 */
#define SPDYF_PRINT_STREAM(strm, size) do { \
	int ___i;\
	for(___i=0;___i<size;___i++){\
		fprintf(stdout,"%x ",*((uint8_t *) strm + ___i));\
		fflush(stdout);\
	}\
	fprintf(stdout,"\n");\
	} while (0)


/**
 * Print message and raise SIGINT for debug purposes.
 *
 * @param msg message (const char *)
 */
#define SPDYF_SIGINT(msg) do { \
	fprintf(stdout,"%i : %s\n", __LINE__,__FILE__);\
	fprintf(stdout,msg);\
	fprintf(stdout,"\n");\
	fflush(stdout);\
	raise(SIGINT); } while (0)


/**
 * Returns monotonic time, to be used for session timeouts.
 *
 * @return time in milliseconds
 */
unsigned long long
SPDYF_monotonic_time(void);

#endif
