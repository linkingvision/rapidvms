//------------------------------------------------------------------------------
// File: Event server impl.hpp
//
// Desc: Event server
//
// Copyright (c) 2014-2018 opencvr.com. All rights reserved.
//------------------------------------------------------------------------------

#ifndef __VSC_EVENT_SERVER_IMPL_H_
#define __VSC_EVENT_SERVER_IMPL_H_

inline VEventServerCallbackTask::VEventServerCallbackTask(Factory &pFactory)
:m_Factory(pFactory)
{
}

inline VEventServerCallbackTask::~VEventServerCallbackTask()
{

}

inline void VEventServerCallbackTask::PushEvent(VEventData &pData)
{
	m_Queue.Push(pData);

	return;
}
/* Register the call back for the event */
inline BOOL VEventServerCallbackTask::RegEventNotify(void * pData, FunctionEventNotify callback)
{
	XGuard guard(m_cMutex);
	m_NotifyMap[pData] = callback;

	return TRUE;
}
inline BOOL VEventServerCallbackTask::UnRegEventNotify(void * pData)
{
	XGuard guard(m_cMutex);
	m_NotifyMap.erase(pData);

	return TRUE;
}

inline void VEventServerCallbackTask::run()
{
	while(m_Queue.BlockingPeek() == true)
	{
		VEventData sData = m_Queue.Pop();
		VDC_DEBUG( "%s Pop a Event \n",__FUNCTION__);
		/* Call the callback */
		XGuard guard(m_cMutex);
		FunctionEventNotifyMap::iterator it = m_NotifyMap.begin(); 
		for(; it!=m_NotifyMap.end(); ++it)
		{
			if ((*it).second)
			{
				(*it).second((*it).first, sData);
			}
		}
	}
}

inline VEventServerDbTask::VEventServerDbTask(Factory &pFactory)
:m_Factory(pFactory)
{
}

inline VEventServerDbTask::~VEventServerDbTask()
{

}

inline void VEventServerDbTask::PushEvent(VEventData &pData)
{
	m_Queue.Push(pData);

	return;
}

inline void VEventServerDbTask::run()
{
	//m_Factory.GetEventDBPath();
	/* Check if need  create new DB file*/
	while(m_Queue.BlockingPeek() == true)
	{
		VEventData sData = m_Queue.Pop();
		VDC_DEBUG( "%s Pop a Event \n",__FUNCTION__);
		/* Write the Data to DB */
	}
}
		


inline VEventServer::VEventServer(Factory &pFactory)
:m_Factory(pFactory), m_DbTask(pFactory), m_CallbackTask(pFactory)
{
}

inline VEventServer::~VEventServer()
{

}

inline void VEventServer::PushEvent(VEventData &pData)
{
	m_DbTask.PushEvent(pData);
	m_CallbackTask.PushEvent(pData);

	return;
}
/* Register the call back for the event */
inline BOOL VEventServer::RegEventNotify(void * pData, FunctionEventNotify callback)
{
	return m_CallbackTask.RegEventNotify(pData, callback);
}
inline BOOL VEventServer::UnRegEventNotify(void * pData)
{
	return m_CallbackTask.UnRegEventNotify(pData);
}

inline BOOL VEventServer::Init()
{
	m_DbTask.start();
	m_CallbackTask.start();

	return TRUE;
}


#endif // __VSC_EVENT_SERVER_IMPL_H_
