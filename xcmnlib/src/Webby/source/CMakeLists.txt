cmake_minimum_required(VERSION 2.8)
project(Webby)

set(SOURCES source/ClientSideRequest.cpp
            source/ClientSideResponse.cpp
            source/RequestBody.cpp
            source/ServerSideRequest.cpp
            source/ServerSideResponse.cpp
            source/URI.cpp
            source/URL.cpp
            source/WebbyException.cpp
            source/WebbyUtils.cpp
            source/SoapArgs.cpp)

set(WINDOWS_LIBS XSDK)
set(LINUX_LIBS XSDK)

include(common.cmake NO_POLICY_SCOPE)
include("${devel_artifacts_path}/build/base_lib.cmake" NO_POLICY_SCOPE)
