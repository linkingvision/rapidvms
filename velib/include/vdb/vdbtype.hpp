#ifndef __VDB_TYPE_HPP__
#define __VDB_TYPE_HPP__
#include "utility.hpp"

#ifndef WIN32
#define VDB_HANDLE s32
#define VDB_INVALID -1
#else
#define	VDB_HANDLE HANDLE
#define VDB_INVALID NULL
#endif /* WIN32 */

#ifndef WIN32   
    #ifndef _VDB_PACKED_1_   
    #define _VDB_PACKED_1_  __attribute__ ((packed))     
    #endif   
#else   
    #ifndef _VDB_PACKED_1_   
    #define _VDB_PACKED_1_   
    #endif   
#endif   
 

#ifdef WIN32
#ifndef	O_RDONLY
#define	O_RDONLY 0x0
#endif

#ifndef	O_WRONLY
#define	O_WRONLY 0x01
#endif

#ifndef	O_RDWR
#define	O_RDWR	 0x02
#endif

#ifndef O_CREAT
#define O_CREAT 0x04
#endif 

#ifndef O_EXCL
#define O_EXCL 0x08
#endif 

#ifndef O_TRUNC
#define O_TRUNC 0x10
#endif

#endif /* WIN32 */



#endif /* __VDB_TYPE_HPP__ */