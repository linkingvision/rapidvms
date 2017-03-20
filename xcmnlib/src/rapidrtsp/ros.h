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
#ifndef __GNU_WIN_H__
#define __GNU_WIN_H__

#if defined(_WIN32) || defined(_WIN64)
/*******************************************************************************************************
* interface define in windows 
*******************************************************************************************************/
#include <winsock2.h>
#include <process.h>
#include <windows.h>
// lock: critical_section
typedef CRITICAL_SECTION 	Lock_t;
#define LOCK_init(lock) 	InitializeCriticalSection(lock)
#define LOCK_destroy(lock)	DeleteCriticalSection(lock)
#define LOCK_lock(lock) 	EnterCriticalSection(lock)
#define LOCK_unlock(lock) 	LeaveCriticalSection(lock)
// event
typedef void* EventHandle_t;
#define EVT_init(h,value)		h=CreateEvent(NULL,FALSE,value,NULL)
#define EVT_post(h)				SetEvent(h)
#define EVT_wait(h,timeout,ret)	ret=WaitForSingleObject(h,timeout)
#define EVT_destroy(h)			CloseHandle(h)
//thread
typedef unsigned long ThreadId_t;
#define THREAD_create(tid,proc,arg)	\
	do{\
		tid=_beginthread(proc,0,arg);\
		if(tid == (ThreadId_t)(-1)){\
			printf("create thread failed\n");\
			return -1;\
		}\
	}while(0)
#define THREAD_exit()		_endthread()
// last errno
#define GetLastError()	GetLastError()
// millisecond	
typedef DWORD MillisecondTimer_t;
typedef DWORD MilliSecond_t;
////mtimer and mtimer_tmp is type of MillisecondTimer_t,lasttime is type of MilliSecond_t which is duration time between start and stop
#define MilliTimerStart(mtimer)	mtimer=GetTickCount()	
#define MilliTimerAdd(mtimer,val) mtimer+=(val)
#define MilliTimerMinus(mtimer,val) mtimer-=(val)
#define MilliTimerSet(mtimer,sec,usec) mtimer=(sec)*1000+((usec)/1000)
#define MilliTimerGet(mtimer,sec,usec) \
	do{\
		sec = mtimer/1000;\
		usec = (mtimer%1000)*1000;\
	}while(0)
#define MilliTimerIsClear(mtimer) ((mtimer == 0) ? TRUE : FALSE)
#define MilliTimerStop(mtimer,mtimer_tmp,lasttime) \
	do{\
		mtimer_tmp = GetTickCount();\
		lasttime=mtimer_tmp-mtimer;\
	}while(0)
// strings
//#define STRCASESTR	strstr
// sleep, 
#define MSLEEP(n)	Sleep((n))
#define SLEEP(n)	Sleep((n)*1000)
//
#else
/*******************************************************************************************************
* interface define in gnu
*******************************************************************************************************/
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <semaphore.h>
#include <sys/time.h>
#include <time.h>
// thread
typedef pthread_t ThreadId_t;
#define THREAD_create(tid,proc,arg)	\
	do{\
		if(pthread_create(&tid,NULL,proc,arg)!=0){\
			printf("create thread failed\n");\
			return -1;\
		}\
	}while(0)
#define THREAD_exit()		pthread_exit(NULL)
// last errno
#define GetLastError()	errno
// millisecond	
typedef struct timeval MillisecondTimer_t;
typedef unsigned int MilliSecond_t;
////mtimer and mtimer_tmp is type of MillisecondTimer_t,lasttime is type of MilliSecond_t which is duration time between start and stop
#define MilliTimerStart(mtimer)	gettimeofday(&mtimer,NULL)
#define MilliTimerSet(mtimer,sec,usec) \
	do{\
		mtimer.tv_sec = (sec);\
		mtimer.tv_usec = (usec);\
	}while(0)
#define MilliTimerGet(mtimer,sec,usec) \
		do{\
			sec = mtimer.tv_sec;\
			usec = mtimer.tv_usec;\
		}while(0)
#define MilliTimerIsClear(mtimer) ((mtimer.tv_sec == 0 && mtimer.tv_usec == 0) ? TRUE : FALSE)
#define MilliTimerAdd(mtimer,val) \
	do{\
		mtimer.tv_sec+=(val)/1000;\
		mtimer.tv_usec+=((val)%1000)*1000;\
		if(mtimer.tv_usec >= 1000000){\
			mtimer.tv_usec -= 1000000;\
			mtimer.tv_sec++;\
		}\
	}while(0)
#define MilliTimerMinus(mtimer,val) \
	do{\
		mtimer.tv_sec-=(val)/1000;\
		if(mtimer.tv_usec < (((val)%1000)*1000)){\
			mtimer.tv_usec = mtimer.tv_usec + 1000000 - ((val)%1000)*1000;\
			mtimer.tv_sec--;\
		}else{\
			mtimer.tv_usec -= ((val)%1000)*1000;\
		}\
	}while(0)
#define MilliTimerStop(mtimer,mtimer_tmp,lasttime) \
	do{\
		gettimeofday(&mtimer_tmp,NULL);\
		lasttime=(mtimer_tmp.tv_sec-mtimer.tv_sec)*1000+(mtimer_tmp.tv_usec - mtimer.tv_usec)/1000;\
	}while(0)
// strings
//#define STRCASESTR	strcasestr
// sleep
#define MSLEEP(n)	usleep((n)*1000)
#define SLEEP(n)	sleep((n))
// lock: pthread mutex
#if 1
typedef pthread_mutex_t 	Lock_t;
#define LOCK_init(lock)		pthread_mutex_init(lock,NULL)
#define LOCK_destroy(lock)	pthread_mutex_destroy(lock)
#define LOCK_lock(lock) 	pthread_mutex_lock(lock)
#define LOCK_unlock(lock) 	pthread_mutex_unlock(lock)
#else
typedef pthread_spinlock_t 	Lock_t;
#define LOCK_init(lock) 	pthread_spin_init(lock,0)
#define LOCK_destroy(lock)	pthread_spin_destroy(lock)
#define LOCK_lock(lock) 	pthread_spin_lock(lock)
#define LOCK_unlock(lock)	pthread_spin_unlock(lock)
#endif
// event : semaphore
typedef sem_t EventHandle_t;
#define EVT_init(h,value) 		sem_init(&h,0,value)
#define EVT_post(h)				sem_post(&h)
#ifndef __APPLE__
#define EVT_wait(h,timeout_us,ret)	\
	do{\
	time_t cur_t;\
	time(&cur_t);\
	struct timespec abs_to;\
	abs_to.tv_sec = timeout_us/1000000+cur_t;\
	abs_to.tv_nsec= (timeout_us%1000000)*1000;\
	ret=sem_timedwait(&h,&abs_to);\
	}while(0)
#else
#define EVT_wait(h,timeout_us,ret)	\
	do{\
	ret=sem_trywait(&h);\
	}while(0)
#endif
	
#define EVT_destroy(h)			sem_destroy(&h)
#endif

#include "utility/videotype.hpp"

#endif
