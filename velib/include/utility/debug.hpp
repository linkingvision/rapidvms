#ifndef _V_DEBUG_H_
#define _V_DEBUG_H_

#include "utility.hpp"
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

struct cli_def;
class VE_LIBRARY_API Debug 
{
public:
	static void run(void * pParam);
	void run1();

public:
	static void init(s32 nPort);
	static void DebugPrint( const char* format, ... );


	
public:
	void setup();
	
	static int check_auth(const char *username, const char *password);
    static int check_enable(const char *password);
	

public:
	static int test(struct cli_def *cli, const char *command, 
		char *argv[], int argc);

	
private:
	static Debug * gDebug;
	static thread *gThread;
public:
    struct cli_def *cli;

};


#define  VDC_DEBUG Debug::DebugPrint

#endif