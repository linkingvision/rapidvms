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
#include "pbwrapper.hpp"
#include "videotype.hpp"
#include "vdb.hpp"

#define PB_WRAPPER_DEBUG 1

class PlaybackWrapperImpl 
{
public:
	PlaybackWrapperImpl(VDB &pVdb, astring deviceId, u32 nPlaytime, 
				PBCallbackFunctionPtr callback, 
				BOOL eolLoop, void * pParam);
	~PlaybackWrapperImpl();
public:
	void Lock(){m_Lock.lock();}
	void UnLock(){m_Lock.unlock();}

public:
	BOOL SeekToTime(u32 seekTime);
	BOOL StartPlay();
	BOOL PausePlay();
	BOOL QuitPlay();
	BOOL SetSpeed(float fSpeed);
	BOOL SetDirection(BOOL bForward);
	BOOL SetTimeCallback(PBTimeCallbackFunctionPtr callback, void * pParam);
	
public:
	/* main loop for playback */
	void run();

public:
	BOOL FeedAFrame(VideoFrame &pbFrame, BOOL bWaiting);
	BOOL MakeUpSessionNear(int seekTime);
	BOOL MakeUpSessionNext();
	BOOL MakeUpSession(int seekTime);
	
private:
	fast_mutex m_Lock;
	VDB &m_pVdb;
	astring m_deviceId; 
	VdbRecordItem m_recordItem;
	u32 m_nPlaytime; 
	PBCallbackFunctionPtr m_callback;
	void * m_pParam;

	PBTimeCallbackFunctionPtr m_timeCallback;
	void * m_pTimeParam;

	PlaybackSession *m_pbSession;
	VideoFrame m_pbFrame;

	BOOL m_Quit;
	BOOL m_Playing;

	s32 m_nLastSec;
	s32 m_nLastSeekTime;

	BOOL m_eolLoop;
	BOOL m_gotEol;
};

PlaybackWrapperImpl::PlaybackWrapperImpl(VDB &pVdb, astring deviceId, u32 nPlaytime, 
				PBCallbackFunctionPtr callback, 
				BOOL eolLoop, void * pParam)
: m_pVdb(pVdb), m_deviceId(deviceId), m_nPlaytime(nPlaytime), 
m_callback(callback), m_pParam(pParam), m_pbSession(NULL), 
m_Playing(FALSE), m_Quit(FALSE), m_eolLoop(eolLoop), m_gotEol(FALSE), m_timeCallback(NULL)
{
	m_pbFrame.dataBuf = (u8 *)malloc(1024 * 1024);
	m_nLastSec = nPlaytime;
	m_nLastSeekTime = nPlaytime;
}

PlaybackWrapperImpl::~PlaybackWrapperImpl()
{
	if (m_pbSession)
	{
	    	astring strBlockPath = m_pbSession->GetBlockPath();
		m_pVdb.FinishedAMFRead(m_recordItem, strBlockPath);
		delete m_pbSession;
		m_pbSession = NULL;
	}
	if (m_pbFrame.dataBuf)
	{
		free(m_pbFrame.dataBuf);
		m_pbFrame.dataBuf = NULL;
	}

}

BOOL PlaybackWrapperImpl::SeekToTime(u32 seekTime)
{
	astring strPath;

	m_nLastSeekTime = seekTime;
	Lock();

	if (m_timeCallback)
	{
		m_timeCallback(m_nLastSeekTime, TRUE, m_pTimeParam);
	}

	/* Step 1 */
	if (m_pbSession)
	{
	    if (m_pbSession->SeekToTime(seekTime) == FALSE)
	    {
	    	astring strBlockPath = m_pbSession->GetBlockPath();
		m_pVdb.FinishedAMFRead(m_recordItem, strBlockPath);
		delete m_pbSession;
		m_pbSession = NULL;
	    }else
	    {
	    	m_Playing = FALSE;
	    	FeedAFrame(m_pbFrame, FALSE);
		UnLock();
		return TRUE;
	    }
	}
	/* Step 2 */
	MakeUpSession(seekTime);
	if (m_pbSession)
	{
	    if (m_pbSession->SeekToTime(seekTime) == FALSE)
	    {
	    	/* Here can not happen */
		astring strBlockPath = m_pbSession->GetBlockPath();
		m_pVdb.FinishedAMFRead(m_recordItem, strBlockPath);
		delete m_pbSession;
		m_pbSession = NULL;
	    }else
	    {
	    	m_Playing = FALSE;
	    	FeedAFrame(m_pbFrame, FALSE);
		UnLock();
		return TRUE;
	    }
	}   

	UnLock();
	return TRUE;
}
BOOL PlaybackWrapperImpl::StartPlay()
{
	m_Playing = TRUE;
	return TRUE;
}
BOOL PlaybackWrapperImpl::PausePlay()
{
	Lock();
	m_Playing = FALSE;
	if (m_pbSession)
	{
		m_pbSession->Pause();
	}

	UnLock();

	return TRUE;
}

