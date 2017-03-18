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
#include "utility/utility.hpp"
#include "Poco/Net/Net.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Timespan.h"

using namespace tthread;
using namespace std;

namespace UtilityLib
{
#ifndef _WIN32
double GetTickCount(void) 
{
  struct timespec now;
  if (clock_gettime(CLOCK_MONOTONIC, &now))
    return 0;
  return now.tv_sec * 1000.0 + now.tv_nsec / 1000000.0;
}
#endif

void ve_sleep(int ms)
{
#ifdef WIN32
	Sleep(ms);
#else
	usleep(ms * 1000);
#endif
}
 unsigned UtilityGetBufferSize(int bufOptName,
                  int socket)
{
  unsigned curSize;
  socklen_t sizeSize = sizeof curSize;
  if (getsockopt(socket, SOL_SOCKET, bufOptName,
         (char*)&curSize, &sizeSize) < 0) {
    return 0;
  }

  return curSize;
}

 unsigned UtilityGetSendBufferSize(int socket)
{
  return UtilityGetBufferSize(SO_SNDBUF, socket);
}

 unsigned UtilityGetReceiveBufferSize(int socket)
{
  return UtilityGetBufferSize(SO_RCVBUF, socket);
}

 unsigned UtilitySetBufferTo(int bufOptName, int socket, unsigned requestedSize)
{
    int sizeSize = sizeof requestedSize;
    setsockopt(socket, SOL_SOCKET, bufOptName, (char*)&requestedSize, sizeSize);

  // Get and return the actual, resulting buffer size:
  return UtilityGetBufferSize(bufOptName, socket);
}

 unsigned UtilitySetSendBufferTo(int socket, unsigned requestedSize)
{
    return UtilitySetBufferTo(SO_SNDBUF, socket, requestedSize);
}

 unsigned UtilitySetReceiveBufferTo(int socket, unsigned requestedSize)
{
    return UtilitySetBufferTo(SO_RCVBUF, socket, requestedSize);
}

 BOOL makeSocketNonBlocking(int sock) {
#if defined(__WIN32__) || defined(WIN32) || defined(IMN_PIM)
  unsigned long arg = 1;
  return ioctlsocket(sock, FIONBIO, &arg) == 0;
#elif defined(VXWORKS)
  int arg = 1;
  return ioctl(sock, FIONBIO, (int)&arg) == 0;
#else
  int curFlags = fcntl(sock, F_GETFL, 0);
  return fcntl(sock, F_SETFL, curFlags|O_NONBLOCK) >= 0;
#endif
}

 std::string currentDateTime(time_t nTime) 
{
    time_t     now = nTime;
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);

    strftime(buf, sizeof(buf), "%Y/%m/%d %X", &tstruct);

    return buf;
}

 void DumpTxtData(unsigned char *pNuf, int nLen)
{
    int i = 0;
    for (i = 0; i < nLen; i ++)
    {
		printf("%c", pNuf[i]);
    }
	printf("\n\n");
}

 void DumpHexData(unsigned char *pNuf, int nLen)
{
    int i = 0;
    for (i = 0; i < nLen; i ++)
    {
		printf(" 0x%x", pNuf[i]);
    }
	printf("\n\n");
}

 int image_write_raw (unsigned char *pImg, int size, const char* fname)
{
    FILE * f = NULL;
    int ret;

    f = fopen (fname, "wb");
    if (f == NULL)
    {
        printf ("image_write_raw: fopen failed");
        return -1;
    }

    ret = fwrite(pImg, 1, size, f);
    if (ret != size)
    {
        printf ("image_write_raw: fwrite failed");
        return -1;
    }

    fclose (f);

    return 0;
}

 const std::string Time2String(time_t nTime) {
    time_t     now = nTime;
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://www.cplusplus.com/reference/clibrary/ctime/strftime/
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y/%m/%d %X", &tstruct);

    return buf;
}


