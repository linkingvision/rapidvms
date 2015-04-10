
#include "XSystemStatsTest.h"
#include "XSDK/TimeUtils.h"
#include "XSDK/XMemory.h"
#include "XSDK/XRef.h"
#include <ctime>
#include <cmath>
#include <climits>
#include <cstdio>
#include "XSDK/OS/Sprintf.h"
#include "XSDK/LargeFiles.h"

using namespace XSDK;

REGISTER_TEST_FIXTURE(XSystemStatsTest);

static const int cpuWindow = 20;
static const int cpuSamples = 15;
static const int cpuTestTime = 100000;

static const unsigned int systemMemoryTestSize = 1024*1024*4;

void busywait(long usecs)
{
    timeval start, now;
    x_gettimeofday(&start);
    while(true)
    {
        x_gettimeofday(&now);
        long diff;
        timerdiff(&now, &start, diff);
        if (diff >= usecs)
            return;
        int val = rand();
        if (val%10 < 3)
            x_usleep(1);
        else
            atan((double)rand());
    }
}


void XSystemStatsTest::setup()
{
    UT_ASSERT_NO_THROW(_numberOfProcessors = XSystemStats::GetNumberOfProcessors());
}

void XSystemStatsTest::teardown()
{
}

void XSystemStatsTest::TestGetNumberOfProcessors(void)
{

    UT_ASSERT(_numberOfProcessors > 0);
}

void XSystemStatsTest::TestGetCurrentProcessCPULoad(void)
{

    XRef<XSystemStats> stats;
    UT_ASSERT_NO_THROW( stats = new XSystemStats() );

    XSystemStats::CPULoad load;
    for (int ii=0; ii<5; ++ii)
    {
        busywait(1000000);
        UT_ASSERT_NO_THROW( stats->GetCurrentProcessCPULoad(load) );
        UT_ASSERT( load.free == 100 - load.total );
        UT_ASSERT( load.total == load.user + load.kernel );
    }

    UT_ASSERT_NO_THROW( stats->Reset() );
    busywait(1000000);
    UT_ASSERT_NO_THROW( stats->GetCurrentProcessCPULoad(load) );
    UT_ASSERT( load.free == 100 - load.total );
    UT_ASSERT( load.total == load.user + load.kernel );

    // Test high-speed calling
    for (int ii=0; ii<10000; ++ii)
    {
        UT_ASSERT_NO_THROW( stats->GetCurrentProcessCPULoad(load) );
        UT_ASSERT( load.free == 100 - load.total );
        UT_ASSERT( load.total == load.user + load.kernel );
    }
}

void XSystemStatsTest::TestGetSystemMemory(void)
{

    XSystemStats::SystemMemory sysMemControl;
    XSystemStats::SystemMemory sysMemoryTest;

    UT_ASSERT_NO_THROW(XSystemStats::GetSystemMemory(sysMemControl));

    XSDK::XMemory buf(systemMemoryTestSize);
    memset(&buf.Extend(0),INT_MAX,systemMemoryTestSize);

    UT_ASSERT_NO_THROW(XSystemStats::GetSystemMemory(sysMemoryTest));

    UT_ASSERT(sysMemoryTest.physicallyFreeBytes < sysMemControl.physicallyFreeBytes);
    UT_ASSERT(sysMemoryTest.physicallyUsedBytes > sysMemControl.physicallyUsedBytes);
    UT_ASSERT(sysMemoryTest.physicallyTotalBytes == sysMemControl.physicallyTotalBytes);
    UT_ASSERT(sysMemoryTest.virtualTotalBytes == sysMemControl.virtualTotalBytes);
}

void XSystemStatsTest::TestGetDiskUsage(void)
{

    // Get the current working directory
    XString cwd;
#ifdef IS_WINDOWS
    const int blen = MAX_PATH;
    TCHAR temp[blen+1];
    int bytes = GetModuleFileName(0, temp, blen);
    UT_ASSERT(bytes > 0); // Error getting working directory
    cwd = temp;
    size_t slash = cwd.rfind('\\'); // Remove application name
    cwd = cwd.substr(0, slash);
#else
    const int blen = 256;
    char temp[blen+1];
    char link[64];
    snprintf(link, 64, "/proc/%d/exe", getpid());
    int bytes = readlink(link, temp, blen);
    UT_ASSERT(bytes > 0); // Error getting working directory
    temp[bytes] = '\0';
    cwd = temp;
    size_t slash = cwd.rfind('/'); // Remove application name
    cwd = cwd.substr(0, slash);
#endif

    //Using cwd does weird things with chrooted environments which involve symlinks.
    XString tempDir = TempDir();

    uint64_t size1, free1, size2, free2;
    UT_ASSERT( XSystemStats::GetDiskUsage(tempDir, 0, 0) );
    UT_ASSERT( XSystemStats::GetDiskUsage(tempDir, &size1, 0) );
    UT_ASSERT( XSystemStats::GetDiskUsage(tempDir, 0, &free1) );
    UT_ASSERT( XSystemStats::GetDiskUsage(tempDir, &size2, &free2) );
    UT_ASSERT( size1 == size2 );

    // Write a large file and make sure the free space decreases by at least half the file size
    const size_t filesize = 10 * 1024 * 1024;
    XString fname = tempDir;
#ifdef IS_WINDOWS
    fname += "\\";
#else
    fname += "/";
#endif
    fname += "remove_me_test_file.txt";
    remove(fname.c_str());  // Remove file if an old one exists.
    UT_ASSERT( XSystemStats::GetDiskUsage(tempDir, &size1, &free1) );
    FILE *f = fopen(fname.c_str(), "wt");
    UT_ASSERT( f != NULL );

    char data[] = { 'd','e','a','d','b','e','e','f' };
    for (size_t ii=0; ii<filesize; )
    {
        size_t wr = fwrite(data, 1, sizeof(data), f);
        ii += wr;
    }
    fclose(f);

    UT_ASSERT( XSystemStats::GetDiskUsage(tempDir, &size2, &free2) );
    UT_ASSERT( size1 == size2 );
    UT_ASSERT( free2 < free1 - (filesize / 2) );

    // Delete the file
    UT_ASSERT( remove(fname.c_str()) == 0 );
}

void XSystemStatsTest::TestGetProcessName(void)
{

    XString fullpath;
    UT_ASSERT_NO_THROW( XSystemStats::GetProcessName() );
    UT_ASSERT_NO_THROW( XSystemStats::GetProcessName(&fullpath) );
    UT_ASSERT( XSystemStats::GetProcessName().length() > 0 );
    UT_ASSERT( fullpath.EndsWith(XSystemStats::GetProcessName()) );
}

void XSystemStatsTest::TestGetSystemInformation(void)
{

    XString fullpath;
    UT_ASSERT_NO_THROW( XSystemStats::GetSystemInformation() );
    UT_ASSERT( XSystemStats::GetSystemInformation().length() > 0 );
}
