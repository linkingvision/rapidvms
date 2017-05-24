#include "onvifagentlib.hpp"

static OnvifAgentLib gOnvifAgentLib;

#ifndef _WIN32
#include <unistd.h>
void sig_handler(int signo)
{
	printf("signo %d\n", signo);
	if (signo == SIGINT
	|| signo == SIGKILL
	|| signo == SIGTERM)
	{
		gOnvifAgentLib.SetExit();
		usleep(1000 * 100);
		system("killall onvifagent");
	}
	_exit(signo);
}
#endif

bool OnvifAgentLib::Initialize()
{
#ifndef _WIN32
	//signal(SIGUSR1, sig_handler);
	signal(SIGINT, sig_handler);
	signal(SIGKILL, sig_handler);
	
	signal(SIGTERM, sig_handler);
#endif
#if defined(_WIN32) && !defined(__SYMBIAN32__)
	WSADATA data;
	WSAStartup(MAKEWORD(2, 2), &data);
#endif 	/* _WIN32 && !__SYMBIAN32__ */
	/* Start the agent process */
	m_watch = new std::thread(OnvifAgentLib::WatchThread, this);

	return true;
}
bool OnvifAgentLib::UnInitialize()
{
	//std::lock_guard<std::mutex> guard(m_lock);
	m_bExit = true;

	m_watch->join();
	delete m_watch;
	if (m_process)
	{
		m_process->kill(true);
	}
	return true;
}