
#ifndef __ONVIF_CLIENT_EXAMPLE__
#define __ONVIF_CLIENT_EXAMPLE__


#include "onvifagentlib.hpp"

#include "h5slib.hpp"

class CRapidRTSPONVIF
{
public:
	inline CRapidRTSPONVIF(std::string streamUrl, int transport, 
				std::string userName, std::string userPwd, 
				bool bEnableAudio);
	inline virtual ~CRapidRTSPONVIF();
public:
	inline virtual int start(){return true;}
protected:
	std::string m_userName;
	std::string m_userPwd;

	std::string m_streamUrl;
	/* the full url will include the user & password */
	std::string m_streamFullUrl;

	bool m_bEnableAudio;
};

class CRapidRTSPONVIFLive555 :public CRapidRTSPONVIF , public H5SCallback
{
public:
	inline CRapidRTSPONVIFLive555(std::string streamUrl, int transport, 
				std::string userName, std::string userPwd, 
				bool bEnableAudio);
	inline ~CRapidRTSPONVIFLive555();
public:
	inline virtual int start();
	inline virtual bool   onH5SData(unsigned char* buffer, int size, unsigned long long secs, 
			unsigned long long msecs, H5SCodecType codec, H5SStreamType stream, 
			H5SFrameType frm);
	/* Watch thread is for reconnect */
	inline static bool WatchThread(void* pData);
	inline bool WatchThreadRun();
	inline bool CheckRTSPClient();
private:
	H5SLibRTSP *m_rtsp;
	std::mutex  m_Mutex;
	std::mutex  m_MutexData;
	long long m_lastRecv;
	long long m_currRecv;
	bool m_bExit ;
	bool m_bStarted;
	std::thread * m_pWatchThread;

};


CRapidRTSPONVIF::CRapidRTSPONVIF(std::string streamUrl, int transport, 
	std::string userName, std::string userPwd, 
	bool bEnableAudio)
: m_bEnableAudio(bEnableAudio),
  m_userName(userName), m_userPwd(userPwd), m_streamUrl(streamUrl)
{

}

CRapidRTSPONVIF::~CRapidRTSPONVIF()
{

}

CRapidRTSPONVIFLive555::CRapidRTSPONVIFLive555(std::string streamUrl, int transport, 
	std::string userName, std::string userPwd, 
	bool bEnableAudio)
: CRapidRTSPONVIF(streamUrl, transport, userName, userPwd, bEnableAudio), 
m_rtsp(NULL), m_currRecv(0), m_lastRecv(0), m_bExit(false), m_bStarted(false)
{
	m_pWatchThread = new std::thread(CRapidRTSPONVIFLive555::WatchThread, this);
	return;
}

CRapidRTSPONVIFLive555::~CRapidRTSPONVIFLive555()
{
	/* Stop the watch thread */
	m_bExit = true;
	m_pWatchThread->join();
	delete m_pWatchThread;
	
	if (m_rtsp)
	{
		delete m_rtsp;
		m_rtsp = NULL;
	}
	m_bStarted = false;
}

bool CRapidRTSPONVIFLive555::WatchThread(void* pData)
{
	CRapidRTSPONVIFLive555 * pThread = (CRapidRTSPONVIFLive555 *)pData;

	if (pThread)
	{
		return pThread->WatchThreadRun();
	}
	return false;
}
bool CRapidRTSPONVIFLive555::WatchThreadRun()
{
	int i = 0;
	while(!m_bExit)
	{
		std::chrono::milliseconds dura(500);
		std::this_thread::sleep_for( dura );
		if (i ++ >= 40)
		{
			CheckRTSPClient();
			i = 0;
		}
	}
	return true;
}

bool CRapidRTSPONVIFLive555::CheckRTSPClient()
{
	long long nCurrRecv = 0;

	{
		std::lock_guard<std::mutex> guard(m_MutexData);
		nCurrRecv = m_currRecv;
	}

	std::lock_guard<std::mutex> guard(m_Mutex);
	if (m_bStarted == false)
	{
		return true;
	}

	if (nCurrRecv <= m_lastRecv)
	{
		if (m_rtsp)
		{
			delete m_rtsp;
			m_rtsp = NULL;
		}
		bool bTCP = true;

		m_rtsp = new H5SLibRTSP(m_streamUrl, m_userName, m_userPwd, bTCP);
		/* Register the data callback */
		m_rtsp->RegCallback(this, this);
		m_rtsp->Start();
	}
	m_lastRecv = nCurrRecv;
	return true;
}

int CRapidRTSPONVIFLive555::start()
{
	std::lock_guard<std::mutex> guard(m_Mutex);
	if (m_rtsp)
	{
		delete m_rtsp;
		m_rtsp = NULL;
	}
	bool bTCP = true;

	m_rtsp = new H5SLibRTSP(m_streamUrl, m_userName, m_userPwd, bTCP);
	/* Register the data callback */
	m_rtsp->RegCallback(this, this);
	m_rtsp->Start();
	m_bStarted = true;

	return true;
}


bool   CRapidRTSPONVIFLive555::onH5SData(unsigned char* buffer, int size, unsigned long long secs, 
	unsigned long long msecs, H5SCodecType codec, H5SStreamType stream, 
	H5SFrameType frm)
{
	/* update receive */
	{
		std::lock_guard<std::mutex> guard(m_MutexData);
		m_currRecv = m_currRecv + size;
	}
	printf("Got data %d (%d, %d)\n", size, secs, msecs);
	
	return true;
}


inline int example()
{
	int ret;
	/* 192.168.1.1 is the NVT, 192.168.1.234 is the NVC */
	std::string url = "http://192.168.0.64/onvif/device_service";
	/* Below is where to receive the event */
	std::string eventNotify = "http://192.168.22.123:9091/subscription-2";
	std::string user = "admin";
	std::string pass =  "12345";

	OnvifAgentC agent(user, pass, url);
	OnvifAgentCProfileMap pProfiles;

	//while(1)
	{
		agent.Login();
		pProfiles.clear();
		agent.GetProfiles(pProfiles);
	}

	if (pProfiles.size() > 0)
	{
		OnvifAgentCProfile profile;
		profile = pProfiles.begin()->second;

		CRapidRTSPONVIFLive555 stream(profile.strRtspUrl, 0, user, pass, false);
		stream.start();
	
		
		while(1)
		{
			agent.PTZAction(pProfiles.begin()->first,AGENT_PTZ_ZOOM_IN, 0.5);
			std::chrono::milliseconds dura( 1000 );
			std::this_thread::sleep_for( dura );

		}
	}
	while(1)
	{
		std::chrono::milliseconds dura(1000);
		std::this_thread::sleep_for( dura );
	}
	
	return 0;
}

#endif 
