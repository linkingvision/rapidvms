
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef _X_SYSTEM_STATS_H_
#define _X_SYSTEM_STATS_H_

#ifdef IS_WINDOWS
#include <windows.h>
#else
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/sysinfo.h>
#include <sys/statvfs.h>
#include <unistd.h>
#endif

#include "XSDK/TimeUtils.h"
#include "XSDK/XSharedLib.h"

namespace XSDK
{

class XSystemStats
{
public:
    struct SystemMemory
    {
        uint64_t physicallyUsedBytes;
        uint64_t physicallyFreeBytes;
        uint64_t physicallyTotalBytes;
        uint64_t virtualUsedBytes;
        uint64_t virtualFreeBytes;
        uint64_t virtualTotalBytes;
    };

    /// CPU load range is 0-100 (percent)
    /// 0 denotes no load on the system
    /// 100 denotes a fully loaded system, regardless of the amount of logical processors
    struct CPULoad
    {
        uint8_t kernel;
        uint8_t user;
        uint8_t total;
        uint8_t free;
    };

    X_API XSystemStats();
    X_API virtual ~XSystemStats();

    /// Resets the load counts.
    X_API void Reset();

    /// Gets the load on the cpu for this process. The load is calculated
    /// as the average since the last time this method was called OR since
    /// Reset() was called.
    X_API void GetCurrentProcessCPULoad(CPULoad &);

    ///Returns the number of logical processors
    /// @note 1 physical core with hyperthreading enabled will be reported as two logical processors
    X_API static int GetNumberOfProcessors(void);

    ///Gets the load on the cpu for this process
    //X_API static void GetCurrentProcessCPULoad(CPULoad &);

    ///Gets system wide memory usage statistics
    X_API static void GetSystemMemory(SystemMemory &);

    /// Gets the disk usage at the specified path.
    /// @note This method returns the number of bytes available to the CURRENT user
    ///       on Windows and the bytes available to a UNPRIVILEGED user on Linux.
    /// @param path The filesystem path to query
    /// @param size Returns total size of the disk in bytes. Can be NULL.
    /// @param free Returns available free bytes on the disk. Can be NULL.
    /// @return 'true' if the operation succeeded.
    X_API static bool GetDiskUsage(const XString& path, uint64_t* size, uint64_t* free);

    /// Gets the current process name.
    /// @param pFullPath Optional pointer to retrieve the full path to the executable. This will include the executable name.
    /// @return The process name.
    X_API static XString GetProcessName(XString* pFullPath=0);

    /// Gets information about the system the current process is executing on.
    /// @note The value returned is in a system-specific format and be different between machines.
    /// @return A string describing the system information.
    X_API static XString GetSystemInformation();

private:

    timeval _sampleTime; ///< The time the usage samples were taken.

#ifdef IS_WINDOWS
    HANDLE _hProcess;           ///< Windows: The current process ID.
    ULARGE_INTEGER _userTime;   ///< Windows: Time in user space at start of interval.
    ULARGE_INTEGER _kernelTime; ///< Windows: Time in kernel space at start of interval.
#else
    rusage _usage;  ///< Posix: Usage counts at the start of interval.
#endif
};

}

#endif
