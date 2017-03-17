
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "XSDK/Errors.h"
#include "XSDK/XSystemStats.h"
#include "XSDK/OS.h"

#ifndef IS_WINDOWS
    #ifndef _GNU_SOURCE
        #define _GNU_SOURCE // glib feature test macro
    #endif
    #include <errno.h>  // Required for program_invocation_name macro
#endif

using namespace XSDK;

XSystemStats::XSystemStats()
{
#ifdef IS_WINDOWS
    _hProcess = GetCurrentProcess();
#endif
    Reset();
}

XSystemStats::~XSystemStats()
{
}

void XSystemStats::Reset()
{
#ifdef IS_WINDOWS
    FILETIME creationTime, exitTime, kernelTime, userTime;
    if( GetProcessTimes(_hProcess, &creationTime, &exitTime, &kernelTime, &userTime) == 0 )
        X_THROW(("XSystemStats", "Failed to retrieve process times\n"));
    _kernelTime.LowPart  = kernelTime.dwLowDateTime;
    _kernelTime.HighPart = kernelTime.dwHighDateTime;
    _userTime.LowPart    = userTime.dwLowDateTime;
    _userTime.HighPart   = userTime.dwHighDateTime;
#else
    getrusage(RUSAGE_SELF, &_usage);
#endif

    x_gettimeofday(&_sampleTime);
}

void XSystemStats::GetCurrentProcessCPULoad(CPULoad & cpuLoad)
{
    timeval now;
    XSDK::x_gettimeofday(&now);

#ifdef IS_WINDOWS

    FILETIME creationTime, exitTime, kernelTime, userTime;
    if( GetProcessTimes(_hProcess, &creationTime, &exitTime, &kernelTime, &userTime) == 0 )
        X_THROW(("XSystemStats", "Failed to retrieve process times\n"));

    ULARGE_INTEGER curKernelTime, curUserTime;
    curKernelTime.LowPart  = kernelTime.dwLowDateTime;
    curKernelTime.HighPart = kernelTime.dwHighDateTime;
    curUserTime.LowPart    = userTime.dwLowDateTime;
    curUserTime.HighPart   = userTime.dwHighDateTime;

    float kernelProcessTime = (curKernelTime.QuadPart - _kernelTime.QuadPart) / 100000.0f;
    float userProcessTime = (curUserTime.QuadPart - _userTime.QuadPart) / 100000.0f;

    _kernelTime = curKernelTime;
    _userTime = curUserTime;

#else

    rusage curUsage;
    getrusage(RUSAGE_SELF, &curUsage);

    float userProcessTime = ((float)(curUsage.ru_utime.tv_sec - _usage.ru_utime.tv_sec) +
        ((float)(curUsage.ru_utime.tv_usec - _usage.ru_utime.tv_usec) / 1000000.0f)) * 100.0f;
    float kernelProcessTime = ((float)(curUsage.ru_stime.tv_sec - _usage.ru_stime.tv_sec) +
        ((float)(curUsage.ru_stime.tv_usec - _usage.ru_stime.tv_usec) / 1000000.0f)) * 100.0f;

    _usage = curUsage;

#endif

    float timeScale = GetNumberOfProcessors() * ((float)(now.tv_sec - _sampleTime.tv_sec) + ((float)(now.tv_usec - _sampleTime.tv_usec) / 1000000.0f));
    _sampleTime = now;

    cpuLoad.kernel = 0;
    cpuLoad.user   = 0;
    if (timeScale > 0.0) {
        cpuLoad.kernel = (uint8_t)(kernelProcessTime / timeScale);
        cpuLoad.user   = (uint8_t)(userProcessTime   / timeScale);
    }
    // Handle the case where we are queried faster than the resolution of the
    // system's ability to provide statistics (seen on linux).
    if (cpuLoad.kernel > 100 || cpuLoad.user > 100) {
        cpuLoad.kernel = 0;
        cpuLoad.user = 0;
    }
    cpuLoad.total  = cpuLoad.kernel + cpuLoad.user;
    cpuLoad.free   = 100 - cpuLoad.total;
}

int XSystemStats::GetNumberOfProcessors(void)
{
#ifdef WIN32
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    return sysinfo.dwNumberOfProcessors;
#else
    return sysconf(_SC_NPROCESSORS_ONLN);
#endif
}

void XSystemStats::GetSystemMemory(SystemMemory & systemMemory)
{
#ifdef WIN32

    MEMORYSTATUSEX memStatus;
    memStatus.dwLength = sizeof (memStatus);
    GlobalMemoryStatusEx(&memStatus);

    systemMemory.physicallyFreeBytes = memStatus.ullAvailPhys;
    systemMemory.physicallyUsedBytes = memStatus.ullTotalPhys - memStatus.ullAvailPhys;
    systemMemory.physicallyTotalBytes = memStatus.ullTotalPhys;
    systemMemory.virtualFreeBytes = memStatus.ullAvailVirtual;
    systemMemory.virtualUsedBytes = memStatus.ullTotalVirtual - memStatus.ullAvailVirtual;
    systemMemory.virtualTotalBytes = memStatus.ullTotalVirtual;

#else
#ifndef __APPLE__
    struct sysinfo info;
    sysinfo(&info);

    systemMemory.physicallyFreeBytes = info.freeram * info.mem_unit;
    systemMemory.physicallyUsedBytes = (info.totalram - info.freeram) * info.mem_unit;
    systemMemory.physicallyTotalBytes = info.totalram * info.mem_unit;
    systemMemory.virtualFreeBytes = info.freeswap * info.mem_unit;
    systemMemory.virtualUsedBytes = (info.totalswap - info.freeswap) * info.mem_unit;
    systemMemory.virtualTotalBytes = info.totalswap * info.mem_unit;
#endif

#endif
}

