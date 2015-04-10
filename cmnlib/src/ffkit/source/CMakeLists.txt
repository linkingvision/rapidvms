cmake_minimum_required(VERSION 2.8)
project(ffkit)

set(SOURCES source/argb24_to_yuv420p.cpp
            source/yuv420p_to_argb24.cpp
            source/h264mp4_to_annexb.cpp
            source/av_demuxer.cpp
            source/av_muxer.cpp
            source/options.cpp
            source/h264_decoder.cpp
            source/h264_encoder.cpp
            source/h264_transcoder.cpp
            source/jpeg_encoder.cpp
            source/locky.cpp
            source/utils.cpp
            source/av_packet.cpp
            source/av_packet_factory.cpp)

set(COMMON_LIBS cppkit avcodec avformat avutil swscale x264)

if(CMAKE_SYSTEM MATCHES "Linux-")
    include_directories(/usr/include/ffmpeg)
    add_definitions(-D__STDC_CONSTANT_MACROS)
    include_directories(/usr/include/ffmpeg)
endif(CMAKE_SYSTEM MATCHES "Linux-")

include(common.cmake NO_POLICY_SCOPE)
include("${devel_artifacts_path}/build/base_lib.cmake" NO_POLICY_SCOPE)
