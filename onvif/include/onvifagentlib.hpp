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
 #ifndef __ONVIF_AGENT_LIB__
#define __ONVIF_AGENT_LIB__
#if defined(_WIN32) && !defined(__SYMBIAN32__) 
#include <winsock2.h> 
#include <windows.h>
#endif

#ifndef _WIN32
#include <unistd.h>
#endif

#include <string.h>
#include <vector>
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <map>

#include "onvifagentcli.hpp"
#include "process.hpp"

class  OnvifAgentLib
{
public:
	OnvifAgentLib()
	:m_process(NULL), m_watch(NULL), m_bExit(false)
	{
		Initialize();
#ifndef _WIN32
		usleep(500 * 1000);
#endif
	}
	~OnvifAgentLib()
	{	
		UnInitialize();
	}
public:
	bool Initialize();
	bool UnInitialize();
	bool SetExit()
	{
		m_bExit = true;
		return true;
	}
public:
	static void WatchThread(void *user_data)
	{	
		OnvifAgentLib* pThread = (OnvifAgentLib*)user_data;

		if (pThread)
		{
			return pThread->WatchThread1();
		}
		return;
	}
	void WatchThread1()

	{
		while(!m_bExit)
		{
			{
				std::lock_guard<std::mutex> guard(m_lock);
				if (m_process)
				{
					delete m_process;
				}
#ifdef _WIN32
				Process::string_type strCmd = L"onvifagent.exe";
#else
				Process::string_type strCmd = "onvifagent";
#endif
				m_process = new Process(strCmd);
			}
		
			while(!m_bExit)
			{
				std::chrono::milliseconds dura(20);
				std::this_thread::sleep_for( dura );

				if (m_process->get_exit_status() == 0)
				{
					continue;
				}else
				{
					printf("%s %d onvif Agent process exit !!! \n", __FILE__, __LINE__);
					break;
				}
			
			}
		}
		return;
	}	
private:
	Process *m_process;
	std::thread *m_watch;
	std::mutex m_lock;
	bool m_bExit;
};


#endif /* __ONVIF_AGENT_LIB__ */
