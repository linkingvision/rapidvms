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
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include "rtspdef.h"
#include "rapidrtsplog.h"

#if defined(_WIN32) || defined(_WIN64)
#define vsnprintf _vsnprintf
#define vsprintf _vsprintf
#endif

#define MAX_PRINT_LEN	2048

//#define RRLOG_FILE	"rapidrtsplog.txt"
//static FILE *g_log_file=NULL;

RRLOG_Level_t debuglevel = RRLOG_INFO;
static int neednl;
static FILE *fmsg;

static const char *levels[] = {
    "ERROR+", "ERROR", "WARNING", "INFO",
    "DEBUG", "DEBUGHIGH"
};

static fRRLOG_Callback rapidrtsplog_default, *cb = rapidrtsplog_default;

static void rapidrtsplog_default(int level, const char *format, va_list vl) {
    char str[MAX_PRINT_LEN]="";
    vsnprintf(str, MAX_PRINT_LEN-1, format, vl);

    /* Filter out 'no-name' */
    if ( debuglevel<RRLOG_ALL && strstr(str, "no-name" ) != NULL )
        return;
	
#ifdef RRLOG_FILE
	if(g_log_file == NULL){
		g_log_file=fopen(RRLOG_FILE,"wb+");
		if(g_log_file==NULL){
			printf("open file failed\n");
			return NULL;
		}
		printf("create file:%s success",RRLOG_FILE);
	}
    if ( !fmsg ) fmsg = g_log_file;
#else
	if ( !fmsg ) fmsg = stderr;
#endif

    if ( level <= debuglevel ) {
        if (neednl) {
            putc('\n', fmsg);
            neednl = 0;
        }
        fprintf(fmsg, "[RRLOG-%s:%s\n", levels[level], str);
#ifdef _DEBUG
        fflush(fmsg);
#endif
    }
}

void RRLOG_SetOutput(FILE *file) {
    fmsg = file;
}

void RRLOG_SetLevel(RRLOG_Level_t level) {
    debuglevel = level;
}

void RRLOG_SetCallback(fRRLOG_Callback *cbp) {
    cb = cbp;
}

RRLOG_Level_t RRLOG_GetLevel() {
    return debuglevel;
}

void RRLOG(int level, const char *format, ...) {
#ifdef  RAPID_PERF_DEBUG
    va_list args;
    va_start(args, format);
    cb(level, format, args);
    va_end(args);
#endif
}

static const char hexdig[] = "0123456789abcdef";

void RRLOG_Hex(int level, const unsigned char *data, unsigned long len) {
    unsigned long i;
    char line[50], *ptr;

    if ( level > debuglevel )
        return;

    ptr = line;

    for (i=0; i<len; i++) {
        *ptr++ = hexdig[0x0f & (data[i] >> 4)];
        *ptr++ = hexdig[0x0f & data[i]];
        if ((i & 0x0f) == 0x0f) {
            *ptr = '\0';
            ptr = line;
            RRLOG(level, "%s", line);
        } else {
            *ptr++ = ' ';
        }
    }
    if (i & 0x0f) {
        *ptr = '\0';
        RRLOG(level, "%s", line);
    }
}

void RRLOG_HexString(int level, const unsigned char *data, unsigned long len) {
#define BP_OFFSET 9
#define BP_GRAPH 60
#define BP_LEN	80
    char	line[BP_LEN];
    unsigned long i;

    if ( !data || level > debuglevel )
        return;

    /* in case len is zero */
    line[0] = '\0';

    for ( i = 0 ; i < len ; i++ ) {
        int n = i % 16;
        unsigned off;

        if ( !n ) {
            if ( i ) RRLOG( level, "%s", line );
            memset( line, ' ', sizeof(line)-2 );
            line[sizeof(line)-2] = '\0';

            off = i % 0x0ffffU;

            line[2] = hexdig[0x0f & (off >> 12)];
            line[3] = hexdig[0x0f & (off >>  8)];
            line[4] = hexdig[0x0f & (off >>  4)];
            line[5] = hexdig[0x0f & off];
            line[6] = ':';
        }

        off = BP_OFFSET + n*3 + ((n >= 8)?1:0);
        line[off] = hexdig[0x0f & ( data[i] >> 4 )];
        line[off+1] = hexdig[0x0f & data[i]];

        off = BP_GRAPH + n + ((n >= 8)?1:0);

        if ( isprint( data[i] )) {
            line[BP_GRAPH + n] = data[i];
        } else {
            line[BP_GRAPH + n] = '.';
        }
    }

    RRLOG( level, "%s", line );
}

/* These should only be used by apps, never by the library itself */
void RRLOG_Printf(const char *format, ...) {
    char str[MAX_PRINT_LEN]="";
    int len;
    va_list args;
    va_start(args, format);
    len = vsnprintf(str, MAX_PRINT_LEN-1, format, args);
    va_end(args);

    if ( debuglevel==RRLOG_CRIT )
        return;

    if ( !fmsg ) fmsg = stderr;

    if (neednl) {
        putc('\n', fmsg);
        neednl = 0;
    }

    if (len > MAX_PRINT_LEN-1)
        len = MAX_PRINT_LEN-1;
    fprintf(fmsg, "%s", str);
    if (str[len-1] == '\n')
        fflush(fmsg);
}

void RRLOG_Status(const char *format, ...) {
    char str[MAX_PRINT_LEN]="";
    va_list args;
    va_start(args, format);
    vsnprintf(str, MAX_PRINT_LEN-1, format, args);
    va_end(args);

    if ( debuglevel==RRLOG_CRIT )
        return;

    if ( !fmsg ) fmsg = stderr;

    fprintf(fmsg, "%s", str);
    fflush(fmsg);
    neednl = 1;
}

