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
#ifndef _V_MINING_TYPE_H_
#define _V_MINING_TYPE_H_
#include "type.hpp"

#ifdef WIN32
#pragma   pack(1)
#endif
typedef struct __VeBox
{
	u32 x0; 
	u32 y0; 
	u32 w;/* box width */
	u32 h;/* box height */	
	u32 rw;/* image width */
	u32 rh;/* image height */
}VeBox;

typedef struct __VeObj
{
	s64 id;
	VeBox box;
}VeObj;

/* Max object in each frame*/
#define VE_OBJ_GROUP_MAX 64 
typedef struct __VeObjGroup
{
	VeObj obj[VE_OBJ_GROUP_MAX];
}VeObjGroup;

/* (1024/16) * (1024/16) =  4096, every 16 x 16 has a Box */
#define VE_MOT_BOX_MAX 4096
typedef struct __VeMotionBox
{
	u32 size;/* Total num of BOX */
	u32 rw; /* image width */
	u32 rh; /* image heigh */
	u32 survival;/* survival frames */ 
	VeBox mot[VE_MOT_BOX_MAX];
}VeMotionBox;

#ifdef WIN32
#pragma   pack()
#endif

#endif /* _V_MINING_TYPE_H_ */
