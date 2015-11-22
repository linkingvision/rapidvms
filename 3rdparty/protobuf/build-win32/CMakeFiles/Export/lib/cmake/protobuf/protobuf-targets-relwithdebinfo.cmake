#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "libprotobuf-lite" for configuration "RelWithDebInfo"
set_property(TARGET libprotobuf-lite APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(libprotobuf-lite PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELWITHDEBINFO "CXX"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/libprotobuf-lite.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS libprotobuf-lite )
list(APPEND _IMPORT_CHECK_FILES_FOR_libprotobuf-lite "${_IMPORT_PREFIX}/lib/libprotobuf-lite.lib" )

# Import target "libprotobuf" for configuration "RelWithDebInfo"
set_property(TARGET libprotobuf APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(libprotobuf PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELWITHDEBINFO "CXX"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/libprotobuf.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS libprotobuf )
list(APPEND _IMPORT_CHECK_FILES_FOR_libprotobuf "${_IMPORT_PREFIX}/lib/libprotobuf.lib" )

# Import target "libprotoc" for configuration "RelWithDebInfo"
set_property(TARGET libprotoc APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(libprotoc PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELWITHDEBINFO "CXX"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/libprotoc.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS libprotoc )
list(APPEND _IMPORT_CHECK_FILES_FOR_libprotoc "${_IMPORT_PREFIX}/lib/libprotoc.lib" )

# Import target "protoc" for configuration "RelWithDebInfo"
set_property(TARGET protoc APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(protoc PROPERTIES
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/bin/protoc.exe"
  )

list(APPEND _IMPORT_CHECK_TARGETS protoc )
list(APPEND _IMPORT_CHECK_FILES_FOR_protoc "${_IMPORT_PREFIX}/bin/protoc.exe" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
