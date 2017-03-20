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
#ifndef __RTSPDEF_H__
#define __RTSPDEF_H__


#include "ros.h"
/* Enable and disable the RAPID DEBUG */
//#define RAPID_PERF_DEBUG

#define RTSP_SOCK_TIMEOUT			(8000) //  unit: ms , socket send and receive timeout
#define RTSP_PLAYER_BUFFER_TIME		(3500)	// unit: ms

#define RTSP_ENABLE_LAN_CHECK

#define RTCP_SENDER
#define RTCP_RECEIVER

#define H264_TS_TO_MILLISECOND(TS)	((TS)/90)//(1000/90000*TS)
#define G711_TS_TO_MILLISECOND(TS)  ((TS)/8)//(1000/8000*TS)

#define RTSP_INTERLEAVED_MAGIC	'$'//0X24


#define H264_NON_IDR				1
#define H264_SLICE_PARTITION_A		2
#define H264_SLICE_PARTITION_B		3
#define H264_SLICE_PARTITION_C		4
#define H264_IDR					5
#define H264_SEI					6
#define H264_SPS					7
#define H264_PPS					8
#define H264_ACCESS_UNIT_DELIMITER	9
#define H264_END_OF_SEQUENCE		10
#define H264_END_OF_STREAM			11
#define H264_FILLER_DATA			12

#define RTP_MAX_PAD_SIZE 1024 * 16
#define RTP_FLAG_HAS_SPS_PPS 1

#define RTSP_NET_PROC	(0x01)
#define RTSP_DEC_PROC	(0x02)
#define RTSPC_RUNNING	(RTSP_NET_PROC | RTSP_DEC_PROC)

#define RTSP_RET_OK				(0)
#define RTSP_RET_FAIL			(-1)

#ifndef true
#define true	(1)
#endif
#ifndef false
#define false	(0)
#endif

#ifndef TRUE
#define TRUE	(1)
#endif
#ifndef FALSE
#define FALSE	(0)
#endif

#ifndef NULL
#define NULL ((void *)0)
#endif

#define ABS_MINUS(A,B) (((A) > (B)) ? ((A)-(B)) : ((B)-(A)))
#define MAXAB(A,B) (((A) > (B)) ? (A) : (B))
#define MAXABC(A,B,C) ((MAXAB((A),(B)) > (C)) ? MAXAB((A),(B)) : (C))


#ifndef uint8_t
typedef unsigned char uint8_t;
#endif
#ifndef uint16_t
typedef unsigned short uint16_t;
#endif
#ifndef uint32_t
typedef unsigned int 	uint32_t;
#endif
#ifndef uint64_t
#ifdef _WIN32
typedef unsigned __int64 uint64_t;
#else
//typedef unsigned long long uint64_t;
#endif
#endif

typedef struct _thread_args
{
	void *data;
	void *LParam;
	int RParam;
}ThreadArgs_t;

#endif

