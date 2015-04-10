cmake_minimum_required(VERSION 2.8)
project(ut)

set(SOURCES source/framework.cpp
            source/hyperhttp_client_server_test.cpp
            source/uri_test.cpp
            source/url_test.cpp)

set(COMMON_LIBS cppkit hyperhttp)
set(WINDOWS_LIBS DbgHelp ws2_32 iphlpapi)

include(../../devel_artifacts/build/base_ut.cmake)
