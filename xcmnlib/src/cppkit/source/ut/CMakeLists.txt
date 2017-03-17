cmake_minimum_required(VERSION 2.8)
project(ut)

set(SOURCES source/framework.cpp
            source/chrono_test.cpp
            source/ck_byte_ptr_test.cpp
            source/ck_large_files_test.cpp
            source/ck_memory_test.cpp
            source/ck_random_test.cpp
            source/ck_socket_address_test.cpp
            source/ck_socket_test.cpp
            source/ck_string_test.cpp
            source/condition_variable_test.cpp
            source/recursive_mutex_test.cpp
            source/ck_variant_test.cpp
            source/ck_json_test.cpp
            source/ck_nullable_test.cpp
            source/ck_dynamic_library_test.cpp
            source/ck_actor_test.cpp
            source/ck_command_queue_test.cpp
            source/ck_memory_map_test.cpp
            source/ck_path_test.cpp
            source/ck_udp_receiver_test.cpp
            source/ck_udp_sender_test.cpp
            source/ck_string_slice_test.cpp
            source/ck_timer_test.cpp
            source/ck_statistics_test.cpp
            source/ck_bitsy_test.cpp
            source/ck_pool_test.cpp)

set(COMMON_LIBS cppkit)
set(WINDOWS_LIBS DbgHelp ws2_32 iphlpapi)

include(../../devel_artifacts/build/base_ut.cmake)
