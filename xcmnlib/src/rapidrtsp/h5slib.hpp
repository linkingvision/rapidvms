#ifndef __H5S_LIB__
#define __H5S_LIB__

#include <string.h>
#include <vector>
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <map>
#include "live555_rtsp_client.hpp"
#include "h5s_sink.hpp"

typedef std::map<void *, H5SCallback * > H5SCallbackMap;
typedef std::map<std::string, std::string> H5SParameterMap;
//std::this_thread::sleep_for(std::chrono::seconds(2));
/* RTSP Client */
class  H5S_LIBRARY_API H5SLibRTSP : public Live555RTSPClient , public H5SCallback
{
public:
	H5SLibRTSP(const std::string uri, 
		std::string strUser, std::string strPasswd, bool bTCP = true);
  
	virtual ~H5SLibRTSP();
	
	virtual bool onNewSession(Live555CodecType nCodec, const char* media, const char* codec);
	/* parseSPropParameterSets to get pps & sps */
	virtual bool onData(unsigned char* buffer, int size, unsigned long long secs, 
										unsigned long long msecs, Live555CodecType codec);
	virtual bool onParameter(std::string strKey, std::string strValue);
	bool RegCallback(void * pIndex, H5SCallback * pCallback);
	bool UnRegCallback(void * pIndex);
	static bool Thread(void* pData);
	bool ThreadRun();

	bool Start();
	bool Stop();
public:
	virtual bool   onH5SData(unsigned char* buffer, int size, unsigned long long secs, 
			unsigned long long msecs, H5SCodecType codec, H5SStreamType stream, 
			H5SFrameType frm);
	bool UpdateCodecSink(H5SCodecType codec);

private:
	bool m_bStarted;
	std::thread * m_pThread;
	H5SSink *m_pVideoSink;
	H5SCodecType m_VideoCodec;
	std::mutex m_pCBLock;
	H5SCallbackMap m_pCBMap;
	H5SParameterMap m_pParamMap;
	
		
		
};

#endif 

