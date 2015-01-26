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
	u32 w;
	u32 h;	
}VeBox;

/* (1024/16) * (1024/16) =  4096 */
#define VE_MOT_BOX_MAX 4096
typedef struct __VeMotionBox
{
	VeBox mot[VE_MOT_BOX_MAX];
	u32 size;
	u32 w;
	u32 h;
}VeMotionBox;

#ifdef WIN32
#pragma   pack()
#endif

#endif /* _V_MINING_TYPE_H_ */
