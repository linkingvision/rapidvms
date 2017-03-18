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
#include "recordwrapper.hpp"
#include "XSDK/XMutex.h"
#include "XSDK/XGuard.h"
#include "videotype.hpp"
#include "vdb.hpp"


using namespace UtilityLib;
using namespace XSDK;
using namespace Poco;


/* A thread get the queue of the tcmalloc queue, and take care of the RecordSchedWeek 
   use POCO DateTime.h to update the schedule.
*/
class RecordWrapperImpl 
{
public:
	RecordWrapperImpl(VDB &pVdb, astring deviceId, 
		RecordSchedWeekMap cSchedMap, RecChangeFunctionPtr pCallback, void *pParam);
	~RecordWrapperImpl();

public:
	bool SetPrePostRecordTime(s32 nPreTime, s32 nPostTime);
	MFStatus PushAFrame(VideoFrame &frame);
	bool UpdateSchedMap(RecordSchedWeekMap cSchedMap);
	bool ApplySched(std::time_t pTime);
	bool StopRecord();
	bool FireAlarm(s64 nStartTime);
	bool ManualRecStart();
	bool ManualRecStop();
	
private:
	XMutex m_cMutex;
	VDB &m_pVdb;
	astring m_deviceId; 
	RecordSession *m_pRecord;
	bool m_SchedChanged;/* if sched changed or :00 time, change the new sched */
	RecordSchedWeekMap m_cSchedMap;
	RecordSchedWeek m_cSchedMerge;
	u32 m_nCurrRectype;
	RecChangeFunctionPtr m_pCallback;
	void *m_pParam;
	bool m_bManualStart;
	s32 m_nPreTime;
	s32 m_nPostTime;
};

RecordWrapperImpl::RecordWrapperImpl(VDB &pVdb, astring deviceId, 
	RecordSchedWeekMap cSchedMap, RecChangeFunctionPtr pCallback, void *pParam)
: m_pVdb(pVdb), m_deviceId(deviceId), m_pRecord(NULL), m_SchedChanged(true), 
m_cSchedMap(cSchedMap), m_nCurrRectype(R_OFF), m_pCallback(pCallback), 
m_pParam(pParam), m_bManualStart(false), m_nPreTime(60), m_nPostTime(120)
{
	UpdateSchedMap(m_cSchedMap);
}

/* Set the pre record time of each device, the time is in second */
bool RecordWrapperImpl::SetPrePostRecordTime(s32 nPreTime, s32 nPostTime)
{
	m_nPreTime = nPreTime;
	m_nPostTime = nPostTime;
	return TRUE;
}


RecordWrapperImpl::~RecordWrapperImpl()
{
	StopRecord();	
}

bool RecordWrapperImpl::ApplySched(std::time_t pTime)
{
	Timestamp pTimeStamp = Timestamp::fromEpochTime(pTime);

	/* Use next sec to check  */
	DateTime pTimeTime(pTimeStamp + 1);
	LocalDateTime pLocalTime(pTimeTime);

	int nDayOfWeek = pLocalTime.dayOfWeek() -1;
	if (nDayOfWeek < 0)
	{
		nDayOfWeek = VDB_SUN;
	}
	int nHourOfDay = pLocalTime.hour();
	if (nDayOfWeek < 0 || nDayOfWeek >= VDB_DAYS_IN_WEEK
	||nHourOfDay < 0 ||  nHourOfDay >= VDB_HOURS_IN_DAY)
	{
		return false;
	}

	int nNextRectype = m_cSchedMerge.week[nDayOfWeek].type[nHourOfDay];

	/* manual is highest priority */
	if (m_bManualStart == true)
	{
		m_pVdb.AddSchedItem(m_deviceId, pTime, pTime + 3600, R_MANUAL);

	}else
	{
		/* Record the sched */
		if ((RecordingType)nNextRectype == R_SCHED)
		{
			m_pVdb.AddSchedItem(m_deviceId, pTime, pTime + 3600, (RecordingType)nNextRectype);
		}
	}

	/* Don't need stop, just keep record  */
	if (m_nCurrRectype != nNextRectype)
	{
		//StopRecord();
		m_nCurrRectype = nNextRectype;
	}
}

bool RecordWrapperImpl::StopRecord()
{
	XGuard guard(m_cMutex);

	m_pCallback(m_deviceId, false, m_pParam);
	if (m_pRecord)
	{
	    u32 endTime = m_pRecord->GetEndTime();
	    if (endTime != 0)
	    {
	    	 m_pVdb.FinishRecord(m_pRecord);
	    }
	    delete m_pRecord;
	    m_pRecord = NULL;
	}
	

	return true;
}

