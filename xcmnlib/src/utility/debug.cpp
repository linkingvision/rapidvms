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
#include "utility/debug.hpp"
#include "utility/libcli.h"
Debug * Debug::gDebug = NULL;
tthread::thread * Debug::gThread = NULL;

#define MAX_LOG_LEN	2048

static void internalNetInit()
{
#ifdef WIN32
	/* Start up the windows networking */
	WORD version_wanted = MAKEWORD(1,1);
	WSADATA wsaData;

	if ( WSAStartup(version_wanted, &wsaData) != 0 ) {
		printf("Couldn't initialize Winsock 1.1\n");
		return;
	}
#endif
}

void Debug::run(void * pParam)
{
	if (pParam)
	{
		Debug * pDebug = (Debug *)pParam;
		pDebug->run1();
	}
}
void Debug::run1()
{
	int s, x;
	   struct sockaddr_in addr;
	char on = 1;
	
	// listen to a socket
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket");
		return ;
	}
	setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(m_nPort);
	if (::bind(s, (struct sockaddr *) &addr, sizeof(addr)) < 0)
	{
		perror("bind");
		return ;
	}
	if (listen(s, 50) < 0)
	{
		perror("listen");
		return ;
	}
	//printf("Listening on %d\n", m_nPort);
	//printf("Login with x/x, enable with 'x'\n"); 
	while ((x = accept(s, NULL, 0)))
	{

		cli_loop(cli, x);
#ifdef WIN32
		closesocket(x);
#else
		close(x);
#endif
	}

	return ;	
}

void Debug::init(s32 nPort, std::string strLoggerPath)
{
	if (gDebug != NULL)
	{
		return;
	}
	internalNetInit();
	gDebug = new Debug(nPort, strLoggerPath);
		   gDebug->setup();
	
		   gThread = new tthread::thread(Debug::run, (void *)gDebug);
	
	return;
	
}

spd::logger & Debug::getLogger()
{
	return *(m_pLogger.get());
}

spd::logger& Debug::logger()
{
	return gDebug->getLogger();
}

void Debug::DebugPrint( const char* format, ... )
{
	{
		std::string prefix;
		prefix = currentDateTime(time(NULL));
		prefix += " [V]: ";
		va_list vl;
		/* Output to telnet */
		va_start(vl, format);
		cli_print(gDebug->cli, prefix.c_str());
		cli_vabufprint(gDebug->cli, format, vl);
		va_end(vl);

		/* Output to console */
		va_list vl2;
		va_start(vl2, format);
		printf("%s", prefix.c_str());
		vprintf(format, vl2);
		va_end(vl2);
	}

	{
		/* Output to log */
		char str[MAX_LOG_LEN]="";
		int len;
		va_list args;
		va_start(args, format);
		len = vsnprintf(str, MAX_LOG_LEN-1, format, args);
		if (len >2)
		{
			str[len -1] = '\0';
		}
		logger().debug(str);
		va_end(args);
	}
}

void Debug::setup()
{
	cli = cli_init();
	cli_set_banner(cli, "V");
	cli_set_hostname(cli, "V");
	cli_telnet_protocol(cli, 1);

	cli_set_auth_callback(cli, & Debug::check_auth);
	cli_set_enable_callback(cli, & Debug::check_enable);
	
	cli_register_command(cli, NULL, "test", Debug::test, PRIVILEGE_PRIVILEGED, MODE_CONFIG_INT, "Veye test");
}

int Debug::check_auth(const char *username, const char *password)
{
	if (strcmp(username, "x") != 0)
		return CLI_ERROR;
	if (strcmp(password, "x") != 0)
		return CLI_ERROR;
	return CLI_OK;
}
int Debug::check_enable(const char *password)
{
	return !strcmp(password, "x");
}

int Debug::test(struct cli_def *cli, const char *command, char *argv[], int argc)
{
	int i;
	cli_print(cli, "called %s with \"%s\"", __FUNCTION__, command);
	cli_print(cli, "%d arguments:", argc);
	for (i = 0; i < argc; i++)
		cli_print(cli, "        %s", argv[i]);

	return CLI_OK;
}

