# Version info variables
set(PROTOBUF_VERSION        "3.0.0")
set(PROTOBUF_VERSION_STRING "3.0.0-beta-3")

# Current dir
get_filename_component(_PROTOBUF_PACKAGE_PREFIX
  "${CMAKE_CURRENT_LIST_FILE}" PATH)

# Imported targets
include("${_PROTOBUF_PACKAGE_PREFIX}/protobuf-targets.cmake")

# Compute the installation prefix relative to this file.
get_filename_component(_PROTOBUF_IMPORT_PREFIX
  "${_PROTOBUF_PACKAGE_PREFIX}" PATH)
get_filename_component(_PROTOBUF_IMPORT_PREFIX
  "${_PROTOBUF_IMPORT_PREFIX}" PATH)
get_filename_component(_PROTOBUF_IMPORT_PREFIX
  "${_PROTOBUF_IMPORT_PREFIX}" PATH)

# CMake FindProtobuf module compatible file
if(NOT DEFINED PROTOBUF_MODULE_COMPATIBLE OR "${PROTOBUF_MODULE_COMPATIBLE}")
  include("${_PROTOBUF_PACKAGE_PREFIX}/protobuf-module.cmake")
endif()

# Cleanup temporary variables.
set(_PROTOBUF_PACKAGE_PREFIX)
set(_PROTOBUF_IMPORT_PREFIX)
