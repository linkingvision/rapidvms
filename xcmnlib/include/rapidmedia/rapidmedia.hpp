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
	BOOL Init(BOOL bRealStream, string strUrl, string strUser, 
				string strPass, BOOL bHWAccel = FALSE);
	BOOL PutData(VideoFrame& packet);
	void	SetDataHandler(RMDataHandler handle, void *pContext);
	void	SetRawDataHandler(RMRawVideoHandler handle, void *pContext);

	BOOL Start();
	BOOL Stop();

public:
	RapidMediaData * m_data;
};
#endif /* __V_RAPID_MEDIA_HPP__ */
