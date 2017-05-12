#ifndef __ONVIF_AGENT_LIB__
#define __ONVIF_AGENT_LIB__
#if defined(_WIN32) && !defined(__SYMBIAN32__) 
#include <winsock2.h> 
#include <windows.h>
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
	:m_process(NULL), m_watch(NULL)
	{
		Initialize();
	}
	~OnvifAgentLib()
	{	
		UnInitialize();
	}
public:
	bool Initialize()
	{
#if defined(_WIN32) && !defined(__SYMBIAN32__)
		WSADATA data;
		WSAStartup(MAKEWORD(2, 2), &data);
#endif 	/* _WIN32 && !__SYMBIAN32__ */
		/* Start the agent process */
		m_watch = new std::thread(OnvifAgentLib::WatchThread, this);

		return true;
	}
	bool UnInitialize()
	{
		std::lock_guard<std::mutex> guard(m_lock);
		if (m_process)
		{
			m_process->kill(true);
		}
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
		while(1)
		{
			{
				std::lock_guard<std::mutex> guard(m_lock);
				if (m_process)
				{
					delete m_process;
				}
				Process::string_type strCmd = L"onvifagent.exe";
				m_process = new Process(strCmd);
			}
		
			while(1)
			{
				std::chrono::milliseconds dura( 1000 );
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
};


#endif /* __ONVIF_AGENT_LIB__ */