bool RecordWrapperImpl::FireAlarm(s64 nStartTime)
{	
	XGuard guard(m_cMutex);
	std::time_t pTime = time(NULL);
	std::time_t pStart = pTime - m_nPreTime;
	std::time_t pEnd = pTime + 120 +  m_nPostTime;

	m_pVdb.AddSchedItem(m_deviceId, pStart, pEnd, R_ALARM);
	//m_pVdb.AddAlarmItem(m_deviceId, nStartTime, nStartTime + 60);
	return true;
}

bool RecordWrapperImpl::ManualRecStart()
{
	XGuard guard(m_cMutex);
	if (m_bManualStart == true)
	{
		return true;
	}
	m_bManualStart = true;

	m_SchedChanged = true;
	return true;
}
bool RecordWrapperImpl::ManualRecStop()
{
	XGuard guard(m_cMutex);
	if (m_bManualStart == false)
	{
		return true;
	}
	m_bManualStart = false;

	m_SchedChanged = true;
	return true;
}

bool RecordWrapperImpl::UpdateSchedMap(RecordSchedWeekMap cSchedMap)
{
	XGuard guard(m_cMutex);

	RecordSchedWeek cSchedMerge;
	

	RecordSchedWeekMap::iterator it = cSchedMap.begin(); 
	for(; it!=cSchedMap.end(); ++it)
	{
		cSchedMerge = cSchedMerge + (*it).second;
	}

	m_cSchedMap = cSchedMap;
	m_cSchedMerge = cSchedMerge;
	
	m_SchedChanged = true;

	return true;
}

MFStatus RecordWrapperImpl::PushAFrame(VideoFrame &frame)
{
	XGuard guard(m_cMutex);
	
	MFStatus ret =  MF_ERROR;

	std::time_t pTime = time(NULL);

	if ((pTime + 2)%3600 == 0 
		|| m_SchedChanged == true)
	{
		ApplySched(pTime + 2);
		m_SchedChanged = false;
	}

	if (m_nCurrRectype == R_OFF && m_bManualStart != true)
	{
		if (m_pRecord)
		{
			StopRecord();
		}
		return MF_OK;
	}
	
	if (m_pRecord == NULL)
	{
	    	m_pRecord = m_pVdb.StartRecord(m_deviceId, (int)(frame.secs), R_SCHED);
		m_pCallback(m_deviceId, true, m_pParam);
	}
	
	//VDC_DEBUG("Recording Size %d stream %d frame %d (%d, %d)\n", frame.dataLen,      
	// 	frame.streamType, frame.frameType, frame.secs, frame.msecs);
	
	/* Just skip the info stream for recording */
	if (m_pRecord != NULL 
	&& frame.streamType != VIDEO_STREAM_INFO && m_pRecord->PushAFrame(&frame) == MF_WRTIE_REACH_END)
	{
		u32 endTime = m_pRecord->GetEndTime();
		if (endTime != 0)
		{
			m_pVdb.FinishRecord(m_pRecord);
		}
	    	delete m_pRecord;
	    	m_pRecord = m_pVdb.StartRecord(m_deviceId, (int)(frame.secs), R_SCHED);
		if (m_pRecord != NULL)
		{
		       ret = m_pRecord->PushAFrame(&frame);	 
		}
	}	

	return ret;
}

RecordWrapper::RecordWrapper(VDB &pVdb, astring deviceId, RecordSchedWeekMap cSchedMap,
	 RecChangeFunctionPtr pCallback, void *pParam)
: m_pImpl(new RecordWrapperImpl(pVdb, deviceId, cSchedMap, pCallback, 
	pParam))
{

}
RecordWrapper::~RecordWrapper()
{
	delete m_pImpl;
}

bool RecordWrapper::UpdateSchedMap(RecordSchedWeekMap cSchedMap)
{
	return m_pImpl->UpdateSchedMap(cSchedMap);
}

MFStatus RecordWrapper::PushAFrame(VideoFrame &frame)
{
	return m_pImpl->PushAFrame(frame);
}

bool RecordWrapper::FireAlarm(s64 nStartTime)
{
	return m_pImpl->FireAlarm(nStartTime);
}

bool RecordWrapper::ManualRecStart()
{
	return m_pImpl->ManualRecStart();
}
bool RecordWrapper::ManualRecStop()
{
	return m_pImpl->ManualRecStop();
}

bool RecordWrapper::SetPrePostRecordTime(s32 nPreTime, s32 nPostTime)
{
	return m_pImpl->SetPrePostRecordTime(nPreTime, nPostTime);
}

