/*
     This file is part of libmicrohttpd
     Copyright (C) 2008 Christian Grothoff (and other contributing authors)

     This library is free software; you can redistribute it and/or
     modify it under the terms of the GNU Lesser General Public
     License as published by the Free Software Foundation; either
     version 2.1 of the License, or (at your option) any later version.

     This library is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
     Lesser General Public License for more details.

     You should have received a copy of the GNU Lesser General Public
     License along with this library; if not, write to the Free Software
     Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

/**
 * @file platform.h
 * @brief platform-specific includes for libmicrohttpd
 * @author Christian Grothoff
 *
 * This file is included by the libmicrohttpd code
 * before "microhttpd.h"; it provides the required
 * standard headers (which are platform-specific).<p>
 *
 * Note that this file depends on our configure.ac
 * build process and the generated config.h file.
 * Hence you cannot include it directly in applications
 * that use libmicrohttpd.
 */
#ifndef MHD_PLATFORM_H
#define MHD_PLATFORM_H

#include "MHD_config.h"

#ifndef BUILDING_MHD_LIB
#ifdef _MHD_EXTERN
#undef _MHD_EXTERN
#endif /* _MHD_EXTERN */
#if defined(_WIN32) && defined(MHD_W32LIB)
#define _MHD_EXTERN extern
#elif defined (_WIN32) && defined(MHD_W32DLL)
#define _MHD_EXTERN __declspec(dllimport) 
#else
#define _MHD_EXTERN extern
#endif
#elif !defined(_MHD_EXTERN) /* && BUILDING_MHD_LIB */
#if defined(_WIN32) && defined(MHD_W32LIB)
#define _MHD_EXTERN extern
#elif defined (_WIN32) && defined(MHD_W32DLL)
#define _MHD_EXTERN extern __declspec(dllexport) 
#else
#define _MHD_EXTERN extern
#endif
#endif /* BUILDING_MHD_LIB */

#define _XOPEN_SOURCE_EXTENDED  1
#if OS390
#define _OPEN_THREADS
#define _OPEN_SYS_SOCK_IPV6
#define _OPEN_MSGQ_EXT
#define _LP64
#endif

#if defined(_WIN32)
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#else // _WIN32_WINNT
#if _WIN32_WINNT < 0x0501
#error "Headers for Windows XP or later are required"
#endif // _WIN32_WINNT < 0x0501
#endif // _WIN32_WINNT
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif /* !WIN32_LEAN_AND_MEAN */
#endif // _WIN32

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <stdarg.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stddef.h>
#ifdef MHD_USE_POSIX_THREADS
#undef HAVE_CONFIG_H
#include <pthread.h>
#define HAVE_CONFIG_H 1
#endif // MHD_USE_POSIX_THREADS

/* different OSes have fd_set in
   a broad range of header files;
   we just include most of them (if they
   are available) */


#ifdef OS_VXWORKS
#include <sockLib.h>
#include <netinet/in.h>
#include <stdarg.h>
#include <sys/mman.h>
#define RESTRICT __restrict__
#endif
#if HAVE_MEMORY_H
#include <memory.h>
#endif

#if HAVE_SYS_SELECT_H
#include <sys/select.h>
#endif
#if HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#if HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#if HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#if HAVE_SYS_MSG_H
#include <sys/msg.h>
#endif
#if HAVE_SYS_MMAN_H
#include <sys/mman.h>
#endif
#if HAVE_NETDB_H
#include <netdb.h>
#endif
#if HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif
#if HAVE_TIME_H
#include <time.h>
#endif
#if HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif
#if HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif


#if defined(_WIN32) && !defined(__CYGWIN__)
#include <ws2tcpip.h>
#define sleep(seconds) (SleepEx((seconds)*1000, 1)/1000)
#define usleep(useconds) (void)SleepEx((useconds)/1000, 1)
#endif

#if !defined(SHUT_WR) && defined(SD_SEND)
#define SHUT_WR SD_SEND
#endif
#if !defined(SHUT_RD) && defined(SD_RECEIVE)
#define SHUT_RD SD_RECEIVE
#endif
#if !defined(SHUT_RDWR) && defined(SD_BOTH)
#define SHUT_RDWR SD_BOTH
#endif

#if defined(_MSC_FULL_VER) && !defined (_SSIZE_T_DEFINED)
#define _SSIZE_T_DEFINED
typedef intptr_t ssize_t;
#endif // !_SSIZE_T_DEFINED */
#ifndef MHD_SOCKET_DEFINED
/**
 * MHD_socket is type for socket FDs
 */
#if !defined(_WIN32) || defined(_SYS_TYPES_FD_SET)
#define MHD_POSIX_SOCKETS 1
typedef int MHD_socket;
#define MHD_INVALID_SOCKET (-1)
#else /* !defined(_WIN32) || defined(_SYS_TYPES_FD_SET) */
#define MHD_WINSOCK_SOCKETS 1
#include <winsock2.h>
typedef SOCKET MHD_socket;
#define MHD_INVALID_SOCKET (INVALID_SOCKET)
#endif /* !defined(_WIN32) || defined(_SYS_TYPES_FD_SET) */
#define MHD_SOCKET_DEFINED 1
#endif /* MHD_SOCKET_DEFINED */

/* Force don't use pipes on W32 */
#if defined(_WIN32) && !defined(MHD_DONT_USE_PIPES)
#define MHD_DONT_USE_PIPES 1
#endif /* defined(_WIN32) && !defined(MHD_DONT_USE_PIPES) */

/* MHD_pipe is type for pipe FDs*/
#ifndef MHD_DONT_USE_PIPES
typedef int MHD_pipe;
#else /* ! MHD_DONT_USE_PIPES */
typedef MHD_socket MHD_pipe;
#endif /* ! MHD_DONT_USE_PIPES */

#if !defined(IPPROTO_IPV6) && defined(_MSC_FULL_VER) && _WIN32_WINNT >= 0x0501
/* VC use IPPROTO_IPV6 as part of enum */
#define IPPROTO_IPV6 IPPROTO_IPV6
#endif

#endif
