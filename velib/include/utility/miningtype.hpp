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
