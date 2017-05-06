#include "h5slib.hpp"
#include "h5s_h264_sink.hpp"

//https://juanchopanzacpp.wordpress.com/2013/02/26/concurrent-queue-c11/
//http://codereview.stackexchange.com/questions/39199/multi-producer-consumer-queue-without-boost-in-c11

H5SLibRTSP::H5SLibRTSP(const std::string uri, 
	std::string strUser, std::string strPasswd, bool bTCP) 
: Live555RTSPClient(uri, strUser, strPasswd, bTCP), m_bStarted(false), 
m_pThread(NULL), m_pVideoSink(NULL), m_VideoCodec(H5S_CODEC_NONE)
{
}

H5SLibRTSP::~H5SLibRTSP() 
{
	Stop();
}


bool H5SLibRTSP::RegCallback(void * pIndex, H5SCallback * pCallback)
{
	std::lock_guard<std::mutex> guard(m_pCBLock);

	m_pCBMap[pIndex] = pCallback;

	return true;
}
bool H5SLibRTSP::UnRegCallback(void * pIndex)
{
	std::lock_guard<std::mutex> guard(m_pCBLock);

	m_pCBMap.erase(pIndex);

	return true;
}

bool H5SLibRTSP::Start()
{
	if (m_bStarted == true)
	{
		return true;
	}
	m_pThread = new std::thread(H5SLibRTSP::Thread, this);
	m_bStarted = true;

	return true;
}
bool H5SLibRTSP::Stop()
{
	if (m_bStarted == false)
	{
		return true;
	}
	RTSPStop();
	m_pThread->join();
	delete m_pThread;
	m_bStarted = false;

	return true;
}

bool H5SLibRTSP::onNewSession(Live555CodecType nCodec, const char* media, const char* codec)
{
	bool success = false;
	if ( (strcasecmp(media, "video") == 0) && (strcasecmp(codec, "H264") == 0) )
	{
		success = true;
	}
	else if ((strcasecmp(media, "video") == 0) && (strcasecmp(codec, "JPEG") == 0))
	{
		success = true;
	}
	else if ((strcasecmp(media, "video") == 0) && (strcasecmp(codec, "H265") == 0))
	{
		success = true;
	}
	printf("%s media %s %d %s codec %s\n", __FUNCTION__, __FILE__, __LINE__, media, codec);
	return success;			
}

bool H5SLibRTSP::onParameter(std::string strKey, std::string strValue)
{
	if (strKey.size() <= 0 || strValue.size() <= 0)
	{
		return false;
	}
	printf("%s key %s value %s\n", __FUNCTION__, strKey.c_str(), strValue.c_str());

	m_pParamMap[strKey] = strValue;
	return true;
}

bool H5SLibRTSP::onData(unsigned char* buffer, int size, unsigned long long secs, 
									unsigned long long msecs, Live555CodecType codec)
{
	//std::cout << "onData" << "  " << codec << "  " << "  " << secs << "  " << msecs  << "  " << size << std::endl;

	/* Just callback none H264 & H265 & MPEG4 */
	if (codec != LIVE555_CODEC_H264 && codec != LIVE555_CODEC_H265
		&& codec != LIVE555_CODEC_MPEG4)
	{	
		H5SStreamType streamType = H5S_STREAM_AUDIO;
		if (LIVE555_CODEC_MJPEG == codec)
		{
			streamType = H5S_STREAM_VIDEO;
		}
		onH5SData(buffer, size, secs, msecs, (H5SCodecType)codec, streamType, 
			H5S_FRM_I);
		return true;
	}

	
	if (m_VideoCodec != codec)
	{
		UpdateCodecSink((H5SCodecType)codec);
	}

	if (m_pVideoSink)
	{
		return m_pVideoSink->ProcessData(buffer, size, secs, msecs, (H5SCodecType)codec);
	}

	return true;
}

bool H5SLibRTSP::UpdateCodecSink(H5SCodecType codec)
{
	if (m_pVideoSink)
	{
		delete m_pVideoSink;
		m_pVideoSink = NULL;
	}
	switch (codec)
	{
		case H5S_CODEC_H264:
		{
			SPropRecord* sPropRecords = NULL;
			u_int8_t* sps = NULL; unsigned spsSize = 0;
			u_int8_t* pps = NULL; unsigned ppsSize = 0;
			/* Add parseSPropParameterSets to get pps & sps here  */
			H5SParameterMap::iterator it = m_pParamMap.begin(); 
			for(; it!=m_pParamMap.end(); ++it)
			{
				if ((*it).first == "sprop-parameter-sets")
				{
					unsigned numSPropRecords;
					sPropRecords = parseSPropParameterSets((*it).second.c_str(), 
									numSPropRecords);
					for (unsigned i = 0; i < numSPropRecords; ++i) {
						if (sPropRecords[i].sPropLength == 0) continue; // bad data
						u_int8_t nal_unit_type = (sPropRecords[i].sPropBytes[0])&0x1F;
						if (nal_unit_type == 7/*SPS*/) {
						  sps = sPropRecords[i].sPropBytes;
						  spsSize = sPropRecords[i].sPropLength;
						} else if (nal_unit_type == 8/*PPS*/) {
						  pps = sPropRecords[i].sPropBytes;
						  ppsSize = sPropRecords[i].sPropLength;
						}
					}
				}
			}
			printf("%s sps size %d pps %d\n", __FUNCTION__, spsSize, ppsSize);
			m_pVideoSink = new H5SH264Sink(*this, sps, spsSize, pps, ppsSize);
			if (sPropRecords)
			{
				delete[] sPropRecords;
			}
			break;
		}
		default:
		   	return false;
	};
	m_VideoCodec = codec;
	return true;
}

bool   H5SLibRTSP::onH5SData(unsigned char* buffer, int size, unsigned long long secs, 
			unsigned long long msecs, H5SCodecType codec, H5SStreamType stream, 
			H5SFrameType frm)
{
	//Live555DumpHex(buffer, 100);
	//printf("Function %s (%lld, %lld) type %d len %d\n", __FUNCTION__, secs, msecs, frm, size);

	std::lock_guard<std::mutex> guard(m_pCBLock);

	H5SCallbackMap::iterator it = m_pCBMap.begin(); 
	for(; it!=m_pCBMap.end(); ++it)
	{
		if ((*it).second)
		{
			(*it).second->onH5SData(buffer, size, secs, msecs, codec, stream, frm);;
		}
	}
	return true;
}


bool H5SLibRTSP::Thread(void* pData)
{
	H5SLibRTSP * pThread = (H5SLibRTSP *)pData;

	if (pThread)
	{
		return pThread->ThreadRun();
	}
	return false;
}

bool H5SLibRTSP::ThreadRun()
{
	Run();

	return true;
}