bool XSystemStats::GetDiskUsage(const XString& path, uint64_t* size, uint64_t* free)
{
#ifdef IS_WINDOWS
    ULARGE_INTEGER winFree, winTotal;
    if (!GetDiskFreeSpaceExW(path.get_wide_string().data(), &winFree, &winTotal, 0))
        return false;
    if (size)
        *size = winTotal.QuadPart;
    if (free)
        *free = winFree.QuadPart;
#else
    struct statvfs stat;
    if (statvfs(path.c_str(), &stat) != 0)
        return false;
    if (size)
        *size = (uint64_t)stat.f_blocks * (uint64_t)stat.f_frsize;
    if (free)
        *free = (uint64_t)stat.f_bavail * (uint64_t)stat.f_bsize;
#endif
    return true;
}

XString XSystemStats::GetProcessName(XString* pFullPath)
{
#ifdef IS_WINDOWS
    wchar_t fullpath[MAX_PATH+1];
    GetModuleFileNameW(NULL, fullpath, MAX_PATH);
    wchar_t filename[_MAX_FNAME];
    wchar_t ext[_MAX_EXT];
    _wsplitpath_s(fullpath, 0, 0, 0, 0, filename, _MAX_FNAME, ext, _MAX_EXT);
    if (pFullPath)
        *pFullPath = fullpath;
    return XString(filename) + XString(ext);
#else
#ifndef __APPLE__
    if (pFullPath)
        *pFullPath = program_invocation_name;
    return program_invocation_short_name;
#else
    return "not support";
#endif
#endif
}

XString XSystemStats::GetSystemInformation()
{
#ifdef IS_WINDOWS
    //"Windows <version> <cpu arch> <num processors> <computer name>"
    OSVERSIONINFOEX osInfo;
    osInfo.dwOSVersionInfoSize = sizeof(osInfo);
    if (!GetVersionEx((LPOSVERSIONINFO)&osInfo))
        X_THROW(("XSystemStats: Failed to get OS version information. %s", GetLastErrorMsg().c_str()));

    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);

    XString info = "Windows ";
    if (osInfo.dwMajorVersion == 5) {
        if (osInfo.dwMinorVersion == 1)
            info += "XP";
        else if (osInfo.dwMinorVersion == 2) {
            if (osInfo.wProductType == VER_NT_WORKSTATION && sysInfo.dwProcessorType == PROCESSOR_ARCHITECTURE_AMD64)
                info += "XP Pro x64";
            else
                info += "Server 2003";
        }
        else
            info += XString::Format("%u.%u (unknown)", osInfo.dwMajorVersion, osInfo.dwMinorVersion);
    }
    else if (osInfo.dwMajorVersion == 6) {
        if (osInfo.dwMinorVersion == 0) {
            if (osInfo.wProductType == VER_NT_WORKSTATION)
                info += "Vista";
            else
                info += "Server 2008";
        }
        else if (osInfo.dwMinorVersion == 1) {
            if (osInfo.wProductType == VER_NT_WORKSTATION)
                info += "7";
            else
                info += "Server 2008 R2";
        }
        else if (osInfo.dwMinorVersion == 2) {
            if (osInfo.wProductType == VER_NT_WORKSTATION)
                info += "8";
            else
                info += "Server 2012";
        }
        else
            info += XString::Format("%u.%u (unknown)", osInfo.dwMajorVersion, osInfo.dwMinorVersion);
    }
    else
        info += XString::Format("%u.%u (unknown)", osInfo.dwMajorVersion, osInfo.dwMinorVersion);

    if (osInfo.wServicePackMajor > 0)
        info += XString::Format(" Service Pack %u.%u", osInfo.wServicePackMajor, osInfo.wServicePackMinor);

    // CPU arch and count
    switch (sysInfo.wProcessorArchitecture)
    {
    case PROCESSOR_ARCHITECTURE_INTEL:
        info += ", x86";
        break;
    case PROCESSOR_ARCHITECTURE_AMD64:
        info += ", x64";
        break;
    case PROCESSOR_ARCHITECTURE_IA64:
        info += ", Itanium";
        break;
    default:
        info += ", [unknown arch]";
    };
    info += XString::Format(" (%u cores)", sysInfo.dwNumberOfProcessors);

    // Computer name
    DWORD len = MAX_COMPUTERNAME_LENGTH;
    TCHAR name[MAX_COMPUTERNAME_LENGTH+1];
    if (GetComputerName(name, &len))
        info += XString::Format(", \"%s\"", name);

    return info;
#else
    FILE* f = fopen("/proc/version", "r");
    if (!f)
        X_THROW(("XSystemStats: Unable to open /proc/version for reading. %s", GetLastErrorMsg().c_str()));
    size_t len = 255;
    char version[len+1];
    size_t read = fread(version, 1, len, f);
    bool eof = feof(f);
    fclose(f);
    if (read != len && !eof)
        X_THROW(("XSystemStats: Failed to read data from /proc/version. %s", GetLastErrorMsg().c_str()));
    version[read] = '\0';
    return version;
#endif
}
