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
 * @file common.h
 * @brief  Common functions used by the tests.
 * @author Andrey Uzunov
 */
 
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>

#define FAIL_TEST(msg) do{\
	printf("%i:%s\n", __LINE__, msg);\
	fflush(stdout);\
	exit(-10);\
	}\
	while(0)
	
uint16_t
get_port(uint16_t min);
