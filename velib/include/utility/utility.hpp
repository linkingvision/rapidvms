#ifndef _V_UTILITY_H_
#define _V_UTILITY_H_

#define NOMINMAX
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tinythread.h"
#include "fast_mutex.h"

#include <iostream>
#include <sstream>
#include <string>
#include <list>
#include <map>
#ifdef WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>
#include <Ws2tcpip.h>
#include "type.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <assert.h>
#include <stdarg.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <sys/time.h>
#include <time.h>
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/file.h>
#include <ctype.h>
#include <errno.h>
#include <assert.h>
#include <unistd.h>
#include <math.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>

#define closesocket(x) close(x)
#endif
using namespace tthread;
using namespace std;

typedef std::string astring;

namespace UtilityLib
{



#ifdef WIN32
VE_LIBRARY_API int socketpair(int    const domain,
                        int    const type,
                        int    const protocol,
                        SOCKET       socks[2]);
#endif

VE_LIBRARY_API unsigned UtilityGetBufferSize(int bufOptName,
                  int socket);

VE_LIBRARY_API unsigned UtilityGetSendBufferSize(int socket);

VE_LIBRARY_API unsigned UtilityGetReceiveBufferSize(int socket);
VE_LIBRARY_API unsigned UtilitySetBufferTo(int bufOptName, int socket, 
					unsigned requestedSize);

VE_LIBRARY_API unsigned UtilitySetSendBufferTo(int socket, unsigned requestedSize);

VE_LIBRARY_API unsigned UtilitySetReceiveBufferTo(int socket, unsigned requestedSize);

VE_LIBRARY_API BOOL makeSocketNonBlocking(int sock);

VE_LIBRARY_API std::string currentDateTime(time_t nTime);

VE_LIBRARY_API void UtilityDumpTxt(unsigned char *pNuf, int nLen);

VE_LIBRARY_API void UtilityDumpHex(unsigned char *pNuf, int nLen);

VE_LIBRARY_API int image_write_raw (unsigned char *pImg, int size, const char* fname);

VE_LIBRARY_API const std::string Time2String(time_t nTime);

VE_LIBRARY_API bool TryCheckDevice(std::string ip, std::string port);
VE_LIBRARY_API void ve_sleep(int ms);
#ifdef WIN32
VE_LIBRARY_API int gettimeofday(struct timeval* tp, int* /*tz*/);
#endif

}/* UtilityLib */

/* Get the value for a given parameter,  */
VE_LIBRARY_API int uriParserGetParaByName(const char * uri, const char * name,  
        char *out, const int out_len);

#endif /* _VDC_UTILITY_H_ */
