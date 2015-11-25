#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "libprotobuf-lite" for configuration "Debug"
set_property(TARGET libprotobuf-lite APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(libprotobuf-lite PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/libprotobuf-lited.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/libprotobuf-lited.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS libprotobuf-lite )
list(APPEND _IMPORT_CHECK_FILES_FOR_libprotobuf-lite "${_IMPORT_PREFIX}/lib/libprotobuf-lited.lib" "${_IMPORT_PREFIX}/bin/libprotobuf-lited.dll" )

# Import target "libprotobuf" for configuration "Debug"
set_property(TARGET libprotobuf APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(libprotobuf PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/libprotobufd.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/libprotobufd.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS libprotobuf )
list(APPEND _IMPORT_CHECK_FILES_FOR_libprotobuf "${_IMPORT_PREFIX}/lib/libprotobufd.lib" "${_IMPORT_PREFIX}/bin/libprotobufd.dll" )

# Import target "libprotoc" for configuration "Debug"
set_property(TARGET libprotoc APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(libprotoc PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/libprotocd.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/libprotocd.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS libprotoc )
list(APPEND _IMPORT_CHECK_FILES_FOR_libprotoc "${_IMPORT_PREFIX}/lib/libprotocd.lib" "${_IMPORT_PREFIX}/bin/libprotocd.dll" )

# Import target "protoc" for configuration "Debug"
set_property(TARGET protoc APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(protoc PROPERTIES
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/protoc.exe"
  )

list(APPEND _IMPORT_CHECK_TARGETS protoc )
list(APPEND _IMPORT_CHECK_FILES_FOR_protoc "${_IMPORT_PREFIX}/bin/protoc.exe" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
