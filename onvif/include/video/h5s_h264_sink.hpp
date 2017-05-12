#ifndef __H5S_H264_SINK_LIB__
#define __H5S_H264_SINK_LIB__

#ifdef LINUX
#include <stdlib.h>
#endif

#include <string.h>
#include <vector>
#include <iostream>
#include "h5s_sink.hpp"


class H5S_LIBRARY_API H5SH264Sink : public H5SSink
{
public:
	H5SH264Sink(H5SCallback &pCallback, unsigned char * sps, int spsLen, unsigned char * pps, 
					int ppsLen)
	:H5SSink(pCallback), m_PPSLen(0), m_SPSLen(0), m_pPPS(NULL), m_pSPS(NULL), 
	m_bGetIFrame(false), m_pBuffer(NULL), m_BufferLen(0)
	{
		/* Default buffer is 512k */
		m_BufferLen = 1024 * 512;
		m_pBuffer = (unsigned char * )malloc(m_BufferLen);
		if (sps)
		{
			UpdateSPS(sps, spsLen);
		}
		if (pps)
		{
			UpdatePPS(pps, ppsLen);
		}
	}
	~H5SH264Sink();
public:
	virtual bool   ProcessData(unsigned char* buffer, int size, unsigned long long secs, 
			unsigned long long msecs, H5SCodecType codec);
public:
	void UpdateSPS(unsigned char* buffer, int size);
	void UpdatePPS(unsigned char* buffer, int size);
	void CallbackIFrame(unsigned char* buffer, int size, unsigned long long secs, 
			unsigned long long msecs, H5SCodecType codec);
	void CallbackPFrame(unsigned char* buffer, int size, unsigned long long secs, 
			unsigned long long msecs, H5SCodecType codec);
private:
	unsigned char * m_pPPS;
	int m_PPSLen;
	unsigned char * m_pSPS;
	int m_SPSLen;
	bool m_bGetIFrame;
	unsigned char * m_pBuffer;
	int m_BufferLen;

};

#endif 

