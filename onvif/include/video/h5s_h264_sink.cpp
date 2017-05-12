
#include <string.h>
#include <vector>
#include <iostream>
#include "h5s_h264_sink.hpp"
#include "live555_rtsp_client.hpp"
#include "h264_parser.h"

using namespace media;

H5SH264Sink::~H5SH264Sink()
{
	if (m_pSPS != NULL)
	{
		free(m_pSPS);
	}
	if (m_pPPS != NULL)
	{
		free(m_pPPS);
	}
	if (m_pBuffer != NULL)
	{
		free(m_pBuffer);
	}
}

void H5SH264Sink::CallbackIFrame(unsigned char* buffer, int size, unsigned long long secs, 
		unsigned long long msecs, H5SCodecType codec)
{
	if (m_pSPS == NULL || m_pPPS == NULL)
	{
		return;
	}
	
	int frameSize = size + H5S_START_CODE_LEN + m_SPSLen + m_PPSLen;
	if (m_BufferLen < frameSize)
	{
		free(m_pBuffer);
		/* Add some extern buffer to avoid free again */
		m_BufferLen = frameSize + frameSize/4;
		m_pBuffer = (unsigned char * )malloc(m_BufferLen);		
	}

	unsigned char * p = m_pBuffer;

	memcpy(p, m_pSPS, m_SPSLen);
	p = p + m_SPSLen;
	memcpy(p, m_pPPS, m_PPSLen);
	p = p + m_PPSLen;
	memcpy(p, start_code, H5S_START_CODE_LEN);
	p = p + H5S_START_CODE_LEN;

	memcpy(p, buffer, size);
	m_pCallback.onH5SData(m_pBuffer, frameSize, secs, msecs, codec, H5S_STREAM_VIDEO, 
						H5S_FRM_I);
	
	m_bGetIFrame = true;
	return;
}
void H5SH264Sink::CallbackPFrame(unsigned char* buffer, int size, unsigned long long secs, 
		unsigned long long msecs, H5SCodecType codec)
{
	/* Remove all the P frame before got the I frame */
	if (m_pSPS == NULL || m_pPPS == NULL || m_bGetIFrame == false)
	{
		return;
	}
	int frameSize = size + H5S_START_CODE_LEN;
	if (m_BufferLen < frameSize)
	{
		free(m_pBuffer);
		/* Add some extern buffer to avoid free again */
		m_BufferLen = frameSize + frameSize/4;
		m_pBuffer = (unsigned char * )malloc(m_BufferLen);		
	}

	unsigned char * p = m_pBuffer;

	memcpy(p, start_code, H5S_START_CODE_LEN);
	p = p + H5S_START_CODE_LEN;

	memcpy(p, buffer, size);
	m_pCallback.onH5SData(m_pBuffer, frameSize, secs, msecs, codec, H5S_STREAM_VIDEO, 
						H5S_FRM_P);
	return;
}

void H5SH264Sink::UpdateSPS(unsigned char* buffer, int size)
{	
	if (m_pSPS != NULL)
	{
		free(m_pSPS);
	}
	m_pSPS = (unsigned char *)malloc(size + H5S_START_CODE_LEN);

	memcpy(m_pSPS, start_code, H5S_START_CODE_LEN);
	memcpy(m_pSPS + H5S_START_CODE_LEN, buffer, size);
	m_SPSLen = size + H5S_START_CODE_LEN;
	return;
}
void H5SH264Sink::UpdatePPS(unsigned char* buffer, int size)
{
	if (m_pPPS != NULL)
	{
		free(m_pPPS);
	}
	m_pPPS = (unsigned char *)malloc(size + H5S_START_CODE_LEN);

	memcpy(m_pPPS, start_code, H5S_START_CODE_LEN);
	memcpy(m_pPPS + H5S_START_CODE_LEN, buffer, size);
	m_PPSLen = size + H5S_START_CODE_LEN;
	return;
}

bool   H5SH264Sink::ProcessData(unsigned char* buffer, int size, unsigned long long secs, 
			unsigned long long msecs, H5SCodecType codec) 
{

	//Live555DumpHex(buffer, 3);

	unsigned char nal_type = buffer[0]&0x1f;

	//std::cout << "H5SH264Sink::ProcessData" << "  " << codec << "  " << "  " << secs << "  " << msecs  << "  " << size  << std::endl;
	//printf("nal type %d\n", nal_type);
	switch (nal_type)
	{
		case H264NALU::kSPS :
		{
			UpdateSPS(buffer, size);
			break;
		}
		case H264NALU::kPPS :
		{
			UpdatePPS(buffer, size);
			break;
		}
		case H264NALU::kIDRSlice :
		{
			CallbackIFrame(buffer, size, secs, msecs, codec);
			break;
		}
		case H264NALU::kNonIDRSlice :
		//case H264NALU::kSEIMessage :
		{
			CallbackPFrame(buffer, size, secs, msecs, codec);
			break;
		}
		default:
		   	return true;
	};

	

	return true;
}


