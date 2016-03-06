#ifndef __PB_WRAPPER_HPP__
#define __PB_WRAPPER_HPP__
#include "utility.hpp"
#include "mediafile.hpp"
#include "pbsession.hpp"

#ifdef WIN32
/* pb callback */
typedef void (__cdecl * PBCallbackFunctionPtr)(VideoFrame& frame, void * pParam);
typedef void (__cdecl * PBTimeCallbackFunctionPtr)(u32 currTime, bool bPause, void * pParam);
#else
/* pb callback */
typedef void ( * PBCallbackFunctionPtr)(VideoFrame& frame,  void * pParam);
typedef void ( * PBTimeCallbackFunctionPtr)(u32 currTime, bool bPause, void * pParam);
#endif


class VDB;
class PlaybackWrapperImpl;
class VE_LIBRARY_API PlaybackWrapper
{
public:
    PlaybackWrapper(VDB &pVdb, astring deviceId, u32 nPlaytime, PBCallbackFunctionPtr callback, 
				BOOL eolLoop, void * pParam);
    ~PlaybackWrapper();
public:
	BOOL SeekToTime(u32 seekTime);
	BOOL StartPlay();
	BOOL StopPlay();/* same with pause */
	BOOL QuitPlay();/* End of play */
	BOOL SetSpeed(float fSpeed);
	BOOL SetDirection(BOOL bForward);
	BOOL SetTimeCallback(PBTimeCallbackFunctionPtr callback, void * pParam);
	
public:
	/* main loop for playback */
	void run();
	
private:
	PlaybackWrapperImpl *m_pImpl;
};

#endif /* __PB_WRAPPER_HPP__ */
