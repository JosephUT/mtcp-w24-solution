cmake_minimum_required(VERSION 3.27)
project(mtcp)
set(CMAKE_CXX_STANDARD 20)

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
    include/messageTypes/pose.hpp
    include/messageTypes/twist.hpp
)

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
        controller.cpp
)

target_link_libraries(controller PUBLIC mtcp_lib)

add_executable(system
        src/mros_json.cpp
        system.cpp
)

target_link_libraries(system PUBLIC mtcp_lib)


# TESTS

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

