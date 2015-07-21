/*
    This file is part of libmicrospdy
    Copyright Copyright (C) 2013 Andrey Uzunov

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file test_proxies.c
 * @brief  test curl > mhd2spdylay > microspdy2http > mhd
 * @author Andrey Uzunov
 */

#include "platform.h"
#include "microspdy.h"
#include "common.h"
#include <sys/wait.h>
#include <stdio.h>   /* printf, stderr, fprintf */
#include <sys/types.h> /* pid_t */
#include <unistd.h>  /* _exit, fork */
#include <stdlib.h>  /* exit */
#include <errno.h>   /* errno */
#include <sys/wait.h> /* pid_t */
#include "common.h"

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif /* !WIN32_LEAN_AND_MEAN */
#include <windows.h>
#endif

#define EXPECTED_BODY "<html><head><title>libmicrohttpd demo</title></head><body>libmicrohttpd demo</body></html>"


pid_t parent;
	pid_t child_mhd;
	pid_t child_spdy2http;
	pid_t child_mhd2spdy;
	pid_t child_curl;

	uint16_t mhd_port;
	uint16_t spdy2http_port;
	uint16_t mhd2spdy_port;

void
killproc(int pid, const char *message)
{
	printf("%s\nkilling %i\n",message,pid);
	kill(pid, SIGKILL);
}


void killchildren()
{
    if(0 != child_mhd)
      killproc(child_mhd,"kill mhd\n");
    if(0 != child_spdy2http)
      killproc(child_spdy2http,"kill spdy2http\n");
    if(0 != child_mhd2spdy)
      killproc(child_mhd2spdy,"kill mhd2spdy\n");
    if(0 != child_curl)
      killproc(child_curl,"kill curl\n");
}

pid_t au_fork()
{
  pid_t child = fork();
	if (child == -1)
	{
    killchildren();

		killproc(parent,"fork failed\n");
	}

	return child;
}


int main()
{
  //pid_t child;
	int childstatus;
	pid_t wpid;

	parent = getpid();
	mhd_port = get_port(4000);
	spdy2http_port = get_port(4100);
	mhd2spdy_port = get_port(4200);

	child_mhd = au_fork();
	if (child_mhd == 0)
	{
    //run MHD
		pid_t devnull;
    char *port_s;

		close(1);
		devnull = open("/dev/null", O_WRONLY);
                if (-1 == devnull)
                  abort();
		if (1 != devnull)
		{
			dup2(devnull, 1);
			close(devnull);
		}
    asprintf(&port_s, "%i", mhd_port);
		execlp ("../examples/minimal_example", "minimal_example", port_s, NULL);
		fprintf(stderr, "executing mhd failed\nFor this test 'make' must be run before 'make check'!\n");
    //killchildren();
    _exit(1);
	}


	child_spdy2http = au_fork();
	if (child_spdy2http == 0)
	{
    //run spdy2http
		pid_t devnull;
    char *port_s;
    //char *url;

		close(1);
		devnull = open("/dev/null", O_WRONLY);
                if (-1 == devnull)
                  abort();
		if (1 != devnull)
		{
			dup2(devnull, 1);
			close(devnull);
		}
    //asprintf(&url, "127.0.0.1:%i", mhd_port);
    asprintf(&port_s, "%i", spdy2http_port);
    sleep(1);
		execlp ("../spdy2http/microspdy2http", "microspdy2http", "-v4rtT", "10", "-p", port_s, NULL);
		fprintf(stderr, "executing microspdy2http failed\n");
    //killchildren();
    _exit(1);
	}

	child_mhd2spdy = au_fork();
	if (child_mhd2spdy == 0)
	{
    //run MHD2sdpy
		pid_t devnull;
    char *port_s;
    char *url;

		close(1);
		devnull = open("/dev/null", O_WRONLY);
                if (-1 == devnull)
                  abort();
		if (1 != devnull)
		{
			dup2(devnull, 1);
			close(devnull);
		}
    asprintf(&url, "http://127.0.0.1:%i", spdy2http_port);
    asprintf(&port_s, "%i", mhd2spdy_port);
    sleep(2);
		execlp ("../examples/mhd2spdy", "mhd2spdy", "-vosb", url, "-p", port_s, NULL);
		fprintf(stderr, "executing mhd2spdy failed\n");
    //killchildren();
    _exit(1);
	}

	child_curl = au_fork();
	if (child_curl == 0)
	{
    //run curl
    FILE *p;
		pid_t devnull;
		char *cmd;
    unsigned int i;
    int retc;
    char buf[strlen(EXPECTED_BODY) + 1];

		close(1);
		devnull = open("/dev/null", O_WRONLY);
                if (-1 == devnull)
                  abort ();
		if (1 != devnull)
		{
			dup2(devnull, 1);
			close(devnull);
		}

		asprintf (&cmd, "curl --proxy http://127.0.0.1:%i http://127.0.0.1:%i/", mhd2spdy_port, mhd_port);
    sleep(3);
    p = popen(cmd, "r");
    if (p != NULL)
    {
      for (i = 0; i < strlen(EXPECTED_BODY) && !feof(p); i++)
      {
          retc = fgetc (p);
          if (EOF == retc)
            abort (); /* what did feof(p) do there!? */
          buf[i] = (char) retc;
      }

      pclose(p);
      buf[i] = 0;
      _exit(strcmp(EXPECTED_BODY, buf));
    }
		fprintf(stderr, "executing curl failed\n");
    //killchildren();
    _exit(1);
	}

  do
  {
    wpid = waitpid(child_mhd,&childstatus,WNOHANG);
    if(wpid == child_mhd)
    {
      fprintf(stderr, "mhd died unexpectedly\n");
      killchildren();
      return 1;
    }

    wpid = waitpid(child_spdy2http,&childstatus,WNOHANG);
    if(wpid == child_spdy2http)
    {
      fprintf(stderr, "spdy2http died unexpectedly\n");
      killchildren();
      return 1;
    }

    wpid = waitpid(child_mhd2spdy,&childstatus,WNOHANG);
    if(wpid == child_mhd2spdy)
    {
      fprintf(stderr, "mhd2spdy died unexpectedly\n");
      killchildren();
      return 1;
    }

    if(waitpid(child_curl,&childstatus,WNOHANG) == child_curl)
    {
      killchildren();
      return WEXITSTATUS(childstatus);
    }
    sleep(1);
  }
	while(true);
}
