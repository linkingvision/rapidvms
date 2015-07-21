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
 * @file common.c
 * @brief  Common functions used by the tests.
 * @author Andrey Uzunov
 */
 

#include "common.h"
#include <sys/time.h>

#ifdef __GNUC__
#define FUNC_CONSTRUCTOR(f) static void __attribute__ ((constructor)) f
#define FUNC_DESTRUCTOR(f) static void __attribute__ ((destructor)) f
#else  // !__GNUC__
#define FUNC_CONSTRUCTOR(f) _MHD_EXTERN void f
#define FUNC_DESTRUCTOR(f) _MHD_EXTERN void f
#endif  // __GNUC__

FUNC_CONSTRUCTOR (constructor)()
{
	printf("\nTEST START -------------------------------------------------------\n");
}

FUNC_DESTRUCTOR (destructor)()
{
	printf("------------------------------------------------------- TEST END\n");
}

uint16_t
get_port(uint16_t min)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	if(2 > min) min=2;
	uint16_t port =  min + (tv.tv_usec+10) % ((1 << 16)  - min);
	
	//port = 12345;
	printf("Port used: %i\n", port);
	
	return port;
}
