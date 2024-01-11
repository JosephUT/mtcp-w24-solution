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

add_library(mtcp_lib SHARED
        ${SOCKET_SOURCES}
)

target_include_directories(mtcp_lib PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include/mros>
)

add_executable(controller
        src/mros_json.cpp
        main.cpp
)

target_link_libraries(controller PUBLIC mtcp_lib)
