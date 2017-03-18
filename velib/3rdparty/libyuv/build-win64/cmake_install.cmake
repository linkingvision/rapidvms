# Install script for directory: C:/work/60-video/ve/opencvr/velib/3rdparty/libyuv

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files/Project")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/work/60-video/ve/opencvr/velib/3rdparty/libyuv/build-win64/Debug/yuv.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/work/60-video/ve/opencvr/velib/3rdparty/libyuv/build-win64/Release/yuv.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/work/60-video/ve/opencvr/velib/3rdparty/libyuv/build-win64/MinSizeRel/yuv.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/work/60-video/ve/opencvr/velib/3rdparty/libyuv/build-win64/RelWithDebInfo/yuv.lib")
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libyuv" TYPE FILE FILES
    "C:/work/60-video/ve/opencvr/velib/3rdparty/libyuv/include/libyuv/basic_types.h"
    "C:/work/60-video/ve/opencvr/velib/3rdparty/libyuv/include/libyuv/compare.h"
    "C:/work/60-video/ve/opencvr/velib/3rdparty/libyuv/include/libyuv/convert.h"
    "C:/work/60-video/ve/opencvr/velib/3rdparty/libyuv/include/libyuv/convert_argb.h"
    "C:/work/60-video/ve/opencvr/velib/3rdparty/libyuv/include/libyuv/convert_from.h"
    "C:/work/60-video/ve/opencvr/velib/3rdparty/libyuv/include/libyuv/convert_from_argb.h"
    "C:/work/60-video/ve/opencvr/velib/3rdparty/libyuv/include/libyuv/cpu_id.h"
    "C:/work/60-video/ve/opencvr/velib/3rdparty/libyuv/include/libyuv/planar_functions.h"
    "C:/work/60-video/ve/opencvr/velib/3rdparty/libyuv/include/libyuv/rotate.h"
    "C:/work/60-video/ve/opencvr/velib/3rdparty/libyuv/include/libyuv/rotate_argb.h"
    "C:/work/60-video/ve/opencvr/velib/3rdparty/libyuv/include/libyuv/row.h"
    "C:/work/60-video/ve/opencvr/velib/3rdparty/libyuv/include/libyuv/scale.h"
    "C:/work/60-video/ve/opencvr/velib/3rdparty/libyuv/include/libyuv/scale_argb.h"
    "C:/work/60-video/ve/opencvr/velib/3rdparty/libyuv/include/libyuv/scale_row.h"
    "C:/work/60-video/ve/opencvr/velib/3rdparty/libyuv/include/libyuv/version.h"
    "C:/work/60-video/ve/opencvr/velib/3rdparty/libyuv/include/libyuv/video_common.h"
    "C:/work/60-video/ve/opencvr/velib/3rdparty/libyuv/include/libyuv/mjpeg_decoder.h"
    )
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES "C:/work/60-video/ve/opencvr/velib/3rdparty/libyuv/include/libyuv.h")
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

file(WRITE "C:/work/60-video/ve/opencvr/velib/3rdparty/libyuv/build-win64/${CMAKE_INSTALL_MANIFEST}" "")
foreach(file ${CMAKE_INSTALL_MANIFEST_FILES})
  file(APPEND "C:/work/60-video/ve/opencvr/velib/3rdparty/libyuv/build-win64/${CMAKE_INSTALL_MANIFEST}" "${file}\n")
endforeach()
