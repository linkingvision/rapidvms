/*
    This file is part of libmicrospdy
    Copyright Copyright (C) 2012 Andrey Uzunov

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
 * @file daemon_start_stop.c
 * @brief  starts and stops a SPDY daemon
 * @author Andrey Uzunov
 */

#include "platform.h"
#include "microspdy.h"
#include "common.h"

int
main()
{
	SPDY_init();
	
	struct SPDY_Daemon *daemon = SPDY_start_daemon(get_port(16123),
	 DATA_DIR "cert-and-key.pem",
	 DATA_DIR "cert-and-key.pem",
	NULL,NULL,NULL,NULL,NULL,SPDY_DAEMON_OPTION_END);
	
	if(NULL==daemon){
		printf("no daemon\n");
		return 1;
	}
	
	SPDY_stop_daemon(daemon);
	
	SPDY_deinit();
	
	return 0;
}