BOOL PlaybackWrapperImpl::QuitPlay()
{
	m_Quit = TRUE;
	return TRUE;
}

BOOL PlaybackWrapperImpl::SetTimeCallback(PBTimeCallbackFunctionPtr callback, void * pParam)
{
	Lock();

	m_timeCallback = callback;
	m_pTimeParam = pParam;

	UnLock();

	return TRUE;
}

BOOL PlaybackWrapperImpl::SetSpeed(float fSpeed)
{
	return TRUE;
}
BOOL PlaybackWrapperImpl::SetDirection(BOOL bForward)
{
	return TRUE;
}

BOOL PlaybackWrapperImpl::FeedAFrame(VideoFrame &pbFrame, 
				BOOL bWaiting)
{
	s32 nDataType;
	astring strPath;
	s32 nLastSec = 0;
	u32 nKeyFrame = 0;
	s32 waiting;
	if (m_pbSession)
	{
	         if (m_pbSession->GetAFrame(pbFrame, waiting) == MF_WRTIE_REACH_END
			|| pbFrame.secs == 0)
	         {
#ifdef PB_WRAPPER_DEBUG
			VDC_DEBUG("%s Session reach end\n", __FUNCTION__);
#endif
			MakeUpSessionNext();
			waiting = 0;
			return TRUE;
	         }
#ifdef PB_WRAPPER_DEBUG
	         //VDC_DEBUG("Read Size %d stream %d frame %d (%d, %d) waiting %d\n", pbFrame.dataLen,      
		//	 	pbFrame.streamType, pbFrame.frameType, pbFrame.secs, pbFrame.msecs, waiting);
#endif
		if (pbFrame.secs == 0)
		{
			return TRUE;
		}
		if (pbFrame.secs != m_nLastSec &&m_Playing == TRUE)
		{
			if (m_timeCallback)
			{
				m_timeCallback(m_nLastSeekTime, FALSE, m_pTimeParam);
			}
			m_nLastSec = pbFrame.secs;
		}
		m_nLastSeekTime = pbFrame.secs;
		if (bWaiting == TRUE)
		{
			if (waiting > 5000)
			{
				m_gotEol = TRUE;
			}
			if (waiting > 1000 || waiting < 0)
			{
				VDC_DEBUG("%s waiting %d !!! \n", __FUNCTION__, waiting);
				//waiting = 5;
			}
			
			ve_sleep(waiting);
#ifdef PB_WRAPPER_DEBUG
		   	VDC_DEBUG("%s waiting %d \n", __FUNCTION__, waiting);
#endif
		}
	       if (m_callback)
	       {
			m_callback(pbFrame, m_pParam);
	       }
	}else
	{
		MakeUpSessionNext();
		waiting = 0;
		return TRUE;
	}
	return TRUE;
}


BOOL PlaybackWrapperImpl::MakeUpSessionNear(int seekTime)
{
	astring strPath;
	if (m_pVdb.SearchAItem(m_deviceId, seekTime, m_recordItem) == TRUE)
	{
	       if (m_pVdb.RequestAMFRead(m_recordItem, strPath) == TRUE)
	      {
#ifdef PB_WRAPPER_DEBUG
			VDC_DEBUG("%s Create new pb session %s \n", __FUNCTION__, strPath.c_str());
#endif
			m_pbSession = new PlaybackSession(m_deviceId, strPath, TRUE);
	      }
	}else
	{
#ifdef PB_WRAPPER_DEBUG
		VDC_DEBUG("%s Can not Search Playtime %d \n", __FUNCTION__, seekTime);
		ve_sleep(500);
#endif        	
	}
	if (m_pbSession == NULL)
	{
		if (m_pVdb.SearchAItemNear(m_deviceId, seekTime, m_recordItem) == TRUE)
		{
		       if (m_pVdb.RequestAMFRead(m_recordItem, strPath) == TRUE)
		      {
#ifdef PB_WRAPPER_DEBUG
				VDC_DEBUG("%s Create new pb Near session %s \n", __FUNCTION__, strPath.c_str());
#endif
				m_pbSession = new PlaybackSession(m_deviceId, strPath, TRUE);
		      }
		}else
		{
#ifdef PB_WRAPPER_DEBUG
			VDC_DEBUG("%s Can not Search Near Playtime %d \n", __FUNCTION__, seekTime);
#endif        
			ve_sleep(500);
			m_gotEol = TRUE;
			return FALSE;
		}
	}

	return TRUE;
}


