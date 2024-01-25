cmake_minimum_required(VERSION 3.25)
project(python_cpp_socket)
set(CMAKE_CXX_STANDARD 20)

add_library(message_socket
    STATIC
    socket/message_socket/client_message_socket.cpp
    socket/message_socket/connection_message_socket.cpp
    socket/message_socket/message_socket.cpp
    socket/client_socket.cpp
    socket/connection_socket.cpp
    socket/server_socket.cpp
    socket/socket.cpp
)
target_include_directories(message_socket PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_executable(test_python_cpp_socket test_python_cpp_socket.cpp)
target_link_libraries(test_python_cpp_socket message_socket)