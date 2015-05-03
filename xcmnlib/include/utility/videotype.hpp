#ifndef _V_VIDEO_TYPE_H_
#define _V_VIDEO_TYPE_H_
#include "utility/type.hpp"

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

typedef struct __RawFrame {
 	u32 secs;       /* timestamp in seconds */
	u32 msecs;      /* timestamp in mseconds */
#define VE_NUM_POINTERS 8
	char *data[VE_NUM_POINTERS];
	int linesize[VE_NUM_POINTERS];
	int width, height;
	int format;
} RawFrame;

typedef enum __VideoStreamType
{
	VIDEO_STREAM_VIDEO = 1,
	VIDEO_STREAM_AUDIO,
	VIDEO_STREAM_INFO,
	VIDEO_STREAM_TEXT,
	VIDEO_STREAM_LAST
}VideoStreamType;

typedef enum __CodecType
{
        CODEC_H264 = 96,
        CODEC_H265 = 98,
        CODEC_MPEG4 = 97,
        CODEC_MJPEG = 26,
        CODEC_PCMA = 8,
        CODEC_PCMU = 0, 
        CODEC_G711A = 19,
        CODEC_G711U = 20, 
        CODEC_LAST = 1000
}CodecType;

typedef enum __VideoFrameType
{
    VIDEO_FRM_NONE = 0,
    VIDEO_FRM_I = 1,
    VIDEO_FRM_P = 2,
    VIDEO_FRM_B = 3,
    VIDEO_FRM_INFO = 4,
    VIDEO_FRM_LAST
} VideoFrameType;

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

typedef struct __VideoFrameHeader
{
	u32 streamType; /* VideoStreamType */
 	u32 frameType; /* VideoFrameType */
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
	u16 video;/* CodecType */
	u16 audio;/* CodecType */
	u32 padding1;
}InfoFrameP;

typedef struct __InfoFrameI /* All I frame use thie */
{
	u16 video;/* CodecType */
	u16 audio;/* CodecType */
	u32 padding0;
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


#define MF_FILE_MAX_LENGTH 16 * 1024 * 1024

typedef enum
{
    MF_OK = 0,
    MF_WRTIE_REACH_END,
    MF_FRAME_TIME_ERROR,
    MF_LAST
} MFStatus;


typedef enum
{
    R_MANUAL = 1,
    R_ALARM,
    R_SCHED,
    R_ALL,
    R_LAST
} RecordingType;

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
} VdbRecordItem;

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

typedef std::map<int, VdbRecordItem> RecordItemMap;

typedef enum
{
    EVENT_TYPE_MOTION = 1,
    EVENT_TYPE_LAST
} VscEventType;

#endif /* _V_VIDEO_TYPE_H_ */
