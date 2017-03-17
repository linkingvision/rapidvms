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
#ifndef __EPOLL_WRAPPER_HPP__
#define __EPOLL_WRAPPER_HPP__

#include <sys/epoll.h>
#include <assert.h>


typedef int (*Function_EpollHandler) (int);

#define  MAX_EPOLL_EVENT_NUMBER  1024

class EpollWrapper
{
public:
	EpollWrapper(int iFDSize = 256)
	:m_iEpollWaitingTime(1000)
	{
		EpollCreate(iFDSize);
	}
	~EpollWrapper()
	{
		close(m_iEpollFD);
	}
public:

	void SetHandler_Error(Function_EpollHandler pfError);
	void SetHandler_Read(Function_EpollHandler pfRead);
	void SetHandler_Write(Function_EpollHandler pfWrite);

public:

	int EpollCreate(int iFDSize);
	int EpollWait(int iTimeout);
	bool FDIsSet(int iFD, int iEpollEventNumber, unsigned int &uiEpollEvent);

	int EpollAdd(int iFD);
	int EpollDelete(int iFD);

	int SetNonBlock(int iFD);
    	int SetNagleOff(int iFD);

	inline void SetEpollWaitingTime(int iEpollWaitingTime);

	bool IsErrorEvent(unsigned int uiEvent, int iFD);
	bool IsReadEvent(unsigned int uiEvent);
	bool IsWriteEvent(unsigned int uiEvent);

	int NotifyErrorEvent(int iFD);
	int NotifyReadEvent(int iFD);
	int NotifyWriteEvent(int iFD);
public:
	epoll_event m_lastEpollEvent[MAX_EPOLL_EVENT_NUMBER];

private:

	Function_EpollHandler m_pfError;
	Function_EpollHandler m_pfRead;
	Function_EpollHandler m_pfWrite;

private:

	int m_iEpollFD;
	int m_iEpollEventSize;
	int m_iEpollWaitingTime;
	epoll_event m_stOneEpollEvent;

};

//inline void EpollWrapper::SetEpollEventSize(int iEpollEventSize)
//{
//	m_iEpollEventSize = iEpollEventSize;
//}

inline void EpollWrapper::SetEpollWaitingTime(int iEpollWaitingTime)
{
	m_iEpollWaitingTime = iEpollWaitingTime;
}



#endif
