#ifndef __H5S_SINK__
#define __H5S_SINK__

#include <string.h>
#include <vector>
#include <iostream>

#define H5S_LIBRARY_API 

/* !!!!!! Keep below same with the videotype.hpp */

typedef enum __H5SCodecType
{
        H5S_CODEC_PCMU = 0, 
        H5S_CODEC_PCMA = 8,
        H5S_CODEC_G711A = 19,
        H5S_CODEC_G711U = 20, 
        H5S_CODEC_MJPEG = 26,
        H5S_CODEC_H264 = 96,
        H5S_CODEC_H265 = 98,
        H5S_CODEC_MPEG4 = 97,
        H5S_CODEC_AAC = 100,
        H5S_CODEC_NONE = 254,
        H5S_CODEC_END = 1000
}H5SCodecType;

typedef enum __H5SStreamType
{
	H5S_STREAM_VIDEO = 1,
	H5S_STREAM_AUDIO,
	H5S_STREAM_END
}H5SStreamType;

typedef enum __H5SFrameType
{
    H5S_FRM_NONE = 0,
    H5S_FRM_I = 1,
    H5S_FRM_P = 2,
    H5S_FRM_B = 3,
    H5S_FRM_INFO = 4,
    H5S_FRM_AUDIO = 5,
    H5S_FRM_END
} H5SFrameType;

#define H5S_START_CODE_LEN 4

class H5S_LIBRARY_API H5SCallback
{
public:
	virtual bool   onH5SData(unsigned char* buffer, int size, unsigned long long secs, 
			unsigned long long msecs, H5SCodecType codec, H5SStreamType stream, 
			H5SFrameType frm) = 0;
};

class H5S_LIBRARY_API H5SSink
{
public:
	H5SSink(H5SCallback &pCallback)
	:m_pCallback(pCallback)
	{
		memset(start_code, 0, H5S_START_CODE_LEN);
		start_code[3] = 0x01;
	}
public:
	virtual bool   ProcessData(unsigned char* buffer, int size, unsigned long long secs, 
			unsigned long long msecs, H5SCodecType codec) = 0;
public:
	H5SCallback &m_pCallback;
	unsigned char start_code[H5S_START_CODE_LEN];
};



#endif 

