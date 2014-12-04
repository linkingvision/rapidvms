
#ifndef cppkit_time_utils_h
#define cppkit_time_utils_h

#include "cppkit/os/ck_platform.h"
#include "cppkit/ck_exception.h"

#include <time.h>
#include <ctime>

#ifdef IS_LINUX
#include <sys/time.h>
#include <unistd.h>
#endif

#ifdef IS_WINDOWS
#include <winsock2.h>
#endif

namespace cppkit
{

#ifdef IS_WINDOWS
  #if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
    #define DELTA_EPOCH_IN_MICROSECS  11644473600000000Ui64
  #else
    #define DELTA_EPOCH_IN_MICROSECS  11644473600000000ULL
  #endif

  CK_API int ck_win32_gettimeofday( struct timeval* tv, void* obsolete );

  #define timeradd(a, b, result) \
    CK_MACRO_BEGIN \
      (result)->tv_sec = (a)->tv_sec + (b)->tv_sec; \
      (result)->tv_usec = (a)->tv_usec + (b)->tv_usec; \
      if ((result)->tv_usec >= 1000000) { \
        ++(result)->tv_sec; \
        (result)->tv_usec -= 1000000; \
      } \
    CK_MACRO_END

  #define timersub(a, b, result) \
    CK_MACRO_BEGIN \
      (result)->tv_sec = (a)->tv_sec - (b)->tv_sec; \
      (result)->tv_usec = (a)->tv_usec - (b)->tv_usec; \
      if ((result)->tv_usec < 0) { \
        --(result)->tv_sec; \
        (result)->tv_usec += 1000000; \
      } \
    CK_MACRO_END

  #define TIMEVAL_TO_TIMESPEC(tv, ts) { \
    (ts)->tv_sec = (tv)->tv_sec; \
    (ts)->tv_nsec = (tv)->tv_usec * 1000; \
}

#endif

/// Calculates the difference between 2 timevals in microseconds.
/// 'result' is a long. Can be negative, if b > a.
#define timerdiff(a, b, result) \
  CK_MACRO_BEGIN \
    (result) =  ((a)->tv_sec * 1000000) + (a)->tv_usec; \
    (result) -= ((b)->tv_sec * 1000000) + (b)->tv_usec; \
  CK_MACRO_END

#define timercopy(dst, src) \
  CK_MACRO_BEGIN \
    (dst)->tv_sec  = (src)->tv_sec; \
    (dst)->tv_usec = (src)->tv_usec; \
  CK_MACRO_END

/// Converts a logical clock from one time-scale to another.
/// @note This method exists because it is frequently convenient to work with
/// time as real values.
/// @param value The clock value to convert
/// @param freq_cur The frequency of 'value' (in ticks-per-second)
/// @param freq_new The frequency to convert 'value' to.
/// @return The converted clock value
inline double ck_convert_time_scale_real(uint64_t value, uint64_t freq_cur, uint64_t freq_new)
{
    if (freq_cur != freq_new)
        return (double)value * ((double)freq_new / (double)freq_cur);
    return (double)value;
}

/// Converts a logical clock from one time-scale to another.
/// @param value The clock value to convert
/// @param freq_cur The frequency of 'value' (in ticks-per-second)
/// @param freq_new The frequency to convert 'value' to.
/// @return The converted clock value
inline uint64_t ck_convert_time_scale(uint64_t value, uint64_t freq_cur, uint64_t freq_new)
{
    if (freq_cur != freq_new)
        return (uint64_t)(value * ((double)freq_new / (double)freq_cur));
    return value;
}

/// Calculates the number of seconds that have elapsed between 'start' and 'end'.
/// @param start The clock value at the beginning of the interval.
/// @param end The clock value at the end of the interval.
/// @param frequency The frequency of the clock.
/// @return The number of seconds that elapsed during the interval.
inline double ck_get_elapsed_time(uint64_t start, uint64_t end, uint64_t frequency)
{
    // NOTE: Since we are using unsigned math, the difference between 'start'
    // and 'end' will always be correct even when the clock wraps back to 0.
    // This is due to arithmetic underflow.
    return (double)(end - start) / (double)frequency;
}

#ifdef IS_WINDOWS
inline int ck_gettimeofday(struct timeval* tv) {return ck_win32_gettimeofday(tv, 0);}
#else
inline int ck_gettimeofday(struct timeval* tv) {return gettimeofday(tv, 0);}
#endif

//sleep and usleep
#ifdef IS_WINDOWS
inline void ck_sleep(unsigned int seconds) {Sleep((DWORD)(seconds * 1000));}
// Windows Sleep() doesn't support usec resolution. Sleep(0) can return immediately.
inline void ck_usleep(unsigned int usec)
{
    if (usec >= 1000)
        Sleep((DWORD)(usec / 1000));
    else if (usec == 0)
        Sleep(0); // Support an explicit sleep time of 0.
    else
        Sleep(1);
}
#else
inline void ck_sleep(unsigned int seconds) {sleep(seconds);}
inline void ck_usleep(useconds_t usec) {usleep(usec);}
#endif

#if 0
/// \brief Sleep for the given duration.
///
/// Calls x_usleep, so it has microsecond precision.
inline void ck_sleep(const XDuration& duration)
{
    X_ASSERT(!duration.IsNegative());
    x_usleep((unsigned int)duration.Total(USECS));
}
#endif

#ifdef IS_WINDOWS

CK_API char* ck_win32_strptime( const char *buf, const char *fmt, struct tm *tm );

inline char* ck_strptime( const char* time, const char* format, struct tm* tm )
{
    return ck_win32_strptime( time, format, tm );
}
#else
inline char* ck_strptime( const char* time, const char* format, struct tm* tm )
{
    return strptime( time, format, tm );
}
#endif

}

#endif
