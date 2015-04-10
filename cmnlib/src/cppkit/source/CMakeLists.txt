cmake_minimum_required(VERSION 2.8)
project(cppkit)

set(SOURCES source/ck_library_id.cpp
            source/ck_byte_ptr.cpp
            source/ck_error_msg.cpp
            source/ck_exception.cpp
            source/ck_large_files.cpp
            source/ck_logger.cpp
            source/ck_memory.cpp
            source/ck_socket_address.cpp
            source/ck_socket.cpp
            source/ck_sprintf.cpp
            source/ck_stack_trace.cpp
            source/ck_string.cpp
            source/ck_string_slice.cpp
            source/ck_time_utils.cpp
            source/ck_uuid_utils.cpp
            source/ck_variant.cpp
            source/ck_json.cpp
            source/ck_nullable.cpp
            source/ck_dynamic_library.cpp
            source/ck_actor.cpp
            source/ck_command_queue.cpp
            source/ck_memory_map.cpp
            source/ck_path.cpp
            source/ck_udp_sender.cpp
            source/ck_udp_receiver.cpp
            source/ck_timer.cpp
            source/ck_pool.cpp)

set(WINDOWS_LIBS DbgHelp ws2_32 iphlpapi)
set(LINUX_LIBS pthread rt dl)
#set(COMMON_LIBS "")

# XSDK is special and has to create ../devel_artifacts for everyone else.
file(MAKE_DIRECTORY ../devel_artifacts)

include(cmake/common.cmake NO_POLICY_SCOPE)
include(cmake/base_lib.cmake NO_POLICY_SCOPE)

install(FILES cmake/common.cmake DESTINATION "${devel_artifacts_path}/build")
install(FILES cmake/base_lib.cmake DESTINATION "${devel_artifacts_path}/build")
install(FILES cmake/base_ut.cmake DESTINATION "${devel_artifacts_path}/build")
install(FILES cmake/base_app.cmake DESTINATION "${devel_artifacts_path}/build")
