cmake_minimum_required(VERSION 2.8)
project(ut)

set(SOURCES source/framework.cpp
            source/XMD5Test.cpp
            source/XStringTest.cpp
            source/LargeFileTests.cpp
            source/NewDeleteTest.cpp
            source/UuidUtilsTest.cpp
            source/XBitsyTest.cpp
            source/XBlockingQueueTest.cpp
            source/XBytePtrTest.cpp
            source/XCacheTest.cpp
            source/XCommandQueueTest.cpp
            source/XConditionTest.cpp
            source/XDateTupleTest.cpp
            source/XDomParserTest.cpp
            source/XDurationTest.cpp
            source/XDynamicLibraryTest.cpp
            source/XExceptionTest.cpp
            source/XFullVariantTest.cpp
            source/XFunctorFPTest.cpp
            source/XGuardTest.cpp
            source/XHashTest.cpp
            source/XInternalListTest.cpp
            source/XIRefTest.cpp
            source/XIWeakRefTest.cpp
            source/XJSONTest.cpp
            source/XListBoxTest.cpp
            source/DerivedObject.cpp
            source/XLoggerTest.cpp
            source/XMemoryMapTest.cpp
            source/XMemoryTest.cpp
            source/XMonoClockTest.cpp
            source/XMutexTest.cpp
            source/XNullableTest.cpp
            source/XObjCountTest.cpp
            source/XPathTest.cpp
            source/XRefTest.cpp
            source/XSocketAddressTest.cpp
            source/XSocketTest.cpp
            source/XStatisticsTest.cpp
            source/XStopWatchTest.cpp
            source/XStringSliceTest.cpp
            source/XStringTest.cpp
            source/XSystemStatsTest.cpp
            source/XTaskBaseTest.cpp
            source/XTimedCommandQueueTest.cpp
            source/XTimerTest.cpp
            source/XTimeTest.cpp
            source/XTimeZoneTest.cpp
            source/XUDPReceiverTest.cpp
            source/XUDPSenderTest.cpp
            source/XUuidTest.cpp
            source/XVariantTest.cpp
            source/XWeakRefTest.cpp)


set(COMMON_LIBS XSDK)
set(WINDOWS_LIBS XSDK DbgHelp ws2_32 iphlpapi)

include(common.cmake NO_POLICY_SCOPE)
include("${devel_artifacts_path}/build/base_ut.cmake" NO_POLICY_SCOPE)
