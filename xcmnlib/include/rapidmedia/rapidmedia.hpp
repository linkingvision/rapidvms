#ifndef __V_RAPID_MEDIA_HPP__
#define __V_RAPID_MEDIA_HPP__
#include "cppkit/os/ck_exports.h"
#include "utility/type.hpp"
#include "utility/videotype.hpp"

using namespace std;

typedef BOOL (*RMDataHandler)(void* pContext, VideoFrame& packet);
typedef BOOL (*RMRawVideoHandler)(void* pContext, RawFrame& packet);

class RapidMediaData;
class VE_LIBRARY_API RapidMedia
{
public:
	RapidMedia();
	~RapidMedia();
public:
	/* The Decode one i frame  */
	static bool DecodeiFrame(VideoFrame& packet, RawFrame& pRaw);
public:
	BOOL Init(BOOL bRealStream, string strUrl, string strUser, 
				string strPass, BOOL bHWAccel = FALSE, VSCConnectType connectType = VSC_CONNECT_TCP);
	BOOL PutData(VideoFrame& packet);
	void	SetDataHandler(RMDataHandler handle, void *pContext);
	void	SetRawDataHandler(RMRawVideoHandler handle, void *pContext);

	BOOL Start();
	BOOL Stop();
	BOOL StartRaw();
	BOOL StopRaw();
public:
	static std::string GetCodecName(CodecType codec);

public:
	RapidMediaData * m_data;
};
#endif /* __V_RAPID_MEDIA_HPP__ */
