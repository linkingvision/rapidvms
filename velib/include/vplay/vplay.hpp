#ifndef __V_PLAY_HPP__
#define __V_PLAY_HPP__
#include "utility.hpp"
#include "debug.hpp"
#include "utility/videotype.hpp"

using namespace UtilityLib;

typedef BOOL (*VPlayDataHandler)(void* pData, VideoFrame& packet);
typedef BOOL (*VPlayPBTimeCallback)(void* pData, int time);

typedef BOOL (*VPlayRawFrameHandler)(void* pData, RawFrame& frame);

typedef enum __VPlayCmd
{
	VPLAY_CMD_PLAY = 1,
	VPLAY_CMD_PAUSE,
	VPLAY_CMD_SPEED,
	VPLAY_CMD_MOVE,
	VPLAY_CMD_LAST
}VPlayCmd;

typedef enum __RenderType
{
	RENDER_TYPE_SDL = 1,
	RENDER_TYPE_D3D,
	RENDER_TYPE_OPENGL,
	RENDER_TYPE_DDRAW,
	RENDER_TYPE_VAAPI,
	RENDER_TYPE_LAST
}RenderType;

typedef struct __VPlayCmdParam
{
	int speed;
	int moveto;
}VPlayCmdParam;

class VPlayData;
class VE_LIBRARY_API VPlay
{
public:

	VPlay();
	~VPlay();
public:
	static BOOL SetLicense(astring &strLicense);
	static BOOL GetLicenseInfo(astring &strHostId, int &ch, astring &type, astring &expireTime);
public:
	BOOL Init(BOOL bRealStream, string strUrl, string strUser, string strPass, 
		BOOL bHWAccel = FALSE, VSCConnectType connectType = VSC_CONNECT_TCP);
	BOOL Init(string strFile, bool bMJPEG, string strUser, 
		string strPass, BOOL bHWAccel = FALSE);
#ifdef WIN32
	BOOL AttachWidget(HWND hWnd, int w, int h, RenderType render = RENDER_TYPE_D3D);
#else
	BOOL AttachWidget(HWND hWnd, int w, int h, RenderType render = RENDER_TYPE_SDL);
#endif
	BOOL UpdateWidget(HWND hWnd, int w, int h);
	BOOL DetachWidget(HWND hWnd);
	BOOL SetPbTimeCallback(HWND hWnd, void * pData, VPlayPBTimeCallback callback);
	BOOL Control(VPlayCmd cmd, VPlayCmdParam param);
	BOOL EnableMot(HWND hWnd, bool enable);

	BOOL StartGetData(void * pData, VPlayDataHandler callback);
	BOOL StopGetData();
	BOOL StartGetRawFrame(void * pData, VPlayRawFrameHandler callback);
	BOOL StopGetRawFrame();
	BOOL PutRawData(VideoFrame& packet);
	BOOL ShowAlarm(HWND hWnd);
	BOOL GetStreamInfo(VideoStreamInfo &pInfo);
	

public:
    VPlayData * m_data;
};
#endif /* __V_PLAY_HPP__ */
