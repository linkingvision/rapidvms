#ifndef _V_DEBUG_H_
#define _V_DEBUG_H_

#include "utility.hpp"
#include "spdlog/spdlog.h"
#define NOMINMAX

#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#include "shellapi.h"
#endif
#include <string>

#define MODE_CONFIG_INT             10

using namespace UtilityLib;
using namespace tthread;

namespace spd = spdlog;

struct cli_def;
class VE_LIBRARY_API Debug 
{
public:
	Debug(s32 nPort, std::string strLoggerPath)
	:m_nPort(nPort), m_strLoggerPath(strLoggerPath)
	{
		spd::set_level(spd::level::debug);
		m_pLogger = spd::rotating_logger_mt("OpenCVR", m_strLoggerPath, 1024 * 1024 * 5, 3, true);
	}
public:
	static void run(void * pParam);
	void run1();
	spd::logger  & getLogger();

public:
	static void init(s32 nPort, std::string strLoggerPath);
	static void DebugPrint( const char* format, ... );
	static spd::logger & logger();

	
public:
	void setup();
	
	static int check_auth(const char *username, const char *password);
    static int check_enable(const char *password);
	

public:
	static int test(struct cli_def *cli, const char *command, 
		char *argv[], int argc);

	
private:
	static Debug * gDebug;
	static tthread::thread *gThread;
public:
    struct cli_def *cli;
	s32 m_nPort;
	std::shared_ptr<spd::logger> m_pLogger;
	std::string m_strLoggerPath;

};


#define  VDC_DEBUG Debug::DebugPrint

#endif