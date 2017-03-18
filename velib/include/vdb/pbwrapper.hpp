/** <!--
 *
 *  Copyright (C) 2017 veyesys support@veyesys.com
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  If you would like this software to be made available to you under an 
 *  alternate commercial license please email support@veyesys.com 
 *  for more information.
 *
 * -->
 */
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
    PlaybackWrapper(VDB &pVdb, astring deviceId, u32 nPlaytime, 
				PBCallbackFunctionPtr callback, 
				BOOL eolLoop, void * pParam);
    ~PlaybackWrapper();
public:
	BOOL SeekToTime(u32 seekTime);
	BOOL StartPlay();
	BOOL PausePlay();/* same with pause */
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
