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
#ifndef _V_VIDEO_TYPE_H_
#define _V_VIDEO_TYPE_H_
#include "utility/type.hpp"
#include <vector>
#if 0
#define MAKE_VE_RAW(a,b,c,d) (a | (b << 8) | (c << 16) | (d << 24))

#define VE_PIX_FMT_YUV420P MAKE_VE_RAW('I','4','2','0') /* 16  YVU420 planar */
#define VE_PIX_FMT_IYUV    MAKE_VE_RAW('I','Y','U','V') /* 16  IYUV   planar */
#define VE_PIX_FMT_YVU410  MAKE_VE_RAW('Y','V','U','9') /*  9  YVU 4:1:0     */
#define VE_PIX_FMT_YVU420  MAKE_VE_RAW('Y','V','1','2') /* 12  YVU 4:2:0     */
#define VE_PIX_FMT_YUY2    MAKE_VE_RAW('Y','U','Y','2') /* 16  YUYV 4:2:2 */
#define VE_PIX_FMT_UYVY    MAKE_VE_RAW('U','Y','V','Y') /* 16  UYVY 4:2:2 */
#define VE_PIX_FMT_YVYU    MAKE_VE_RAW('Y','V','Y','U') /* 16  UYVY 4:2:2 */

#define VE_PIX_FMT_RGB332  MAKE_VE_RAW('R','G','B','1') /*  8  RGB-3-3-2     */
#define VE_PIX_FMT_RGB555  MAKE_VE_RAW('R','G','B','O') /* 16  RGB-5-5-5     */
#define VE_PIX_FMT_RGB565  MAKE_VE_RAW('R','G','B','P') /* 16  RGB-5-6-5     */
#define VE_PIX_FMT_RGB555X MAKE_VE_RAW('R','G','B','Q') /* 16  RGB-5-5-5 BE  */
#define VE_PIX_FMT_RGB565X MAKE_VE_RAW('R','G','B','R') /* 16  RGB-5-6-5 BE  */
#define VE_PIX_FMT_BGR15   MAKE_VE_RAW('B','G','R',15)  /* 15  BGR-5-5-5     */
#define VE_PIX_FMT_RGB15   MAKE_VE_RAW('R','G','B',15)  /* 15  RGB-5-5-5     */
#define VE_PIX_FMT_BGR16   MAKE_VE_RAW('B','G','R',16)  /* 32  BGR-5-6-5     */
#define VE_PIX_FMT_RGB16   MAKE_VE_RAW('R','G','B',16)  /* 32  RGB-5-6-5     */
#define VE_PIX_FMT_BGR24   MAKE_VE_RAW('B','G','R',24)  /* 24  BGR-8-8-8     */
#define VE_PIX_FMT_RGB24   MAKE_VE_RAW('R','G','B',24)  /* 24  RGB-8-8-8     */
#define VE_PIX_FMT_BGR32   MAKE_VE_RAW('B','G','R',32)  /* 32  BGR-8-8-8-8   */
#define VE_PIX_FMT_RGB32   MAKE_VE_RAW('R','G','B',32)  /* 32  RGB-8-8-8-8   */
#define VE_PIX_FMT_BGR     (('B'<<24)|('G'<<16)|('R'<<8))
#define VE_PIX_FMT_BGR8    (VE_PIX_FMT_BGR|8)
#endif
typedef enum __VideoRawType
{
	VIDEO_RAW_VIDEO = 1,
	VIDEO_RAW_AUDIO,
	VIDEO_RAW_TEXT,
	VIDEO_RAW_LAST
}VideoRawType;

typedef enum __CodecType
{
        CODEC_PCMU = 0, 
        CODEC_PCMA = 8,
        CODEC_G711A = 19,
        CODEC_G711U = 20, 
        CODEC_H264 = 96,
        CODEC_H265 = 98,
        CODEC_MPEG4 = 97,
        CODEC_MJPEG = 26,
        CODEC_AAC = 100,
        CODEC_NONE = 254,
        CODEC_LAST = 1000
}CodecType;

typedef enum __VideoHardwareCmd
{
	VIDEO_HW_NONE = 0,
	VIDEO_HW_DATA = 1, 
	VIDEO_HW_GET_D3D_DEV,
	VIDEO_HW_LAST
}VideoHardwareCmd;

struct __RawFrame;

typedef struct __RawFrame {
	VideoRawType type;
	CodecType codec;
 	u32 secs;       /* timestamp in seconds */
	u32 msecs;      /* timestamp in mseconds */
#define VE_NUM_POINTERS 8
	char *data[VE_NUM_POINTERS];
	int linesize[VE_NUM_POINTERS];
	int width, height;
	int format;

	/* Hardware command */
	VideoHardwareCmd hwCmd;
	void * hwD3DDev;

	__RawFrame()
	{
		hwCmd = VIDEO_HW_NONE;
		hwD3DDev = NULL;
		codec = CODEC_NONE;
		format = 0;
		width = 0;
		height = 0;
	}
} RawFrame;

typedef enum __VideoStreamType
{
	VIDEO_STREAM_VIDEO = 1,
	VIDEO_STREAM_AUDIO,
	VIDEO_STREAM_INFO,
	VIDEO_STREAM_TEXT,
	VIDEO_STREAM_END,
	VIDEO_STREAM_LAST
}VideoStreamType;

