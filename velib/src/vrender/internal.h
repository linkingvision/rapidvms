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
#ifndef __INTERNAL_H__
#define __INTERNAL_H__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif


#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>

#include <set>
#include <map>
#include <list>
#include <algorithm>

extern "C"
{
#include "libswscale/swscale.h"
#include "libavcodec/avcodec.h"
}

#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>

#include <ddraw.h>
#include <d3d9.h>

#include <MMSystem.h>
#include <dsound.h>

#endif // __INTERNAL_H__
