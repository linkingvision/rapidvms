cmake_minimum_required(VERSION 2.8)
project(hyperhttp)

set(SOURCES source/hyperhttp_exception.cpp
            source/hyperhttp_utils.cpp
            source/uri.cpp
            source/url.cpp
            source/client_request.cpp
            source/client_response.cpp
            source/server_request.cpp
            source/server_response.cpp
            source/methods.cpp)

set(COMMON_LIBS cppkit)

include(common.cmake NO_POLICY_SCOPE)
include(../devel_artifacts/build/base_lib.cmake NO_POLICY_SCOPE)
