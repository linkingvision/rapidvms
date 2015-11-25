#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "libprotobuf-lite" for configuration "Release"
set_property(TARGET libprotobuf-lite APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(libprotobuf-lite PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/libprotobuf-lite.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/libprotobuf-lite.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS libprotobuf-lite )
list(APPEND _IMPORT_CHECK_FILES_FOR_libprotobuf-lite "${_IMPORT_PREFIX}/lib/libprotobuf-lite.lib" "${_IMPORT_PREFIX}/bin/libprotobuf-lite.dll" )

# Import target "libprotobuf" for configuration "Release"
set_property(TARGET libprotobuf APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(libprotobuf PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/libprotobuf.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/libprotobuf.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS libprotobuf )
list(APPEND _IMPORT_CHECK_FILES_FOR_libprotobuf "${_IMPORT_PREFIX}/lib/libprotobuf.lib" "${_IMPORT_PREFIX}/bin/libprotobuf.dll" )

# Import target "libprotoc" for configuration "Release"
set_property(TARGET libprotoc APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(libprotoc PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/libprotoc.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/libprotoc.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS libprotoc )
list(APPEND _IMPORT_CHECK_FILES_FOR_libprotoc "${_IMPORT_PREFIX}/lib/libprotoc.lib" "${_IMPORT_PREFIX}/bin/libprotoc.dll" )

# Import target "protoc" for configuration "Release"
set_property(TARGET protoc APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(protoc PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/protoc.exe"
  )

list(APPEND _IMPORT_CHECK_TARGETS protoc )
list(APPEND _IMPORT_CHECK_FILES_FOR_protoc "${_IMPORT_PREFIX}/bin/protoc.exe" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
