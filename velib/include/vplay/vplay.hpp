#ifndef __V_PLAY_HPP__
#define __V_PLAY_HPP__
#include "utility.hpp"
#include "debug.hpp"
#include "videotype.hpp"

using namespace UtilityLib;

typedef BOOL (*VPlayDataHandler)(void* pData, VideoFrame& packet);
typedef BOOL (*VPlayPBTimeCallback)(void* pData, int time);

typedef enum __VPlayCmd
{
	VPLAY_CMD_PLAY = 1,
	VPLAY_CMD_PAUSE,
	VPLAY_CMD_SPEED,
	VPLAY_CMD_MOVE,
	VPLAY_CMD_LAST
}VPlayCmd;

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
	static BOOL GetLicenseInfo(astring &strHostId, int &ch, int &type);
public:
	BOOL Init(BOOL bRealStream, string strUrl, string strUser, string strPass);
	BOOL AttachWidget(HWND hWnd, int w, int h);
	BOOL UpdateWidget(HWND hWnd, int w, int h);
	BOOL DetachWidget(HWND hWnd);
	BOOL SetPbTimeCallback(HWND hWnd, void * pData, VPlayPBTimeCallback callback);
	BOOL Control(VPlayCmd cmd, VPlayCmdParam param);
	BOOL EnablePtz(HWND hWnd, bool enable);
	BOOL DrawPtzDirection(HWND hWnd, int x1, int y1, int x2,  int y2);
	BOOL ClearPtzDirection(HWND hWnd);

	BOOL StartGetData(void * pData, VPlayDataHandler callback);
	BOOL StopGetData();
	BOOL PutRawData(VideoFrame& packet);
	BOOL ShowAlarm(HWND hWnd);
	

public:
    VPlayData * m_data;
};
#endif /* __V_PLAY_HPP__ */