#ifdef WIN32
#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
  #define DELTA_EPOCH_IN_MICROSECS  11644473600000000Ui64
#else
  #define DELTA_EPOCH_IN_MICROSECS  11644473600000000ULL
#endif
 int gettimeofday(struct timeval* tv, int* /*tz*/) 
{
    FILETIME ft;
    unsigned __int64 tmpres = 0;

    if( tv )
    {
        GetSystemTimeAsFileTime( &ft );

        tmpres |= ft.dwHighDateTime;
        tmpres <<= 32;
        tmpres |= ft.dwLowDateTime;

        tmpres /= 10;

        tmpres -= DELTA_EPOCH_IN_MICROSECS;

        tv->tv_sec = (time_t)(tmpres / 1000000UL);
        tv->tv_usec = (int)(tmpres % 1000000UL);

        return 0;
    }

    return -1;
}
#endif

bool TryCheckDevice(std::string ip, std::string port)
{
	if (ip.length() <= 0 || port.length() <= 0)
	{
		return false;
	}
	Poco::Net::SocketAddress sa(ip, port);
	Poco::Net::StreamSocket ss;
	Poco::Timespan tto(500 * 1000);
	try
	{
		ss.connect(sa, tto);
	}
	catch (Poco::Exception&)
	{
		return false;
	}	
	s8 *GetCap = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
"<soap:Envelope xmlns:soap=\"http://www.w3.org/2003/05/soap-envelope\" "
"xmlns:sch=\"http://www.onvif.org/ver10/schema\" "
"xmlns:wsu=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd\" "
"xmlns:wsse=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd\" "
"xmlns:wsdl=\"http://www.onvif.org/ver10/device/wsdl\">"
" <soap:Header>"
" </soap:Header>"
" <soap:Body>"
"  <wsdl:GetCapabilities>"
"   <wsdl:Category>Media</wsdl:Category>"
" </wsdl:GetCapabilities>"
" </soap:Body>"
"</soap:Envelope>";
	u8 Req[4096] = {0};
	sprintf((s8 *)Req,
	"POST /onvif/device_service HTTP/1.1\r\n"
	"Host: %s\r\n"
	"Content-Type: application/soap+xml; charset=utf-8\r\n"		
	"Connection: close\r\n"
	"Content-Length: %u\r\n"
	"\r\n"
	"%s", ip.c_str(), strlen((s8 *)GetCap), (s8 *)GetCap);

	if (ss.sendBytes((s8 *)Req, strlen((s8 *)Req), 0) <= 0)
	{
		return false;
	}

	return true;
}

}/* UtilityLib */

/* Get the value for a given parameter,  */
 int uriParserGetParaByName(const char * uri, const char * name,  
        char *out, const int out_len)
{
    char *p = NULL;
    int uri_len = -1;
    char * p_next = NULL;
    int ret = -1;
    
    //printf("%s---%d\n", __FILE__, __LINE__);

    if (uri == NULL
    || name == NULL
    || out == NULL){
        return 0;
    }

    uri_len = strlen(uri);

    /* Get the postion of the name */
    p = strstr((char *)uri, name);
    if (p == NULL){
        return 0;
    }

    p_next = strstr(p, "=");
    if (p_next == NULL){
        return 0;
    }

    p_next ++;
    if (p_next == NULL){
        return 0;
    }

    /* this para is the end of the uri */
    if (strstr(p_next, "=") == NULL){
        if ((int)(strlen(p_next)) >=  out_len){
            return -1;
        }

        strcpy(out, p_next);
        return strlen(p_next);
    }

    /* this para is not the end of the uri */
    p = strstr(p_next, "&");
    if (p == NULL){
        return 0;
    }
    //printf("p %c\n", *p);

    ret = p - p_next;
    //printf("ret %d\n", ret);
    if (ret >= out_len){
        return -1;
    }
    memcpy(out, p_next, ret);
    out[ret] = '\0';
    //printf("out %s %d\n", out, strlen(out));
    
    return ret;
}

