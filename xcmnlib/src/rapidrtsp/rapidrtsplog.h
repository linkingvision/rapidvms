/** <!--
 *
 *  Copyright (C) 2017 veyesys support@veyesys.com
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  If you would like this software to be made available to you under an 
 *  alternate commercial license please email support@veyesys.com 
 *  for more information.
 *
 * -->
 */
#ifndef __RRLOG_H__
#define __RRLOG_H__

#include <stdio.h>
#include <stdarg.h>
//#include <stdint.h>

    /* Enable this to get full debugging output */
    /* #define _DEBUG */

#ifdef _DEBUG
#undef NODEBUG
#endif

typedef enum
{ 
	RRLOG_CRIT=0,
	RRLOG_ERROR,
	RRLOG_WARNING,
	RRLOG_INFO,
    RRLOG_DEBUG,
    RRLOG_DEBUGHIGH,
    RRLOG_ALL
} RRLOG_Level_t;

extern RRLOG_Level_t debuglevel;

typedef void (fRRLOG_Callback)(int level, const char *fmt, va_list);
void RRLOG_SetCallback(fRRLOG_Callback *cb);
void RRLOG_SetOutput(FILE *file);

#ifdef __GNUC__
void RRLOG_Printf(const char *format, ...) __attribute__ ((__format__ (__printf__, 1, 2)));
void RRLOG_Status(const char *format, ...) __attribute__ ((__format__ (__printf__, 1, 2)));
void RRLOG(int level, const char *format, ...) __attribute__ ((__format__ (__printf__, 2, 3)));
#else
void RRLOG_Printf(const char *format, ...);
void RRLOG_Status(const char *format, ...);
void RRLOG(int level, const char *format, ...);
#endif
void RRLOG_Hex(int level, const unsigned char *data, unsigned long len);
void RRLOG_HexString(int level, const unsigned char *data, unsigned long len);
void RRLOG_SetLevel(RRLOG_Level_t lvl);
RRLOG_Level_t RRLOG_GetLevel(void);


#endif
