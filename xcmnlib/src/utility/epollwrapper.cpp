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
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include "epollwrapper.hpp"
#include <errno.h>

void EpollWrapper::SetHandler_Error(Function_EpollHandler pfError)
{
	m_pfError = pfError;
}

void EpollWrapper::SetHandler_Read(Function_EpollHandler pfRead)
{
	m_pfRead = pfRead;
}

void EpollWrapper::SetHandler_Write(Function_EpollHandler pfWrite)
{
	m_pfWrite = pfWrite;
}



int EpollWrapper::EpollCreate(int iFDSize)
{
	m_iEpollEventSize = iFDSize;
	m_iEpollFD = epoll_create(m_iEpollEventSize);
	if(m_iEpollFD < 0)
	{
		return -1;
	}

	memset(&m_stOneEpollEvent, 0, sizeof(m_stOneEpollEvent));
	m_stOneEpollEvent.events = EPOLLIN | EPOLLOUT | EPOLLERR | EPOLLHUP;
	//m_stOneEpollEvent.events = EPOLLIN | EPOLLERR | EPOLLHUP;
	m_stOneEpollEvent.data.ptr = NULL;
	m_stOneEpollEvent.data.fd  = -1;

	return 0;
}

bool EpollWrapper::FDIsSet(int iFD, int iEpollEventNumber, unsigned int &uiEpollEvent)
{
	int i;
	for(i = 0; i < iEpollEventNumber; ++i)
	{
		if (iFD == m_lastEpollEvent[i].data.fd)
		{
			uiEpollEvent = m_lastEpollEvent[i].events;
			return true;
		}
	}

	return false;
}

int EpollWrapper::EpollWait(int iTimeout)
{
	int iEpollTimeout = iTimeout;
	int iEpollEventNumber;

	if (iEpollTimeout <= 0)
	{
		iEpollTimeout = m_iEpollWaitingTime;
	}
	
	
	iEpollEventNumber = epoll_wait(m_iEpollFD, &m_lastEpollEvent[0], m_iEpollEventSize, iEpollTimeout);
	if(iEpollEventNumber < 0)
	{
		if (errno != EINTR)
		{
			printf("%s epoll_wait Err!Errno=%d,ErrStr=%s\n", __FUNCTION__, 
										errno, strerror(errno));
		}
		
		return -1;
	}
	return iEpollEventNumber;
#if 0	
	int i;
	int iFD;
	unsigned int uiEpollEvent;
	for(i = 0; i < iEpollEventNumber; ++i)
	{
		iFD = m_lastEpollEvent[i].data.fd;
		uiEpollEvent = m_lastEpollEvent[i].events;
		
		if(IsReadEvent(uiEpollEvent))
		{
			NotifyReadEvent(iFD);
		}
		else if(IsWriteEvent(uiEpollEvent))
		{
			NotifyWriteEvent(iFD);
		}
		else if(IsErrorEvent(uiEpollEvent, iFD))
		{
			NotifyErrorEvent(iFD);
		}
	}

	return 0;
#endif
}

int EpollWrapper::EpollAdd(int iFD)
{
	int iRet = 0;
	m_stOneEpollEvent.data.fd = iFD;
	iRet = epoll_ctl(m_iEpollFD, EPOLL_CTL_ADD, iFD, &m_stOneEpollEvent);
	if(iRet < 0)
	{
		return -1;
	}
	return 0;
}

int EpollWrapper::EpollDelete(int iFD)
{
	int iRet = 0;
	iRet = epoll_ctl(m_iEpollFD, EPOLL_CTL_DEL, iFD, &m_stOneEpollEvent);
	if(iRet < 0)
	{
		return -1;
	}
	return 0;
}

bool EpollWrapper::IsErrorEvent(unsigned int uiEvent, int iFD)
{
	if((EPOLLERR | EPOLLHUP) & uiEvent)
	{
		printf("ErrorEvent: FD = %d, event = %u, error = %d, hup = %d, errno = %d\n", 
			iFD, uiEvent, uiEvent & EPOLLERR, uiEvent & EPOLLHUP, errno);
		return true;
	}
	return false;
}

bool EpollWrapper::IsReadEvent(unsigned int uiEvent)
{
	if((EPOLLIN) & uiEvent)
	{
		return true;
	}
	return false;
}

bool EpollWrapper::IsWriteEvent(unsigned int uiEvent)
{
	if((EPOLLOUT) & uiEvent)
	{
		return true;
	}
	return false;
}

int EpollWrapper::NotifyErrorEvent(int iFD)
{
	(*m_pfError)(iFD);
	return 0;
}

int EpollWrapper::NotifyReadEvent(int iFD)
{
	(*m_pfRead)(iFD);
	return 0;
}

int EpollWrapper::NotifyWriteEvent(int iFD)
{
	(*m_pfWrite)(iFD);
	return 0;
}

//////////////////////////////////////////////////////////////////////////

int EpollWrapper::SetNonBlock(int iFD)
{
#ifdef WIN32
	unsigned int uiNonBlock = 1;
	ioctlsocket(iFD, FIONBIO, &uiNonBlock);
#else
	int iFlags;
	iFlags = fcntl(iFD, F_GETFL, 0);
	iFlags |= O_NONBLOCK;
	//iFlags |= O_NDELAY;
	fcntl(iFD, F_SETFL, iFlags);
#endif
	return 0;
}

int EpollWrapper::SetNagleOff(int iFD)
{
    /* Disable the Nagle (TCP No Delay) algorithm */ 
    int flag = 1; 

    setsockopt(iFD, IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof(flag));
    return 0;
}
