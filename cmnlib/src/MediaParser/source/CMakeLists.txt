cmake_minimum_required(VERSION 2.8)
project(MediaParser)

set(SOURCES source/H264Info.cpp
            source/H264MediaParser.cpp
            source/H264UserData.cpp
            source/MediaInfo.cpp
            source/MediaParser.cpp
	    source/MJPEGInfo.cpp
	    source/MJPEGMediaParser.cpp
	    source/MPEG4Info.cpp
	    source/MPEG4MediaParser.cpp
	    source/RBSPIter.cpp)

set(WINDOWS_LIBS XSDK)
set(LINUX_LIBS XSDK)

include(common.cmake NO_POLICY_SCOPE)
include("${devel_artifacts_path}/build/base_lib.cmake" NO_POLICY_SCOPE)