BOOL PlaybackWrapperImpl::MakeUpSessionNext()
{
    	astring strPath;
	if (m_pbSession)
	{
		astring strBlockPath = m_pbSession->GetBlockPath();
		m_pVdb.FinishedAMFRead(m_recordItem, strBlockPath);
		delete m_pbSession;
		m_pbSession = NULL;

		s64 lastId = m_recordItem.id;
		if (m_pVdb.SearchNextItem(m_deviceId, m_recordItem.id, m_recordItem) == FALSE)
		{
		 	return TRUE;
		}
		BOOL bSkiped = FALSE;
		if (m_recordItem.id != lastId + 1)
		{
			bSkiped = TRUE;
		}
		if (m_pVdb.RequestAMFRead(m_recordItem, strPath) == FALSE)
		{
		  	return TRUE;
		}
		m_pbSession = new PlaybackSession(m_deviceId, strPath, bSkiped);

		return TRUE;
	}else
	{
		MakeUpSessionNear(m_nLastSeekTime);
	}
}


BOOL PlaybackWrapperImpl::MakeUpSession(int seekTime)
{
	astring strPath;
	if (m_pVdb.SearchAItem(m_deviceId, seekTime, m_recordItem) == TRUE)
	{
		if (m_pVdb.RequestAMFRead(m_recordItem, strPath) == FALSE)
		{
		        return TRUE;
		}
		m_pbSession = new PlaybackSession(m_deviceId, strPath, TRUE);
	}
	
	return TRUE;
}

/* Loop for reading from disk */
void PlaybackWrapperImpl::run()
{
#ifdef PB_WRAPPER_DEBUG
	VDC_DEBUG("%s Playtime %d \n", __FUNCTION__, m_nPlaytime);
#endif

        if (MakeUpSessionNear(m_nPlaytime) == FALSE && m_eolLoop == TRUE)
        {
        	//return;
        }
	
        int i = 0;
        while(! m_Quit)
         {
        	
              while (!m_Quit  && m_Playing == TRUE)
              {
              	m_gotEol = FALSE;
			Lock();
			FeedAFrame(m_pbFrame, TRUE);
			UnLock();
			if (m_eolLoop == TRUE && m_gotEol == TRUE)
			{
				return;
			}
			
              }
		ve_sleep(100);
        }
}


PlaybackWrapper::PlaybackWrapper(VDB &pVdb, astring deviceId, u32 nPlaytime,
				PBCallbackFunctionPtr callback, 
				BOOL eolLoop, void * pParam)
: m_pImpl(new PlaybackWrapperImpl(pVdb, deviceId, nPlaytime, 
callback, eolLoop, pParam))
{

}

PlaybackWrapper::~PlaybackWrapper()
{
	if (m_pImpl)
	{
		delete m_pImpl;
	}
}

BOOL PlaybackWrapper::SeekToTime(u32 seekTime)
{
	return m_pImpl->SeekToTime(seekTime);
}
BOOL PlaybackWrapper::StartPlay()
{
	return m_pImpl->StartPlay();
}
BOOL PlaybackWrapper::PausePlay()
{
	return m_pImpl->PausePlay();
}

BOOL PlaybackWrapper::QuitPlay()
{
	return m_pImpl->QuitPlay();
}

BOOL PlaybackWrapper::SetSpeed(float fSpeed)
{
	return m_pImpl->SetSpeed(fSpeed);
}

BOOL PlaybackWrapper::SetDirection(BOOL fSpeed)
{
	return m_pImpl->SetDirection(fSpeed);
}

BOOL PlaybackWrapper::SetTimeCallback(PBTimeCallbackFunctionPtr callback, void * pParam)
{
	return m_pImpl->SetTimeCallback(callback, pParam);
}

void PlaybackWrapper::run()
{
	return m_pImpl->run();
}