typedef enum __VideoFrameType
{
    VIDEO_FRM_NONE = 0,
    VIDEO_FRM_I = 1,
    VIDEO_FRM_P = 2,
    VIDEO_FRM_B = 3,
    VIDEO_FRM_INFO = 4,
    VIDEO_FRM_AUDIO = 5,
    VIDEO_FRM_LAST
} VideoFrameType;


/* Connection Type */
typedef enum
{
    VSC_CONNECT_TCP = 1,
    VSC_CONNECT_UDP,
    VSC_CONNECT_HTTP,
    VSC_CONNECT_HTTPS,
    VSC_CONNECT_LAST
} VSCConnectType;


typedef struct __VideoFrame
{
	VideoStreamType streamType;
 	VideoFrameType frameType;
 	u32 secs;       /* timestamp in seconds */
	u32 msecs;      /* timestamp in mseconds */
	u32 dataLen; /* the length of the payload data */
	u8   *dataBuf;
	u32 bufLen;/* the length of the buffer */
}VideoFrame;

typedef struct __VideoStreamInfo
{
	CodecType codec;
	int width;
	int height;
	int fps;
}VideoStreamInfo;

typedef struct __VideoFrameHeader
{
	u32 streamType; /* VideoStreamType */
 	u32 frameType; /* VideoFrameType */
	u32 seq;/* seq for frame lost */
 	u32 secs;       /* timestamp in seconds */
	u32 msecs;      /* timestamp in mseconds */
	u32 dataLen;
}VideoFrameHeader;

typedef enum __VideoSeqType
{
	VIDEO_SEQ_MP4 = 1,
	VIDEO_SEQ_AVI,
	VIDEO_SEQ_LAST
}VideoSeqType;

typedef struct __InfoFrameP /* All P frame use this */
{
	u8 video;/* CodecType */
	u8 audio;/* CodecType */
	u16 padding1;
}InfoFrameP;

typedef struct __InfoFrameI /* All I frame use thie */
{
	u8 video;/* CodecType */
	u8 audio;/* CodecType */
	u16 padding0;
	u16 vFps;
	u32 vWidth;
	u32 vHeight;
	u16 aSampleRate;
	u16 aSampleSize;
	u16 aChannels;
	u32 padding1;
	u32 padding2;
	u32 padding3;
	u32 padding4;
	u16 padding5;
}InfoFrameI;


typedef struct __InfoFrame
{
        CodecType video;
        u32 vWidth;
        u32 vHeight;
        u32 vFps;
        CodecType audio;
        u32 aRate;
        u32 aBitrate;
        u32 aChannels;
        u32 aBitspersample;
        u32 padding1;
        u32 padding2;
        u32 padding3;
        u32 padding4;
        u32 padding5;
        u32 padding6;
        u32 padding7;
        u32 padding8;
}InfoFrame;


#define MF_FILE_MAX_LENGTH 16 * 1024 * 1024

typedef enum
{
    MF_OK = 0,
    MF_ERROR,
    MF_WRTIE_REACH_END,
    MF_FRAME_TIME_ERROR,
    MF_LAST
} MFStatus;


typedef enum
{
	R_OFF = 0,
	R_ALARM = 1,
	R_SCHED = 2,
	R_MANUAL = 4,
	R_LAST
} RecordingType;

typedef enum
{
    A_MOT_CAM = 1,
    A_MOT_SERVER,
    A_DIGTAL_IN,
    A_DIGTAL_OUT,
    A_LAST
} AlarmType;

enum RecordingMode {
   RECORDING_FULL_STOP = 1,
   RECORDING_FULL_LOOP
};

typedef enum
{
    HDD_DISK_LOST = 1,
    HDD_DISK_ERROR,
    HDD_DISK_OK,
    HDD_DISK_READ_ONLY,
    HDD_DISK_LAST
} DiskStatus;

typedef struct __VdbRecordItem
{	
    s64 id;	
    s32 start;	
    s32 end;
    u32 type;
} VdbRecordItem;

typedef struct __VdbHasRecordItem
{	
    s64 id;	
    s32 start;	
    s32 end;
    u32 type;
    bool has;
} VdbHasRecordItem;

typedef struct __VdbDiskItem
{	
	s64 limit;	
	s64 used;
	astring hdd;
	astring path;
	s64 loading;//loading is for writing to different hdd when find block
} VdbDiskItem;
typedef struct __VdbDiskStatus
{	
	astring hdd;
	astring path;
	DiskStatus status;
	s64 freeSize;
	s64 totalSize;
    
} VdbDiskStatus;

typedef std::map<u64, VdbRecordItem> RecordItemMap;
typedef std::map<u64, VdbHasRecordItem> HasRecordItemMap;

typedef enum
{
    EVENT_TYPE_MOTION = 1,
    EVENT_TYPE_LAST
} VscEventType;

typedef struct __VBlob
{	
	__VBlob(s32 a0, s32 a1, s32 b0, s32 b1)
	:x0(a0), y0(b0), x1(a1), y1(b1)
	{
	}
	s32 x0;
	s32 y0;
	s32 x1;
	s32 y1;
} VBlob;
typedef std::vector<VBlob> VBlobVec;


typedef BOOL (*RMDataHandler)(void* pContext, VideoFrame& packet);
typedef BOOL (*RMRawVideoHandler)(void* pContext, RawFrame& packet);

#endif /* _V_VIDEO_TYPE_H_ */
