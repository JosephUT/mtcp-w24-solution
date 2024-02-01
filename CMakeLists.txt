cmake_minimum_required(VERSION 3.25)
project(mtcp)
set(CMAKE_CXX_STANDARD 20)

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

set(SOCKET_SOURCES
        src/socket/socket.cpp
        src/socket/server_socket.cpp
        src/socket/client_socket.cpp
        src/socket/connection_socket.cpp
        src/socket/message_socket/message_socket.cpp
        src/socket/message_socket/client_message_socket.cpp
        src/socket/message_socket/connection_message_socket.cpp
)

set(MESSAGE_TYPES
    include/messages/pose.hpp
    include/messages/twist.hpp
)

find_package(Threads REQUIRED)

add_library(mtcp_lib SHARED
        ${SOCKET_SOURCES}
        ${MESSAGE_TYPES}
)

target_include_directories(mtcp_lib PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include/mtcp>
)

# DRIVERS

add_executable(controller
        src/mros_json.cpp
        src/controller.cpp
)

target_link_libraries(controller PUBLIC mtcp_lib)

add_executable(system
        src/mros_json.cpp
        src/system.cpp
)

target_link_libraries(system PUBLIC mtcp_lib)


# Automated Tests
enable_testing()
add_executable(test_message_socket
        ${SOCKET_SOURCES}
        test/test_message_socket.cpp
)
target_include_directories(test_message_socket PRIVATE include)
target_link_libraries(test_message_socket GTest::gtest_main)
gtest_discover_tests(test_message_socket)

add_executable(test_pose test/test_pose.cpp src/mros_json.cpp)
target_include_directories(test_pose PRIVATE include)
target_link_libraries(test_pose GTest::gtest_main)
gtest_discover_tests(test_pose)

add_executable(test_twist test/test_twist.cpp src/mros_json.cpp)
target_include_directories(test_twist PRIVATE include)
target_link_libraries(test_twist GTest::gtest_main)
gtest_discover_tests(test_twist)


# Manual Tests
add_executable(test_json_reader
        src/mros_json.cpp
        test/json_reader.cpp
)
target_link_libraries(test_json_reader PUBLIC mtcp_lib)

add_executable(test_json_conversions
        src/mros_json.cpp
        test/struct_json.cpp
)
target_link_libraries(test_json_conversions PUBLIC mtcp_lib)

add_executable(test_bridge_and_simulator
        test_manual/test_bridge_and_simulator.cpp
        src/mros_json.cpp
)
target_include_directories(test_bridge_and_simulator PUBLIC include)
target_link_libraries(test_bridge_and_simulator PUBLIC mtcp_lib)